#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "driver/animdrv.h"
#include "driver/mapdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_pouch.h"
#include <dolphin/gx.h>
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_attack_audience.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "data_tables.h"
#include "evt/evt_map.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "unit/unit_mario.h"
#include "battle/unit_work.h"

USER_FUNC(_get_local_frame);
static USER_FUNC(_mario_makkuro_set);
USER_FUNC(_mario_super_emblem_anim_set);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetJumpSound);
static USER_FUNC(_get_mario_hammer_lv);
static void callback(MapTevCallbackInfo* info);
s32 mario_pinch_pose_sound_callback(BattleUnit* unit);

PoseTableEntry pose_table_mario_stay[34];
BattleDataEntry data_table_mario[10];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
PoseTableEntry pose_table_object[3];
BattleUnitKindPart unitpartsdata_Mario[3];
BattleUnitKind unitdata_Mario;
static EvtWord battle_entry_event[73];
EvtWord mario_ride_btl_entry_event[120];
static EvtWord init_event[75];
#ifdef VERSION_G8ME01
EvtWord mario_dead_event[67];
#else
EvtWord mario_dead_event[57];
#endif
EvtWord mario_emergency_revival_event[130];
EvtWord mario_appeal_event[30];
static EvtWord attack_audience_event[15];
EvtWord btlataudevtMarioJump[237];
EvtWord btlataudevtMarioHammer[356];
static PoseSoundTiming pose_sound_timing_table[2];
static EvtWord damage_event[10];
static EvtWord attack_event[2];
static EvtWord wait_event[6];
EvtWord mario_win_reaction[9];

PoseTableEntry pose_table_mario_stay[] = {
    {POSE_TYPE_POISON, "M_S_3"},
    {POSE_TYPE_SLEEP, "M_N_5"},
    {POSE_TYPE_STOP, "M_Z_10"},
    {POSE_TYPE_FREEZE, "M_Z_10"},
    {POSE_TYPE_CONFUSE, "M_X_1"},
    {POSE_TYPE_DIZZY, "M_X_1"},
    {POSE_TYPE_KO, "M_Z_11"},
    {POSE_TYPE_NORMAL, "M_S_1"},
    {POSE_TYPE_DAMAGE_STATUS, "M_V_2"},
    {POSE_TYPE_DAMAGE_DRAINED, "M_B_2"},
    {POSE_TYPE_DAMAGE_TOP_SPIKY, "M_D_6"},
    {POSE_TYPE_DAMAGE_GUARDED, "M_B_1"},
    {32, "M_V_1"},
    {POSE_TYPE_DAMAGE_SURPRISED, "M_N_7"},
    {POSE_TYPE_DAMAGE, "M_D_1"},
    {POSE_TYPE_NEUTRAL, "M_S_1"},
    {POSE_TYPE_WALK, "M_W_1"},
    {POSE_TYPE_RUN, "M_R_1"},
    {POSE_TYPE_JUMP, "M_J_1B"},
    {POSE_TYPE_ITEM, "M_I_1"},
    {POSE_TYPE_PERIL, "M_S_3"},
    {POSE_TYPE_DANGER, "M_S_3"},
    {POSE_TYPE_HAPPY_HEART, "M_V_2"},
    {POSE_TYPE_HAPPY_FLOWER, "M_V_2"},
    {POSE_TYPE_KISSED_CHRISTINE, "M_A_3B"},
    {POSE_TYPE_KISSED_CHUCHURINA, "M_A_3B"},
    {POSE_TYPE_ESCAPE_STUMBLE, "M_D_2"},
    {POSE_TYPE_ESCAPE_RECOVER, "M_V_1"},
    {POSE_TYPE_COMMAND, "M_C_3"},
    {POSE_TYPE_APPEAL, "M_N_8"},
    {POSE_TYPE_ZERO_GRAVITY, "M_I_O"},
    {POSE_TYPE_AVOID_COUNTER, "M_J_1B"},
    {POSE_TYPE_DEFEND, "M_B_2"},
    {POSE_TYPE_ACTION, "M_S_1"},
};

BattleDataEntry data_table_mario[] = {
    {UNIT_DATA_APPEAL_EVENT, mario_appeal_event},
    {UNIT_DATA_ATTACK_AUDIENCE, attack_audience_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {UNIT_DATA_EMERGENCY_REVIVAL_EVENT, mario_emergency_revival_event},
    {UNIT_DATA_DEAD_EVENT, mario_dead_event},
    {UNIT_DATA_WIN_REACTION_EVENT, mario_win_reaction},
    {UNIT_DATA_ATTACK_READY_EVENT, subset_attack_ready_jump_event},
    {UNIT_DATA_RIDE_ENTRY_EVENT, mario_ride_btl_entry_event},
    {UNIT_DATA_AVOID_COUNTER_EVENT, btldefaultevt_avoid_counter_event},
    {0, NULL},
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    100, // sleep
    100, // stop
    100, // dizzy
    100, // poison
    100, // confuse
    100, // electric
    100, // burn
    100, // freeze
    100, // huge
    100, // tiny
    100, // attackUp
    100, // attackDown
    100, // defenseUp
    100, // defenseDown
    100, // allergic
    100, // fright
    100, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    100, // ohko
};

PoseTableEntry pose_table_object[] = {
    {POSE_TYPE_NORMAL, "KRN_H_1"},
    {POSE_TYPE_DAMAGE, "KRN_H_1"},
    {POSE_TYPE_ACTION, "KRN_H_1"},
};

BattleUnitKindPart unitpartsdata_Mario[] = {
    {1, "btl_un_mario", "b_mario", 0.0f, 0.0f, 0.0f, 16.0f, 36.0f, 5.0f, 0.0f, 40.0f, 0.0f, 20, 36, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table_mario_stay},
    {2, "btl_un_mario", "b_mario", 0.0f, 0.0f, 0.0f, 16.0f, 36.0f, 5.0f, 0.0f, 40.0f, 0.0f, 20, 36, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_object},
    {3, "btl_un_mario", "b_mario", 0.0f, 0.0f, 0.0f, 16.0f, 36.0f, 5.0f, 0.0f, 40.0f, 0.0f, 20, 36, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_object},
};

BattleUnitKind unitdata_Mario = {
    UNIT_MARIO, // mUnitType
    "btl_un_mario", // name
    10, // maxHp
    5, // maxFp
    5, // mDangerHP
    1, // mPerilHP
    1, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    0, // bonusCoinRate
    0, // baseCoin
    100, // runRate
    9, // pbSoftCap
    22, // width
    36, // height
    {11, 36}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {0.0f, 40.0f, 0.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {11.0f, 0.0f, 0.0f}, // bintaHitOffset
    {11.0f, 23.4f, 0.0f}, // kissHitOffset
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
    "SFX_MARIO_DAMAGE1", // damageSfxName
    "SFX_MARIO_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_MARIO_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_MARIO_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitLeader | kUnitNoHpGauge, // attributeFlags
    &regist, // statusVulnerability
    3, // numParts
    unitpartsdata_Mario, // mParts
    init_event, // initEventCode
    data_table_mario, // dataTable
};

static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_map_replayanim, 0, STRING("dontyo"))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 61, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(0)
    CALL(_get_local_frame, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(50)
    END_IF
    IF_INT_EQUAL(LW(0), 10)
        GOTO(50)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    LABEL(50)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_S_1"))
    RETURN
EVT_END()

EVT_BEGIN(mario_ride_btl_entry_event)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_map_replayanim, 0, STRING("dontyo"))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_PARTNER, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 50, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_JUMP)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 24, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(0)
    CALL(_get_local_frame, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(50)
    END_IF
    IF_INT_EQUAL(LW(0), 10)
        GOTO(50)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    LABEL(50)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_MARIO_BATTLE_JUMP1"), STRING("SFX_MARIO_JUMP_LANDING1"))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_MARIO_BATTLE_MOVE1L"), STRING("SFX_MARIO_BATTLE_MOVE1R"), 0, 20, 20)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_MARIO_BATTLE_MOVE1L"), STRING("SFX_MARIO_BATTLE_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, MARIO_PARTS_WORK_FIRE_BALL, 0)
    CALL(_mario_super_emblem_anim_set, UNIT_ID_SELF)
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_10_02_TRADING_PLACES)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG4_15_02_BACK_TO_NORMAL)
            CALL(_mario_makkuro_set)
        END_IF
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

USER_FUNC(_get_local_frame) {
    EvtWord* args = event->args;
    s32 index = args[0];
    BattleUnitPart* part;
    AnimationPose* pose;

    part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 1);
    pose = animPoseGetAnimPosePtr(part->poseId);
    evtSetValue(event, index, pose->localFrame);
    return EVT_RETURN_DONE;
}

s32 mario_pinch_pose_sound_callback(BattleUnit* unit) {
    s32 ret;

    if (unit->work[MARIO_WORK_PINCH_VOICE] & 1) {
        ret = BtlUnit_snd_se(unit, "SFX_VOICE_MARIO_RELIEF2", EVT_NULLPTR, 0);
    } else {
        ret = BtlUnit_snd_se(unit, "SFX_VOICE_MARIO_RELIEF1", EVT_NULLPTR, 0);
    }
    unit->work[MARIO_WORK_PINCH_VOICE]++;
    return ret;
}

static void callback(MapTevCallbackInfo* info) {
    GXColor color = {0x4D, 0x09, 0x52, 0xFF};
    s32 tevStage = info->tevStage;
    s32 texMap = info->texMap;
    s32 texCoord = info->texCoord;
    s32 indStage = info->indStage;

    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
    info->tevStage = tevStage + 1;
    info->texCoord = texCoord;
    info->indStage = indStage;
    info->texMap = texMap;
}

void faker_mario_makkuro_set(BattleUnit* unit) {
    mapSetTevCallback(11, callback);
}

static USER_FUNC(_mario_makkuro_set) {
    BattleUnit* unit = BattleGetMarioPtr(_battleWorkPointer);

    if (unit) {
        mapSetTevCallback(11, callback);
        animPoseSetMaterialFlagOn(BtlUnit_GetPartsPtr(unit, 1)->poseId, MAP_OBJ_TEV_CALLBACK(11));
        animPoseSetMaterialFlagOn(BtlUnit_GetPartsPtr(unit, 2)->poseId, MAP_OBJ_TEV_CALLBACK(11));
        animPoseSetMaterialFlagOn(BtlUnit_GetPartsPtr(unit, 3)->poseId, MAP_OBJ_TEV_CALLBACK(11));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_mario_super_emblem_anim_set) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnitPart* part;
    const char* animName;
    s32 id, type;

    id = evtGetValue(event, *args++);
    id = BattleTransID(event, id);
    BattleGetUnitPtr(wp, id);
    type = 0;
    if (pouchEquipCheckBadge(ITEM_L_EMBLEM)) {
        type = 1;
    }
    if (pouchEquipCheckBadge(ITEM_W_EMBLEM)) {
        type += 2;
    }
    switch (type) {
        case 1:
            animName = "b_mario_l";
            break;
        case 2:
            animName = "b_mario_w";
            break;
        case 3:
            animName = "b_mario_wl";
            break;
        default:
            animName = NULL;
            break;
    }
    if (isFirstCall) {
        if (!animName) {
            return EVT_RETURN_DONE;
        }
        part = BattleGetUnitPartsPtr(id, 1);
        animPoseGetAnimPosePtr(part->poseId);
        if (part->poseId != -1) {
            animPoseRelease(part->poseId);
            part->poseId = -1;
        }
        part = BattleGetUnitPartsPtr(id, 2);
        animPoseGetAnimPosePtr(part->poseId);
        if (part->poseId != -1) {
            animPoseRelease(part->poseId);
            part->poseId = -1;
        }
        part = BattleGetUnitPartsPtr(id, 3);
        animPoseGetAnimPosePtr(part->poseId);
        if (part->poseId != -1) {
            animPoseRelease(part->poseId);
            part->poseId = -1;
        }
    }
    if (animEffectAsync(animName, 1)) {
        part = BattleGetUnitPartsPtr(id, 1);
        part->poseId = animPoseEntry(animName, 1);
        part = BattleGetUnitPartsPtr(id, 2);
        part->poseId = animPoseEntry(animName, 1);
        part = BattleGetUnitPartsPtr(id, 3);
        part->poseId = animPoseEntry(animName, 1);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(_get_mario_hammer_lv) {
    s32 index = event->args[0];

    evtSetValue(event, index, pouchGetPtr()->hammerLevel);
    return EVT_RETURN_DONE;
}

#ifdef VERSION_G8ME01
#endif
EVT_BEGIN(mario_dead_event)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
#ifdef VERSION_G8ME01
    CALL(evt_btl_camera_nomove_y_onoff, 1, 0)
#endif
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 1, LW(10), -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 1, 2)
#ifdef VERSION_G8ME01
    CALL(evt_btl_camera_set_posoffset, 1, 0, 0, 0)
#endif
    CALL(evt_btl_camera_set_zoom, 1, 500)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("M_I_S"))
    WAIT_MS(2066)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_CONDITION_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("M_D_2"))
    WAIT_MS(2000)
    CALL(btlevtcmd_KillUnit, LW(10), 1)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    RETURN
EVT_END()

EVT_BEGIN(mario_emergency_revival_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("M_I_S"))
    WAIT_MS(2066)
    CALL(btlevtcmd_ClearAllStatus, LW(10))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_CONDITION_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("M_D_2"))
    WAIT_MS(1000)
    CALL(btlevtcmd_ConsumeItemReserve, LW(10), 151)
    WAIT_MS(1000)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        CALL(btlevtcmd_DispItemIcon, LW(0), LW(1), LW(2), 151, 120)
    END_SIBLING_EVENT
    WAIT_FRAMES(120)
    CALL(btlevtcmd_OnOffStatus, LW(10), STATUS_INSTANT_KILL, 0, 0, 0)
    CALL(btlevtcmd_SetHp, LW(10), 0)
    CALL(btlevtcmd_RecoverHp, LW(10), LW(11), 10)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(2000)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_JUMP)
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(mario_appeal_event)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_APPEAL)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_APPEAL1"), LW(0), LW(1), LW(2), 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AudienceAppeal, UNIT_ID_SELF)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(attack_audience_event)
    CALL(evt_sub_random, 1, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(btlataudevtMarioJump))
        ELSE
        CALL_CHILD(PTR(btlataudevtMarioHammer))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(btlataudevtMarioJump)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(15)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_AUDIENCE)
        CALL(evt_btl_camera_set_homing_unit_audience, 0, UNIT_ID_SELF, 1, LW(3))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 200)
    END_SIBLING_EVENT
    SET_VALUE(LW(5), LW(1))
    ADD(LW(1), 28)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 5)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6))
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
    WAIT_FRAMES(3)
    CALL(_attack_aud, LW(3), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    SET_VALUE(LW(1), LW(5))
    ADD(LW(2), 5)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_MS(1000)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(8.0f))
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_GetStageAudienceAreaSize, LW(4), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), LW(4))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_GetStageSize, LW(4), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), LW(4))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(btlataudevtMarioHammer)
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
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("M_H_5A"))
        SET_VALUE(LW(1), STRING("M_H_5B"))
        SET_VALUE(LW(2), STRING("M_H_14"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("M_H_9A"))
        SET_VALUE(LW(1), STRING("M_H_9B"))
        SET_VALUE(LW(2), STRING("M_H_15"))
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("M_H_1A"))
        SET_VALUE(LW(1), STRING("M_H_1B"))
        SET_VALUE(LW(2), STRING("M_H_13"))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(1))
    WAIT_FRAMES(1)
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    WAIT_FRAMES(7)
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_HIT_GROUND3"), 0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_HIT_GROUND2"), 0)
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_HIT_GROUND1"), 0)
    END_SWITCH
    CALL(_attack_aud, LW(3), 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_MS(1000)
    END_SIBLING_EVENT
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_H_16"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(8.0f))
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_GetStageAudienceAreaSize, LW(4), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), LW(4))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(8), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static PoseSoundTiming pose_sound_timing_table[] = {
    {"M_S_3", 0.25f, mario_pinch_pose_sound_callback, NULL, 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

EVT_BEGIN(mario_win_reaction)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_V_2"))
    WAIT_MS(1000)
    RETURN
EVT_END()

