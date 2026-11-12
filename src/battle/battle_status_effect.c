// Battle status effects: the visual effects of statuses on units and the status-change announcement.

#include "version.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_unit.h"
#include "battle/battle.h"
#include "driver/windowdrv.h"
#include "manager/fontmgr.h"
#include "battle/battle_pad.h"
#include "driver/dispdrv.h"
#include "driver/msgdrv.h"
#include "battle/battle_sub.h"
#include "icon_ids.h"
#include <stdio.h>
#include <string.h>
#include "driver/icondrv.h"
#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include "battle/battle_damage.h"
#include "eff/eff_sleep.h"
#include "eff/eff_fire.h"
#include "eff/eff_ice.h"
#include "eff/eff_spark.h"

BOOL BattleStatusChangeAnnouceMain_Unit(BattleUnit* unit);
static void _st_chg_msg_disp(void);

typedef struct StChgMsgData {
    s8 type;
    const char* name;
    const char* msg1;
    const char* msg0;
} StChgMsgData;

#if VERSION <= VERSION_DEMO_JP
static s8 _st_pri_data[] = {
    STATUS_CHARGE, // charge is elevated
    STATUS_POISON, // poison is elevated
    STATUS_SLEEP,
    STATUS_STOP,
    STATUS_DIZZY,
    STATUS_CONFUSE,
    STATUS_ELECTRIC,
    STATUS_DODGY,
    STATUS_BURN,
    STATUS_FREEZE,
    STATUS_HUGE,
    STATUS_TINY,
    STATUS_ATTACK_UP,
    STATUS_ATTACK_DOWN,
    STATUS_DEFENSE_UP,
    STATUS_DEFENSE_DOWN,
    STATUS_ALLERGIC,
    STATUS_INVISIBLE,
    STATUS_FAST,
    STATUS_SLOW,
    STATUS_PAYBACK,
    STATUS_HOLD_FAST,
    STATUS_HP_REGEN,
    STATUS_FP_REGEN,
    -1,
};
#endif

/**
 * Message for each status change announcement (the Japanese status name is only a label):
 * msg1 when the change is marked changed, msg0 otherwise; only Charge differs ("...But it had no effect!").
 * Ends with type -1.
 */
static StChgMsgData _st_chg_msg_data[] = {
    { STATUS_POISON, "\202\307\202\255", "msg_st_chg_poison", "msg_st_chg_poison" },
    { STATUS_CHARGE, "\203`\203\203\201[\203W", "msg_st_chg_charge", "msg_st_chg_charge3" },
    { STATUS_SLEEP, "\202\313\202\336\202\350", "msg_st_chg_sleep", "msg_st_chg_sleep" },
    { STATUS_STOP, "\203X\203g\203b\203v", "msg_st_chg_stan", "msg_st_chg_stan" },
    { STATUS_DIZZY, "\203O\203\213\203O\203\213", "msg_st_chg_guruguru", "msg_st_chg_guruguru" },
    { STATUS_CONFUSE, "\202\261\202\361\202\347\202\361", "msg_st_chg_confusion", "msg_st_chg_confusion" },
    { STATUS_ELECTRIC, "\203r\203\212\203r\203\212", "msg_st_chg_biribiri", "msg_st_chg_biribiri" },
    { STATUS_DODGY, "\203\210\203P\203\210\203P", "msg_st_chg_dodge", "msg_st_chg_dodge" },
    { STATUS_BURN, "\202\320\202\276\202\351\202\334", "msg_st_chg_fire", "msg_st_chg_fire" },
    { STATUS_FREEZE, "\202\261\202\250\202\350\202\303\202\257", "msg_st_chg_freeze", "msg_st_chg_freeze" },
    { STATUS_HUGE, "\203f\203J\203f\203J", "msg_st_chg_big", "msg_st_chg_big" },
    { STATUS_TINY, "\203~\203j\203~\203j", "msg_st_chg_small", "msg_st_chg_small" },
    { STATUS_ATTACK_UP, "\202\302\202\346\202\302\202\346", "msg_st_chg_tuyotuyo", "msg_st_chg_tuyotuyo" },
    { STATUS_ATTACK_DOWN, "\202\346\202\355\202\346\202\355", "msg_st_chg_yowayowa", "msg_st_chg_yowayowa" },
    { STATUS_DEFENSE_UP, "\203J\203`\203J\203`", "msg_st_chg_hard", "msg_st_chg_hard" },
    { STATUS_DEFENSE_DOWN, "\203t\203j\203\203\203t\203j\203\203", "msg_st_chg_soft", "msg_st_chg_soft" },
    { STATUS_ALLERGIC, "\203A\203\214\203\213\203M\201[", "msg_st_chg_allergy", "msg_st_chg_allergy" },
    { STATUS_INVISIBLE, "\202\306\202\244\202\337\202\242", "msg_st_chg_trans", "msg_st_chg_trans" },
    { STATUS_FAST, "\202\315\202\342\202\242", "msg_st_chg_quick", "msg_st_chg_quick" },
    { STATUS_SLOW, "\202\314\202\353\202\242", "msg_st_chg_slow", "msg_st_chg_slow" },
    { STATUS_PAYBACK, "\202\265\202\251\202\246\202\265", "msg_st_chg_counter", "msg_st_chg_counter" },
    { STATUS_HOLD_FAST, "\203{\203\200\203J\203E\203\223\203^\201[", "msg_st_chg_bombcounter", "msg_st_chg_bombcounter" },
    { STATUS_HP_REGEN, "\202\266\202\355\202\266\202\355\202g\202o", "msg_st_chg_reg_hp", "msg_st_chg_reg_hp" },
    { STATUS_FP_REGEN, "\202\266\202\355\202\266\202\355\202e\202o", "msg_st_chg_reg_fp", "msg_st_chg_reg_fp" },
    { -1, "", "", "" },
};

//.sdata
// Status Type Priority Data
#if VERSION >= VERSION_JP
static s8 _st_pri_data[] = {
    STATUS_CHARGE, // charge is elevated
    STATUS_POISON, // poison is elevated
    STATUS_SLEEP,
    STATUS_STOP,
    STATUS_DIZZY,
    STATUS_CONFUSE,
    STATUS_ELECTRIC,
    STATUS_DODGY,
    STATUS_BURN,
    STATUS_FREEZE,
    STATUS_HUGE,
    STATUS_TINY,
    STATUS_ATTACK_UP,
    STATUS_ATTACK_DOWN,
    STATUS_DEFENSE_UP,
    STATUS_DEFENSE_DOWN,
    STATUS_ALLERGIC,
    STATUS_INVISIBLE,
    STATUS_FAST,
    STATUS_SLOW,
    STATUS_PAYBACK,
    STATUS_HOLD_FAST,
    STATUS_HP_REGEN,
    STATUS_FP_REGEN,
    -1,
};
#endif

// local prototypes
static void BSE_Sleep(BattleUnit* unit);
static void BSE_SleepDelete(BattleUnit* unit);
static void BSE_Biribiri(BattleUnit* unit);
static void BSE_BiribiriDelete(BattleUnit* unit);
static void BSE_Fire(BattleUnit* unit);
static void BSE_FireDelete(BattleUnit* unit);
static void BSE_Freeze(BattleUnit* unit);
static void BSE_FreezeDelete(BattleUnit* unit);
static void BSE_Kagegakure(BattleUnit* unit);
static void BSE_KagegakureDelete(BattleUnit* unit);

/**
 * Status-effect setup for a new unit, called from BtlUnit_Entry;
 * empty in every version (the effect pointers start NULL).
 */
void BattleStatusEffectInit(BattleUnit* unit) {
    ; // empty in every version
}

/**
 * Runs every frame for each unit from battle_disp.c:
 * creates, moves or removes the sleep Zs, Electrified sparks, Burn flames,
 * Freeze ice block and the Veil icon to match the unit's current statuses.
 */
void BattleStatusEffectMain(BattleUnit* unit) {
    BSE_Sleep(unit);
    BSE_Biribiri(unit);
    BSE_Fire(unit);
    BSE_Freeze(unit);
    BSE_Kagegakure(unit);
}

/** Removes all of the unit's status effects; called when the unit is deleted from battle. */
void BattleStatusEffectDelete(BattleUnit* unit) {
    BSE_SleepDelete(unit);
    BSE_BiribiriDelete(unit);
    BSE_FireDelete(unit);
    BSE_FreezeDelete(unit);
    BSE_KagegakureDelete(unit);
}

/**
 * While the unit is asleep, keeps a sleep effect (the floating Zs) at the top-right of its body part:
 * half the width across and 0.95 of the height up, scaled by the unit's size (at body height for ceiling units).
 * Deletes it once the unit wakes.
 */
static void BSE_Sleep(BattleUnit* unit) {
    f32 x, y, z;
    f32* wk;
    f32 dx, dy;
    BOOL on = FALSE;

    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP)) {
        on = TRUE;
    }
    if (on) {
        if (unit->sleepEff == NULL) {
            unit->sleepEff = effSleepEntry(0, 0, -1000.0f, -1000.0f, -1000.0f, 1.0f, 60.0f);
        }
        wk = (f32*)unit->sleepEff->userdata;
        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &x, &y, &z);
        dx = 0.5f * (unit->sizeMultiplier * BtlUnit_GetWidth(unit));
        dy = 0.95f * (unit->sizeMultiplier * BtlUnit_GetHeight(unit));
        if (unit->attributeFlags & kCeiling) {
            dy = 0.0f;
        }
        x += dx;
        y += dy;
        wk[1] = x;
        wk[2] = y;
        wk[3] = z;
    } else {
        BSE_SleepDelete(unit);
    }
}

/** Deletes the unit's sleep effect if it has one. */
static void BSE_SleepDelete(BattleUnit* unit) {
    if (unit->sleepEff != NULL) {
        effDelete(unit->sleepEff);
        unit->sleepEff = NULL;
    }
}

/**
 * Biribiri = Electrified.
 * While the unit is Electrified, keeps a spark effect around the middle of its body part (half its width as
 * radius, half its height up, 10 units in front); deletes it when the status ends.
 */
static void BSE_Biribiri(BattleUnit* unit) {
    f32 x, y, z;
    f32* wk;
    f32 dy;
    BOOL on = FALSE;

    if (BtlUnit_CheckStatus(unit, STATUS_ELECTRIC)) {
        on = TRUE;
    }
    if (on) {
        if (unit->sparkEff == NULL) {
            unit->sparkEff = effSparkEntry(0, 10, -1000.0f, -1000.0f, -1000.0f, BtlUnit_GetWidth(unit) / 2);
        }
        wk = (f32*)unit->sparkEff->userdata;
        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &x, &y, &z);
        dy = unit->sizeMultiplier * (BtlUnit_GetHeight(unit) / 2);
        if (unit->attributeFlags & kCeiling) {
            dy = 0.0f;
        }
        y += dy;
        wk[1] = x;
        wk[2] = y;
        wk[3] = 10.0f + z;
    } else {
        BSE_BiribiriDelete(unit);
    }
}

/** Deletes the unit's Electrified spark effect if it has one. */
static void BSE_BiribiriDelete(BattleUnit* unit) {
    if (unit->sparkEff != NULL) {
        effDelete(unit->sparkEff);
        unit->sparkEff = NULL;
    }
}

/**
 * While the unit is Burned, keeps a flame effect on it at the unit kind's burnFlameOffset (mirrored by facing
 * direction and scaled by size); deletes it when the burn ends.
 */
static void BSE_Fire(BattleUnit* unit) {
    f32 x, y, z;
    f32* wk;
    BOOL on = FALSE;

    if (BtlUnit_CheckStatus(unit, STATUS_BURN)) {
        on = TRUE;
    }
    if (on) {
        if (unit->fireEff == NULL) {
            unit->fireEff = effFireEntry(0, 0, -1000.0f, -1000.0f, -1000.0f, 1.0f);
        }
        wk = (f32*)unit->fireEff->userdata;
        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &x, &y, &z);
        x += unit->sizeMultiplier * (unit->kindParams->burnFlameOffset.x * (unit->faceDirection >= 0 ? 1.0f : -1.0f));
        y += unit->kindParams->burnFlameOffset.y * unit->sizeMultiplier;
        z += unit->kindParams->burnFlameOffset.z * unit->sizeMultiplier;
        wk[1] = x;
        wk[2] = y;
        wk[3] = z + 5.0f;
    } else {
        BSE_FireDelete(unit);
    }
}

/** Deletes the unit's Burn flame effect if it has one. */
static void BSE_FireDelete(BattleUnit* unit) {
    if (unit->fireEff != NULL) {
        effDelete(unit->fireEff);
        unit->fireEff = NULL;
    }
}

/**
 * While the unit is Frozen, keeps an ice-block effect around it,
 * scaled to the smaller of width/20 and height/36 (times the size multiplier)
 * and raised by 20 units per scale step.
 */
static void BSE_Freeze(BattleUnit* unit) {
    f32 x, y, z;
    f32* wk;
    f32 widthScale, heightScale, scale;
    BOOL on = FALSE;
    BattleUnitPart* parts;

    if (BtlUnit_CheckStatus(unit, STATUS_FREEZE)) {
        on = TRUE;
    }
    if (on) {
        if (unit->iceEff == NULL) {
            unit->iceEff = effIceEntry(0, -1, -1000.0f, -1000.0f, -1000.0f, 1.0f);
        }
        wk = (f32*)unit->iceEff->userdata;
        parts = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        widthScale = unit->width / 20.0f;
        heightScale = unit->height / 36.0f;
        scale = widthScale <= heightScale ? widthScale : heightScale;
        scale *= unit->sizeMultiplier;
        wk[4] = scale;
        BtlUnit_GetPartsWorldPos(parts, &x, &y, &z);
        wk[1] = x;
        wk[2] = 20.0f * scale + y;
        wk[3] = 10.0f + z;
    } else {
        BSE_FreezeDelete(unit);
    }
}

/**
 * Removes the unit's ice-block effect with effSoftDelete, so instead of vanishing it plays its animation on from
 * frame 90 to the end and then deletes itself (effIceMain).
 */
static void BSE_FreezeDelete(BattleUnit* unit) {
    if (unit->iceEff != NULL) {
        effSoftDelete(unit->iceEff);
        unit->iceEff = NULL;
    }
}

static void BSE_Kagegakure(BattleUnit* unit) {
    char name[0x10];
    f32 x, y, z;
    BOOL on = FALSE;

    if ((_battleWorkPointer->dispFlags & BATTLE_DISP_FLAG_COMMAND_MENU) && (unit->attributeFlags & kVeiled)) {
        on = TRUE;
    }
    if (on) {
        if (unit->kageIcon == NULL) {
            sprintf(name, "_biK_%02x", unit->unitId);
            iconEntry(name, ICON_MARIO_UNDER_GROUND_VEIL);
            unit->kageIcon = iconNameToPtr(name);
        }
        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &x, &y, &z);
        iconSetPos(unit->kageIcon->name, x, y, z + 5.0f);
    } else {
        BSE_KagegakureDelete(unit);
    }
}

/** Deletes the unit's Veil icon if it has one. */
static void BSE_KagegakureDelete(BattleUnit* unit) {
    if (unit->kageIcon != NULL) {
        iconDelete(unit->kageIcon->name);
        unit->kageIcon = NULL;
    }
}

/**
 * Starts the start-of-turn Burn damage animation for a unit;
 * battle_seq.c calls it with active 1 for every living unit whose Burn strength is above 0.
 */
void BSE_TurnFirstProcessEffectEntry(BattleUnit* unit, s16 active) {
    unit->burnDamageFlameState = 1;
    unit->mbBurnDamageFlameActive = active;
}

/**
 * Steps the start-of-turn Burn animation each frame:
 * spawns a flame at the unit, pulses its scale between 0.4 and 2.0 for 60 frames,
 * then deletes it and deals 1 damage unless the unit is hidden by BtlUnit_CheckShadowGuard.
 * Returns TRUE while still animating.
 */
BOOL BSE_TurnFirstProcessEffectMain(BattleUnit* unit) {
    BattleUnitPart* part;
    f32 x, y, z;
    f32* work;
    u16 active;
    u16 state;

    part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
    state = unit->burnDamageFlameState;
    if (state == 0) {
        return FALSE;
    }
    active = unit->mbBurnDamageFlameActive;
    if (active == 0) {
        return FALSE;
    }
    switch (active) {
        case 1:
            switch (state) {
            case 1:
                unit->burnDamageFlameState = state + 1;
                unit->burnDamageFlameScale = 1.0f;
                BtlUnit_GetPartsWorldPos(part, &x, &y, &z);
                z += 5.0f;
                unit->burnDamageFlameEff = effFireEntry(0, 0, x, y, z, unit->burnDamageFlameScale);
                unit->burnDamageFlameAnimTimer = 0;
                break;
            case 2:
                work = (f32*)unit->burnDamageFlameEff->userdata;
                unit->burnDamageFlameAnimTimer++;
                unit->burnDamageFlameScale = 1.6f * sinfd(unit->burnDamageFlameAnimTimer * 3) + 0.4f;
                work[9] = unit->burnDamageFlameScale;
                if (unit->burnDamageFlameAnimTimer >= 60) {
                    unit->burnDamageFlameState++;
                }
                break;
            case 3:
                effDelete(unit->burnDamageFlameEff);
                unit->burnDamageFlameEff = NULL;
                unit->burnDamageFlameState = 0;
                if (!BtlUnit_CheckShadowGuard(unit)) {
                    BattleDamageDirect(UNIT_ID_SYSTEM, unit, NULL, 1, 0, DAMAGE_RESULT_FINAL_HIT | DAMAGE_CODE_FIRE, 0, 1);
                }

                // fall through
            case 0:
            default:
                return FALSE;
            }
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

/**
 * Empties the unit's six-entry status-change queue (statusChangeInfo).
 * Called when the unit enters battle; in JP and later also from BtlUnit_ClearStatus, and
 * status-message processing clears it while a message is active.
 */
void BattleStatusChangeInfoWorkInit(BattleUnit* unit) {
    s32 i;
    StChgInfo* e = &unit->statusChangeInfo[0];

    for (i = 0; i < 6; i++) {
        StChgInfo* p = &e[i];
        p->type = -1;
        p->strength = 0;
        p->stagger = 0;
        p->stackOffset = 0;
        p->reserved_04 = 0;
    }
}

/**
 * Priority of a status for the change messages:
 * 24 for the first entry of _st_pri_data (Charge) down to 1 for the last, -1 if the status is not listed.
 * type is a StatusEffectType stored as s8 in _st_pri_data
 */
static s32 _get_pri(s8 type) {
    s8* val;
    s32 priority = 24;

    for (val = _st_pri_data;; priority--, val++) {
        if (*val == -1) {
            return -1; // reached end of list
        }
        if (*val == type) {
            return priority;
        }
    }
    return -1;
}

/**
 * Queues a status change on the unit (battle_damage.c calls it when a status is inflicted or changed):
 * inserts it into the six-entry statusChangeInfo list by _get_pri priority,
 * replacing an entry of the same status, then gives each entry a 10-step stagger and a stacking offset.
 * BattleStatusChangeMsgAdjust still reads the queued types; nothing reads the stagger or offsets.
 * Guess: they laid out per-unit popups that BattleStatusChangeAnnouceMain_Unit no longer draws.
 */
void BattleStatusChangeInfoSetAnnouce(BattleUnit* unit, s32 type, s32 strength, s32 changed) {
    StChgInfo* e = &unit->statusChangeInfo[0];
    StChgInfo tmp, swap;
    s8 pri = _get_pri(type);
    s32 i, j, n;
    u8 u, t;

    for (i = 0; i < 6; i++) {
        s32 queuedType = e[i].type;
        n = _get_pri(queuedType);
        if (pri >= (s8)n) {
            break;
        }
    }
    if (i >= 6) {
        return;
    }
    tmp.type = type;
    tmp.strength = strength;
    tmp.changed = changed;
    if (type == e[i].type) {
        e[i] = tmp;
    } else {
        for (; i < 6; i++) {
            swap = e[i];
            e[i] = tmp;
            tmp = swap;
        }
    }
    j = 0;
    n = 0;
    t = 0;
    for (i = 0; i < 6; i++) {
        if (e[i].type == -1) {
            break;
        }
        e[i].stagger = t;
        e[i].reserved_04 = j;
        t += 10;
        n++;
    }

    // FAKE: goto chain for a plain if/else; if (t != 0) { u = 0x50; } else { u = 0x3C; } changes the object
    if (t == 0) goto L3;
    u = 0x50;
    goto Ld;
L3:
    u = 0x3C;
Ld:;
    for (j = n; j >= 0; j--) {
        if (e[j].type != -1) {
            e[j].stackOffset = u;
            u += 10;
        }
    }
}

/**
 * Runs the per-unit status announcements for every unit and returns TRUE while any is still showing;
 * since BattleStatusChangeAnnouceMain_Unit is empty it always returns FALSE.
 * Called from the battle sequence alongside BattleStatusChangeMsgMain.
 */
BOOL BattleStatusChangeAnnouceMain(BattleWork* wp) {
    s32 i;
    BOOL ret;
    BattleUnit* unit;

    ret = FALSE;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && BattleStatusChangeAnnouceMain_Unit(unit)) {
            ret = TRUE;
        }
    }
    return ret;
}

/** Per-unit status announcement step; empty in every version, so it never holds up the battle sequence. */
BOOL BattleStatusChangeAnnouceMain_Unit(BattleUnit* unit) {
    return FALSE;
}

/** Clears the 28 status-change message slots in BattleWork (one per status type) at battle start. */
void BattleStatusChangeMsgWorkInit(void) {
    StChgMsg* msg = &_battleWorkPointer->announce.stChg.msg[0];
    s32 i;

    memset(msg, 0, sizeof(StChgWork));
    for (i = 0; i < 28; i++) {
        msg[i].type = -1;
    }
}

void BattleStatusChangeMsgSetAnnouce(BattleUnit* unit, s32 index, s32 changed) {
    StChgWork* st = &_battleWorkPointer->announce.stChg;
    StChgMsg* msg = &st->msg[index];
    s32 id;

    msg->unitId = unit->unitId;
    msg->type = index;
    msg->timer = 0;
    msg->changed = changed;
    id = unit->currentKind;
    if (id == UNIT_MARIO) {
        msg->flags |= ST_CHG_MSG_MARIO;
    } else if (id >= TYPE_PARTNER_MIN && id < TYPE_PARTNER_MAX && unit->alliance == ALLIANCE_PLAYER) {
        msg->flags |= ST_CHG_MSG_PARTNER;
    } else {
        msg->flags |= ST_CHG_MSG_OTHER;
    }
}

/**
 * Rebuilds the pending status messages before they are shown:
 * for each status, re-marks it from every living unit whose statusChangeInfo queue holds it,
 * and drops it if no unit does or if its bit is set in the StChgWork mask.
 */
void BattleStatusChangeMsgAdjust(BattleWork* wp) {
    s32 t;
    StChgWork* base;
    s8 i;
    StChgMsg* msg;
    s32 j;
    BattleUnit* unit;
    s32 n, found, k;
    StChgInfo* e;

    base = &wp->announce.stChg;

    for (i = 0; i < 28; i++) {
        t = i;
        msg = &base->msg[t];
        if (msg->type == -1) continue;
        msg->flags = 0;
        for (j = 0; j < BATTLE_UNIT_MAX; j++) {
            unit = BattleGetUnitPtr(wp, j);
            if (unit && !BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
                e = &unit->statusChangeInfo[0];
                found = 0;
                k = 0;
                for (n = 0; n < 6; n++) {
                    if (i == e[n].type) {
                        found = 1;
                        break;
                    }
                    k++;
                }
                if (found) {
                    BattleStatusChangeMsgSetAnnouce(unit, t, e[k].changed);
                }
            }
        }
        if (!msg->flags) {
            msg->unitId = -1;
            msg->type = -1;
        } else if (*(s64*)&base->maskHi & (s64)(1 << t)) {
            msg->unitId = -1;
            msg->type = -1;
        }
    }
}

BOOL BattleStatusChangeMsgMain(BattleWork* wp) {
    StChgWork* base = &wp->announce.stChg;
    StChgMsg* msg;
    BOOL ret = FALSE;
    s32 i;

    for (i = 0; _st_pri_data[i] != -1; i++) {
        msg = &base->msg[_st_pri_data[i]];
        if (msg->type == -1) continue;
        ret = TRUE;
        BattleGetUnitPtr(wp, msg->unitId);
        if (msg->timer) {
            msg->timer--;
            if (BattlePadCheckTrigger(PAD_BUTTON_A | PAD_BUTTON_B)) {
                msg->timer = 0;
            }
            if (!msg->timer) {
                msg->type = -1;
            }
        } else {
            sprintf(base->text, "");
            for (i = 0; i < 28; i++) {
                if (msg->type == _st_chg_msg_data[i].type || _st_chg_msg_data[i].type == -1) {
                    if (msg->changed) {
                        sprintf(base->text, msgSearch(_st_chg_msg_data[i].msg1));
                    } else {
                        sprintf(base->text, msgSearch(_st_chg_msg_data[i].msg0));
                    }
                    break;
                }
            }
            msg->timer = 90;
        }
        dispEntry(CAMERA_2D, 900.0f, DISP_MODE_ALPHA_TEST, (DispCallback)_st_chg_msg_disp, 0);
        break;
    }
#if VERSION >= VERSION_JP
    if (ret) {
        for (i = 0; i < BATTLE_UNIT_MAX; i++) {
            BattleUnit* unit = BattleGetUnitPtr(wp, i);
            if (unit) {
                BattleStatusChangeInfoWorkInit(unit);
            }
        }
    }
#endif
    return ret;
}

typedef void (*WakuColByRefFunc)(u16, GXColor*, f32, f32, f32, f32, f32);

/**
 * Draws the current status-change text centred in the lower half of the screen (y -120) in a white-bordered window
 * sized to the text, 29 units per line.
 */
static void _st_chg_msg_disp(void) {
    StChgWork* st = &_battleWorkPointer->announce.stChg;
    f32 y;
    f32 x;
    f32 center = 0.0f;
    u16 width;
    u16 lines;
    GXColor col0;
    width = FontGetMessageWidthLine((const char*)st->text, &lines);
    lines++;
    col0 = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};

    // center is a variable: the retail code computes 0.0f - width / 2 at run time (lfs 0.0f, fsubs); a literal
    // 0.0f folds to fneg
    x = center - (width / 2);
    y = -120.0f;

    // FAKE: called through a cast prototype with the color's address (no caller copy); passing col0 by value changes the object
    ((WakuColByRefFunc)windowDispGX_Waku_col)(0, &col0, x - 20.0f, ((lines - 1) * 29) + -117.0f, (f32)(width + 40), (f32)(lines * 29 + 1), 10.0f);
    FontDrawStart();
    FontDrawMessage(x, (s32)y + (lines - 1) * 29, (const char*)st->text);
}
