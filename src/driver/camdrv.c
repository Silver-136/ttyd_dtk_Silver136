#include "driver/camdrv.h"

#include "battle/battle_camera.h"
#include "cam_road.h"
#include "cam_shift.h"
#include "driver/dispdrv.h"
#include "driver/seqdrv.h"

typedef struct VecRaw {
    u32 x;
    u32 y;
    u32 z;
} VecRaw;

typedef union CamDoubleConv {
    struct {
        u32 hi;
        u32 lo;
    } words;
    f64 value;
} CamDoubleConv;

extern void* camPtrTbl[];
extern void camLetterBox(void);
extern void sysDummyDraw(void* mtx);
extern void camEvalNearFar(void* cam);
extern void GXSetViewport(f32 xOrig, f32 yOrig, f32 wd, f32 ht, f32 nearz, f32 farz);
extern void GXSetScissor(u32 x, u32 y, u32 w, u32 h);
extern void* partyGetPtr(s32 id);
extern s32 bero_get_BeroEXEC(void);
extern s32 marioChkKey(void);
extern s32 marioChkCtrl(void);
extern void psndSetFlag(s32 flag);
extern void psndClearFlag(s32 flag);
extern void camLetterBoxDraw(void);
extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
extern const f32 float_0_8041f6b4;
extern s32 gp;
extern s32 currentCamNo;
extern u32 unk_80429510;
extern const u32 dat_8041f6b0;
extern f32 PSVECDistance(void* a, void* b);
extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
extern void C_MTXPerspective(void* mtx, f32 fovy, f32 aspect, f32 near, f32 far);
extern void GXGetProjectionv(f32* projection);
extern void GXGetScissor(u32* x, u32* y, u32* w, u32* h);
extern void GXSetProjection(void* mtx, u32 type);
extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
extern void GXSetZCompLoc(s32 beforeTex);
extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
extern void GXSetNumChans(s32 count);
extern void GXSetChanMatColor(s32 chan, void* color);
extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
extern void GXSetNumTexGens(s32 count);
extern void GXSetNumTevStages(s32 count);
extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 color);
extern void GXSetTevOp(s32 stage, s32 mode);
extern void GXSetCullMode(s32 mode);
extern void GXClearVtxDesc(void);
extern void GXSetVtxDesc(s32 attr, s32 type);
extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 comptype, s32 compsize, s32 frac);
extern void GXLoadPosMtxImm(void* mtx, s32 id);
extern void GXSetCurrentMtx(s32 id);
extern void GXBegin(s32 primitive, s32 vtxfmt, s32 nverts);
extern void GXSetProjectionv(f32* projection);
extern void* mapGetWork(void);
extern void PSVECSubtract(void* a, void* b, void* out);
extern f32 PSVECDotProduct(void* a, void* b);
typedef void (*CamCallback)(void*);
void* camEntryPersp(
    CamCallback callback,
    const char* name,
    f32 fovy,
    f32 aspect,
    f32 near,
    f32 far,
    f32 viewportNear,
    f32 viewportFar
);
extern void* __memAlloc(s32 heap, u32 size);
extern char* strcpy(char* dst, const char* src);

extern VecRaw camPtDefault;
extern VecRaw camAtDefault;
extern VecRaw camUpDefault;
extern VecRaw vec3_802bf5d8[];

extern const f32 float_8_8041f6fc;
extern const f32 float_400_8041f700;
extern void C_MTXLookAt(void* mtx, void* eye, void* up, void* target);
extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
extern void PSMTXConcat(void* a, void* b, void* out);
extern void PSMTX44Concat(void* a, void* b, void* out);
extern void C_MTXOrtho(void* mtx, f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far);
extern double tan(double value);

extern const f32 float_neg240_8041f704;
extern const f32 float_neg304_8041f708;
extern const f32 float_1p3333_8041f728;
extern const f32 float_10000_8041f720;
extern const f32 float_0p3_8041f724;
extern const f32 float_0p1_8041f738;
extern const f32 float_24_8041f734;

extern const double double_0p21817_802bf658;

extern const char str_off_8041f70c[];
extern const char str_off2_8041f710[];
extern const char str_shadow_802bf648[];
extern const char str_bg_8041f718[];
extern const char str_3d_8041f71c[];
extern const char str_3deff_A_802bf650[];
extern const char str_3dimg_8041f72c[];
extern const char str_3deff_B_802bf660[];
extern const char str_2d_8041f73c[];
extern const char str_fade_8041f740[];
extern const char str_fade2_8041f748[];
extern const char str_dbg_8041f750[];
extern const char str_dbg3d_8041f754[];
extern void* marioGetPtr(void);
extern void* DEMOGetRenderModeObj(void);
extern void GXGetViewportv(f32* viewport);
extern void GXProject(
    f32 x, f32 y, f32 z,
    void* mtx,
    f32* projection,
    f32* viewport,
    f32* outX,
    f32* outY,
    f32* outZ
);

extern const f32 float_deg2rad_8041f6f0;
extern const f32 float_0p5_8041f6f4;
extern const f32 float_0p2_8041f6f8;
extern const f32 float_304_8041f6b8;
extern const f32 float_240_8041f6bc;
extern const f32 dat_8041f6e4;
extern const f32 dat_8041f6e8;
extern const f32 float_neg1_8041f6ec;
extern VecRaw vec3_802bf608[];

extern f32 marioGetCamFollowRate(void);
extern f32 intplGetValue(f32 start, f32 end, u32 type, s32 time, s32 maxTime);
extern void camShiftMain(void* cam, void* mario, f32* out);
extern void camRoadMain(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, void* out);
extern void PSMTXCopy(void* src, void* dst);
extern void PSMTX44Copy(void* src, void* dst);

extern const f32 float_1_8041f6c0;
extern const f32 float_0p01_8041f6c4;
extern const f32 float_1E05_8041f6c8;
extern const f32 float_25_8041f6cc;
extern const f32 float_32768_8041f6d0;
extern const f32 float_100_8041f6d4;
extern const f32 float_1p2667_8041f6d8;
extern const f32 float_608_8041f6dc;
extern const f32 float_480_8041f6e0;
extern const double double_0p21817_802bf638;

typedef struct CamVecRaw {
    u32 x;
    u32 y;
    u32 z;
} CamVecRaw;

typedef struct CamWorkRaw {
    char pad_00[0x0C];
    CamVecRaw pos;      // 0x0C
    CamVecRaw target;   // 0x18
    CamVecRaw up;       // 0x24
    f32 near;           // 0x30
    f32 far;            // 0x34
    f32 fovy;           // 0x38
    f32 aspect;         // 0x3C
    char pad_40[0xD4];
    f32 angle;          // 0x114
    f32 distance;       // 0x118
    char pad_11C[0x40];
} CamWorkRaw;

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

void camEffMain(CamWorkRaw* cam);
void cam3dImgMain(CamWorkRaw* cam);
void cam3dMain(void* cam);

#define INIT_ORTHO_CAM(slot, top, bottom, left, right, camName, wConv, hConv) \
    do {                                                                        \
        renderMode = NULL;                                                      \
        cam = __memAlloc(0, 0x260);                                             \
        memset(cam, 0, 0x260);                                                  \
                                                                                \
        C_MTXOrtho(                                                             \
            (void*)((s32)cam + 0x15C),                                          \
            (top),                                                              \
            (bottom),                                                           \
            (left),                                                             \
            (right),                                                            \
            float_1_8041f6c0,                                                   \
            float_32768_8041f6d0                                                \
        );                                                                      \
                                                                                \
        *(s32*)((s32)cam + 0x19C) = 1;                                          \
                                                                                \
        *(VecRaw*)((s32)cam + 0x0C) = camPtDefault;                             \
        *(VecRaw*)((s32)cam + 0x18) = camAtDefault;                             \
        *(VecRaw*)((s32)cam + 0x24) = camUpDefault;                             \
                                                                                \
        *(f32*)((s32)cam + 0x30) = float_1_8041f6c0;                            \
        *(f32*)((s32)cam + 0x34) = float_32768_8041f6d0;                        \
        *(f32*)((s32)cam + 0x114) = float_0_8041f6b4;                           \
        *(f32*)((s32)cam + 0x118) = PSVECDistance(                              \
            (void*)((s32)cam + 0x0C),                                           \
            (void*)((s32)cam + 0x18)                                            \
        );                                                                      \
                                                                                \
        *(void**)((s32)cam + 0x1EC) = NULL;                                     \
        *(s16*)((s32)cam + 0x2) = 0;                                            \
                                                                                \
        *(f32*)((s32)cam + 0x14C) = float_0_8041f6b4;                           \
        *(f32*)((s32)cam + 0x158) = float_0_8041f6b4;                           \
        *(f32*)((s32)cam + 0x154) = float_0_8041f6b4;                           \
        *(f32*)((s32)cam + 0x150) = float_0_8041f6b4;                           \
        *(f32*)((s32)cam + 0x3C) = float_1p2667_8041f6d8;                       \
        *(f32*)((s32)cam + 0x38) = float_25_8041f6cc;                           \
        *(s32*)((s32)cam + 0x1E8) = 8;                                          \
                                                                                \
        strcpy((char*)((s32)cam + 0x254), (camName));                           \
                                                                                \
        *(s16*)((s32)cam + 0xF4) = 0;                                           \
        *(s16*)((s32)cam + 0xF6) = 0;                                           \
                                                                                \
        renderMode = DEMOGetRenderModeObj();                                   \
        *(u16*)((s32)cam + 0xF8) = *(u16*)((s32)renderMode + 0x4);              \
                                                                                \
        renderMode = DEMOGetRenderModeObj();                                   \
        *(u16*)((s32)cam + 0xFA) = *(u16*)((s32)renderMode + 0x6);              \
                                                                                \
        *(f32*)((s32)cam + 0xFC) = float_0_8041f6b4;                            \
        *(f32*)((s32)cam + 0x100) = float_0_8041f6b4;                           \
                                                                                \
        renderMode = DEMOGetRenderModeObj();                                   \
        (wConv).words.hi = 0x43300000;                                         \
        (wConv).words.lo = *(u16*)((s32)renderMode + 0x4);                     \
        *(f32*)((s32)cam + 0x104) =                                            \
            (f32)((wConv).value - *(f64*)((s32)defaults + 0x68));              \
                                                                                \
        renderMode = DEMOGetRenderModeObj();                                   \
        (hConv).words.hi = 0x43300000;                                         \
        (hConv).words.lo = *(u16*)((s32)renderMode + 0x6);                     \
        *(f32*)((s32)cam + 0x108) =                                            \
            (f32)((hConv).value - *(f64*)((s32)defaults + 0x68));              \
                                                                                \
        *(f32*)((s32)cam + 0x10C) = float_0_8041f6b4;                          \
        *(f32*)((s32)cam + 0x110) = float_1_8041f6c0;                          \
                                                                                \
        camPtrTbl[(slot)] = cam;                                               \
    } while (0)

void camInit(void) {
    VecRaw* defaults;
    CamDoubleConv w0;
    CamDoubleConv h0;
    CamDoubleConv w1;
    CamDoubleConv h1;
    CamDoubleConv w2;
    CamDoubleConv h2;
    CamDoubleConv w3;
    CamDoubleConv h3;
    CamDoubleConv w11;
    CamDoubleConv h11;
    void* cam;
    void* renderMode;
    f32 z;
    VecRaw pos6;
    VecRaw pos8;
    VecRaw pos9;
    VecRaw pos10;
    defaults = vec3_802bf5d8;

    INIT_ORTHO_CAM(
        0,
        float_240_8041f6bc,
        float_neg240_8041f704,
        float_neg304_8041f708,
        float_304_8041f6b8,
        str_off_8041f70c,
        w0,
        h0
    );

    INIT_ORTHO_CAM(
        1,
        float_0_8041f6b4,
        float_480_8041f6e0,
        float_0_8041f6b4,
        float_608_8041f6dc,
        str_off2_8041f710,
        w1,
        h1
    );

    INIT_ORTHO_CAM(
        2,
        float_0_8041f6b4,
        float_480_8041f6e0,
        float_0_8041f6b4,
        float_608_8041f6dc,
        str_shadow_802bf648,
        w2,
        h2
    );

    INIT_ORTHO_CAM(
        3,
        float_0_8041f6b4,
        float_480_8041f6e0,
        float_0_8041f6b4,
        float_608_8041f6dc,
        str_bg_8041f718,
        w3,
        h3
    );

    camPtrTbl[4] = camEntryPersp(
        (CamCallback)cam3dMain,
        str_3d_8041f71c,
        float_25_8041f6cc,
        float_1p2667_8041f6d8,
        float_1_8041f6c0,
        float_32768_8041f6d0,
        float_0_8041f6b4,
        float_1_8041f6c0
    );

    camPtrTbl[5] = camEntryPersp(
        (CamCallback)camEffMain,
        str_3deff_A_802bf650,
        float_25_8041f6cc,
        float_1p2667_8041f6d8,
        float_100_8041f6d4,
        float_10000_8041f720,
        float_0p2_8041f6f8,
        float_0p3_8041f724
    );

    camPtrTbl[6] = camEntryPersp(
        (CamCallback)cam3dImgMain,
        str_3dimg_8041f72c,
        float_25_8041f6cc,
        float_1p3333_8041f728,
        float_1_8041f6c0,
        float_32768_8041f6d0,
        float_0_8041f6b4,
        float_1_8041f6c0
    );

    z = float_24_8041f734 / (f32)tan(double_0p21817_802bf658);
    *(f32*)((s32)camPtrTbl[6] + 0x0C) = float_0_8041f6b4;
    *(f32*)((s32)camPtrTbl[6] + 0x10) = float_0_8041f6b4;
    *(f32*)((s32)camPtrTbl[6] + 0x14) = z;

    camPtrTbl[7] = camEntryPersp(
        (CamCallback)camEffMain,
        str_3deff_B_802bf660,
        float_0_8041f6b4,
        float_1p2667_8041f6d8,
        float_100_8041f6d4,
        float_10000_8041f720,
        float_0p2_8041f6f8,
        float_0p3_8041f724
    );

    camPtrTbl[8] = camEntryPersp(
        (CamCallback)0,
        str_2d_8041f73c,
        float_25_8041f6cc,
        float_1p2667_8041f6d8,
        float_100_8041f6d4,
        float_10000_8041f720,
        float_0p1_8041f738,
        float_0p2_8041f6f8
    );

    z = float_240_8041f6bc / (f32)tan(double_0p21817_802bf658);
    pos8 = *(VecRaw*)((s32)defaults + 0x48);
    *(f32*)&pos8.x = float_0_8041f6b4;
    *(f32*)&pos8.y = float_0_8041f6b4;
    *(f32*)&pos8.z = z;
    *(VecRaw*)((s32)camPtrTbl[8] + 0x0C) = pos8;

    camPtrTbl[9] = camEntryPersp(
        (CamCallback)0,
        str_fade_8041f740,
        float_25_8041f6cc,
        float_1p3333_8041f728,
        float_1_8041f6c0,
        float_32768_8041f6d0,
        float_0_8041f6b4,
        float_0p1_8041f738
    );

    z = float_24_8041f734 / (f32)tan(double_0p21817_802bf658);
    pos9 = *(VecRaw*)((s32)defaults + 0x48);
    *(f32*)&pos9.x = float_0_8041f6b4;
    *(f32*)&pos9.y = float_0_8041f6b4;
    *(f32*)&pos9.z = z;
    *(VecRaw*)((s32)camPtrTbl[9] + 0x0C) = pos9;

    camPtrTbl[10] = camEntryPersp(
        (CamCallback)0,
        str_fade2_8041f748,
        float_25_8041f6cc,
        float_1p3333_8041f728,
        float_1_8041f6c0,
        float_32768_8041f6d0,
        float_0_8041f6b4,
        float_0p1_8041f738
    );

    z = float_24_8041f734 / (f32)tan(double_0p21817_802bf658);
    pos10 = *(VecRaw*)((s32)defaults + 0x48);
    *(f32*)&pos10.x = float_0_8041f6b4;
    *(f32*)&pos10.y = float_0_8041f6b4;
    *(f32*)&pos10.z = z;
    *(VecRaw*)((s32)camPtrTbl[10] + 0x0C) = pos10;
    INIT_ORTHO_CAM(
        11,
        float_0_8041f6b4,
        float_480_8041f6e0,
        float_0_8041f6b4,
        float_608_8041f6dc,
        str_dbg_8041f750,
        w11,
        h11
    );

    camPtrTbl[12] = camEntryPersp(
        (CamCallback)0,
        str_dbg3d_8041f754,
        float_25_8041f6cc,
        float_1p2667_8041f6d8,
        float_1_8041f6c0,
        float_32768_8041f6d0,
        float_0_8041f6b4,
        float_1_8041f6c0
    );
}

void* camEntryPersp(
    CamCallback callback,
    const char* name,
    f32 fovy,
    f32 aspect,
    f32 near,
    f32 far,
    f32 viewportNear,
    f32 viewportFar
) {
    VecRaw* defaults;
    CamDoubleConv widthConv;
    CamDoubleConv heightConv;
    void* cam;
    void* renderMode;

    defaults = vec3_802bf5d8;

    cam = __memAlloc(0, 0x260);
    memset(cam, 0, 0x260);

    C_MTXPerspective(
        (void*)((s32)cam + 0x15C),
        fovy,
        aspect,
        near,
        far
    );

    *(s32*)((s32)cam + 0x19C) = 0;

    *(VecRaw*)((s32)cam + 0x0C) = camPtDefault;
    *(VecRaw*)((s32)cam + 0x18) = camAtDefault;
    *(VecRaw*)((s32)cam + 0x24) = camUpDefault;

    *(f32*)((s32)cam + 0x30) = near;
    *(f32*)((s32)cam + 0x34) = far;
    *(f32*)((s32)cam + 0x114) = float_0_8041f6b4;

    *(f32*)((s32)cam + 0x118) = PSVECDistance(
        (void*)((s32)cam + 0x0C),
        (void*)((s32)cam + 0x18)
    );

    *(void**)((s32)cam + 0x1EC) = callback;

    *(s16*)((s32)cam + 0x2) = 0;
    *(s16*)((s32)cam + 0x4) = 0;
    *(f32*)((s32)cam + 0x8) = float_0_8041f6b4;

    *(f32*)((s32)cam + 0x38) = fovy;
    *(f32*)((s32)cam + 0x3C) = aspect;

    *(f32*)((s32)cam + 0x14C) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x158) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x154) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x150) = float_0_8041f6b4;

    *(s32*)((s32)cam + 0x1E8) = 8;

    strcpy((char*)((s32)cam + 0x254), name);

    *(s16*)((s32)cam + 0xF4) = 0;
    *(s16*)((s32)cam + 0xF6) = 0;

    renderMode = DEMOGetRenderModeObj();
    *(u16*)((s32)cam + 0xF8) = *(u16*)((s32)renderMode + 0x4);

    renderMode = DEMOGetRenderModeObj();
    *(u16*)((s32)cam + 0xFA) = *(u16*)((s32)renderMode + 0x6);

    *(f32*)((s32)cam + 0xFC) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x100) = float_0_8041f6b4;

    renderMode = DEMOGetRenderModeObj();
    widthConv.words.hi = 0x43300000;
    widthConv.words.lo = *(u16*)((s32)renderMode + 0x4);
    *(f32*)((s32)cam + 0x104) = (f32)(widthConv.value - *(f64*)((s32)defaults + 0x68));

    renderMode = DEMOGetRenderModeObj();
    heightConv.words.hi = 0x43300000;
    heightConv.words.lo = *(u16*)((s32)renderMode + 0x6);
    *(f32*)((s32)cam + 0x108) = (f32)(heightConv.value - *(f64*)((s32)defaults + 0x68));

    *(f32*)((s32)cam + 0x10C) = viewportNear;
    *(f32*)((s32)cam + 0x110) = viewportFar;

    *(f32*)((s32)cam + 0x1F0) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x1F4) = float_8_8041f6fc;
    *(f32*)((s32)cam + 0x1F8) = float_400_8041f700;

    *(f32*)((s32)cam + 0x20C) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x210) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x214) = float_0_8041f6b4;

    *(f32*)((s32)cam + 0x200) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x204) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x208) = float_0_8041f6b4;

    *(s16*)((s32)cam + 0x218) = 0;
    *(s16*)((s32)cam + 0x21A) = 0;
    *(s16*)((s32)cam + 0x21C) = 0;
    *(s16*)((s32)cam + 0x21E) = 0;

    *(VecRaw*)((s32)cam + 0x224) = defaults[0];
    *(VecRaw*)((s32)cam + 0x230) = defaults[1];
    *(VecRaw*)((s32)cam + 0x23C) = defaults[2];
    *(VecRaw*)((s32)cam + 0x248) = defaults[3];

    return cam;
}

void camMain(void) {
    void** camPtr;
    void* cam;
    void* cam4;
    void* player;
    void* renderMode;
    CamCallback callback;
    s32 i;

    VecRaw pointB;

    f32 projectedZ;
    f32 projectedY;
    f32 projectedX;

    VecRaw pointA;

    f32* projectedZPtr;
    f32* projectedYPtr;

    f32 deg2rad;
    f32 strength;
    f32 zoom;
    f32 baseScale;
    f32 extentX;
    f32 extentY;
    f32 offsetX;
    f32 offsetY;
    f32 limit;
    f32 zoomScale;

    f32 oldProjection[7];
    f32 viewport[6];
    f32 projection[7];

    f32 rotMtx[3][4];
    f32 postTransMtx[3][4];
    f32 screenTransMtx[3][4];
    f32 scaleMtx[3][4];
    f32 concatMtx[3][4];

    deg2rad = float_deg2rad_8041f6f0;

    projectedZPtr = &projectedZ;
    projectedYPtr = &projectedY;

    camPtr = camPtrTbl;
    for (i = 0; i < 13; i++, camPtr++) {
        cam = *camPtr;

        callback = *(CamCallback*)((s32)cam + 0x1EC);
        if (callback != NULL) {
            callback(cam);
        }

        C_MTXLookAt(
            (void*)((s32)cam + 0x11C),
            (void*)((s32)cam + 0x0C),
            (void*)((s32)cam + 0x24),
            (void*)((s32)cam + 0x18)
        );

        PSMTXRotRad(
            rotMtx,
            'z',
            deg2rad * *(f32*)((s32)cam + 0x14C)
        );

        PSMTXTrans(
            postTransMtx,
            *(f32*)((s32)cam + 0x150),
            *(f32*)((s32)cam + 0x154),
            *(f32*)((s32)cam + 0x158)
        );

        PSMTXConcat(
            rotMtx,
            (void*)((s32)cam + 0x11C),
            (void*)((s32)cam + 0x11C)
        );

        PSMTXConcat(
            postTransMtx,
            (void*)((s32)cam + 0x11C),
            (void*)((s32)cam + 0x11C)
        );

        if ((i == 4) && (*(s32*)((s32)gp + 0x14) == 0)) {
            player = marioGetPtr();
            strength = *(f32*)((s32)player + 0x64);

            pointA = vec3_802bf608[0];

            player = marioGetPtr();
            *(f32*)&pointA.x = *(f32*)((s32)player + 0x8C) + *(f32*)((s32)player + 0x6C);

            player = marioGetPtr();
            *(f32*)&pointA.y = *(f32*)((s32)player + 0x90) + *(f32*)((s32)player + 0x70);

            player = marioGetPtr();
            *(f32*)&pointA.z = *(f32*)((s32)player + 0x94) + *(f32*)((s32)player + 0x74);

            pointB = pointA;

            player = marioGetPtr();
            zoom = *(f32*)((s32)player + 0x68);

            renderMode = DEMOGetRenderModeObj();
            baseScale = strength * (float_1_8041f6c0 - (float_1_8041f6c0 / zoom));
            extentX = baseScale * (f32)*(u16*)((s32)renderMode + 0x4);

            renderMode = DEMOGetRenderModeObj();
            extentY = baseScale * (f32)*(u16*)((s32)renderMode + 0x6);

            GXGetProjectionv(oldProjection);

            cam4 = camPtrTbl[4];
            GXSetProjection(
                (void*)((s32)cam4 + 0x15C),
                *(u32*)((s32)cam4 + 0x19C)
            );

            GXGetProjectionv(projection);
            GXGetViewportv(viewport);

            GXProject(
                *(f32*)&pointB.x,
                *(f32*)&pointB.y,
                *(f32*)&pointB.z,
                (void*)((s32)cam4 + 0x11C),
                projection,
                viewport,
                &projectedX,
                projectedYPtr,
                projectedZPtr
            );

            GXSetProjectionv(oldProjection);

            projectedX = projectedX - float_304_8041f6b8;
            projectedY = float_240_8041f6bc - projectedY;

            if (strength > float_1_8041f6c0) {
                strength = float_1_8041f6c0;
            } else if (strength < float_0_8041f6b4) {
                strength = float_0_8041f6b4;
            }

            offsetX = float_0p5_8041f6f4 * projectedX * strength;
            offsetY = float_0p5_8041f6f4 * projectedY * strength;

            limit = -extentX * float_0p2_8041f6f8;
            if (offsetX < limit) {
                offsetX = limit;
            } else {
                limit = extentX * float_0p2_8041f6f8;
                if (offsetX > limit) {
                    offsetX = limit;
                }
            }

            limit = -extentY * float_0p2_8041f6f8;
            if (offsetY < limit) {
                offsetY = limit;
            } else {
                limit = extentY * float_0p2_8041f6f8;
                if (offsetY > limit) {
                    offsetY = limit;
                }
            }

            zoomScale = (strength * zoom) + (float_1_8041f6c0 - strength);

            PSMTXTrans(
                screenTransMtx,
                -offsetX,
                -offsetY,
                float_0_8041f6b4
            );

            PSMTXScale(
                scaleMtx,
                zoomScale,
                zoomScale,
                float_1_8041f6c0
            );

            PSMTXConcat(scaleMtx, screenTransMtx, concatMtx);

            PSMTXConcat(
                concatMtx,
                (void*)((s32)cam + 0x11C),
                (void*)((s32)cam + 0x11C)
            );
        }

        if ((*(u16*)cam & 8) != 0) {
            PSMTX44Concat(
                (void*)((s32)cam + 0x1A0),
                (void*)((s32)cam + 0x15C),
                (void*)((s32)cam + 0x15C)
            );

            *(u16*)cam &= ~8;
        }
    }
}

void camEvalNearFar(void* cam) {
    typedef struct CamEvalWork {
        u8 pad_00[0x0C];
        Vec3 pos;               // 0x0C
        u8 pad_18[0x30 - 0x18];
        f32 nearClip;           // 0x30
        f32 farClip;            // 0x34
        u8 pad_38[0x10C - 0x38];
        f32 screenNear;         // 0x10C
        f32 screenFar;          // 0x110
        u8 pad_114[0x11C - 0x114];
        f32 viewMtx[3][4];      // 0x11C
        u8 pad_14C[0x184 - 0x14C];
        f32 scale;              // 0x184
        f32 bias;               // 0x188
    } CamEvalWork;

    typedef struct CamEvalMap {
        u8 pad_00[0xB0];
        Vec3 point;             // 0xB0
    } CamEvalMap;

    CamEvalWork* camWork;
    void* map;
    s32 i;

    f32 zero;
    f32 scale;
    f32 bias;
    f32 screenFar;
    f32 nearDepth;
    f32 farDepth;
    f32 screenRange;
    f32 minValue;
    f32 maxValue;

    f32 screenNear;
    f32 z;
    f32 value;
    Vec3 diff;
    Vec3 transformed;

    camWork = (CamEvalWork*)cam;

    nearDepth = float_1_8041f6c0;
    farDepth = float_32768_8041f6d0;
    minValue = dat_8041f6e4;
    maxValue = dat_8041f6e8;

    map = mapGetWork();

    screenFar = camWork->screenFar;
    screenNear = camWork->screenNear;
    scale = camWork->scale;
    screenRange = screenFar - screenNear;
    bias = camWork->bias;
    zero = float_0_8041f6b4;

    for (i = 0; i < 8; i++, map = (void*)((s32)map + 0xC)) {
        PSVECSubtract(
            (void*)((s32)map + 0xB0),
            &camWork->pos,
            &diff
        );

        if (PSVECDotProduct(&diff, (void*)&camWork->viewMtx[2][0]) >= zero) {
            if (zero != minValue) {
                minValue = zero;
                nearDepth = float_neg1_8041f6ec;
            }
        } else {
            PSMTXMultVec(
                camWork->viewMtx,
                (Vec*)&((CamEvalMap*)map)->point,
                (Vec*)&transformed
            );

            z = transformed.z;
            value = (float_1_8041f6c0 / -z) * ((scale * z) + bias);
            value = (screenRange * value) + screenFar;

            if (value < minValue) {
                minValue = value;
                nearDepth = z;
            }

            if (value > maxValue) {
                maxValue = value;
                farDepth = z;
            }
        }
    }

    farDepth = -farDepth;
    nearDepth = -nearDepth;

    if (farDepth < float_1_8041f6c0) {
        farDepth = float_1_8041f6c0;
    }

    camWork->nearClip = *(volatile f32*)&float_1_8041f6c0;
    camWork->nearClip = nearDepth;
    camWork->farClip = farDepth;
}

void camDraw(void) {
    void** camPtr;
    s32 i;
    void* cam;

    camLetterBox();
    dispSort();

    camPtr = camPtrTbl;
    for (i = 0; i < 13; i++, camPtr++) {
        cam = *camPtr;

        if ((*(u16*)cam & 1) == 0) {
            currentCamNo = i;

            GXSetViewport(
                *(f32*)((s32)cam + 0xFC),
                *(f32*)((s32)cam + 0x100),
                *(f32*)((s32)cam + 0x104),
                *(f32*)((s32)cam + 0x108),
                *(f32*)((s32)cam + 0x10C),
                *(f32*)((s32)cam + 0x110)
            );

            GXSetScissor(
                *(u16*)((s32)cam + 0xF4),
                *(u16*)((s32)cam + 0xF6),
                *(u16*)((s32)cam + 0xF8),
                *(u16*)((s32)cam + 0xFA)
            );

            if (((*(u16*)cam & 0x40) != 0) && (*(s32*)((s32)cam + 0x19C) == 0)) {
                camEvalNearFar(cam);

                C_MTXPerspective(
                    (void*)((s32)cam + 0x15C),
                    *(f32*)((s32)cam + 0x38),
                    *(f32*)((s32)cam + 0x3C),
                    *(f32*)((s32)cam + 0x30),
                    *(f32*)((s32)cam + 0x34)
                );
            }

            GXSetProjection((void*)((s32)cam + 0x15C), *(u32*)((s32)cam + 0x19C));
            dispDraw(i);
            sysDummyDraw((void*)((s32)cam + 0x11C));
        }
    }

    dispReInit();
}

void camLoadRoad(s32 camId, const char* name) {
    void* cam;

    cam = camPtrTbl[camId];

    *(u16*)cam &= ~0x10;

    *(f32*)((s32)cam + 0x90) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x8C) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x88) = float_0_8041f6b4;

    if (camRoadSetup(name) != 0) {
        *(u16*)cam |= 0x4;
        *(u16*)((s32)camPtrTbl[camId] + 0x2) = 1;
    } else {
        *(u16*)cam &= ~0x4;
        *(u16*)((s32)camPtrTbl[camId] + 0x2) = 3;
    }

    *(f32*)((s32)cam + 0x38) = float_25_8041f6cc;

    *(f32*)((s32)cam + 0x14C) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x158) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x154) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x150) = float_0_8041f6b4;

    *(u16*)((s32)cam + 0x4) = 0;
    *(u8*)((s32)cam + 0x80) = 0xB;
    *(f32*)((s32)cam + 0x8) = float_0_8041f6b4;
    *(u8*)((s32)cam + 0xE0) = 0xB;
    *(f32*)((s32)cam + 0x84) = float_0_8041f6b4;

    *(f32*)((s32)cam + 0x9C) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x98) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x94) = float_0_8041f6b4;
}

void camUnLoadRoad(s32 camId) {
    void* cam;

    cam = camPtrTbl[camId];

    *(u16*)cam &= ~4;
    *(u16*)((s32)camPtrTbl[camId] + 0x2) = 0;
    *(u16*)((s32)cam + 0x4) = 0;

    *(f32*)((s32)cam + 0x8) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x38) = float_25_8041f6cc;

    *(f32*)((s32)cam + 0x14C) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x158) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x154) = float_0_8041f6b4;
    *(f32*)((s32)cam + 0x150) = float_0_8041f6b4;

    *(u16*)cam &= ~8;
}

void* camGetPtr(s32 camId) {
    return camPtrTbl[camId];
}

void* camGetCurPtr(void) {
    return camPtrTbl[currentCamNo];
}

s32 camGetCurNo(void) {
    return currentCamNo;
}

void camSetCurNo(s32 camNo) {
    currentCamNo = camNo;
}

void cam3dMain(void* cam) {
    typedef struct CameraEntryLocal {
        u16 flags;
        u16 mode;
        u16 stateA;
        u8 align_6[2];
        f32 progressA;
        Vec3 cameraPos;
        Vec3 cameraAt;
        Vec3 cameraUp;
        f32 near;
        f32 far;
        f32 fovY;
        f32 aspect;
        Vec3 storedPos;
        Vec3 storedAt;
        Vec3 presetPos;
        Vec3 presetAt;
        s64 startTimeA;
        s64 durationA;
        u8 curveA;
        u8 align_81[1];
        u16 stateB;
        f32 progressB;
        Vec3 blendPos;
        Vec3 marioPos;
        Vec3 blendAt;
        Vec3 marioAt;
        Vec3 anchor;
        Vec3 anchorPrev;
        s64 startTimeB;
        s64 durationB;
        u8 curveB;
        u8 align_E1[3];
        f32 dollyPercent;
        f32 dollyStart;
        f32 dollyEnd;
        f32 dollyStartAlt;
        u16 scissor[4];
        f32 viewport[6];
        f32 viewYaw;
        f32 viewDistance;
        f32 view[3][4];
        f32 rollAngle;
        Vec3 viewOffset;
        f32 projection[4][4];
        s32 projType;
    } CameraEntryLocal;
    typedef struct CameraParamsLocal {
        f32 fovY;
        f32 aspect;
        f32 far;
        Vec3 pos;
        Vec3 at;
    } CameraParamsLocal;
    typedef struct MarioWorkLocal {
        u8 pad_00[0xEC];
        Vec3 field_EC;
        Vec3 field_F8;
    } MarioWorkLocal;
    CameraEntryLocal* camera = cam;

    if (camera->flags & 2) {
        return;
    }

    switch (camera->mode) {
        case 1:
            switch (camera->stateA) {
                case 1:
                    if (camera->durationA > 0) {
                        camera->progressA = float_1_8041f6c0 - (f32)(*(s64*)((s32)gp + 0x38) - camera->startTimeA) /
                            (f32)camera->durationA;
                        if (camera->progressA < float_0_8041f6b4) {
                            camera->progressA = float_0_8041f6b4;
                            camera->stateA = 0;
                        }
                    } else {
                        camera->progressA = float_0_8041f6b4;
                        camera->stateA = 0;
                    }
                    break;
                case 2:
                case 3:
                    if (camera->durationA > 0) {
                        camera->progressA = (f32)(*(s64*)((s32)gp + 0x38) - camera->startTimeA) /
                            (f32)camera->durationA;
                        if (camera->progressA > float_1_8041f6c0) {
                            camera->progressA = float_1_8041f6c0;
                            camera->stateA = 0;
                        }
                    } else {
                        camera->progressA = float_1_8041f6c0;
                        camera->stateA = 0;
                    }
                    break;
                case 0:
                    break;
            }

            switch (camera->stateB) {
                case 1:
                    if (camera->durationB > 0) {
                        camera->progressB = float_1_8041f6c0 - (f32)(*(s64*)((s32)gp + 0x38) - camera->startTimeB) /
                            (f32)camera->durationB;
                        if (camera->progressB < float_0_8041f6b4) {
                            camera->progressB = float_0_8041f6b4;
                            camera->stateB = 0;
                        }
                    } else {
                        camera->progressB = float_0_8041f6b4;
                        camera->stateB = 0;
                    }
                    break;
                case 2:
                case 3:
                    if (camera->durationB > 0) {
                        camera->progressB = (f32)(*(s64*)((s32)gp + 0x38) - camera->startTimeB) /
                            (f32)camera->durationB;
                        if (camera->progressB > float_1_8041f6c0) {
                            camera->progressB = float_1_8041f6c0;
                            camera->stateB = 0;
                        }
                    } else {
                        camera->progressB = float_1_8041f6c0;
                        camera->stateB = 0;
                    }
                    break;
                case 0:
                    break;
            }

            if (!(camera->flags & 0x10)) {
                MarioWorkLocal* mario = marioGetPtr();
                camera->marioPos.y += ((float_1_8041f6c0 + mario->field_EC.y - camera->marioPos.y) * marioGetCamFollowRate());
            }
            {
                MarioWorkLocal* mario = marioGetPtr();
                camera->marioPos.x = float_0p01_8041f6c4 + mario->field_EC.x;
                camera->marioPos.z = mario->field_EC.z;
                camera->dollyStart = float_0_8041f6b4;
                camera->marioAt.x = mario->field_F8.x;
                camera->marioAt.y = float_1_8041f6c0 + mario->field_F8.y;
                camera->marioAt.z = mario->field_F8.z;
            }

            {
                f32 t, it;
                t = intplGetValue(float_0_8041f6b4, float_1_8041f6c0, camera->curveB,
                    (s32)(float_1E05_8041f6c8 * camera->progressB), 100000);
                it = float_1_8041f6c0 - t;
                switch (camera->stateB) {
                    case 0:
                    case 1:
                    case 2:
                        camera->blendPos.x = (camera->anchor.x * t) + (camera->marioPos.x * it);
                        camera->blendPos.y = (camera->anchor.y * t) + (camera->marioPos.y * it);
                        camera->blendPos.z = (camera->anchor.z * t) + (camera->marioPos.z * it);
                        camera->blendAt.x = (camera->anchor.x * t) + (camera->marioAt.x * it);
                        camera->blendAt.y = (camera->anchor.y * t) + (camera->marioAt.y * it);
                        camera->blendAt.z = (camera->anchor.z * t) + (camera->marioAt.z * it);
                        camera->dollyPercent = (camera->dollyEnd * t) + (camera->dollyStart * it);
                        break;
                    case 3:
                        camera->blendPos.x = (camera->anchor.x * t) + (camera->anchorPrev.x * it);
                        camera->blendPos.y = (camera->anchor.y * t) + (camera->anchorPrev.y * it);
                        camera->blendPos.z = (camera->anchor.z * t) + (camera->anchorPrev.z * it);
                        camera->blendAt = camera->blendPos;
                        camera->dollyPercent = (camera->dollyEnd * t) + (camera->dollyStartAlt * it);
                        break;
                }
            }

            {
                CameraParamsLocal work;
                CameraParamsLocal work2;
                work.fovY = float_25_8041f6cc;
                work.aspect = float_1_8041f6c0;
                work.far = float_32768_8041f6d0;
                work.pos = camera->storedPos;
                work.at = camera->storedAt;

                switch (camera->stateA) {
                    case 0:
                    case 1:
                    case 2:
                        if (camera->flags & 4) {
                            MarioWorkLocal* mario = marioGetPtr();
                            f32 bank = float_0_8041f6b4;
                            f32 dolly;
                            f32 oldx, oldy, oldz;
                            camShiftMain(camera, mario, &bank);
                            camRoadMain(camera->blendPos.x, camera->blendPos.y, camera->blendPos.z,
                                camera->blendAt.x, camera->blendAt.y, camera->blendAt.z, &work2);
                            dolly = camera->dollyPercent;
                            oldx = work2.pos.x;
                            oldy = work2.pos.y;
                            oldz = work2.pos.z;
                            work2.pos.x = oldx + (work2.at.x - oldx) * dolly / float_100_8041f6d4;
                            work2.pos.y = oldy + (work2.at.y - oldy) * dolly / float_100_8041f6d4;
                            work2.pos.z = oldz + (work2.at.z - oldz) * dolly / float_100_8041f6d4;
                        } else {
                            work2 = work;
                        }
                        break;
                    case 3:
                        work2.fovY = work.fovY;
                        work2.aspect = work.aspect;
                        work2.far = work.far;
                        work2.pos = camera->presetPos;
                        work2.at = camera->presetAt;
                        break;
                }

                camShiftPostMain();
                {
                    f32 t, it;
                    t = intplGetValue(float_0_8041f6b4, float_1_8041f6c0, camera->curveA,
                        (s32)(float_1E05_8041f6c8 * (float_1_8041f6c0 - camera->progressA)), 100000);
                    it = float_1_8041f6c0 - t;
                    camera->cameraAt.x = (work.at.x * it) + (work2.at.x * t);
                    camera->cameraAt.y = (work.at.y * it) + (work2.at.y * t);
                    camera->cameraAt.z = (work.at.z * it) + (work2.at.z * t);
                    camera->cameraPos.x = (work.pos.x * it) + (work2.pos.x * t);
                    camera->cameraPos.y = (work.pos.y * it) + (work2.pos.y * t);
                    camera->cameraPos.z = (work.pos.z * it) + (work2.pos.z * t);
                }
            }

            camera->viewDistance = PSVECDistance(&camera->cameraPos, &camera->cameraAt);
            camera->viewYaw = angleABf(camera->cameraPos.x, camera->cameraPos.z,
                camera->cameraAt.x, camera->cameraAt.z);
            if (camera->projType == 0) {
                C_MTXPerspective(camera->projection, camera->fovY, float_1p2667_8041f6d8, camera->near, camera->far);
            } else {
                f32 h, w;
                h = camera->viewDistance * (f32)tan(double_0p21817_802bf638);
                w = float_608_8041f6dc * h / float_480_8041f6e0;
                C_MTXOrtho(camera->projection, h, -h, -w, w, camera->near, camera->far);
            }
            break;

        case 2:
            if (*(s32*)((s32)gp + 0x14)) {
                battleCameraMain();
                camera->viewDistance = PSVECDistance(&camera->cameraPos, &camera->cameraAt);
                camera->viewYaw = angleABf(camera->cameraPos.x, camera->cameraPos.z,
                    camera->cameraAt.x, camera->cameraAt.z);
                if (camera->projType == 0) {
                    C_MTXPerspective(camera->projection, camera->fovY, float_1p2667_8041f6d8, camera->near, camera->far);
                } else {
                    f32 h, w;
                    h = camera->viewDistance * (f32)tan(double_0p21817_802bf638);
                    w = float_608_8041f6dc * h / float_480_8041f6e0;
                    C_MTXOrtho(camera->projection, h, -h, -w, w, camera->near, camera->far);
                }
            }
            break;
        case 0:
        case 3:
            break;
    }

    {
        CameraEntryLocal* camera3D = camGetPtr(4);
        CameraEntryLocal* offscreen2 = camGetPtr(1);
        PSMTXCopy(camera3D->view, offscreen2->view);
        PSMTX44Copy(camera3D->projection, offscreen2->projection);
        offscreen2->projType = camera3D->projType;
    }
}

void cam3dImgMain(CamWorkRaw* cam) {
    C_MTXPerspective(
        (void*)((s32)cam + 0x15C),
        cam->fovy,
        cam->aspect,
        cam->near,
        cam->far
    );
}

void camEffMain(CamWorkRaw* cam) {
    CamWorkRaw* src;

    src = camPtrTbl[4];

    cam->pos = src->pos;
    cam->up = src->up;
    cam->target = src->target;

    cam->distance = PSVECDistance(&cam->pos, &cam->target);

    cam->angle = angleABf(
        *(f32*)&cam->pos.x,
        *(f32*)&cam->pos.z,
        *(f32*)&cam->target.x,
        *(f32*)&cam->target.z
    );

    C_MTXPerspective(
        (void*)((s32)cam + 0x15C),
        cam->fovy,
        cam->aspect,
        cam->near,
        cam->far
    );
}

void camSetMode(s32 camId, s16 mode) {
    *(s16*)((s32)camPtrTbl[camId] + 0x2) = mode;
}

void L_camDispOn(s32 camId) {
    *(u16*)camPtrTbl[camId] &= ~1;
}

void L_camDispOff(s32 camId) {
    *(u16*)camPtrTbl[camId] |= 1;
}

void camCtrlOn(s32 camId) {
    *(u16*)camPtrTbl[camId] &= ~2;
}

void camCtrlOff(s32 camId) {
    *(u16*)camPtrTbl[camId] |= 2;
}

void camSetTypePersp(s32 camId) {
    *(s32*)((s32)camPtrTbl[camId] + 0x19C) = 0;
}

void camSetTypeOrtho(s32 camId) {
    *(s32*)((s32)camPtrTbl[camId] + 0x19C) = 1;
}

void getScreenPoint(void* position, f32* output) {
    Vec3* pos = (Vec3*)position;
    Vec3* out = (Vec3*)output;
    void* cam;
    f32 projection[7];
    f32 viewport[6];
    f32 oldProjection[7];

    GXGetProjectionv(oldProjection);

    cam = camPtrTbl[4];

    GXSetProjection((void*)((s32)cam + 0x15C), *(u32*)((s32)cam + 0x19C));

    GXGetProjectionv(projection);
    GXGetViewportv(viewport);

    GXProject(
        pos->x,
        pos->y,
        pos->z,
        (void*)((s32)cam + 0x11C),
        projection,
        viewport,
        &out->x,
        &out->y,
        &out->z
    );

    GXSetProjectionv(oldProjection);

    out->x = out->x - float_304_8041f6b8;
    out->y = float_240_8041f6bc - out->y;
}

void camLetterBoxDraw(void) {
    void* cam;
    volatile s16* fifo;

    f32 viewport[6];
    f32 projection[7];
    s16 topY;
    s16 bottomY;
    u32 scissorX;
    u32 scissorY;
    u32 scissorW;
    u32 scissorH;

    u32 matColor;
    u32 colorTemp;
    u32 fogColor;

    cam = camPtrTbl[8];

    if ((*(u32*)gp & 0x8000) != 0) {
        return;
    }

    if (*(s16*)((s32)cam + 0x1E0) == 0) {
        return;
    }

    GXGetViewportv(viewport);
    GXGetProjectionv(projection);
    GXGetScissor(&scissorX, &scissorY, &scissorW, &scissorH);

    GXSetProjection((void*)((s32)cam + 0x15C), *(u32*)((s32)cam + 0x19C));
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    GXSetNumChans(1);

    colorTemp = unk_80429510;
    *(u8*)((s32)&colorTemp + 3) = *(s16*)((s32)cam + 0x1E0);
    matColor = colorTemp;
    GXSetChanMatColor(4, &matColor);

    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);

    fogColor = dat_8041f6b0;
    GXSetFog(0, float_0_8041f6b4, float_0_8041f6b4, float_0_8041f6b4, float_0_8041f6b4, &fogColor);

    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevOp(0, 4);
    GXSetCullMode(0);

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);

    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXSetCurrentMtx(0);

    GXBegin(0x80, 0, 8);

    fifo = (volatile s16*)0xCC008000;

    *fifo = -0x130;
    *fifo = 0xF0;
    *fifo = 0;

    *fifo = 0x130;
    *fifo = 0xF0;
    *fifo = 0;

    topY = 0xF0 - *(s16*)((s32)cam + 0x1E2);
    *fifo = 0x130;
    *fifo = topY;
    *fifo = 0;

    topY = 0xF0 - *(s16*)((s32)cam + 0x1E2);
    *fifo = -0x130;
    *fifo = topY;
    *fifo = 0;

    bottomY = *(s16*)((s32)cam + 0x1E4) - 0xF0;
    *fifo = -0x130;
    *fifo = bottomY;
    *fifo = 0;

    bottomY = *(s16*)((s32)cam + 0x1E4) - 0xF0;
    *fifo = 0x130;
    *fifo = bottomY;
    *fifo = 0;

    *fifo = 0x130;
    *fifo = -0xF0;
    *fifo = 0;

    *fifo = -0x130;
    *fifo = -0xF0;
    *fifo = 0;

    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    GXSetProjectionv(projection);
    GXSetScissor(scissorX, scissorY, scissorW, scissorH);
}

void camLetterBox(void) {
    void* cam;
    void* party;
    s32 value;
    s16 alpha;
    u16 flags;

    cam = camPtrTbl[8];
    party = partyGetPtr(0);

    if ((*(u16*)cam & 0x200) != 0) {
        goto after_initial_check;
    }

    if ((bero_get_BeroEXEC() & 1) != 0) {
        goto after_initial_check;
    }

    if (seqGetSeq() == 2) {
        if (marioChkKey() != 0) {
            if (marioChkCtrl() != 0) {
                goto clear_letterbox;
            }
        }

        if (party != NULL) {
            if (party == NULL) {
                goto clear_letterbox;
            }

            if ((*(u32*)party & 0x100) != 0) {
                goto clear_letterbox;
            }
        }

        flags = *(u16*)cam;
        if ((flags & 0x100) == 0) {
            *(u16*)cam = flags | 0x100;
            psndSetFlag(0x20);

            *(s16*)((s32)cam + 0x1E0) = 0xFF;
            *(s16*)((s32)cam + 0x1E2) = 0;
            *(s16*)((s32)cam + 0x1E4) = 0;
            *(s32*)((s32)cam + 0x1E8) = 8;
        }

        goto after_initial_check;
    }

clear_letterbox:
    if (seqGetSeq() != 3) {
        *(u16*)cam &= ~0x100;
        psndClearFlag(0x20);
    }

after_initial_check:

    if ((*(u16*)cam & 0x100) != 0) {
        *(s16*)((s32)cam + 0x1E0) = 0xFF;

        value = *(s16*)((s32)cam + 0x1E2) + 3;
        *(s16*)((s32)cam + 0x1E2) = value;
        if (*(s16*)((s32)cam + 0x1E2) > 0x31) {
            *(s16*)((s32)cam + 0x1E2) = 0x31;
        }

        value = *(s16*)((s32)cam + 0x1E4) + 3;
        *(s16*)((s32)cam + 0x1E4) = value;
        if (*(s16*)((s32)cam + 0x1E4) > 0x2C) {
            *(s16*)((s32)cam + 0x1E4) = 0x2C;
        }

        flags = *(u16*)cam;
        if ((flags & 0x400) != 0) {
            *(u16*)cam = flags & ~0x400;
            *(s16*)((s32)cam + 0x1E2) = 0x31;
            *(s16*)((s32)cam + 0x1E4) = 0x2C;
        }
    } else {
        *(s16*)((s32)cam + 0x1E0) =
            *(s16*)((s32)cam + 0x1E0) -
            ((0xFF / *(s32*)((s32)gp + 0x4)) * 2);

        if (*(s16*)((s32)cam + 0x1E0) < 0) {
            *(s16*)((s32)cam + 0x1E0) = 0;
        }

        flags = *(u16*)cam;
        if ((flags & 0x400) != 0) {
            *(u16*)cam = flags & ~0x400;
            *(s16*)((s32)cam + 0x1E0) = 0;
        }
    }

    dispEntry(*(s32*)((s32)cam + 0x1E8), 3, camLetterBoxDraw, NULL, float_0_8041f6b4);
}

void* camPtrTbl[13];

VecRaw camPtDefault = { 0x00000000, 0x00000000, 0x447A0000 };
VecRaw camAtDefault = { 0x00000000, 0x00000000, 0x00000000 };
VecRaw camUpDefault = { 0x00000000, 0x3F800000, 0x00000000 };

s32 currentCamNo;

const u32 dat_8041f6b0 = 0xFFFFFFFF;
const f32 float_0_8041f6b4 = 0.0f;
const f32 float_304_8041f6b8 = 304.0f;
const f32 float_240_8041f6bc = 240.0f;
const f32 float_1_8041f6c0 = 1.0f;
const f32 float_0p01_8041f6c4 = 0.01f;
const f32 float_1E05_8041f6c8 = 100000.0f;
const f32 float_25_8041f6cc = 25.0f;
const f32 float_32768_8041f6d0 = 32768.0f;
const f32 float_100_8041f6d4 = 100.0f;
const f32 float_1p2667_8041f6d8 = 1.2666667f;
const f32 float_608_8041f6dc = 608.0f;
const f32 float_480_8041f6e0 = 480.0f;
const f32 dat_8041f6e4 = 1000000000.0f;
const f32 dat_8041f6e8 = -1000000000.0f;
const f32 float_neg1_8041f6ec = -1.0f;
const f32 float_deg2rad_8041f6f0 = 0.017453292f;
const f32 float_0p5_8041f6f4 = 0.5f;
const f32 float_0p2_8041f6f8 = 0.2f;
const f32 float_8_8041f6fc = 8.0f;
const f32 float_400_8041f700 = 400.0f;
const f32 float_neg240_8041f704 = -240.0f;
const f32 float_neg304_8041f708 = -304.0f;
const char str_off_8041f70c[] = "off";
const char str_off2_8041f710[] = "off2";
const char str_bg_8041f718[] = "bg";
const char str_3d_8041f71c[] = "3d";
const f32 float_10000_8041f720 = 10000.0f;
const f32 float_0p3_8041f724 = 0.3f;
const f32 float_1p3333_8041f728 = 1.3333334f;
const char str_3dimg_8041f72c[] = "3dimg";
const f32 float_24_8041f734 = 24.0f;
const f32 float_0p1_8041f738 = 0.1f;
const char str_2d_8041f73c[] = "2d";
const char str_fade_8041f740[] = "fade";
const char str_fade2_8041f748[] = "fade2";
const char str_dbg_8041f750[] = "dbg";
const char str_dbg3d_8041f754[] = "dbg3d";
