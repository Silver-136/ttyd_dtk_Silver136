#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_cmd.h"
#include "evt/evt_sub.h"
#include "battle/battle_status_effect.h"
#include "battle/unit_work.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "manager/evtmgr_cmd.h"

USER_FUNC(btlevtcmd_SetEventUnisonPhase);
static USER_FUNC(battleGetBarriernSatelliteInitPos);

BattleUnitKind unit_barriern_z_satellite;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitKindPart parts[6];
static StatusVulnerability regist;
static PoseTableEntry pose_table[17];
static BattleDataEntry data_table[1];
static EvtWord init_event[29];
static EvtWord damage_event[10];
static EvtWord phase_event[2];
static EvtWord unison_phase_event[5];
static EvtWord attack_event[5];
static EvtWord wait_event[56];

BattleUnitKind unit_barriern_z_satellite = {
    UNIT_MINI_Z_YUX, // mUnitType
    "btl_un_barriern_satellite_z", // name
    2, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    0, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    16, // width
    16, // height
    {0, 8}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {-3, -6}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {8.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {8.0f, 0.0f, 0.0f}, // bintaHitOffset
    {5.0f, 6.0f, 0.0f}, // kissHitOffset
    {0.0f, 0.0f, 0.0f}, // cutBaseOffset
    16.0f, // cutWidth
    16.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_BARRI_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating, // attributeFlags
    &regist, // statusVulnerability
    6, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_barriern_satellite_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 10.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
    {2, "btl_un_barriern_satellite_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 10.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {3, "btl_un_barriern_satellite_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 10.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_barriern_satellite_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 10.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {5, "btl_un_barriern_satellite_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 10.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {6, "btl_un_barriern_satellite_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 10.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

static StatusVulnerability regist = {
    70, // sleep
    90, // stop
    95, // dizzy
    0, // poison
    80, // confuse
    100, // electric
    100, // burn
    0, // freeze
    100, // huge
    85, // tiny
    100, // attackUp
    85, // attackDown
    100, // defenseUp
    90, // defenseDown
    80, // allergic
    0, // fright
    95, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    100, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_2"},
    {POSE_TYPE_STOP, "Y_2"},
    {POSE_TYPE_FREEZE, "Y_2"},
    {POSE_TYPE_CONFUSE, "K_2"},
    {POSE_TYPE_POISON, "X_2"},
    {POSE_TYPE_DIZZY, "X_2"},
    {POSE_TYPE_NORMAL, "S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_2"},
    {POSE_TYPE_DAMAGE, "D_2"},
    {POSE_TYPE_RUN, "R_2"},
    {POSE_TYPE_RUN_HOME, "R_2"},
    {POSE_TYPE_WALK, "W_2"},
    {POSE_TYPE_DANGER, "X_2"},
    {POSE_TYPE_PERIL, "X_2"},
    {POSE_TYPE_ACTION, "S_2"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventUnisonPhase, UNIT_ID_SELF, PTR(unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_FLOAT_CAPABLE | BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    LABEL(0)
    CALL(evt_sub_random, 120, LW(0))
    ADD(LW(0), 30)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, BARRIERN_SATELLITE_WORK_IDLE_POSE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_4"))
                WAIT_FRAMES(180)
                CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
            END_IF
        END_IF
    END_IF
    GOTO(0)
    RETURN
EVT_END()

static USER_FUNC(battleGetBarriernSatelliteInitPos) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 num;
    s32 index;
    s32 id;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 j;
    static const Vec VecData[] = {
        // 1 satellite
        {25.0f, 0.0f, 0.0f},
        // 2 satellites
        {25.0f, 0.0f, 0.0f},
        {-25.0f, 0.0f, 0.0f},
        // 3 satellites
        {0.0f, 25.0f, 0.0f},
        {25.0f, -25.0f, 0.0f},
        {-25.0f, -25.0f, 0.0f},
        // 4 satellites
        {25.0f, 25.0f, 0.0f},
        {-25.0f, 25.0f, 0.0f},
        {25.0f, -25.0f, 0.0f},
        {-25.0f, -25.0f, 0.0f},
    };
    const Vec* p;
    BattleWork* wp = _battleWorkPointer;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    num = evtGetValue(event, args[1]);
    index = evtGetValue(event, args[2]);
    x = args[3];
    y = args[4];
    z = args[5];
    p = VecData;
    for (i = 0; i < num; i++) {
        for (j = 0; j < i; j++) {
            p++;
        }
    }
    for (j = 0; j < index; j++) {
        p++;
    }
    evtSetValue(event, x, (s32)(p->x * unit->faceDirection));
    evtSetValue(event, y, (s32)p->y);
    evtSetValue(event, z, (s32)p->z);
    return EVT_RETURN_DONE;
}
