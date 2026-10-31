#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "las.h"
#include "battle/battle_status_effect.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"

static USER_FUNC(killer_get_angle);
static USER_FUNC(killer_copy_status);

BattleUnitKind unit_super_killer_cannon;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[3];
static BattleUnitSetup entry_killer;
static PoseTableEntry pose_table[16];
static BattleDataEntry data_table[1];
static EvtWord init_event[63];
static EvtWord damage_event[10];
static EvtWord attack_event[738];
static EvtWord friend_event[244];
static EvtWord friend_event_sky[318];
static EvtWord wait_event[6];

static USER_FUNC(killer_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = BattleTransID(event, evtGetValue(event, args[0]));
    s32 id2 = BattleTransID(event, evtGetValue(event, args[1]));
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    unit2->mSizeChangeTurns = unit1->mSizeChangeTurns;
    unit2->mSizeChangeStrength = unit1->mSizeChangeStrength;
    return EVT_RETURN_DONE;
}

/**
 * Unit data of the B. Bill Blaster: 10 HP, 52x38 units in size; ties together its sounds, parts,
 * status vulnerability, init event and event table.
 */
BattleUnitKind unit_super_killer_cannon = {
    UNIT_BOMBSHELL_BILL_BLASTER, // mUnitType
    "btl_un_super_killer_cannon", // name
    10, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    31, // mLevel
    0, // mBonusExp
    1, // bonusCoin
    70, // bonusCoinRate
    3, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    52, // width
    38, // height
    {5, 38}, // hitOffset
    {0.0f, 19.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {26.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    20.0f, // vivianOffsetX
    10.0f, // vivianOffsetZ
    {26.0f, 0.0f, 0.0f}, // bintaHitOffset
    {26.0f, 24.7f, 0.0f}, // kissHitOffset
    {-10.0f, 19.0f, 0.0f}, // cutBaseOffset
    52.0f, // cutWidth
    38.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_KILLER_CANON_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    3, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of the B. Bill Blaster: 4 against every kind of hit. */
static BattleUnitDefense defence = {{4, 4, 4, 4, 4}};

/** Defence attributes of the B. Bill Blaster: none, every element hits it normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    0, // sleep
    0, // stop
    0, // dizzy
    0, // poison
    0, // confuse
    150, // electric
    100, // burn
    90, // freeze
    100, // huge
    80, // tiny
    100, // attackUp
    80, // attackDown
    100, // defenseUp
    95, // defenseDown
    40, // allergic
    0, // fright
    5, // galeForce
    100, // fast
    40, // slow
    100, // dodgy
    100, // invisible
    90, // ohko
};

/** Parts of the B. Bill Blaster: 3 parts (model c_killtai_g). */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_super_killer_cannon", "c_killtai_g", 0.0f, 0.0f, 0.0f, 0.0f, 30.0f, 0.0f, 0.0f, 35.0f, 0.0f, 35, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
    {2, "btl_un_super_killer_cannon", "c_killtai_g", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 255, &defence, &defence_attr, kNotTargetable, 0, pose_table},
    {3, "btl_un_super_killer_cannon", "c_killtai_g", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 255, &defence, &defence_attr, kNotTargetable, 0, pose_table},
};

/**
 * Spawn setup for the Bombshell Bill (unit_super_killer): enemy side, placed off-screen at first;
 * used by friend_event.
 */
static BattleUnitSetup entry_killer = {
    &unit_super_killer, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "KTA_N_1"},
    {POSE_TYPE_STOP, "KTA_Y_1"},
    {POSE_TYPE_FREEZE, "KTA_Y_1"},
    {POSE_TYPE_CONFUSE, "KTA_K_1"},
    {POSE_TYPE_POISON, "KTA_X_1"},
    {POSE_TYPE_DIZZY, "KTA_X_1"},
    {POSE_TYPE_NORMAL, "KTA_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KTA_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KTA_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KTA_A_1"},
    {POSE_TYPE_DAMAGE, "KTA_D_1"},
    {POSE_TYPE_DANGER, "KTA_X_1"},
    {POSE_TYPE_PERIL, "KTA_X_1"},
    {POSE_TYPE_ATTACK, "KTA_A_1"},
    {POSE_TYPE_TALK, "KTA_T_1"},
    {POSE_TYPE_ACTION, "KTA_S_1"},
};

/** Event table of the B. Bill Blaster: it has no special event scripts. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/**
 * Init event of the Bombshell Bill Blaster: installs its wait, attack, damage and confusion events,
 * shifts it 20 units right, sets the poses of the two upper barrel parts (KTA_Z_3 and KTA_Z_2)
 * and hides them: part attributes 0x1000000 (not drawn) and 0x2000000 (no shadow).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 20, 0, 0)
    CALL(btlevtcmd_AddHomePos, UNIT_ID_SELF, 20, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_Z_3"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KTA_Z_2"))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kNoShadow)
    RETURN
EVT_END()

/** Damage event of the Bombshell Bill Blaster: default damage handling on its body part. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Attack event of the Bombshell Bill Blaster: it never attacks directly,
 * it fires a Bombshell Bill into free space beside it.
 * It can use an item; otherwise it draws a number and picks one of the weighted launches:
 * from the front barrel (friend_event), or from the upper barrel (friend_event_sky) after raising it;
 * LW(12) = 1 selects the charged shot with extra charging flashes.
 * If no free space is found on either side nothing is launched.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(0), 15)
    ADD(LW(0), 0)
    ADD(LW(0), 10)
    ADD(LW(0), 20)
    ADD(LW(0), 0)
    ADD(LW(0), 10)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 15)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(PTR(friend_event))
                    GOTO(99)
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        GOTO(99)
    END_IF
    ADD(LW(0), 0)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 1)
                    CALL_CHILD(PTR(friend_event))
                    GOTO(99)
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        GOTO(99)
    END_IF
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(PTR(friend_event))
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        GOTO(99)
    END_IF
    ADD(LW(0), 20)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, 0)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(PTR(friend_event_sky))
                    CALL(btlevtcmd_GetPartsRotate, UNIT_ID_SELF, 2, LW(5), LW(6), LW(11))
                    LOOP(0)
                        IF_INT_GREATER_OR_EQUAL(LW(11), 0)
                            BREAK_LOOP
                        END_IF
                        ADD(LW(11), 2)
                        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(11))
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHidden)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
                    GOTO(99)
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        GOTO(99)
    END_IF
    ADD(LW(0), 0)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, 0)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 1)
                    CALL_CHILD(PTR(friend_event_sky))
                    CALL(btlevtcmd_GetPartsRotate, UNIT_ID_SELF, 2, LW(5), LW(6), LW(11))
                    LOOP(0)
                        IF_INT_GREATER_OR_EQUAL(LW(11), 0)
                            BREAK_LOOP
                        END_IF
                        ADD(LW(11), 2)
                        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(11))
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHidden)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
                    GOTO(99)
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        GOTO(99)
    END_IF
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, 0)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(PTR(friend_event_sky))
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(btlevtcmd_GetPartsRotate, UNIT_ID_SELF, 2, LW(5), LW(6), LW(11))
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(11), 0)
                BREAK_LOOP
            END_IF
            ADD(LW(11), 2)
            CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(11))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Launches a Bombshell Bill (entry_killer) from the front barrel:
 * the barrel animation and cannon sound play, a smoke puff appears at the muzzle (bigger and higher when the
 * cannon
 * is Huge, status 10, smaller when Tiny, status 11), the camera shakes,
 * the new unit copies the cannon's status (killer_copy_status),
 * flies to the free spot found by the caller and takes that as its home.
 * With LW(12) = 1 it charges first and makes the new Bill Charged (status 16).
 */
static EVT_BEGIN(friend_event)
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KTA_A_2A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KTA_Z_1"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KTA_A_2A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KTA_Z_1"))
        WAIT_FRAMES(10)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KTA_A_1"))
    WAIT_FRAMES(40)
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 70)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 50, LW(9), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 28)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 15, LW(9), FLOAT(0.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 40)
    ADD(LW(9), 10)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 30, LW(9), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    GOTO(10)
    LABEL(10)
    CALL(evt_btl_camera_shake_w, 0, 5, 0, 20, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_killer), 0)
    CALL(killer_copy_status, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_SetPos, LW(3), LW(7), 10, LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(3), 0)
    CALL(btlevtcmd_SetJumpSpeed, LW(3), 0)
    CALL(btlevtcmd_JumpPosition, LW(3), LW(0), 10, LW(2), FLOAT(7.0f), MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), 10, LW(2))
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_OnOffStatus, LW(3), STATUS_CHARGE, 1, 1, 1)
    END_IF
    RETURN
EVT_END()

/**
 * Launches a Bombshell Bill (entry_killer) from the raised upper barrel (part 2):
 * the barrel turns to aim at the free spot (killer_get_angle),
 * then it fires like friend_event with a slightly different smoke and sound,
 * and the new Bill flies with the barrel's angle at height 40 and gets part attributes 0x400000
 * and 0x200000 on its body (shell-toss-like and hammer-like moves cannot target it).
 */
static EVT_BEGIN(friend_event_sky)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(evt_sub_get_dir, LW(7), LW(8), LW(0), 40, LW(10))
    CALL(killer_get_angle, LW(7), LW(8), LW(0), 40, LW(10))
    CALL(btlevtcmd_GetPartsRotate, UNIT_ID_SELF, 2, LW(5), LW(6), LW(11))
    LOOP(0)
        IF_INT_GREATER(LW(11), 0)
            BREAK_LOOP
        END_IF
        IF_INT_LESS_OR_EQUAL(LW(11), LW(10))
            BREAK_LOOP
        END_IF
        SUBTRACT(LW(11), 2)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(11))
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_A_3A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_Z_3"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_A_3A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_Z_3"))
        WAIT_FRAMES(10)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_A_3"))
    WAIT_FRAMES(40)
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 60)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 75, LW(9), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 28)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 20, LW(9), FLOAT(0.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 40)
    ADD(LW(9), 10)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 38, LW(9), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    GOTO(10)
    LABEL(10)
    CALL(evt_btl_camera_shake_w, 0, 5, 0, 20, 0)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_killer), 0)
    CALL(killer_copy_status, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_SetPos, LW(3), LW(7), 10, LW(2))
    CALL(btlevtcmd_SetPartsRotate, LW(3), LW(4), 0, 0, LW(10))
    CALL(btlevtcmd_SetFallAccel, LW(3), 0)
    CALL(btlevtcmd_SetJumpSpeed, LW(3), 0)
    CALL(btlevtcmd_JumpPosition, LW(3), LW(0), 40, LW(2), FLOAT(7.0f), MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetPartsRotate, LW(3), LW(4), 0, 0, 0)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), 40, LW(2))
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_OnOffStatus, LW(3), STATUS_CHARGE, 1, 1, 1)
    END_IF
    CALL(btlevtcmd_OnPartsAttribute, LW(3), LW(4), kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OnPartsAttribute, LW(3), LW(4), kHammerlikeCannotTarget)
    RETURN
EVT_END()

double atan(double x);

/**
 * Script helper: returns in args[4] the angle in degrees of the line from (args[0], args[1]) to (args[2],
 * args[3]) as atan of its slope, used to aim the Bombshell Bill.
 */
static USER_FUNC(killer_get_angle) {
    EvtWord* args = event->args;
    f32 x1 = evtGetFloat(event, args[0]);
    f32 y1 = evtGetFloat(event, args[1]);
    f32 x2 = evtGetFloat(event, args[2]);
    f32 y2 = evtGetFloat(event, args[3]);
    f32 angle = atan((y2 - y1) / (x2 - x1));

    evtSetFloat(event, args[4], 360.0f * angle / 6.283184f);
    return EVT_RETURN_DONE;
}

/** Wait event of the Bombshell Bill Blaster: plays its idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

