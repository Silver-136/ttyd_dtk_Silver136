// Battle menu drawing: the action, weapon-select and operation menus and the pieces they are built from.

#include "version.h"
#include "battle/battle_disp.h"

#define SUBMENU_ICON_VISIBLE (1)
#define SUBMENU_ICON_SPEECH_BUBBLE (2)
#define SUBMENU_ROW_VISIBLE (1)
#include "battle/battle_menu_disp.h"
#include "battle/battle_seq_command.h"
#include "battle/battle_unit.h"
#include "system.h"
#include "battle/battle_database_common.h"
#include "data/item_data.h"
#include "driver/msgdrv.h"
#include "driver/windowdrv.h"
#include "manager/fontmgr.h"
#include "battle/battle.h"
#include "battle/battle_item_data.h"
#include "libc_decls.h"

void FontDrawString(f32 x, f32 y, const char* msg);
double sin(double);

void DrawMultiItemWin(void);
void DrawChangePartyWin(void);
void DrawOperationWin(void);
void DrawWeaponWin(void);
void DrawMainMenu(BattleWork* wp);
#include "battle/battle_audience.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "pmario_sound.h"
#if VERSION <= VERSION_DEMO_US
static void DrawSubIconSub(BattleSubMenu* menu, s32 i, Mtx m, s32 enabled, s32 icon, const char* name, s32 cost, s32 p34, void* party);
#else
void DrawSubIconSub(BattleSubMenu* menu, s32 i, Mtx m, s32 enabled, s32 icon, const char* name, s32 cost, s32 p34, void* party, s32 y);
#endif
void DrawMainIconCircle(BattleSubMenu* menu, BattleCommandAct* act, Mtx m, Vec v, f32 s);
void DrawMainIconHukidasi(BattleSubMenu* menu, BattleCommandAct* act, Mtx m, f32 s);
void DrawMenuPartyChangeButton_Sub(u32 id, f32 x, f32 y, f32 angle);
#include "mario/mariost.h"
#include "sdk/texPalette.h"
#include "icon_ids.h"
// US: menu animation lengths scale with the display rate (gp->framerate is 60 or 50)
#if VERSION >= VERSION_US
#define FRAMES(n) (gp->framerate * (n) / 60)
#define FRAMES_STEP(jp, n, x) ((x) * ((n) / gp->framerate))
#else
#define FRAMES(n) (n)
#define FRAMES_STEP(jp, n, x) ((jp) * (x))
#endif
void FontDrawStringCenterMtx(Mtx mtx, const char* msg);
/**
 * Angles in degrees of the command icons around the battle menu's ring, one row each for 3, 4 and 5 commands
 * (indexed by the icon's slot); used to place and turn the icons when the ring spins.
 */
f32 kakutable[3][5] = {{240.0f, 120.0f, 0.0f, 0.0f, 0.0f}, {270.0f, 180.0f, 90.0f, 0.0f, 0.0f}, {270.0f, 203.0f, 157.0f, 90.0f, 0.0f}};

//.sdata
GXColor seleItemCoordCol = {0xFF, 0xC0, 0xC0, 0xCF};

/**
 * Cycles seleItemCoordCol, the highlight colour of the selected list row, one step per frame through pale red,
 * yellow, green, cyan, blue and magenta (each channel between 0xC0 and 0xFF).
 * Called every frame from BattleCommandDisplay.
 */
void SelectedItemCoordinateColorUpDate(void) {
    static s8 seq;
    switch (seq) {
        case 0:
            if (++seleItemCoordCol.g == 0xFF) {
                seq = 1;
            }
            break;
        case 1:
            if (--seleItemCoordCol.r == 0xC0) {
                seq = 2;
            }
            break;
        case 2:
            if (++seleItemCoordCol.b == 0xFF) {
                seq = 3;
            }
            break;
        case 3:
            if (--seleItemCoordCol.g == 0xC0) {
                seq = 4;
            }
            break;
        case 4:
            if (++seleItemCoordCol.r == 0xFF) {
                seq = 5;
            }
            break;
        case 5:
            if (--seleItemCoordCol.b == 0xC0) {
                seq = 0;
            }
            break;
    }
}

/**
 * TRUE when the command menu may take input: the current sub-menu has finished opening (shown),
 * or an audience attack check has fired (commandMenu.attackAudience).
 * Polled by the battle_seq_command.c menu loops.
 */
BOOL BattleMenuKeyOKInACT(BattleWork* wp) {
    if (!wp->commandMenu.attackAudience && !wp->commandMenu.subMenu->shown) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Loads battle/common/battle_common.tpl, the textures for the command ring, list rows and DANGER/PERIL marks,
 * into wp->menuTex; called from btlDispInit.
 */
void battleMenuDispInit(void) {
    BattleWork* wp = _battleWorkPointer;
    char path[256];

    sprintf(path, "%s/battle/common/battle_common.tpl", getMarioStDvdRoot());
    wp->menuTex = fileAlloc(path, FILE_TYPE_TPL);
}

/** Frees the battle menu textures loaded by battleMenuDispInit; called at battle end. */
void battleMenuDispEnd(void) {
    fileFree(_battleWorkPointer->menuTex);
}

/**
 * Screen-space centre of the command ring: the acting unit's screen position moved 22.5 right and 105 up,
 * with x kept at -180 or more so the ring stays on screen.
 */
void GetRingCenter(Vec* out) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);

    btlGetScreenPoint(&unit->position, out);
    out->x += 22.5f;
    out->y += 105.0f;
    out->z = 0.0f;
    if (out->x <= -180.0f) {
        out->x = -180.0f;
    }
}

/**
 * Offset of a ring icon at angle (radians, 0 = front/bottom of the ring) on the 80 x 35 ellipse of the command
 * ring.
 */
void GetRingOffset(f32 angle, f32* x, f32* y) {
    f32 t = 4.712389f - angle;
    *x = 80.0f * (f32)cos(t);
    *y = 35.0f * (f32)sin(t);
}

/**
 * Draws the command-ring icons back to front (sorted by y, so the far side of the ring goes first):
 * the selected one as a speech-bubble label (iconFlags bit 1), the rest as round icons.
 * Only icons with iconFlags bit 0 are drawn.
 */
void DrawMainIcon(BattleSubMenu* menu, BattleCommandAct* acts, Mtx* a, Vec* b, f32* c) {
    s32 order[5];
    f32 z[6];
    BattleWork* wp = _battleWorkPointer;
    BattleCursor* cursor;
    f32 tf;
    s32 ti;
    s32 i;
    s32 j;
    s32 k;

    if (BattleGetUnitPtr(wp, wp->activeUnitId)->currentKind == UNIT_MARIO) {
        cursor = &wp->commandMenu.cursor[0];
    } else {
        cursor = &wp->commandMenu.cursor[1];
    }
    for (i = 0; i < cursor->count; i++) {
        order[i] = i;
        z[i] = b[i].y;
    }
    for (i = 0; i < cursor->count - 1; i++) {
        for (j = i + 1; j < cursor->count; j++) {
            if (z[i] < z[j]) {
                tf = z[i];
                ti = order[i];
                z[i] = z[j];
                order[i] = order[j];
                z[j] = tf;
                order[j] = ti;
            }
        }
    }
    for (i = 0; i < cursor->count; i++) {
        k = order[i];
        if (menu->iconFlags[k] & SUBMENU_ICON_VISIBLE) {
            if (menu->iconFlags[k] & SUBMENU_ICON_SPEECH_BUBBLE) {
                DrawMainIconHukidasi(menu, &acts[k], a[k], c[k]);
            } else {
                DrawMainIconCircle(menu, &acts[k], a[k], b[k], c[k]);
            }
        }
    }
}

/**
 * Hukidasi = speech bubble.
 * Draws the selected command as a bubble with its icon and name,
 * with a drop shadow that shifts as the bubble turns about y by s degrees; disabled commands are greyed.
 * The US release squeezes names wider than 90 units.
 */
void DrawMainIconHukidasi(BattleSubMenu* menu, BattleCommandAct* act, Mtx m, f32 s) {
    GXColor tev;
    GXColor col;
    Mtx rot;
    Mtx scl;
    Mtx trn;
    Mtx out;
    GXTexObj texobj;
    BattleWork* wp = _battleWorkPointer;
    CameraEntry* cam;
    const char* name;
    u16 icon;
    f32 rad;
    f32 ofs;

    cam = camGetPtr(CAMERA_2D);
    btlDispGXInit2D();
    tev = (GXColor){0xE1, 0xE1, 0xE1, 0xFF};
    GXSetTevColor(GX_TEVREG0, tev);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    PSMTXScale(scl, 1.0f, 1.0f, 1.0f);
    PSMTXTrans(trn, 0.0f, -5.0f, 0.0f);
    rad = 0.017453292f * s;
    PSMTXRotRad(rot, 'y', rad);
    PSMTXConcat(m, rot, out);
    PSMTXConcat(out, scl, out);
    PSMTXConcat(out, trn, out);
    PSMTXConcat(cam->view, out, out);
    GXLoadPosMtxImm(out, GX_PNMTX0);
    TEXGetGXTexObjFromPalette(*wp->menuTex->data, &texobj, 1);
    GXLoadTexObj(&texobj, GX_TEXMAP0);
    if (s >= 0.0f && s <= 180.0f) {
        ofs = 4.0f - 8.0f * s / 180.0f;
    }
    if (s >= 180.0f && s <= 360.0f) {
        ofs = -4.0f + 8.0f * (s - 180.0f) / 180.0f;
    }
    if (menu->iconsArrived == 0) {
        btlDispGXQuads2D(64.0f + ofs, 76.0f, -64.0f + ofs, -4.0f, 0, 0, 0, 0x80);
        btlDispGXQuads2D(64.0f, 80.0f, -64.0f, 0.0f, 0xFF, 0xFF, 0xFF, 0xFF);
    } else {
        btlDispGXQuads2D(-64.0f + ofs, 76.0f, 64.0f + ofs, -4.0f, 0, 0, 0, 0x80);
        btlDispGXQuads2D(-64.0f, 80.0f, 64.0f, 0.0f, 0xFF, 0xFF, 0xFF, 0xFF);
    }
    PSMTXTrans(trn, -67.0f, 20.0f, 0.0f);
    PSMTXScale(scl, 1.0f, 1.0f, 1.0f);
    PSMTXRotRad(rot, 'y', rad);
    PSMTXConcat(m, rot, out);
    PSMTXConcat(out, trn, out);
    PSMTXConcat(out, scl, out);
    icon = act->icon;
    if (!icon) {
        icon = ICON_ITEM_COIN;
    }
    if (act->enabled == 1) {
        ((void (*)(Mtx, s16, u16))iconDispGx2)(out, ICON_FLAG_2D, icon);
    } else {
        ((void (*)(Mtx, s16, u16))iconDispGx2)(out, ICON_FLAG_2D | ICON_FLAG_GRAY, icon);
    }
    name = act->name;
    PSMTXTrans(trn, 0.0f, 52.5f, 0.0f);
#if VERSION >= VERSION_US
    {
        // US: long command names are squeezed to 90 units wide
        f32 w = FontGetMessageWidth(name);
        PSMTXScale(scl, w > 90.0f ? 0.9f * (90.0f / w) : 0.9f, 0.9f, 0.9f);
    }
#else
    PSMTXScale(scl, 0.9f, 0.9f, 0.9f);
#endif
    PSMTXRotRad(rot, 'y', rad);
    PSMTXConcat(m, rot, out);
    PSMTXConcat(out, trn, out);
    PSMTXConcat(out, scl, out);
    FontDrawStart();
    if (act->enabled == 1) {
        col = (GXColor){0x00, 0x00, 0x00, 0xFF};
    } else {
        col = (GXColor){0x80, 0x80, 0x80, 0xFF};
    }
    FontDrawColor(col);
    FontDrawStringCenterMtx(out, name);
}

/**
 * Draws one round command icon on the ring, turned about y by s degrees,
 * scaled and darkened by its depth v.z so the far side of the ring looks smaller and dimmer;
 * disabled commands are greyed.
 */
void DrawMainIconCircle(BattleSubMenu* menu, BattleCommandAct* act, Mtx m, Vec v, f32 s) {
    GXColor tev;
    Mtx rot;
    Mtx scl;
    Mtx trn;
    Mtx out;
    GXTexObj texobj;
    BattleWork* wp = _battleWorkPointer;
    CameraEntry* cam;
    u8 c;
    u32 icon;
    f32 rad;
    f32 ofs;
    f32 sc;
    s32 i;
    s32 j;

    cam = camGetPtr(CAMERA_2D);
    c = 0xC0 - (s32)(1.5f * v.z);
    btlDispGXInit2D();
    tev = (GXColor){0xE1, 0xE1, 0xE1, 0xFF};
    GXSetTevColor(GX_TEVREG0, tev);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    sc = 1.3f - 0.005f * v.z;
    PSMTXScale(scl, sc, sc, 1.0f);
    rad = 0.017453292f * s;
    PSMTXRotRad(rot, 'y', rad);
    PSMTXConcat(m, rot, out);
    PSMTXConcat(out, scl, out);
    PSMTXConcat(cam->view, out, out);
    GXLoadPosMtxImm(out, GX_PNMTX0);
    TEXGetGXTexObjFromPalette(*wp->menuTex->data, &texobj, 0);
    GXLoadTexObj(&texobj, GX_TEXMAP0);
    if (s >= 0.0f && s <= 180.0f) {
        ofs = (4.0f - 8.0f * s / 180.0f) / 1.3f;
    }
    if (s >= 180.0f && s <= 360.0f) {
        ofs = (-4.0f + 8.0f * (s - 180.0f) / 180.0f) / 1.3f;
    }
    {
        f32 pos[4][8] = {
            {-20.0f, 40.0f, 0.0f, 40.0f, 0.0f, 20.0f, -20.0f, 20.0f},
            {20.0f, 40.0f, 20.0f, 20.0f, 0.0f, 20.0f, 0.0f, 40.0f},
            {-20.0f, 20.0f, 0.0f, 20.0f, 0.0f, 0.0f, -20.0f, 0.0f},
            {0.0f, 20.0f, 20.0f, 20.0f, 20.0f, 0.0f, 0.0f, 0.0f}
        };
        f32 uv[4][8] = {
            {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f}
        };
        for (i = 0; i < 4; i++) {
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            for (j = 0; j < 7; j += 2) {
                btlDispGXPoint2D(ofs + pos[i][j], -4.0f / 1.3f + pos[i][j + 1], 0, 0, 0, 0x80, uv[i][j], uv[i][j + 1]);
            }
        }
        for (i = 0; i < 4; i++) {
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            for (j = 0; j < 7; j += 2) {
                btlDispGXPoint2D(pos[i][j], pos[i][j + 1], c, c, c, 0xFF, uv[i][j], uv[i][j + 1]);
            }
        }
    }
    PSMTXTrans(trn, 0.0f, 7.5f, 0.0f);
    sc = 0.66f * (1.3f - 0.005f * v.z);
    PSMTXScale(scl, sc, sc, 1.0f);
    PSMTXRotRad(rot, 'y', rad);
    PSMTXIdentity(rot);
    PSMTXConcat(m, rot, out);
    PSMTXConcat(out, scl, out);
    PSMTXConcat(out, trn, out);
    icon = act->icon;
    if (!icon) {
        icon = ICON_ITEM_COIN;
    }
    if (act->enabled == 1) {
        iconDispGxCol(out, ICON_FLAG_2D, icon, (GXColor){c, c, c, 0xFF});
    } else {
        iconDispGxCol(out, ICON_FLAG_2D | ICON_FLAG_GRAY, icon, (GXColor){c, c, c, 0xFF});
    }
}

/** Retail signature: y is the name-width setting described above. */
#if VERSION <= VERSION_DEMO_US
static void DrawSubIcon(BattleSubMenu* menu, Mtx* a, s32* enabled, u16* icons, const char** names, s32* costs, s32* p34, void* party) {
#else
void DrawSubIcon(BattleSubMenu* menu, Mtx* a, s32* enabled, u16* icons, const char** names, s32* costs, s32* p34, void* party, s32 y) {
#endif
    Mtx out[7];
    Mtx m;
    BattleWork* wp = _battleWorkPointer;
    BattleCursor* cursor;
    f32 ofs;
    s32 i;

    BattleGetUnitPtr(wp, wp->activeUnitId);
    cursor = menu->cursor;
    ofs = menu->scrollPos - (f32)(s32)menu->scrollPos;
    ofs = 24.0f * ofs;
    for (i = 0; i < 7; i++) {
        if ((s32)menu->scrollPos + i < cursor->count && (menu->rowFlags[i] & SUBMENU_ROW_VISIBLE)) {
            PSMTXTrans(m, 100.0f, 35.0f - 24.0f * i + ofs, 0.0f);
            PSMTXConcat(m, a[i], out[i]);
#if VERSION <= VERSION_DEMO_US
            DrawSubIconSub(menu, i, out[i], enabled[i], icons[i], names[i], costs[i], p34[i], party);
#else
            DrawSubIconSub(menu, i, out[i], enabled[i], icons[i], names[i], costs[i], p34[i], party, y);
#endif
        }
    }
}

/** Retail signature: y selects the name squeezing described above. */
#if VERSION <= VERSION_DEMO_US
static void DrawSubIconSub(BattleSubMenu* menu, s32 i, Mtx m, s32 enabled, s32 icon, const char* name, s32 cost, s32 p34, void* party) {
BattleWork* wp = _battleWorkPointer;
    CameraEntry* cam;
    BattleCursor* cursor = menu->cursor;
    f32 frac;
    u8 alpha;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    Mtx scl;
    Mtx trn;
    Mtx out;
    GXTexObj texobj;
    s32 d1[3];
    s32 o1[3];
    s32 d2[3];
    s32 o2[3];
    s32 d3[3];
    s32 o3[3];
    char buf1[2];
    char buf2[2];
    char buf3[2];
    s32 j;
    s32 n;

    cam = camGetPtr(CAMERA_2D);
    frac = menu->scrollPos - ((s32)menu->scrollPos);
    alpha = 0xFF;
    if (i == 0) {
        alpha = 255.0f * (1.0f - frac);
    }
    if (i == 6) {
        alpha = 255.0f * frac;
    }
    j = ((s32)menu->scrollPos) + i;
    if (j == cursor->absPos) {
        r = seleItemCoordCol.r;
        g = seleItemCoordCol.g;
        b = seleItemCoordCol.b;
        a = alpha;
    } else {
        b = 0xFF;
        g = 0xFF;
        r = 0xFF;
        a = (144.0f * ((u8)alpha)) / 255.0f;
    }
    btlDispGXInit2D();
    PSMTXConcat(cam->view, m, out);
    GXLoadPosMtxImm(out, GX_PNMTX0);
    TEXGetGXTexObjFromPalette(*wp->menuTex->data, &texobj, 2);
    GXLoadTexObj(&texobj, GX_TEXMAP0);
    btlDispGXQuads2D(-152.0f, 24.0f, -136.0f, 0.0f, r, g, b, a);
    btlDispGXQuads2D(152.0f, 24.0f, 136.0f, 0.0f, r, g, b, a);
    TEXGetGXTexObjFromPalette(*wp->menuTex->data, &texobj, 3);
    GXLoadTexObj(&texobj, GX_TEXMAP0);
    btlDispGXQuads2D(-136.0f, 24.0f, 136.0f, 0.0f, r, g, b, a);
    PSMTXTrans(trn, -119.0f, 2.0f, 0.0f);
    PSMTXScale(scl, 0.5f, 0.5f, 1.0f);
    PSMTXConcat(m, trn, out);
    PSMTXConcat(out, scl, out);
    if (!((u16)icon)) {
        icon = ICON_ITEM_COIN;
    }
    if (enabled == 1) {
        iconDispGxCol(out, ICON_FLAG_2D, icon, (GXColor){0xFF, 0xFF, 0xFF, alpha});
    } else {
        iconDispGxCol(out, ICON_FLAG_2D | ICON_FLAG_GRAY, icon, (GXColor){0xFF, 0xFF, 0xFF, alpha});
    }
    FontDrawStart();
    {
        GXColor fc;
        if (enabled == 1) {
            fc = (GXColor){0x00, 0x00, 0x00, alpha};
        } else {
            fc = (GXColor){0x40, 0x40, 0x40, alpha};
        }
        FontDrawColor(fc);
    }
    PSMTXTrans(trn, -104.0f, 22.0f, 0.0f);
PSMTXScale(scl, 0.8f, 0.8f, 0.8f);
PSMTXConcat(m, trn, out);
    PSMTXConcat(out, scl, out);
    FontDrawStringMtx(out, name);
    if (cost != 0) {
        d1[2] = cost / 100;
        d1[1] = (cost - (d1[2] * 100)) / 10;
        d1[0] = (cost - (d1[2] * 100)) - (d1[1] * 10);
        if (d1[2] > 0) {
            o1[0] = 28;
            o1[1] = 14;
            o1[2] = 0;
            n = 3;
        } else if (d1[1] > 0) {
            o1[0] = 21;
            o1[1] = 7;
            o1[2] = 0;
            n = 2;
        } else {
            o1[0] = 14;
            o1[1] = 0;
            o1[2] = 0;
            n = 1;
        }
        PSMTXScale(scl, 1.0f, 1.0f, 1.0f);
        for (j = 0; j < n; j++) {
            buf1[0] = d1[j] + '0';
            buf1[1] = 0;
            PSMTXTrans(trn, 4.0f + (72.0f + o1[j]), 25.0f, 0.0f);
            PSMTXConcat(m, trn, out);
            PSMTXConcat(out, scl, out);
            FontDrawStringMtx(out, buf1);
        }
        PSMTXTrans(trn, 120.0f, 22.0f, 0.0f);
        PSMTXScale(scl, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(m, trn, out);
        PSMTXConcat(out, scl, out);
        FontDrawStringMtx(out, msgSearch("btl_cost_disp_FP"));
    }
    if (p34 != 0) {
        s32 n;
        d2[2] = p34 / 100;
        d2[1] = (p34 - (d2[2] * 100)) / 10;
        d2[0] = (p34 - (d2[2] * 100)) - (d2[1] * 10);
        if (d2[2] > 0) {
            o2[0] = 28;
            o2[1] = 14;
            o2[2] = 0;
            n = 3;
        } else if (d2[1] > 0) {
            o2[0] = 21;
            o2[1] = 7;
            o2[2] = 0;
            n = 2;
        } else {
            o2[0] = 14;
            o2[1] = 0;
            o2[2] = 0;
            n = 1;
        }
        PSMTXScale(scl, 1.0f, 1.0f, 1.0f);
        for (j = 0; j < n; j++) {
            buf2[0] = d2[j] + '0';
            buf2[1] = 0;
            PSMTXTrans(trn, 4.0f + (72.0f + o2[j]), 25.0f, 0.0f);
            PSMTXConcat(m, trn, out);
            PSMTXConcat(out, scl, out);
            FontDrawStringMtx(out, buf2);
        }
        PSMTXTrans(trn, 120.0f, 22.0f, 0.0f);
        PSMTXScale(scl, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(m, trn, out);
        PSMTXConcat(out, scl, out);
        FontDrawStringMtx(out, msgSearch("btl_cost_disp_AP"));
    }
if (party != NULL) {
        BattleCommandParty* entry = (BattleCommandParty*)(((u8*)party) + ((i + ((s32)menu->scrollPos)) * 0x18));
        s32 n;
        s32 hp;
        s32 maxhp;
        f32 x;
        hp = entry->hp;
        maxhp = entry->maxHp;
        x = 30.0f;
        PSMTXTrans(trn, x, 22.0f, 0.0f);
        PSMTXScale(scl, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(m, trn, out);
        PSMTXConcat(out, scl, out);
        FontDrawStringMtx(out, msgSearch("btl_disp_HP"));
        d3[2] = hp / 100;
        d3[1] = (hp - (d3[2] * 100)) / 10;
        d3[0] = (hp - (d3[2] * 100)) - (d3[1] * 10);
        if (d3[2] > 0) {
            o3[0] = 28;
            o3[1] = 14;
            o3[2] = 0;
            n = 3;
        } else if (d3[1] > 0) {
            o3[0] = 21;
            o3[1] = 7;
            o3[2] = 0;
            n = 2;
        } else {
            o3[0] = 14;
            o3[1] = 0;
            o3[2] = 0;
            n = 1;
        }
        PSMTXScale(scl, 1.0f, 1.0f, 1.0f);
        for (j = 0; j < n; j++) {
            buf3[0] = d3[j] + '0';
            buf3[1] = 0;
            PSMTXTrans(trn, 32.0f + (30.0f + o3[j]), 25.0f, 0.0f);
            PSMTXConcat(m, trn, out);
            PSMTXConcat(out, scl, out);
            FontDrawStringMtx(out, buf3);
        }
        if (((u32)gp->language) == 1) {
            x += 10.0f;
        }
        PSMTXTrans(trn, 64.0f + x, 22.0f, 0.0f);
        PSMTXScale(scl, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(m, trn, out);
        PSMTXConcat(out, scl, out);
        FontDrawStringMtx(out, msgSearch("btl_disp_slash"));
        d3[2] = maxhp / 100;
        d3[1] = (maxhp - (d3[2] * 100)) / 10;
        d3[0] = (maxhp - (d3[2] * 100)) - (d3[1] * 10);
        if (d3[2] > 0) {
            o3[0] = 28;
            o3[1] = 14;
            o3[2] = 0;
            n = 3;
        } else if (d3[1] > 0) {
            o3[0] = 21;
            o3[1] = 7;
            o3[2] = 0;
            n = 2;
        } else {
            o3[0] = 14;
            o3[1] = 0;
            o3[2] = 0;
            n = 1;
        }
        PSMTXScale(scl, 1.0f, 1.0f, 1.0f);
        for (j = 0; j < n; j++) {
            buf3[0] = d3[j] + '0';
            buf3[1] = 0;
            PSMTXTrans(trn, 72.0f + (x + o3[j]), 25.0f, 0.0f);
            PSMTXConcat(m, trn, out);
            PSMTXConcat(out, scl, out);
            FontDrawStringMtx(out, buf3);
        }
    }
}
#else
void DrawSubIconSub(BattleSubMenu* menu, s32 i, Mtx m, s32 enabled, s32 icon, const char* name, s32 cost, s32 p34, void* party, s32 y) {
    BattleWork* wp = _battleWorkPointer;
    CameraEntry* cam;
    BattleCursor* cursor = menu->cursor;
    f32 frac;
    u8 alpha;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    Mtx scl;
    Mtx trn;
    Mtx out;
    GXTexObj texobj;
    s32 d1[3];
    s32 o1[3];
    s32 d2[3];
    s32 o2[3];
    s32 d3[3];
    s32 o3[3];
    char buf1[2];
    char buf2[2];
    char buf3[2];
    s32 j;
    s32 n;

    cam = camGetPtr(CAMERA_2D);
    frac = menu->scrollPos - (s32)menu->scrollPos;
    alpha = 0xFF;
    if (i == 0) {
        alpha = 255.0f * (1.0f - frac);
    }
    if (i == 6) {
        alpha = 255.0f * frac;
    }
    j = (s32)menu->scrollPos + i;
    if (j == cursor->absPos) {
        r = seleItemCoordCol.r;
        g = seleItemCoordCol.g;
        b = seleItemCoordCol.b;
        a = alpha;
    } else {
        b = 0xFF;
        g = 0xFF;
        r = 0xFF;
        a = 144.0f * alpha / 255.0f;
    }
    btlDispGXInit2D();
    PSMTXConcat(cam->view, m, out);
    GXLoadPosMtxImm(out, GX_PNMTX0);
    TEXGetGXTexObjFromPalette(*wp->menuTex->data, &texobj, 2);
    GXLoadTexObj(&texobj, GX_TEXMAP0);
    btlDispGXQuads2D(-152.0f, 24.0f, -136.0f, 0.0f, r, g, b, a);
    btlDispGXQuads2D(152.0f, 24.0f, 136.0f, 0.0f, r, g, b, a);
    TEXGetGXTexObjFromPalette(*wp->menuTex->data, &texobj, 3);
    GXLoadTexObj(&texobj, GX_TEXMAP0);
    btlDispGXQuads2D(-136.0f, 24.0f, 136.0f, 0.0f, r, g, b, a);
    PSMTXTrans(trn, -119.0f, 2.0f, 0.0f);
    PSMTXScale(scl, 0.5f, 0.5f, 1.0f);
    PSMTXConcat(m, trn, out);
    PSMTXConcat(out, scl, out);
    if (!(u16)icon) {
        icon = ICON_ITEM_COIN;
    }
    if (enabled == 1) {
        iconDispGxCol(out, ICON_FLAG_2D, icon, (GXColor){0xFF, 0xFF, 0xFF, alpha});
    } else {
        iconDispGxCol(out, ICON_FLAG_2D | ICON_FLAG_GRAY, icon, (GXColor){0xFF, 0xFF, 0xFF, alpha});
    }
    FontDrawStart();
    {
        GXColor fc;
        if (enabled == 1) {
            fc = (GXColor){0x00, 0x00, 0x00, alpha};
        } else {
            fc = (GXColor){0x40, 0x40, 0x40, alpha};
        }
        FontDrawColor(fc);
    }
    PSMTXTrans(trn, -104.0f, 22.0f, 0.0f);
#if VERSION == VERSION_EU
    if (y != 0) {
        PSMTXScale(scl, FontGetMessageWidth(name) > y ? 0.8f * y / FontGetMessageWidth(name) : 0.8f, 0.8f, 0.8f);
#else
    if (y == 1) {
        PSMTXScale(scl, FontGetMessageWidth(name) > 150 ? 120.0f / FontGetMessageWidth(name) : 0.8f, 0.8f, 0.8f);
#endif
    } else {
        PSMTXScale(scl, 0.8f, 0.8f, 0.8f);
    }
    PSMTXConcat(m, trn, out);
    PSMTXConcat(out, scl, out);
    FontDrawStringMtx(out, name);
    if (cost != 0) {
        d1[2] = cost / 100;
        d1[1] = (cost - d1[2] * 100) / 10;
        d1[0] = cost - d1[2] * 100 - d1[1] * 10;
        if (d1[2] > 0) {
            o1[0] = 28;
            o1[1] = 14;
            o1[2] = 0;
            n = 3;
        } else if (d1[1] > 0) {
            o1[0] = 21;
            o1[1] = 7;
            o1[2] = 0;
            n = 2;
        } else {
            o1[0] = 14;
            o1[1] = 0;
            o1[2] = 0;
            n = 1;
        }
        PSMTXScale(scl, 1.0f, 1.0f, 1.0f);
        for (j = 0; j < n; j++) {
            buf1[0] = d1[j] + '0';
            buf1[1] = 0;
            PSMTXTrans(trn, 4.0f + (72.0f + o1[j]), 25.0f, 0.0f);
            PSMTXConcat(m, trn, out);
            PSMTXConcat(out, scl, out);
            FontDrawStringMtx(out, buf1);
        }
        PSMTXTrans(trn, 120.0f, 22.0f, 0.0f);
        PSMTXScale(scl, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(m, trn, out);
        PSMTXConcat(out, scl, out);
        FontDrawStringMtx(out, msgSearch("btl_cost_disp_FP"));
    }
    if (p34 != 0) {
        d2[2] = p34 / 100;
        d2[1] = (p34 - d2[2] * 100) / 10;
        d2[0] = p34 - d2[2] * 100 - d2[1] * 10;
        if (d2[2] > 0) {
            o2[0] = 28;
            o2[1] = 14;
            o2[2] = 0;
            n = 3;
        } else if (d2[1] > 0) {
            o2[0] = 21;
            o2[1] = 7;
            o2[2] = 0;
            n = 2;
        } else {
            o2[0] = 14;
            o2[1] = 0;
            o2[2] = 0;
            n = 1;
        }
        PSMTXScale(scl, 1.0f, 1.0f, 1.0f);
        for (j = 0; j < n; j++) {
            buf2[0] = d2[j] + '0';
            buf2[1] = 0;
            PSMTXTrans(trn, 4.0f + (72.0f + o2[j]), 25.0f, 0.0f);
            PSMTXConcat(m, trn, out);
            PSMTXConcat(out, scl, out);
            FontDrawStringMtx(out, buf2);
        }
        PSMTXTrans(trn, 120.0f, 22.0f, 0.0f);
        PSMTXScale(scl, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(m, trn, out);
        PSMTXConcat(out, scl, out);
        FontDrawStringMtx(out, msgSearch("btl_cost_disp_AP"));
    }
    if (party != NULL) {
        BattleCommandParty* entry = (BattleCommandParty*)party + (i + (s32)menu->scrollPos);
        s32 n;
        s32 hp;
        s32 maxhp;
        hp = entry->hp;
        maxhp = entry->maxHp;
        PSMTXTrans(trn, 30.0f, 22.0f, 0.0f);
        PSMTXScale(scl, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(m, trn, out);
        PSMTXConcat(out, scl, out);
        FontDrawStringMtx(out, msgSearch("btl_disp_HP"));
        d3[2] = hp / 100;
        d3[1] = (hp - d3[2] * 100) / 10;
        d3[0] = hp - d3[2] * 100 - d3[1] * 10;
        if (d3[2] > 0) {
            o3[0] = 28;
            o3[1] = 14;
            o3[2] = 0;
            n = 3;
        } else if (d3[1] > 0) {
            o3[0] = 21;
            o3[1] = 7;
            o3[2] = 0;
            n = 2;
        } else {
            o3[0] = 14;
            o3[1] = 0;
            o3[2] = 0;
            n = 1;
        }
        PSMTXScale(scl, 1.0f, 1.0f, 1.0f);
        for (j = 0; j < n; j++) {
            buf3[0] = d3[j] + '0';
            buf3[1] = 0;
            if ((u32)gp->language == LANGUAGE_JP) {
                PSMTXTrans(trn, 26.0f + (30.0f + o3[j]), 25.0f, 0.0f);
            } else {
                PSMTXTrans(trn, 28.0f + (30.0f + o3[j]), 25.0f, 0.0f);
            }
            PSMTXConcat(m, trn, out);
            PSMTXConcat(out, scl, out);
            FontDrawStringMtx(out, buf3);
        }
        if ((u32)gp->language == LANGUAGE_JP) {
            PSMTXTrans(trn, 92.0f, 22.0f, 0.0f);
        } else {
            PSMTXTrans(trn, 100.0f, 22.0f, 0.0f);
        }
        PSMTXScale(scl, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(m, trn, out);
        PSMTXConcat(out, scl, out);
        FontDrawStringMtx(out, msgSearch("btl_disp_slash"));
        d3[2] = maxhp / 100;
        d3[1] = (maxhp - d3[2] * 100) / 10;
        d3[0] = maxhp - d3[2] * 100 - d3[1] * 10;
        if (d3[2] > 0) {
            o3[0] = 28;
            o3[1] = 14;
            o3[2] = 0;
            n = 3;
        } else if (d3[1] > 0) {
            o3[0] = 21;
            o3[1] = 7;
            o3[2] = 0;
            n = 2;
        } else {
            o3[0] = 14;
            o3[1] = 0;
            o3[2] = 0;
            n = 1;
        }
        PSMTXScale(scl, 1.0f, 1.0f, 1.0f);
        for (j = 0; j < n; j++) {
            buf3[0] = d3[j] + '0';
            buf3[1] = 0;
            if ((u32)gp->language == LANGUAGE_JP) {
                PSMTXTrans(trn, 76.0f + (30.0f + o3[j]), 25.0f, 0.0f);
            } else {
                PSMTXTrans(trn, 78.0f + (30.0f + o3[j]), 25.0f, 0.0f);
            }
            PSMTXConcat(m, trn, out);
            PSMTXConcat(out, scl, out);
            FontDrawStringMtx(out, buf3);
        }
    }
}
#endif

/**
 * Draws the switch-places button between Mario and his partner
 * when BattleCommandCheckChangePositionEnable allows it.
 * While an audience item throw is pending (BattleAudience_GetItemOn), flips in texture 5 instead,
 * the throw warning:
 * between the two with the SFX_BTL_SYS_CAUTION_THROW1 sound as it starts,
 * or above Mario without a sound if he has no partner.
 */
void DrawMenuPartyChangeButton(void) {
    Vec p;
    Vec q;
    s32 id;
    f32 ix;
    f32 iy;
    f32 iz;
    BattleUnit* party;
    BattleUnit* mario;
    BattleWork* wp;
    BattleSubMenu* menu;
    f32 x;
    f32 y;

    wp = _battleWorkPointer;
    menu = wp->commandMenu.subMenu;
    camGetPtr(CAMERA_2D);
    mario = BattleGetMarioPtr(wp);
    party = BattleGetPartyPtr(wp);
    if (!party) {
        BattleAudience_GetItemOn(&id, &ix, &iy, &iz, NULL);
        if (id != -1) {
            if (menu->partyBtnAngle > 0.0f) {
                menu->partyBtnAngle -= 5.0f;
            } else {
                menu->partyBtnAngle = 0.0f;
            }
            btlGetScreenPoint(&mario->position, &p);
            DrawMenuPartyChangeButton_Sub(5, p.x, p.y - 20.0f, menu->partyBtnAngle);
        } else {
            menu->partyBtnAngle = 90.0f;
        }
    } else {
        btlGetScreenPoint(&mario->position, &p);
        btlGetScreenPoint(&party->position, &q);
        x = (p.x + q.x) / 2.0f;
        y = p.y;
        BattleAudience_GetItemOn(&id, &ix, &iy, &iz, NULL);
        if (id != -1) {
            if (menu->partyBtnAngle == 90.0f) {
                psndSFXOn("SFX_BTL_SYS_CAUTION_THROW1");
            }
            if (menu->partyBtnAngle > 0.0f) {
                menu->partyBtnAngle -= 5.0f;
            } else {
                menu->partyBtnAngle = 0.0f;
            }
            DrawMenuPartyChangeButton_Sub(5, x, y - 20.0f, menu->partyBtnAngle);
        } else {
            menu->partyBtnAngle = 90.0f;
            if (BattleCommandCheckChangePositionEnable(wp)) {
                DrawMenuPartyChangeButton_Sub(4, x, y - 20.0f, 0.0f);
            }
        }
    }
}

/** Draws battle texture plane id at screen x, y, turned about y by angle degrees (used to flip the button in). */
void DrawMenuPartyChangeButton_Sub(u32 id, f32 x, f32 y, f32 angle) {
    Mtx mtx;

    PSMTXRotRad(mtx, 'y', 0.017453292f * angle);
    PSMTXTransApply(mtx, mtx, x, y, 0.0f);
    btlDispTexPlane2(mtx, id, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, 0);
}

#if VERSION >= VERSION_US
// US: per-language "DANGER"/"PERIL" textures, the marks drawn larger outside Japanese, and the
// Mario and partner marks split into two functions drawn in an order that keeps the front one on top.
u16 danger_icon_tbl[6] = {6, 7, 8, 7, 9, 10};
u16 peril_icon_tbl[6] = {11, 12, 13, 14, 15, 16};

/**
 * Draws Mario's DANGER mark when his HP is 5 or less (and below max),
 * or the PERIL mark flashing yellow/red every 25 frames at 1 HP, using the texture for the current language,
 * 1.5x larger outside Japanese.
 */
// Descriptive name (not in the symbol maps).
void DrawMenuMarioPinchMark(void) {
    Mtx m;
    Mtx out;
    Mtx s;
    GXTexObj texobj;
    Vec p;
#else
/**
 * Draws the DANGER/PERIL marks for Mario and his partner,
 * drawing first whichever stands behind (from Mario's x position) so the front one stays on top.
 */
void DrawMenuPinchMark(void) {
    Mtx m;
    Mtx out;
    GXTexObj texobj;
    Vec p;
    Vec q;
#endif
    BattleWork* wp = _battleWorkPointer;
    CameraEntry* cam;
    BattleUnit* unit;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    u32 id;

    cam = camGetPtr(CAMERA_2D);
#if VERSION < VERSION_US
    btlDispGXInit2D();
#endif
    unit = BattleGetMarioPtr(wp);
    if (unit->currentHp > 0 && unit->currentHp < unit->maxHp && unit->currentHp <= 5) {
        if (unit->currentHp == 1) {
#if VERSION >= VERSION_US
            id = peril_icon_tbl[gp->language];
#else
            id = (gp->language ? 0 : -1) + 9;
            if (gp->retraceCount % 50 <= 24) {
                r = 0xFF;
                g = 0xE9;
                b = 0x40;
                a = 0xFF;
            } else {
                r = 0xE9;
                g = 0x52;
                b = 0x2F;
                a = 0xFF;
            }
        } else {
            id = gp->language == LANGUAGE_JP ? 6 : 7;
            r = 0xFF;
            g = 0xE9;
            b = 0x40;
            a = 0xFF;
        }
        TEXGetGXTexObjFromPalette(*wp->menuTex->data, &texobj, id);
        GXLoadTexObj(&texobj, GX_TEXMAP0);
        btlGetScreenPoint(&unit->position, &p);
        PSMTXTrans(m, p.x - 20.0f, 60.0f + p.y, 0.0f);
        PSMTXConcat(cam->view, m, out);
        GXLoadPosMtxImm(out, GX_PNMTX0);
        btlDispGXQuads2D(-28.0f, 12.0f, 28.0f, -12.0f, r, g, b, a);
    }
    unit = BattleGetPartyPtr(wp);
    if (unit && unit->currentHp > 0 && unit->currentHp < unit->maxHp && unit->currentHp <= 5) {
        if (unit->currentHp == 1) {
            id = (gp->language ? 0 : -1) + 9;
#endif
            if (gp->retraceCount % 50 <= 24) {
                r = 0xFF;
                g = 0xE9;
                b = 0x40;
                a = 0xFF;
            } else {
                r = 0xE9;
                g = 0x52;
                b = 0x2F;
                a = 0xFF;
            }
        } else {
            r = 0xFF;
            g = 0xE9;
            b = 0x40;
            a = 0xFF;
#if VERSION >= VERSION_US
            id = danger_icon_tbl[gp->language];
#else
            id = (gp->language ? 0 : -1) + 7;
#endif
        }
        TEXGetGXTexObjFromPalette(*wp->menuTex->data, &texobj, id);
        GXLoadTexObj(&texobj, GX_TEXMAP0);
#if VERSION >= VERSION_US
        btlGetScreenPoint(&unit->position, &p);
        if (unit->position.x > -120.0f) {
            PSMTXTrans(m, p.x - 20.0f, 60.0f + p.y, 20.0f);
        } else {
            PSMTXTrans(m, p.x - 20.0f, 60.0f + p.y, 0.0f);
        }
        PSMTXConcat(cam->view, m, out);
        if ((u32)gp->language != LANGUAGE_JP) {
            PSMTXScale(s, 1.5f, 1.5f, 1.5f);
            PSMTXConcat(out, s, out);
        }
        GXLoadPosMtxImm(out, GX_PNMTX0);
        btlDispGXQuads2D(-28.0f, 12.0f, 28.0f, -12.0f, r, g, b, a);
    }
}

/**
 * Draws the partner's DANGER or PERIL mark, the same as DrawMenuMarioPinchMark,
 * slightly further back than Mario's.
 */
// Descriptive name (not in the symbol maps).
void DrawMenuPartyPinchMark(void) {
    Mtx m;
    Mtx out;
    Mtx s;
    GXTexObj texobj;
    Vec p;
    BattleWork* wp = _battleWorkPointer;
    CameraEntry* cam;
    BattleUnit* unit;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    u32 id;

    cam = camGetPtr(CAMERA_2D);
    unit = BattleGetPartyPtr(wp);
    if (unit && unit->currentHp > 0 && unit->currentHp < unit->maxHp && unit->currentHp <= 5) {
        if (unit->currentHp == 1) {
            id = peril_icon_tbl[gp->language];
            if (gp->retraceCount % 50 <= 24) {
                r = 0xFF;
                g = 0xE9;
                b = 0x40;
                a = 0xFF;
            } else {
                r = 0xE9;
                g = 0x52;
                b = 0x2F;
                a = 0xFF;
            }
        } else {
            r = 0xFF;
            g = 0xE9;
            b = 0x40;
            a = 0xFF;
            id = danger_icon_tbl[gp->language];
        }
        TEXGetGXTexObjFromPalette(*wp->menuTex->data, &texobj, id);
        GXLoadTexObj(&texobj, GX_TEXMAP0);
        btlGetScreenPoint(&unit->position, &p);
        if (unit->position.x > -120.0f) {
            PSMTXTrans(m, p.x - 20.0f, 60.0f + p.y, 10.0f);
        } else {
            PSMTXTrans(m, p.x - 20.0f, 60.0f + p.y, 0.0f);
        }
        PSMTXConcat(cam->view, m, out);
        if ((u32)gp->language != LANGUAGE_JP) {
            PSMTXScale(s, 1.5f, 1.5f, 1.5f);
            PSMTXConcat(out, s, out);
        }
        GXLoadPosMtxImm(out, GX_PNMTX0);
        btlDispGXQuads2D(-28.0f, 12.0f, 28.0f, -12.0f, r, g, b, a);
    }
}

/**
 * Draws the DANGER/PERIL marks for Mario and his partner,
 * drawing first whichever stands behind (from Mario's x position) so the front one stays on top.
 */
void DrawMenuPinchMark(void) {
    BattleUnit* unit = BattleGetMarioPtr(_battleWorkPointer);
    btlDispGXInit2D();
    if (unit->position.x > -120.0f) {
        DrawMenuPartyPinchMark();
        DrawMenuMarioPinchMark();
    } else {
        DrawMenuMarioPinchMark();
        DrawMenuPartyPinchMark();
#else
        btlGetScreenPoint(&unit->position, &q);
        PSMTXTrans(m, q.x - 20.0f, 60.0f + q.y, 0.0f);
        PSMTXConcat(cam->view, m, out);
        GXLoadPosMtxImm(out, GX_PNMTX0);
        btlDispGXQuads2D(-28.0f, 12.0f, 28.0f, -12.0f, r, g, b, a);
#endif
    }
}

/**
 * Draws a sub-menu help window at x, y: looks up the message,
 * strips its <tags> and draws its first three lines at 0.8 scale.
 * With no message it shows a Japanese placeholder (there is no description!!).
 */
void DrawMenuHelpWin(const char* msg, f32 x, f32 y) {
    f32 tx;
    s8 lines[4][0x30];
    s32 line;
    const s8* p;
    s8* dst;
    s32 i;

    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF}, x, y, 400.0f, 50.0f, 10.0f);
    FontDrawStart();
    FontDrawColorIDX(0);
    FontDrawScale(0.8f);
    msg = msgSearch(msg);
    if (msg) {
        for (i = 0; i < 4; i++) {
            lines[i][0] = 0;
        }
        p = (const s8*)msg;
        line = 0;
        dst = lines[0];
        while (1) {
            if (*p == '<') {
                while (*++p != '>') {
                }
                p++;
            }
            if (*p == 0) {
                *dst = 0;
                break;
            }
            if (*p == '\n') {
                *dst = 0;
                dst = lines[++line];
                p++;
                continue;
            }
            *dst = *p;
            p++;
            dst++;
        }
        if (lines[0][0]) {
            FontDrawScale(0.8f);
        }
        tx = 20.0f + x;
        FontDrawString(tx, y - 4.0f, (char*)lines[0]);
        if (lines[1][0]) {
            FontDrawScale(0.8f);
        }
        FontDrawString(tx, y - 28.0f, (char*)lines[1]);
        if (lines[2][0]) {
            FontDrawScale(0.8f);
        }
        FontDrawString(tx, y - 52.0f, (char*)lines[2]);
    } else {
        FontDrawString(10.0f + x, y - 4.0f, "\202\271\202\302\202\337\202\242\202\315\201@\202\310\202\242\202\314\202\276\201I\201I");
    }
}

/**
 * Draws the sub-menu cursor icon on the selected visible row and the bobbing up/down arrows
 * when the list can scroll further.
 */
void DrawMenuCursorAndScrollArrow(const Vec* v) {
    Vec pos;
    BattleWork* wp = _battleWorkPointer;
    BattleSubMenu* menu = wp->commandMenu.subMenu;
    BattleCursor* cursor = menu->cursor;
    f32 bob;
    s32 n;

    bob = intplGetValue(EASING_QUADRATIC_OUT, 0.0f, 10.0f, menu->bobTimer, 6);
    menu->bobTimer++;
#if VERSION >= VERSION_US
    if (menu->bobTimer >= gp->framerate * 12 / 60) {
#else
    if (menu->bobTimer >= 12) {
#endif
        menu->bobTimer = 0;
    }
    if (cursor->relPos > 0) {
        pos.x = 155.0f + v->x;
        pos.y = 15.0f + v->y + bob;
        pos.z = 0.0f;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_ARROW_UP, 0.75f);
    }
    if (cursor->relPos < cursor->count - 6) {
        pos.x = 155.0f + v->x;
        pos.y = 20.0f + v->y - 164.0f - bob;
        pos.z = 0.0f;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_ARROW_DOWN, 0.75f);
    }
    n = cursor->absPos - cursor->relPos;
    if (n < 0) {
        n = 0;
    }
    if (n >= 6) {
        n = 5;
    }
    pos.x = v->x;
    pos.y = v->y - (f32)n * 24.0f;
    pos.z = 0.0f;
    iconDispGx(&pos, ICON_FLAG_2D | ICON_FLAG_FLIP_X, ICON_YUBI_CURSOR, 1.0f);
}

/**
 * Allocates a fresh BattleSubMenu (freeing the old one) for the list cursor,
 * sets every row to flip in one frame after the row above,
 * and returns the acting unit's command-ring cursor (Mario's or the partner's) through mainCursor.
 */
void InitSubMenuCommonProcess(BattleSubMenu** out, BattleCursor** mainCursor, BattleCursor* cursor) {
    BattleWork* wp = _battleWorkPointer;
    s32 i;

    if (wp->commandMenu.subMenu) {
        BattleFree(wp->commandMenu.subMenu);
    }
    wp->commandMenu.subMenu = BattleAlloc(sizeof(BattleSubMenu));
    *out = wp->commandMenu.subMenu;
    (*out)->cursor = cursor;
    (*out)->scrollState = 0;
    (*out)->reserved_10 = 0;
    (*out)->shown = 0;
    (*out)->prevAbsPos = cursor->absPos;
    if (BattleGetUnitPtr(wp, wp->activeUnitId)->currentKind == UNIT_MARIO) {
        *mainCursor = &wp->commandMenu.cursor[0];
    } else {
        *mainCursor = &wp->commandMenu.cursor[1];
    }
    (*out)->scrollPos = 0.0f;
    for (i = 0; i < 7; i++) {
        (*out)->rowFlags[i] = 0;
        (*out)->rowState[i] = 1;
        (*out)->rowTimer[i] = -i;
        (*out)->reserved_F8[i] = 0;
    }
}

/**
 * Sets up the command-ring icons for a sub-menu that opens from the ring:
 * the selected command flies to the bubble at the upper left (iconState 0), the others shrink away (10).
 */
void InitSubMenuCommonProcess2(BattleSubMenu* menu, BattleCursor* cursor) {
    s32 i;
    s32 idx = cursor->absPos;

    for (i = 0; i < cursor->count; i++) {
        if (++idx >= cursor->count) {
            idx = 0;
        }
        menu->iconTimer[idx] = 0;
        menu->iconSlot[idx] = i;
        menu->iconFlags[idx] = 0;
        menu->iconFlags[idx] |= SUBMENU_ICON_VISIBLE;
        if (i == cursor->count - 1) {
            menu->iconState[idx] = 0;
        } else {
            menu->iconState[idx] = 10;
        }
    }
}

/**
 * Sets up the command-ring icons for a sub-menu shown without the ring animation:
 * the selected command already sits in the bubble (iconState 1) and the others are hidden (11).
 */
void InitSubMenuCommonProcess3(BattleSubMenu* menu, BattleCursor* cursor) {
    s32 i;
    s32 idx = cursor->absPos;

    for (i = 0; i < cursor->count; i++) {
        if (++idx >= cursor->count) {
            idx = 0;
        }
        menu->iconTimer[idx] = 0;
        menu->iconSlot[idx] = i;
        menu->iconFlags[idx] = 0;
        menu->iconFlags[idx] |= SUBMENU_ICON_VISIBLE;
        if (i == cursor->count - 1) {
            menu->iconState[idx] = 1;
        } else {
            menu->iconState[idx] = 11;
        }
    }
}

/**
 * Starts a list scroll animation when the first visible row (relPos) has moved:
 * one row down (10) or up (20), or the wrap-around jumps from bottom to top (30) and top to bottom (40).
 * key is the number of visible rows.
 */
void DrawSubMenuCommonProcessSub1(BattleSubMenu* menu, BattleCursor* cursor, s32 prev, s32 key) {
    s32 i;

    if (key == 6 && cursor->count > 6 && prev == cursor->count - 6 && cursor->relPos == 0 && cursor->absPos != 1) {
        menu->scrollState = 30;
        menu->reserved_10 = 0;
        menu->scrollFrom = cursor->count - 6;
        menu->scrollTo = 0.0f;
        for (i = 0; i < 7; i++) {
            menu->rowState[i] = 30;
            menu->rowTimer[i] = 0;
        }
    } else if (key == 6 && cursor->count > 6 && prev == 0 && cursor->relPos == cursor->count - 6 && cursor->absPos != 5) {
        menu->scrollState = 40;
        menu->reserved_10 = 0;
        menu->scrollFrom = 0.0f;
        menu->scrollTo = cursor->count - 6;
        for (i = 0; i < 7; i++) {
            menu->rowState[i] = 40;
            menu->rowTimer[i] = 0;
        }
    } else if (cursor->relPos == prev + 1) {
        menu->scrollState = 10;
        menu->reserved_10 = 0;
        menu->scrollFrom = prev;
        menu->scrollTo = prev + 1;
        for (i = 0; i < 7; i++) {
            menu->rowState[i] = 10;
            menu->rowTimer[i] = 0;
        }
    } else if (cursor->relPos == prev - 1) {
        menu->scrollState = 20;
        menu->reserved_10 = 0;
        menu->scrollFrom = prev;
        menu->scrollTo = prev - 1;
        for (i = 0; i < 7; i++) {
            menu->rowState[i] = 20;
            menu->rowTimer[i] = 0;
        }
    }
}

/**
 * Per-frame animation shared by the list sub-menus:
 * keeps the cursor row on screen, steps the scroll,
 * the ring icons leaving for the bubble (filling a, b and c with their matrices, positions and angles)
 * and the rows flipping in (d). Returns the help window position in x, y.
 */
void DrawSubMenuCommonProcess(f32* x, f32* y, Mtx* a, Vec* b, f32* c, Mtx* d) {
    Mtx tm;
    Mtx sm;
    f32 ox;
    f32 oy;
    BattleWork* wp = _battleWorkPointer;
    BattleSubMenu* menu = wp->commandMenu.subMenu;
    BattleCursor* cursor = menu->cursor;
    BattleCursor* mainCursor;
    s32 steps;
    s32 prev;
    s32 n;
    s32 cnt;
    s32 cnt2;
    s32 i;
    f32 ang;
    f32 s;
    s32 sw;

    if (BattleGetUnitPtr(wp, wp->activeUnitId)->currentKind == UNIT_MARIO) {
        mainCursor = &wp->commandMenu.cursor[0];
    } else {
        mainCursor = &wp->commandMenu.cursor[1];
    }
    // FAKE: no-op (s32) cast on the min; `n = (6 <= cursor->count) ? 6 : cursor->count;` changes the object
    n = (s32)((6 <= cursor->count) ? 6 : cursor->count);
    prev = cursor->relPos;
    if (cursor->absPos <= cursor->relPos) {
        cursor->relPos = cursor->absPos - 1;
    } else if (cursor->absPos >= cursor->relPos + n - 1) {
        cursor->relPos = cursor->absPos - n + 2;
    }
    if (cursor->relPos < 0) {
        cursor->relPos = 0;
    }
    if (cursor->relPos >= cursor->count - n) {
        cursor->relPos = cursor->count - n;
    }
    *x = -200.0f;
    *y = -100.0f;
    switch (sw = menu->scrollState) {
        case 0:
            menu->shown = 1;
            menu->scrollPos = cursor->relPos;
            cnt = 0;
            for (i = 0; i < mainCursor->count; i++) {
                if (menu->iconState[i] == 1 || menu->iconState[i] == 11) {
                    cnt++;
                }
            }
            for (i = 0, cnt2 = 0; i < 7; i++) {
                if (menu->rowState[i] == 2) {
                    cnt2++;
                }
            }
            if (cnt == mainCursor->count && cnt2 == 7) {
                menu->scrollState = 2;
            }
            DrawSubMenuCommonProcessSub1(menu, cursor, prev, n);
            break;
        case 2:
            menu->shown = 1;
            menu->scrollPos = cursor->relPos;
            DrawSubMenuCommonProcessSub1(menu, cursor, prev, n);
            break;
        case 10:
        case 20:
        case 30:
        case 40:
            if (sw == 10 || sw == 20) {
                menu->shown = 1;
            }
            cnt = 0;
            for (i = 0; i < 7; i++) {
                if (menu->rowState[i] == 2) {
                    cnt++;
                }
            }
            if (cnt == 7) {
                menu->scrollState = 2;
                menu->scrollPos = cursor->relPos;
            }
            DrawSubMenuCommonProcessSub1(menu, cursor, prev, n);
            break;
    }
    menu->iconsArrived = 0;
    for (i = 0; i < mainCursor->count; i++) {
        switch (menu->iconState[i]) {
            case 0:
                menu->iconFlags[i] |= SUBMENU_ICON_VISIBLE;
                menu->iconFlags[i] |= SUBMENU_ICON_SPEECH_BUBBLE;
                GetRingCenter(&b[i]);
                {
                    f32 px = b[i].x;
                    f32 py;
                    b[i].x = intplGetValue(EASING_QUADRATIC_OUT, px, -60.0f, menu->iconTimer[i], FRAMES(12));
                    py = b[i].y;
                    b[i].y = intplGetValue(EASING_QUADRATIC_OUT, py, 55.0f, menu->iconTimer[i], FRAMES(12));
                }
                PSMTXTrans(a[i], 20.0f + b[i].x, b[i].y, b[i].z);
                c[i] = intplGetValue(EASING_LINEAR, 0.0f, 360.0f, menu->iconTimer[i], FRAMES(12));
                if (c[i] > 360.0f) {
                    c[i] -= 360.0f;
                }
                if (menu->iconTimer[i] >= 3) {
                    menu->iconsArrived = 1;
                }
                menu->iconTimer[i]++;
                if (menu->iconTimer[i] > FRAMES(12)) {
                    menu->iconState[i] = 1;
                }
                break;
            case 1:
                menu->iconFlags[i] |= SUBMENU_ICON_VISIBLE;
                menu->iconFlags[i] |= SUBMENU_ICON_SPEECH_BUBBLE;
                PSMTXTrans(a[i], -40.0f, 55.0f, 0.0f);
                c[i] = 360.0f;
                menu->iconsArrived = 1;
                break;
            case 10:
                menu->iconFlags[i] |= SUBMENU_ICON_VISIBLE;
                menu->iconFlags[i] &= ~SUBMENU_ICON_SPEECH_BUBBLE;
                ang = 3.1415927f * kakutable[mainCursor->count - 3][menu->iconSlot[i]] / 180.0f;
                GetRingCenter(&b[i]);
                GetRingOffset(ang, &ox, &oy);
                b[i].x += ox;
                b[i].y += oy;
                b[i].z = oy;
                s = 1.0f - 0.1f * menu->iconTimer[i];
                if (s <= 0.0f) {
                    menu->iconState[i] = 11;
                }
                PSMTXScale(sm, s, s, 1.0f);
                PSMTXTrans(tm, b[i].x, b[i].y, b[i].z);
                PSMTXConcat(tm, sm, a[i]);
                c[i] = 0.0f;
                menu->iconTimer[i]++;
                break;
            case 11:
                menu->iconFlags[i] &= ~SUBMENU_ICON_VISIBLE;
                break;
        }
    }
    for (i = 0; i < 7; i++) {
        switch (sw = menu->rowState[i]) {
            case 0:
                menu->rowFlags[i] &= ~SUBMENU_ROW_VISIBLE;
                menu->reserved_114[i] = 0xFF;
                break;
            case 1:
                menu->rowFlags[i] &= ~SUBMENU_ROW_VISIBLE;
                menu->reserved_114[i] = 0xFF;
                if (menu->rowTimer[i] >= 0 && i != 6) {
                    s = intplGetValue(EASING_LINEAR, 90.0f, 0.0f, menu->rowTimer[i], FRAMES(12));
                    menu->rowFlags[i] |= SUBMENU_ROW_VISIBLE;
                    PSMTXRotRad(d[i], 'y', 0.017453292f * s);
                }
                menu->rowTimer[i]++;
                if (menu->rowTimer[i] > FRAMES(12)) {
                    menu->rowState[i] = 2;
                }
                break;
            case 2:
                menu->reserved_114[i] = 0xFF;
                if (i != 6) {
                    menu->rowFlags[i] |= SUBMENU_ROW_VISIBLE;
                    PSMTXIdentity(d[i]);
                } else {
                    menu->rowFlags[i] &= ~SUBMENU_ROW_VISIBLE;
                }
                break;
            case 10:
            case 20:
            case 30:
            case 40:
                if (sw == 10 || sw == 20) {
                    steps = 12;
                }
                if (sw == 30 || sw == 40) {
                    steps = (cursor->count - 6) / 2 + 12;
                }
                menu->rowFlags[i] |= SUBMENU_ROW_VISIBLE;
                PSMTXIdentity(d[i]);
                menu->scrollPos = intplGetValue(EASING_QUADRATIC_OUT, menu->scrollFrom, menu->scrollTo, menu->rowTimer[i], steps);
                menu->rowTimer[i]++;
                if (menu->rowTimer[i] > steps) {
                    menu->rowState[i] = 2;
                    menu->rowTimer[i] = 0;
                }
                break;
        }
    }
}

/**
 * Sets up the main command ring.
 * With flag bit 0, the selected command flies back from the bubble and the others grow in;
 * otherwise all icons spread out around the ring one after another (the menu opening fresh).
 */
void BattleMenuDisp_ActSelect_Init(BattleWork* wp, BattleCursor* cursor, BattleCommandAct* acts, BOOL flag) {
    s32 i;
    s32 idx;
    BattleSubMenu* menu;

    if (wp->commandMenu.subMenu) {
        BattleFree(wp->commandMenu.subMenu);
    }
    wp->commandMenu.subMenu = BattleAlloc(sizeof(BattleSubMenu));
    if (flag & 1) {
        menu = wp->commandMenu.subMenu;
        menu->cursor = cursor;
        menu->acts = acts;
        menu->scrollState = 10;
        menu->shown = 0;
        menu->prevAbsPos = cursor->absPos;
        idx = cursor->absPos;
        for (i = 0; i < cursor->count; i++) {
            if (idx == cursor->absPos) {
                menu->iconState[idx] = 10;
            } else {
                menu->iconState[idx] = 11;
            }
            menu->iconTimer[idx] = 0;
            menu->iconSlot[idx] = cursor->count - 1 + i;
            if (menu->iconSlot[idx] >= cursor->count) {
                menu->iconSlot[idx] -= cursor->count;
            }
            menu->iconFlags[idx] = 0;
            if (++idx >= cursor->count) {
                idx = 0;
            }
        }
    } else {
        menu = wp->commandMenu.subMenu;
        menu->cursor = cursor;
        menu->acts = acts;
        menu->scrollState = 0;
        menu->shown = 0;
        menu->prevAbsPos = cursor->absPos;
        idx = cursor->absPos;
        for (i = 0; i < cursor->count; i++) {
            if (++idx >= cursor->count) {
                idx = 0;
            }
            menu->iconState[idx] = 0;
#if VERSION >= VERSION_US
            menu->iconTimer[idx] = (i * (gp->framerate * 10 / 60)) / 2;
#else
            menu->iconTimer[idx] = (i * 10) / 2;
#endif
            menu->iconSlot[idx] = i;
            menu->iconFlags[idx] = 0;
        }
    }
}

/** Per-frame update of the command ring; empty, as the animation runs in DrawMainMenu. */
void BattleMenuDisp_ActSelect_Main(BattleWork* wp) {
}

/** Draw callback of the command ring. */
void BattleMenuDisp_ActSelect_Disp(BattleWork* wp) {
    DrawMainMenu(wp);
}

/** Closes the command ring and frees its BattleSubMenu. */
void BattleMenuDisp_ActSelect_End(BattleWork* wp) {
    if (wp->commandMenu.subMenu) {
        BattleFree(wp->commandMenu.subMenu);
        wp->commandMenu.subMenu = NULL;
    }
}

/**
 * Animates and draws the main command ring: icons spread out around the ring,
 * rotate one slot left or right when the cursor moves, and the selected one becomes a bubble;
 * then draws the switch-places button and the DANGER/PERIL marks.
 * kakutable holds the slot angles for 3, 4 and 5 commands.
 */
void DrawMainMenu(BattleWork* wp_) {
    Mtx mtx[5];
    Vec pos[5];
    Mtx trans;
    Mtx rot;
    Mtx scale;
    f32 z[5];
    f32 a;
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleSubMenu* menu = wp->commandMenu.subMenu;
    BattleCursor* cursor = menu->cursor;
    f32 c;
    f32 ox;
    f32 oy;
    f32 b;
    s32 n;
    s32 diff;
    BattleCursor* mc;

    camGetPtr(CAMERA_2D);
    switch (menu->scrollState) {
    case 0:
        n = 0;
        for (i = 0; i < cursor->count; i++) {
            if (menu->iconState[i] == 3) {
                n++;
            }
        }
        if (n == cursor->count) {
            menu->scrollState = 1;
        }
        break;
    case 1:
        menu->shown = 1;
        mc = menu->cursor;
        diff = mc->absPos - menu->prevAbsPos;
        if (diff == 1 || -(mc->count - 1) == diff) {
            menu->scrollState = 2;
            menu->shown = 0;
            menu->prevAbsPos = menu->cursor->absPos;
            for (i = 0; i < cursor->count; i++) {
                menu->iconState[i] = 4;
                menu->iconTimer[i] = 0;
                menu->iconTimer[i + 10] = menu->iconSlot[i];
                menu->iconSlot[i]--;
                if (menu->iconSlot[i] == -1) {
                    menu->iconSlot[i] = cursor->count - 1;
                }
            }
        } else if (diff == -1 || mc->count - 1 == diff) {
            menu->scrollState = 2;
            menu->shown = 0;
            menu->prevAbsPos = menu->cursor->absPos;
            for (i = 0; i < cursor->count; i++) {
                menu->iconState[i] = 5;
                menu->iconTimer[i] = 0;
                menu->iconTimer[i + 10] = menu->iconSlot[i];
                menu->iconSlot[i]++;
                if (menu->iconSlot[i] == cursor->count) {
                    menu->iconSlot[i] = 0;
                }
            }
        }
        break;
    case 2:
        n = 0;
        for (i = 0; i < cursor->count; i++) {
            if (menu->iconState[i] == 3) {
                n++;
            }
        }
        if (n == cursor->count) {
            menu->scrollState = 1;
        }
        break;
    case 10:
        menu->scrollState = 1;
        menu->shown = 1;
        break;
    }

    menu->iconsArrived = 0;
    for (i = 0; i < cursor->count; i++) {
        switch (menu->iconState[i]) {
        case 0:
            menu->iconTimer[i]--;
            if (menu->iconTimer[i] <= 0) {
                menu->iconState[i] = 1;
                menu->iconTimer[i] = 0;
            }
            break;
        case 1:
            menu->iconFlags[i] |= SUBMENU_ICON_VISIBLE;
            c = 0.0f;
            a = c + FRAMES_STEP(0.2f, 12.0f, menu->iconTimer[i]);
            b = 3.1415927f * kakutable[cursor->count - 3][menu->iconSlot[i]] / 180.0f;
            if (a >= b) {
                a = b;
            }
            GetRingCenter(&pos[i]);
            GetRingOffset(a, &ox, &oy);
            pos[i].x += ox;
            pos[i].y += oy;
            pos[i].z = oy;
            if (i == cursor->absPos) {
                c = 270.0f + 90.0f * menu->iconTimer[i] / (f32)FRAMES(10);
                if (c >= 360.0f) {
                    c = 0.0f;
                }
                PSMTXRotRad(rot, 'x', 0.017453292f * c);
                PSMTXTrans(trans, 20.0f + pos[i].x, pos[i].y, pos[i].z);
                PSMTXConcat(trans, rot, mtx[i]);
                menu->iconFlags[i] |= SUBMENU_ICON_SPEECH_BUBBLE;
            } else {
                c = 270.0f + 90.0f * menu->iconTimer[i] / (f32)FRAMES(10);
                if (c >= 360.0f) {
                    c = 0.0f;
                }
                PSMTXRotRad(rot, 'x', 0.017453292f * c);
                PSMTXTrans(trans, pos[i].x, pos[i].y, pos[i].z);
                PSMTXConcat(trans, rot, mtx[i]);
            }
            z[i] = 0.0f;
            if (a >= b && menu->iconTimer[i] >= FRAMES(10)) {
                menu->iconState[i] = 3;
                menu->iconTimer[i] = -1;
            }
            menu->iconTimer[i]++;
            break;
        case 3:
            a = 3.1415927f * kakutable[cursor->count - 3][menu->iconSlot[i]] / 180.0f;
            GetRingCenter(&pos[i]);
            GetRingOffset(a, &ox, &oy);
            pos[i].x += ox;
            pos[i].y += oy;
            pos[i].z = oy;
            if (i == cursor->absPos) {
                PSMTXTrans(mtx[i], 20.0f + pos[i].x, pos[i].y, pos[i].z);
                menu->iconFlags[i] |= SUBMENU_ICON_SPEECH_BUBBLE;
            } else {
                PSMTXTrans(mtx[i], pos[i].x, pos[i].y, pos[i].z);
            }
            z[i] = 0.0f;
            break;
        case 4:
            c = kakutable[cursor->count - 3][menu->iconTimer[i + 10]];
            b = kakutable[cursor->count - 3][menu->iconSlot[i]];
            if (0.0f == b) {
                b = 360.0f;
            }
            a = 3.1415927f * c / 180.0f + FRAMES_STEP(0.2f, 12.0f, menu->iconTimer[i]);
            b = 3.1415927f * b / 180.0f;
            if (a >= b) {
                a = b;
                menu->iconState[i] = 3;
                menu->iconTimer[i] = -1;
            }
            GetRingCenter(&pos[i]);
            GetRingOffset(a, &ox, &oy);
            pos[i].x += ox;
            pos[i].y += oy;
            pos[i].z = oy;
            PSMTXTrans(mtx[i], pos[i].x, pos[i].y, pos[i].z);
            z[i] = 0.0f;
            menu->iconFlags[i] &= ~SUBMENU_ICON_SPEECH_BUBBLE;
            menu->iconTimer[i]++;
            break;
        case 5:
            c = kakutable[cursor->count - 3][menu->iconTimer[i + 10]];
            if (0.0f == c) {
                c = 360.0f;
            }
            a = 3.1415927f * c / 180.0f - FRAMES_STEP(0.2f, 12.0f, menu->iconTimer[i]);
            b = 3.1415927f * kakutable[cursor->count - 3][menu->iconSlot[i]] / 180.0f;
            if (a <= b) {
                a = b;
                menu->iconState[i] = 3;
                menu->iconTimer[i] = -1;
            }
            GetRingCenter(&pos[i]);
            GetRingOffset(a, &ox, &oy);
            pos[i].x += ox;
            pos[i].y += oy;
            pos[i].z = oy;
            PSMTXTrans(mtx[i], pos[i].x, pos[i].y, pos[i].z);
            z[i] = 0.0f;
            menu->iconFlags[i] &= ~SUBMENU_ICON_SPEECH_BUBBLE;
            menu->iconTimer[i]++;
            break;
        case 10:
            menu->iconFlags[i] |= SUBMENU_ICON_VISIBLE;
            menu->iconFlags[i] |= SUBMENU_ICON_SPEECH_BUBBLE;
            GetRingCenter(&pos[i]);
            GetRingOffset(0.0f, &ox, &oy);
            pos[i].x += ox;
            pos[i].y += oy;
            pos[i].z = oy;
            pos[i].x = intplGetValue(EASING_QUADRATIC_OUT, -60.0f, pos[i].x, menu->iconTimer[i], FRAMES(12));
            pos[i].y = intplGetValue(EASING_QUADRATIC_OUT, 55.0f, pos[i].y, menu->iconTimer[i], FRAMES(12));
            PSMTXTrans(mtx[i], 20.0f + pos[i].x, pos[i].y, pos[i].z);
            z[i] = intplGetValue(EASING_LINEAR, 360.0f, 0.0f, menu->iconTimer[i], FRAMES(12));
            menu->iconTimer[i]++;
            if (menu->iconTimer[i] > FRAMES(12)) {
                menu->iconState[i] = 3;
            }
            break;
        case 11:
            menu->iconFlags[i] |= SUBMENU_ICON_VISIBLE;
            menu->iconFlags[i] &= ~SUBMENU_ICON_SPEECH_BUBBLE;
            a = 3.1415927f * kakutable[cursor->count - 3][menu->iconSlot[i]] / 180.0f;
            GetRingCenter(&pos[i]);
            GetRingOffset(a, &ox, &oy);
            pos[i].x += ox;
            pos[i].y += oy;
            pos[i].z = oy;
            a = 0.1f * menu->iconTimer[i];
            if (a >= 1.0f) {
                menu->iconState[i] = 3;
            }
            PSMTXScale(scale, a, a, 1.0f);
            PSMTXTrans(trans, pos[i].x, pos[i].y, pos[i].z);
            PSMTXConcat(trans, scale, mtx[i]);
            z[i] = 0.0f;
            menu->iconTimer[i]++;
            break;
        }
    }
    DrawMainIcon(menu, menu->acts, mtx, pos, z);
    DrawMenuPartyChangeButton();
    DrawMenuPinchMark();
}

/**
 * Sets up the move list (jumps, hammers, items, special moves or partner moves) for the given weapon table;
 * flag bit 0 plays the ring icons' leaving animation.
 */
void BattleMenuDisp_WeaponSelect_Init(BattleWork* wp, BattleCursor* cursor, BattleCommandWeapon* weapons, BOOL flag) {
    BattleSubMenu* menu = NULL;
    BattleCursor* mainCursor = NULL;

    InitSubMenuCommonProcess(&menu, &mainCursor, cursor);
    menu->weapons = weapons;
    if (flag & 1) {
        InitSubMenuCommonProcess2(menu, mainCursor);
    } else {
        InitSubMenuCommonProcess3(menu, mainCursor);
    }
}

/** Per-frame update of the move list; empty. */
void BattleMenuDisp_WeaponSelect_Main(BattleWork* wp) {
}

/** Draw callback of the move list. */
void BattleMenuDisp_WeaponSelect_Disp(BattleWork* wp) {
    DrawWeaponWin();
}

/** Closes the move list and frees its BattleSubMenu. */
void BattleMenuDisp_WeaponSelect_End(BattleWork* wp) {
    if (wp->commandMenu.subMenu) {
        BattleFree(wp->commandMenu.subMenu);
        wp->commandMenu.subMenu = NULL;
    }
}

/**
 * Draws the move list: the ring icons, rows with each move's FP and Star Power costs,
 * the cursor and the help window (the item description for items and cooked dishes,
 * otherwise the move's description).
 */
#if VERSION <= VERSION_DEMO_US
void DrawWeaponWin(void) {
    Mtx rowMtx[7];
    Mtx iconMtx[5];
    Vec iconPos[5];
    f32 iconAngle[5];
    s32 enabled[7];
    const char* names[7];
    s32 costs[7];
    s32 p34[7];
    u16 icons[8];
    Vec v;
    f32 x;
    f32 y;
    BattleWork* wp = _battleWorkPointer;
    BattleCommand* cmd;
    BattleSubMenu* menu;
    BattleCursor* cursor;
    BattleUnit* unit;
    BattleWeapon* weapon;
    const char* help;
    s32 i;
    s32 idx;

    cmd = &wp->commandMenu;
    menu = cmd->subMenu;
    cursor = menu->cursor;
    unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    weapon = menu->weapons[cursor->absPos].weapon;
    DrawSubMenuCommonProcess(&x, &y, iconMtx, iconPos, iconAngle, rowMtx);
    for (i = 0; i < 7; i++) {
        idx = (s32)menu->scrollPos + i;
        if (idx < cursor->count) {
            enabled[i] = menu->weapons[idx].enabled;
            icons[i] = cmd->weapon[idx].icon;
            names[i] = menu->weapons[idx].name;
            if (menu->weapons[idx].weapon) {
                costs[i] = BtlUnit_GetWeaponCost(unit, menu->weapons[idx].weapon);
            } else {
                costs[i] = 0;
            }
            if (menu->weapons[idx].weapon) {
                p34[i] = (u8)menu->weapons[idx].weapon->baseSpCost;
            } else {
                p34[i] = 0;
            }
        }
    }
    DrawMainIcon(menu, wp->commandMenu.act, iconMtx, iconPos, iconAngle);
DrawSubIcon(menu, rowMtx, enabled, icons, names, costs, p34, NULL);
v = (Vec){-60.0f, 35.0f, 0.0f};
    DrawMenuCursorAndScrollArrow(&v);
    if (weapon) {
        if (weapon->itemId) {
            help = itemDataTable[weapon->itemId].description;
        } else if (weapon == &ItemWeaponData_CookingItem) {
            help = itemDataTable[menu->weapons[cursor->absPos].itemId].description;
        } else {
            help = weapon->description;
        }
    } else {
        help = itemDataTable[menu->weapons[cursor->absPos].itemId].description;
    }
    if (help) {
        DrawMenuHelpWin(help, x, y);
    }
}
#else
void DrawWeaponWin(void) {
    Mtx rowMtx[7];
    Mtx iconMtx[5];
    Vec iconPos[5];
    f32 iconAngle[5];
    s32 enabled[7];
    const char* names[7];
    s32 costs[7];
    s32 p34[7];
    u16 icons[8];
    Vec v;
    f32 x;
    f32 y;
    BattleWork* wp = _battleWorkPointer;
    BattleCommand* cmd;
    BattleSubMenu* menu;
    BattleCursor* cursor;
    BattleUnit* unit;
    BattleWeapon* weapon;
    const char* help;
    s32 i;
    s32 idx;

    cmd = &wp->commandMenu;
    menu = cmd->subMenu;
    cursor = menu->cursor;
    unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    weapon = menu->weapons[cursor->absPos].weapon;
    DrawSubMenuCommonProcess(&x, &y, iconMtx, iconPos, iconAngle, rowMtx);
    for (i = 0; i < 7; i++) {
        idx = (s32)menu->scrollPos + i;
        if (idx < cursor->count) {
            enabled[i] = menu->weapons[idx].enabled;
            icons[i] = cmd->weapon[idx].icon;
            names[i] = menu->weapons[idx].name;
            if (menu->weapons[idx].weapon) {
                costs[i] = BtlUnit_GetWeaponCost(unit, menu->weapons[idx].weapon);
            } else {
                costs[i] = 0;
            }
            if (menu->weapons[idx].weapon) {
                p34[i] = (u8)menu->weapons[idx].weapon->baseSpCost;
            } else {
                p34[i] = 0;
            }
        }
    }
    DrawMainIcon(menu, wp->commandMenu.act, iconMtx, iconPos, iconAngle);
#if VERSION == VERSION_EU
    DrawSubIcon(menu, rowMtx, enabled, icons, names, costs, p34, NULL, 220); // name width that the label is scaled down to fit
#else
    DrawSubIcon(menu, rowMtx, enabled, icons, names, costs, p34, NULL, 0);
#endif
    v = (Vec){-60.0f, 35.0f, 0.0f};
    DrawMenuCursorAndScrollArrow(&v);
    if (weapon) {
        if (weapon->itemId) {
            help = itemDataTable[weapon->itemId].description;
        } else if (weapon == &ItemWeaponData_CookingItem) {
            help = itemDataTable[menu->weapons[cursor->absPos].itemId].description;
        } else {
            help = weapon->description;
        }
    } else {
        help = itemDataTable[menu->weapons[cursor->absPos].itemId].description;
    }
    if (help) {
        DrawMenuHelpWin(help, x, y);
    }
}
#endif

/** Sets up the Tactics list on Mario's or the partner's cursor; flag bit 0 plays the ring icons' leaving animation. */
void BattleMenuDisp_Operation_Init(BattleWork* wp, BOOL flag) {
    BattleSubMenu* menu;
    BattleCursor* mainCursor;
    BattleCursor* cursor;

    if (BattleGetUnitPtr(wp, wp->activeUnitId)->currentKind == UNIT_MARIO) {
        cursor = &wp->commandMenu.cursor[8];
    } else {
        cursor = &wp->commandMenu.cursor[9];
    }
    InitSubMenuCommonProcess(&menu, &mainCursor, cursor);
    if (flag & 1) {
        InitSubMenuCommonProcess2(menu, mainCursor);
    } else {
        InitSubMenuCommonProcess3(menu, mainCursor);
    }
}

/** Per-frame update of the Tactics list; empty. */
void BattleMenuDisp_Operation_Main(BattleWork* wp) {
}

/** Draw callback of the Tactics list. */
void BattleMenuDisp_Operation_Disp(BattleWork* wp) {
    DrawOperationWin();
}

/** Closes the Tactics list and frees its BattleSubMenu. */
void BattleMenuDisp_Operation_End(BattleWork* wp) {
    if (wp->commandMenu.subMenu) {
        BattleFree(wp->commandMenu.subMenu);
        wp->commandMenu.subMenu = NULL;
    }
}

void DrawOperationWin(void) {
    Mtx rowMtx[7];
    Mtx iconMtx[5];
    Vec iconPos[5];
    f32 iconAngle[5];
    s32 enabled[7];
    const char* names[7];
    s32 costs[7];
    s32 p34[7];
    u16 icons[8];
    Vec v;
    f32 x;
    f32 y;
    BattleWork* wp = _battleWorkPointer;
    BattleSubMenu* menu = wp->commandMenu.subMenu;
    BattleCursor* cursor = menu->cursor;
    BattleCommandOperation* items;
    s32 i;
    s32 idx;

    BattleGetUnitPtr(wp, wp->activeUnitId);
    items = wp->commandMenu.operation;
    DrawSubMenuCommonProcess(&x, &y, iconMtx, iconPos, iconAngle, rowMtx);
    for (i = 0; i < 7; i++) {
        idx = (s32)menu->scrollPos + i;
        if (idx < cursor->count) {
            enabled[i] = items[idx].enabled;
            // BUG: tests row i of the list instead of row idx, so once the list scrolls the coin fallback follows the
            // wrong entry (retail code).
            if (items[i].icon) {
                icons[i] = items[idx].icon;
            } else {
                icons[i] = ICON_ITEM_COIN;
            }
            names[i] = items[idx].name;
#if VERSION <= VERSION_DEMO_US
            costs[i] = items->cost;
#else
            costs[i] = items[idx].cost;
#endif
            p34[i] = 0;
        }
    }
    DrawMainIcon(menu, wp->commandMenu.act, iconMtx, iconPos, iconAngle);
#if VERSION <= VERSION_DEMO_US
    DrawSubIcon(menu, rowMtx, enabled, icons, names, costs, p34, NULL);
#else
    DrawSubIcon(menu, rowMtx, enabled, icons, names, costs, p34, NULL, 0);
#endif
    v = (Vec){-60.0f, 35.0f, 0.0f};
    DrawMenuCursorAndScrollArrow(&v);
    if (items[cursor->absPos].help) {
        DrawMenuHelpWin(items[cursor->absPos].help, x, y);
    }
}

/**
 * Sets up the Change Partner list; here flag bit 0 skips the ring icons' leaving animation (the reverse of the
 * other lists).
 */
void BattleMenuDisp_ChangePartySelect_Init(BattleWork* wp, BOOL flag) {
    BattleSubMenu* menu;
    BattleCursor* mainCursor;

    InitSubMenuCommonProcess(&menu, &mainCursor, &wp->commandMenu.cursor[10]);
    if (flag & 1) {
        InitSubMenuCommonProcess3(menu, mainCursor);
    } else {
        InitSubMenuCommonProcess2(menu, mainCursor);
    }
}

/** Per-frame update of the Change Partner list; empty. */
void BattleMenuDisp_ChangePartySelect_Main(BattleWork* wp) {
}

/** Draw callback of the Change Partner list. */
void BattleMenuDisp_ChangePartySelect_Disp(BattleWork* wp) {
    DrawChangePartyWin();
}

/** Closes the Change Partner list and frees its BattleSubMenu. */
void BattleMenuDisp_ChangePartySelect_End(BattleWork* wp) {
    if (wp->commandMenu.subMenu) {
        BattleFree(wp->commandMenu.subMenu);
        wp->commandMenu.subMenu = NULL;
    }
}

/** Draws the Change Partner list with each partner's HP, the cursor and the help window. */
#if VERSION <= VERSION_DEMO_US
void DrawChangePartyWin(void) {
    Mtx rowMtx[7];
    Mtx iconMtx[5];
    Vec iconPos[5];
    f32 iconAngle[5];
    s32 enabled[7];
    const char* names[7];
    s32 costs[7];
    s32 p34[7];
    u16 icons[8];
    Vec v;
    f32 x;
    f32 y;
    BattleWork* wp = _battleWorkPointer;
    BattleSubMenu* menu = wp->commandMenu.subMenu;
    BattleCursor* cursor = menu->cursor;
    BattleCommandParty* items;
    s32 i;
    s32 idx;

    BattleGetUnitPtr(wp, wp->activeUnitId);
    items = wp->commandMenu.party;
    DrawSubMenuCommonProcess(&x, &y, iconMtx, iconPos, iconAngle, rowMtx);
    for (i = 0; i < 7; i++) {
        idx = (s32)menu->scrollPos + i;
        if (idx < cursor->count) {
            enabled[i] = items[idx].enabled;
            if (items[idx].icon) {
                icons[i] = items[idx].icon;
            } else {
                icons[i] = ICON_ITEM_COIN;
            }
            names[i] = items[idx].name;
            costs[i] = 0;
            p34[i] = 0;
        }
    }
    DrawMainIcon(menu, wp->commandMenu.act, iconMtx, iconPos, iconAngle);
DrawSubIcon(menu, rowMtx, enabled, icons, names, costs, p34, items);
v = (Vec){-60.0f, 35.0f, 0.0f};
    DrawMenuCursorAndScrollArrow(&v);
    if (items[cursor->absPos].help) {
        DrawMenuHelpWin(items[cursor->absPos].help, x, y);
    }
}
#else
void DrawChangePartyWin(void) {
    Mtx rowMtx[7];
    Mtx iconMtx[5];
    Vec iconPos[5];
    f32 iconAngle[5];
    s32 enabled[7];
    const char* names[7];
    s32 costs[7];
    s32 p34[7];
    u16 icons[8];
    Vec v;
    f32 x;
    f32 y;
    BattleWork* wp = _battleWorkPointer;
    BattleSubMenu* menu = wp->commandMenu.subMenu;
    BattleCursor* cursor = menu->cursor;
    BattleCommandParty* items;
    s32 i;
    s32 idx;

    BattleGetUnitPtr(wp, wp->activeUnitId);
    items = wp->commandMenu.party;
    DrawSubMenuCommonProcess(&x, &y, iconMtx, iconPos, iconAngle, rowMtx);
    for (i = 0; i < 7; i++) {
        idx = (s32)menu->scrollPos + i;
        if (idx < cursor->count) {
            enabled[i] = items[idx].enabled;
            if (items[idx].icon) {
                icons[i] = items[idx].icon;
            } else {
                icons[i] = ICON_ITEM_COIN;
            }
            names[i] = items[idx].name;
            costs[i] = 0;
            p34[i] = 0;
        }
    }
    DrawMainIcon(menu, wp->commandMenu.act, iconMtx, iconPos, iconAngle);
#if VERSION == VERSION_EU
    DrawSubIcon(menu, rowMtx, enabled, icons, names, costs, p34, items, 150); // name width that the label is scaled down to fit
#else
    DrawSubIcon(menu, rowMtx, enabled, icons, names, costs, p34, items, 1);
#endif
    v = (Vec){-60.0f, 35.0f, 0.0f};
    DrawMenuCursorAndScrollArrow(&v);
    if (items[cursor->absPos].help) {
        DrawMenuHelpWin(items[cursor->absPos].help, x, y);
    }
}
#endif

/** Sets up the Double/Triple Dip choice list; flag bit 0 plays the ring icons' leaving animation. */
void BattleMenuDisp_MultiItem_Init(BattleWork* wp, BOOL flag) {
    BattleSubMenu* menu;
    BattleCursor* mainCursor;

    InitSubMenuCommonProcess(&menu, &mainCursor, &wp->commandMenu.cursor[12]);
    if (flag & 1) {
        InitSubMenuCommonProcess2(menu, mainCursor);
    } else {
        InitSubMenuCommonProcess3(menu, mainCursor);
    }
}

/** Per-frame update of the Double/Triple Dip list; empty. */
void BattleMenuDisp_MultiItem_Main(BattleWork* wp) {
}

/** Draw callback of the Double/Triple Dip list. */
void BattleMenuDisp_MultiItem_Disp(BattleWork* wp) {
    DrawMultiItemWin();
}

/** Closes the Double/Triple Dip list and frees its BattleSubMenu. */
void BattleMenuDisp_MultiItem_End(BattleWork* wp) {
    if (wp->commandMenu.subMenu) {
        BattleFree(wp->commandMenu.subMenu);
        wp->commandMenu.subMenu = NULL;
    }
}

/**
 * Draws the Double/Triple Dip list (one, two or three items this turn) with each option's FP cost,
 * the cursor and the help window.
 */
void DrawMultiItemWin(void) {
    Mtx rowMtx[7];
    Mtx iconMtx[5];
    Vec iconPos[5];
    f32 iconAngle[5];
    s32 enabled[7];
    const char* names[7];
    s32 costs[7];
    s32 p34[7];
    u16 icons[8];
    Vec v;
    f32 x;
    f32 y;
    BattleWork* wp = _battleWorkPointer;
    BattleSubMenu* menu = wp->commandMenu.subMenu;
    BattleCursor* cursor = menu->cursor;
    BattleCommandMultiItem* items;
    s32 i;
    s32 idx;

    BattleGetUnitPtr(wp, wp->activeUnitId);
    items = wp->commandMenu.multiItem;
    DrawSubMenuCommonProcess(&x, &y, iconMtx, iconPos, iconAngle, rowMtx);
    for (i = 0; i < 7; i++) {
        idx = (s32)menu->scrollPos + i;
        if (idx < cursor->count) {
            enabled[i] = items[idx].enabled;
            if (items[idx].icon) {
                icons[i] = items[idx].icon;
            } else {
                icons[i] = ICON_ITEM_COIN;
            }
            names[i] = items[idx].name;
            costs[i] = items[idx].cost;
            p34[i] = 0;
        }
    }
    DrawMainIcon(menu, wp->commandMenu.act, iconMtx, iconPos, iconAngle);
#if VERSION <= VERSION_DEMO_US
    DrawSubIcon(menu, rowMtx, enabled, icons, names, costs, p34, NULL);
#else
    DrawSubIcon(menu, rowMtx, enabled, icons, names, costs, p34, NULL, 0);
#endif
    v = (Vec){-60.0f, 35.0f, 0.0f};
    DrawMenuCursorAndScrollArrow(&v);
    if (items[cursor->absPos].help) {
        DrawMenuHelpWin(items[cursor->absPos].help, x, y);
    }
}

