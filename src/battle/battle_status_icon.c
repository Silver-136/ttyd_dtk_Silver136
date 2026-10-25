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

void BattleStatusIconInit(BattleUnit* unit) {
    memset(&unit->mStatusIconWork, 0, sizeof(BattleWorkStatusIcon));
}

void BattleStatusIconMain(BattleUnit* unit) {
    BattleWorkStatusIcon* icon = &unit->mStatusIconWork;
    BattleWork* work = _battleWorkPointer;
    u32 i;

    icon->mNumStatusIcons = 0;
    if (work->flags & 0x10000 || work->dispFlags & 1) {
        BtlUnit_GetPos(unit, &icon->mPosition.x, &icon->mPosition.y, &icon->mPosition.z);
        icon->mPosition.x += ((f32)(unit->mStatusIconOffset[0]) * unit->mSizeMultiplier);
        icon->mPosition.y += ((f32)(unit->mStatusIconOffset[1]) * unit->mSizeMultiplier);
        icon->mScreenSpaceOffset.x = 13.440001f;
        icon->mScreenSpaceOffset.z = 0.0f;
        icon->mFlags = 0;
        if (!(unit->mAttributeFlags & kCeiling)) {
            icon->mDistanceBetweenIcons = 25.6f;
            icon->mScreenSpaceOffset.y = 19.200001f;
        }
        else {
            icon->mDistanceBetweenIcons = 25.6f;
            icon->mScreenSpaceOffset.y = 0.0f;
        }

        for (i = 0; i < 0x17; i++) {
#ifdef VERSION_G8ME01
            IconDispTableEntry* d;
            BattleWorkStatusIconEntry* e;
            switch (gp->language) {
                case 2:
                    d = &_disp_table_1[i];
                    break;
                case 3:
                case 4:
                    d = &_disp_table_2[i];
                    break;
                case 5:
                    d = &_disp_table_3[i];
                    break;
                default:
                    d = &_disp_table[i];
                    break;
            }
#else
            IconDispTableEntry* d = (IconDispTableEntry*)((u8*)_disp_table + i * 16);
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

void BattleStatusIconDelete(BattleUnit* unit) {
    ; //stubbed in retail
}

void btlDispTexPlane(u32 id, GXColor color, f32 tX, f32 tY, f32 tZ, f32 sX, f32 sY, s32 extra);
void btlDispTexPlane2(Mtx mtx, u32 id, GXColor color, s32 extra);
extern void btlGetScreenPoint(Vec* in, Vec* out);

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
        if (!(e->flags & 1)) {
            if (!(icon->mFlags & 1)) {
                btlDispTexPlane(VERSION_PICK(0x23, 0x30), col, p.x, p.y, p.z, 0.8f, 0.8f, 0);
            } else {
                btlDispTexPlane(VERSION_PICK(0x23, 0x30), col, p.x, p.y, p.z, 0.8f, -0.8f, 0);
            }
        }
        if (e->icon == 0) continue;
        if (!(e->flags & 2)) {
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
            btlDispTexPlane2(m, (u32)e->icon, e->color, 0);
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

BOOL _bsi_default_turn(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry) {
    s32 type = disp->id;
    s8 turns, strength;
    s32 kind;
    if (_battleWorkPointer->flags & 0x10000) {
        if (type != 1 && !((BOOL(*)(BattleUnit*, s8))BtlUnit_CheckStatus)(unit, disp->id)) return FALSE;
        kind = (s32)unit->currentKind;
        if (!((kind >= UNIT_GOOMBA && kind < UNIT_UNUSED_TREE) || (kind >= UNIT_MARIO && kind < TYPE_PARTNER_MIN) || (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX))) return FALSE;
    } else if (!((BOOL(*)(BattleUnit*, s8))BtlUnit_CheckStatus)(unit, disp->id)) {
        return FALSE;
    }
    ((void(*)(BattleUnit*, s8, s8*, s8*))BtlUnit_GetStatus)(unit, type, &turns, &strength);
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

BOOL _bsi_guruguru(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry) {
    int type = disp->id;
    s8 turns, strength;
    Vec v;
    if (!((BOOL(*)(BattleUnit*, s8))BtlUnit_CheckStatus)(unit, type)) {
        unit->mStatusIconWork.dizzyAngle = 0.0f;
        return FALSE;
    }
    unit->mStatusIconWork.dizzyAngle += 4.0f;
    ((void(*)(BattleUnit*, s8, s8*, s8*))BtlUnit_GetStatus)(unit, type, &turns, &strength);
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

BOOL _bsi_allergy(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry) {
    int type = disp->id;
    s8 turns, strength;
    if (!((BOOL(*)(BattleUnit*, s8))BtlUnit_CheckStatus)(unit, type)) {
        unit->mStatusIconWork.allergyBlinkTimer = 0;
        return FALSE;
    }
    ((void(*)(BattleUnit*, s8, s8*, s8*))BtlUnit_GetStatus)(unit, type, &turns, &strength);
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
    if (unit->mStatusIconWork.allergyBlinkTimer & 0x10) entry->flags |= 2;
    return TRUE;
}

BOOL _bsi_biribiri(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry) {
    s32 type = disp->id;
    s8 turns, strength;
    if (!((BOOL(*)(BattleUnit*, s8))BtlUnit_CheckStatus)(unit, disp->id)) {
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
    ((void(*)(BattleUnit*, s8, s8*, s8*))BtlUnit_GetStatus)(unit, type, &turns, &strength);
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

BOOL _bsi_regeneration(BattleUnit* unit, IconDispTableEntry* disp, BattleWorkStatusIconEntry* entry) {
    s32 type = disp->id;
    s16* ctr;
    GXColor c0, c1, col;
    s32 t;
    s8 turns, strength;
    if (type == 0x17) {
        ctr = &unit->mStatusIconWork.hpRegenTimer;
        c0 = (GXColor){0xED, 0x14, 0x5B, 0xFF};
        c1 = (GXColor){0xFF, 0xEA, 0x05, 0xFF};
    } else if (type == 0x18) {
        ctr = &unit->mStatusIconWork.fpRegenTimer;
        c0 = (GXColor){0x3D, 0xD1, 0x09, 0xFF};
        c1 = (GXColor){0x61, 0xE9, 0xE7, 0xFF};
    } else {
        return FALSE;
    }
    if (!((BOOL(*)(BattleUnit*, s8))BtlUnit_CheckStatus)(unit, disp->id)) {
        *ctr = 0;
        return FALSE;
    }
    (*ctr)++; t = (*ctr * 12) % 720;
    if (t >= 360) t = 720 - t;
    ((void(*)(BattleUnit*, s8, s8*, s8*))BtlUnit_GetStatus)(unit, type, &turns, &strength);
    entry->type = type;
    entry->turns = turns >= 0 ? turns : -turns;
    entry->icon = disp->something;
    entry->scale = (Vec){1.0f,1.0f,1.0f};
    entry->pivot = (Vec){0.0f,0.0f,0.0f};
    entry->rot = (Vec){0.0f,0.0f,0.0f};
    col = (GXColor){0x00, 0x00, 0x00, 0xFF};
    col.r = intplGetValue(11, c0.r, c1.r, t, 360);
    col.g = intplGetValue(11, c0.g, c1.g, t, 360);
    col.b = intplGetValue(11, c0.b, c1.b, t, 360);
    entry->color = col;
    entry->flags = disp->something2;
    entry->trans = (Vec){0.0f,0.0f,0.0f};
    return TRUE;
}
