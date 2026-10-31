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
#include <stdio.h>
#include <string.h>
#include "driver/icondrv.h"
#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include "battle/battle_damage.h"

// FAKE: no-op cast of the msg array to StChgMsg*; &_battleWorkPointer->announce.stChg.msg[i] changes the object
#define ST_CHG_MSG(i) ((StChgMsg*)_battleWorkPointer->announce.stChg.msg + (i))

BOOL BattleStatusChangeAnnouceMain_Unit(BattleUnit* unit);
void _st_chg_msg_disp(void);

typedef struct StChgMsgData {
    s8 type;
    const char* name;
    const char* msg1;
    const char* msg0;
} StChgMsgData;

/**
 * Message for each status change announcement (the Japanese status name is only a label):
 * msg1 when the status took hold, msg0 when not; only Charge differs ("...But it had no effect!").
 * Ends with type -1.
 */
StChgMsgData _st_chg_msg_data[] = {
    { 4, "\202\307\202\255", "msg_st_chg_poison", "msg_st_chg_poison" },
    { 16, "\203`\203\203\201[\203W", "msg_st_chg_charge", "msg_st_chg_charge3" },
    { 1, "\202\313\202\336\202\350", "msg_st_chg_sleep", "msg_st_chg_sleep" },
    { 2, "\203X\203g\203b\203v", "msg_st_chg_stan", "msg_st_chg_stan" },
    { 3, "\203O\203\213\203O\203\213", "msg_st_chg_guruguru", "msg_st_chg_guruguru" },
    { 5, "\202\261\202\361\202\347\202\361", "msg_st_chg_confusion", "msg_st_chg_confusion" },
    { 6, "\203r\203\212\203r\203\212", "msg_st_chg_biribiri", "msg_st_chg_biribiri" },
    { 7, "\203\210\203P\203\210\203P", "msg_st_chg_dodge", "msg_st_chg_dodge" },
    { 8, "\202\320\202\276\202\351\202\334", "msg_st_chg_fire", "msg_st_chg_fire" },
    { 9, "\202\261\202\250\202\350\202\303\202\257", "msg_st_chg_freeze", "msg_st_chg_freeze" },
    { 10, "\203f\203J\203f\203J", "msg_st_chg_big", "msg_st_chg_big" },
    { 11, "\203~\203j\203~\203j", "msg_st_chg_small", "msg_st_chg_small" },
    { 12, "\202\302\202\346\202\302\202\346", "msg_st_chg_tuyotuyo", "msg_st_chg_tuyotuyo" },
    { 13, "\202\346\202\355\202\346\202\355", "msg_st_chg_yowayowa", "msg_st_chg_yowayowa" },
    { 14, "\203J\203`\203J\203`", "msg_st_chg_hard", "msg_st_chg_hard" },
    { 15, "\203t\203j\203\203\203t\203j\203\203", "msg_st_chg_soft", "msg_st_chg_soft" },
    { 0, "\203A\203\214\203\213\203M\201[", "msg_st_chg_allergy", "msg_st_chg_allergy" },
    { 18, "\202\306\202\244\202\337\202\242", "msg_st_chg_trans", "msg_st_chg_trans" },
    { 19, "\202\315\202\342\202\242", "msg_st_chg_quick", "msg_st_chg_quick" },
    { 20, "\202\314\202\353\202\242", "msg_st_chg_slow", "msg_st_chg_slow" },
    { 21, "\202\265\202\251\202\246\202\265", "msg_st_chg_counter", "msg_st_chg_counter" },
    { 22, "\203{\203\200\203J\203E\203\223\203^\201[", "msg_st_chg_bombcounter", "msg_st_chg_bombcounter" },
    { 23, "\202\266\202\355\202\266\202\355\202g\202o", "msg_st_chg_reg_hp", "msg_st_chg_reg_hp" },
    { 24, "\202\266\202\355\202\266\202\355\202e\202o", "msg_st_chg_reg_fp", "msg_st_chg_reg_fp" },
    { -1, "", "", "" },
};

//.sdata
//Status Type Priority Data
s8 _st_pri_data[] = {
    STATUS_CHARGE, //charge is elevated
    STATUS_POISON, //poison is elevated
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

//local prototypes
void BSE_Sleep(BattleUnit* unit);
void BSE_SleepDelete(BattleUnit* unit);
void BSE_Biribiri(BattleUnit* unit);
void BSE_BiribiriDelete(BattleUnit* unit);
void BSE_Fire(BattleUnit* unit);
void BSE_FireDelete(BattleUnit* unit);
void BSE_Freeze(BattleUnit* unit);
void BSE_FreezeDelete(BattleUnit* unit);
void BSE_Kagegakure(BattleUnit* unit);
void BSE_KagegakureDelete(BattleUnit* unit);

/**
 * Status-effect setup for a new unit, called from BtlUnit_Entry;
 * empty in every version (the effect pointers start NULL).
 */
void BattleStatusEffectInit(BattleUnit* unit) {
    ; //stubbed in retail
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

EffectEntry* effSparkEntry(s32, s32, f32, f32, f32, f32);
EffectEntry* effSleepEntry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale, f32 rotation);

/**
 * While the unit is asleep, keeps a sleep effect (the floating Zs) at the top-right of its body part:
 * half the width across and 0.95 of the height up, scaled by the unit's size (at body height for ceiling units).
 * Deletes it once the unit wakes.
 */
void BSE_Sleep(BattleUnit* unit) {
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
void BSE_SleepDelete(BattleUnit* unit) {
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
void BSE_Biribiri(BattleUnit* unit) {
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
void BSE_BiribiriDelete(BattleUnit* unit) {
    if (unit->sparkEff != NULL) {
        effDelete(unit->sparkEff);
        unit->sparkEff = NULL;
    }
}

EffectEntry* effFireEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale);
EffectEntry* effIceEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale);

/**
 * While the unit is Burned, keeps a flame effect on it at the unit kind's burnFlameOffset (mirrored by facing
 * direction and scaled by size); deletes it when the burn ends.
 */
void BSE_Fire(BattleUnit* unit) {
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
void BSE_FireDelete(BattleUnit* unit) {
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
void BSE_Freeze(BattleUnit* unit) {
    f32 x, y, z;
    f32* wk;
    f32 widthScale, heightScale, scale;
    BOOL on = FALSE;

    if (BtlUnit_CheckStatus(unit, STATUS_FREEZE)) {
        on = TRUE;
    }
    if (on) {
        if (unit->iceEff == NULL) {
            unit->iceEff = effIceEntry(0, -1, -1000.0f, -1000.0f, -1000.0f, 1.0f);
        }
        wk = (f32*)unit->iceEff->userdata;
        {
            BattleUnitPart* parts = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
            widthScale = unit->width / 20.0f;
            heightScale = unit->height / 36.0f;
            scale = widthScale <= heightScale ? widthScale : heightScale;
            scale *= unit->sizeMultiplier;
            wk[4] = scale;
            BtlUnit_GetPartsWorldPos(parts, &x, &y, &z);
        }
        wk[1] = x;
        wk[2] = 20.0f * scale + y;
        wk[3] = 10.0f + z;
    } else {
        BSE_FreezeDelete(unit);
    }
}

/**
 * Removes the unit's ice-block effect with effSoftDelete, so instead of vanishing it plays out the rest of its
 * animation and then sags, fades and drips smoke puffs (effIceMain).
 */
void BSE_FreezeDelete(BattleUnit* unit) {
    if (unit->iceEff != NULL) {
        effSoftDelete(unit->iceEff);
        unit->iceEff = NULL;
    }
}


void BSE_Kagegakure(BattleUnit* unit) {
    char name[0x10];
    f32 x, y, z;
    BOOL on = FALSE;

    if ((_battleWorkPointer->dispFlags & BATTLE_DISP_FLAG_COMMAND_MENU) && (unit->attributeFlags & kVeiled)) {
        on = TRUE;
    }
    if (on) {
        if (unit->kageIcon == NULL) {
            sprintf(name, "_biK_%02x", unit->unitId);
#ifdef VERSION_G8ME01
            iconEntry(name, 0xA1);
#else
            iconEntry(name, 0x99);
#endif
            unit->kageIcon = iconNameToPtr(name);
        }
        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &x, &y, &z);
        iconSetPos(unit->kageIcon->name, x, y, z + 5.0f);
    } else {
        BSE_KagegakureDelete(unit);
    }
}

/** Deletes the unit's Veil icon if it has one. */
void BSE_KagegakureDelete(BattleUnit* unit) {
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

f32 sinfd(f32 degrees);

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
                BattleDamageDirect(-5, unit, NULL, 1, 0, DAMAGE_RESULT_FINAL_HIT | DAMAGE_CODE_FIRE, 0, 1);
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
s32 _get_pri(s8 type) {
    s8* val;
    s32 priority = 24;

    for (val = _st_pri_data;; priority--, val++) {
        if (*val == -1) {
            return -1; //reached end of list
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
        n = _get_pri((s8)(u8)e[i].type); // FAKE: no-op (s8)(u8) round trip on the s8 type (lbz, extsb at the call), as in the target
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
    StChgMsg* msg = ST_CHG_MSG(index);
    s32 id;

    msg->unitId = unit->unitId;
    msg->type = index;
    msg->timer = 0;
    msg->changed = changed;
    id = unit->currentKind;
    if (id == UNIT_MARIO) {
        msg->flags |= 1;
    } else if (id >= TYPE_PARTNER_MIN && id < TYPE_PARTNER_MAX && unit->alliance == ALLIANCE_PLAYER) {
        msg->flags |= 2;
    } else {
        msg->flags |= 4;
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
            if (unit && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
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
        dispEntry(CAMERA_2D, 900.0f, 1, (DispCallback)_st_chg_msg_disp, 0);
        break;
    }
    if (ret) {
        for (i = 0; i < BATTLE_UNIT_MAX; i++) {
            BattleUnit* unit = BattleGetUnitPtr(wp, i);
            if (unit) {
                BattleStatusChangeInfoWorkInit(unit);
            }
        }
    }
    return ret;
}

typedef void (*WakuColByRefFunc)(u16, GXColor*, f32, f32, f32, f32, f32);

/**
 * Draws the current status-change text centred near the top of the screen (y -120) in a white-bordered window
 * sized to the text, 29 units per line.
 */
void _st_chg_msg_disp(void) {
    StChgWork* st = &_battleWorkPointer->announce.stChg;
    f32 y;
    f32 x;
    u16 width;
    u16 lines;
    GXColor col0;
    width = FontGetMessageWidthLine((const char*)st->text, &lines);
    lines++;
    col0 = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    // FAKE: y doubles as a 0.0f temporary; x = 0.0f - (width / 2) changes the object
    y = 0.0f;
    x = y - (width / 2);
    y = -120.0f;
    // FAKE: called through a cast prototype with the color's address (no caller copy); passing col0 by value changes the object
    ((WakuColByRefFunc)windowDispGX_Waku_col)(0, &col0, x - 20.0f, ((lines - 1) * 29) + -117.0f, (f32)(width + 40), (f32)(lines * 29 + 1), 10.0f);
    FontDrawStart();
    FontDrawMessage(x, (s32)y + (lines - 1) * 29, (const char*)st->text);
}

