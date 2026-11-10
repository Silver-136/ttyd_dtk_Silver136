// Battle drawing helpers: 2D and textured-quad drawing, pose lookup and unit item display.

#include "version.h"
#include "battle/battle_disp.h"
#include "battle/battle.h"
#include "battle/battle_menu_disp.h"
#include "battle/battle_unit.h"
#include "sdk/texPalette.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "data/item_data.h"
#include "system.h"
#include <string.h>
#include "icon_ids.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_audience.h"
#include "battle/battle_status_icon.h"
#include "manager/evtmgr.h"
#include "battle/battle_break_slot.h"
#include "battle/battle_stage.h"
#include "battle/battle_sub.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "driver/shadowdrv.h"
#include "battle/battle_seq_command.h"
#include "driver/mapdrv.h"
#include "battle/unit_work.h"

// Statuses that select the unit's pose, in priority order (_GetStatusPoseType)
static s32 _status_pose_table[] = {
    STATUS_INSTANT_KILL,
#if VERSION <= VERSION_DEMO_JP
    STATUS_SLEEP,
#endif
    STATUS_STOP,
    STATUS_FREEZE,
#if VERSION >= VERSION_JP
    STATUS_SLEEP,
#endif
    STATUS_POISON,
    STATUS_CONFUSE,
    STATUS_DIZZY,
    -1
};

// local prototypes
static void _partsBlurControl(BattleUnitPart* part, s32 flags, Mtx orientation, GXColor color, f32 rotation);
static void btlUnitItemDisp(CameraId camId, BattleUnit* unit);
static void _pose_def(BattleUnitPart* part);
static void _pose_one_pattern(BattleUnitPart* part);
static void _pose_two_pattern(BattleUnitPart* part);
static void _btlDispTex4(u32 id, Vec pos, Vec scale, Vec rot, Vec pos2, Vec scale2, GXColor color);
static void _btlStockExpDisp(CameraId camId, void* param);

/**
 * Returns the animation name for a pose type from a unit part's pose table:
 * the first entry that matches type or is the POSE_TYPE_NORMAL fallback. NULL for no table.
 */
const char* searchPoseTbl(PoseTableEntry* table, s32 type) {
    int i;

    if (!table) return NULL;
    for (i = 0; i < POSE_TYPE_NORMAL; i++, table++) { // searches at most 28 entries
        if (table->poseType == type || table->poseType == POSE_TYPE_NORMAL) {
            return table->poseName;
        }
    }
    return NULL;
}

/**
 * Shared 2D render state for the battle overlays:
 * no culling, alpha blending, Z test always passing without writes, one vertex-colour channel.
 */
static void btlDispGXInit2DSub(void) {
    GXSetCullMode(GX_CULL_NONE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXClearVtxDesc();
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
}

/**
 * Sets up GX for flat-coloured 2D quads (position and colour, no texture);
 * action command gauges use it before btlDispGXQuads2DRasta.
 */
void btlDispGXInit2DRasta(void) {
    btlDispGXInit2DSub();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
}

/** Sets up GX for textured 2D quads (texture times vertex colour); the battle menu uses it before btlDispGXQuads2D. */
void btlDispGXInit2D(void) {
    btlDispGXInit2DSub();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
}

static void BattleStockExpDisp(void);
void btlDispAnimeSpeed(BattleUnitPart* part, f32 speed);
static void btlUnitPartsBlurControl(BattleUnitPart* part, s32 unused, Mtx orientation, GXColor color, f32 rotation);
static void floatOffsetControl(BattleUnitPart* part);
f32 getFloatDispOffset(BattleUnitPart* part);
f32 getGravityDispOffset(BattleUnitPart* part);
static void gravityOffsetControl(BattleUnitPart* part);
static void statusPoseControl(BattleUnitPart* part);

/** Emits one vertex of a flat-coloured quad at (x, y) on the 2D plane. */
void btlDispGXPoint2DRasta(f32 x, f32 y, u8 r, u8 g, u8 b, u8 a) {
    GXPosition3f32(x, y, 0.0f);
    GXColor4u8(r, g, b, a);
}

/** Draws a flat-coloured rectangle from (x0, y0) to (x1, y1); used for action command gauges and bars. */
void btlDispGXQuads2DRasta(f32 x0, f32 y0, f32 x1, f32 y1, u8 r, u8 g, u8 b, u8 a) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    btlDispGXPoint2DRasta(x0, y0, r, g, b, a);
    btlDispGXPoint2DRasta(x1, y0, r, g, b, a);
    btlDispGXPoint2DRasta(x1, y1, r, g, b, a);
    btlDispGXPoint2DRasta(x0, y1, r, g, b, a);
}

/** Emits one textured vertex at (x, y) with texture coordinate (s, t). */
void btlDispGXPoint2D(f32 x, f32 y, u8 r, u8 g, u8 b, u8 a, f32 s, f32 t) {
    GXPosition3f32(x, y, 0.0f);
    GXColor4u8(r, g, b, a);
    GXTexCoord2f32(s, t);
}

/** Draws a textured rectangle from (x0, y0) to (x1, y1) with the full texture mapped on it. */
void btlDispGXQuads2D(f32 x0, f32 y0, f32 x1, f32 y1, u8 r, u8 g, u8 b, u8 a) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    btlDispGXPoint2D(x0, y0, r, g, b, a, 0.0f, 0.0f);
    btlDispGXPoint2D(x1, y0, r, g, b, a, 1.0f, 0.0f);
    btlDispGXPoint2D(x1, y1, r, g, b, a, 1.0f, 1.0f);
    btlDispGXPoint2D(x0, y1, r, g, b, a, 0.0f, 1.0f);
}

/**
 * GX state for the battle texture planes: textured, colour and alpha from the vertices
 * (lighting off; only the ambient source is the register).
 */
static void btlDispTexPlaneInit(void) {
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
}

/**
 * Draws texture id of the battle menu texture palette as a quad centred on the loaded matrix,
 * sized to the texture, with one colour per corner.
 * Intensity (I4) textures are drawn as a colour mask, others are modulated.
 */
static void btlDispTexPlainGX(u32 id, GXColor color0, GXColor color1, GXColor color2, GXColor color3) {
    GXTexObj texobj;
    f32 x, y;

    TEXGetGXTexObjFromPalette(*_battleWorkPointer->menuTex->data, &texobj, id);
    GXLoadTexObj(&texobj, GX_TEXMAP0);
    switch (GXGetTexObjFmt(&texobj)) {
        case GX_TF_I4:
            GXSetNumTevStages(1);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            break;
        default:
            GXSetNumTevStages(1);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
            break;
    }

    x = 0.5f * (f32)GXGetTexObjWidth(&texobj);
    y = 0.5f * (f32)GXGetTexObjHeight(&texobj);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition3f32(-x, y, 0.0f);
    GXColor4u8(color0.r, color0.g, color0.b, color0.a);
    GXTexCoord2f32(0.0f, 0.0f);

    GXPosition3f32(x, y, 0.0f);
    GXColor4u8(color1.r, color1.g, color1.b, color1.a);
    GXTexCoord2f32(1.0f, 0.0f);

    GXPosition3f32(x, -y, 0.0f);
    GXColor4u8(color2.r, color2.g, color2.b, color2.a);
    GXTexCoord2f32(1.0f, 1.0f);

    GXPosition3f32(-x, -y, 0.0f);
    GXColor4u8(color3.r, color3.g, color3.b, color3.a);
    GXTexCoord2f32(0.0f, 1.0f);
}

/**
 * Draws a battle menu texture as a flat plane in the 3D scene at (tX, tY, tZ), scaled by (sX, sY);
 * used for action command signs, status icons, the Yoshi gauge and similar in-world markers.
 */
void btlDispTexPlane(f32 tX, f32 tY, f32 tZ, f32 sX, f32 sY, u32 id, GXColor color, s32 unused) {
    CameraEntry* camera;
    Mtx scale, trans, mtx;

    btlDispTexPlaneInit();
    camera = camGetCurPtr();
    PSMTXScale(scale, sX, sY, 1.0f);
    PSMTXTrans(trans, tX, tY, tZ);
    PSMTXConcat(trans, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    btlDispTexPlainGX(id, color, color, color, color);
}

/** Draws a battle menu texture with a caller-built model matrix, in camera space. */
void btlDispTexPlane2(Mtx mtx, u32 id, GXColor color, s32 unused) {
    CameraEntry* camera;
    Mtx mtx2;

    btlDispTexPlaneInit();
    MTXCopy(mtx, mtx2);
    camera = camGetCurPtr();
    MTXConcat(camera->view, mtx2, mtx2);
    GXLoadPosMtxImm(mtx2, GX_PNMTX0);
    btlDispTexPlainGX(id, color, color, color, color);
}

/** Like btlDispTexPlane2 with a separate colour for each corner (used by a Star Power special move's display). */
void btlDispTexPlane3(Mtx mtx, u32 id, GXColor color0, GXColor color1, GXColor color2, GXColor color3, s32 unused) {
    CameraEntry* camera;
    Mtx mtx2;

    btlDispTexPlaneInit();
    PSMTXCopy(mtx, mtx2);
    camera = camGetCurPtr();
    PSMTXConcat(camera->view, mtx2, mtx2);
    GXLoadPosMtxImm(mtx2, GX_PNMTX0);
    btlDispTexPlainGX(id, color0, color1, color2, color3);
}

/** Returns the width and height in texels of a texture in the battle menu texture palette. */
void btlDispGetTexSize(u32 id, u16* width, u16* height) {
    GXTexObj texobj;

    TEXGetGXTexObjFromPalette(*_battleWorkPointer->menuTex->data, &texobj, id);
    *width = GXGetTexObjWidth(&texobj);
    *height = GXGetTexObjHeight(&texobj);
}

/**
 * Battle display setup at battle start: the battle cameras, the menu display, and BATTLE_DISP_FLAG_STAR_POINTS,
 * which turns on the Star Point counter.
 */
void btlDispInit(void) {
    BattleWork* wp = _battleWorkPointer;

    battleCameraInit();
    battleMenuDispInit();
    wp->dispFlags |= BATTLE_DISP_FLAG_STAR_POINTS;
}

/**
 * Picks the status pose for a unit: the KO pose when it is inactive,
 * else the first status it has from _status_pose_table (the early demos check sleep before stop and freeze,
 * JP and later check it after), else STATUS_INVALID (the value of POSE_TYPE_NORMAL).
 */
StatusEffectType _GetStatusPoseType(BattleUnit* unit) {
    s32* table = _status_pose_table;
    StatusEffectType type;

    if (unit->attributeFlags & kInactive) {
        return STATUS_INSTANT_KILL;
    }
    while (1) {
        type = *table;
        if (*table == -1) break;
        if (BtlUnit_CheckStatus(unit, type)) {
            return type;
        }
        table++;
    }
    return STATUS_INVALID;
}

static void btlUnitPartsBlurDisp(CameraId camId, void* payload);
static void btlUnitItemDisp2(CameraId camId, void* payload);
static void btlUnitStolenItemDisp(CameraId camId, void* param);

/**
 * Battle per-frame display update for every unit:
 * moves Koops' shell shield after its owner, eases the moveColorLv shading,
 * applies Tiny (down to half size) and Huge (up to double) to the size, runs status effects and icons,
 * chooses each part's pose (status, peril, danger, defend), sets the animation speed for Fast and Slow,
 * picks normal, translucent (invisible) or dodgy draw types, queues the part,
 * blur and shadow draws and held-item icons, then the Star Point counter, stage, command menu,
 * audience and break slot. JP and later drop the shell to the floor height.
 */
void btlDispMain(void) {
    BattleUnit* unit;
    BattleUnitPart* part;
    BattleWork* wp = _battleWorkPointer;
    s32 i;
    const char* poseName;
    BattleUnit* target;
    s32 loop;
    s32 type;
    BattleUnit* partner;
    f32 h;
    f32 scale;
    Vec tpos;
    s32 xlu;
    Vec pos;
    Vec zpos;
    Vec ppos;
    Vec spos;
    Vec vpos;
    Vec viv;
    f32 wx;
    f32 wy;
    f32 wz;
    s8 turns;
    s8 strength;
    s32 k;
    f64 a;

    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(_battleWorkPointer, i);
        if (!unit) continue;
        if (unit->currentKind == UNIT_SHELL_SHIELD && unit->work[SHELL_SHIELD_WORK_FOLLOW] != 0) {
            target = BattleGetUnitPtr(wp, unit->work[SHELL_SHIELD_WORK_TARGET]);
            if (target) {
                if (target->attackEventId && evtCheckID(target->attackEventId)) {
                    if (!(target->attackEventFlags & ACT_EVENT_SHELL_FOLLOWS)) goto next;
                }
                if (target->damageEventId && evtCheckID(target->damageEventId)) goto next;
                BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
                BtlUnit_GetPos(target, &tpos.x, &tpos.y, &tpos.z);
#if VERSION <= VERSION_DEMO_JP
                tpos.z += 5.0f;
#else
                tpos.z += 7.0f;
                h = BattleGetFloorHeight(wp, tpos.x, tpos.y, tpos.z);
#endif
                switch (unit->work[SHELL_SHIELD_WORK_FOLLOW]) {
                    case 2:
                        a = __fabs(pos.x - tpos.x);
                        if (a > 10.0) {
                            if (tpos.x > pos.x) {
                                pos.x = pos.x + ((tpos.x - pos.x) - 10.0f);
                            } else {
                                pos.x = pos.x - ((pos.x - tpos.x) - 10.0f);
                            }
                        }
                        BtlUnit_SetPos(unit, pos.x, tpos.y, tpos.z);
                        break;
                    case 3:
                        if (__fabs(pos.x - tpos.x) < 1.0) {
#if VERSION <= VERSION_DEMO_JP
                            BtlUnit_SetPos(unit, tpos.x, tpos.y, tpos.z);
#else
                            BtlUnit_SetPos(unit, tpos.x, h, tpos.z);
#endif
                            unit->work[SHELL_SHIELD_WORK_FOLLOW] = 1;
                        } else if (tpos.x > pos.x) {
                            pos.x += 1.0f;
#if VERSION <= VERSION_DEMO_JP
                            BtlUnit_SetPos(unit, pos.x, tpos.y, tpos.z);
#else
                            BtlUnit_SetPos(unit, pos.x, h, tpos.z);
#endif
                        } else {
                            pos.x -= 1.0f;
#if VERSION <= VERSION_DEMO_JP
                            BtlUnit_SetPos(unit, pos.x, tpos.y, tpos.z);
#else
                            BtlUnit_SetPos(unit, pos.x, h, tpos.z);
#endif
                        }
                        break;
                    default:
#if VERSION <= VERSION_DEMO_JP
                        BtlUnit_SetPos(unit, tpos.x, tpos.y, tpos.z);
#else
                        BtlUnit_SetPos(unit, tpos.x, h, tpos.z);
#endif
                        break;
                }
            }
        }
next:
        loop = unit->moveColorLvDisp;
        BtlUnit_OffUnitFlag(unit, BATTLE_UNIT_FLAG_MOVE_COLOR_FADING);
        if (loop != unit->moveColorLv) {
            BtlUnit_OnUnitFlag(unit, BATTLE_UNIT_FLAG_MOVE_COLOR_FADING);
            if (unit->moveColorLv > loop) {
                loop += 12;
                if (loop > unit->moveColorLv) loop = unit->moveColorLv;
            } else {
                loop -= 2;
                if (loop < unit->moveColorLv) loop = unit->moveColorLv;
            }
        }
        unit->moveColorLvDisp = loop;
        BtlUnit_HpGaugeMain(unit);
        unit->sizeMultiplier = 1.0f;
        BtlUnit_GetStatus(unit, STATUS_TINY, &turns, &strength);
        if (turns > 0 && strength < 0) {
            scale = 0.5f * strength + 1.0f;
            if (scale < 0.5f) scale = 0.5f;
            unit->sizeMultiplier *= scale;
        }
        BtlUnit_GetStatus(unit, STATUS_HUGE, &turns, &strength);
        if (turns > 0 && strength > 0) {
            scale = 1.0f + strength;
            if (scale > 2.0f) scale = 2.0f;
            unit->sizeMultiplier *= scale;
        }
        BattleStatusEffectMain(unit);
        BattleStatusIconMain(unit);

        for (part = unit->parts; part; part = part->nextPart) {
            if (part->poseId == -1) continue;
            poseName = part->poseName;
            if (!(part->animFlags & PART_ANIM_EVENT_POSE)) {
                type = _GetStatusPoseType(unit);
                if (unit->currentKind >= UNIT_GOOMBA && unit->currentKind < UNIT_UNUSED_TREE) {
                    poseName = searchPoseTbl(part->poseTable, type);
                } else {
                    if (type == POSE_TYPE_NORMAL && !BtlUnit_CheckStatus(unit, STATUS_HOLD_FAST) && (unit->statusFlags & UNIT_STATUS_FLAG_PERIL)) {
                        poseName = BtlUnit_GetPoseNameFromType(part, POSE_TYPE_PERIL);
                        if (!poseName) poseName = searchPoseTbl(part->poseTable, POSE_TYPE_NORMAL);
                    } else if (type == POSE_TYPE_NORMAL && !BtlUnit_CheckStatus(unit, STATUS_HOLD_FAST) && (unit->statusFlags & UNIT_STATUS_FLAG_DANGER)) {
                        poseName = BtlUnit_GetPoseNameFromType(part, POSE_TYPE_DANGER);
                        if (!poseName) poseName = searchPoseTbl(part->poseTable, POSE_TYPE_NORMAL);
                    } else {
                        poseName = searchPoseTbl(part->poseTable, type);
                    }
                    if (BtlUnit_CheckStatusFlag(unit, UNIT_STATUS_FLAG_DEFEND) && BtlUnit_CanActStatus(unit)) {
                        poseName = BtlUnit_GetPoseNameFromType(part, POSE_TYPE_DEFEND);
                    }
                }
                if (BtlUnit_CheckStatus(unit, STATUS_FAST)) {
                    btlDispAnimeSpeed(part, 2.0f);
                } else if (BtlUnit_CheckStatus(unit, STATUS_SLOW)) {
                    btlDispAnimeSpeed(part, 0.5f);
                } else {
                    btlDispAnimeSpeed(part, 1.0f);
                }
            } else {
                btlDispAnimeSpeed(part, 1.0f);
            }
            if (part->animFlags & PART_ANIM_CHANGED) {
                xlu = 0;
                part->animFlags &= ~PART_ANIM_CHANGED;

                // BUG: `&=` clears the restart bit and tests the bits left, so any other set bit restarts the animation
                if (part->animFlags &= ~PART_ANIM_RESTART) {
                    animPoseSetAnim(part->poseId, poseName, TRUE);
                } else {
                    animPoseSetAnim(part->poseId, poseName, FALSE);
                }
                part->poseState = 0;
                if (part->animFlags & PART_ANIM_PERA) {
                    part->look = PART_LOOK_NONE;
                } else {
                    part->look = PART_LOOK_NORMAL;
                    if (BtlUnit_CheckStatus(unit, STATUS_INVISIBLE) || part->color.a != 0xFF) {
                        part->look = PART_LOOK_XLU;
                        xlu = 1;
                    }
                }
                if (!(part->animFlags & PART_ANIM_EVENT_POSE) || (part->animFlags & PART_ANIM_DAMAGE_POSE) || (part->animFlags & PART_ANIM_COMMAND_POSE)) {
                    part->look = PART_LOOK_NORMAL;
                    if (BtlUnit_CheckStatus(unit, STATUS_INVISIBLE) || part->color.a != 0xFF) {
                        part->look = PART_LOOK_XLU;
                        xlu = 1;
                    }
                    if (BtlUnit_CheckStatus(unit, STATUS_DODGY)) {
                        part->look = PART_LOOK_DODGY;
                        xlu = 1;
                    }
                }
                if (xlu && (part->attributes & kNoTranslucentLook)) {
                    xlu = 0;
                    part->look = PART_LOOK_NONE;
                }
                if (!(part->animFlags & PART_ANIM_PERA)) {
                    if (xlu && !(part->animFlags & PART_ANIM_XLU)) {
                        part->animFlags |= PART_ANIM_XLU;
                    } else if (!xlu && (part->animFlags & PART_ANIM_XLU)) {
                        part->animFlags &= ~PART_ANIM_XLU;
                    }
                }
            }
            gravityOffsetControl(part);
            floatOffsetControl(part);
            statusPoseControl(part);
            BtlUnit_GetPartsWorldPos(part, &wx, &wy, &wz);
            animPoseMain(part->poseId);
            zpos = (Vec){wx, wy, wz + part->dispZOffset};
            part->dispColor.a = part->kindPartParams->baseAlpha;
            if (BtlUnit_CheckStatus(unit, STATUS_INVISIBLE) || (unit->attributeFlags & kPermanentlyInvisible)) {
                part->dispColor.a = (part->dispColor.a << 7) / 255;
            }
            part->dispColor.a = part->dispColor.a * part->color.a / 255;
            part->dispColor.r = part->color.r;
            part->dispColor.g = part->color.g;
            part->dispColor.b = part->color.b;
            if (!(unit->attributeFlags & kUnitHidden) && !(part->attributes & kHidden)) {
                if (!(part->attributes & kMainPart) || !(unit->attributeFlags & kVeiled)) {
                    if (part->dispColor.a < 0xFF || animPoseTestXLU(part->poseId)) {
                        if (!(part->animFlags & PART_ANIM_NO_DISP)) {
                            dispEntry(CAMERA_3D, dispCalcZ(&zpos), DISP_MODE_XLU, btlUnitPartsDisp, part);
                        }
                    } else {
                        if (!(part->animFlags & PART_ANIM_NO_DISP)) {
                            dispEntry(CAMERA_3D, dispCalcZ(&zpos), DISP_MODE_ALPHA_TEST, btlUnitPartsDisp, part);
                        }
                    }
                }
                if (part->attributes & kMotionBlur) {
                    dispEntry(CAMERA_3D, wz, DISP_MODE_XLU, btlUnitPartsBlurDisp, part);
                }
            }
            if (!(unit->attributeFlags & kUnitNoShadow) && !(part->attributes & kNoShadow)) {
                BtlUnit_GetPartsWorldPos(part, &ppos.x, &ppos.y, &ppos.z);
                spos = ppos;
                if ((unit->flags & BATTLE_UNIT_FLAG_SHADOW_WITH_PARTNER) && (partner = BattleGetPartnerPtr(wp, unit)) != NULL) {
                    BtlUnit_GetPos(partner, &vpos.x, &vpos.y, &vpos.z);
                    spos.x += vpos.x;
                    spos.x *= 0.5f;
                    spos.y += vpos.y;
                    spos.y *= 0.5f;
                    spos.z += vpos.z;
                    spos.z *= 0.5f;
                }
                spos.y = BattleGetFloorHeight(wp, spos.x, spos.y, spos.z);
                h = spos.y;
                if (ppos.y <= h) {
                    scale = 1.0f;
                } else {
                    scale = (100.0f - (ppos.y - h)) / 100.0f;
                }
                if (scale > 1.0) scale = 1.0f;
                if (scale < 0.1f) scale = 0.1f;
                scale = scale * ((f32)part->shadowSize * unit->mScale.x * part->mScale.x * unit->sizeMultiplier);
                scale = scale * part->shadowScale;
                if ((unit->flags & BATTLE_UNIT_FLAG_SHADOW_WITH_PARTNER) && scale < 16.0f) scale = 16.0f;
                if (scale < 10.0f) scale = 8.0f;
                shadowEntry(spos.x, 10.0f + h, spos.z, scale);
            }

            k = unit->currentKind;

            if (k == UNIT_VIVIAN || k == UNIT_DOOPLISS_CHAPTER8_VIVIAN || k == UNIT_MARILYN_CHAPTER8
                || k == UNIT_BELDAM_CHAPTER2 || k == UNIT_MARILYN_CHAPTER2 || k == UNIT_VIVIAN_CHAPTER2
                || k == UNIT_UNUSED_BELDAM2 || k == UNIT_UNUSED_MARILYN2 || k == UNIT_UNUSED_VIVIAN2
                || k == UNIT_UNUSED_BELDAM3 || k == UNIT_UNUSED_MARILYN3
                || k == UNIT_BELDAM_CHAPTER8) {
                viv.x = unit->work[SHADOW_TAIL_WORK_X];
                viv.y = unit->work[SHADOW_TAIL_WORK_Y];
                viv.z = unit->work[SHADOW_TAIL_WORK_Z];
                animPoseVivianMain(part->poseId, &viv);
            }
            BtlUnit_ControlPoseSoundMain(part);
        }
        if (unit->heldItem) {
            if ((unit->flags & BATTLE_UNIT_FLAG_STOLEN_ITEM) && !(unit->flags & BATTLE_UNIT_FLAG_HIDE_HELD_ITEM)) {
                dispEntry(CAMERA_3D, 0.0f, DISP_MODE_XLU, btlUnitStolenItemDisp, unit);
            } else if ((!(unit->flags & BATTLE_UNIT_FLAG_ACTING) || (unit->flags & BATTLE_UNIT_FLAG_SHOW_ITEM_WHILE_ACTING)) && !(unit->flags & BATTLE_UNIT_FLAG_HIDE_HELD_ITEM)) {
                dispEntry(CAMERA_3D, 0.0f, DISP_MODE_XLU, (DispCallback)btlUnitItemDisp, unit);
            }
        } else if (wp->flags & BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED) {
            dispEntry(CAMERA_3D, 0.0f, DISP_MODE_XLU, btlUnitItemDisp2, unit);
        }
    }
    BattleStockExpDisp();
    BattleStageDisp();
    BattleCommandDisplay(wp);
    BattleAudience_Disp();
    BattleBreakSlot_Disp();
    if (wp->flags & BATTLE_FLAG_FORCE_STATUS_ICONS_UNUSED) wp->flags &= ~BATTLE_FLAG_FORCE_STATUS_ICONS_UNUSED;
    if (wp->flags & BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED) wp->flags &= ~BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED;
}

/**
 * Draw callback for one unit part: builds the model matrix from the unit's and part's position,
 * rotation (in the part's rotation order), scale and size multiplier,
 * mirrors it when the part faces away from the camera,
 * then draws the pose tinted by its colour (black when blackened, green-tinted for poison,
 * dimmed by moveColorLv) with the Charge shine. Also records the matrix for the blur trail.
 */
void btlUnitPartsDisp(CameraId camId, void* param) {
    Mtx uMtx;
    Mtx drawMtx;
    Mtx pS2;
    Mtx blurMtx;
    Mtx uM;
    Mtx pPos2;
    Mtx pTN2;
    Mtx pT2;
    Mtx pRx2;
    Mtx pRy2;
    Mtx pRz2;
    Mtx pM;

    // FAKE: uPos is written by PSMTXTrans and never read; removing it and the call changes the object
    Mtx uPos;
    Mtx uDisp;
    Mtx uRx;
    Mtx uRy;
    Mtx uRz;
    Mtx uTN;
    Mtx uT;
    Mtx uS;
    Mtx pPos;
    Mtx pDisp;
    Mtx pRx;
    Mtx pRy;
    Mtx pRz;
    Mtx pTN;
    Mtx pT;
    Mtx pS;
    Mtx mT5;
    BattleWork* wp;
    BattleUnitPart* part;
    BattleUnit* unit;
    s32 gz;
    CameraEntry* cam;
    f32 rotY;
    f32 ex;
    f32 ey;
    f32 ez;
    f32 x, y, z;
    f32 angle;
    f32 dir;
    f32 camAngle;
    f32 a;
    f32 zscale;
    GXColor* color;
    part = param;
    wp = _battleWorkPointer;
    zscale = 1.0f;
    unit = part->owner;
    if (part->poseId < 0) return;

    // FAKE: dead int guard (COLOURING W). part becomes a multi-def named local (not folded back into the param), so it
    // numbers above unit, and gz adds wp's missing neighbour; the arm is deleted after colouring and leaves no code.
    gz = 0;
    if (gz != 0) part = (BattleUnitPart*)unit;

    PSMTXTrans(uTN, -unit->rotationOffset.x, -unit->rotationOffset.y, -unit->rotationOffset.z);
    PSMTXTrans(uT, unit->rotationOffset.x, unit->rotationOffset.y, unit->rotationOffset.z);
    PSMTXTrans(uDisp, unit->displayOffset.x, unit->displayOffset.y, unit->displayOffset.z);
    PSMTXTrans(uPos, unit->position.x, unit->position.y, unit->position.z);
    PSMTXRotRad(uRx, 'x', 0.017453292f * (unit->rotation.x + unit->baseRotation.x));
    PSMTXRotRad(uRy, 'y', 0.017453292f * (unit->rotation.y + unit->baseRotation.y));
    PSMTXRotRad(uRz, 'z', 0.017453292f * (unit->rotation.z + unit->baseRotation.z));
    PSMTXScale(uS, unit->sizeMultiplier * (unit->baseScale.x * unit->mScale.x), unit->sizeMultiplier * (unit->baseScale.y * unit->mScale.y), unit->baseScale.z * unit->mScale.z);
    PSMTXIdentity(uM);
    PSMTXConcat(uS, uM, uM);
    PSMTXConcat(uTN, uM, uM);
    PSMTXConcat(uRy, uM, uM);
    PSMTXConcat(uRz, uM, uM);
    PSMTXConcat(uRx, uM, uM);
    PSMTXConcat(uT, uM, uMtx);
    PSMTXConcat(uDisp, uMtx, uMtx);

    angle = 270.0f;
    if (part->attributes & kAbsolutePosition) {
        if (part->faceDirection >= 0) angle = 90.0f;
        x = part->position.x;
        y = part->position.y + getGravityDispOffset(part) + getFloatDispOffset(part);
        z = part->position.z;
        cam = camGetCurPtr();
        a = angleABf(cam->cameraPos.x, cam->cameraPos.z, cam->cameraAt.x, cam->cameraAt.z);
        dir = reviseAngle(a - angleABf(cam->cameraPos.x, cam->cameraPos.z, x, z));
        PSMTXTrans(pTN2, -part->rotationOffset.x, -part->rotationOffset.y, -part->rotationOffset.z);
        PSMTXTrans(pT2, part->rotationOffset.x, part->rotationOffset.y, part->rotationOffset.z);
        PSMTXRotRad(pRx2, 'x', 0.017453292f * (part->rotation.x + part->baseRotation.x));
        PSMTXRotRad(pRy2, 'y', 0.017453292f * (dir + (part->rotation.y + part->baseRotation.y)));
        PSMTXRotRad(pRz2, 'z', 0.017453292f * (part->rotation.z + part->baseRotation.z));
        PSMTXTrans(pPos2, x, y, z);
        rotY = reviseAngle(-1.0f * angle - 90.0f);
        z = reviseAngle(part->baseRotation.y + (rotY + part->rotation.y));
        cam = camGetCurPtr();
        camAngle = angleABf(cam->cameraPos.x, cam->cameraPos.z, cam->cameraAt.x, cam->cameraAt.z);
        a = reviseAngle(z + camAngle);
        if (a >= 90.0f && a <= 270.0f) {
            zscale *= -1.0f;
            PSMTXRotRad(pRy2, 'y', 0.017453292f * ((dir + (part->rotation.y + part->baseRotation.y)) - 2.0f * (180.0f - (a - camAngle))));
        }
        if (part->attributes & kIgnoreUnitSize) {
            PSMTXScale(pS2, part->baseScale.x * part->mScale.x, part->baseScale.y * part->mScale.y, zscale * (part->baseScale.z * part->mScale.z));
        } else {
            PSMTXScale(pS2, unit->sizeMultiplier * (part->baseScale.x * part->mScale.x), unit->sizeMultiplier * (part->baseScale.y * part->mScale.y), zscale * (part->baseScale.z * part->mScale.z));
        }
        PSMTXIdentity(pM);
        PSMTXConcat(pS2, pM, pM);
        PSMTXConcat(pTN2, pM, pM);
        PSMTXConcat(pRz2, pM, pM);
        PSMTXConcat(pRx2, pM, pM);
        PSMTXConcat(pRy2, pM, pM);
        PSMTXConcat(pT2, pM, pM);
        PSMTXConcat(pPos2, pM, drawMtx);
        PSMTXCopy(drawMtx, blurMtx);
        PSMTXTrans(mT5, 0.0f, 0.0f, -5.0f);
        PSMTXConcat(mT5, blurMtx, blurMtx);
    } else {
        if (unit->faceDirection >= 0) angle = 90.0f;
        dir = part->faceDirection;
        if (dir > 0.0f) {
            dir = 1.0f;
        } else if (dir < 0.0f) {
            dir = -1.0f;
        } else {
            dir = wp->alliances[unit->alliance].attackDirection;
        }
        ex = part->positionOffset.x * dir + unit->position.x;
        ey = unit->position.y + part->positionOffset.y + getGravityDispOffset(part) + getFloatDispOffset(part);
        ez = unit->position.z + part->positionOffset.z;
        dir = 0.0f;
        if (part->owner->attributeFlags & kUnitFaceCamera) {
            cam = camGetCurPtr();
            a = angleABf(cam->cameraPos.x, cam->cameraPos.z, cam->cameraAt.x, cam->cameraAt.z);
            dir = reviseAngle(a - angleABf(cam->cameraPos.x, cam->cameraPos.z, ex, ez));
        }
        PSMTXTrans(pTN, -part->rotationOffset.x, -part->rotationOffset.y, -part->rotationOffset.z);
        PSMTXTrans(pT, part->rotationOffset.x, part->rotationOffset.y, part->rotationOffset.z);
        PSMTXRotRad(pRx, 'x', 0.017453292f * (part->rotation.x + part->baseRotation.x));
        PSMTXRotRad(pRy, 'y', 0.017453292f * (dir + (part->rotation.y + part->baseRotation.y)));
        PSMTXRotRad(pRz, 'z', 0.017453292f * (part->rotation.z + part->baseRotation.z));
        PSMTXTrans(pPos, ex, ey, ez);
        PSMTXTrans(pDisp, part->displayOffset.x, part->displayOffset.y, part->displayOffset.z);
        rotY = reviseAngle(-1.0f * angle - 90.0f);
        if (!(part->owner->flags & BATTLE_UNIT_FLAG_SOLID_MODEL)) {
            a = reviseAngle(rotY + unit->rotation.y + part->rotation.y + unit->baseRotation.y + part->baseRotation.y);
            cam = camGetCurPtr();
            a = reviseAngle(a + angleABf(cam->cameraPos.x, cam->cameraPos.z, cam->cameraAt.x, cam->cameraAt.z));
            if (a >= 90.0f && a <= 270.0f) {
                PSMTXScale(uS, unit->sizeMultiplier * (unit->baseScale.x * unit->mScale.x), unit->sizeMultiplier * (unit->baseScale.y * unit->mScale.y), -1.0f * (unit->baseScale.z * unit->mScale.z));
                PSMTXIdentity(uM);
                PSMTXConcat(uS, uM, uM);
                PSMTXConcat(uTN, uM, uM);
                switch (part->rotationOrder) {
                    case 0:
                        PSMTXConcat(uRy, uM, uM);
                        PSMTXConcat(uRz, uM, uM);
                        PSMTXConcat(uRx, uM, uM);
                        break;
                    case 1:
                        PSMTXConcat(uRy, uM, uM);
                        PSMTXConcat(uRx, uM, uM);
                        PSMTXConcat(uRz, uM, uM);
                        break;
                    case 2:
                        PSMTXConcat(uRx, uM, uM);
                        PSMTXConcat(uRy, uM, uM);
                        PSMTXConcat(uRz, uM, uM);
                        break;
                    case 3:
                        PSMTXConcat(uRx, uM, uM);
                        PSMTXConcat(uRz, uM, uM);
                        PSMTXConcat(uRy, uM, uM);
                        break;
                    case 4:
                        PSMTXConcat(uRz, uM, uM);
                        PSMTXConcat(uRx, uM, uM);
                        PSMTXConcat(uRy, uM, uM);
                        break;
                    case 5:
                        PSMTXConcat(uRz, uM, uM);
                        PSMTXConcat(uRy, uM, uM);
                        PSMTXConcat(uRx, uM, uM);
                        break;
                }
                PSMTXConcat(uT, uM, uMtx);
                PSMTXConcat(uDisp, uMtx, uMtx);
            }
        }
        PSMTXScale(pS, part->baseScale.x * part->mScale.x, part->baseScale.y * part->mScale.y, zscale * (part->baseScale.z * part->mScale.z));
        PSMTXIdentity(uM);
        PSMTXConcat(uMtx, uM, uM);
        PSMTXConcat(pS, uM, uM);
        PSMTXConcat(pTN, uM, uM);
        switch (part->rotationOrder) {
            case 0:
                PSMTXConcat(pRy, uM, uM);
                PSMTXConcat(pRz, uM, uM);
                PSMTXConcat(pRx, uM, uM);
                break;
            case 1:
                PSMTXConcat(pRy, uM, uM);
                PSMTXConcat(pRx, uM, uM);
                PSMTXConcat(pRz, uM, uM);
                break;
            case 2:
                PSMTXConcat(pRx, uM, uM);
                PSMTXConcat(pRy, uM, uM);
                PSMTXConcat(pRz, uM, uM);
                break;
            case 3:
                PSMTXConcat(pRx, uM, uM);
                PSMTXConcat(pRz, uM, uM);
                PSMTXConcat(pRy, uM, uM);
                break;
            case 4:
                PSMTXConcat(pRz, uM, uM);
                PSMTXConcat(pRx, uM, uM);
                PSMTXConcat(pRy, uM, uM);
                break;
            case 5:
                PSMTXConcat(pRz, uM, uM);
                PSMTXConcat(pRy, uM, uM);
                PSMTXConcat(pRx, uM, uM);
                break;
        }
        PSMTXConcat(pT, uM, uM);
        PSMTXConcat(pPos, uM, drawMtx);
        PSMTXConcat(pDisp, drawMtx, drawMtx);
        PSMTXCopy(drawMtx, blurMtx);
        PSMTXTrans(mT5, 0.0f, 0.0f, -5.0f);
        PSMTXConcat(mT5, blurMtx, blurMtx);
    }

    color = &part->drawColor;
    *color = part->dispColor;
    if (part->animFlags & PART_ANIM_BURNT) {
        color->r = 0;
        color->g = 0;
        color->b = 0;
    }
    if (part->animFlags & PART_ANIM_INKED) {
        color->r = 0;
        color->g = 0;
        color->b = 0;
    }
    if (color->r == 0xFF && color->g == 0xFF && color->b == 0xFF) {
        if (BtlUnit_CheckStatus(unit, STATUS_POISON)) {
            color->r = color->r * 160 / 255;

            // BUG: scales the already reduced r (160), so b becomes 100, not 160.
            color->b = color->r * 160 / 255;
        }
        color->r = color->r * unit->moveColorLvDisp / 255;
        color->g = color->g * unit->moveColorLvDisp / 255;
        color->b = color->b * unit->moveColorLvDisp / 255;
    }
    animPoseSetMaterialEvtColor(part->poseId, *color);
    animPoseSetMaterialFlagOn(part->poseId, MAP_OBJ_FLAG_COLOR);
    if (part->dispCallback) {
        part->dispCallback(part, 0);
    }
    if (BtlUnit_CheckStatus(unit, STATUS_CHARGE)) {
        animPoseSetMaterialFlagOn(part->poseId, MAP_OBJ_FLAG_WAVE_TEX);
    } else {
        animPoseSetMaterialFlagOff(part->poseId, MAP_OBJ_FLAG_WAVE_TEX);
    }
    animPoseDrawMtx(part->poseId, drawMtx, 1, rotY, 2.0f);
    animPoseDrawMtx(part->poseId, drawMtx, 2, rotY, 2.0f);
    animPoseDrawMtx(part->poseId, drawMtx, 3, rotY, 2.0f);
    if (part->dispCallback) {
        part->dispCallback(part, 1);
    }
    btlUnitPartsBlurControl(part, 0, blurMtx, *color, rotY);
}

// FAKE: per-function opt_dead_assignments off keeps the unused height store; without it the object changes
#pragma push
// Dead-assignment optimisation off for btlUnitItemDisp only (until the pop): the original register allocation needs it.
#pragma opt_dead_assignments off
static void btlUnitItemDisp(CameraId camId, BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    s32 item;
    f32 height;
    Vec pos;

    height = BtlUnit_GetHeight(unit);

    item = BtlUnit_EnemyItemCanUseCheck(unit->heldItem);
    if ((wp->flags & BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED) && !item) {
        item = ITEM_MUSHROOM;
    }
    if (item) {
        BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
        pos.x += unit->heldItemBaseOffset.x;
        pos.y += unit->heldItemBaseOffset.y;
        pos.z += unit->heldItemBaseOffset.z;
        pos.x += unit->heldItemOffset.x;
        pos.y += unit->heldItemOffset.y;
        pos.z += unit->heldItemOffset.z;

        // FAKE: float-first cast of iconDispGx; the plain call changes the object
        ((void (*)(f32, Vec*, s16, u16))iconDispGx)(1.0f, &pos, 0, itemDataTable[item].icon_id);
    }
}
#pragma pop

static void btlUnitItemDisp2(CameraId camId, void* payload) {
    BattleUnit* unit = payload;
    BattleWork* wp = _battleWorkPointer;
    s32 kind = unit->currentKind;

    if (!((kind >= UNIT_GOOMBA && kind < UNIT_UNUSED_TREE) || (kind >= UNIT_MARIO && kind < TYPE_PARTNER_MIN) ||
          (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX))) {
        return;
    }
    if (wp->flags & BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED) {
        btlUnitItemDisp(camId, unit);
    } else {
        wp->flags |= BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED;
        btlUnitItemDisp(camId, unit);
        wp->flags &= ~BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED;
    }
}

/** Draws an item stolen from Mario (by a Bandit or similar) floating above the thief's head. */
static void btlUnitStolenItemDisp(CameraId camId, void* param) {
    BattleUnit* unit = param;
    Vec pos;
    s32 item = unit->heldItem;

    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.y += 5.0f + unit->sizeMultiplier * (f32)BtlUnit_GetHeight(unit);

    // FAKE: float-first cast of iconDispGx; the plain call changes the object
    ((void (*)(f32, Vec*, s16, u16))iconDispGx)(1.0f, &pos, 0, itemDataTable[item].icon_id);
}

static void btlUnitPartsBlurControl(BattleUnitPart* part, s32 unused, Mtx orientation, GXColor color, f32 rotation) {
    if (part->attributes & kMotionBlur) {
        _partsBlurControl(part, 1, orientation, color, rotation);
    } else {
        _partsBlurControl(part, 0, orientation, color, rotation);
    }
}

/**
 * Shifts the part's 10-entry blur history down one and stores this frame's matrix,
 * colour and blur colours in entry 0.
 */
static void _partsBlurControl(BattleUnitPart* part, s32 flags, Mtx orientation, GXColor color, f32 rotation) {
    BattleWorkUnitPartBlur* blur;
    int i;

    blur = &part->blurWork;
    for (i = 9; i > 0; i--) { // shift all slots forward one
        blur->work[i] = blur->work[i - 1];
    }
    blur->work[0].flags = flags;
    MTXCopy(orientation, blur->work[0].orientation);
    blur->work[0].rotation = rotation;
    blur->work[0].base = color;
    blur->work[0].blurColor[0] = blur->color[0];
    blur->work[0].blurColor[1] = blur->color[1];
    if ((blur->flags & 1) && (blur->work[1].flags & 1)) {
        blur->work[0].flags &= ~1;
    }
}

/**
 * Draws the part's afterimage trail: each history entry is redrawn with the base colour faded between the two
 * blur colours along the trail.
 */
static void btlUnitPartsBlurDisp(CameraId camId, void* payload) {
    BattleUnitPart* part = payload;
    BattleWorkUnitPartBlur* blur;
    BattleWorkUnitPartBlurEntry* entry;
    GXColor color;
    GXColor* c;
    s32 i;

    blur = &part->blurWork;
    for (i = 0; i < 10; i++) {
        entry = &blur->work[i];
        color = entry->base;
        if (entry->flags & 1) {
            c = entry->blurColor;
            color.r = color.r * (u8)((c[0].r * (10 - i)) / 10 + (c[1].r * i) / 10) / 255;
            color.g = color.g * (u8)((c[0].g * (10 - i)) / 10 + (c[1].g * i) / 10) / 255;
            color.b = color.b * (u8)((c[0].b * (10 - i)) / 10 + (c[1].b * i) / 10) / 255;
            color.a = color.a * (u8)((c[0].a * (10 - i)) / 10 + (c[1].a * i) / 10) / 255;
            if (color.a) {
                animPoseSetMaterialEvtColor(part->poseId, color);
                animPoseSetMaterialFlagOn(part->poseId, MAP_OBJ_FLAG_COLOR);
                animPoseDrawMtx(part->poseId, entry->orientation, 1, entry->rotation, 2.0f);
                animPoseDrawMtx(part->poseId, entry->orientation, 2, entry->rotation, 2.0f);
                animPoseDrawMtx(part->poseId, entry->orientation, 3, entry->rotation, 2.0f);
            }
        }
    }
}

/**
 * Creates the animation pose for every part of a unit from its model name, starting in its normal pose;
 * parts without a pose table or model get poseId -1.
 */
void btlDispEntAnime(BattleUnit* unit) {
    BattleUnitPart* part;
    const char* name;

    for (part = unit->parts; part; part = part->nextPart) {
        if (!part->poseTable) {
            part->poseId = -1;
        } else {
            name = part->kindPartParams->modelName;
            if (!name) {
                part->poseId = -1;
            } else {
                strcpy(part->poseName, searchPoseTbl(part->poseTable, POSE_TYPE_NORMAL));
                part->poseId = animPoseEntry(name, 1);
            }
        }
    }
}

/**
 * Plays a named animation on a part, taking manual control of its pose (status poses no longer override it).
 * damagePose marks it as a hurt pose, which keeps the status draw types.
 */
void btlDispChangeAnime(BattleUnitPart* part, const char* name, BOOL damagePose) {
    strcpy(part->poseName, name);
    part->animFlags |= PART_ANIM_EVENT_POSE;
    part->animFlags |= PART_ANIM_CHANGED;
    part->animFlags &= ~PART_ANIM_COMMAND_POSE;
    part->animFlags &= ~PART_ANIM_RESTART;
    part->animFlags &= ~PART_ANIM_BURNT;
    if (damagePose) {
        part->animFlags |= PART_ANIM_DAMAGE_POSE;
    } else {
        part->animFlags &= ~PART_ANIM_DAMAGE_POSE;
    }
}

/** Plays a paper effect animation (pera) on a part, for flattened and similar effects, with the plain draw type. */
void btlDispChangePeraAnime(BattleUnitPart* part, const char* name) {
    part->animFlags |= PART_ANIM_PERA;
    part->look = PART_LOOK_NONE;
    animPoseSetEffectAnim(part->poseId, name, 1);
}

/**
 * Hands a part's pose back to automatic status-based selection, clearing manual, blackened,
 * command-wait and pera states.
 */
void btlDispPoseAnime(BattleUnitPart* part) {
    part->animFlags &= ~(PART_ANIM_EVENT_POSE | PART_ANIM_BURNT);
    part->animFlags |= PART_ANIM_CHANGED;
    part->animFlags &= ~PART_ANIM_COMMAND_POSE;
    part->animFlags &= ~PART_ANIM_PERA;
}

/**
 * Advances the zero-gravity bob timer of a part
 * while the battle is in zero gravity (and the unit does not float on its own),
 * and eases it back to rest when it ends; the cycle is 120 frames.
 */
static void gravityOffsetControl(BattleUnitPart* part) {
    BOOL active = TRUE;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = part->owner;

    if (!(wp->flags & BATTLE_FLAG_ZERO_GRAVITY_UNUSED)) {
        active = FALSE;
    } else if (unit->attributeFlags & (kFloating | kNoGravityBob)) {
        active = FALSE;
    }
    if (active) {
        part->gravityTimer++;
    } else if (part->gravityTimer) {
        part->gravityTimer %= 120;
        if (part->gravityTimer >= 60) {
            part->gravityTimer++;
        } else if (part->gravityTimer) {
            part->gravityTimer--;
        }
    }
}

/** Vertical display offset from zero gravity: 0 to 8 units, following the bob timer. */
f32 getGravityDispOffset(BattleUnitPart* part) {
    return 4.0f * sinfd((part->gravityTimer * 3 + 270) % 360) + 4.0f;
}

static void floatOffsetControl(BattleUnitPart* part) {
    BOOL active = TRUE;
    BattleUnit* unit = part->owner;

    if (!(unit->flags & BATTLE_UNIT_FLAG_FLOAT_CAPABLE)) {
        active = FALSE;
    } else if (!(unit->flags & BATTLE_UNIT_FLAG_FLOAT_ACTIVE)) {
        active = FALSE;
    } else if (BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
        active = FALSE;
    }
    if (active) {
        part->floatTimer++;
    } else if (part->floatTimer) {
        part->floatTimer %= 90;
        if (part->floatTimer >= 45) {
            part->floatTimer++;
        } else if (part->floatTimer) {
            part->floatTimer--;
        }
    }
}

/** Vertical display offset from floating: 0 to 4 units, following the float timer. */
f32 getFloatDispOffset(BattleUnitPart* part) {
    return 2.0f * sinfd((part->floatTimer * 4 + 270) % 360) + 2.0f;
}

/**
 * Runs the per-frame effect animation for the part's draw type:
 * one-shot effects for types 2, 3 and 7, cycling effects for 4 to 6, default (no effect) otherwise.
 */
static void statusPoseControl(BattleUnitPart* part) {
    switch (part->look) {
        case PART_LOOK_NONE:
            break;
        case PART_LOOK_PM_C_UNUSED:
        case PART_LOOK_PM_F_UNUSED:
        case PART_LOOK_XLU:
            _pose_one_pattern(part);
            break;
        case PART_LOOK_DODGY:
        case PART_LOOK_PM_C2_UNUSED:
        case PART_LOOK_PM_F2_UNUSED:
            _pose_two_pattern(part);
            break;
        default:
            _pose_def(part);
            break;
    }
}

/** Clears the part's effect animation once; paper-craft units (no backface flip) also get the paper-craft light. */
static void _pose_def(BattleUnitPart* part) {
    switch (part->poseState) {
        case 0:
            part->poseState = 1;
            animPoseSetEffect(part->poseId, NULL, 0);
            if (part->owner->flags & BATTLE_UNIT_FLAG_SOLID_MODEL) {
                animPoseSetMaterialLightFlagOn(part->poseId, MATERIAL_LIGHT_PAPER_CRAFT);
            }
            break;
    }
}

/**
 * Starts a one-shot "p_b_st" effect animation for the draw type: PM_Y_1A for translucent (type 7).
 * Types 2 and 3 (PM_C_1A, PM_F_1A) are never set:
 * the draw code and btlevtcmd only give a part types 0, 1, 4 and 7.
 * Guess: C and F are a stone-like and a frozen look, from the effect names.
 */
static void _pose_one_pattern(BattleUnitPart* part) {
    switch (part->poseState) {
        case 0:
            part->poseState = 1;
            switch (part->look) {
                case PART_LOOK_PM_C_UNUSED:
                    animPoseSetEffect(part->poseId, "p_b_st", 1);
                    animPoseSetEffectAnim(part->poseId, "PM_C_1A", 1);
                    break;
                case PART_LOOK_PM_F_UNUSED:
                    animPoseSetEffect(part->poseId, "p_b_st", 1);
                    animPoseSetEffectAnim(part->poseId, "PM_F_1A", 1);
                    break;
                case PART_LOOK_XLU:
                    animPoseSetEffect(part->poseId, "p_b_st", 1);
                    animPoseSetEffectAnim(part->poseId, "PM_Y_1A", 1);
                    break;
            }
            break;
    }
}

/**
 * Cycles a "p_b_st" effect animation: type 4 (Dodgy) replays PM_Y_1B every 2.2 seconds;
 * types 5 and 6 hold PM_C_1A or PM_F_2 for a random while,
 * then play PM_C_1B or PM_F_1B for 22 frames and start over.
 */
static void _pose_two_pattern(BattleUnitPart* part) {
    switch (part->poseState) {
        case 0:
            part->poseState = 1;
            switch (part->look) {
                case PART_LOOK_DODGY:
                    animPoseSetEffect(part->poseId, "p_b_st", 1);
                    animPoseSetEffectAnim(part->poseId, "PM_Y_1B", 1);
                    part->poseTimer = sysMsec2Frame(2200);
                    break;
                case PART_LOOK_PM_C2_UNUSED:
                    animPoseSetEffect(part->poseId, "p_b_st", 1);
                    animPoseSetEffectAnim(part->poseId, "PM_C_1A", 1);
                    part->poseTimer = (irand(4) + 6) * sysMsec2Frame(2200);
                    break;
                case PART_LOOK_PM_F2_UNUSED:
                    animPoseSetEffect(part->poseId, "p_b_st", 1);
                    animPoseSetEffectAnim(part->poseId, "PM_F_2", 1);
                    part->poseTimer = (irand(4) + 7) * 22;
                    break;
            }
            break;
        case 1:
            switch (part->look) {
                case PART_LOOK_DODGY:
                    part->poseTimer--;
                    if (part->poseTimer == 0) {
                        animPoseSetEffectAnim(part->poseId, "PM_Y_1B", 1);
                        part->poseTimer = sysMsec2Frame(2200);
                    }
                    break;
                case PART_LOOK_PM_C2_UNUSED:
                    part->poseTimer--;
                    if (part->poseTimer == 0) {
                        part->poseState = 2;
                        animPoseSetEffectAnim(part->poseId, "PM_C_1B", 1);
                        part->poseTimer = 22;
                    }
                    break;
                case PART_LOOK_PM_F2_UNUSED:
                    part->poseTimer--;
                    if (part->poseTimer == 0) {
                        part->poseState = 2;
                        animPoseSetEffectAnim(part->poseId, "PM_F_1B", 1);
                        part->poseTimer = 22;
                    }
                    break;
            }
            break;
        case 2:
            switch (part->look) {
                case PART_LOOK_DODGY:
                    part->poseTimer--;
                    if (part->poseTimer == 0) {
                        part->poseState = 0;
                        _pose_two_pattern(part);
                    }
                    break;
                case PART_LOOK_PM_C2_UNUSED:
                    part->poseTimer--;
                    if (part->poseTimer == 0) {
                        part->poseState = 0;
                        _pose_two_pattern(part);
                    }
                    break;
                case PART_LOOK_PM_F2_UNUSED:
                    part->poseTimer--;
                    if (part->poseTimer == 0) {
                        part->poseState = 0;
                        _pose_two_pattern(part);
                    }
                    break;
            }
            break;
    }
}

// FAKE: unused constant local kept by a per-function opt_dead_assignments off; without them the object changes
#pragma push
// dead assignments optimisation off for btlDispAnimeSpeed only (until the pop): the original register allocation and scheduling need it
#pragma opt_dead_assignments off
/** Sets a part's status animation speed (2 for Fast, 0.5 for Slow) and applies it times its scripted speed. */
void btlDispAnimeSpeed(BattleUnitPart* part, f32 speed) {
    f32 unusedOne = 1.0f;
    part->animSpeed = speed;
    animPoseSetLocalTimeRate(part->poseId, part->animSpeed * part->animSpeed2);
}
#pragma pop

/** Projects a 3D battle position to 2D screen coordinates (the 608x480 2D camera space centred on the screen). */
void btlGetScreenPoint(Vec* in, Vec* out) {
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    Vec v;

    v.x = in->x;
    v.y = in->y;
    v.z = in->z;

    PSMTXMultVec(camera->view, &v, &v);
    PSMTX44MultVec(camera->projection, &v, &v);
    out->x = 304.0f * v.x;
    out->y = 240.0f * v.y;
    out->z = 0.0f;
}

/**
 * Draws a battle menu texture four times, mirrored into the four quadrants around pos,
 * so one corner texture makes a full symmetrical sight;
 * used by aiming action commands (Tattle and shot targets).
 */
void btlDispTex4(u32 id, Vec pos, Vec scale, Vec rot, GXColor color) {
    u16 width, height;
    f32 hw, hh;
    Vec off;

    off.z = 0.0f;

    btlDispGetTexSize(id, &width, &height);
    hw = 0.5f * ((f32)width * scale.x);
    hh = 0.5f * ((f32)height * scale.y);
    off.x = -hw;
    off.y = hh;
    _btlDispTex4(id, pos, scale, rot, off, (Vec){1.0f, 1.0f, 1.0f}, color);
    off.x = hw;
    off.y = hh;
    _btlDispTex4(id, pos, scale, rot, off, (Vec){-1.0f, 1.0f, 1.0f}, color);
    off.x = hw;
    off.y = -hh;
    _btlDispTex4(id, pos, scale, rot, off, (Vec){-1.0f, -1.0f, 1.0f}, color);
    off.x = -hw;
    off.y = -hh;
    _btlDispTex4(id, pos, scale, rot, off, (Vec){1.0f, -1.0f, 1.0f}, color);
}

/**
 * Draws one quadrant of btlDispTex4: scaled, mirrored by scale2, offset by pos2,
 * then rotated (Z, X, Y) and placed at pos.
 */
static void _btlDispTex4(u32 id, Vec pos, Vec scale, Vec rot, Vec pos2, Vec scale2, GXColor color) {
    Mtx mtx, trans, scl, rotX, rotY, rotZ, trans2, scl2;

    PSMTXIdentity(mtx);
    PSMTXTrans(trans, pos.x, pos.y, pos.z);
    PSMTXTrans(trans2, pos2.x, pos2.y, pos2.z);
    PSMTXScale(scl, scale.x, scale.y, scale.z);
    PSMTXScale(scl2, scale2.x, scale2.y, scale2.z);
    PSMTXRotRad(rotX, 'x', 0.017453292f * rot.x);
    PSMTXRotRad(rotY, 'y', 0.017453292f * rot.y);
    PSMTXRotRad(rotZ, 'z', 0.017453292f * rot.z);
    PSMTXConcat(scl, mtx, mtx);
    PSMTXConcat(scl2, mtx, mtx);
    PSMTXConcat(trans2, mtx, mtx);
    PSMTXConcat(rotZ, mtx, mtx);
    PSMTXConcat(rotX, mtx, mtx);
    PSMTXConcat(rotY, mtx, mtx);
    PSMTXConcat(trans, mtx, mtx);
    btlDispTexPlane2(mtx, id, color, 0);
}

/**
 * Counts the displayed Star Points up towards the ones earned
 * so far this battle (one per frame after a 4-frame delay) and queues the counter draw
 * while BATTLE_DISP_FLAG_STAR_POINTS is on.
 */
static void BattleStockExpDisp(void) {
    BattleWork* wp = _battleWorkPointer;

    if (wp->stockExp > wp->stockExpDisp) {
        wp->stockExpTimer++;
        if (wp->stockExpTimer >= 4) {
            wp->stockExpDisp++;
        }
    }
    if (wp->dispFlags & BATTLE_DISP_FLAG_STAR_POINTS) {
        dispEntry(CAMERA_2D, 901.0f, DISP_MODE_ALPHA_TEST, _btlStockExpDisp, NULL);
    }
}

/**
 * Draws the earned Star Points in the lower right as star icons:
 * a large one per ten and a small one per unit, up to 100.
 */
static void _btlStockExpDisp(CameraId camId, void* param) {
    static f32 y;
    static f32 x;
    s32 exp;
    s32 tens;
    s32 ones;
    s32 i;
    s32 n;

    exp = _battleWorkPointer->stockExpDisp;
    if (exp > 0) {
        n = exp;
        if (exp > 100) {
            n = 100;
        }
        tens = n / 10;
        ones = n - tens * 10;
        for (i = 0; i < ones; i++) {
            Vec pos = {0.0f, 0.0f, -100.0f};
            pos.x = 308.0f + x - 3.5f - 13.0f * i;
            pos.y = y - 220.0f;
            iconDispGx(&pos, ICON_FLAG_2D, ICON_STAR_POINT, 0.35f);
        }
        for (i = 0; i < tens; i++) {
            Vec pos = {0.0f, 0.0f, -100.0f};
            pos.x = 4.0f + (308.0f + x - 10.0f - 20.0f * i);
            pos.y = 16.0f + (y - 220.0f);
            iconDispGx(&pos, ICON_FLAG_2D, ICON_STAR_POINT, 0.5f);
        }
    }
}
