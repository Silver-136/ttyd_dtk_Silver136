#include "driver/extdrv.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "string.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include "gxsub.h"
#include "driver/animdrv.h"
#include "eff/eff_n64.h"
#include "libc_decls.h"

void animPoseDisp_MakeExtTexture(const char* group, const char* anim, f32 frame, f32 scale, GXTexObj* obj, void*** image, s32* x0, s32* y0, s32* x1, s32* y1);
void qqsort(void* array, u32 num, u32 size, s32 (*compare)(const void*, const void*));


static ExtdrvWork work[2];
static u8 shadow_dl[32];
static f32 shadow_vtx[16];
static f32 shadow_coord[8];

#define extGetWork() (gp->inBattle ? &work[1] : &work[0])

void extMakeTexture(CameraId camId, void* param);
static s32 compare(const void* a, const void* b);

static f32 sintbl[450] = {
    0.0f, 0.0174519997f, 0.0348990001f, 0.0523359999f, 0.0697560012f, 0.0871559978f, 0.104528002f, 0.121868998f,
    0.139173001f, 0.156434f, 0.173648f, 0.190808997f, 0.207911998f, 0.224950999f, 0.241922006f, 0.258819014f,
    0.275637001f, 0.292371988f, 0.309017003f, 0.325567991f, 0.342020005f, 0.358368009f, 0.374606997f, 0.390731007f,
    0.406737f, 0.422618002f, 0.438371003f, 0.453990012f, 0.469471008f, 0.484809995f, 0.5f, 0.515038013f,
    0.529919028f, 0.544638991f, 0.559193015f, 0.573575974f, 0.587785006f, 0.601814985f, 0.615661025f, 0.629320025f,
    0.642787993f, 0.656059027f, 0.669130981f, 0.681998014f, 0.694657981f, 0.707107008f, 0.719340026f, 0.731353998f,
    0.743144989f, 0.754709005f, 0.766044021f, 0.777145982f, 0.788011014f, 0.798635006f, 0.809017003f, 0.819151998f,
    0.829037011f, 0.838670015f, 0.848047972f, 0.857167006f, 0.866024971f, 0.87462002f, 0.882947028f, 0.891005993f,
    0.898793995f, 0.906307995f, 0.913545012f, 0.920504987f, 0.927183986f, 0.933579981f, 0.939692974f, 0.945518017f,
    0.951056004f, 0.956305027f, 0.961261988f, 0.965925992f, 0.970296025f, 0.974370003f, 0.978147984f, 0.981626987f,
    0.984808028f, 0.987688005f, 0.990267992f, 0.992546022f, 0.994521976f, 0.996195018f, 0.997564018f, 0.998629987f,
    0.999391019f, 0.999848008f, 1.0f, 0.999848008f, 0.999391019f, 0.998629987f, 0.997564018f, 0.996195018f,
    0.994521976f, 0.992546022f, 0.990267992f, 0.987688005f, 0.984808028f, 0.981626987f, 0.978147984f, 0.974370003f,
    0.970296025f, 0.965925992f, 0.961261988f, 0.956305027f, 0.951057017f, 0.94551897f, 0.939692974f, 0.933580995f,
    0.927183986f, 0.920504987f, 0.913546026f, 0.906307995f, 0.898793995f, 0.891007006f, 0.882947981f, 0.87462002f,
    0.866025984f, 0.857168019f, 0.848047972f, 0.838671029f, 0.829038024f, 0.819151998f, 0.809017003f, 0.798636019f,
    0.788011014f, 0.777145982f, 0.766044974f, 0.754710019f, 0.743144989f, 0.731353998f, 0.719340026f, 0.707107008f,
    0.694658995f, 0.681999028f, 0.669130981f, 0.656059027f, 0.642787993f, 0.629320979f, 0.615661979f, 0.601814985f,
    0.587786019f, 0.573576987f, 0.559193015f, 0.544638991f, 0.529919982f, 0.515039027f, 0.5f, 0.484809995f,
    0.469471991f, 0.453990996f, 0.438371986f, 0.422618985f, 0.406737f, 0.39073199f, 0.374606997f, 0.358368993f,
    0.342020988f, 0.325569004f, 0.309017003f, 0.292371988f, 0.275638014f, 0.258819997f, 0.241923004f, 0.224951997f,
    0.207911998f, 0.190808997f, 0.173648998f, 0.156434998f, 0.139173999f, 0.121870004f, 0.104529001f, 0.0871559978f,
    0.0697569996f, 0.0523369983f, 0.0348999985f, 0.0174529999f, 9.99999997e-07f, -0.0174519997f, -0.0348990001f, -0.0523350015f,
    -0.0697560012f, -0.0871549994f, -0.104528002f, -0.121868998f, -0.139173001f, -0.156434f, -0.173648f, -0.190807998f,
    -0.207911f, -0.224950001f, -0.241920993f, -0.258819014f, -0.275637001f, -0.292371005f, -0.309015989f, -0.325567991f,
    -0.342018992f, -0.358366996f, -0.374606013f, -0.390729994f, -0.406735986f, -0.422618002f, -0.438371003f, -0.453990012f,
    -0.469471008f, -0.484809011f, -0.499998987f, -0.515037f, -0.529919028f, -0.544638991f, -0.559192002f, -0.573575974f,
    -0.587785006f, -0.601813972f, -0.615661025f, -0.629320025f, -0.64278698f, -0.656058013f, -0.669130027f, -0.681998014f,
    -0.694657981f, -0.707105994f, -0.719339013f, -0.731352985f, -0.743143976f, -0.754709005f, -0.766044021f, -0.777145028f,
    -0.788010001f, -0.798635006f, -0.809015989f, -0.819151998f, -0.829037011f, -0.838670015f, -0.848047972f, -0.857167006f,
    -0.866024971f, -0.874619007f, -0.882947028f, -0.891005993f, -0.898793995f, -0.906306982f, -0.913545012f, -0.920503974f,
    -0.927182972f, -0.933579981f, -0.93969202f, -0.945518017f, -0.951056004f, -0.956304014f, -0.961260974f, -0.965925992f,
    -0.970295012f, -0.974370003f, -0.97814697f, -0.981626987f, -0.984808028f, -0.987688005f, -0.990267992f, -0.992546022f,
    -0.994521976f, -0.996195018f, -0.997564018f, -0.998628974f, -0.999391019f, -0.999848008f, -1.0f, -0.999848008f,
    -0.999391019f, -0.998629987f, -0.997564018f, -0.996195018f, -0.994521976f, -0.992546022f, -0.990267992f, -0.987688005f,
    -0.984808028f, -0.981626987f, -0.978147984f, -0.974370003f, -0.970296025f, -0.965925992f, -0.961261988f, -0.956305027f,
    -0.951057017f, -0.94551897f, -0.939692974f, -0.933580995f, -0.927183986f, -0.920504987f, -0.913546026f, -0.906307995f,
    -0.898795009f, -0.891007006f, -0.882947981f, -0.87462002f, -0.866025984f, -0.857168019f, -0.848048985f, -0.838671029f,
    -0.829038024f, -0.819153011f, -0.809018016f, -0.798636019f, -0.788011014f, -0.777146995f, -0.766044974f, -0.754710019f,
    -0.743146002f, -0.731353998f, -0.71934098f, -0.707108021f, -0.694658995f, -0.681999028f, -0.669131994f, -0.65605998f,
    -0.642787993f, -0.629320979f, -0.615661979f, -0.601815999f, -0.587786019f, -0.573576987f, -0.559194028f, -0.544640005f,
    -0.529919982f, -0.515039027f, -0.500001013f, -0.484811008f, -0.469473004f, -0.453990996f, -0.438371986f, -0.422618985f,
    -0.406737f, -0.39073199f, -0.37460801f, -0.358368993f, -0.342022002f, -0.325569004f, -0.309017986f, -0.292373002f,
    -0.275638014f, -0.258819997f, -0.241923004f, -0.224951997f, -0.207912996f, -0.190809995f, -0.173648998f, -0.156434998f,
    -0.139174998f, -0.121871002f, -0.104529999f, -0.0871570036f, -0.069757998f, -0.0523369983f, -0.0349010006f, -0.0174540002f,
    -9.99999997e-07f, 0.0174509995f, 0.0348990001f, 0.0523350015f, 0.0697550029f, 0.087154001f, 0.104526997f, 0.121867999f,
    0.139172003f, 0.156433001f, 0.173647001f, 0.190807998f, 0.207910001f, 0.224950001f, 0.241920993f, 0.258818001f,
    0.275635988f, 0.292371005f, 0.309015006f, 0.325567007f, 0.342018992f, 0.358366996f, 0.374605f, 0.390729994f,
    0.406735986f, 0.422616988f, 0.438369989f, 0.453990012f, 0.469471008f, 0.484809011f, 0.499998987f, 0.515037f,
    0.529918015f, 0.544637978f, 0.559190989f, 0.57357502f, 0.587783992f, 0.601813972f, 0.615661025f, 0.629319012f,
    0.64278698f, 0.656058013f, 0.669130027f, 0.681997001f, 0.694657028f, 0.707104981f, 0.719339013f, 0.731352985f,
    0.743143976f, 0.754709005f, 0.766044021f, 0.777145028f, 0.788010001f, 0.798635006f, 0.809015989f, 0.819150984f,
    0.829037011f, 0.838670015f, 0.848047018f, 0.857165992f, 0.866024971f, 0.874619007f, 0.882947028f, 0.891005993f,
    0.898792982f, 0.906306982f, 0.913545012f, 0.920503974f, 0.927182972f, 0.933579981f, 0.93969202f, 0.945518017f,
    0.951056004f, 0.956304014f, 0.961260974f, 0.965924978f, 0.970295012f, 0.974370003f, 0.97814697f, 0.981626987f,
    0.984808028f, 0.987688005f, 0.990267992f, 0.992546022f, 0.994521976f, 0.996195018f, 0.997564018f, 0.998628974f,
    0.999391019f, 0.999848008f,
};

void extInit(void) {
    ExtdrvWork* wp = extGetWork();
    wp->poseNum = 0;
    wp->poses = NULL;
}

void extEntry(s32 poseNum, ExtdrvTexInfo* texInfo, void (*initFunc)(void), void (*mainFunc)(void), void (*mainFunc2)(void)) {
    static f32 Normals[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f};
    static GXColor Colors[] = {{0xFF, 0xFF, 0xFF, 0xFF}};
    static f32 UVs[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
    ExtdrvWork* wp = extGetWork();
    ExtdrvTexInfo* info;
    s32 texNum;
    s32 i;

    texNum = 0;
    for (info = texInfo; info->group != NULL; info++, texNum++) {
    }
    wp->poses = __memAlloc(HEAP_EXT, poseNum * sizeof(ExtdrvPose));
    wp->poseNum = poseNum;
    wp->mainFunc = mainFunc;
    wp->mainFunc2 = mainFunc2;
    wp->texs = __memAlloc(HEAP_EXT, texNum * sizeof(ExtdrvTex));
    wp->texNum = texNum;
    wp->mtxs = __memAlloc(HEAP_EXT, poseNum * sizeof(Mtx));
    wp->vtx = __memAlloc(HEAP_EXT, texNum * 12 * sizeof(f32));
    wp->normals = Normals;
    wp->colors = Colors;
    wp->uvs = UVs;
    wp->sort = __memAlloc(HEAP_EXT, poseNum * sizeof(ExtdrvSort));
    wp->dl = __memAlloc(HEAP_EXT, texNum * sizeof(*wp->dl));
    DCInvalidateRange(wp->dl, texNum * sizeof(*wp->dl));
    wp->dlSize = __memAlloc(HEAP_EXT, texNum * sizeof(u32));
    memset(wp->poses, 0, wp->poseNum * sizeof(ExtdrvPose));
    for (i = 0; i < poseNum; i++) {
        wp->sort[i].index = i;
        wp->poses[i].mtx = &wp->mtxs[i];
    }
    PSMTXIdentity(wp->shadowMtx);
    dispEntry(CAMERA_OFFSCREEN, 0.0f, 1, extMakeTexture, texInfo);
    if (initFunc != NULL) {
        initFunc();
    }
}

void extMakeTexture(CameraId camId, void* param) {
    ExtdrvWork* wp = extGetWork();
    ExtdrvTexInfo* info;
    ExtdrvTex* tex;
    s32 i;
    s32 n;
    s32 v;
    s32 x0, y0, x1, y1;
    f32 save[4];

    anim_ext_tex_size = 0;
    tex = wp->texs;
    info = param;
    memcpy(save, queen_uv, sizeof(save));
    queen_uv[0] = 0.0f;
    queen_uv[1] = -2.0f / wp->texNum;
    queen_uv[2] = 0.0f;
    queen_uv[3] = -1.0f / wp->texNum;
    for (i = 0; i < wp->texNum; i++, info++, tex++) {
        n = i * 12;
        animPoseDisp_MakeExtTexture(info->group, info->anim, info->frame, 4.0f, &tex->obj, &tex->image, &x0, &y0, &x1, &y1);
        wp->vtx[n++] = x0 / 2.0f;
        wp->vtx[n++] = -y0 / 2.0f;
        wp->vtx[n++] = 0.0f;
        wp->vtx[n++] = x1 / 2.0f;
        wp->vtx[n++] = -y0 / 2.0f;
        wp->vtx[n++] = 0.0f;
        wp->vtx[n++] = x1 / 2.0f;
        wp->vtx[n++] = -y1 / 2.0f;
        wp->vtx[n++] = 0.0f;
        wp->vtx[n++] = x0 / 2.0f;
        wp->vtx[n++] = -y1 / 2.0f;
        wp->vtx[n++] = 0.0f;
        GXBeginDisplayList(wp->dl[i], sizeof(wp->dl[i]));
        v = i * 4;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition1x16(v++);
        GXTexCoord1x16(0);
        GXPosition1x16(v++);
        GXTexCoord1x16(1);
        GXPosition1x16(v++);
        GXTexCoord1x16(2);
        GXPosition1x16(v++);
        GXTexCoord1x16(3);
        GXEnd();
        wp->dlSize[i] = GXEndDisplayList();
    }
    memcpy(queen_uv, save, sizeof(save));
}

void extReset(void) {
    ExtdrvWork* wp = extGetWork();

    if (wp->poses != NULL) {
        __memFree(HEAP_EXT, wp->poses);
    }
    if (wp->texs != NULL) {
        __memFree(HEAP_EXT, wp->texs);
    }
    if (wp->mtxs != NULL) {
        __memFree(HEAP_EXT, wp->mtxs);
    }
    if (wp->vtx != NULL) {
        __memFree(HEAP_EXT, wp->vtx);
    }
    if (wp->sort != NULL) {
        __memFree(HEAP_EXT, wp->sort);
    }
    if (wp->dl != NULL) {
        __memFree(HEAP_EXT, wp->dl);
    }
    if (wp->dlSize != NULL) {
        __memFree(HEAP_EXT, wp->dlSize);
    }
    memset(wp, 0, sizeof(ExtdrvWork));
}

static inline f32 camGetRy(void) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    f32 ry = 57.29579f * (f32)atan2(-(cam->cameraAt.x - cam->cameraPos.x), -(cam->cameraAt.z - cam->cameraPos.z));

    while (ry >= 360.0f) {
        ry -= 360.0f;
    }
    if (ry < 0.0f) {
        ry += 360.0f;
    }
    return ry;
}

void extMain(void) {
    ExtdrvWork* wp = extGetWork();
    s32 i;
    ExtdrvPose* pose;
    ExtdrvSort* sort;
    CameraEntry* cam;
    f32 ry, angle, diff, c, s;
    u32 flags;

    if (marioStGetSystemLevel() == 0 && wp->mainFunc != NULL) {
        wp->mainFunc();
    }
    if (wp->mainFunc2 != NULL) {
        wp->mainFunc2();
    }
    pose = wp->poses;
    sort = wp->sort;
    ry = camGetRy();
    cam = camGetPtr(CAMERA_3D);
    cam = camGetPtr(CAMERA_3D);
    for (i = wp->poseNum - 1; i >= 0; i--, pose++, sort++) {
        flags = pose->flags;
        sort->index = i;
        if (flags & 1) {
            sort->z = pose->texId;
        }
        if (flags & 2) {
            angle = 90.0f + (ry - pose->dir);
            while (angle < -360.0f) {
                angle += 360.0f;
            }
            while (angle >= 360.0f) {
                angle -= 360.0f;
            }
            if ((angle > 90.0f && angle <= 270.0f) || (angle < -90.0f && angle >= -270.0f)) {
                while (angle < -360.0f) {
                    angle += 360.0f;
                }
                while (angle >= 360.0f) {
                    angle -= 360.0f;
                }
                pose->targetRy = 180.0f + ry;
            } else {
                pose->targetRy = camGetRy();
            }
            while (pose->targetRy - pose->ry > 180.0f) {
                pose->ry += 360.0f;
            }
            while ((diff = pose->targetRy - pose->ry) < -180.0f) {
                pose->ry -= 360.0f;
            }
            pose->ry += diff * 0.1f;
            angle = pose->ry;
            while (angle < 0.0f) {
                angle += 360.0f;
            }
            while (angle >= 360.0f) {
                angle -= 360.0f;
            }
            c = sintbl[(s32)angle + 90];
            s = sintbl[(s32)angle];
            (*pose->mtx)[0][0] = c;
            (*pose->mtx)[2][2] = c;
            (*pose->mtx)[0][2] = s;
            (*pose->mtx)[2][0] = -s;
        }
    }
    if (wp->poseNum != 0) {
        qqsort(wp->sort, wp->poseNum, sizeof(ExtdrvSort), compare);
    }
    wp->curTex = -1;
    wp->usedTmem = 0;
}

static s32 compare(const void* a, const void* b) {
    if (((ExtdrvSort*)a)->z > ((ExtdrvSort*)b)->z) {
        return 1;
    }
    if (((ExtdrvSort*)a)->z < ((ExtdrvSort*)b)->z) {
        return -1;
    }
    return 0;
}

ExtdrvPose* extGetPosePtr(void) {
    return extGetWork()->poses;
}

s32 extGetPoseNum(void) {
    return extGetWork()->poseNum;
}

void extPoseDraw(s32 index) {
    ExtdrvWork* wp = extGetWork();
    ExtdrvPose* pose = &wp->poses[index];
    s32 texId;
    ExtdrvTex* tex;
    Mtx mtx;
    Mtx nrm;

    if (pose->flags & 1) {
        texId = pose->texId;
        if (wp->curTex != texId) {
            tex = &wp->texs[texId];
            GXLoadTexObjPreLoaded(smartTexObj(&tex->obj, tex->image), &tex->region, GX_TEXMAP0);
            wp->curTex = texId;
        }
        PSMTXConcat(camGetCurPtr()->view, *pose->mtx, mtx);
        GXLoadPosMtxImm(mtx, 0);
        PSMTXInvXpose(mtx, nrm);
        GXLoadNrmMtxImm(nrm, 0);
        GXSetCurrentMtx(0);
        GXCallDisplayList(wp->dl[pose->texId], wp->dlSize[pose->texId]);
    }
}

void extDraw(void) {
    ExtdrvWork* wp = extGetWork();
    s32 i;
    ExtdrvSort* sort;
    ExtdrvPose* pose;
    s32 texId;
    ExtdrvTex* tex;
    Mtx mtx;

    for (i = wp->poseNum - 1, sort = wp->sort; i >= 0; i--, sort++) {
        pose = &wp->poses[sort->index];
        if (pose->flags & 1) {
            texId = pose->texId;
            if (texId != wp->curTex) {
                tex = &wp->texs[texId];
                GXLoadTexObjPreLoaded(smartTexObj(&tex->obj, tex->image), &tex->region, GX_TEXMAP0);
                wp->curTex = texId;
            }
            PSMTXConcat(camGetCurPtr()->view, *pose->mtx, mtx);
            GXLoadPosMtxImm(mtx, 0);
            GXSetCurrentMtx(0);
            GXCallDisplayList(wp->dl[texId], wp->dlSize[texId]);
        }
    }
}

void extLoadRenderMode(void) {
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
}

void extLoadVertex(void) {
    ExtdrvWork* wp = extGetWork();

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetArray(GX_VA_POS, wp->vtx, 12);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_TEX0, wp->uvs, 8);
    GXSetCullMode(GX_CULL_NONE);
}

void extLoadTexture(void) {
    ExtdrvWork* wp = extGetWork();
    ExtdrvTex* tex = wp->texs;
    s32 i;
    u32 offset = 0;
    u32 size;

    GXInvalidateTexAll();
    for (i = 0; i < wp->texNum; i++, tex++) {
        size = GXGetTexBufferSize(GXGetTexObjWidth(&tex->obj), GXGetTexObjHeight(&tex->obj),
                                  GXGetTexObjFmt(&tex->obj), GX_FALSE, 0);
        if (offset < 0x80000 && offset + size >= 0x80000) {
            offset = 0x80000;
        }
        GXInitTexPreLoadRegion(&tex->region, offset, size, 0, 0);
        GXPreLoadEntireTexture(smartTexObj(&tex->obj, tex->image), &tex->region);
        offset += size;
    }
    wp->usedTmem = offset;
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
}

void extLoadTextureExit(void) {
    GXTexModeSync();
}

void extLoadTev(void) {
    GXColor fogColor = {0xFF, 0xFF, 0xFF, 0xFF};

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, fogColor);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
}

static u8 shadow_dl[32] ATTRIBUTE_ALIGN(32) = {
    0x80, 0x00, 0x04, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03,
};

__declspec(section ".sdata") static f32 shadow_vtx[16] = {
    -1.2f, 0.0f, -1.2f,
    -1.2f, 0.0f, 1.2f,
    1.2f, 0.0f, 1.2f,
    1.2f, 0.0f, -1.2f,
};

static f32 shadow_coord[8] = {
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
};

void extLoadShadowVertex(void) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_POS, shadow_vtx, 12);
    GXSetArray(GX_VA_TEX0, shadow_coord, 8);
    GXSetCullMode(GX_CULL_NONE);
}

void extLoadShadowTev(void) {
    GXTexObj obj;

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    {
        GXColor color = {0x00, 0x00, 0x00, 0xA0};
        GXSetChanMatColor(GX_COLOR0A0, color);
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    effGetTexObjN64(0x2C, &obj);
    GXLoadTexObj(&obj, GX_TEXMAP0);
}

void extLoadShadowRenderMode(void) {
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
}

void extLoadShadowMtx(Mtx mtx) {
    PSMTXCopy(mtx, extGetWork()->shadowMtx);
}

void extDrawShadow(void) {
    ExtdrvWork* wp = extGetWork();
    s32 i;
    ExtdrvSort* sort;
    ExtdrvPose* pose;
    Mtx mtx;

    sort = &wp->sort[wp->poseNum - 1];
    for (i = wp->poseNum - 1; i >= 0; i--, sort--) {
        pose = &wp->poses[sort->index];
        if (!(pose->flags & 4)) {
            PSMTXConcat(camGetCurPtr()->view, *pose->mtx, mtx);
            PSMTXConcat(mtx, wp->shadowMtx, mtx);
            if (pose->flags & 0x10) {
                PSMTXTransApply(mtx, mtx, 0.0f, -(*pose->mtx)[1][3] + pose->shadowY, 0.0f);
            }
            GXLoadPosMtxImm(mtx, 0);
            GXSetCurrentMtx(0);
            GXCallDisplayList(shadow_dl, 0x20);
        }
    }
}
