/* "imgdrv" - Image Driver
 * Images for event scripts: screen captures (imgCapture) drawn as planes or projections (imgDisp,
 * imgDisp_ProjPlane).
 */
#include "driver/imgdrv.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include <string.h>
#include "system.h"
#include "driver/offscreendrv.h"
#include "driver/shadowdrv.h"
#include "libc_decls.h"
#include "driver/mapdrv.h"
#include "sdk/DEMOInit.h"

//.data

//.sbss
static ImageWork work;

//.sdata
static ImageWork* wp = &work;
#pragma explicit_zero_data on
#pragma explicit_zero_data off

//local prototypes
static void zFill(void);
void imgDisp(CameraId camId, void* param);
void imgDisp_ProjPlane(CameraId camId, void* param);
void imgCapture_Prim(ImageEntry* entry, s32 id);
void imgCapture(CameraId camId, void* param);


void imgShadowDisp(CameraId camId, void* param);

static void zFill(void) {
    static s32 depth_image[32] ATTRIBUTE_ALIGN(32) = { // GX texture: 32-byte aligned (US .data placement shows it)
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    CameraEntry* camera;
    f32 y0, y1, x0, x1;
    f32 zpos;
    GXTexObj obj;
    Mtx mtx;

    camera = camGetCurPtr();
    {
        f32 t = (6.2831855f * camera->fovY) / 360.0f;
        y0 = (camera->near + camera->far) / 2.0f;
        y0 = y0 * (f32)tan(0.5f * t);
    }
    x1 = (y0 * camera->aspect);
    y1 = -y0;
    x0 = -x1;
    GXInitTexObj(&obj, depth_image, 4, 4, GX_TF_Z8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetZTexture(GX_ZT_REPLACE, GX_TF_Z8, 0);
    GXSetNumChans(0);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_TRUE);
    GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    GXSetColorUpdate(GX_FALSE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    MTXIdentity(mtx);
    GXLoadPosMtxImm(camera->view, 0);
    GXSetCurrentMtx(0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    zpos = -1000.0f;
    GXPosition3f32(x0, y0, zpos);
    GXTexCoord2u8(0, 0);
    GXPosition3f32(x1, y0, zpos);
    GXTexCoord2u8(1, 0);
    GXPosition3f32(x1, y1, zpos);
    GXTexCoord2u8(1, 1);
    GXPosition3f32(x0, y1, zpos);
    GXTexCoord2u8(0, 1);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);
    GXSetColorUpdate(GX_TRUE);
}


void imgDisp(CameraId camId, void* param) {
    ImageEntry* entry = param;
    ImageShadow* sh;
    GXTexObj* texs[3];
    void* datas[3];
    Mtx mtxs[3];
    MtxPtr mp[3];
    Mtx m3, m4, m2, m1;
    Vec v1, v2;
    CameraEntry* cam;
    int i;
    int first = -1;
    f32 fh, fw, fy, fx, scale, zoff, fl, ft, fr, fb;
    ImageEntry* e2;

    for (i = 0; i < 3; i++) {
        sh = entry->shadows + i;
        if (!(sh->flags & 4)) {
            texs[i] = NULL;
            datas[i] = NULL;
        } else if (sh->flags & 0x10) {
            texs[i] = NULL;
            datas[i] = NULL;
        } else {
            if (entry->flags & 4) {
                fx = sh->rect.left;
                fy = sh->rect.top;
                fw = sh->rect.width;
                fh = sh->rect.height;
            } else {
                fx = sh->x;
                fy = sh->y;
                fw = entry->width;
                fh = entry->height;
            }
            PSMTXTrans(m1, -((f32)sh->rect.left - fx) / (f32)sh->rect.width, -((f32)sh->rect.top - fy) / (f32)sh->rect.height, 0.0f);
            PSMTXScale(m2, fw / (f32)sh->rect.width, fh / (f32)sh->rect.height, 1.0f);
            PSMTXConcat(m1, m2, mtxs[i]);
            texs[i] = &sh->texObj;
            datas[i] = sh->data;
        }
        if (texs[i] != NULL) {
            mp[i] = mtxs[i];
            if (first == -1) first = i;
        } else {
            mp[i] = NULL;
        }
    }
    if (first == -1) {
        entry->flags |= 0x10000000;
        return;
    }
    entry->flags &= ~0x10000000;
    animSetPaperTexMtx(mp[0], mp[1], mp[2]);
    animSetPaperTexObj(texs[0], texs[1], texs[2], datas[0], datas[1], datas[2],
                       entry->shadows[0].offscreenId != -1 ? 1 : 0, entry->shadows[1].offscreenId != -1 ? 1 : 0);
    if (entry->animName != NULL) {
        animPoseSetAnim(entry->poseId, entry->animName, FALSE);
    }
    animPoseSetStartTime(entry->poseId, entry->animTime);
    animPoseMain(entry->poseId);
    animPoseSetMaterialLightFlagOn(entry->poseId, 1);
    entry->loopTimes = animPoseGetLoopTimes(entry->poseId);
    if (entry->flags & 8) {
        zFill();
        scale = 1.0f;
        zoff = 0.0f;
    } else {
        f32 d, t;
        t = 3.141592f * (camGetPtr(CAMERA_3D)->fovY / 2.0f / 180.0f);
        d = 24.0f / (f32)tan(t);
        PSVECSubtract(&camGetPtr(CAMERA_3D)->cameraPos, &entry->point, &v1);
        v2 = *(Vec*)camGetPtr(CAMERA_3D)->view[2];
        PSVECNormalize(&v2, &v2);
        v1.x = v1.x * v2.x;
        v1.y = v1.y * v2.y;
        v1.z = v1.z * v2.z;
        t = PSVECMag(&v1);
        scale = t / d;
        zoff = d - t;
    }
    {
        if (entry->flags & 4) {
            fl = entry->shadows[first].rect.left; ft = entry->shadows[first].rect.top; fr = entry->shadows[first].rect.width; fb = entry->shadows[first].rect.height;
        } else {
            fl = entry->shadows[first].x; ft = entry->shadows[first].y; fr = entry->width; fb = entry->height;
        }
    }
    PSMTXTrans(m3, 64.0f * ((entry->offsetX + fl) / (f32)((GXRenderModeObj*)DEMOGetRenderModeObj())->fbWidth), 48.0f * (-(entry->offsetY + ft) / (f32)((GXRenderModeObj*)DEMOGetRenderModeObj())->efbHeight), zoff);
    PSMTXTrans(m4, -32.0f, 24.0f, 0.0f);
    PSMTXConcat(m3, m4, m3);
    PSMTXScale(m4, fr / (f32)((GXRenderModeObj*)DEMOGetRenderModeObj())->fbWidth, fb / (f32)((GXRenderModeObj*)DEMOGetRenderModeObj())->efbHeight, fr / (f32)((GXRenderModeObj*)DEMOGetRenderModeObj())->fbWidth);
    PSMTXConcat(m3, m4, m3);
    PSMTXTrans(m4, 32.0f, -24.0f, 0.0f);
    PSMTXConcat(m3, m4, m3);
    PSMTXConcat(entry->mtx, m3, m3);
    animPoseDrawMtx(entry->poseId, m3, 1, 0.0f, scale);
    animPoseDrawMtx(entry->poseId, m3, 2, 0.0f, scale);
    animPoseDrawMtx(entry->poseId, m3, 3, 0.0f, scale);
    animSetPaperTexMtx(NULL, NULL, NULL);
    e2 = wp->entries;
    for (i = 0; i < wp->count; i++, e2++) {
        if ((e2->flags & 1) && e2->poseId != -1 && !(e2->flags & 2) && strcmp(e2->name, entry->name) != 0 &&
            strcmp(animPoseGetAnimBaseDataPtr(e2->poseId)->fileName, animPoseGetAnimBaseDataPtr(entry->poseId)->fileName) == 0)
            break;
    }
    if (i < wp->count) sysWaitDrawSync();
}


void imgDisp_ProjPlane(CameraId camId, void* param) {
    ImageEntry* entry = param;
    static const Vec lightPos = {0.0f, 0.0f, 0.0f};
    static const GXColor color1 = {0xFF, 0xFF, 0xFF, 0xFF};
    static const GXColor color2 = {0xFF, 0xFF, 0xFF, 0xFF};
    Mtx mtx;
    Mtx mtx2;
    Vec pos;
    s32 flags;

    switch (entry->shadowId) {
        case 0:
            return;
        case 1:
            flags = 0x20000;
            PSMTXIdentity(mtx);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            break;
        case 2:
            flags = 0x40000;
            PSMTXTrans(mtx, 0.0f, 0.0f, entry->offsetZ);
            GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            break;
    }
    pos = lightPos;
    PSMTXMultVec(mtx, &pos, &pos);
    mapSetMaterialLight(0, pos);
    mapSetMaterialLastStageBlend(flags, color1, color2);
    mapSetMaterialTev(0, 0, flags, mtx);
    PSMTXConcat(camGetCurPtr()->view, mtx, mtx2);
    GXLoadPosMtxImm(mtx2, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-32.0f, 24.0f, 0.0f);
    GXColor4u8(0, 0, 0, 0);
    GXPosition3f32(32.0f, 24.0f, 0.0f);
    GXColor4u8(0, 0, 0, 0);
    GXPosition3f32(32.0f, -24.0f, 0.0f);
    GXColor4u8(0, 0, 0, 0);
    GXPosition3f32(-32.0f, -24.0f, 0.0f);
    GXColor4u8(0, 0, 0, 0);
}


static inline void imgCaptureCopy(ImageShadow* sh, ImageEntry* entry) {
    s32 fmt, dstFmt, alloc;
    u16 h, w;
    u32 size;
    smartEntry* data;
    sh->rect.left = sh->x;
    sh->rect.top = sh->y;
    sh->rect.width = entry->width;
    sh->rect.height = entry->height;
    h = sh->rect.height;
    w = sh->rect.width;
    fmt = sh->texFmt;
    dstFmt = sh->copyFmt;
    alloc = sh->allocMode;
    GXSetTexCopySrc(sh->rect.left, sh->rect.top, w, h);
    GXSetTexCopyDst(w, h, dstFmt, 0);
    size = GXGetTexBufferSize(w, h, fmt, 0, 0);
    if (alloc == 1) data = smartAlloc(size, 1);
    else if (alloc == 2) data = smartAlloc(size, 0);
    else data = smartAlloc(size, 3);
    GXSetZMode(1, 3, 1);
    GXCopyTex(data->address, 0);
    GXPixModeSync();
    GXInitTexObj(&sh->texObj, data->address, w, h, fmt, 0, 0, 0);
    GXInitTexObjLOD(&sh->texObj, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    sh->data = data;
}

void imgCapture_Prim(ImageEntry* entry, s32 id) {
    ImageShadow* sh = &entry->shadows[id];
    s32 offId;
    u16 w, h;
    s32 alloc, fmt, dstFmt;
    u32 size;
    smartEntry* data;
    GXTexObj* obj;
    smartEntry* buf;
    if (!(sh->flags & 4)) return;
    offId = sh->offscreenId;
    sh->flags |= 0x10;
    if (offId != -1) {
        if (sh->allocMode) {
            if (sh->flags & 0x100) {
                offscreenGetTexObj(offId, &obj, &buf);
                if (obj) {
                    w = GXGetTexObjWidth(obj);
                    h = GXGetTexObjHeight(obj);
                    fmt = GXGetTexObjFmt(obj);
                    size = GXGetTexBufferSize(w, h, fmt, 0, 0);
                    if (sh->allocMode == 1) {
                        sh->data = smartAlloc(size, 1);
                    } else if (sh->allocMode == 2) {
                        sh->data = smartAlloc(size, 0);
                    }
                    memcpy(sh->data->address, buf->address, size);
                    DCFlushRange(sh->data->address, size);
                    GXInitTexObj(&sh->texObj, sh->data->address, w, h, fmt, 0, 0, 0);
                    GXInitTexObjLOD(&sh->texObj, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
                    offscreenGetBoundingBox(offId, &sh->rect.left, &sh->rect.top, &sh->rect.width, &sh->rect.height);
                    sh->rect.width -= sh->rect.left;
                    sh->rect.height -= sh->rect.top;
                    sh->flags &= ~0x100;
                }
            }
            if (!(sh->flags & 0x100)) sh->flags &= ~0x10;
        } else {
            GXTexObj* o2; smartEntry* b2;
            offscreenGetTexObj(offId, &o2, &b2);
            if (o2) {
                sh->texObj = *o2;
                sh->data = b2;
                offscreenGetBoundingBox(offId, &sh->rect.left, &sh->rect.top, &sh->rect.width, &sh->rect.height);
                sh->rect.width -= sh->rect.left;
                sh->rect.height -= sh->rect.top;
                sh->flags &= ~0x10;
            }
        }
    } else if (sh->allocMode) {
        if (sh->flags & 0x100) {
            imgCaptureCopy(sh, entry);
            sh->flags &= ~0x100;
        }
        if (!(sh->flags & 0x100)) sh->flags &= ~0x10;
    } else {
        imgCaptureCopy(sh, entry);
        sh->flags &= ~0x10;
    }
}

void imgCapture(CameraId camId, void* param) {
    s32 type = gp->inBattle != 0;
    int i, count;
    ImageEntry* entry;

    i = 0;
    count = wp->count;
    for (entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1 && entry->poseId != -1) {
            if (entry->type == 2 || entry->type == type) {
                if (entry->dispCamId == camId) {
                    int j;
                    for (j = 0; j < 3; j++) {
                        imgCapture_Prim(entry, j);
                    }
                }
            }
        }
    }
}

void imgInit(void) {
    wp->count = 10;
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(ImageEntry));
    memset(wp->entries, 0, wp->count * sizeof(ImageEntry));
}

void imgAutoRelease(s32 type) {
    int i, count;
    ImageEntry* entry;

    i = 0;
    count = wp->count;
    for (entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1 && entry->type == type) {
            imgRelease(entry, type);
        }
    }
}

s32 imgEntry(const char* name, s32 type) {
    int i, count;
    ImageEntry* entry;

    //check if already created
    for (i = 0, count = wp->count, entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1 && entry->type == type) {
            if (!strcmp(entry->name, name)) break;
        }
    }

    //find new slot
    for (i = 0, entry = wp->entries; i < count; i++, entry++) {
        if (!(entry->flags & 1)) break;
    }

    entry->flags = 9;
    entry->type = type;
    strcpy(entry->name, name);
    entry->poseId = -1;
    entry->animName = NULL;
    entry->width = 0;
    entry->height = 0;
    entry->offsetX = 0.0f;
    entry->offsetY = 0.0f;
    entry->order = 0.0f;
    entry->loopTimes = 0.0f;
    entry->shadowId = 0;
    entry->offsetZ = 0.0f;
    MTXIdentity(entry->mtx);
    entry->camId = CAMERA_3D_IMAGE;
    entry->dispCamId = 4;

    entry->shadows[0].flags = 0;
    entry->shadows[0].x = 0;
    entry->shadows[0].y = 0;
    entry->shadows[0].offscreenId = -1;
    entry->shadows[0].copyFmt = 6;
    entry->shadows[0].texFmt = 6;
    entry->shadows[0].allocMode = 0;

    entry->shadows[1].flags = 0;
    entry->shadows[1].x = 0;
    entry->shadows[1].y = 0;
    entry->shadows[1].offscreenId = -1;
    entry->shadows[1].copyFmt = 6;
    entry->shadows[1].texFmt = 6;
    entry->shadows[1].allocMode = 0;

    entry->shadows[2].flags = 0;
    entry->shadows[2].x = 0;
    entry->shadows[2].y = 0;
    entry->shadows[2].offscreenId = -1;
    entry->shadows[2].copyFmt = 6;
    entry->shadows[2].texFmt = 6;
    entry->shadows[2].allocMode = 0;
    entry->shadows[2].copyFmt = 39;
    entry->shadows[2].texFmt = 1;

    return i;
}

void imgMain(void) {
    s32 type = gp->inBattle != 0;
    int shadow, i, count;
    ImageEntry* entry;

    i = 0;
    count = wp->count;
    for (entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1 && entry->poseId != -1 && !(entry->flags & 2)) {
            if (entry->type == 2 || entry->type == type) {
                for (shadow = 0; shadow < 3; shadow++) {
                    if (entry->shadows[shadow].flags & 4) break;
                }
                if (shadow != 3) {
                    dispEntry(entry->camId, entry->order, 1, imgDisp, entry);
                    switch (entry->shadowId) {
                        case 0:
                            animPoseSetMaterialFlagOff(entry->poseId, 0x10000);
                            break;
                        case 1:
                            dispEntry(CAMERA_SHADOW, 0.0f, 1, imgShadowDisp, entry);
                            dispEntry(CAMERA_3D_IMAGE, 0.0f, 5, imgDisp_ProjPlane, entry);
                            break;
                        case 2:
                            animPoseSetMaterialFlagOn(entry->poseId, 0x10000);
                            dispEntry(CAMERA_SHADOW, 0.0f, 6, imgShadowDisp, entry);
                            dispEntry(CAMERA_3D_IMAGE, 0.0f, 5, imgDisp_ProjPlane, entry);
                            break;
                    }
                }
            }
        }
    }

    for (i = 0; i < 13; i++) {
        dispEntry(i, 0.0f, 7, imgCapture, NULL);
    }
}

ImageEntry* imgNameToPtr(const char* name, s32 type) {
    int i, count;
    ImageEntry* entry;

    i = 0;
    count = wp->count;
    for (entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1 && (entry->type == 2 || entry->type == type)) {
            if (!strcmp(entry->name, name)) break;
        }
    }
    return entry;
}

void imgFreeCapture(ImageEntry* entry, s32 shadowId) {
    ImageShadow* shadow = &entry->shadows[shadowId];
    if (shadow->flags & 4) {
        shadow->flags &= ~0x10;
        if (shadow->flags & 0x100) {
            shadow->flags = 0;
        }
        else if (!shadow->allocMode) {
            shadow->flags = 0;
        }
        else {
            shadow->flags = 0;
            smartFree(shadow->data);
        }
    }
}

void imgSetVirtualPoint(ImageEntry* entry, const Vec* point) {
    entry->flags &= ~8;
    entry->point = *point;
}

void imgClearVirtualPoint(ImageEntry* entry) {
    entry->flags |= 8;
}

void imgRelease(ImageEntry* entry, s32 type) {
    int i;

    for (i = 0; i < 3; i++) {
        imgFreeCapture(entry, i);
    }
    entry->flags = 0;
    memset(entry, 0, sizeof(ImageEntry));
}

void imgSetShadow(ImageEntry* entry, s32 shadowId) {
    entry->shadowId = shadowId;
}


void imgShadowDisp(CameraId camId, void* param) {
    ImageShadow* sh;
    ImageEntry* entry = param;
    GXTexObj* objs[3];
    void* datas[3];
    Mtx mtxs[3];
    Mtx* mp[3];
    int first = -1;
    int i;
    f32 y0, x0, h, w;
    f32 sx, sy, ox, oy;
    f32 s0, s1;
    Mtx t, m2, m1, m;

    for (i = 0; i < 3; i++) {
        sh = &entry->shadows[i];
        if (!(sh->flags & 4)) {
            objs[i] = NULL;
            datas[i] = NULL;
        } else if (sh->flags & 0x10) {
            objs[i] = NULL;
            datas[i] = NULL;
        } else {
            ImageRect* r = &sh->rect;
            if (entry->flags & 4) {
                x0 = r->left;
                y0 = r->top;
                w = r->width;
                h = r->height;
            } else {
                x0 = sh->x;
                y0 = sh->y;
                w = entry->width;
                h = entry->height;
            }
            r = &sh->rect;
            PSMTXTrans(m, -((f32)r->left - x0) / (f32)r->width, -((f32)r->top - y0) / (f32)r->height, 0.0f);
            PSMTXScale(m1, w / (f32)r->width, h / (f32)r->height, 1.0f);
            PSMTXConcat(m, m1, mtxs[i]);
            objs[i] = &sh->texObj;
            datas[i] = sh->data;
        }
        if (objs[i] != NULL) {
            mp[i] = &mtxs[i];
            if (first == -1) first = i;
        } else {
            mp[i] = NULL;
        }
    }
    if (first == -1) {
        entry->flags |= 0x10000000;
        return;
    }
    entry->flags &= ~0x10000000;
    animSetPaperTexMtx((void*)mp[0], (void*)mp[1], (void*)mp[2]);
    animSetPaperTexObj(objs[0], objs[1], objs[2], datas[0], datas[1], datas[2],
                       entry->shadows[0].offscreenId != -1 ? 1 : 0, entry->shadows[1].offscreenId != -1 ? 1 : 0);
    if (entry->animName != NULL) animPoseSetAnim(entry->poseId, entry->animName, 0);
    animPoseSetStartTime(entry->poseId, entry->animTime);
    animPoseMain(entry->poseId);
    animPoseSetMaterialLightFlagOn(entry->poseId, 1);
    entry->loopTimes = animPoseGetLoopTimes(entry->poseId);
    {
        f32 x0, y0, w, h; f32 sc, z;
        typedef struct { u16 p, q, w, h; } RM;
        RM* rm;
        if (entry->flags & 8) {
            sc = 1.0f; z = 0.0f;
        } else {
            sc = 1.0f; z = 0.0f;
        }
        if (entry->flags & 4) {
            x0 = entry->shadows[first].rect.left; y0 = entry->shadows[first].rect.top; w = entry->shadows[first].rect.width; h = entry->shadows[first].rect.height;
        } else {
            x0 = entry->shadows[first].x;
            y0 = entry->shadows[first].y;
            w = entry->width; h = entry->height;
        }
        PSMTXTrans(t, 64.0f * ((entry->offsetX + x0) / (f32)((RM*)DEMOGetRenderModeObj())->w), 48.0f * (-(entry->offsetY + y0) / (f32)((RM*)DEMOGetRenderModeObj())->h), z);
        PSMTXTrans(m2, -32.0f, 24.0f, 0.0f);
        PSMTXConcat(t, m2, t);
        PSMTXScale(m2, w / (f32)((RM*)DEMOGetRenderModeObj())->w, h / (f32)((RM*)DEMOGetRenderModeObj())->h, w / (f32)((RM*)DEMOGetRenderModeObj())->w);
        PSMTXConcat(t, m2, t);
        PSMTXTrans(m2, 32.0f, -24.0f, 0.0f);
        PSMTXConcat(t, m2, t);
        animPoseDrawMtx(entry->poseId, t, 1, 0.0f, sc);
        animPoseDrawMtx(entry->poseId, t, 2, 0.0f, sc);
    }
    switch (entry->shadowId) {
    case 1: shadowProjShadowDrawCountUp(); break;
    case 2: shadowDepthShadowDrawCountUp(); break;
    }
}
