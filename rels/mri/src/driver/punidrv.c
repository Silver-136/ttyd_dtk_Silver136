#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "driver/mapdrv.h"
#include "libc_decls.h"

#include "driver/punidrv.h"
#include "memory.h"
#include "driver/dispdrv.h"
#include "string.h"
#include "win/win_main.h"
#include "system.h"
#include "driver/animdrv.h"

void animPoseDisp_MakeExtTexture(const char* group, const char* anim, f32 frame, f32 scale, GXTexObj* obj, void*** image, s32* x0, s32* y0, s32* x1, s32* y1);
static f32 camGetRy(void);
static s32 compare(const void* a, const void* b);

void puniEntry(s32 poseNum, PunidrvTexInfo* texInfo, void (*initFunc)(void), void (*mainFunc)(void), void (*mainFunc2)(void));

static f32 sintbl[450];
static u8 shadow_dl[32];
static f32 shadow_vtx[56];
static PunidrvWork* work;

void puniSinCosRet(u32 deg, f32* x, f32* z, f32 rx, f32 rz) {
    deg %= 360;
    *x += rx * sintbl[deg];
    *z += rz * sintbl[deg + 90];
}

void puniInit(void) {
    PunidrvWork* wp;

    work = wp = _mapAlloc(sizeof(PunidrvWork));
    wp->poseNum = 0;
    wp->poses = NULL;
    wp->useFade = TRUE;
}

static f32 sintbl[450] = {
    0.0f, 0.017452f, 0.034899f, 0.052336f, 0.069756f, 0.087156f, 0.104528f, 0.121869f,
    0.139173f, 0.156434f, 0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 0.258819f,
    0.275637f, 0.292372f, 0.309017f, 0.325568f, 0.34202f, 0.358368f, 0.374607f, 0.390731f,
    0.406737f, 0.422618f, 0.438371f, 0.45399f, 0.469471f, 0.48481f, 0.5f, 0.515038f,
    0.529919f, 0.544639f, 0.559193f, 0.573576f, 0.587785f, 0.601815f, 0.615661f, 0.62932f,
    0.642788f, 0.656059f, 0.669131f, 0.681998f, 0.694658f, 0.707107f, 0.71934f, 0.731354f,
    0.743145f, 0.754709f, 0.766044f, 0.777146f, 0.788011f, 0.798635f, 0.809017f, 0.819152f,
    0.829037f, 0.83867f, 0.848048f, 0.857167f, 0.866025f, 0.87462f, 0.882947f, 0.891006f,
    0.898794f, 0.906308f, 0.913545f, 0.920505f, 0.927184f, 0.93358f, 0.939693f, 0.945518f,
    0.951056f, 0.956305f, 0.961262f, 0.965926f, 0.970296f, 0.97437f, 0.978148f, 0.981627f,
    0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 0.996195f, 0.997564f, 0.99863f,
    0.999391f, 0.999848f, 1.0f, 0.999848f, 0.999391f, 0.99863f, 0.997564f, 0.996195f,
    0.994522f, 0.992546f, 0.990268f, 0.987688f, 0.984808f, 0.981627f, 0.978148f, 0.97437f,
    0.970296f, 0.965926f, 0.961262f, 0.956305f, 0.951057f, 0.945519f, 0.939693f, 0.933581f,
    0.927184f, 0.920505f, 0.913546f, 0.906308f, 0.898794f, 0.891007f, 0.882948f, 0.87462f,
    0.866026f, 0.857168f, 0.848048f, 0.838671f, 0.829038f, 0.819152f, 0.809017f, 0.798636f,
    0.788011f, 0.777146f, 0.766045f, 0.75471f, 0.743145f, 0.731354f, 0.71934f, 0.707107f,
    0.694659f, 0.681999f, 0.669131f, 0.656059f, 0.642788f, 0.629321f, 0.615662f, 0.601815f,
    0.587786f, 0.573577f, 0.559193f, 0.544639f, 0.52992f, 0.515039f, 0.5f, 0.48481f,
    0.469472f, 0.453991f, 0.438372f, 0.422619f, 0.406737f, 0.390732f, 0.374607f, 0.358369f,
    0.342021f, 0.325569f, 0.309017f, 0.292372f, 0.275638f, 0.25882f, 0.241923f, 0.224952f,
    0.207912f, 0.190809f, 0.173649f, 0.156435f, 0.139174f, 0.12187f, 0.104529f, 0.087156f,
    0.069757f, 0.052337f, 0.0349f, 0.017453f, 1e-06f, -0.017452f, -0.034899f, -0.052335f,
    -0.069756f, -0.087155f, -0.104528f, -0.121869f, -0.139173f, -0.156434f, -0.173648f, -0.190808f,
    -0.207911f, -0.22495f, -0.241921f, -0.258819f, -0.275637f, -0.292371f, -0.309016f, -0.325568f,
    -0.342019f, -0.358367f, -0.374606f, -0.39073f, -0.406736f, -0.422618f, -0.438371f, -0.45399f,
    -0.469471f, -0.484809f, -0.499999f, -0.515037f, -0.529919f, -0.544639f, -0.559192f, -0.573576f,
    -0.587785f, -0.601814f, -0.615661f, -0.62932f, -0.642787f, -0.656058f, -0.66913f, -0.681998f,
    -0.694658f, -0.707106f, -0.719339f, -0.731353f, -0.743144f, -0.754709f, -0.766044f, -0.777145f,
    -0.78801f, -0.798635f, -0.809016f, -0.819152f, -0.829037f, -0.83867f, -0.848048f, -0.857167f,
    -0.866025f, -0.874619f, -0.882947f, -0.891006f, -0.898794f, -0.906307f, -0.913545f, -0.920504f,
    -0.927183f, -0.93358f, -0.939692f, -0.945518f, -0.951056f, -0.956304f, -0.961261f, -0.965926f,
    -0.970295f, -0.97437f, -0.978147f, -0.981627f, -0.984808f, -0.987688f, -0.990268f, -0.992546f,
    -0.994522f, -0.996195f, -0.997564f, -0.998629f, -0.999391f, -0.999848f, -1.0f, -0.999848f,
    -0.999391f, -0.99863f, -0.997564f, -0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f,
    -0.984808f, -0.981627f, -0.978148f, -0.97437f, -0.970296f, -0.965926f, -0.961262f, -0.956305f,
    -0.951057f, -0.945519f, -0.939693f, -0.933581f, -0.927184f, -0.920505f, -0.913546f, -0.906308f,
    -0.898795f, -0.891007f, -0.882948f, -0.87462f, -0.866026f, -0.857168f, -0.848049f, -0.838671f,
    -0.829038f, -0.819153f, -0.809018f, -0.798636f, -0.788011f, -0.777147f, -0.766045f, -0.75471f,
    -0.743146f, -0.731354f, -0.719341f, -0.707108f, -0.694659f, -0.681999f, -0.669132f, -0.65606f,
    -0.642788f, -0.629321f, -0.615662f, -0.601816f, -0.587786f, -0.573577f, -0.559194f, -0.54464f,
    -0.52992f, -0.515039f, -0.500001f, -0.484811f, -0.469473f, -0.453991f, -0.438372f, -0.422619f,
    -0.406737f, -0.390732f, -0.374608f, -0.358369f, -0.342022f, -0.325569f, -0.309018f, -0.292373f,
    -0.275638f, -0.25882f, -0.241923f, -0.224952f, -0.207913f, -0.19081f, -0.173649f, -0.156435f,
    -0.139175f, -0.121871f, -0.10453f, -0.087157f, -0.069758f, -0.052337f, -0.034901f, -0.017454f,
    -1e-06f, 0.017451f, 0.034899f, 0.052335f, 0.069755f, 0.087154f, 0.104527f, 0.121868f,
    0.139172f, 0.156433f, 0.173647f, 0.190808f, 0.20791f, 0.22495f, 0.241921f, 0.258818f,
    0.275636f, 0.292371f, 0.309015f, 0.325567f, 0.342019f, 0.358367f, 0.374605f, 0.39073f,
    0.406736f, 0.422617f, 0.43837f, 0.45399f, 0.469471f, 0.484809f, 0.499999f, 0.515037f,
    0.529918f, 0.544638f, 0.559191f, 0.573575f, 0.587784f, 0.601814f, 0.615661f, 0.629319f,
    0.642787f, 0.656058f, 0.66913f, 0.681997f, 0.694657f, 0.707105f, 0.719339f, 0.731353f,
    0.743144f, 0.754709f, 0.766044f, 0.777145f, 0.78801f, 0.798635f, 0.809016f, 0.819151f,
    0.829037f, 0.83867f, 0.848047f, 0.857166f, 0.866025f, 0.874619f, 0.882947f, 0.891006f,
    0.898793f, 0.906307f, 0.913545f, 0.920504f, 0.927183f, 0.93358f, 0.939692f, 0.945518f,
    0.951056f, 0.956304f, 0.961261f, 0.965925f, 0.970295f, 0.97437f, 0.978147f, 0.981627f,
    0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 0.996195f, 0.997564f, 0.998629f,
    0.999391f, 0.999848f,
};

void puniEntry(s32 poseNum, PunidrvTexInfo* texInfo, void (*initFunc)(void), void (*mainFunc)(void),
               void (*mainFunc2)(void)) {
    static f32 Normals[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f};
    static GXColor Colors[] = {{0xFF, 0xFF, 0xFF, 0xFF}};
    static f32 UVs[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};
    PunidrvWork* wp = work;
    PunidrvTexInfo* info;
    s32 texNum;
    s32 i;

    texNum = 0;
    for (info = texInfo; info->group != NULL; info++) {
        texNum++;
    }
    wp->poses = __memAlloc(HEAP_EXT, poseNum * sizeof(PunidrvPose));
    wp->poseNum = poseNum;
    wp->mainFunc = mainFunc;
    wp->mainFunc2 = mainFunc2;
    wp->texs = __memAlloc(HEAP_EXT, texNum * sizeof(PunidrvTex));
    wp->texNum = texNum;
    wp->mtxs = __memAlloc(HEAP_EXT, poseNum * sizeof(Mtx));
    wp->vtx = __memAlloc(HEAP_EXT, texNum * 12 * sizeof(f32));
    wp->normals = Normals;
    wp->colors = Colors;
    wp->uvs = UVs;
    wp->sort = __memAlloc(HEAP_EXT, poseNum * sizeof(PunidrvSort));
    wp->dl = __memAlloc(HEAP_EXT, texNum * 0x80);
    DCInvalidateRange(wp->dl, texNum * 0x80);
    wp->dlSize = __memAlloc(HEAP_EXT, texNum * sizeof(u32));
    memset(wp->poses, 0, wp->poseNum * sizeof(PunidrvPose));
    for (i = 0; i < poseNum; i++) {
        wp->sort[i].index = i;
        wp->poses[i].mtx = &wp->mtxs[i];
        wp->poses[i].scale = 1.0f;
    }
    PSMTXIdentity(wp->shadowMtx);
    dispEntry(CAMERA_OFFSCREEN, 0.0f, 1, puniMakeTexture, texInfo);
    if (initFunc != NULL) {
        initFunc();
    }
}

void puniMakeTexture(CameraId camId, void* param) {
    PunidrvWork* wp = work;
    PunidrvTexInfo* info = param;
    PunidrvTex* tex = wp->texs;
    s32 i;
    s32 n;
    s32 v;
    f32 scale = 0.34246576f;
    s32 x0, y0, x1, y1;

    anim_ext_tex_size = 0;
    for (i = 0; i < wp->texNum; i++, info++, tex++) {
        n = i * 12;
        animPoseDisp_MakeExtTexture(info->group, info->anim, info->frame, 5.84f, &tex->obj, &tex->image, &x0, &y0, &x1, &y1);
        wp->vtx[n++] = x0 * scale;
        wp->vtx[n++] = -y0 * scale;
        wp->vtx[n++] = 0.0f;
        wp->vtx[n++] = x1 * scale;
        wp->vtx[n++] = -y0 * scale;
        wp->vtx[n++] = 0.0f;
        wp->vtx[n++] = x1 * scale;
        wp->vtx[n++] = -y1 * scale;
        wp->vtx[n++] = 0.0f;
        wp->vtx[n++] = x0 * scale;
        wp->vtx[n++] = -y1 * scale;
        wp->vtx[n++] = 0.0f;
        GXBeginDisplayList(wp->dl[i], 0x80);
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
}

void puniReset(void) {
    PunidrvWork* wp = work;

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
    memset(wp, 0, sizeof(PunidrvWork));
}

void puniFadeOff(BOOL useFade) {
    work->useFade = useFade;
}

void puniMain(void) {
    PunidrvWork* wp = work;
    s32 i;
    PunidrvPose* pose;
    PunidrvSort* sort;
    CameraEntry* cam;
    f32 ry, angle, diff, c, s;
    u32 flags;

    if (!winCheck() && wp->mainFunc != NULL) {
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
        angle = 90.0f + (ry - pose->dir);
        while (angle < -360.0f) {
            angle += 360.0f;
        }
        while (angle >= 360.0f) {
            angle -= 360.0f;
        }
        if ((angle > 90.0f && angle <= 270.0f) || (angle < -90.0f && angle >= -270.0f)) {
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
        if (flags & 2) {
            pose->ry = pose->ry + diff * 0.1f;
        } else {
            pose->ry = pose->targetRy;
        }
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
    if (wp->poseNum != 0) {
        qqsort(wp->sort, wp->poseNum, sizeof(PunidrvSort), (void*)compare);
    }
    wp->curTex = -1;
    wp->usedTmem = 0;
}

static s32 compare(const void* a, const void* b) {
    if (((PunidrvSort*)a)->z > ((PunidrvSort*)b)->z) {
        return 1;
    }
    if (((PunidrvSort*)a)->z < ((PunidrvSort*)b)->z) {
        return -1;
    }
    return 0;
}

PunidrvPose* puniGetPosePtr(void) {
    return work->poses;
}

s32 puniGetPoseNum(void) {
    return work->poseNum;
}

void puniPoseDraw(s32 index) {
    PunidrvWork* wp = work;
    PunidrvPose* pose = &wp->poses[index];
    s32 texId;
    PunidrvTex* tex;
    Mtx mtx;

    if (pose->flags & 1) {
        texId = pose->texId;
        if (wp->curTex != texId) {
            tex = &wp->texs[texId];
            GXLoadTexObjPreLoaded(smartTexObj(&tex->obj, tex->image), &tex->region, GX_TEXMAP0);
            wp->curTex = texId;
        }
        PSMTXConcat(camGetCurPtr()->view, *pose->mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXCallDisplayList(wp->dl[pose->texId], wp->dlSize[pose->texId]);
    }
}

void puniDraw(void) {
    PunidrvWork* wp = work;
    s32 i;
    PunidrvSort* sort;
    PunidrvPose* pose;
    s32 texId;
    PunidrvTex* tex;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx trans;

    for (i = wp->poseNum - 1, sort = wp->sort; i >= 0; i--, sort++) {
        pose = &wp->poses[sort->index];
        if (pose->flags & 1) {
            texId = pose->texId;
            if (texId != wp->curTex) {
                tex = &wp->texs[texId];
                GXLoadTexObjPreLoaded(smartTexObj(&tex->obj, tex->image), &tex->region, GX_TEXMAP0);
                wp->curTex = texId;
            }
            PSMTXTrans(trans, 0.0f, pose->offsetY, 0.0f);
            PSMTXRotRad(rot, 'Z', 0.017453292f * pose->rotZ);
            PSMTXScale(scale, pose->scale, pose->scale, 1.0f);
            PSMTXConcat(*pose->mtx, trans, mtx);
            PSMTXConcat(mtx, rot, mtx);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(camGetCurPtr()->view, mtx, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXCallDisplayList(wp->dl[texId], wp->dlSize[texId]);
        }
    }
}

void puniLoadRenderMode(void) {
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
}

void puniLoadVertex(void) {
    PunidrvWork* wp = work;

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetArray(GX_VA_POS, wp->vtx, 12);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_TEX0, wp->uvs, 8);
    GXSetCullMode(GX_CULL_NONE);
}

void puniLoadTexture(void) {
    PunidrvWork* wp = work;
    PunidrvTex* tex = wp->texs;
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

u32 puniGetUsedTmemSize(void) {
    return work->usedTmem;
}

void puniLoadTextureExit(void) {
    GXTexModeSync();
}

static u8 shadow_dl[32] ATTRIBUTE_ALIGN(32) = {
    0xA0, 0x00, 0x12, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x01,
};

static f32 shadow_vtx[56] ATTRIBUTE_ALIGN(32) = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.707107f, 0.0f,
    0.707107f, 1.0f, 0.0f, 0.0f, 0.707107f, 0.0f, -0.707106f, 1e-06f,
    0.0f, -1.0f, -0.707106f, 0.0f, -0.707107f, -1.0f, 0.0f, -1e-06f,
    -0.707108f, 0.0f, 0.707106f, -1e-06f, 0.0f, 1.0f, 0.707105f, 0.0f,
    0.707108f, 1.0f, 0.0f, 2e-06f, 0.707108f, 0.0f, -0.707105f, 2e-06f,
    0.0f, -1.0f, -0.707105f, 0.0f, -0.707109f, -1.0f, 0.0f, -2e-06f,
    -0.707108f, 0.0f, 0.707105f,
};

GXColor shadow_col[8] ATTRIBUTE_ALIGN(32) = {
    {0x00, 0x00, 0x00, 0x80},
};

void puniLoadTev(void) {
    GXColor blend;
    GXColor unused0 = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor unused1 = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor fogColor = {0xFF, 0xFF, 0xFF, 0xFF};

    if (!work->useFade) {
        mapGetBlend(&blend);
        GXSetTevColor(GX_TEVREG0, blend);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    } else {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    }
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, fogColor);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
}

static f32 camGetRy(void) {
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

void puniLoadShadowVertex(void) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetArray(GX_VA_POS, shadow_vtx, 12);
    GXSetCullMode(GX_CULL_NONE);
}

void puniLoadShadowTev(void) {
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    {
        GXColor color = {0x00, 0x00, 0x00, 0x40};
        GXSetChanMatColor(GX_COLOR0A0, color);
    }
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
}

void puniLoadShadowRenderMode(void) {
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
}

void puniLoadShadowMtx(Mtx mtx) {
    PSMTXCopy(mtx, work->shadowMtx);
}

void puniDrawShadow(void) {
    PunidrvWork* wp = work;
    s32 i;
    PunidrvSort* sort;
    PunidrvPose* pose;
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
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXCallDisplayList(shadow_dl, 0x20);
        }
    }
}

