#include "driver/windowdrv.h"
#include "dolphin/os.h"
#include "driver/arcdrv.h"
#include "driver/camdrv.h"
#include "memory.h"
#include "system.h"
#include "sdk/texPalette.h"
#include <string.h>
#include "libc_decls.h"

//.sbss
WindowEntry* WinObjects;
GXTexObj* wakuTexObj;


//msgWindow.tpl texture indices (light/dark pairs)
typedef enum WakuTexId {
    WAKU_ARROW = 1,
    WAKU_ARROW_DARK = 2,
    WAKU_SMALL = 3,
    WAKU_SMALL_DARK = 4,
    WAKU_MAJO_ARROW = 7,
    WAKU_MAJO_ARROW_DARK = 8
} WakuTexId;

static const GXColor fog_color = {0xFF, 0xFF, 0xFF, 0xFF};

static inline void fogOff(void) {
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, fog_color);
}

void windowInit(void) {
    WindowEntry* w;

    w = WinObjects = __memAlloc(HEAP_DEFAULT, sizeof(WindowEntry) * 7);
    w[0].flags = 0;
    w[0].userdata = NULL;
    w[1].flags = 0;
    w[1].userdata = NULL;
    w[2].flags = 0;
    w[2].userdata = NULL;
    w[3].flags = 0;
    w[3].userdata = NULL;
    w[4].flags = 0;
    w[4].userdata = NULL;
    w[5].flags = 0;
    w[5].userdata = NULL;
    w[6].flags = 0;
    w[6].userdata = NULL;
    winTpl = NULL;
    tpl_loaded = FALSE;
}

// Completion callback of the msgWindow.tpl DVD read in windowTexSetup (new in retail)
// Descriptive name (not in the symbol maps); placeholder unk_8007f204, JP 0x8007F204. See docs/NAME_PROVENANCE.md.
void windowTexSetup_callback(s32 error, DVDFileInfo* info) {
    DVDEntry* entry = info->cb.userData;
    s32 i;

    UnpackTexPalette(winTpl);
    DVDMgrClose(entry);
    wakuTexObj = __memAlloc(HEAP_DEFAULT, sizeof(GXTexObj) * winTpl->numDescriptors);

    for (i = 0; i < winTpl->numDescriptors; i++) {
        TEXGetGXTexObjFromPalette(winTpl, &wakuTexObj[i], i);
    }
    tpl_loaded = TRUE;
}

void windowTexSetup(void) {
    DVDEntry* entry;
    u32 size;
    char path[128];
    s32 i;

    winTpl = NULL;
    tpl_loaded = FALSE;
    winTpl = arcOpen("font/msgWindow.tpl", NULL, NULL);
    if (!winTpl) {
        sprintf(path, "%s/f/msgWindow.tpl", getMarioStDvdRoot());
        entry = DVDMgrOpen(path, 2, 0);
        size = OSRoundUp32B(DVDMgrGetLength(entry));
        winTpl = __memAlloc(HEAP_DEFAULT, size);
        entry->info.cb.userData = entry;
        DVDMgrReadAsync(entry, winTpl, size, 0, windowTexSetup_callback);
    } else {
        UnpackTexPalette(winTpl);
        wakuTexObj = __memAlloc(HEAP_DEFAULT, sizeof(GXTexObj) * winTpl->numDescriptors);

        for (i = 0; i < winTpl->numDescriptors; i++) {
            TEXGetGXTexObjFromPalette(winTpl, &wakuTexObj[i], i);
        }
    }
}

void windowReInit(void) {
    WindowEntry* w = WinObjects;

    w[0].flags = 0;
    w[0].userdata = NULL;
    w[1].flags = 0;
    w[1].userdata = NULL;
    w[2].flags = 0;
    w[2].userdata = NULL;
    w[3].flags = 0;
    w[3].userdata = NULL;
    w[4].flags = 0;
    w[4].userdata = NULL;
    w[5].flags = 0;
    w[5].userdata = NULL;
    w[6].flags = 0;
    w[6].userdata = NULL;
}

s32 windowEntry(u16 priority) {
    WindowEntry* entry;
    int i;

    entry = WinObjects;
    for (i = 0; i < 7; i++, entry++) {
        if (!(entry->flags & WINDOW_FLAG_ACTIVE)) {
            memset(entry, 0, sizeof(WindowEntry));
            entry->flags = WINDOW_FLAG_ACTIVE;
            entry->priority = priority;
            return i;
        }
    }
    return -1;
}

BOOL windowDelete(WindowEntry* entry) {
    if (entry->delete) {
        entry->delete(entry);
    }
    entry->flags = 0;
    entry->userdata = NULL;
    return TRUE;
}

BOOL windowDeleteID(s32 id) {
    WindowEntry* entry = &WinObjects[id];
    if (!(entry->flags & WINDOW_FLAG_ACTIVE)) {
        return FALSE;
    }
    if (entry->delete) {
        entry->delete(entry);
    }
    entry->flags = 0;
    entry->userdata = NULL;
    return TRUE;
}

void windowMain(void) {
    WindowEntry* entry;
    int i;

    entry = WinObjects;
    for (i = 0; i < 7; i++, entry++) {
        if (entry->flags & WINDOW_FLAG_ACTIVE) {
            if (entry->main) {
                entry->main(entry);
            }
        }
    }
}

void windowDispGX_Kanban(s32 type, u8 alpha, f32 x, f32 y, f32 height, f32 width) {
    CameraEntry* camera;
    Mtx trans, scale;
    Mtx wood_mtx1, wood_mtx2, wood_mtx3, wood_mtx4; // WINDOW_TYPE_PLATE, WINDOW_TYPE_KANBAN
    Mtx lines_mtx1, lines_mtx2, lines_mtx3, lines_mtx4; // WINDOW_TYPE_HOUSOU
    Mtx letter_mtx;

    camera = camGetCurPtr();
    GXSetCullMode(GX_CULL_NONE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    fogOff();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFF, alpha});
    MTXTrans(trans, x, y, 0.0f);
    MTXScale(scale, height / 560.0f, width / 176.0f, 1.0f);
    MTXConcat(trans, scale, trans);
    MTXConcat(camera->view, trans, trans);
    GXLoadPosMtxImm(trans, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    switch (type) {
        case WINDOW_TYPE_PLATE: //images 29, 30, 31, 32
        case WINDOW_TYPE_KANBAN:
            //left side of the frame
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(wood_mtx1, __fabsf(72.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[29]),
                __fabsf(176.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[29]), 1.0f);
            GXLoadTexMtxImm(wood_mtx1, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[29], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(72.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(72.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(0.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //top side of the frame
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(wood_mtx2, __fabsf(416.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[31]),
                __fabsf(136.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[31]), 1.0f);
            GXLoadTexMtxImm(wood_mtx2, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[31], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(72.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(488.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(488.0f, -136.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(72.0f, -136.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //right side of the frame
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(wood_mtx3, __fabsf(72.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[30]),
                __fabsf(176.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[30]), 1.0f);
            GXLoadTexMtxImm(wood_mtx3, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[30], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(488.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(560.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(560.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(488.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //bottom side of the frame
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(wood_mtx4, __fabsf(416.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[32]),
                __fabsf(40.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[32]), 1.0f);
            GXLoadTexMtxImm(wood_mtx4, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[32], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(72.0f, -136.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(488.0f, -136.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(488.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(72.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
            break;

        case WINDOW_TYPE_HOUSOU: //images 25, 26, 27, 28
            //left side of the frame
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(lines_mtx1, __fabsf(72.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[25]),
                __fabsf(176.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[25]), 1.0f);
            GXLoadTexMtxImm(lines_mtx1, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[25], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(72.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(72.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(0.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //top side of the frame
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(lines_mtx2, __fabsf(416.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[27]),
                __fabsf(136.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[27]), 1.0f);
            GXLoadTexMtxImm(lines_mtx2, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[27], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(72.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(488.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(488.0f, -136.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(72.0f, -136.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //right side of the frame
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(lines_mtx3, __fabsf(72.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[26]),
                __fabsf(176.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[26]), 1.0f);
            GXLoadTexMtxImm(lines_mtx3, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[26], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(488.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(560.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(560.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(488.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //bottom side of the frame
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(lines_mtx4, __fabsf(416.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[28]),
                __fabsf(40.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[28]), 1.0f);
            GXLoadTexMtxImm(lines_mtx4, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[28], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(72.0f, -136.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(488.0f, -136.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(488.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(72.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
            break;

        case WINDOW_TYPE_DIARY: //image 6
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(letter_mtx, __fabsf(560.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[6]),
                __fabsf(176.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[6]), 1.0f);
            GXLoadTexMtxImm(letter_mtx, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[6], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(560.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(560.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(0.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
            break;

        default:
            break;
    }
}

void windowDispGX_System(s32 type, u8 alpha, f32 x, f32 y, f32 height, f32 width) {
    CameraEntry* camera;
    f32 x5, scroll;

    Mtx mtx;
    Mtx trans, scale;
    f32 x2, y2;
    Mtx trans2, scale2;
    f32 y3, y4;
    Mtx trans3, scale3;
    f32 x3, x4;
    Mtx trans4, scale4;
    static f32 scrl;

    camera = camGetCurPtr();
    scrl += 0.005f;
    if (scrl > 10.0f) {
        scrl -= 10.0f;
    }
    GXSetCullMode(GX_CULL_NONE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    fogOff();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumChans(0);
    GXSetNumTexGens(1);

    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFF, alpha});
    GXSetNumTevStages(4);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);

    MTXTrans(mtx, x, y, 0.0f);
    MTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    //top left of the frame
    scroll = scrl;
    GXLoadTexObj(&wakuTexObj[5], GX_TEXMAP0);
    GXLoadTexObj(&wakuTexObj[42], GX_TEXMAP1);
    GXLoadTexObj(&wakuTexObj[46], GX_TEXMAP2);

    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    MTXTrans(trans, scroll + (0.0f / (f32)GXGetTexObjWidth(&wakuTexObj[5])),
        (-0.0f / (f32)GXGetTexObjHeight(&wakuTexObj[5])) - scroll, 0.0f);
    MTXScale(scale, __fabsf(height - 32.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[5]),
        __fabsf(width - 32.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[5]), 1.0f);
    MTXConcat(trans, scale, trans);
    GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1);
    MTXScale(scale, __fabsf(height - 32.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[42]),
        __fabsf(width - 32.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[42]), 1.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition3f32(0.0f, 0.0f, 0.0f);
    x5 = 0.0f;
    x2 = x5 + (height - 32.0f);
    y2 = x5 - (width - 32.0f);
    GXTexCoord2f32(0.0f, 0.0f);

    GXPosition3f32(x2, 0.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);

    GXPosition3f32(x2, y2, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);

    GXPosition3f32(0.0f, y2, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);

    //bottom left of the frame
    scroll = scrl;
    GXLoadTexObj(&wakuTexObj[5], GX_TEXMAP0);
    GXLoadTexObj(&wakuTexObj[43], GX_TEXMAP1);
    GXLoadTexObj(&wakuTexObj[47], GX_TEXMAP2);

    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    MTXTrans(trans2, scroll + (0.0f / (f32)GXGetTexObjWidth(&wakuTexObj[5])),
        ((width - 32.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[5])) - scroll, 0.0f);
    MTXScale(scale2, __fabsf(height - 32.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[5]),
        __fabsf(32.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[5]), 1.0f);
    MTXConcat(trans2, scale2, trans2);
    GXLoadTexMtxImm(trans2, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1);
    MTXScale(scale2, __fabsf(height - 32.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[43]),
        __fabsf(32.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[43]), 1.0f);
    GXLoadTexMtxImm(scale2, GX_TEXMTX1, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    // FAKE: GXPosition3f32(0.0f, y3, 0.0f) written out as FIFO stores with y3/y4 computed between them; the plain call changes the object
    GXWGFifo.f32 = 0.0f;
    y3 = -(width - 32.0f);
    y4 = y3 - 32.0f;
    GXWGFifo.f32 = y3;
    GXWGFifo.f32 = 0.0f;
    GXTexCoord2f32(0.0f, 0.0f);

    GXPosition3f32(x2, y3, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);

    GXPosition3f32(x2, y4, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);

    GXPosition3f32(0.0f, y4, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);

    //top right of the frame
    scroll = scrl;
    GXLoadTexObj(&wakuTexObj[5], GX_TEXMAP0);
    GXLoadTexObj(&wakuTexObj[44], GX_TEXMAP1);
    GXLoadTexObj(&wakuTexObj[48], GX_TEXMAP2);

    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    MTXTrans(trans3, scroll + ((height - 32.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[5])),
        (-0.0f / (f32)GXGetTexObjHeight(&wakuTexObj[5])) - scroll, 0.0f);
    MTXScale(scale3, __fabsf(32.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[5]),
        __fabsf(width - 32.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[5]), 1.0f);
    MTXConcat(trans3, scale3, trans3);
    GXLoadTexMtxImm(trans3, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1);
    MTXScale(scale3, __fabsf(32.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[44]),
        __fabsf(width - 32.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[44]), 1.0f);
    GXLoadTexMtxImm(scale3, GX_TEXMTX1, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    // FAKE: x4 first holds the constant 32.0f; `x3 = height - 32.0f; x4 = x3 + 32.0f;` changes the object
    x4 = 32.0f;
    x3 = height - x4;
    x4 = x3 + x4;

    GXPosition3f32(x3, 0.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);

    GXPosition3f32(x4, 0.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);

    GXPosition3f32(x4, y2, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);

    GXPosition3f32(x3, y2, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);

    //bottom right of the frame
    scroll = scrl;
    GXLoadTexObj(&wakuTexObj[5], GX_TEXMAP0);
    GXLoadTexObj(&wakuTexObj[45], GX_TEXMAP1);
    GXLoadTexObj(&wakuTexObj[49], GX_TEXMAP2);

    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    MTXTrans(trans4, scroll + ((height - 32.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[5])),
        ((width - 32.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[5])) - scroll, 0.0f);
    MTXScale(scale4, __fabsf(32.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[5]),
        __fabsf(32.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[5]), 1.0f);
    MTXConcat(trans4, scale4, trans4);
    GXLoadTexMtxImm(trans4, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1);
    MTXScale(scale4, __fabsf(32.0f) / (f32)GXGetTexObjWidth(&wakuTexObj[45]),
        __fabsf(32.0f) / (f32)GXGetTexObjHeight(&wakuTexObj[45]), 1.0f);
    GXLoadTexMtxImm(scale4, GX_TEXMTX1, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    x5 = height - 32.0f;

    GXPosition3f32(x5, y3, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);

    GXPosition3f32(x4, y3, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);

    GXPosition3f32(x4, y4, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);

    GXPosition3f32(x5, y4, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
}

void _windowDispGX_Message(s32 type, s32 tailFlags, u8 alpha, BOOL dark, f32 x, f32 y, f32 height, f32 width, f32 tailX, f32 tailY) {
    CameraEntry* camera;
    Mtx trans, scale;
    Mtx norm_mtx1, norm_mtx2, norm_mtx3, norm_mtx4;
    Mtx boss_mtx1, boss_mtx2, boss_mtx3, boss_mtx4;
    Mtx tec_mtx1, tec_mtx2;
    Mtx majo_mtx1, majo_mtx2, majo_mtx3, majo_mtx4;
    f32 r;
    f32 xx;
    f32 y2;

    camera = camGetCurPtr();
    GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFF, alpha});
    MTXTrans(trans, x, y, 0.0f);
    MTXScale(scale, height / 560.0f, width / 176.0f, 1.0f);
    MTXConcat(trans, scale, trans);
    MTXConcat(camera->view, trans, trans);
    GXLoadPosMtxImm(trans, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    switch (type) {
        case WINDOW_TYPE_NORMAL: //images 9-12, 13-16 for light/dark
            //left side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(norm_mtx1, __fabsf(72.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 13 : 9]),
                __fabsf(176.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 13 : 9]), 1.0f);
            GXLoadTexMtxImm(norm_mtx1, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 13 : 9], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(72.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(72.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(0.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //top side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(norm_mtx2, __fabsf(416.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 15 : 11]),
                __fabsf(136.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 15 : 11]), 1.0f);
            GXLoadTexMtxImm(norm_mtx2, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 15 : 11], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(72.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(488.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(488.0f, -136.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(72.0f, -136.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //right side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(norm_mtx3, __fabsf(72.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 14 : 10]),
                __fabsf(176.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 14 : 10]), 1.0f);
            GXLoadTexMtxImm(norm_mtx3, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 14 : 10], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(488.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(560.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(560.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(488.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //bottom side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(norm_mtx4, __fabsf(416.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 16 : 12]),
                __fabsf(40.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 16 : 12]), 1.0f);
            GXLoadTexMtxImm(norm_mtx4, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 16 : 12], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(72.0f, -136.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(488.0f, -136.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(488.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(72.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
            break;

        case WINDOW_TYPE_SMALL: //images 3, 4
            GXLoadTexObj(&wakuTexObj[dark ? WAKU_SMALL_DARK : WAKU_SMALL], GX_TEXMAP0);
            MTXScale(trans, height, width, 1.0f);
            MTXTransApply(trans, trans, x, y, 0.0f);
            MTXConcat(camera->view, trans, trans);
            GXLoadPosMtxImm(trans, GX_PNMTX0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(1.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(1.0f, -1.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(0.0f, -1.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
            break;

        case WINDOW_TYPE_BOSS: //images 17-20, 21-24
            //left side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(boss_mtx1, __fabsf(72.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 21 : 17]),
                __fabsf(176.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 21 : 17]), 1.0f);
            GXLoadTexMtxImm(boss_mtx1, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 21 : 17], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(72.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(72.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(0.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //top side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(boss_mtx2, __fabsf(416.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 23 : 19]),
                __fabsf(136.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 23 : 19]), 1.0f);
            GXLoadTexMtxImm(boss_mtx2, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 23 : 19], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(72.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(488.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(488.0f, -136.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(72.0f, -136.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //right side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(boss_mtx3, __fabsf(72.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 22 : 18]),
                __fabsf(176.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 22 : 18]), 1.0f);
            GXLoadTexMtxImm(boss_mtx3, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 22 : 18], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(488.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(560.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(560.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(488.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //bottom side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(boss_mtx4, __fabsf(416.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 24 : 20]),
                __fabsf(40.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 24 : 20]), 1.0f);
            GXLoadTexMtxImm(boss_mtx4, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 24 : 20], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(72.0f, -136.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(488.0f, -136.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(488.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(72.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
            break;

        case WINDOW_TYPE_TEC: //image 41
            //main part: x 0 to 536
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(tec_mtx1, __fabsf(536.0f) / GXGetTexObjWidth(&wakuTexObj[41]),
                __fabsf(176.0f) / GXGetTexObjHeight(&wakuTexObj[41]), 1.0f);
            GXLoadTexMtxImm(tec_mtx1, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[41], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(536.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(536.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(0.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //right end: x 560 back to 536, the same texture mirrored
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(tec_mtx2, __fabsf(-24.0f) / GXGetTexObjWidth(&wakuTexObj[41]),
                __fabsf(176.0f) / GXGetTexObjHeight(&wakuTexObj[41]), 1.0f);
            GXLoadTexMtxImm(tec_mtx2, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[41], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(560.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(536.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(536.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(560.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
            break;

        case WINDOW_TYPE_MAJO: //images 33-36, 37-40
            //left side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(majo_mtx1, __fabsf(72.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 37 : 33]),
                __fabsf(176.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 37 : 33]), 1.0f);
            GXLoadTexMtxImm(majo_mtx1, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 37 : 33], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(72.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(72.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(0.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //top side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(majo_mtx2, __fabsf(416.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 39 : 35]),
                __fabsf(136.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 39 : 35]), 1.0f);
            GXLoadTexMtxImm(majo_mtx2, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 39 : 35], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(72.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(488.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(488.0f, -136.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(72.0f, -136.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //right side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(majo_mtx3, __fabsf(72.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 38 : 34]),
                __fabsf(176.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 38 : 34]), 1.0f);
            GXLoadTexMtxImm(majo_mtx3, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 38 : 34], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(488.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(560.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(560.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(488.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);

            //bottom side
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
            MTXScale(majo_mtx4, __fabsf(416.0f) / GXGetTexObjWidth(&wakuTexObj[dark ? 40 : 36]),
                __fabsf(40.0f) / GXGetTexObjHeight(&wakuTexObj[dark ? 40 : 36]), 1.0f);
            GXLoadTexMtxImm(majo_mtx4, GX_TEXMTX0, GX_MTX2x4);
            GXLoadTexObj(&wakuTexObj[dark ? 40 : 36], GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            GXPosition3f32(72.0f, -136.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition3f32(488.0f, -136.0f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition3f32(488.0f, -176.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);

            GXPosition3f32(72.0f, -176.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
            break;

        default:
            break;
    }
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    if (tailFlags & 1) {
        if (tailFlags & 4) {
            xx = (0.7f * height) + x;
        } else {
            xx = (0.38f * height) + x;
        }

        if (tailFlags & 8) {
            y2 = -((0.175f * width) - y);
            y = ((0.225f * width) + y);
            if (y2 >= tailY) goto end;
        } else {
            y2 = -((0.8f * width) - y);
            y = -((1.2f * width) - y);
            if (y2 <= tailY) goto end;
        }
        r = (xx - (((xx - tailX) * (y2 - y)) / (y2 - tailY)));
        if (tailX >= xx) {
            if (r > tailX) {
                r = tailX;
            }
        } else {
            if (r < tailX) {
                r = tailX;
            }
        }

        switch (type) {
            case WINDOW_TYPE_NORMAL:
            case WINDOW_TYPE_SMALL:
            case WINDOW_TYPE_BOSS:
                GXLoadTexObj(&wakuTexObj[dark ? WAKU_ARROW_DARK : WAKU_ARROW], GX_TEXMAP0);
                break;

            case WINDOW_TYPE_MAJO:
                GXLoadTexObj(&wakuTexObj[dark ? WAKU_MAJO_ARROW_DARK : WAKU_MAJO_ARROW], GX_TEXMAP0);
                break;

            default:
                break;
        }

        GXLoadPosMtxImm(camera->view, GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4u);


        GXPosition3f32(xx - 0.05f * height, y2, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);

        GXPosition3f32(xx + 0.05f * height, y2, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);

        GXPosition3f32(r + 0.05f * height, y, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);

        GXPosition3f32(r - 0.05f * height, y, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
end:
    ;
}

BOOL tpl_loaded; // TRUE once msgWindow.tpl is unpacked and wakuTexObj is built (new in retail)
TEXPalette* winTpl; //new in retail

void windowDispGX_Message(s32 type, s32 tailFlags, u8 alpha, f32 x, f32 y, f32 height, f32 width, f32 tailX, f32 tailY) {
    GXSetCullMode(GX_CULL_NONE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    fogOff();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetCurrentMtx(GX_PNMTX0);
    switch (type) {
        case WINDOW_TYPE_TEC:
            _windowDispGX_Message(type, 0, alpha, TRUE, x, y, height, width, tailX, tailY);
            break;
        default:
            _windowDispGX_Message(type, tailFlags, alpha, TRUE, x, y, height, width, tailX, tailY);
            _windowDispGX_Message(type, tailFlags, alpha, FALSE, x, y, height, width, tailX, tailY);
            break;
    }
}

void windowDispGX_Waku_col(u16 gxTexMapID, GXColor color, f32 x, f32 y, f32 width, f32 height, f32 curve) {
    CameraEntry* camera = camGetCurPtr();
    Mtx mtx;
    f32 twothirds, third;
    f32 right, bottom, innerLeft, innerRight, innerTop, innerBottom;

    if (tpl_loaded) {
        if (gxTexMapID) {
            gxTexMapID = 0;
        }
        GXSetCullMode(GX_CULL_NONE);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
        GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
        fogOff();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetNumChans(0);
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, color);
        MTXIdentity(mtx);
        MTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXLoadTexObj(&wakuTexObj[gxTexMapID], GX_TEXMAP0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 0x24);

        right = x + width;
        bottom = y - height;
        innerLeft = x + curve;
        innerTop = y - curve;
        innerRight = right - curve;
        innerBottom = curve + bottom;
        third = 0.33333334f;
        twothirds = 0.6666667f;

        GXPosition3f32(x, y, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);

        GXPosition3f32(innerLeft, y, 0.0f);
        GXTexCoord2f32(third, 0.0f);

        GXPosition3f32(innerLeft, innerTop, 0.0f);
        GXTexCoord2f32(third, third);

        GXPosition3f32(x, innerTop, 0.0f);
        GXTexCoord2f32(0.0f, third);



        GXPosition3f32(innerLeft, y, 0.0f);
        GXTexCoord2f32(third, 0.0f);

        GXPosition3f32(innerRight, y, 0.0f);
        GXTexCoord2f32(twothirds, 0.0f);

        GXPosition3f32(innerRight, innerTop, 0.0f);
        GXTexCoord2f32(twothirds, third);

        GXPosition3f32(innerLeft, innerTop, 0.0f);
        GXTexCoord2f32(third, third);



        GXPosition3f32(innerRight, y, 0.0f);
        GXTexCoord2f32(twothirds, 0.0f);

        GXPosition3f32(right, y, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);

        GXPosition3f32(right, innerTop, 0.0f);
        GXTexCoord2f32(1.0f, third);

        GXPosition3f32(innerRight, innerTop, 0.0f);
        GXTexCoord2f32(twothirds, third);



        GXPosition3f32(x, innerTop, 0.0f);
        GXTexCoord2f32(0.0f, third);

        GXPosition3f32(innerLeft, innerTop, 0.0f);
        GXTexCoord2f32(third, third);

        GXPosition3f32(innerLeft, innerBottom, 0.0f);
        GXTexCoord2f32(third, twothirds);

        GXPosition3f32(x, innerBottom, 0.0f);
        GXTexCoord2f32(0.0f, twothirds);



        GXPosition3f32(innerLeft, innerTop, 0.0f);
        GXTexCoord2f32(third, third);

        GXPosition3f32(innerRight, innerTop, 0.0f);
        GXTexCoord2f32(twothirds, third);

        GXPosition3f32(innerRight, innerBottom, 0.0f);
        GXTexCoord2f32(twothirds, twothirds);

        GXPosition3f32(innerLeft, innerBottom, 0.0f);
        GXTexCoord2f32(third, twothirds);



        GXPosition3f32(innerRight, innerTop, 0.0f);
        GXTexCoord2f32(twothirds, third);

        GXPosition3f32(right, innerTop, 0.0f);
        GXTexCoord2f32(1.0f, third);

        GXPosition3f32(right, innerBottom, 0.0f);
        GXTexCoord2f32(1.0f, twothirds);

        GXPosition3f32(innerRight, innerBottom, 0.0f);
        GXTexCoord2f32(twothirds, twothirds);



        GXPosition3f32(x, innerBottom, 0.0f);
        GXTexCoord2f32(0.0f, twothirds);

        GXPosition3f32(innerLeft, innerBottom, 0.0f);
        GXTexCoord2f32(third, twothirds);

        GXPosition3f32(innerLeft, bottom, 0.0f);
        GXTexCoord2f32(third, 1.0f);

        GXPosition3f32(x, bottom, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);



        GXPosition3f32(innerLeft, innerBottom, 0.0f);
        GXTexCoord2f32(third, twothirds);

        GXPosition3f32(innerRight, innerBottom, 0.0f);
        GXTexCoord2f32(twothirds, twothirds);

        GXPosition3f32(innerRight, bottom, 0.0f);
        GXTexCoord2f32(twothirds, 1.0f);

        GXPosition3f32(innerLeft, bottom, 0.0f);
        GXTexCoord2f32(third, 1.0f);



        GXPosition3f32(innerRight, innerBottom, 0.0f);
        GXTexCoord2f32(twothirds, twothirds);

        GXPosition3f32(right, innerBottom, 0.0f);
        GXTexCoord2f32(1.0f, twothirds);

        GXPosition3f32(right, bottom, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);

        GXPosition3f32(innerRight, bottom, 0.0f);
        GXTexCoord2f32(twothirds, 1.0f);
    }
}

void windowDispGX2_Waku_col(Mtx mtx, u16 gxTexMapID, GXColor color, f32 x, f32 y, f32 width, f32 height, f32 curve) {
    CameraEntry* camera = camGetCurPtr();
    Mtx mtx2;
    f32 top, left;
    f32 twothirds, third;
    f32 right, bottom, innerLeft, innerRight, innerTop, innerBottom;

    if (tpl_loaded) {
        if (gxTexMapID) {
            gxTexMapID = 0;
        }
        GXSetCullMode(GX_CULL_NONE);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
        GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
        fogOff();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetNumChans(0);
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, color);

        top = height / 2.0f;
        // FAKE: negated and halved in two steps with a no-op (f32) cast; `left = -width / 2.0f;` changes the object
        left = -width;
        left = (f32)(left / 2.0f);
        MTXTrans(mtx2, x - left, y - top, 0.0f);
        MTXConcat(mtx2, mtx, mtx2);
        MTXConcat(camera->view, mtx2, mtx2);
        GXLoadPosMtxImm(mtx2, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXLoadTexObj(&wakuTexObj[gxTexMapID], GX_TEXMAP0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 0x24);


        right = left + width;
        bottom = top - height;
        innerLeft = left + curve;
        innerTop = top - curve;
        innerRight = right - curve;
        innerBottom = curve + bottom;
        third = 0.33333334f;
        twothirds = 0.6666667f;

        GXPosition3f32(left, top, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);

        GXPosition3f32(innerLeft, top, 0.0f);
        GXTexCoord2f32(third, 0.0f);

        GXPosition3f32(innerLeft, innerTop, 0.0f);
        GXTexCoord2f32(third, third);

        GXPosition3f32(left, innerTop, 0.0f);
        GXTexCoord2f32(0.0f, third);



        GXPosition3f32(innerLeft, top, 0.0f);
        GXTexCoord2f32(third, 0.0f);

        GXPosition3f32(innerRight, top, 0.0f);
        GXTexCoord2f32(twothirds, 0.0f);

        GXPosition3f32(innerRight, innerTop, 0.0f);
        GXTexCoord2f32(twothirds, third);

        GXPosition3f32(innerLeft, innerTop, 0.0f);
        GXTexCoord2f32(third, third);



        GXPosition3f32(innerRight, top, 0.0f);
        GXTexCoord2f32(twothirds, 0.0f);

        GXPosition3f32(right, top, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);

        GXPosition3f32(right, innerTop, 0.0f);
        GXTexCoord2f32(1.0f, third);

        GXPosition3f32(innerRight, innerTop, 0.0f);
        GXTexCoord2f32(twothirds, third);



        GXPosition3f32(left, innerTop, 0.0f);
        GXTexCoord2f32(0.0f, third);

        GXPosition3f32(innerLeft, innerTop, 0.0f);
        GXTexCoord2f32(third, third);

        GXPosition3f32(innerLeft, innerBottom, 0.0f);
        GXTexCoord2f32(third, twothirds);

        GXPosition3f32(left, innerBottom, 0.0f);
        GXTexCoord2f32(0.0f, twothirds);



        GXPosition3f32(innerLeft, innerTop, 0.0f);
        GXTexCoord2f32(third, third);

        GXPosition3f32(innerRight, innerTop, 0.0f);
        GXTexCoord2f32(twothirds, third);

        GXPosition3f32(innerRight, innerBottom, 0.0f);
        GXTexCoord2f32(twothirds, twothirds);

        GXPosition3f32(innerLeft, innerBottom, 0.0f);
        GXTexCoord2f32(third, twothirds);



        GXPosition3f32(innerRight, innerTop, 0.0f);
        GXTexCoord2f32(twothirds, third);

        GXPosition3f32(right, innerTop, 0.0f);
        GXTexCoord2f32(1.0f, third);

        GXPosition3f32(right, innerBottom, 0.0f);
        GXTexCoord2f32(1.0f, twothirds);

        GXPosition3f32(innerRight, innerBottom, 0.0f);
        GXTexCoord2f32(twothirds, twothirds);



        GXPosition3f32(left, innerBottom, 0.0f);
        GXTexCoord2f32(0.0f, twothirds);

        GXPosition3f32(innerLeft, innerBottom, 0.0f);
        GXTexCoord2f32(third, twothirds);

        GXPosition3f32(innerLeft, bottom, 0.0f);
        GXTexCoord2f32(third, 1.0f);

        GXPosition3f32(left, bottom, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);



        GXPosition3f32(innerLeft, innerBottom, 0.0f);
        GXTexCoord2f32(third, twothirds);

        GXPosition3f32(innerRight, innerBottom, 0.0f);
        GXTexCoord2f32(twothirds, twothirds);

        GXPosition3f32(innerRight, bottom, 0.0f);
        GXTexCoord2f32(twothirds, 1.0f);

        GXPosition3f32(innerLeft, bottom, 0.0f);
        GXTexCoord2f32(third, 1.0f);



        GXPosition3f32(innerRight, innerBottom, 0.0f);
        GXTexCoord2f32(twothirds, twothirds);

        GXPosition3f32(right, innerBottom, 0.0f);
        GXTexCoord2f32(1.0f, twothirds);

        GXPosition3f32(right, bottom, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);

        GXPosition3f32(innerRight, bottom, 0.0f);
        GXTexCoord2f32(twothirds, 1.0f);
    }
}

s32 windowCheckID(s32 id) {
    return windowGetPointer(id)->flags & WINDOW_FLAG_BUSY;
}

WindowEntry* windowGetPointer(s32 id) {
    return &WinObjects[id];
}

GXTexObj* getWakuTexObj(s32 id) {
    return &wakuTexObj[id];
}

