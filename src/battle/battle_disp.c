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


extern ItemData itemDataTable[];

#define PART_GRAVITY(p) ((p)->gravityTimer)
#define PART_FLOAT(p) ((p)->floatTimer)
#define PART_POSENAME(p) ((p)->poseName)
#define PART_ANIMFLAGS(p) ((p)->animFlags)
#define PART_ANIMSPEED(p) ((p)->animSpeed)
#define PART_ANIMSPEED2(p) ((p)->animSpeed2)
#define PART_POSESTATE(p) ((p)->poseState)
#define PART_POSETYPE(p) ((p)->poseType)
#define PART_POSETIMER(p) ((p)->poseTimer)

//.sdata
s32 _status_pose_table[] = {
    STATUS_INSTANT_KILL,
    STATUS_STOP,
    STATUS_FREEZE,
    STATUS_SLEEP,
    STATUS_POISON,
    STATUS_CONFUSE,
    STATUS_DIZZY,
    -1
};

//local prototypes
const char* searchPoseTbl(BattleDispPose* table, s32 poseId);
void btlDispGXInit2DSub(void);
void btlDispTexPlaneInit(void);
void btlDispTexPlainGX(u32 id, GXColor color0, GXColor color1, GXColor color2, GXColor color3);
void btlDispGetTexSize(u32 id, u16* width, u16* height);
void _partsBlurControl(BattleUnitPart* part, s32 flags, Mtx orientation, GXColor color, f32 rotation);
void btlUnitItemDisp(CameraId camId, BattleUnit* unit);
void _pose_def(BattleUnitPart* part);
void _pose_one_pattern(BattleUnitPart* part);
void _pose_two_pattern(BattleUnitPart* part);
void _btlDispTex4(u32 id, Vec pos, Vec scale, Vec rot, Vec pos2, Vec scale2, GXColor color);
void _btlStockExpDisp(CameraId camId, void* param);

const char* searchPoseTbl(BattleDispPose* table, s32 poseId) {
    int i;

    if (!table) return NULL;
    for (i = 0; i < 0x1C; i++, table++) {
        if ((s32)table->poseId == poseId || (s32)table->poseId == 0x1C) {
            return table->poseName;
        }
    }
    return NULL;
}

void btlDispGXInit2DSub(void) {
    GXSetCullMode(GX_CULL_NONE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXClearVtxDesc();
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
}

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

void BattleCommandDisplay(BattleWork* wp);
void BattleStockExpDisp(void);
void btlDispAnimeSpeed(BattleUnitPart* part, f32 speed);
void btlUnitPartsBlurControl(BattleUnitPart* part, s32 unused, Mtx orientation, GXColor color, f32 rotation);
void floatOffsetControl(BattleUnitPart* part);
f32 getFloatDispOffset(BattleUnitPart* part);
f32 getGravityDispOffset(BattleUnitPart* part);
void gravityOffsetControl(BattleUnitPart* part);
void statusPoseControl(BattleUnitPart* part);
void btlDispGXPoint2DRasta(f32 x, f32 y, u8 r, u8 g, u8 b, u8 a) {
    GXPosition3f32(x, y, 0.0f);
    GXColor4u8(r, g, b, a);
}

void btlDispGXQuads2DRasta(f32 x0, f32 y0, f32 x1, f32 y1, u8 r, u8 g, u8 b, u8 a) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    btlDispGXPoint2DRasta(x0, y0, r, g, b, a);
    btlDispGXPoint2DRasta(x1, y0, r, g, b, a);
    btlDispGXPoint2DRasta(x1, y1, r, g, b, a);
    btlDispGXPoint2DRasta(x0, y1, r, g, b, a);
}

void btlDispGXPoint2D(f32 x, f32 y, u8 r, u8 g, u8 b, u8 a, f32 s, f32 t) {
    GXPosition3f32(x, y, 0.0f);
    GXColor4u8(r, g, b, a);
    GXTexCoord2f32(s, t);
}

void btlDispGXQuads2D(f32 x0, f32 y0, f32 x1, f32 y1, u8 r, u8 g, u8 b, u8 a) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    btlDispGXPoint2D(x0, y0, r, g, b, a, 0.0f, 0.0f);
    btlDispGXPoint2D(x1, y0, r, g, b, a, 1.0f, 0.0f);
    btlDispGXPoint2D(x1, y1, r, g, b, a, 1.0f, 1.0f);
    btlDispGXPoint2D(x0, y1, r, g, b, a, 0.0f, 1.0f);
}
void btlDispTexPlaneInit(void) {
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

void btlDispTexPlainGX(u32 id, GXColor color0, GXColor color1, GXColor color2, GXColor color3) {
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

void btlDispTexPlane(f32 tX, f32 tY, f32 tZ, f32 sX, f32 sY, u32 id, GXColor color, s32 unused) {
    CameraEntry* camera;
    Mtx scale, trans, mtx;

    btlDispTexPlaneInit();
    camera = camGetCurPtr();
    PSMTXScale(scale, sX, sY, 1.0f);
    PSMTXTrans(trans, tX, tY, tZ);
    PSMTXConcat(trans, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    btlDispTexPlainGX(id, color, color, color, color);
}

void btlDispTexPlane2(Mtx mtx, u32 id, GXColor color, s32 unused) {
    CameraEntry* camera;
    Mtx mtx2;

    btlDispTexPlaneInit();
    MTXCopy(mtx, mtx2);
    camera = camGetCurPtr();
    MTXConcat(camera->view, mtx2, mtx2);
    GXLoadPosMtxImm(mtx2, 0);
    btlDispTexPlainGX(id, color, color, color, color);
}

void btlDispTexPlane3(Mtx mtx, u32 id, GXColor color0, GXColor color1, GXColor color2, GXColor color3, s32 unused) {
    CameraEntry* camera;
    Mtx mtx2;

    btlDispTexPlaneInit();
    PSMTXCopy(mtx, mtx2);
    camera = camGetCurPtr();
    PSMTXConcat(camera->view, mtx2, mtx2);
    GXLoadPosMtxImm(mtx2, 0);
    btlDispTexPlainGX(id, color0, color1, color2, color3);
}

void btlDispGetTexSize(u32 id, u16* width, u16* height) {
    GXTexObj texobj;

    TEXGetGXTexObjFromPalette(*_battleWorkPointer->menuTex->data, &texobj, id);
    *width = GXGetTexObjWidth(&texobj);
    *height = GXGetTexObjHeight(&texobj);
}

void btlDispInit(void) {
    BattleWork* wp = _battleWorkPointer;
    
    battleCameraInit();
    battleMenuDispInit();
    wp->dispFlags |= 2;
}

StatusEffectType _GetStatusPoseType(BattleUnit* unit) {
    s32* table = _status_pose_table;
    StatusEffectType type;

    if (unit->mAttributeFlags & kInactive) {
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

typedef void (*PartsDispCb)(BattleUnitPart*, s32);
void btlUnitPartsBlurDisp(CameraId camId, BattleUnitPart* part);
void btlUnitItemDisp2(CameraId camId, BattleUnit* unit);
void btlUnitStolenItemDisp(CameraId camId, void* param);
void btlUnitPartsDisp(CameraId camId, BattleUnitPart* part);

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

    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(_battleWorkPointer, i);
        if (!unit) continue;
        if (unit->currentKind == UNIT_SHELL_SHIELD && unit->work[2] != 0) {
            target = BattleGetUnitPtr(wp, unit->work[0]);
            if (target) {
                if (target->attackEventId && evtCheckID(target->attackEventId)) {
                    if (!((u32)target->attackEventFlags & 4)) goto next;
                }
                if (target->damageEventId && evtCheckID(target->damageEventId)) goto next;
                BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
                BtlUnit_GetPos(target, &tpos.x, &tpos.y, &tpos.z);
                tpos.z += 7.0f;
                h = BattleGetFloorHeight(wp, tpos.x, tpos.y, tpos.z);
                switch (unit->work[2]) {
                    case 2:
                        // FAKE: double temp for the fabs result (register order of the diff/fabs webs)
                        { f64 a = __fabs(pos.x - tpos.x);
                        if (a > 10.0) {
                            if (tpos.x > pos.x) {
                                pos.x = pos.x + ((tpos.x - pos.x) - 10.0f);
                            } else {
                                pos.x = pos.x - ((pos.x - tpos.x) - 10.0f);
                            }
                        } }
                        BtlUnit_SetPos(unit, pos.x, tpos.y, tpos.z);
                        break;
                    case 3:
                        if (__fabs(pos.x - tpos.x) < 1.0) {
                            BtlUnit_SetPos(unit, tpos.x, h, tpos.z);
                            unit->work[2] = 1;
                        } else if (tpos.x > pos.x) {
                            pos.x += 1.0f;
                            BtlUnit_SetPos(unit, pos.x, h, tpos.z);
                        } else {
                            pos.x -= 1.0f;
                            BtlUnit_SetPos(unit, pos.x, h, tpos.z);
                        }
                        break;
                    default:
                        BtlUnit_SetPos(unit, tpos.x, h, tpos.z);
                        break;
                }
            }
        }
next:
        loop = unit->moveColorLvDisp;
        BtlUnit_OffUnitFlag(unit, 2);
        if (loop != unit->mMoveColorLv) {
            BtlUnit_OnUnitFlag(unit, 2);
            if (unit->mMoveColorLv > loop) {
                loop += 12;
                if (loop > unit->mMoveColorLv) loop = unit->mMoveColorLv;
            } else {
                loop -= 2;
                if (loop < unit->mMoveColorLv) loop = unit->mMoveColorLv;
            }
        }
        unit->moveColorLvDisp = loop;
        BtlUnit_HpGaugeMain(unit);
        unit->mSizeMultiplier = 1.0f;
        BtlUnit_GetStatus(unit, STATUS_TINY, &turns, &strength);
        if (turns > 0 && strength < 0) {
            scale = 0.5f * strength + 1.0f;
            if (scale < 0.5f) scale = 0.5f;
            unit->mSizeMultiplier *= scale;
        }
        BtlUnit_GetStatus(unit, STATUS_HUGE, &turns, &strength);
        if (turns > 0 && strength > 0) {
            scale = 1.0f + strength;
            if (scale > 2.0f) scale = 2.0f;
            unit->mSizeMultiplier *= scale;
        }
        BattleStatusEffectMain(unit);
        BattleStatusIconMain(unit);

        for (part = unit->mParts; part; part = part->mNextPart) {
            if (part->poseId == -1) continue;
            poseName = PART_POSENAME(part);
            if (!(PART_ANIMFLAGS(part) & 2)) {
                type = _GetStatusPoseType(unit);
                if (unit->currentKind >= UNIT_GOOMBA && unit->currentKind < UNIT_UNUSED_TREE) {
                    poseName = searchPoseTbl(part->poseTable, type);
                } else {
                    if (type == 0x1C && !BtlUnit_CheckStatus(unit, STATUS_HOLD_FAST) && (unit->mStatusFlags & UNIT_STATUS_FLAG_PERIL)) {
                        poseName = BtlUnit_GetPoseNameFromType(part, 0x39);
                        if (!poseName) poseName = searchPoseTbl(part->poseTable, 0x1C);
                    } else if (type == 0x1C && !BtlUnit_CheckStatus(unit, STATUS_HOLD_FAST) && (unit->mStatusFlags & UNIT_STATUS_FLAG_DANGER)) {
                        poseName = BtlUnit_GetPoseNameFromType(part, 0x38);
                        if (!poseName) poseName = searchPoseTbl(part->poseTable, 0x1C);
                    } else {
                        poseName = searchPoseTbl(part->poseTable, type);
                    }
                    if (BtlUnit_CheckStatusFlag(unit, UNIT_STATUS_FLAG_DEFEND) && BtlUnit_CanActStatus(unit)) {
                        poseName = BtlUnit_GetPoseNameFromType(part, 0x2D);
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
            if (PART_ANIMFLAGS(part) & 1) {
                xlu = 0;
                PART_ANIMFLAGS(part) &= ~1;
                if (PART_ANIMFLAGS(part) &= ~0x200) {
                    animPoseSetAnim(part->poseId, poseName, TRUE);
                } else {
                    animPoseSetAnim(part->poseId, poseName, FALSE);
                }
                PART_POSESTATE(part) = 0;
                if (PART_ANIMFLAGS(part) & 4) {
                    PART_POSETYPE(part) = 0;
                } else {
                    PART_POSETYPE(part) = 1;
                    if (BtlUnit_CheckStatus(unit, STATUS_INVISIBLE) || part->color.a != 0xFF) {
                        PART_POSETYPE(part) = 7;
                        xlu = 1;
                    }
                }
                if (!(PART_ANIMFLAGS(part) & 2) || (PART_ANIMFLAGS(part) & 8) || (PART_ANIMFLAGS(part) & 0x80)) {
                    PART_POSETYPE(part) = 1;
                    if (BtlUnit_CheckStatus(unit, STATUS_INVISIBLE) || part->color.a != 0xFF) {
                        PART_POSETYPE(part) = 7;
                        xlu = 1;
                    }
                    if (BtlUnit_CheckStatus(unit, STATUS_DODGY)) {
                        PART_POSETYPE(part) = 4;
                        xlu = 1;
                    }
                }
                if (xlu && (part->attributes & 0x8000)) {
                    xlu = 0;
                    PART_POSETYPE(part) = 0;
                }
                if (!(PART_ANIMFLAGS(part) & 4)) {
                    if (xlu && !(PART_ANIMFLAGS(part) & 0x40)) {
                        PART_ANIMFLAGS(part) |= 0x40;
                    } else if (!xlu && (PART_ANIMFLAGS(part) & 0x40)) {
                        PART_ANIMFLAGS(part) &= ~0x40;
                    }
                }
            }
            gravityOffsetControl(part);
            floatOffsetControl(part);
            statusPoseControl(part);
            BtlUnit_GetPartsWorldPos(part, &wx, &wy, &wz);
            animPoseMain(part->poseId);
            zpos = (Vec){wx, wy, wz + part->dispZOffset};
            part->dispColor.a = part->mKindPartParams->baseAlpha;
            if (BtlUnit_CheckStatus(unit, STATUS_INVISIBLE) || (unit->mAttributeFlags & kPermanentlyInvisible)) {
                part->dispColor.a = (part->dispColor.a << 7) / 255;
            }
            part->dispColor.a = part->dispColor.a * part->color.a / 255;
            part->dispColor.r = part->color.r;
            part->dispColor.g = part->color.g;
            part->dispColor.b = part->color.b;
            if (!(unit->mAttributeFlags & kUnitHidden) && !(part->attributes & kHidden)) {
                if (!(part->attributes & kMainPart) || !(unit->mAttributeFlags & kVeiled)) {
                    if (part->dispColor.a < 0xFF || animPoseTestXLU(part->poseId)) {
                        if (!(PART_ANIMFLAGS(part) & 0x100)) {
                            dispEntry(CAMERA_3D, dispCalcZ(&zpos), 2, (DispCallback)btlUnitPartsDisp, part);
                        }
                    } else {
                        if (!(PART_ANIMFLAGS(part) & 0x100)) {
                            dispEntry(CAMERA_3D, dispCalcZ(&zpos), 1, (DispCallback)btlUnitPartsDisp, part);
                        }
                    }
                }
                if (part->attributes & kMotionBlur) {
                    dispEntry(CAMERA_3D, wz, 2, (DispCallback)btlUnitPartsBlurDisp, part);
                }
            }
            if (!(unit->mAttributeFlags & kUnitNoShadow) && !(part->attributes & kNoShadow)) {
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
                scale = scale * ((f32)part->shadowSize * unit->mScale.x * part->mScale.x * unit->mSizeMultiplier) ;
                scale = scale * part->shadowScale;
                if ((unit->flags & BATTLE_UNIT_FLAG_SHADOW_WITH_PARTNER) && scale < 16.0f) scale = 16.0f;
                if (scale < 10.0f) scale = 8.0f;
                shadowEntry(spos.x, 10.0f + h, spos.z, scale);
            }
            { s32 k = unit->currentKind;
            if (k == 0xE4 || k == 0x8D || k == 0x86 || (u32)(k - 0x1F) <= 2 || (u32)(k - 0xBD) <= 4 || k == 0x85) {
                    viv.x = unit->work[0];
                    viv.y = unit->work[1];
                    viv.z = unit->work[2];
                    animPoseVivianMain(part->poseId, &viv);
            } }
            BtlUnit_ControlPoseSoundMain(part);
        }
        if (unit->heldItem) {
            if ((unit->flags & BATTLE_UNIT_FLAG_STOLEN_ITEM) && !(unit->flags & BATTLE_UNIT_FLAG_HIDE_HELD_ITEM)) {
                dispEntry(CAMERA_3D, 0.0f, 2, (DispCallback)btlUnitStolenItemDisp, unit);
            } else if ((!(unit->flags & BATTLE_UNIT_FLAG_ACTING) || (unit->flags & BATTLE_UNIT_FLAG_SHOW_ITEM_WHILE_ACTING)) && !(unit->flags & BATTLE_UNIT_FLAG_HIDE_HELD_ITEM)) {
                dispEntry(CAMERA_3D, 0.0f, 2, (DispCallback)btlUnitItemDisp, unit);
            }
        } else if (wp->flags & BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED) {
            dispEntry(CAMERA_3D, 0.0f, 2, (DispCallback)btlUnitItemDisp2, unit);
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

#define MTX_TRANS_NEG(m, v) PSMTXTrans(m, -(v).x, -(v).y, -(v).z)
#define MTX_TRANS(m, v) PSMTXTrans(m, (v).x, (v).y, (v).z)
#define CAMDIR(c) angleABf((c)->cameraPos.x, (c)->cameraPos.z, (c)->cameraAt.x, (c)->cameraAt.z)

void btlUnitPartsDisp(CameraId camId, BattleUnitPart* param) {
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
    unit = part->mOwner;
    if (part->poseId < 0) return;
    // FAKE: dead int guard (COLOURING W). part becomes a multi-def named local (not folded back into the param), so it
    // numbers above unit, and gz adds wp's missing neighbour; the arm is deleted after colouring and leaves no code.
    gz = 0;
    if (gz != 0) part = (BattleUnitPart*)unit;

    MTX_TRANS_NEG(uTN, unit->mRotationOffset);
    MTX_TRANS(uT, unit->mRotationOffset);
    MTX_TRANS(uDisp, unit->mDisplayOffset);
    MTX_TRANS(uPos, unit->mPosition);
    PSMTXRotRad(uRx, 'x', 0.017453292f * (unit->mRotation.x + unit->mBaseRotation.x));
    PSMTXRotRad(uRy, 'y', 0.017453292f * (unit->mRotation.y + unit->mBaseRotation.y));
    PSMTXRotRad(uRz, 'z', 0.017453292f * (unit->mRotation.z + unit->mBaseRotation.z));
    PSMTXScale(uS, unit->mSizeMultiplier * (unit->mBaseScale.x * unit->mScale.x), unit->mSizeMultiplier * (unit->mBaseScale.y * unit->mScale.y), unit->mBaseScale.z * unit->mScale.z);
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
        x = part->mPosition.x;
        y = part->mPosition.y + getGravityDispOffset(part) + getFloatDispOffset(part);
        z = part->mPosition.z;
        cam = camGetCurPtr();
        a = CAMDIR(cam);
        dir = reviseAngle(a - angleABf(cam->cameraPos.x, cam->cameraPos.z, x, z));
        MTX_TRANS_NEG(pTN2, part->mRotationOffset);
        MTX_TRANS(pT2, part->mRotationOffset);
        PSMTXRotRad(pRx2, 'x', 0.017453292f * (part->mRotation.x + part->mBaseRotation.x));
        PSMTXRotRad(pRy2, 'y', 0.017453292f * (dir + (part->mRotation.y + part->mBaseRotation.y)));
        PSMTXRotRad(pRz2, 'z', 0.017453292f * (part->mRotation.z + part->mBaseRotation.z));
        PSMTXTrans(pPos2, x, y, z);
        rotY = reviseAngle(-1.0f * angle - 90.0f);
        z = reviseAngle(part->mBaseRotation.y + (rotY + part->mRotation.y));
        cam = camGetCurPtr();
        camAngle = CAMDIR(cam);
        a = reviseAngle(z + camAngle);
        if (a >= 90.0f && a <= 270.0f) {
            zscale *= -1.0f;
            PSMTXRotRad(pRy2, 'y', 0.017453292f * ((dir + (part->mRotation.y + part->mBaseRotation.y)) - 2.0f * (180.0f - (a - camAngle))));
        }
        if (part->attributes & 0x8000000) {
            PSMTXScale(pS2, part->mBaseScale.x * part->mScale.x, part->mBaseScale.y * part->mScale.y, zscale * (part->mBaseScale.z * part->mScale.z));
        } else {
            PSMTXScale(pS2, unit->mSizeMultiplier * (part->mBaseScale.x * part->mScale.x), unit->mSizeMultiplier * (part->mBaseScale.y * part->mScale.y), zscale * (part->mBaseScale.z * part->mScale.z));
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
            dir = wp->alliances[unit->mAlliance].attackDirection;
        }
        ex = part->mPositionOffset.x * dir + unit->mPosition.x;
        ey = unit->mPosition.y + part->mPositionOffset.y + getGravityDispOffset(part) + getFloatDispOffset(part);
        ez = unit->mPosition.z + part->mPositionOffset.z;
        dir = 0.0f;
        if (part->mOwner->mAttributeFlags & 0x4000000) {
            cam = camGetCurPtr();
            a = CAMDIR(cam);
            dir = reviseAngle(a - angleABf(cam->cameraPos.x, cam->cameraPos.z, ex, ez));
        }
        MTX_TRANS_NEG(pTN, part->mRotationOffset);
        MTX_TRANS(pT, part->mRotationOffset);
        PSMTXRotRad(pRx, 'x', 0.017453292f * (part->mRotation.x + part->mBaseRotation.x));
        PSMTXRotRad(pRy, 'y', 0.017453292f * (dir + (part->mRotation.y + part->mBaseRotation.y)));
        PSMTXRotRad(pRz, 'z', 0.017453292f * (part->mRotation.z + part->mBaseRotation.z));
        PSMTXTrans(pPos, ex, ey, ez);
        MTX_TRANS(pDisp, part->mDisplayOffset);
        rotY = reviseAngle(-1.0f * angle - 90.0f);
        if (!(part->mOwner->flags & 0x1000000)) {
            a = reviseAngle(rotY + unit->mRotation.y + part->mRotation.y + unit->mBaseRotation.y + part->mBaseRotation.y);
            cam = camGetCurPtr();
            a = reviseAngle(a + CAMDIR(cam));
            if (a >= 90.0f && a <= 270.0f) {
                PSMTXScale(uS, unit->mSizeMultiplier * (unit->mBaseScale.x * unit->mScale.x), unit->mSizeMultiplier * (unit->mBaseScale.y * unit->mScale.y), -1.0f * (unit->mBaseScale.z * unit->mScale.z));
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
        PSMTXScale(pS, part->mBaseScale.x * part->mScale.x, part->mBaseScale.y * part->mScale.y, zscale * (part->mBaseScale.z * part->mScale.z));
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
    if (PART_ANIMFLAGS(part) & 0x10) {
        color->r = 0;
        color->g = 0;
        color->b = 0;
    }
    if (PART_ANIMFLAGS(part) & 0x20) {
        color->r = 0;
        color->g = 0;
        color->b = 0;
    }
    if (color->r == 0xFF && color->g == 0xFF && color->b == 0xFF) {
        if (BtlUnit_CheckStatus(unit, STATUS_POISON)) {
            color->r = color->r * 160 / 255;
            color->b = color->r * 160 / 255;
        }
        color->r = color->r * unit->moveColorLvDisp / 255;
        color->g = color->g * unit->moveColorLvDisp / 255;
        color->b = color->b * unit->moveColorLvDisp / 255;
    }
    animPoseSetMaterialEvtColor(part->poseId, *color);
    animPoseSetMaterialFlagOn(part->poseId, 0x40);
    if (part->dispCallback) {
        part->dispCallback(part, 0);
    }
    if (BtlUnit_CheckStatus(unit, STATUS_CHARGE)) {
        animPoseSetMaterialFlagOn(part->poseId, 0x800000);
    } else {
        animPoseSetMaterialFlagOff(part->poseId, 0x800000);
    }
    animPoseDrawMtx(part->poseId, drawMtx, 1, rotY, 2.0f);
    animPoseDrawMtx(part->poseId, drawMtx, 2, rotY, 2.0f);
    animPoseDrawMtx(part->poseId, drawMtx, 3, rotY, 2.0f);
    if (part->dispCallback) {
        part->dispCallback(part, 1);
    }
    btlUnitPartsBlurControl(part, 0, blurMtx, *color, rotY);
}

#pragma push
#pragma opt_dead_assignments off
void btlUnitItemDisp(CameraId camId, BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    s32 item;
    f32 height;
    Vec pos;

    height = BtlUnit_GetHeight(unit);

    item = BtlUnit_EnemyItemCanUseCheck(unit->heldItem);
    if (wp->flags & BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED && !item) {
        item = 0x94;
    }
    if (item) {
        BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
        pos.x += unit->heldItemBaseOffset.x;
        pos.y += unit->heldItemBaseOffset.y;
        pos.z += unit->heldItemBaseOffset.z;
        pos.x += unit->heldItemOffset.x;
        pos.y += unit->heldItemOffset.y;
        pos.z += unit->heldItemOffset.z;
        ((void (*)(f32, Vec*, s16, u16))iconDispGx)(1.0f, &pos, 0, itemDataTable[item].icon_id);
    }
}
#pragma pop

void btlUnitItemDisp2(CameraId camId, BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    s32 kind = unit->currentKind;

    if (kind >= 1 && kind < 0xD8) goto disp;
    if (kind >= 0xDE && kind < 0xE0) goto disp;
    if (kind < 0xE0 || kind >= 0xE7) return;
disp:
    if (wp->flags & BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED) {
        btlUnitItemDisp(camId, unit);
    } else {
        wp->flags |= BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED;
        btlUnitItemDisp(camId, unit);
        wp->flags &= ~BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED;
    }
}

void btlUnitStolenItemDisp(CameraId camId, void* param) {
    BattleUnit* unit = param;
    Vec pos;
    s32 item = unit->heldItem;

    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.y += 5.0f + unit->mSizeMultiplier * (f32)BtlUnit_GetHeight(unit);
    ((void (*)(f32, Vec*, s16, u16))iconDispGx)(1.0f, &pos, 0, itemDataTable[item].icon_id);
}

void btlUnitPartsBlurControl(BattleUnitPart* part, s32 unused, Mtx orientation, GXColor color, f32 rotation) {

    if (part->attributes & kMotionBlur) {
        _partsBlurControl(part, 1, orientation, color, rotation);
    } else {
        _partsBlurControl(part, 0, orientation, color, rotation);
    }
}

void _partsBlurControl(BattleUnitPart* part, s32 flags, Mtx orientation, GXColor color, f32 rotation) {
    BattleWorkUnitPartBlur* blur;
    int i;

    blur = &part->blurWork;
    for (i = 9; i > 0; i--) { //shift all slots forward one
        blur->work[i] = blur->work[i - 1];
    }
    blur->work[0].flags = flags;
    MTXCopy(orientation, blur->work[0].orientation);
    blur->work[0].rotation = rotation;
    blur->work[0].base = color;
    blur->work[0].blurColor[0] = blur->color[0];
    blur->work[0].blurColor[1] = blur->color[1];
    if (blur->flags & 1 && blur->work[1].flags & 1) {
        blur->work[0].flags &= ~1;
    }
}

void btlUnitPartsBlurDisp(CameraId camId, BattleUnitPart* part) {
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
                animPoseSetMaterialFlagOn(part->poseId, 0x40);
                animPoseDrawMtx(part->poseId, entry->orientation, 1, entry->rotation, 2.0f);
                animPoseDrawMtx(part->poseId, entry->orientation, 2, entry->rotation, 2.0f);
                animPoseDrawMtx(part->poseId, entry->orientation, 3, entry->rotation, 2.0f);
            }
        }
    }
}

void btlDispEntAnime(BattleUnit* unit) {
    BattleUnitPart* part;
    const char* name;

    for (part = unit->mParts; part; part = part->mNextPart) {
        if (!part->poseTable) {
            part->poseId = -1;
        } else {
            name = part->mKindPartParams->modelName;
            if (!name) {
                part->poseId = -1;
            } else {
                strcpy(PART_POSENAME(part), searchPoseTbl(part->poseTable, 0x1C));
                part->poseId = animPoseEntry(name, 1);
            }
        }
    }
}

void btlDispChangeAnime(BattleUnitPart* part, const char* name, BOOL flag) {
    strcpy(PART_POSENAME(part), name);
    PART_ANIMFLAGS(part) |= 2;
    PART_ANIMFLAGS(part) |= 1;
    PART_ANIMFLAGS(part) &= ~0x80;
    PART_ANIMFLAGS(part) &= ~0x200;
    PART_ANIMFLAGS(part) &= ~0x10;
    if (flag) {
        PART_ANIMFLAGS(part) |= 8;
    } else {
        PART_ANIMFLAGS(part) &= ~8;
    }
}

void btlDispChangePeraAnime(BattleUnitPart* part, const char* name) {
    PART_ANIMFLAGS(part) |= 4;
    PART_POSETYPE(part) = 0;
    animPoseSetEffectAnim(part->poseId, name, 1);
}

void btlDispPoseAnime(BattleUnitPart* part) {
    PART_ANIMFLAGS(part) &= ~0x12;
    PART_ANIMFLAGS(part) |= 1;
    PART_ANIMFLAGS(part) &= ~0x80;
    PART_ANIMFLAGS(part) &= ~0x4;
}

void gravityOffsetControl(BattleUnitPart* part) {
    BOOL active = TRUE;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = part->mOwner;

    if (!(wp->flags & BATTLE_FLAG_ZERO_GRAVITY_UNUSED)) {
        active = FALSE;
    } else if (unit->mAttributeFlags & (kFloating | kNoGravityBob)) {
        active = FALSE;
    }
    if (active) {
        PART_GRAVITY(part)++;
    } else if (PART_GRAVITY(part)) {
        PART_GRAVITY(part) %= 120;
        if (PART_GRAVITY(part) >= 60) {
            PART_GRAVITY(part)++;
        } else if (PART_GRAVITY(part)) {
            PART_GRAVITY(part)--;
        }
    }
}

f32 getGravityDispOffset(BattleUnitPart* part) {
    return 4.0f * sinfd((PART_GRAVITY(part) * 3 + 270) % 360) + 4.0f;
}

void floatOffsetControl(BattleUnitPart* part) {
    BOOL active = TRUE;
    BattleUnit* unit = part->mOwner;

    if (!(unit->flags & BATTLE_UNIT_FLAG_FLOAT_CAPABLE)) {
        active = FALSE;
    } else if (!(unit->flags & BATTLE_UNIT_FLAG_FLOAT_ACTIVE)) {
        active = FALSE;
    } else if (BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
        active = FALSE;
    }
    if (active) {
        PART_FLOAT(part)++;
    } else if (PART_FLOAT(part)) {
        PART_FLOAT(part) %= 90;
        if (PART_FLOAT(part) >= 45) {
            PART_FLOAT(part)++;
        } else if (PART_FLOAT(part)) {
            PART_FLOAT(part)--;
        }
    }
}

f32 getFloatDispOffset(BattleUnitPart* part) {
    return 2.0f * sinfd((PART_FLOAT(part) * 4 + 270) % 360) + 2.0f;
}

void statusPoseControl(BattleUnitPart* part) {
    switch (PART_POSETYPE(part)) {
        case 0:
            break;
        case 2:
        case 3:
        case 7:
            _pose_one_pattern(part);
            break;
        case 4:
        case 5:
        case 6:
            _pose_two_pattern(part);
            break;
        default:
            _pose_def(part);
            break;
    }
}

void _pose_def(BattleUnitPart* part) {
    switch (PART_POSESTATE(part)) {
        case 0:
            PART_POSESTATE(part) = 1;
            animPoseSetEffect(part->poseId, NULL, 0);
            if (part->mOwner->flags & 0x1000000) {
                animPoseSetMaterialLightFlagOn(part->poseId, 8);
            }
            break;
    }
}

void _pose_one_pattern(BattleUnitPart* part) {
    switch (PART_POSESTATE(part)) {
    case 0:
        PART_POSESTATE(part) = 1;
        switch (PART_POSETYPE(part)) {
            case 2:
                animPoseSetEffect(part->poseId, "p_b_st", 1);
                animPoseSetEffectAnim(part->poseId, "PM_C_1A", 1);
                break;
            case 3:
                animPoseSetEffect(part->poseId, "p_b_st", 1);
                animPoseSetEffectAnim(part->poseId, "PM_F_1A", 1);
                break;
            case 7:
                animPoseSetEffect(part->poseId, "p_b_st", 1);
                animPoseSetEffectAnim(part->poseId, "PM_Y_1A", 1);
                break;
        }
        break;
    }
}

void _pose_two_pattern(BattleUnitPart* part) {
    switch (PART_POSESTATE(part)) {
        case 0:
            PART_POSESTATE(part) = 1;
            switch (PART_POSETYPE(part)) {
                case 4:
                    animPoseSetEffect(part->poseId, "p_b_st", 1);
                    animPoseSetEffectAnim(part->poseId, "PM_Y_1B", 1);
                    PART_POSETIMER(part) = sysMsec2Frame(2200);
                    break;
                case 5:
                    animPoseSetEffect(part->poseId, "p_b_st", 1);
                    animPoseSetEffectAnim(part->poseId, "PM_C_1A", 1);
                    PART_POSETIMER(part) = (irand(4) + 6) * sysMsec2Frame(2200);
                    break;
                case 6:
                    animPoseSetEffect(part->poseId, "p_b_st", 1);
                    animPoseSetEffectAnim(part->poseId, "PM_F_2", 1);
                    PART_POSETIMER(part) = (irand(4) + 7) * 22;
                    break;
            }
            break;
        case 1:
            switch (PART_POSETYPE(part)) {
                case 4:
                    PART_POSETIMER(part)--;
                    if (PART_POSETIMER(part) == 0) {
                        animPoseSetEffectAnim(part->poseId, "PM_Y_1B", 1);
                        PART_POSETIMER(part) = sysMsec2Frame(2200);
                    }
                    break;
                case 5:
                    PART_POSETIMER(part)--;
                    if (PART_POSETIMER(part) == 0) {
                        PART_POSESTATE(part) = 2;
                        animPoseSetEffectAnim(part->poseId, "PM_C_1B", 1);
                        PART_POSETIMER(part) = 22;
                    }
                    break;
                case 6:
                    PART_POSETIMER(part)--;
                    if (PART_POSETIMER(part) == 0) {
                        PART_POSESTATE(part) = 2;
                        animPoseSetEffectAnim(part->poseId, "PM_F_1B", 1);
                        PART_POSETIMER(part) = 22;
                    }
                    break;
            }
            break;
        case 2:
            switch (PART_POSETYPE(part)) {
                case 4:
                    PART_POSETIMER(part)--;
                    if (PART_POSETIMER(part) == 0) {
                        PART_POSESTATE(part) = 0;
                        _pose_two_pattern(part);
                    }
                    break;
                case 5:
                    PART_POSETIMER(part)--;
                    if (PART_POSETIMER(part) == 0) {
                        PART_POSESTATE(part) = 0;
                        _pose_two_pattern(part);
                    }
                    break;
                case 6:
                    PART_POSETIMER(part)--;
                    if (PART_POSETIMER(part) == 0) {
                        PART_POSESTATE(part) = 0;
                        _pose_two_pattern(part);
                    }
                    break;
            }
            break;
    }
}

#pragma push
#pragma opt_dead_assignments off
void btlDispAnimeSpeed(BattleUnitPart* part, f32 speed) {
    f32 unusedOne = 1.0f;
    PART_ANIMSPEED(part) = speed;
    animPoseSetLocalTimeRate(part->poseId, PART_ANIMSPEED(part) * PART_ANIMSPEED2(part));
}
#pragma pop

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

void btlDispTex4(u32 id, Vec pos, Vec scale, Vec rot, GXColor color) {
    u16 width, height;
    f32 hw, hh;
    Vec off;
    Vec unused;
    unused.x = 0.0f;
    unused.y = 0.0f;
    unused.z = 0.0f;

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

void _btlDispTex4(u32 id, Vec pos, Vec scale, Vec rot, Vec pos2, Vec scale2, GXColor color) {
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

void BattleStockExpDisp(void) {
    BattleWork* wp = _battleWorkPointer;

    if (wp->stockExp > wp->stockExpDisp) {
        wp->stockExpTimer++;
        if (wp->stockExpTimer >= 4) {
            wp->stockExpDisp++;
        }
    }
    if (wp->dispFlags & 2) {
        dispEntry(CAMERA_2D, 901.0f, 1, _btlStockExpDisp, NULL);
    }
}

void _btlStockExpDisp(CameraId camId, void* param) {
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
            iconDispGx(&pos, 0x10, ICON_18C, 0.35f);
        }
        for (i = 0; i < tens; i++) {
            Vec pos = {0.0f, 0.0f, -100.0f};
            pos.x = 4.0f + (308.0f + x - 10.0f - 20.0f * i);
            pos.y = 16.0f + (y - 220.0f);
            iconDispGx(&pos, 0x10, ICON_18C, 0.5f);
        }
    }
}


