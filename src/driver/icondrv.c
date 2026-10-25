#include "driver/icondrv.h"
#include "driver/camdrv.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "driver/dispdrv.h"
#include "driver/mapdrv.h"
#include "evt/evt_cmd.h"
#include "manager/dvdmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include "sdk/texPalette.h"
#include <string.h>
#include "driver/shadowdrv.h"

#include <stdio.h>
#include "icon_ids.h"


//.sbss
static IconWork work[2];
BOOL icon_bin_ok;
BOOL icon_tpl_ok;
IconBinary* icon_bin;
TEXPalette* icon_tpl;

//.sdata

typedef struct IconFrame {
    u16 tplId; // 0x0
    u16 delay; // 0x2
} IconFrame;

typedef struct IconBinData {
    u16 type;            // 0x0
    u16 width;           // 0x2
    u16 height;          // 0x4
    u16 numIcons;        // 0x6
    IconFrame frames[1]; // 0x8
} IconBinData;


#define ICON_ANIM_TPL(data, dst)                                   \
    if ((data)->numIcons > 1) {                                    \
        s32 total = 0;                                             \
        s32 i;                                                     \
        s32 t;                                                     \
        for (i = 0; i < (data)->numIcons; i++) {                   \
            total += (data)->frames[i].delay;                      \
        }                                                          \
        t = gp->retraceCount % total;                              \
        for (i = 0; i < (data)->numIcons; i++) {                   \
            if (t < (data)->frames[i].delay) {                     \
                dst = (data)->frames[i].tplId;                     \
                break;                                             \
            }                                                      \
            t -= (data)->frames[i].delay;                          \
        }                                                          \
    }

#define ICON_ANIM_TPL_S(data, dst)                                   \
    if ((data)->numIcons > 1) {                                    \
        s32 i;                                                     \
        s32 t;                                                     \
        s32 total = 0;                                             \
        for (i = 0; i < (data)->numIcons; i++) {                   \
            total += (data)->frames[i].delay;                      \
        }                                                          \
        t = (s32)gp->retraceCount % total;                              \
        for (i = 0; i < (data)->numIcons; i++) {                   \
            if (t < (data)->frames[i].delay) {                     \
                dst = (data)->frames[i].tplId;                     \
                break;                                             \
            }                                                      \
            t -= (data)->frames[i].delay;                          \
        }                                                          \
    }

#ifdef VERSION_G8ME01
// US: language-specific variants of icons 0x49 and 0x67 (indexed by gp->language)
u16 icon_049_lang_tbl[6] = {0x49, 0x4A, 0x4B, 0x4C, 0x49, 0x4D};
u16 icon_067_lang_tbl[6] = {0x67, 0x68, 0x69, 0x6A, 0x67, 0x6B};
#endif

#ifdef VERSION_G8ME01
static inline IconBinData* iconGetBinData(s32 iconId) {
    IconBinary* bin = icon_bin;
    if (!icon_tpl_ok || !icon_bin_ok) {
        return NULL;
    }
    switch ((u16)iconId) {
        case 0x49:
            iconId = icon_049_lang_tbl[gp->language];
            break;
        case 0x67:
            iconId = icon_067_lang_tbl[gp->language];
            break;
    }
    return (IconBinData*)((u8*)bin + bin->offset[(u16)iconId]);
}
#else
static inline IconBinData* iconGetBinData(u16 iconId) {
    IconBinary* bin = icon_bin;
    if (!icon_tpl_ok || !icon_bin_ok) {
        return NULL;
    }
    return (IconBinData*)((u8*)bin + bin->offset[iconId]);
}
#endif

// local prototypes
void _callback_tpl(s32 error, DVDFileInfo* info);
void _callback_bin(s32 error, DVDFileInfo* info);
void iconDisp(CameraId camId, void* param);
void iconGX(Mtx mtx, IconEntry* entry);

// TODO: inline function? I see nothing in the symbol map
#define iconGetWork() (gp->inBattle ? &work[1] : &work[0])


// TODO: remove? no function in original symbol map
inline IconEntry* iconFindIn(IconWork* wp, const char* name) {
    int i;
    IconEntry* entry = wp->entries;

    for (i = 0; i < wp->count; i++, entry++) {
        if (entry->uflags & 1 && !strcmp(entry->name, name)) {
            return entry;
        }
    }
    return NULL;
}

inline IconEntry* iconGetEntry(const char* name) {
    IconWork* wp = iconGetWork();
    IconEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if (entry->uflags & 1 && !strcmp(entry->name, name)) {
            return entry;
        }
    }
    return NULL;
}

void _callback_tpl(s32 error, DVDFileInfo* info) {
    DVDEntry* entry = info->cb.userData;
    UnpackTexPalette(icon_tpl);
    DVDMgrClose(entry);
    icon_tpl_ok = TRUE;
}

void _callback_bin(s32 error, DVDFileInfo* info) {
    DVDMgrClose(info->cb.userData);
    icon_bin_ok = TRUE;
}

void iconInit(void) {
    char path[128];
    DVDEntry* entry;
    u32 size;
    IconWork* wp;

    work[0].count = 32;
    work[0].entries = __memAlloc(HEAP_DEFAULT, sizeof(IconEntry) * work[0].count);
    memset(work[0].entries, 0, sizeof(IconEntry) * work[0].count);
    wp = &work[1];

    wp->count = 32;
    wp->entries = __memAlloc(HEAP_DEFAULT, sizeof(IconEntry) * wp->count);
    memset(wp->entries, 0, sizeof(IconEntry) * wp->count);
    icon_tpl = NULL;
    icon_bin = NULL;
    icon_tpl_ok = FALSE;
    icon_bin_ok = FALSE;

    sprintf(path, "%s/icon.tpl", getMarioStDvdRoot());
    entry = DVDMgrOpen(path, 2, 0);
    if (entry) {
        size = OSRoundUp32B(DVDMgrGetLength(entry));
        icon_tpl = __memAlloc(HEAP_DEFAULT, size);
        entry->info.cb.userData = entry; // this file's userData is a pointer to the entry
        DVDMgrReadAsync(entry, icon_tpl, size, 0, _callback_tpl);
    }

    sprintf(path, "%s/icon.bin", getMarioStDvdRoot());
    entry = DVDMgrOpen(path, 2, 0);
    if (entry) {
        size = OSRoundUp32B(DVDMgrGetLength(entry));
        icon_bin = __memAlloc(HEAP_DEFAULT, size);
        entry->info.cb.userData = entry; // this file's userData is a pointer to the entry
        DVDMgrReadAsync(entry, icon_bin, size, 0, _callback_bin);
    }
}

void iconTexSetup(void) {
    ; // stubbed in retail
}

void iconReInit(void) {
    IconWork* wp = iconGetWork();
    memset(wp->entries, 0, sizeof(IconEntry) * wp->count);
}


void iconMain(void) {
    IconWork* wp;
    int i;
    IconEntry* entry;
    IconBinData* data;

    wp = iconGetWork();
    if (!icon_tpl_ok || !icon_bin_ok)
        return;
    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (entry->uflags & 1) {
            if (entry->uflags & 0x2000) {
                entry->uflags &= ~0x2000;
#ifdef VERSION_G8ME01
                data = iconGetBinData(entry->iconId);
#endif
                // warning: data may be invalid (JP)
                entry->width = data->width;
                entry->height = data->height;
                entry->numIcons = data->numIcons;
                entry->currIcon = 0;
                entry->tplId = data->frames[0].tplId;
                entry->frameDelay = data->frames[0].delay;
                if (data->type & 1) {
                    entry->uflags |= 0x1000;
                }
            }
            if (entry->uflags & 0x1000) {
                if (entry->frameDelay) {
                    entry->frameDelay--;
                }
                if (!entry->frameDelay) {
                    entry->currIcon++;
                    if (entry->currIcon >= entry->numIcons) {
                        entry->currIcon = 0;
                    }
                    data = iconGetBinData(entry->iconId);
                    entry->tplId = data->frames[entry->currIcon].tplId;
                    entry->frameDelay = data->frames[entry->currIcon].delay;
                }
            }
            if (!(entry->uflags & 2)) {
                if (entry->uflags & 0x10) {
                    dispEntry(CAMERA_2D, 200.0f, 1, iconDisp, entry);
                }
                else if (entry->uflags & 0x100) {
                    shadowEntry(entry->position.x, entry->position.y, entry->position.z, 10.0f);
                    dispEntry(CAMERA_3D_EFFECTA, dispCalcZ(&entry->position), 1, iconDisp, entry);
                } else if (entry->uflags & 0x200) {
                    shadowEntry(entry->position.x, entry->position.y, entry->position.z, 10.0f);
                    dispEntry(CAMERA_3D_EFFECTB, dispCalcZ(&entry->position), 1, iconDisp, entry);
                } else {
                    shadowEntry(entry->position.x, entry->position.y, entry->position.z, 10.0f);
                    if (entry->color.a != 0xFF) {
                        dispEntry(CAMERA_3D, dispCalcZ(&entry->position), 2, iconDisp, entry);
                    } else {
                        dispEntry(CAMERA_3D, dispCalcZ(&entry->position), 1, iconDisp, entry);
                    }
                }
            }
        }
    }
}

void iconEntry(const char* name, u16 iconId) {
    IconWork* wp = iconGetWork();
    int i;
    IconEntry* entry = iconFindIn(wp, name); // inline
    IconBinData* data;

    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (!(entry->uflags & 1)) { // looking for unused entry to allocate
            break;
        }
    }

    memset(entry, 0, sizeof(IconEntry));
    entry->position = (Vec){ 0.0f, 0.0f, 0.0f };
    entry->scale = 1.0f;
    entry->color = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    strcpy(entry->name, name);
    entry->iconId = iconId;
    data = iconGetBinData(entry->iconId);
    if (data) {
        entry->width = data->width;
        entry->height = data->height;
        entry->numIcons = data->numIcons;
        entry->currIcon = 0;
        entry->tplId = data->frames[0].tplId;
        entry->frameDelay = data->frames[0].delay;
        if (data->type & 1) {
            entry->uflags |= 0x1000;
        }
    } else {
        entry->uflags |= 0x2000;
    }
    entry->uflags |= 1;
    if (mapGetWork()->entries[0].flags & 2) {
        if (!strncmp(gp->currentMapName, "aji", 3) && evtGetValue(0, GSW_STORY_PROGRESS) == 0x178) {
            entry->uflags &= ~0x40;
        } else {
            entry->uflags |= 0x40;
        }
    }
}

void iconEntry2D(const char* name, s16 iconId) {
    IconWork* wp = iconGetWork();
    int i;
    IconEntry* entry = iconFindIn(wp, name); // inline
    IconBinData* data;

    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (!(entry->uflags & 1)) { // looking for unused entry to allocate
            break;
        }
    }

    memset(entry, 0, sizeof(IconEntry));
    entry->position = (Vec){ 0.0f, 0.0f, 0.0f };
    entry->scale = 1.0f;
    entry->color = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    strcpy(entry->name, name);
    entry->iconId = iconId;
    data = iconGetBinData(entry->iconId);
    if (data) {
        entry->width = data->width;
        entry->height = data->height;
        entry->numIcons = data->numIcons;
        entry->currIcon = 0;
        entry->tplId = data->frames[0].tplId;
        entry->frameDelay = data->frames[0].delay;
        if (data->type & 1) {
            entry->uflags |= 0x1000;
        }
    } else {
        entry->uflags |= 0x2000;
    }
    entry->uflags |= 0x11;
}

void iconDelete(const char* name) {
    IconWork* wp = iconGetWork();
    IconEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if (entry->uflags & 1 && !strcmp(entry->name, name)) {
            entry->uflags &= ~1; // no longer in use
            break;
        }
    }
}

void iconChange(const char* name, u16 iconId) {
    IconEntry* entry = iconNameToPtr(name); // inline
    IconBinData* data;

    entry->iconId = iconId;
    data = iconGetBinData(entry->iconId);
    entry->width = data->width;
    entry->height = data->height;
    entry->numIcons = data->numIcons;
    entry->currIcon = 0;
    entry->tplId = data->frames[0].tplId;
    entry->frameDelay = data->frames[0].delay;
    if (data->type & 1) {
        entry->uflags |= 0x1000;
    }
    entry->uflags |= 1;
}

void iconDisp(CameraId camId, void* param) {
    IconEntry* entry = param; // cast to correct type
    CameraEntry* camera;
    Mtx trans, scale, rot, mtx;

    camera = camGetPtr(camId);
    MTXTrans(trans, entry->position.x, entry->position.y, entry->position.z);
    MTXScale(scale, entry->scale, entry->scale, entry->scale);
    MTXRotRad(rot, 'y', MTXDegToRad(-camera->viewYaw));
    MTXConcat(trans, rot, mtx);
    MTXConcat(mtx, scale, mtx);
    MTXConcat(camera->view, mtx, mtx);
    iconGX(mtx, entry);
}

void iconDispGxAlpha(Vec *position, s16 flags, u16 iconId, u8 alpha, f32 scale)
{
  IconEntry entry;
  Mtx transmtx;
  Mtx scalemtx;
  Mtx rotmtx;
  Mtx mtx;
  CameraEntry *camera;
  IconBinData *data;
  IconEntry *ep;
  camera = camGetCurPtr();
  ep = &entry;
  if ((!icon_tpl_ok) || (!icon_bin_ok))
  {
    return;
  }
  PSMTXTrans(transmtx, position->x, position->y, position->z);
  PSMTXScale(scalemtx, scale, scale, scale);
  PSMTXRotRad(rotmtx, 'y', (-camera->viewYaw) * 0.01745329252f);
  PSMTXConcat(transmtx, rotmtx, mtx);
  PSMTXConcat(mtx, scalemtx, mtx);
  PSMTXConcat(camera->view, mtx, mtx);
  entry.color = (GXColor){0xFF, 0xFF, 0xFF, alpha};
  entry.uflags = flags;
  iconId++;
  iconId--;
  entry.iconId = iconId;
  data = iconGetBinData(iconId);
  entry.width = data->width;
  entry.height = data->height;
  entry.tplId = data->frames[0].tplId;
  entry.frameDelay = data->frames[0].delay;
  if (data->numIcons > 1)
  {
    s32 i;
    s32 t;
    s32 total = 0;
    for (i = 0; i < data->numIcons; i++)
    {
      total += data->frames[i].delay;
    }

    t = ((s32) gp->retraceCount) % total;
    for (i = 0; i < data->numIcons; i++)
    {
      if (t < data->frames[i].delay)
      {
        entry.tplId = data->frames[i].tplId;
        break;
      }
      t -= data->frames[i].delay;
    }

  }
  ;
  iconGX(mtx, ep);
}

void iconDispGx(Vec* position, s16 flags, u16 iconId, f32 scale) {
    iconDispGxAlpha(position, flags, iconId, 0xFF, scale);
}

void iconDispGx2(Mtx mtx, s16 flags, s16 iconId)
{
  IconEntry entry;
  Mtx m;
  CameraEntry *camera;
  IconBinData *data;
  IconEntry *ep;
  u16 id;
  camera = camGetCurPtr();
  ep = &entry;
  if ((!icon_tpl_ok) || (!icon_bin_ok))
  {
    return;
  }
  entry.uflags = flags;
  entry.color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
  if (icon_bin_ok && icon_bin_ok)
  {
  }
#ifdef VERSION_G8ME01
  /* FAKE: (u16)(s16) round-trip is a no-op on the u16 field (the s32 id goes through s16 first); it steers MWCC's register choice */
  ep->iconId = (u16)(s16)iconId;
  data = iconGetBinData((u16)iconId);
#else
  entry.iconId = (id = iconId);
  data = iconGetBinData(id);
#endif
  entry.width = data->width;
  entry.height = data->height;
  entry.tplId = data->frames[0].tplId;
  entry.frameDelay = data->frames[0].delay;
  if (data->numIcons > 1)
  {
    s32 i;
    s32 t;
    s32 total = 0;
    for (i = 0; i < data->numIcons; i++)
    {
      total += data->frames[i].delay;
    }

    t = ((s32) gp->retraceCount) % total;
    for (i = i * 0; i < data->numIcons; i++)
    {
      if (t < data->frames[i].delay)
      {
        entry.tplId = data->frames[i].tplId;
        break;
      }
      t -= data->frames[i].delay;
    }

  }
  ;
  PSMTXConcat(camera->view, mtx, m);
  iconGX(m, ep);
}

void iconDispGxCol(Mtx pos, s16 flags, s32 iconId, GXColor color) {
    IconEntry entry;
    Mtx m;
    CameraEntry* camera;
    IconBinData* data;
    IconEntry* ep;
#ifndef VERSION_G8ME01
    u16 id;
#endif

    camera = camGetCurPtr();
    ep = &entry;
    if (!icon_tpl_ok || !icon_bin_ok) {
        return;
    }
    entry.uflags = flags;
    entry.color = color;
    if (icon_bin_ok && icon_bin_ok) {
    }
#ifdef VERSION_G8ME01
    /* FAKE: (u16)(s16) round-trip is a no-op on the u16 field (the s32 id goes through s16 first); it steers MWCC's register choice */
    ep->iconId = (u16)(s16)iconId;
    data = iconGetBinData((u16)iconId);
#else
    entry.iconId = (id = iconId);
    data = iconGetBinData(id);
#endif
    entry.width = data->width;
    entry.height = data->height;
    entry.tplId = data->frames[0].tplId;
    entry.frameDelay = data->frames[0].delay;
    ICON_ANIM_TPL_S(data, entry.tplId);
    MTXConcat(camera->view, pos, m);
    iconGX(m, ep);
}

#ifdef VERSION_G8ME01
void iconGetTexObj(GXTexObj* texObj, s32 iconId) {
#else
void iconGetTexObj(GXTexObj* texObj, s16 iconId) {
#endif
    static char dummy[] ATTRIBUTE_ALIGN(32) = "";
    IconBinData* data;
    u16 tplId;

    if (!icon_tpl_ok || !icon_bin_ok) {
        GXInitTexObj(texObj, dummy, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
        return;
    }
#ifdef VERSION_G8ME01
    data = iconGetBinData(iconId);
#else
    data = iconGetBinData((u16)iconId);
#endif
    tplId = data->frames[0].tplId;
    ICON_ANIM_TPL_S(data, tplId);
    TEXGetGXTexObjFromPalette(icon_tpl, texObj, tplId);
    GXInitTexObjLOD(texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
}

#ifdef VERSION_G8ME01
void iconGetWidthHight(u16* width, u16* height, s32 iconId) {
#else
void iconGetWidthHight(u16* width, u16* height, s16 iconId) {
#endif
    IconBinData* entry;
    TEXDescriptor* descriptor;

    entry = iconGetBinData(iconId);
    *width = entry->width;
    *height = entry->height;
    if (!*width && !*height) { // no tpl override
        descriptor = TEXGet(icon_tpl, (u16)iconId);
        *width = descriptor->textureHeader->width;
        *height = descriptor->textureHeader->height;
    }
}

void iconGX(Mtx mtx, IconEntry* entry) {
    static char dummy[] ATTRIBUTE_ALIGN(32) = "";
    f32 x, y, w, hn;
    f32 h;
    Mtx scalemtx, m;
    GXTexObj texObj;
    Vec pos = { 0.0f, 0.0f, 0.0f };
    CameraEntry* camera;

    pos.x = mtx[0][3];
    pos.y = mtx[1][3];
    pos.z = mtx[2][3];
    MTXScale(scalemtx, (entry->uflags & 4) ? -1.0f : 1.0f, (entry->uflags & 8) ? -1.0f : 1.0f, 1.0f);
    MTXConcat(mtx, scalemtx, m);
    camera = camGetPtr(CAMERA_2D);
    if (camGetCurPtr() != camera && (entry->uflags & 0x10)) {
        MTXScale(scalemtx, 1.0f, -1.0f, 1.0f);
        MTXConcat(m, scalemtx, m);
    }
    GXLoadPosMtxImm(m, 0);
    GXSetCurrentMtx(0);
    {
        u16 tplId = entry->tplId;
        if (!icon_tpl_ok || !icon_bin_ok) {
            GXInitTexObj(&texObj, dummy, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
        } else {
            TEXGetGXTexObjFromPalette(icon_tpl, &texObj, tplId);
            GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        }
    }
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    if (!(entry->uflags & 0x10)) {
        mapSetMaterialLight(4, pos);
    } else {
        GXSetNumChans(1);
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
    if (entry->uflags & 0x20) {
        GXSetNumTevStages(3);
        GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP1);
        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP2);
        GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP3);
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0x55, 0x55, 0x55, 0xFF });
        GXSetTevColor(GX_TEVREG1, entry->color);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    } else {
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevColor(GX_TEVREG0, entry->color);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C0, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
        if (!(entry->uflags & 0x10) && !(entry->uflags & 0x40) && !(entry->uflags & 0x80) &&
            (mapGetWork()->entries[0].flags & 2)) {
            GXColor blend;
            mapGetBlend(&blend);
            GXSetTevColor(GX_TEVREG1, blend);
            GXSetNumTevStages(3);
            GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C1, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A1, GX_CA_ZERO);
        }
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    }
    if (!(entry->uflags & 0x10)) {
        mapSetMaterialFog();
    } else {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    }
    if (entry->color.a < 0xFF) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    } else {
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    }
    if (entry->uflags & 0x10) {
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    }
    if (entry->uflags & 0x400) {
        GXSetCullMode(GX_CULL_BACK);
    } else if (entry->uflags & 0x800) {
        GXSetCullMode(GX_CULL_FRONT);
    } else {
        GXSetCullMode(GX_CULL_NONE);
    }
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    if (!entry->width && !entry->height) {
        w = GXGetTexObjWidth(&texObj);
        h = GXGetTexObjHeight(&texObj);
    } else {
        w = entry->width;
        h = entry->height;
    }
    x = -w / 2.0f;
    y = h;
    hn = -h;
    if (!(entry->uflags & 0x10)) {
        x *= 0.6f;
        y *= 0.6f;
        w *= 0.6f;
        hn *= 0.6f;
        if (entry->iconId == ICON_18B) {
            x *= 0.8f;
            y *= 0.8f;
            w *= 0.8f;
            hn *= 0.8f;
        } else if (entry->iconId == ICON_18D) {
            x *= 0.9f;
            y *= 0.9f;
            w *= 0.9f;
            hn *= 0.9f;
        }
    }
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x + w, y, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(x + w, y + hn, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x, y + hn, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    if (entry->uflags & 0x20) {
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    }
}

IconEntry* iconNameToPtr(const char* name) {
    IconWork* wp = iconGetWork();
    IconEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if (entry->uflags & 1 && !strcmp(entry->name, name)) {
            return entry;
        }
    }
    return NULL;
}

void iconSetPos(const char* name, f32 x, f32 y, f32 z) {
    IconWork* workptr = iconGetWork();
    IconEntry* entry = iconNameToPtr(name); // inline

    if (entry) {
        Vec temp = { 0 };
        temp.x = x;
        temp.y = y;
        temp.z = z;
        entry->position = temp;
    }
}

void iconFlagOn(const char* name, s16 flags) {
    IconEntry* entry = iconNameToPtr(name); // inline
    if (entry) {
        entry->uflags |= flags;
    }
}

void iconFlagOff(const char* name, s16 mask) {
    IconEntry* entry = iconNameToPtr(name); // inline
    if (entry) {
        entry->uflags &= ~mask;
    }
}

void iconSetScale(const char* name, f32 scale) {
    IconEntry* entry = iconNameToPtr(name); // inline
    if (entry) {
        entry->scale = scale;
    }
}

void iconSetAlpha(const char* name, u8 alpha) {
    IconEntry* entry = iconNameToPtr(name); // inline
    if (entry) {
        GXColor color = entry->color;
        color.a = alpha;
        entry->color = color;
    }
}

static u16 suuji[] = { ICON_1D9, ICON_1DB, ICON_1DD, ICON_1DF, ICON_1E1, ICON_1E3, ICON_1E5, ICON_1E7, ICON_1E9, ICON_1EB };   // 0-9
u16 suuji_s[] = { ICON_1DA, ICON_1DC, ICON_1DE, ICON_1E0, ICON_1E2, ICON_1E4, ICON_1E6, ICON_1E8, ICON_1EA, ICON_1EC }; // 0-9 small

s32 iconNumberDispGx(Mtx mtx, s32 number, BOOL small, GXColor color) {
    s32 digits[5];
    Mtx m;
    s32 n;
    s32* dp;
    s32 i;
    s32 count;
    s32 ret;
    f32 width;

    width = !small ? 20.0f : 16.0f;
    ret = 0;
    n = number % 100000;
    digits[4] = n / 10000;
    n %= 10000;
    digits[3] = n / 1000;
    n %= 1000;
    digits[2] = n / 100;
    n %= 100;
    digits[1] = n / 10;
    n %= 10;
    digits[0] = n;
    if (number / 100000) {
        count = 5;
    } else if (digits[4]) {
        count = 5;
    } else if (digits[3]) {
        count = 4;
    } else if (digits[2]) {
        count = 3;
    } else if (digits[1]) {
        count = 2;
    } else {
        count = 1;
    }
    for (dp = digits, i = 0; i < count; dp++, i++) {
        MTXTrans(m, (f32)-i * width, 0.0f, 0.0f);
        MTXConcat(mtx, m, m);
        iconDispGxCol(m, 0x10, !small ? (s32)suuji[*dp] : (s32)suuji_s[*dp], color);
        ret += width;
    }
    return ret;
}

s32 iconNumberDispGx3D(Mtx mtx, s32 number, BOOL small, GXColor color) {
    // One element short: digits[4] below writes past the array (the retail code has the same 16-byte
    // array; a fifth element changes the stack offsets).
    s32 digits[4];
    Mtx m;
    s32 n;
    s32* dp;
    s32 i;
    s32 count;
    s32 ret;
    f32 width;

    width = !small ? 10.0f : 8.0f;
    ret = 0;
    n = number % 100000;
    digits[4] = n / 10000;
    n %= 10000;
    digits[3] = n / 1000;
    n %= 1000;
    digits[2] = n / 100;
    n %= 100;
    digits[1] = n / 10;
    n %= 10;
    digits[0] = n;
    if (number / 100000) {
        count = 5;
    } else if (digits[4]) {
        count = 5;
    } else if (digits[3]) {
        count = 4;
    } else if (digits[2]) {
        count = 3;
    } else if (digits[1]) {
        count = 2;
    } else {
        count = 1;
    }
    for (dp = digits, i = 0; i < count; dp++, i++) {
        MTXTrans(m, (f32)-i * width, 0.0f, 0.0f);
        MTXConcat(mtx, m, m);
        iconDispGxCol(m, 0, !small ? (s32)suuji[*dp] : (s32)suuji_s[*dp], color);
        ret += width;
    }
    return ret;
}
