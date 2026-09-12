#include "cam_road.h"
#include "cam_shift.h"

#define COPY_CAMROAD_BLOCK(dstBase, srcBase)        \
    do {                                            \
        u32* copySrc = (u32*)((s32)(srcBase));      \
        u32 copyA;                                  \
        u32 copyB;                                  \
        u32* copyDst = (u32*)((s32)(dstBase));      \
        s32 copyI;                                  \
        for (copyI = 0; copyI < 0xC; copyI++) {     \
            copyA = copySrc[1];                     \
            copySrc += 2;                           \
            copyB = copySrc[0];                     \
            copyDst[1] = copyA;                     \
            copyDst += 2;                           \
            copyDst[0] = copyB;                     \
        }                                           \
    } while (0)

#define COPY_CAMROAD_BLOCK_O4P_TEST(dstBase, srcBase)       \
    do {                                                    \
        u32* copySrc = (u32*)((s32)(srcBase));              \
        u32 copyA;                                          \
        u32 copyB;                                          \
        u32* copyDst = (u32*)((s32)(dstBase));              \
        volatile s32 copyI;                                 \
        for (copyI = 0; copyI < 0xC; copyI++) {             \
            copyA = copySrc[1];                             \
            copySrc += 2;                                   \
            copyB = copySrc[0];                             \
            copyDst[1] = copyA;                             \
            copyDst += 2;                                   \
            copyDst[0] = copyB;                             \
        }                                                   \
    } while (0)

#define UPDATE_CAMROAD_CC()                                                                 \
    do {                                                                                    \
        if ((*(s32*)((s32)wp + 0xE0) != 0) ||                                                \
            (*(s32*)((s32)wp + 0x140) != 0) ||                                               \
            (((u32)*(s32*)((s32)wp + 0x1D0) == 0x65) &&                                      \
             (((*(void**)((s32)wp + 0xFC) != 0) &&                                          \
               (*(u32*)((s32)*(void**)((s32)wp + 0xFC) + 0x20) != 0)) ||                    \
              ((*(void**)((s32)wp + 0x15C) != 0) &&                                         \
               (*(u32*)((s32)*(void**)((s32)wp + 0x15C) + 0x20) != 0)))) ||                 \
            (((*(void**)((s32)wp + 0xFC) != 0) &&                                           \
              (*(f32*)((s32)*(void**)((s32)wp + 0xFC) + 0x7C) != float_0_8041f62c)) ||      \
             ((*(void**)((s32)wp + 0x15C) != 0) &&                                          \
              (*(f32*)((s32)*(void**)((s32)wp + 0x15C) + 0x7C) != float_0_8041f62c)))) {    \
            *(s32*)((s32)wp + 0xCC) = 1;                                                    \
        } else {                                                                            \
            *(s32*)((s32)wp + 0xCC) = 0;                                                    \
        }                                                                                   \
    } while (0)

    #define STORE_CAMROAD_TIME()                                      \
    do {                                                          \
        void* timeWp;                                             \
        s32 timeA;                                                \
        s32 timeB;                                                \
        timeWp = wp;                                              \
        timeA = *(s32*)((s32)gp + 0x40);                          \
        timeB = *(s32*)((s32)gp + 0x44);                          \
        *(s32*)((s32)timeWp + 0x1CC) = timeB;                     \
        *(s32*)((s32)timeWp + 0x1C8) = timeA;                     \
    } while (0)

    typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct CurveResult {
    f32 t;       // +0x00
    f32 unk04;   // +0x04
    f32 bestY;   // +0x08
    f32 x;       // +0x0C
    f32 y;       // +0x10
    f32 z;       // +0x14
    f32 nx;      // +0x18
    f32 ny;      // +0x1C
    f32 nz;      // +0x20
    s32 index;   // +0x24
    u32 flags;   // +0x28
} CurveResult;

const char vec3_802bf540[12] = {
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const char vec3_802bf54c[12] = {
    0x42, 0xB4, 0x00, 0x00, 0x42, 0xDC, 0x00, 0x00, 0x41, 0x50, 0x00, 0x00,
};
const char vec3_802bf558[12] = {
    0xC2, 0xDC, 0x00, 0x00, 0xC2, 0x8C, 0x00, 0x00, 0xC3, 0x3E, 0x00, 0x00,
};
const Vec3 vec3_802bf564 = {0.0f, -1.0f, 0.0f};
const f64 double_1_802bf570 = 1.0;
const f64 double_0p5_802bf578 = 0.5;
const f64 double_3_802bf580 = 3.0;
const f64 double_0_802bf588 = 0.0;
const f64 double_to_int_802bf590 = 4503601774854144.0;

extern s32 collisionTri_simple(void* a, void* b, void* c, void* d, void* e);
extern s32 collisionCurve(s32 type, void* road, void* pos, void* result);
extern f32 PSVECMag(void* vec);
extern void PSVECScale(void* out, void* in, f32 scale);
extern f32 getV60FPS(f32 value, s32 start, s32 end, u32 timeHi, u32 timeLo);
extern double cos(double);
extern double sin(double);
extern double __frsqrte(double);
const char* rodata;
extern void* wp;
extern void* gp;
extern const char vec3_802bf540[];
extern f32 __float_nan[];
extern void PSMTXRotAxisRad(f32 mtx[3][4], Vec3* axis, f32 rad);
extern void PSMTXMultVec(f32 mtx[3][4], Vec3* src, Vec3* dst);
extern const f32 dat_8041f67c;
extern const f32 dat_8041f688;
extern const f32 float_0_8041f62c;
extern const f32 float_1_8041f638;
extern const f32 float_0p5_8041f64c;
extern const f32 float_3p1416_8041f648;
extern const f32 float_180_8041f668;
extern const f32 float_neg1E06_8041f680;
extern const f32 float_neg1_8041f684;
extern const f32 float_0p01_8041f66c;
extern const f32 float_1E06_8041f694;
extern const f32 float_640_8041f68c;
extern const f32 float_480_8041f690;
extern s32 strcmp(const char* a, const char* b);
extern void* wp;
extern const f32 float_0p005_8041f628;
extern const f32 float_0_8041f62c;
extern const f32 float_0p004_8041f630;
extern const f32 float_0p008_8041f634;
extern void calcCamRoad(
    f32 x,
    f32 y,
    f32 z,
    f32 param4,
    f32 param5,
    f32 param6,
    f32 checkWidth,
    void* out,
    void* prev
);
extern const f32 float_1_8041f638;
extern const f32 float_2_8041f63c;
extern const f32 float_3_8041f640;
extern const f32 float_10_8041f644;
extern void* compareCamRoad(void* a, void* b, void* out);
extern void calcHokanCamRoad(s32 mode, f32 t, void* fromA, void* fromB, void* fromC, void* out);
extern void calcCurrentCamState(void* cam);
extern f64 cos(f64);
extern f64 sin(f64);
extern f64 atan2(f64, f64);
extern const f32 float_3p1416_8041f648;
extern const f32 float_0p5_8041f64c;
extern const f32 float_0p5_8041f650;
extern const f32 float_6p2832_8041f654;
extern const f32 float_3p1416_8041f658;
extern const f32 float_neg3p1416_8041f65c;
extern double acos(double);
extern double __frsqrte(double);
extern f32 __fabsf(f32);
extern const f32 float_100_8041f698;
extern void* wp;
extern const char vec3_802bf540[];
extern f32 __float_nan[];
extern const f32 float_deg2rad_8041f69c;
extern const f32 float_0_8041f62c;
extern const f32 float_1_8041f638;
extern const f32 float_6p2832_8041f654;
extern const f32 float_rad2deg_8041f660;
extern const f32 float_360_8041f664;
extern const f32 float_180_8041f668;
extern const f32 float_0p01_8041f66c;
extern const f32 float_0p3_8041f670;
extern const f32 float_90_8041f674;
extern const f32 float_20_8041f678;
extern void* wp;
extern void* mapalloc_base_ptr;
extern void arcOpen(const char* name, void* dst, void* outSize);
extern const char* getMapDataDvdRoot(void);
extern int sprintf(char* str, const char* format, ...);
extern void* DVDMgrOpen(const char* path, s32 mode, s32 unk);
extern s32 DVDMgrGetLength(void* handle);
extern void* _mapAlloc(void* heap, u32 size);
extern void DVDMgrRead(void* handle, void* dst, u32 size, s32 offset);
extern void DVDMgrClose(void* handle);
extern s32 strcmp(const char* a, const char* b);

extern const f32 float_0p005_8041f628;
extern const f32 float_0p004_8041f630;
extern const f32 float_0p008_8041f634;
extern const f32 float_2_8041f63c;
extern const f32 float_3_8041f640;

extern const char str_PCTs_PCTs_8041f6a0[];
extern const char str_1_01_8041f6a8[];

s32 camRoadSetup(const char* name) {
    void* file;
    void* data;
    s32 length;
    char path[0x80];

    memset(wp, 0, 0x210);

    *(s32*)((s32)wp + 0xC8) = 0;
    *(s32*)((s32)wp + 0xD8) = 0;
    *(s32*)((s32)wp + 0xD8) = 0;

    *(f32*)((s32)wp + 0x19C) = float_0p005_8041f628;
    *(f32*)((s32)wp + 0x1A0) = float_0_8041f62c;
    *(f32*)((s32)wp + 0x1A4) = float_0p004_8041f630;
    *(f32*)((s32)wp + 0x1A8) = float_0p008_8041f634;
    *(f32*)((s32)wp + 0x1AC) = float_1_8041f638;
    *(f32*)((s32)wp + 0x1B0) = float_1_8041f638;
    *(f32*)((s32)wp + 0x1B4) = float_2_8041f63c;
    *(f32*)((s32)wp + 0x1B8) = float_1_8041f638;
    *(f32*)((s32)wp + 0x1BC) = float_1_8041f638;
    *(f32*)((s32)wp + 0x1C0) = float_3_8041f640;
    *(f32*)((s32)wp + 0x1C4) = float_1_8041f638;

    *(s32*)((s32)wp + 0x204) = -1;
    *(s32*)((s32)wp + 0x208) = 0;

    camShiftInit();

    arcOpen(name, wp, &length);

    if (*(void**)wp == NULL) {
        sprintf(path, str_PCTs_PCTs_8041f6a0, getMapDataDvdRoot(), name);

        file = DVDMgrOpen(path, 2, 0);
    if (file != NULL) {
    length = DVDMgrGetLength(file);
    *(void**)wp = _mapAlloc(mapalloc_base_ptr, (length + 0x1F) & ~0x1F);

    DVDMgrRead(file, *(void**)wp, (length + 0x1F) & ~0x1F, 0);
    DVDMgrClose(file);
    } else {
    return 0;
    }
    }

    data = *(void**)wp;

    if (*(u32*)((s32)*(void**)wp + 0xE8) < (u32)*(void**)wp) {
    *(void**)((s32)*(void**)wp + 0xE8) =
        (void*)(*(s32*)((s32)*(void**)wp + 0xE8) + (s32)*(void**)wp);

    *(void**)((s32)*(void**)wp + 0xEC) =
        (void*)(*(s32*)((s32)*(void**)wp + 0xEC) + (s32)*(void**)wp);

    *(void**)((s32)*(void**)wp + 0xF0) =
        (void*)(*(s32*)((s32)*(void**)wp + 0xF0) + (s32)*(void**)wp);

    *(void**)((s32)*(void**)wp + 0xF4) =
        (void*)(*(s32*)((s32)*(void**)wp + 0xF4) + (s32)*(void**)wp);

    *(void**)((s32)*(void**)wp + 0xF8) =
        (void*)(*(s32*)((s32)*(void**)wp + 0xF8) + (s32)*(void**)wp);

    *(void**)((s32)*(void**)wp + 0xFC) =
        (void*)(*(s32*)((s32)*(void**)wp + 0xFC) + (s32)*(void**)wp);

    *(void**)((s32)*(void**)wp + 0x100) =
        (void*)(*(s32*)((s32)*(void**)wp + 0x100) + (s32)*(void**)wp);

    *(void**)((s32)*(void**)wp + 0x104) =
        (void*)(*(s32*)((s32)*(void**)wp + 0x104) + (s32)*(void**)wp);

    *(void**)((s32)*(void**)wp + 0x108) =
        (void*)(*(s32*)((s32)*(void**)wp + 0x108) + (s32)*(void**)wp);
    }

    if (strcmp((char*)((s32)*(void**)wp + 0x44), str_1_01_8041f6a8) == 0) {
        *(s32*)((s32)wp + 0x1D0) = 0x65;
    } else {
        *(s32*)((s32)wp + 0x1D0) = 0x64;
    }

    return 1;
}

void calcCurrentCamState(void* out) {
    void* work0;
    void* baseVec;
    f32 mtx[3][4];
    f32 rot;
    f32 x;
    f32 y;
    f32 z;
    f32 dirZ;
    f32 dirX;
    f32 scale;
    f32 angle;
    f32 cosValue;
    f32 sinValue;
    f32 xOffset;
    f32 yOffset;
    f32 zOffset;
    Vec3 src;
    Vec3 dst;
    Vec3 axis;
    work0 = *(void**)wp;
    baseVec = *(void**)((s32)work0 + 0xE8);
    work0 = *(void**)wp;
    baseVec = *(void**)((s32)work0 + 0xE8);

    *(f32*)((s32)out + 0x00) = *(f32*)((s32)baseVec + 0x00);
    *(f32*)((s32)out + 0x04) = *(f32*)((s32)baseVec + 0x04);
    *(f32*)((s32)out + 0x08) = *(f32*)((s32)baseVec + 0x08);

    if (*(s32*)((s32)wp + 0x08) == 0) {
        *(f32*)((s32)out + 0x0C) = float_0_8041f62c;
        *(f32*)((s32)out + 0x10) = float_100_8041f698;
        *(f32*)((s32)out + 0x14) = float_100_8041f698;
        *(f32*)((s32)out + 0x18) = float_0_8041f62c;
        *(f32*)((s32)out + 0x1C) = float_0_8041f62c;
        *(f32*)((s32)out + 0x20) = float_0_8041f62c;
        return;
    }

    angle = (float_3p1416_8041f648 * *(f32*)((s32)wp + 0x4C)) / float_180_8041f668;

    x = *(f32*)((s32)wp + 0x30);
    y = *(f32*)((s32)wp + 0x34);
    z = *(f32*)((s32)wp + 0x38);
    dirZ = *(f32*)((s32)wp + 0x3C);
    dirX = *(f32*)((s32)wp + 0x44);
    scale = *(f32*)((s32)wp + 0x48);
    rot = *(f32*)((s32)wp + 0x50);

    cosValue = cos(angle);
    xOffset = (dirZ * scale) * cosValue;

    sinValue = sin(angle);
    yOffset = scale * sinValue;

    cosValue = cos(angle);
    zOffset = (dirX * scale) * cosValue;

    *(f32*)((s32)out + 0x0C) = x + xOffset;
    *(f32*)((s32)out + 0x10) = y + yOffset;
    *(f32*)((s32)out + 0x14) = z + zOffset;

    axis.x = -dirX;
    axis.y = float_0_8041f62c;
    axis.z = dirZ;

    PSMTXRotAxisRad(mtx, &axis, float_deg2rad_8041f69c * rot);

    src.x = -xOffset;
    src.y = -yOffset;
    src.z = -zOffset;

    PSMTXMultVec(mtx, &src, &dst);

    *(f32*)((s32)out + 0x18) = *(f32*)((s32)out + 0x0C) + dst.x;
    *(f32*)((s32)out + 0x1C) = *(f32*)((s32)out + 0x10) + dst.y;
    *(f32*)((s32)out + 0x20) = *(f32*)((s32)out + 0x14) + dst.z;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on

s32 collisionTri_simple(void* a, void* b, void* c, void* d, void* e) {
    extern const Vec3 vec3_802bf564;
    extern const f32 float_0_8041f62c;
    extern const f64 double_0_802bf588;

    Vec3* pointPtr = (Vec3*)a;
    Vec3* v0Ptr = (Vec3*)b;
    Vec3* v1Ptr = (Vec3*)c;
    Vec3* v2Ptr = (Vec3*)d;
    f32* outY = (f32*)e;

    Vec3 v0;
    Vec3 v1;
    Vec3 v2;
    Vec3 e01;
    Vec3 e20;
    Vec3 e12;
    Vec3 normal;
    Vec3 direction;
    Vec3 point;
    Vec3 hit;
    f32 dist;
    f32 denom;
    f32 side;
    f32 oldY;
    f32 hitY;
    f32 scale;

    /*
     * A24r keeps A24's aggregate/inline-helper hypothesis, but expresses the
     * helper bodies as function-local macros so chatgpt_try's function-only
     * replacement keeps them in cam_road.c.
     *
     * Each subtraction/cross macro deliberately creates two input Vec copies
     * plus one Vec result, reproducing the 3-Vec aggregate pressure expected
     * from an inlined by-value helper.
     */
#define CT_SUB3(dst_, lhs_, rhs_)                   \
    do {                                             \
        Vec3 ct_lhs = (lhs_);                        \
        Vec3 ct_rhs = (rhs_);                        \
        Vec3 ct_out;                                 \
        ct_out.x = ct_lhs.x - ct_rhs.x;             \
        ct_out.y = ct_lhs.y - ct_rhs.y;             \
        ct_out.z = ct_lhs.z - ct_rhs.z;             \
        (dst_) = ct_out;                             \
    } while (0)

#define CT_CROSS3(dst_, lhs_, rhs_)                 \
    do {                                             \
        Vec3 ct_lhs = (lhs_);                        \
        Vec3 ct_rhs = (rhs_);                        \
        Vec3 ct_out;                                 \
        ct_out.x = (ct_lhs.y * ct_rhs.z) -          \
                   (ct_lhs.z * ct_rhs.y);            \
        ct_out.z = (ct_lhs.x * ct_rhs.y) -          \
                   (ct_lhs.y * ct_rhs.x);            \
        ct_out.y = (ct_lhs.z * ct_rhs.x) -          \
                   (ct_lhs.x * ct_rhs.z);            \
        (dst_) = ct_out;                             \
    } while (0)

#define CT_DOT2(dst_, lhs_, rhs_)                   \
    do {                                             \
        Vec3 ct_lhs = (lhs_);                        \
        Vec3 ct_rhs = (rhs_);                        \
        (dst_) = (ct_lhs.z * ct_rhs.z) +            \
                 (ct_lhs.x * ct_rhs.x) +            \
                 (ct_lhs.y * ct_rhs.y);              \
    } while (0)

    /*
     * A24r2 composes the already-successful by-value subtract/cross shapes
     * inside each side test instead of spelling their arithmetic directly.
     *
     * In A24r the compiler materialized only the four input Vecs for each
     * side block and kept diff/cross in registers, producing 0x30-byte
     * per-test stack regions.  The target has 0x48-byte per-test regions.
     * CT_SUB3 + CT_CROSS3 should force the missing result-Vec
     * materialization/copy topology while preserving the arithmetic that
     * raised the score to 60.390278%.
     */
#define CT_SIDE6(dst_, point_, vertex_, edge_, direction_) \
    do {                                                    \
        Vec3 ct_diff;                                       \
        Vec3 ct_cross;                                      \
        CT_SUB3(ct_diff, (point_), (vertex_));              \
        CT_CROSS3(ct_cross, (edge_), (direction_));         \
        (dst_) = (ct_diff.z * ct_cross.z) +                 \
                 (ct_diff.x * ct_cross.x) +                 \
                 (ct_diff.y * ct_cross.y);                  \
    } while (0)

#define CT_FINAL5(dst_, point_, direction_, scale_)         \
    do {                                                     \
        Vec3 ct_point = (point_);                            \
        Vec3 ct_direction = (direction_);                    \
        Vec3 ct_scaled;                                      \
        Vec3 ct_scaled_copy;                                 \
        Vec3 ct_out;                                         \
        ct_scaled.x = ct_direction.x * (scale_);             \
        ct_scaled.y = ct_direction.y * (scale_);             \
        ct_scaled.z = ct_direction.z * (scale_);             \
        ct_scaled_copy = ct_scaled;                          \
        ct_out.x = ct_point.x + ct_scaled_copy.x;            \
        ct_out.y = ct_point.y + ct_scaled_copy.y;            \
        ct_out.z = ct_point.z + ct_scaled_copy.z;            \
        (dst_) = ct_out;                                     \
    } while (0)

    /*
     * The caller passes four actual Vec3 objects.  Preserve that abstraction
     * directly instead of loading their components into u32 scalars.
     */
    v1 = *v1Ptr;
    v0 = *v0Ptr;
    v2 = *v2Ptr;
    direction = vec3_802bf564;

    CT_SUB3(e01, v0, v1);
    CT_SUB3(e20, v2, v0);
    CT_SUB3(e12, v1, v2);
    CT_CROSS3(normal, e20, e01);

    if ((float_0_8041f62c == normal.x) &&
        (float_0_8041f62c == normal.y) &&
        (float_0_8041f62c == normal.z)) {
        return 0;
    }

    /* Target does not need the query point until after the zero-normal exit. */
    point = *pointPtr;

    {
        Vec3 ct_point = point;
        Vec3 ct_vertex = v0;
        Vec3 ct_diff;
        Vec3 ct_normal = normal;

        ct_diff.x = ct_point.x - ct_vertex.x;
        ct_diff.y = ct_point.y - ct_vertex.y;
        ct_diff.z = ct_point.z - ct_vertex.z;
        dist = (ct_normal.z * ct_diff.z) +
               (ct_normal.x * ct_diff.x) +
               (ct_normal.y * ct_diff.y);
    }

    if (dist <= float_0_8041f62c) {
        CT_DOT2(denom, normal, direction);
        if ((f64)denom <= double_0_802bf588) {
            return 0;
        }

        CT_SIDE6(side, point, v0, e20, direction);
        if ((f64)side < double_0_802bf588) {
            return 0;
        }

        CT_SIDE6(side, point, v1, e01, direction);
        if ((f64)side < double_0_802bf588) {
            return 0;
        }

        CT_SIDE6(side, point, v2, e12, direction);
        if ((f64)side < double_0_802bf588) {
            return 0;
        }
    } else {
        CT_DOT2(denom, normal, direction);
        if (double_0_802bf588 <= (f64)denom) {
            return 0;
        }

        CT_SIDE6(side, point, v0, e20, direction);
        if (double_0_802bf588 < (f64)side) {
            return 0;
        }

        CT_SIDE6(side, point, v1, e01, direction);
        if (double_0_802bf588 < (f64)side) {
            return 0;
        }

        CT_SIDE6(side, point, v2, e12, direction);
        if (double_0_802bf588 < (f64)side) {
            return 0;
        }
    }

    CT_DOT2(denom, normal, direction);
    scale = -dist / denom;
    CT_FINAL5(hit, point, direction, scale);

    oldY = *outY;
    hitY = hit.y;
    if (oldY <= hitY) {
        *outY = hitY;
    }

#undef CT_FINAL5
#undef CT_SIDE6
#undef CT_DOT2
#undef CT_CROSS3
#undef CT_SUB3

    return oldY <= hitY;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

extern s32 collisionCurve(s32 type, void* road, void* pos, void* result) {
    const char* curveRodata = vec3_802bf540;
    f32* point = (f32*)pos;
    CurveResult* out = (CurveResult*)result;
    f32* vertices;
    f32* normals;
    f32* startNormal;
    f32* startVertex;
    f32* endNormal;
    f32* endVertex;
    s32 count;
    s32 found = 0;
    s32 i;
    s32 start;
    s32 end;
    s32 roadMode;
    s32 typeFlag;
    f32 limitA;
    f32 limitB;
    volatile f32 yBounds[2];
    f32 best = dat_8041f688;

#define endY   yBounds[0]
#define startY yBounds[1]

#define CC_SQRT(dst_, sq_)                                                        \
    do {                                                                          \
        f32 cc_sq = (sq_);                                                        \
        f32 cc_result;                                                            \
        if (*(f64*)(curveRodata + 0x48) != (f64)cc_sq) {                         \
            if (cc_sq > float_0_8041f62c) {                                       \
                f64 cc_value = (f64)cc_sq;                                        \
                f64 cc_inv = __frsqrte(cc_value);                                 \
                f64 cc_half = *(f64*)(curveRodata + 0x38);                        \
                f64 cc_three = *(f64*)(curveRodata + 0x40);                       \
                f64 cc_square = cc_inv * cc_inv;                                  \
                cc_inv = (cc_half * cc_inv) * (cc_three - cc_value * cc_square);  \
                cc_square = cc_inv * cc_inv;                                      \
                cc_inv = (cc_half * cc_inv) * (cc_three - cc_value * cc_square);  \
                cc_square = cc_inv * cc_inv;                                      \
                cc_inv = (cc_half * cc_inv) * (cc_three - cc_value * cc_square);  \
                cc_result = (f32)(cc_value * cc_inv);                             \
            } else if ((f64)cc_sq < *(f64*)(curveRodata + 0x48)) {               \
                cc_result = __float_nan[0];                                       \
            } else {                                                              \
                f32 cc_classify = cc_sq;                                          \
                u32 cc_bits = *(u32*)&cc_classify;                                \
                s32 cc_exp = cc_bits & 0x7F800000;                                \
                s32 cc_type;                                                      \
                switch (cc_exp) {                                                 \
                    case 0x7F800000:                                              \
                        if ((cc_bits & 0x007FFFFF) != 0) {                        \
                            cc_type = 1;                                           \
                        } else {                                                  \
                            cc_type = 2;                                           \
                        }                                                         \
                        break;                                                     \
                    case 0:                                                       \
                        if ((cc_bits & 0x007FFFFF) != 0) {                        \
                            cc_type = 5;                                           \
                        } else {                                                  \
                            cc_type = 3;                                           \
                        }                                                         \
                        break;                                                     \
                    default:                                                      \
                        cc_type = 4;                                               \
                        break;                                                     \
                }                                                                 \
                if (cc_type == 1) cc_result = __float_nan[0];                    \
                else cc_result = cc_sq;                                           \
            }                                                                     \
        } else {                                                                  \
            cc_result = float_0_8041f62c;                                         \
        }                                                                         \
        (dst_) = cc_result;                                                       \
    } while (0)

    if (type == 0) {
        s32 rawCount = *(s32*)((s32)road + 0xA4);
        s32 baseIndex = *(s32*)((s32)road + 0xA0);
        f32* geomBase = (f32*)(*(s32*)(*(s32*)wp + 0xF8));

        count = (rawCount + (s32)((u32)rawCount >> 31)) >> 1;
        typeFlag = 0;
        start = *(s32*)((s32)road + 0x40);
        end = *(s32*)((s32)road + 0x44);
        limitB = *(f32*)((s32)road + 0x48);
        limitA = *(f32*)((s32)road + 0x4C);
        roadMode = *(s32*)((s32)road + 0x84);
        vertices = geomBase + baseIndex * 3;
        normals = geomBase + (baseIndex + count) * 3;
    } else {
        s32 rawCount = *(s32*)((s32)road + 0x64);
        s32 baseIndex = *(s32*)((s32)road + 0x60);
        f32* geomBase = (f32*)(*(s32*)(*(s32*)wp + 0xF8));

        count = (rawCount + (s32)((u32)rawCount >> 31)) >> 1;
        start = 0;
        end = 1;
        limitB = float_0_8041f62c;
        limitA = limitB;
        roadMode = 0;
        typeFlag = 1;
        vertices = geomBase + baseIndex * 3;
        normals = geomBase + (baseIndex + count) * 3;
    }

    endY = normals[end * 3 + 1];
    startY = normals[start * 3 + 1];
    if (__fabsf(endY - startY) < *(f64*)(curveRodata + 0x58)) {
        startY = endY;
    }
    if (typeFlag) {
        roadMode = 0;
    }

    startNormal = normals + start * 3;
    startVertex = vertices + start * 3;
    endNormal = normals + end * 3;
    endVertex = vertices + end * 3;

    for (i = 0; i < count - 1; i++, vertices += 3, normals += 3) {
        f32 v0x = vertices[0];
        f32 v0y = vertices[1];
        f32 v0z = vertices[2];
        f32 v1x = vertices[3];
        f32 v1z = vertices[5];
        f32 n0x = normals[0];
        f32 n0z = normals[2];
        f32 n1x = normals[3];
        f32 n1z = normals[5];
        f32 side0;
        f32 side1;
        u32 flags = 0;

        side0 = n0x * (point[0] - v0x) + n0z * (point[2] - v0z);
        side1 = (-n1x) * (point[0] - v1x) + (-n1z) * (point[2] - v1z);
        if (float_0_8041f62c <= side0 && float_0_8041f62c <= side1) {
            f32 t0x = -n0z;
            f32 t0z = n0x;
            f32 t1x = -n1z;
            f32 t1z = n1x;

            if ((__fabsf(t0x - t1x) < *(f64*)(curveRodata + 0x60)) &&
                (__fabsf(t0z - t1z) < *(f64*)(curveRodata + 0x60))) {
                t0x = float_0p5_8041f64c * (t0x + t1x);
                t0z = float_0p5_8041f64c * (t0z + t1z);
                t1x = t0x;
                t1z = t0z;
            }

            if ((t0x != t1x) || (t0z != t1z)) {
                f32 centerStep;
                f32 centerX;
                f32 centerZ;
                f32 aX;
                f32 aZ;
                f32 bX;
                f32 bZ;
                f32 pX;
                f32 pZ;
                f32 distanceSign = float_1_8041f638;
                f32 radiusASq;
                f32 pointRadiusSq;
                f32 radiusBSq;
                f32 radiusA;
                f32 pointRadius;
                f32 radiusB;
                f32 cosA;
                f32 cosB;
                f32 angleA;
                f32 angleB;
                f32 frac;
                f32 distance;
                f32 hitY;
                f32 orientation;
                volatile f32 normalSign = float_1_8041f638;

                if (*(f64*)(curveRodata + 0x48) == (f64)t1x) {
                    if (*(f64*)(curveRodata + 0x48) == (f64)t1z) {
                        centerStep = float_0_8041f62c;
                    } else {
                        centerStep = ((t1x * ((v0z - v1z) / t1z) + v1x) - v0x) /
                                     -(t1x * (t0z / t1z) - t0x);
                    }
                } else {
                    centerStep = ((t1z * ((v0x - v1x) / t1x) + v1z) - v0z) /
                                 -(t1z * (t0x / t1x) - t0z);
                }

                centerZ = centerStep * t0z + v0z;
                centerX = centerStep * t0x + v0x;
                aZ = v0z - centerZ;
                aX = v0x - centerX;
                radiusASq = aX * aX + aZ * aZ;
                CC_SQRT(radiusA, radiusASq);

                pZ = point[2] - centerZ;
                pX = point[0] - centerX;
                pointRadiusSq = pX * pX + pZ * pZ;
                CC_SQRT(pointRadius, pointRadiusSq);

                bZ = v1z - centerZ;
                bX = v1x - centerX;
                radiusBSq = bX * bX + bZ * bZ;
                CC_SQRT(radiusB, radiusBSq);

                cosA = (aX * pX + aZ * pZ) / (radiusA * pointRadius);
                cosB = (bX * pX + bZ * pZ) / (radiusB * pointRadius);
                if (cosA < float_neg1_8041f684) cosA = float_neg1_8041f684;
                else if (float_1_8041f638 < cosA) cosA = float_1_8041f638;
                if (cosB < float_neg1_8041f684) cosB = float_neg1_8041f684;
                else if (float_1_8041f638 < cosB) cosB = float_1_8041f638;

                angleA = (f32)acos((f64)cosA);
                angleB = (f32)acos((f64)cosB);
                frac = angleA / (angleA + angleB);
                distance = frac * ((pointRadius - radiusB) - (pointRadius - radiusA)) +
                           (pointRadius - radiusA);
                if (distance < float_0_8041f62c) {
                    distance = -distance;
                    distanceSign = float_neg1_8041f684;
                }

                if (distance < best) {
                    best = distance;
                    hitY = frac * (normals[4] - normals[1]) + normals[1];
                    orientation = aX * -(v1z - v0z) + aZ * (v1x - v0x);

                    if (roadMode != 0) {
                        if (hitY < startY) {
                            if ((f64)orientation <= *(f64*)(curveRodata + 0x48)) {
                                if (float_0_8041f62c <= distanceSign) {
                                    if (limitB < distance) {
                                        flags |= 2;
                                        distance = limitB;
                                    }
                                } else {
                                    if (limitA < distance) {
                                        flags |= 2;
                                        distance = limitA;
                                    }
                                    distance = -distance;
                                }
                            } else if (float_0_8041f62c <= distanceSign) {
                                if (limitA < distance) {
                                    flags |= 2;
                                    distance = limitA;
                                }
                                distance = -distance;
                            } else if (limitB < distance) {
                                flags |= 2;
                                distance = limitB;
                            }
                            found = 1;
                            out->t = startY;
                            out->x = distance * startNormal[2] + startVertex[0];
                            out->y = float_0_8041f62c;
                            out->z = -(distance * startNormal[0] - startVertex[2]);
                            out->nx = startNormal[0];
                            out->ny = float_0_8041f62c;
                            out->nz = startNormal[2];
                            out->bestY = v0y;
                            out->index = start;
                            out->flags = flags | 1;
                            continue;
                        }

                        if (endY < hitY) {
                            if ((f64)orientation <= *(f64*)(curveRodata + 0x48)) {
                                if (float_0_8041f62c <= distanceSign) {
                                    if (limitB < distance) {
                                        flags |= 2;
                                        distance = limitB;
                                    }
                                } else {
                                    if (limitA < distance) {
                                        flags |= 2;
                                        distance = limitA;
                                    }
                                    distance = -distance;
                                }
                            } else if (float_0_8041f62c <= distanceSign) {
                                if (limitA < distance) {
                                    flags |= 2;
                                    distance = limitA;
                                }
                                distance = -distance;
                            } else if (limitB < distance) {
                                flags |= 2;
                                distance = limitB;
                            }
                            found = 1;
                            out->t = endY;
                            out->x = distance * endNormal[2] + endVertex[0];
                            out->y = float_0_8041f62c;
                            out->z = -(distance * endNormal[0] - endVertex[2]);
                            out->nx = endNormal[0];
                            out->ny = float_0_8041f62c;
                            out->nz = endNormal[2];
                            out->bestY = v0y;
                            out->index = end;
                            out->flags = flags | 1;
                            continue;
                        }

                        if ((f64)orientation <= *(f64*)(curveRodata + 0x48)) {
                            if (float_0_8041f62c <= distanceSign) {
                                if (limitB < distance) {
                                    pX = pX / pointRadius;
                                    flags |= 2;
                                    pZ = pZ / pointRadius;
                                    pointRadius = pointRadius - (distance - limitB);
                                    pX = pX * pointRadius;
                                    pZ = pZ * pointRadius;
                                }
                            } else if (limitA < distance) {
                                pX = pX / pointRadius;
                                flags |= 2;
                                pZ = pZ / pointRadius;
                                pointRadius = pointRadius + (distance - limitA);
                                pX = pX * pointRadius;
                                pZ = pZ * pointRadius;
                            }
                            normalSign = float_neg1_8041f684;
                        } else if (float_0_8041f62c <= distanceSign) {
                            if (limitA < distance) {
                                pX = pX / pointRadius;
                                flags |= 2;
                                pZ = pZ / pointRadius;
                                pointRadius = pointRadius - (distance - limitA);
                                pX = pX * pointRadius;
                                pZ = pZ * pointRadius;
                            }
                        } else if (limitB < distance) {
                            pX = pX / pointRadius;
                            flags |= 2;
                            pZ = pZ / pointRadius;
                            pointRadius = pointRadius + (distance - limitB);
                            pX = pX * pointRadius;
                            pZ = pZ * pointRadius;
                        }
                    } else {
                        if (typeFlag) {
                            if ((f64)orientation <= *(f64*)(curveRodata + 0x48)) {
                                if ((distanceSign < float_0_8041f62c) &&
                                    (float_0_8041f62c < distance)) {
                                    pX = pX / pointRadius;
                                    flags = 3;
                                    pZ = pZ / pointRadius;
                                    pointRadius = pointRadius + distance;
                                    pX = pX * pointRadius;
                                    pZ = pZ * pointRadius;
                                }
                                normalSign = float_neg1_8041f684;
                            } else if ((float_0_8041f62c <= distanceSign) &&
                                       (float_0_8041f62c < distance)) {
                                pX = pX / pointRadius;
                                flags = 3;
                                pZ = pZ / pointRadius;
                                pointRadius = pointRadius - distance;
                                pX = pX * pointRadius;
                                pZ = pZ * pointRadius;
                            }
                        } else if ((f64)orientation <= *(f64*)(curveRodata + 0x48)) {
                            normalSign = float_neg1_8041f684;
                        }
                    }

                    out->x = centerX + pX;
                    out->y = float_0_8041f62c;
                    out->z = centerZ + pZ;
                    if (float_0_8041f62c <= normalSign) {
                        out->nx = pZ / pointRadius;
                        out->ny = float_0_8041f62c;
                        out->nz = -pX / pointRadius;
                    } else {
                        out->nx = -pZ / pointRadius;
                        out->ny = float_0_8041f62c;
                        out->nz = pX / pointRadius;
                    }
                    out->t = hitY;
                    found = 1;
                    out->bestY = v0y;
                    out->index = i;
                    out->flags = flags;
                }
                        } else {
                f32 edgeZ = v1z - v0z;
                f32 edgeX = v1x - v0x;
                f32 negEdgeZ = -edgeZ;
                f32 t;
                f32 hitX;
                f32 hitZ;
                f32 interp = float_0_8041f62c;
                f32 offX;
                f32 offZ;
                f32 distanceSq;
                f32 distance;
                f32 hitY;
                f32 dotSide;

                if (__fabsf(negEdgeZ) <= __fabsf(edgeX)) {
                    if (*(f64*)(curveRodata + 0x48) == (f64)edgeX) {
                        t = float_0_8041f62c;
                    } else {
                        t = ((negEdgeZ * ((v0z - point[2]) / edgeX) + point[0]) - v0x) /
                            -(negEdgeZ * (edgeZ / edgeX) - edgeX);
                    }
                } else {
                    t = ((edgeX * ((v0x - point[0]) / negEdgeZ) + point[2]) - v0z) /
                        -(edgeX * (edgeX / negEdgeZ) - edgeZ);
                }

                hitX = v0x + t * edgeX;
                hitZ = v0z + t * edgeZ;
                if (__fabsf(edgeX) <= __fabsf(edgeZ)) {
                    if (*(f64*)(curveRodata + 0x48) != (f64)edgeZ) {
                        interp = (hitZ - v0z) / edgeZ;
                    }
                } else {
                    interp = (hitX - v0x) / edgeX;
                }

                offX = point[0] - hitX;
                offZ = point[2] - hitZ;
                distanceSq = offX * offX + offZ * offZ;
                CC_SQRT(distance, distanceSq);
                hitY = normals[1] + interp * (normals[4] - normals[1]);

                if (distance < best) {
                    best = distance;
                    dotSide = offX * negEdgeZ + offZ * edgeX;
                    if (roadMode != 0) {
                        if (hitY < startY) {
                            if (float_0_8041f62c <= dotSide) {
                                if (limitA < distance) {
                                    flags |= 2;
                                    distance = limitA;
                                }
                                distance = -distance;
                            } else if (limitB < distance) {
                                flags |= 2;
                                distance = limitB;
                            }
                            found = 1;
                            out->t = startY;
                            out->x = distance * startNormal[2] + startVertex[0];
                            out->y = float_0_8041f62c;
                            out->z = -(distance * startNormal[0] - startVertex[2]);
                            out->nx = startNormal[0];
                            out->ny = float_0_8041f62c;
                            out->nz = startNormal[2];
                            out->bestY = v0y;
                            out->index = start;
                            out->flags = flags | 1;
                            continue;
                        }

                        if (endY < hitY) {
                            if (float_0_8041f62c <= dotSide) {
                                if (limitA < distance) {
                                    flags |= 2;
                                    distance = limitA;
                                }
                                distance = -distance;
                            } else if (limitB < distance) {
                                flags |= 2;
                                distance = limitB;
                            }
                            found = 1;
                            out->t = endY;
                            out->x = distance * endNormal[2] + endVertex[0];
                            out->y = float_0_8041f62c;
                            out->z = -(distance * endNormal[0] - endVertex[2]);
                            out->nx = endNormal[0];
                            out->ny = float_0_8041f62c;
                            out->nz = endNormal[2];
                            out->bestY = v0y;
                            out->index = end;
                            out->flags = flags | 1;
                            continue;
                        }

                        if (float_0_8041f62c <= dotSide) {
                            if ((f64)limitA < (f64)distance) {
                                flags |= 2;
                                offX = limitA * (offX / distance);
                                offZ = limitA * (offZ / distance);
                            }
                        } else if ((f64)limitB < (f64)distance) {
                            flags |= 2;
                            offX = limitB * (offX / distance);
                            offZ = limitB * (offZ / distance);
                        }
                    }

                    out->t = hitY;
                    found = 1;
                    out->x = hitX + offX;
                    out->y = float_0_8041f62c;
                    out->z = hitZ + offZ;
                    out->nx = normals[0] + interp * (normals[3] - normals[0]);
                    out->ny = float_0_8041f62c;
                    out->nz = normals[2] + interp * (normals[5] - normals[2]);
                    out->bestY = v0y;
                    out->index = i;
                    out->flags = flags;
                }
                        }
        }
    }

#undef CC_SQRT
#undef startY
#undef endY

    return found;
}

void calcCamRoad(f32 x, f32 y, f32 z, f32 param4, f32 param5, f32 param6, f32 checkWidth, void* out, void* prev) {
    const char* rodata;
    void* work0;
    void* road;
    void* foundRoad;
    void* roadEntry;
    void* chosenRoad;
    f32 c2;
    f32 s2;
    Vec3 pos;
    Vec3 triPos;
    Vec3 p0;
    Vec3 p1;
    Vec3 p2;
    Vec3 p3;
    Vec3 sidePosA;
    Vec3 sidePosB;
    Vec3 sideVecA;
    Vec3 sideVecB;
    Vec3 magVecA;
    Vec3 magVecB;
    CurveResult hit;
    CurveResult sideHitA;
    CurveResult sideHitB;
    f32 bestY;
    f32 outX;
    f32 outY;
    f32 outZ;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 roadScale;
    f32 saved44;
    f32 saved48;
    f32 saved4C;
    f32 curveAngle;
    f32 c;
    f32 s;
    f32 offset;
    f32 sideX;
    f32 sideZ;
    f32 magA;
    f32 magB;
    f32 limitA;
    f32 limitB;
    f32 hardLimitA;
    f32 hardLimitB;
    f32 signA;
    f32 signB;
    f32 distSq;
    f32 dist;
    f32 dx;
    f32 dz;
    f64 inv;
    f64 half;
    f64 three;
    f64 square;
    u32 flags;
    u32 timeHi;
    u32 timeLo;
    s32 i;
    s32 j;
    s32 k;
    s32 tri;
    s32 blockCount;
    s32 polyCount;
    s32 indexCount;
    s32 active;
    s32 hitAny;
    f32 softSpeed;
    f32 hardSpeed;
    void* vertexBase;
    void* triBase;
    void* indexBase;
    s32 idx0;
    s32 idx1;
    s32 idx2;
    f32 sideARefX;
    f32 sideARefZ;
    s32* firstIndexPtr;
    s32* walkIndexPtr;
    Vec3 sideAxis;
    Vec3 forwardAxis;
    f32 compSide;
    f32 compForward;
    f32 negDirX;
    work0 = *(void**)wp;
    rodata = vec3_802bf540;
    road = *(void**)((s32)work0 + 0xEC);
    bestY = dat_8041f67c;
    foundRoad = NULL;

    pos.x = x;
    pos.y = y;
    pos.z = z;

    triPos.x = param4;
    triPos.y = param5;
    triPos.z = param6;

    for (i = 0; i < *(s32*)((s32)*(void**)wp + 0xC8); i++, road = (void*)((s32)road + 0xB8)) {
        roadEntry = (void*)((s32)*(void**)((s32)*(void**)wp + 0xF0) + (*(s32*)((s32)road + 0xA8) * 0x68));
        blockCount = *(s32*)((s32)road + 0xAC);

        if ((*(u32*)((s32)wp + 0x1D0) == 0x65) && (*(u32*)((s32)road + 0x28) != 0)) {
        continue;
        }

        if (blockCount == 0) {
            f32 roadY = *(f32*)((s32)road + 0x8C);

            if ((roadY <= param5) && (roadY > bestY)) {
                bestY = roadY;
                foundRoad = road;
            }
            continue;
        }

        for (j = 0; j < blockCount; j++, roadEntry = (void*)((s32)roadEntry + 0x68)) {
            vertexBase = (void*)((s32)*(void**)((s32)*(void**)wp + 0xFC) + (*(s32*)((s32)roadEntry + 0x58) * 0xC));
            triBase = (void*)((s32)*(void**)((s32)*(void**)wp + 0xF4) + (*(s32*)((s32)roadEntry + 0x60) * 8));
            polyCount = *(s32*)((s32)roadEntry + 0x64);

            for (k = 0; k < polyCount; k++, triBase = (void*)((s32)triBase + 8)) {
                s32 firstIndex = *(s32*)triBase;
                indexCount = *(s32*)((s32)triBase + 4) - 1;

                firstIndexPtr = (s32*)((s32)*(void**)((s32)*(void**)wp + 0x100) + (firstIndex * 4));
                walkIndexPtr = firstIndexPtr + 1;

                for (tri = 1; tri < indexCount; tri++, walkIndexPtr++) {
                    p3 = *(Vec3*)((s32)vertexBase + (walkIndexPtr[1] * 0xC));
                    p2 = *(Vec3*)((s32)vertexBase + (walkIndexPtr[0] * 0xC));
                    p1 = *(Vec3*)((s32)vertexBase + (firstIndexPtr[0] * 0xC));
                    p0 = triPos;

                    if (collisionTri_simple(&p0, &p1, &p2, &p3, &bestY) != 0) {
                        foundRoad = road;
                        break;
                    }
                }
            }
        }
    }

    if (foundRoad != NULL) {
        Vec3 curveInput;
        hit.bestY = float_neg1E06_8041f680;
        hit.nx = float_0_8041f62c;
        hit.ny = float_0_8041f62c;
        hit.nz = float_0_8041f62c;

        curveInput = pos;
        if (collisionCurve(0, foundRoad, &curveInput, &hit) == 0) {
            foundRoad = NULL;
        }
    }

    chosenRoad = foundRoad;

    if (chosenRoad == NULL) {
        if (*(s32*)prev != 0) {
            chosenRoad = *(void**)((s32)prev + 0x24);
        } else {
            chosenRoad = NULL;
        }

        if (chosenRoad == NULL) {
            *(f32*)((s32)wp + 0x1D4) = x;
            *(f32*)((s32)wp + 0x1D8) = y;
            *(f32*)((s32)wp + 0x1DC) = z;
            *(f32*)((s32)wp + 0x1E0) = param4;
            *(f32*)((s32)wp + 0x1E4) = param5;
            *(f32*)((s32)wp + 0x1E8) = param6;
            return;
        }

       outX = x;

     if ((*(u32*)((s32)wp + 0x1D0) == 0x65) &&
         (chosenRoad != NULL) &&
         (*(u32*)((s32)chosenRoad + 0x20) != 0)) {
        outY = *(f32*)((s32)chosenRoad + 0x24);
     } else {
        outY = y;
    }

    outZ = z;

        if ((*(u32*)((s32)wp + 0x1D0) == 0x65) &&
            (*(u32*)((s32)chosenRoad + 0x20) != 0)) {
            outY = *(f32*)((s32)chosenRoad + 0x24);
        }

        dirX = -*(f32*)((s32)prev + 0x34);
        dirY = -*(f32*)((s32)prev + 0x38);
        dirZ = -*(f32*)((s32)prev + 0x3C);
        roadScale = *(f32*)((s32)prev + 0x40);
        saved44 = *(f32*)((s32)prev + 0x44);
        saved48 = *(f32*)((s32)prev + 0x48);
        saved4C = *(f32*)((s32)prev + 0x4C);
        flags = 0;
    } else {
        if (hit.t < float_0_8041f62c) {
            hit.t = float_0_8041f62c;
        }
        if (hit.t > float_1_8041f638) {
            hit.t = float_1_8041f638;
        }

        saved44 = *(f32*)((s32)chosenRoad + 0x64);
        saved48 = *(f32*)((s32)chosenRoad + 0x70);
        roadScale = *(f32*)((s32)chosenRoad + 0x58);
        outX = hit.x;
        outZ = hit.z;

        if ((*(u32*)((s32)wp + 0x1D0) == 0x65) &&
            (*(u32*)((s32)chosenRoad + 0x20) != 0)) {
            outY = *(f32*)((s32)chosenRoad + 0x24);
        } else {
            outY = y;
        }

        saved4C = (f32)hit.index;
        flags = hit.flags;

        dx = -hit.nz;
        dz = hit.nx;
        dirY = hit.ny;

        distSq = (dx * dx) + (dz * dz);
        if (*(f64*)(rodata + 0x48) != (f64)distSq) {
            if (distSq > float_0_8041f62c) {
                inv = __frsqrte(distSq);
                half = *(f64*)(rodata + 0x38);
                three = *(f64*)(rodata + 0x40);

                square = inv * inv;
                inv = (half * inv) * (three - ((f64)distSq * square));
                square = inv * inv;
                inv = (half * inv) * (three - ((f64)distSq * square));
                square = inv * inv;
                inv = (half * inv) * (three - ((f64)distSq * square));

                dist = (f32)((f64)distSq * inv);
            } else if ((f64)distSq < *(f64*)(rodata + 0x48)) {
                dist = __float_nan[0];
            } else {
                u32 bits = *(u32*)&distSq;
                s32 exponent = bits & 0x7f800000;
                s32 classification;
                switch (exponent) {
                    case 0x7f800000:
                        classification = (bits & 0x7fffff) ? 1 : 2;
                        break;
                    case 0:
                        classification = (bits & 0x7fffff) ? 5 : 3;
                        break;
                    default:
                        classification = 4;
                        break;
                }
                dist = distSq;
                if (classification == 1) {
                    dist = __float_nan[0];
                }
            }
            dx = dx / dist;
            dz = dz / dist;
            dirY = float_0_8041f62c;
        }

        dirX = dx * float_neg1_8041f684;
        dirY = dirY * float_neg1_8041f684;
        dirZ = dz * float_neg1_8041f684;
    }

    if (*(s32*)((s32)chosenRoad + 0xB4) != 0) {
        Vec3 curvePos;
        Vec3 curveInput;

        curvePos.x = outX;
        curvePos.y = dat_8041f688;
        curvePos.z = outZ;
        hit.bestY = float_neg1E06_8041f680;
        curveInput = curvePos;

        if (collisionCurve(1,
                           (void*)((s32)*(void**)((s32)*(void**)wp + 0x104) +
                                  (*(s32*)((s32)chosenRoad + 0xB0) * 0x68)),
                           &curveInput,
                           &hit) != 0) {
            outX = hit.x;
            outZ = hit.z;
            flags |= hit.flags;
        }
    }

    if (chosenRoad != NULL) {
    curveAngle = float_3p1416_8041f648 *
                 ((**(f32**)((s32)*(void**)wp + 0xE8) * float_0p5_8041f64c) /
                  float_180_8041f668);

    if ((checkWidth != float_0_8041f62c) &&
        (foundRoad != NULL) &&
        (*(s32*)prev != 0)) {
        Vec3 sideInputA;
        Vec3 sideCurveInputA;
        Vec3 sideInputB;
        Vec3 sideCurveInputB;

        c = cos(curveAngle);
        s = sin(curveAngle);

        sideX = -dirZ;
        sideZ = dirX;

        sideHitA.bestY = float_neg1E06_8041f680;
        sideHitA.nx = float_0_8041f62c;
        sideHitA.ny = float_0_8041f62c;
        sideHitA.nz = float_0_8041f62c;

        offset = float_0p01_8041f66c *
                 (*(f32*)((s32)prev + 0x58) * float_1_8041f638 *
                  ((float_640_8041f68c * ((roadScale * s) / c)) / float_480_8041f690));

        sidePosA = pos;
        sidePosA.y = float_1E06_8041f694;
        sidePosA.x += offset * sideX;
        sidePosA.z += offset * dirX;

        sideInputA = sidePosA;
        hitAny = collisionCurve(0, chosenRoad, &sideInputA, &sideHitA);
        if (hitAny != 0) {
            sidePosA.x = sideHitA.x;
            sidePosA.y = outY;
            sidePosA.z = sideHitA.z;
        }

        if (*(s32*)((s32)chosenRoad + 0xB4) != 0) {
            sideHitA.bestY = float_neg1E06_8041f680;
            sideCurveInputA = sidePosA;
            if (collisionCurve(1,
                               (void*)((s32)*(void**)((s32)*(void**)wp + 0x104) +
                                      (*(s32*)((s32)chosenRoad + 0xB0) * 0x68)),
                               &sideCurveInputA,
                               &sideHitA) != 0) {
                hitAny = 1;
                sidePosA.x = sideHitA.x;
                sidePosA.y = outY;
                sidePosA.z = sideHitA.z;
            }
        }
        sideARefX = outX;
        sideARefZ = outZ;

        if (hitAny != 0) {
        sideARefX = sidePosA.x;
     sideARefZ = sidePosA.z;
     }
        c2 = cos(curveAngle);
        s2 = sin(curveAngle);
        sideHitB.bestY = float_neg1E06_8041f680;
        sideHitB.nx = float_0_8041f62c;
        sideHitB.ny = float_0_8041f62c;
        sideHitB.nz = float_0_8041f62c;

        offset = float_0p01_8041f66c *
         (checkWidth *
          (float_1_8041f638 *
           ((float_640_8041f68c * ((roadScale * s2) / c2)) / float_480_8041f690)));

        sidePosB = pos;
        sidePosB.y = float_1E06_8041f694;
        sidePosB.x += offset * sideX;
        sidePosB.z += offset * dirX;

        sideInputB = sidePosB;
        active = collisionCurve(0, chosenRoad, &sideInputB, &sideHitB);
        flags = 0;
        if (active != 0) {
            sidePosB.x = sideHitB.x;
            sidePosB.y = outY;
            sidePosB.z = sideHitB.z;
            flags = sideHitB.flags;
        }

        if (*(s32*)((s32)chosenRoad + 0xB4) != 0) {
            sideHitB.bestY = float_neg1E06_8041f680;
            sideCurveInputB = sidePosB;
            if (collisionCurve(1,
                               (void*)((s32)*(void**)((s32)*(void**)wp + 0x104) +
                                      (*(s32*)((s32)chosenRoad + 0xB0) * 0x68)),
                               &sideCurveInputB,
                               &sideHitB) != 0) {
                active = 1;
                sidePosB.x = sideHitB.x;
                flags |= sideHitB.flags;
                sidePosB.z = sideHitB.z;
            }
        }

        if (active != 0) {
            outX = sidePosB.x;
            outZ = sidePosB.z;

            if (((sideHitB.flags & 1) == 0) && ((*(u32*)((s32)prev + 8) & 1) != 0)) {
                *(f32*)((s32)wp + 0x1AC) = float_0_8041f62c;
            }
            if (((sideHitB.flags & 2) == 0) && ((*(u32*)((s32)prev + 8) & 2) != 0)) {
                *(f32*)((s32)wp + 0x1B0) = float_0_8041f62c;
            }
        }

        magVecA.x = sideARefX - *(f32*)((s32)prev + 0x28);
        magVecA.y = float_0_8041f62c;
        magVecA.z = sideARefZ - *(f32*)((s32)prev + 0x30);

        magVecB.x = outX - *(f32*)((s32)prev + 0x28);
        magVecB.y = float_0_8041f62c;
        magVecB.z = outZ - *(f32*)((s32)prev + 0x30);

        magA = PSVECMag(&magVecA);
        PSVECMag(&magVecB);

        softSpeed = getV60FPS(*(f32*)((s32)wp + 0x19C),
                      *(s32*)((s32)prev + 0x50),
                      *(s32*)((s32)prev + 0x54),
                      *(u32*)((s32)gp + 0x40),
                      *(u32*)((s32)gp + 0x44));

        hardSpeed = getV60FPS(*(f32*)((s32)wp + 0x1A0),
                      *(s32*)((s32)prev + 0x50),
                      *(s32*)((s32)prev + 0x54),
                      *(u32*)((s32)gp + 0x40),
                      *(u32*)((s32)gp + 0x44));

        limitA = (magA + softSpeed) * *(f32*)((s32)wp + 0x1AC);
        limitB = (magA + softSpeed) * *(f32*)((s32)wp + 0x1B0);

        hardLimitA = hardSpeed * *(f32*)((s32)wp + 0x1AC);
        hardLimitB = hardSpeed * *(f32*)((s32)wp + 0x1B0);

        sideVecA.x = sideARefX - *(f32*)((s32)prev + 0x28);
        sideVecA.y = float_0_8041f62c;
        sideVecA.z = sideARefZ - *(f32*)((s32)prev + 0x30);

        sideVecB.x = outX - *(f32*)((s32)prev + 0x28);
        sideVecB.y = float_0_8041f62c;
        sideVecB.z = outZ - *(f32*)((s32)prev + 0x30);

        negDirX = -dirX;
        

if (__fabsf(dirX) <= __fabsf(sideX)) {
    compForward = -((dirX * (magVecB.x / sideX)) - magVecB.z) /
                  -((dirX * (negDirX / sideX)) - sideX);
    compSide = -((compForward * negDirX) - magVecB.x) / sideX;
} else {
    compForward = -((sideX * (magVecB.z / dirX)) - magVecB.x) /
                  -((sideX * (sideX / dirX)) - negDirX);
    compSide = -((compForward * sideX) - magVecB.z) / dirX;
}

        sideVecA.x = compSide * sideX;
        sideVecA.y = float_0_8041f62c;
        sideVecA.z = compSide * dirX;

        sideVecB.x = compForward * negDirX;
        sideVecB.y = float_0_8041f62c;
        sideVecB.z = compForward * sideX;

        signA = compSide < float_0_8041f62c ? float_neg1_8041f684 : float_1_8041f638;
        signB = compForward < float_0_8041f62c ? float_neg1_8041f684 : float_1_8041f638;

        sideAxis.x = sideX;
        sideAxis.y = float_0_8041f62c;
        sideAxis.z = dirX;

        forwardAxis.x = negDirX;
        forwardAxis.y = float_0_8041f62c;
        forwardAxis.z = sideX;

        magA = PSVECMag(&sideVecA);
        magB = PSVECMag(&sideVecB);

    if ((magA > limitA) && (magA > hardLimitA)) {
    PSVECScale(&sideAxis, &sideVecA, limitA * signA);
    }
    if ((magB > limitB) && (magB > hardLimitB)) {
    PSVECScale(&forwardAxis, &sideVecB, limitB * signB);
    }

        outX = *(f32*)((s32)prev + 0x28) + sideVecA.x + sideVecB.x;
        outZ = *(f32*)((s32)prev + 0x30) + sideVecA.z + sideVecB.z;
    }
    }

    *(f32*)((s32)wp + 0x1AC) = *(f32*)((s32)wp + 0x1AC) + *(f32*)((s32)wp + 0x1A4);
    if (*(f32*)((s32)wp + 0x1AC) > float_1_8041f638) {
        *(f32*)((s32)wp + 0x1AC) = float_1_8041f638;
    }

    *(f32*)((s32)wp + 0x1B0) = *(f32*)((s32)wp + 0x1B0) + *(f32*)((s32)wp + 0x1A8);
    if (*(f32*)((s32)wp + 0x1B0) > float_1_8041f638) {
        *(f32*)((s32)wp + 0x1B0) = float_1_8041f638;
    }

    *(s32*)((s32)out + 0x00) = 1;
    *(u32*)((s32)out + 0x04) = ((u32)-(s32)foundRoad | (u32)foundRoad) >> 31;
    *(u32*)((s32)out + 0x08) = flags;
    *(f32*)((s32)out + 0x0C) = x;
    *(f32*)((s32)out + 0x10) = y;
    *(f32*)((s32)out + 0x14) = z;
    *(f32*)((s32)out + 0x18) = param4;
    *(f32*)((s32)out + 0x1C) = param5;
    *(f32*)((s32)out + 0x20) = param6;
    *(void**)((s32)out + 0x24) = chosenRoad;
    *(f32*)((s32)out + 0x28) = outX;
    *(f32*)((s32)out + 0x2C) = outY;
    *(f32*)((s32)out + 0x30) = outZ;
    *(f32*)((s32)out + 0x34) = -dirX;
    *(f32*)((s32)out + 0x38) = -dirY;
    *(f32*)((s32)out + 0x3C) = -dirZ;
    *(f32*)((s32)out + 0x40) = roadScale;
    *(f32*)((s32)out + 0x44) = saved44;
    *(f32*)((s32)out + 0x48) = saved48;
    *(f32*)((s32)out + 0x4C) = saved4C;
    timeHi = *(u32*)((s32)gp + 0x40);
    timeLo = *(u32*)((s32)gp + 0x44);
    *(u32*)((s32)out + 0x54) = timeLo;
    *(u32*)((s32)out + 0x50) = timeHi;
    *(f32*)((s32)out + 0x58) = checkWidth;
    *(f32*)((s32)out + 0x5C) = *(f32*)((s32)chosenRoad + 0x7C);
}
void* compareCamRoad(void* a, void* b, void* outRate) {
    const char* rodata;
    u32 flags;
    s32 modeA;
    s32 diff;
    s32 sign;
    s32 absDiff;
    s32 length;
    f32 value;
    f32 maxValue;
    f32 angle;
    f32 angleDiff;
    f32 temp;
    f32 dx;
    f32 dx2;
    f32 dy2;
    f32 dz2;
    f32 dy;
    f32 dz;
    f32 distSq;
    f32 dist;
    f32 classifyValue;
    u32 bits;
    u32 exp;
    s32 type;
    f64 inv;
    f64 half;
    f64 three;
    f64 square;

    rodata = vec3_802bf540;
    flags = 0;

    if (*(s32*)((s32)b + 0x0) == 0) {
        return 0;
    }

    if (*(void**)((s32)a + 0x24) == *(void**)((s32)b + 0x24)) {
        modeA = *(s32*)((s32)a + 0x4);

        if (modeA == 1) {
            if (*(s32*)((s32)b + 0x4) == 0) {
                goto changed;
            }
        }

        if (modeA == 0) {
            if (*(s32*)((s32)b + 0x8) == 1) {
                goto changed;
            }
        }

        diff = (s32)*(f32*)((s32)a + 0x4C) - (s32)*(f32*)((s32)b + 0x4C);
        sign = diff >> 31;
        absDiff = (diff ^ sign) - sign;

        if (absDiff <= 1) {
            return 0;
        }

        length = *(s32*)((s32)*(void**)((s32)a + 0x24) + 0x44) -
                 *(s32*)((s32)*(void**)((s32)a + 0x24) + 0x40);

        sign = diff >> 31;
        absDiff = (diff ^ sign) - sign;

        if (absDiff >= length - 1) {
            return 0;
        }

        sign = diff >> 31;
        absDiff = (diff ^ sign) - sign;

        if (absDiff >= length - 2) {
            flags |= 0x20;
        }
    }

changed:
    if (*(void**)((s32)a + 0x24) != *(void**)((s32)b + 0x24)) {
        flags |= 0x1;
    }

    modeA = *(s32*)((s32)a + 0x4);

    if (modeA == 1) {
        if (*(s32*)((s32)b + 0x4) == 0) {
            flags |= 0x2;
        }
    }

    if (modeA == 0) {
        if (*(s32*)((s32)b + 0x8) == 1) {
            flags |= 0x4;
        }
    }

    value = __fabsf(*(f32*)((s32)a + 0x4C) - *(f32*)((s32)b + 0x4C));
    if ((value <= *(f64*)(rodata + 0x30)) == 0) {
        flags |= 0x8;
    }

    value = (*(f32*)((s32)a + 0x34) * *(f32*)((s32)b + 0x34)) +
            (*(f32*)((s32)a + 0x3C) * *(f32*)((s32)b + 0x3C));

    if (value > float_1_8041f638) {
        value = float_1_8041f638;
    }

    angle = float_rad2deg_8041f660 * (f32)acos(value);
    angle = __fabsf(angle);

    if (angle > float_180_8041f668) {
        angle = float_360_8041f664 - angle;
    }

    maxValue = (((float_6p2832_8041f654 * *(f32*)((s32)a + 0x40)) * angle) / float_360_8041f664) *
               *(f32*)((s32)wp + 0x1B4);

    angleDiff = __fabsf(*(f32*)((s32)a + 0x44) - *(f32*)((s32)b + 0x44));
    if (angleDiff > float_180_8041f668) {
        angleDiff = float_360_8041f664 - angleDiff;
    }

    temp = (((float_6p2832_8041f654 * *(f32*)((s32)a + 0x40)) * angleDiff) / float_360_8041f664) *
           *(f32*)((s32)wp + 0x1B8);

    if (temp > maxValue) {
        maxValue = temp;
    }

    angleDiff = __fabsf(*(f32*)((s32)a + 0x48) - *(f32*)((s32)b + 0x48));
    if (angleDiff > float_180_8041f668) {
        angleDiff = float_360_8041f664 - angleDiff;
    }

    temp = (((float_6p2832_8041f654 * *(f32*)((s32)a + 0x40)) * angleDiff) / float_360_8041f664) *
           *(f32*)((s32)wp + 0x1BC);

    if (temp > maxValue) {
        maxValue = temp;
    }

    temp = __fabsf(*(f32*)((s32)a + 0x40) - *(f32*)((s32)b + 0x40)) *
           *(f32*)((s32)wp + 0x1C0);

    if (temp > maxValue) {
        maxValue = temp;
    }

     dx = *(f32*)((s32)a + 0x28) - *(f32*)((s32)b + 0x28);
    dy = *(f32*)((s32)a + 0x2C) - *(f32*)((s32)b + 0x2C);
    dz = *(f32*)((s32)a + 0x30) - *(f32*)((s32)b + 0x30);

    dx2 = dx * dx;
    dy2 = dy * dy;
    dz2 = dz * dz;

    distSq = dx2 + dy2;
    distSq = distSq + dz2;

    if (distSq != float_0_8041f62c) {
        if (distSq > float_0_8041f62c) {
            inv = __frsqrte(distSq);
            half = *(f64*)(rodata + 0x38);
            three = *(f64*)(rodata + 0x40);

            square = inv * inv;
            inv = (half * inv) * (three - (distSq * square));
            square = inv * inv;
            inv = (half * inv) * (three - (distSq * square));
            square = inv * inv;
            inv = (half * inv) * (three - (distSq * square));

            dist = (f32)(distSq * inv);
        } else if (distSq < *(f64*)(rodata + 0x48)) {
            dist = __float_nan[0];
        } else {
            classifyValue = distSq;
            bits = *(u32*)&classifyValue;
            exp = bits & 0x7F800000;

            if (exp == 0x7F800000) {
                if ((bits & 0x007FFFFF) == 0) {
                    type = 2;
                } else {
                    type = 1;
                }
            } else if (exp == 0) {
                if ((bits & 0x007FFFFF) == 0) {
                    type = 3;
                } else {
                    type = 5;
                }
            } else {
                type = 4;
            }

            if (type == 1) {
                dist = __float_nan[0];
            } else {
                dist = distSq;
            }
        }
    } else {
        dist = float_0_8041f62c;
    }

    if (maxValue < float_0p01_8041f66c) {
        if (dist < float_0p3_8041f670) {
            return 0;
        }
    }

    temp = dist * *(f32*)((s32)wp + 0x1C4);
    if (temp > maxValue) {
        maxValue = temp;
    }

    if (maxValue > float_90_8041f674) {
        maxValue = float_90_8041f674;
    }

    if (maxValue < float_20_8041f678) {
        maxValue = float_20_8041f678;
    }

    *(f32*)outRate = float_1_8041f638 / maxValue;

    if (flags == 0) {
        flags |= 0x10;
    }

    return (void*)flags;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void calcHokanCamRoad(s32 useInterp, f32 t, void* a, void* b, void* unused, void* out) {
    f32 cosA;
    f32 cosB;
    f32 cosT;
    f32 rate;
    f32 piT;
    f32 oneForCos;
    f32 denom;
    f32 scale;
    f32 term;
    f32 value;
    f32 angleATmp;
    f32 angleA;
    f32 angleB;
    f32 twoPi;
    f32 limit;
    f32 diff;
    f32 atanBX;
    f32 atanBZ;

    cosA = cos(float_0_8041f62c);
    cosB = cos(float_0_8041f62c);

    oneForCos = float_1_8041f638;
    piT = float_3p1416_8041f648 * t;
    cosT = cos(piT * oneForCos + float_0_8041f62c);

    denom = float_1_8041f638 + cosA;
    scale = float_2_8041f63c / denom;
    term = float_0p5_8041f64c * (float_1_8041f638 - cosB) + cosT;
    value = scale * term;
    value = float_1_8041f638 + value;
    value = float_2_8041f63c - value;
    rate = value * float_0p5_8041f650;

    if (rate > float_1_8041f638) {
        rate = float_1_8041f638;
    }

    if (useInterp != 0) {
        *(f32*)((s32)out + 0x28) = rate * (*(f32*)((s32)b + 0x28) - *(f32*)((s32)a + 0x28)) + *(f32*)((s32)a + 0x28);
        *(f32*)((s32)out + 0x2C) = rate * (*(f32*)((s32)b + 0x2C) - *(f32*)((s32)a + 0x2C)) + *(f32*)((s32)a + 0x2C);
        *(f32*)((s32)out + 0x30) = rate * (*(f32*)((s32)b + 0x30) - *(f32*)((s32)a + 0x30)) + *(f32*)((s32)a + 0x30);
    } else {
        *(f32*)((s32)out + 0x28) = *(f32*)((s32)b + 0x28);
        *(f32*)((s32)out + 0x2C) = *(f32*)((s32)b + 0x2C);
        *(f32*)((s32)out + 0x30) = *(f32*)((s32)b + 0x30);
    }

    angleATmp = atan2(*(f32*)((s32)a + 0x34), *(f32*)((s32)a + 0x3C));
    atanBX = *(f32*)((s32)b + 0x34);
    atanBZ = *(f32*)((s32)b + 0x3C);
    angleA = angleATmp;
    angleB = atan2(atanBX, atanBZ);

    twoPi = float_6p2832_8041f654;
    limit = float_3p1416_8041f658;
    while (float_3p1416_8041f658 < (angleA - angleB)) {
        angleA -= float_6p2832_8041f654;
    }

    twoPi = float_6p2832_8041f654;
    limit = float_neg3p1416_8041f65c;
     while (float_neg3p1416_8041f65c > (angleA - angleB)) {
        angleA += float_6p2832_8041f654;
    }

    angleA = rate * (angleB - angleA) + angleA;

    *(f32*)((s32)out + 0x34) = sin(angleA);
    *(f32*)((s32)out + 0x3C) = cos(angleA);

    *(f32*)((s32)out + 0x38) = rate * (*(f32*)((s32)b + 0x38) - *(f32*)((s32)a + 0x38)) + *(f32*)((s32)a + 0x38);
    *(f32*)((s32)out + 0x40) = rate * (*(f32*)((s32)b + 0x40) - *(f32*)((s32)a + 0x40)) + *(f32*)((s32)a + 0x40);
    *(f32*)((s32)out + 0x44) = rate * (*(f32*)((s32)b + 0x44) - *(f32*)((s32)a + 0x44)) + *(f32*)((s32)a + 0x44);
    *(f32*)((s32)out + 0x48) = rate * (*(f32*)((s32)b + 0x48) - *(f32*)((s32)a + 0x48)) + *(f32*)((s32)a + 0x48);
    *(f32*)((s32)out + 0x5C) = rate * (*(f32*)((s32)b + 0x5C) - *(f32*)((s32)a + 0x5C)) + *(f32*)((s32)a + 0x5C);
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


void camRoadMain(void* cam, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f) {
    void* hit;
    f32 value;
    f32 one;
    s32 timeA;
    s32 timeB;

    *(f32*)((s32)wp + 0x1EC) = a;
    *(f32*)((s32)wp + 0x1F0) = b;
    *(f32*)((s32)wp + 0x1F4) = c;
    *(f32*)((s32)wp + 0x1F8) = d;
    *(f32*)((s32)wp + 0x1FC) = e;
    *(f32*)((s32)wp + 0x200) = f;

    calcCamRoad(
    a / float_10_8041f644,
    b / float_10_8041f644,
    c / float_10_8041f644,
    d / float_10_8041f644,
    e / float_10_8041f644,
    f / float_10_8041f644,
    float_0_8041f62c,
    (void*)((s32)wp + 0x68),
    (void*)((s32)wp + 0xD8)
);

    if (*(s32*)((s32)wp + 0xC8) != 0) {
        hit = compareCamRoad(
            (void*)((s32)wp + 0xD8),
            (void*)((s32)wp + 0x68),
            (void*)((s32)wp + 0xD4)
        );

        if (hit != 0) {
            *(void**)((s32)wp + 0x198) = hit;
            *(s32*)((s32)wp + 0xC8) = 1;

            STORE_CAMROAD_TIME();

            *(f32*)((s32)wp + 0xD0) = float_0_8041f62c;

            COPY_CAMROAD_BLOCK((s32)wp + 0x134, (s32)wp + 0x4);
            COPY_CAMROAD_BLOCK((s32)wp + 0xD4, (s32)wp + 0x64);

            UPDATE_CAMROAD_CC();
        } else {
            COPY_CAMROAD_BLOCK((s32)wp + 0xD4, (s32)wp + 0x64);
        }
    }

    if (*(s32*)((s32)wp + 0xC8) == 0) {
        hit = compareCamRoad(
            (void*)((s32)wp + 0x68),
            (void*)((s32)wp + 0xD8),
            (void*)((s32)wp + 0xD4)
        );

        if (hit != 0) {
            *(void**)((s32)wp + 0x198) = hit;
            *(s32*)((s32)wp + 0xC8) = 1;

            STORE_CAMROAD_TIME();

            *(f32*)((s32)wp + 0xD0) = float_0_8041f62c;

            COPY_CAMROAD_BLOCK((s32)wp + 0x134, (s32)wp + 0xD4);
            COPY_CAMROAD_BLOCK((s32)wp + 0xD4, (s32)wp + 0x64);

            UPDATE_CAMROAD_CC();
        }
    }

    if (*(s32*)((s32)wp + 0xC8) != 0) {
        value = getV60FPS(
            *(f32*)((s32)wp + 0xD4),
            *(s32*)((s32)wp + 0x1C8),
            *(s32*)((s32)wp + 0x1CC),
            *(s32*)((s32)gp + 0x40),
            *(s32*)((s32)gp + 0x44)
        );

        one = float_1_8041f638;

        *(f32*)((s32)wp + 0xD0) = *(f32*)((s32)wp + 0xD0) + value;
        if (*(f32*)((s32)wp + 0xD0) > one) {
            *(f32*)((s32)wp + 0xD0) = one;
        }

        STORE_CAMROAD_TIME();

        calcHokanCamRoad(
            *(s32*)((s32)wp + 0xCC),
            *(f32*)((s32)wp + 0xD0),
            (void*)((s32)wp + 0x138),
            (void*)((s32)wp + 0xD8),
            (void*)((s32)wp + 0x68),
            (void*)((s32)wp + 0x8)
        );

        if (*(s32*)((s32)wp + 0xCC) != 0) {
            *(s32*)((s32)wp + 0x10) = 1;
        } else {
            *(s32*)((s32)wp + 0x10) = 0;
        }

        if (*(f32*)((s32)wp + 0xD0) == float_1_8041f638) {
            COPY_CAMROAD_BLOCK((s32)wp + 0x4, (s32)wp + 0xD4);
            *(s32*)((s32)wp + 0xC8) = 0;
        }
    } else {
        COPY_CAMROAD_BLOCK((s32)wp + 0xD4, (s32)wp + 0x64);
        COPY_CAMROAD_BLOCK((s32)wp + 0x4, (s32)wp + 0xD4);
    }

    calcCurrentCamState(cam);

    *(f32*)((s32)cam + 0x18) = *(f32*)((s32)cam + 0x18) * float_10_8041f644;
    *(f32*)((s32)cam + 0x1C) = *(f32*)((s32)cam + 0x1C) * float_10_8041f644;
    *(f32*)((s32)cam + 0x20) = *(f32*)((s32)cam + 0x20) * float_10_8041f644;
    *(f32*)((s32)cam + 0x0C) = *(f32*)((s32)cam + 0x0C) * float_10_8041f644;
    *(f32*)((s32)cam + 0x10) = *(f32*)((s32)cam + 0x10) * float_10_8041f644;
    *(f32*)((s32)cam + 0x14) = *(f32*)((s32)cam + 0x14) * float_10_8041f644;
}

void* camRoadGetWorkPtr(void) {
    return wp;
}
f32 camRoadGetCurrentShiftXRate(void) {
    if (*(s32*)((s32)wp + 0x68) != 0) {
        return *(f32*)((s32)wp + 0xC4);
    }

    return float_0_8041f62c;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void camRoadReset(void) {
    s32 saved204;
    s32 saved0;
    s32 saved1D0;

    saved204 = *(s32*)((s32)wp + 0x204);
    saved0 = *(s32*)wp;
    saved1D0 = *(s32*)((s32)wp + 0x1D0);

    memset(wp, 0, 0x210);

    *(s32*)((s32)wp + 0xC8) = 0;
    *(s32*)((s32)wp + 0xD8) = 0;
    *(s32*)((s32)wp + 0xD8) = 0;

    *(f32*)((s32)wp + 0x19C) = float_0p005_8041f628;
    *(f32*)((s32)wp + 0x1A0) = float_0_8041f62c;
    *(f32*)((s32)wp + 0x1A4) = float_0p004_8041f630;
    *(f32*)((s32)wp + 0x1A8) = float_0p008_8041f634;
    *(f32*)((s32)wp + 0x1AC) = float_1_8041f638;
    *(f32*)((s32)wp + 0x1B0) = float_1_8041f638;
    *(f32*)((s32)wp + 0x1B4) = float_2_8041f63c;
    *(f32*)((s32)wp + 0x1B8) = float_1_8041f638;
    *(f32*)((s32)wp + 0x1BC) = float_1_8041f638;
    *(f32*)((s32)wp + 0x1C0) = float_3_8041f640;
    *(f32*)((s32)wp + 0x1C4) = float_1_8041f638;

    *(s32*)((s32)wp + 0x204) = -1;
    *(s32*)((s32)wp + 0x208) = 0;

    *(s32*)((s32)wp + 0x204) = saved204;
    *(s32*)wp = saved0;
    *(s32*)((s32)wp + 0x1D0) = saved1D0;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void camRoadCurveOn(char* name) {
    void* entry;
    s32 i;

    entry = *(void**)((s32)*(void**)wp + 0xEC);

    if ((u32)*(s32*)((s32)wp + 0x1D0) == 0x65) {
        i = 0;
        while (i < *(s32*)((s32)*(void**)wp + 0xC8)) {
            if (strcmp(entry, name) == 0) {
                *(s32*)((s32)entry + 0x28) = 0;
                break;
            }

            i++;
            entry = (void*)((s32)entry + 0xB8);
        }
    }
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void camRoadCurveOff(char* name) {
    void* entry;
    s32 i;

    entry = *(void**)((s32)*(void**)wp + 0xEC);

    if ((u32)*(s32*)((s32)wp + 0x1D0) == 0x65) {
        i = 0;
        while (i < *(s32*)((s32)*(void**)wp + 0xC8)) {
            if (strcmp(entry, name) == 0) {
                *(s32*)((s32)entry + 0x28) = 1;
                break;
            }

            i++;
            entry = (void*)((s32)entry + 0xB8);
        }
    }
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

u8 work[0x210];
void* wp = work;

const f32 float_0p005_8041f628 = 0.005f;
const f32 float_0_8041f62c = 0.0f;
const f32 float_0p004_8041f630 = 0.004f;
const f32 float_0p008_8041f634 = 0.008f;
const f32 float_1_8041f638 = 1.0f;
const f32 float_2_8041f63c = 2.0f;
const f32 float_3_8041f640 = 3.0f;
const f32 float_10_8041f644 = 10.0f;
const f32 float_3p1416_8041f648 = 3.141592f;
const f32 float_0p5_8041f64c = 0.5f;
const f32 float_0p5_8041f650 = 0.500001f;
const f32 float_6p2832_8041f654 = 6.2831855f;
const f32 float_3p1416_8041f658 = 3.1415927f;
const f32 float_neg3p1416_8041f65c = -3.1415927f;
const f32 float_rad2deg_8041f660 = 57.29578f;
const f32 float_360_8041f664 = 360.0f;
const f32 float_180_8041f668 = 180.0f;
const f32 float_0p01_8041f66c = 0.01f;
const f32 float_0p3_8041f670 = 0.3f;
const f32 float_90_8041f674 = 90.0f;
const f32 float_20_8041f678 = 20.0f;
const f32 dat_8041f67c = -100000000.0f;
const f32 float_neg1E06_8041f680 = -1000000.0f;
const f32 float_neg1_8041f684 = -1.0f;
const f32 dat_8041f688 = 100000000.0f;
const f32 float_640_8041f68c = 640.0f;
const f32 float_480_8041f690 = 480.0f;
const f32 float_1E06_8041f694 = 1000000.0f;
const f32 float_100_8041f698 = 100.0f;
const f32 float_deg2rad_8041f69c = 0.017453292f;
const char str_PCTs_PCTs_8041f6a0[] = "%s/%s";
const char str_1_01_8041f6a8[] = "1.01";
