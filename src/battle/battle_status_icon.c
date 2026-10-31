// Battle status icons: the icons shown over units for statuses such as sleep and poison.

#include "battle/battle_disp.h"
#include "version.h"
#include "battle/battle_status_icon.h"
#include "battle/battle.h"
#include <string.h>
#include "driver/icondrv.h"
#include "system.h"
#ifdef VERSION_G8ME01
#include "mario/mariost.h"
#endif

typedef struct IconDispTableEntry IconDispTableEntry;
struct IconDispTableEntry {
    s32 id;
    BOOL (*callback)(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry); //0x4
    s32 something;
    s32 something2;
};

BOOL _bsi_default_turn(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry);
BOOL _bsi_guruguru(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry);
BOOL _bsi_biribiri(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry);
BOOL _bsi_allergy(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry);
BOOL _bsi_regeneration(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry);

IconDispTableEntry _disp_table[0x17] = {
#ifdef VERSION_G8ME01
    {1, _bsi_default_turn, 22, 1},
    {4, _bsi_default_turn, 20, 1},
    {21, _bsi_default_turn, 21, 1},
    {2, _bsi_default_turn, 23, 1},
    {8, _bsi_default_turn, 24, 1},
    {9, _bsi_default_turn, 25, 1},
    {19, _bsi_default_turn, 26, 1},
    {20, _bsi_default_turn, 27, 1},
    {14, _bsi_default_turn, 28, 1},
    {15, _bsi_default_turn, 29, 1},
    {7, _bsi_default_turn, 41, 1},
    {10, _bsi_default_turn, 42, 1},
    {11, _bsi_default_turn, 43, 1},
    {5, _bsi_default_turn, 44, 1},
    {12, _bsi_default_turn, 45, 1},
    {13, _bsi_default_turn, 46, 1},
    {18, _bsi_default_turn, 47, 1},
    {3, _bsi_guruguru, 39, 0},
    {6, _bsi_biribiri, 38, 0},
    {0, _bsi_allergy, 40, 0},
    {23, _bsi_regeneration, 30, 0},
    {24, _bsi_regeneration, 31, 0},
    {-1, NULL, 0, 0},
};
IconDispTableEntry _disp_table_1[0x17] = {
    {1, _bsi_default_turn, 22, 1},
    {4, _bsi_default_turn, 20, 1},
    {21, _bsi_default_turn, 21, 1},
    {2, _bsi_default_turn, 23, 1},
    {8, _bsi_default_turn, 24, 1},
    {9, _bsi_default_turn, 25, 1},
    {19, _bsi_default_turn, 26, 1},
    {20, _bsi_default_turn, 27, 1},
    {14, _bsi_default_turn, 28, 1},
    {15, _bsi_default_turn, 29, 1},
    {7, _bsi_default_turn, 41, 1},
    {10, _bsi_default_turn, 42, 1},
    {11, _bsi_default_turn, 43, 1},
    {5, _bsi_default_turn, 44, 1},
    {12, _bsi_default_turn, 45, 1},
    {13, _bsi_default_turn, 46, 1},
    {18, _bsi_default_turn, 47, 1},
    {3, _bsi_guruguru, 39, 0},
    {6, _bsi_biribiri, 38, 0},
    {0, _bsi_allergy, 40, 0},
    {23, _bsi_regeneration, 32, 0},
    {24, _bsi_regeneration, 33, 0},
    {-1, NULL, 0, 0},
};
IconDispTableEntry _disp_table_2[0x17] = {
    {1, _bsi_default_turn, 22, 1},
    {4, _bsi_default_turn, 20, 1},
    {21, _bsi_default_turn, 21, 1},
    {2, _bsi_default_turn, 23, 1},
    {8, _bsi_default_turn, 24, 1},
    {9, _bsi_default_turn, 25, 1},
    {19, _bsi_default_turn, 26, 1},
    {20, _bsi_default_turn, 27, 1},
    {14, _bsi_default_turn, 28, 1},
    {15, _bsi_default_turn, 29, 1},
    {7, _bsi_default_turn, 41, 1},
    {10, _bsi_default_turn, 42, 1},
    {11, _bsi_default_turn, 43, 1},
    {5, _bsi_default_turn, 44, 1},
    {12, _bsi_default_turn, 45, 1},
    {13, _bsi_default_turn, 46, 1},
    {18, _bsi_default_turn, 47, 1},
    {3, _bsi_guruguru, 39, 0},
    {6, _bsi_biribiri, 38, 0},
    {0, _bsi_allergy, 40, 0},
    {23, _bsi_regeneration, 34, 0},
    {24, _bsi_regeneration, 35, 0},
    {-1, NULL, 0, 0},
};
IconDispTableEntry _disp_table_3[0x17] = {
    {1, _bsi_default_turn, 22, 1},
    {4, _bsi_default_turn, 20, 1},
    {21, _bsi_default_turn, 21, 1},
    {2, _bsi_default_turn, 23, 1},
    {8, _bsi_default_turn, 24, 1},
    {9, _bsi_default_turn, 25, 1},
    {19, _bsi_default_turn, 26, 1},
    {20, _bsi_default_turn, 27, 1},
    {14, _bsi_default_turn, 28, 1},
    {15, _bsi_default_turn, 29, 1},
    {7, _bsi_default_turn, 41, 1},
    {10, _bsi_default_turn, 42, 1},
    {11, _bsi_default_turn, 43, 1},
    {5, _bsi_default_turn, 44, 1},
    {12, _bsi_default_turn, 45, 1},
    {13, _bsi_default_turn, 46, 1},
    {18, _bsi_default_turn, 47, 1},
    {3, _bsi_guruguru, 39, 0},
    {6, _bsi_biribiri, 38, 0},
    {0, _bsi_allergy, 40, 0},
    {23, _bsi_regeneration, 36, 0},
    {24, _bsi_regeneration, 37, 0},
    {-1, NULL, 0, 0},
#else
    {1, _bsi_default_turn, 15, 1},
    {4, _bsi_default_turn, 13, 1},
    {21, _bsi_default_turn, 14, 1},
    {2, _bsi_default_turn, 16, 1},
    {8, _bsi_default_turn, 17, 1},
    {9, _bsi_default_turn, 18, 1},
    {19, _bsi_default_turn, 19, 1},
    {20, _bsi_default_turn, 20, 1},
    {14, _bsi_default_turn, 21, 1},
    {15, _bsi_default_turn, 22, 1},
    {7, _bsi_default_turn, 28, 1},
    {10, _bsi_default_turn, 29, 1},
    {11, _bsi_default_turn, 30, 1},
    {5, _bsi_default_turn, 31, 1},
    {12, _bsi_default_turn, 32, 1},
    {13, _bsi_default_turn, 33, 1},
    {18, _bsi_default_turn, 34, 1},
    {3, _bsi_guruguru, 26, 0},
    {6, _bsi_biribiri, 25, 0},
    {0, _bsi_allergy, 27, 0},
    {23, _bsi_regeneration, 23, 0},
    {24, _bsi_regeneration, 24, 0},
    {-1, NULL, 0, 0},
#endif
};

//local prototypes
void BattleStatusIconDisp(CameraId camId, void* param);

/** Clears the unit's status icon work; called from BtlUnit_Entry. */
void BattleStatusIconInit(BattleUnit* unit) {
    memset(&unit->mStatusIconWork, 0, sizeof(BattleWorkStatusIcon));
}

void BattleStatusIconMain(BattleUnit* unit) {
    BattleWorkStatusIcon* icon = &unit->mStatusIconWork;
    BattleWork* work = _battleWorkPointer;
    u32 i;

    icon->mNumStatusIcons = 0;
    if (work->flags & BATTLE_FLAG_FORCE_STATUS_ICONS_UNUSED || work->dispFlags & 1) {
        BtlUnit_GetPos(unit, &icon->mPosition.x, &icon->mPosition.y, &icon->mPosition.z);
        icon->mPosition.x += ((f32)(unit->mStatusIconOffset[0]) * unit->mSizeMultiplier);
        icon->mPosition.y += ((f32)(unit->mStatusIconOffset[1]) * unit->mSizeMultiplier);
        icon->mScreenSpaceOffset.x = 13.440001f;
        icon->mScreenSpaceOffset.z = 0.0f;
        icon->mFlags = 0;
        if (!(unit->mAttributeFlags & kCeiling)) {
            icon->mDistanceBetweenIcons = 25.6f;
            icon->mScreenSpaceOffset.y = 19.200001f;
        } else {
            icon->mDistanceBetweenIcons = 25.6f;
            icon->mScreenSpaceOffset.y = 0.0f;
        }

        for (i = 0; i < 0x17; i++) {
#ifdef VERSION_G8ME01
            IconDispTableEntry* d;
            BattleWorkStatusIconEntry* e;
            switch (gp->language) {
                case LANGUAGE_GE:
                    d = &_disp_table_1[i];
                    break;
                case LANGUAGE_FR:
                case LANGUAGE_SP:
                    d = &_disp_table_2[i];
                    break;
                case LANGUAGE_IT:
                    d = &_disp_table_3[i];
                    break;
                default:
                    d = &_disp_table[i];
                    break;
            }
#else
            IconDispTableEntry* d = &_disp_table[i];
#endif
#ifdef VERSION_G8ME01
            e = &icon->mIcons[icon->mNumStatusIcons];
#else
            BattleWorkStatusIconEntry* e = &icon->mIcons[icon->mNumStatusIcons];
#endif
            if (d->id == -1) break;
            if (d->callback(unit, d, e)) {
                icon->mNumStatusIcons++;
            }
        }
        if (icon->mNumStatusIcons > 0) {
            dispEntry(CAMERA_2D, 901.0f, 1, BattleStatusIconDisp, unit);
        }
    }
}

/** Status-icon cleanup when a unit is deleted; empty in every version. */
void BattleStatusIconDelete(BattleUnit* unit) {
    ; //stubbed in retail
}

void BattleStatusIconDisp(CameraId camId, void* param) {
    BattleWorkStatusIcon* icon = &((BattleUnit*)param)->mStatusIconWork;
    GXColor col = {0xFF, 0xFF, 0xFF, 0xFF};
    Vec screen, p, n;
    Mtx m, trans, scale, npivot, pivot, rx, ry, rz, nm;
    BattleWorkStatusIconEntry* e;
    s32 i;

    btlGetScreenPoint(&icon->mPosition, &screen);
    for (i = icon->mNumStatusIcons - 1; i >= 0; i--) {
        e = &icon->mIcons[i];
        p = screen;
        p.y += icon->mDistanceBetweenIcons * (f32)i;
        p.y += icon->mScreenSpaceOffset.y;
        p.x += icon->mScreenSpaceOffset.x;
        p.z += icon->mScreenSpaceOffset.z;
        p.z -= (f32)(-(i * 2));
        if (!(e->flags & STATUS_ICON_NO_PLATE)) {
            if (!(icon->mFlags & 1)) {
                btlDispTexPlane(p.x, p.y, p.z, 0.8f, 0.8f, VERSION_PICK(0x23, 0x30), col, 0);
            } else {
                btlDispTexPlane(p.x, p.y, p.z, 0.8f, -0.8f, VERSION_PICK(0x23, 0x30), col, 0);
            }
        }
        if (e->icon == 0) {
            continue;
        }
        if (!(e->flags & STATUS_ICON_HIDDEN)) {
            PSMTXTrans(trans, p.x + e->trans.x, p.y + e->trans.y, p.z + e->trans.z);
            PSMTXScale(scale, 0.8f * e->scale.x, 0.8f * e->scale.y, e->scale.z);
            PSMTXTrans(npivot, -e->pivot.x, -e->pivot.y, -e->pivot.z);
            PSMTXTrans(pivot, e->pivot.x, e->pivot.y, e->pivot.z);
            PSMTXRotRad(rx, 'x', 0.017453292f * e->rot.x);
            PSMTXRotRad(ry, 'y', 0.017453292f * e->rot.y);
            PSMTXRotRad(rz, 'z', 0.017453292f * e->rot.z);
            PSMTXIdentity(m);
            PSMTXConcat(rz, scale, m);
            PSMTXConcat(npivot, m, m);
            PSMTXConcat(rz, m, m);
            PSMTXConcat(ry, m, m);
            PSMTXConcat(rx, m, m);
            PSMTXConcat(pivot, m, m);
            PSMTXConcat(trans, m, m);
            btlDispTexPlane2(m, e->icon, e->color, 0);
        }
        if (e->turns != 0) {
            n = p;
            n.x += 21.6f;
            n.y -= 16.0f;
            PSMTXIdentity(nm);
            PSMTXTrans(trans, n.x, n.y, n.z);
            PSMTXConcat(trans, nm, nm);
            iconNumberDispGx(nm, e->turns, TRUE, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
    }
}

/**
 * Icon builder for the plain turn-count statuses (Sleep, Poison, Stop, Burn, Freeze, Fast, Slow,
 * attack and defense changes, Dodgy, Huge, Tiny, Confused, Invisible, Payback):
 * fills the entry with the status's icon and turn count and returns TRUE if the unit has the status.
 */
BOOL _bsi_default_turn(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry) {
    s32 type = disp->id;
    s8 turns, strength;
    s32 kind;
    if (_battleWorkPointer->flags & BATTLE_FLAG_FORCE_STATUS_ICONS_UNUSED) {
        if (type != STATUS_SLEEP && !BtlUnit_CheckStatus(unit, disp->id)) {
            return FALSE;
        }
        kind = unit->currentKind;
        if (!((kind >= UNIT_GOOMBA && kind < UNIT_UNUSED_TREE) || (kind >= UNIT_MARIO && kind < TYPE_PARTNER_MIN) || (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX))) return FALSE;
    } else if (!BtlUnit_CheckStatus(unit, disp->id)) {
        return FALSE;
    }
    BtlUnit_GetStatus(unit, type, &turns, &strength);
    entry->type = type;
    entry->turns = turns >= 0 ? turns : -turns;
    entry->icon = disp->something;
    entry->scale = (Vec){1.0f,1.0f,1.0f};
    entry->pivot = (Vec){0.0f,0.0f,0.0f};
    entry->rot = (Vec){0.0f,0.0f,0.0f};
    entry->color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    entry->flags = disp->something2;
    entry->trans = (Vec){0.0f,0.0f,0.0f};
    return TRUE;
}

/**
 * Guruguru = Dizzy.
 * Builds the Dizzy icon, spinning it 4 degrees per frame about its centre and raised 4 units;
 * resets the spin and returns FALSE when the unit is not Dizzy.
 */
BOOL _bsi_guruguru(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry) {
    int type = disp->id;
    s8 turns, strength;
    Vec v;
    if (!BtlUnit_CheckStatus(unit, type)) {
        unit->mStatusIconWork.dizzyAngle = 0.0f;
        return FALSE;
    }
    unit->mStatusIconWork.dizzyAngle += 4.0f;
    BtlUnit_GetStatus(unit, type, &turns, &strength);
    entry->type = type;
    entry->turns = turns >= 0 ? turns : -turns;
    entry->icon = disp->something;
    entry->scale = (Vec){1.0f,1.0f,1.0f};
    entry->pivot = (Vec){0.0f,0.0f,0.0f};
    v = (Vec){0.0f,0.0f,0.0f};
    v.z = unit->mStatusIconWork.dizzyAngle;
    entry->rot = v;
    entry->color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    entry->flags = disp->something2;
    entry->trans = (Vec){0.0f,4.0f,0.0f};
    return TRUE;
}

/**
 * Builds the Allergic icon, which blinks: it is hidden for 16 frames out of every 32.
 * Returns FALSE (and resets the blink timer) when the unit is not Allergic.
 */
BOOL _bsi_allergy(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry) {
    int type = disp->id;
    s8 turns, strength;
    if (!BtlUnit_CheckStatus(unit, type)) {
        unit->mStatusIconWork.allergyBlinkTimer = 0;
        return FALSE;
    }
    BtlUnit_GetStatus(unit, type, &turns, &strength);
    entry->type = type;
    entry->turns = turns >= 0 ? turns : -turns;
    entry->icon = disp->something;
    entry->scale = (Vec){1.0f,1.0f,1.0f};
    entry->pivot = (Vec){0.0f,0.0f,0.0f};
    entry->rot = (Vec){0.0f,0.0f,0.0f};
    entry->color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    entry->flags = disp->something2;
    entry->trans = (Vec){0.0f,0.0f,0.0f};
    unit->mStatusIconWork.allergyBlinkTimer++;
    if (unit->mStatusIconWork.allergyBlinkTimer & 0x10) {
        entry->flags |= STATUS_ICON_HIDDEN;
    }
    return TRUE;
}

/**
 * Biribiri = Electrified.
 * Builds the Electrified icon, which flickers: full size for 4-9 frames, then shrunk to 0.8 for 30-129 frames,
 * at random lengths. Returns FALSE and resets the flicker when the unit is not Electrified.
 */
BOOL _bsi_biribiri(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry) {
    s32 type = disp->id;
    s8 turns, strength;
    if (!BtlUnit_CheckStatus(unit, disp->id)) {
        unit->mStatusIconWork.elecState = 0;
        unit->mStatusIconWork.elecTimer = 0;
        return FALSE;
    }
    entry->scale = (Vec){1.0f,1.0f,1.0f};
    switch (unit->mStatusIconWork.elecState) {
        case 0:
            unit->mStatusIconWork.elecState++;
            unit->mStatusIconWork.elecTimer = irand(6) + 4;
        case 1:
            entry->scale = (Vec){1.0f,1.0f,1.0f};
            unit->mStatusIconWork.elecTimer--;
            if (unit->mStatusIconWork.elecTimer < 0) {
                unit->mStatusIconWork.elecState++;
                unit->mStatusIconWork.elecTimer = irand(100) + 30;
            }
            break;
        case 2:
            entry->scale = (Vec){0.8f,0.8f,1.0f};
            unit->mStatusIconWork.elecTimer--;
            if (unit->mStatusIconWork.elecTimer < 0) {
                unit->mStatusIconWork.elecTimer = irand(6) + 4;
                unit->mStatusIconWork.elecState = 1;
            }
            break;
    }
    BtlUnit_GetStatus(unit, type, &turns, &strength);
    entry->type = type;
    entry->turns = turns >= 0 ? turns : -turns;
    entry->icon = disp->something;
    entry->pivot = (Vec){0.0f,0.0f,0.0f};
    entry->rot = (Vec){0.0f,0.0f,0.0f};
    entry->color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    entry->flags = disp->something2;
    entry->trans = (Vec){0.0f,0.0f,0.0f};
    return TRUE;
}

/**
 * Builds the HP Regen or FP Regen icon (status 23 or 24),
 * tinting it back and forth over 60 frames between pink-red and yellow (HP) or green and cyan (FP).
 * Returns FALSE when the unit lacks the status or for any other status.
 */
BOOL _bsi_regeneration(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry) {
    s32 type = disp->id;
    s16* ctr;
    GXColor c0, c1, col;
    s32 t;
    s8 turns, strength;
    if (type == STATUS_HP_REGEN) {
        ctr = &unit->mStatusIconWork.hpRegenTimer;
        c0 = (GXColor){0xED, 0x14, 0x5B, 0xFF};
        c1 = (GXColor){0xFF, 0xEA, 0x05, 0xFF};
    } else if (type == STATUS_FP_REGEN) {
        ctr = &unit->mStatusIconWork.fpRegenTimer;
        c0 = (GXColor){0x3D, 0xD1, 0x09, 0xFF};
        c1 = (GXColor){0x61, 0xE9, 0xE7, 0xFF};
    } else {
        return FALSE;
    }
    if (!BtlUnit_CheckStatus(unit, disp->id)) {
        *ctr = 0;
        return FALSE;
    }
    (*ctr)++;
    t = (*ctr * 12) % 720;
    if (t >= 360) {
        t = 720 - t;
    }
    BtlUnit_GetStatus(unit, type, &turns, &strength);
    entry->type = type;
    entry->turns = turns >= 0 ? turns : -turns;
    entry->icon = disp->something;
    entry->scale = (Vec){1.0f,1.0f,1.0f};
    entry->pivot = (Vec){0.0f,0.0f,0.0f};
    entry->rot = (Vec){0.0f,0.0f,0.0f};
    col = (GXColor){0x00, 0x00, 0x00, 0xFF};
    col.r = intplGetValue(EASING_SINE_IN_OUT, c0.r, c1.r, t, 360);
    col.g = intplGetValue(EASING_SINE_IN_OUT, c0.g, c1.g, t, 360);
    col.b = intplGetValue(EASING_SINE_IN_OUT, c0.b, c1.b, t, 360);
    entry->color = col;
    entry->flags = disp->something2;
    entry->trans = (Vec){0.0f,0.0f,0.0f};
    return TRUE;
}
