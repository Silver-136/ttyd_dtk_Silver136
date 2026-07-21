#include "cam_road.h"

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
extern void camShiftInit(void);
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

extern char str_PCTs_PCTs_8041f6a0;
extern char str_1_01_8041f6a8;

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
        sprintf(path, &str_PCTs_PCTs_8041f6a0, getMapDataDvdRoot(), name);

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

    if (strcmp((char*)((s32)*(void**)wp + 0x44), &str_1_01_8041f6a8) == 0) {
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on


s32 collisionTri_simple(void* a, void* b, void* c, void* d, void* e) {
    extern const Vec3 vec3_802bf564;
    extern const f32 float_0_8041f62c;
    extern const f64 double_0_802bf588;

    u32 p0;
    u32 p1;
    u32 p2;
    u32 v00;
    u32 v01;
    u32 v02;
    u32 v10;
    u32 v11;
    u32 v12;
    u32 v20;
    u32 v21;
    u32 v22;
    u32 dir0;
    u32 dir1;
    u32 dir2;
    u32* dirPtr;

    volatile u32 scratch[96];
    volatile u32 e01[3];
    volatile u32 e20[3];
    volatile u32 e12[3];
    volatile u32 normal[3];
    volatile u32 diff[3];
    volatile u32 tmpVec[3];
    volatile u32 hit[3];
    volatile u32 distBits;
    volatile u32 denomBits;
    volatile u32 sideBits;

    f32 tmp;
    f32 dist;
    f32 denom;
    f32 side;
    f32 oldY;
    f32 scale;
    f32 hitY;

    v10 = ((u32*)c)[0];
    v00 = ((u32*)b)[0];
    v01 = ((u32*)b)[1];
    v11 = ((u32*)c)[1];
    v02 = ((u32*)b)[2];
    v12 = ((u32*)c)[2];
    v20 = ((u32*)d)[0];
    v21 = ((u32*)d)[1];
    v22 = ((u32*)d)[2];

    p0 = ((u32*)a)[0];
    p1 = ((u32*)a)[1];
    p2 = ((u32*)a)[2];

    dirPtr = (u32*)&vec3_802bf564;
    dir0 = dirPtr[0];
    dir1 = dirPtr[1];
    dir2 = dirPtr[2];

    tmp = ((scratch[13] = (v00)), (*(volatile f32*)&scratch[13])) - ((scratch[10] = (v10)), (*(volatile f32*)&scratch[10]));
    (*(volatile f32*)&scratch[0]) = (tmp);
    e01[0] = scratch[0];

    tmp = ((scratch[14] = (v01)), (*(volatile f32*)&scratch[14])) - ((scratch[11] = (v11)), (*(volatile f32*)&scratch[11]));
    (*(volatile f32*)&scratch[1]) = (tmp);
    e01[1] = scratch[1];

    tmp = ((scratch[15] = (v02)), (*(volatile f32*)&scratch[15])) - ((scratch[12] = (v12)), (*(volatile f32*)&scratch[12]));
    (*(volatile f32*)&scratch[2]) = (tmp);
    e01[2] = scratch[2];

    tmp = ((scratch[16] = (v20)), (*(volatile f32*)&scratch[16])) - ((scratch[13] = (v00)), (*(volatile f32*)&scratch[13]));
    (*(volatile f32*)&scratch[3]) = (tmp);
    e20[0] = scratch[3];

    tmp = ((scratch[17] = (v21)), (*(volatile f32*)&scratch[17])) - ((scratch[14] = (v01)), (*(volatile f32*)&scratch[14]));
    (*(volatile f32*)&scratch[4]) = (tmp);
    e20[1] = scratch[4];

    tmp = ((scratch[18] = (v22)), (*(volatile f32*)&scratch[18])) - ((scratch[15] = (v02)), (*(volatile f32*)&scratch[15]));
    (*(volatile f32*)&scratch[5]) = (tmp);
    e20[2] = scratch[5];

    tmp = ((scratch[10] = (v10)), (*(volatile f32*)&scratch[10])) - ((scratch[16] = (v20)), (*(volatile f32*)&scratch[16]));
    (*(volatile f32*)&scratch[6]) = (tmp);
    e12[0] = scratch[6];

    tmp = ((scratch[11] = (v11)), (*(volatile f32*)&scratch[11])) - ((scratch[17] = (v21)), (*(volatile f32*)&scratch[17]));
    (*(volatile f32*)&scratch[7]) = (tmp);
    e12[1] = scratch[7];

    tmp = ((scratch[12] = (v12)), (*(volatile f32*)&scratch[12])) - ((scratch[18] = (v22)), (*(volatile f32*)&scratch[18]));
    (*(volatile f32*)&scratch[8]) = (tmp);
    e12[2] = scratch[8];

    tmp = ((*(volatile f32*)&e20[1]) * (*(volatile f32*)&e01[2])) -
          ((*(volatile f32*)&e20[2]) * (*(volatile f32*)&e01[1]));
    (*(volatile f32*)&scratch[20]) = (tmp);
    normal[0] = scratch[20];

    tmp = ((*(volatile f32*)&e20[2]) * (*(volatile f32*)&e01[0])) -
          ((*(volatile f32*)&e20[0]) * (*(volatile f32*)&e01[2]));
    (*(volatile f32*)&scratch[21]) = (tmp);
    normal[1] = scratch[21];

    tmp = ((*(volatile f32*)&e20[0]) * (*(volatile f32*)&e01[1])) -
          ((*(volatile f32*)&e20[1]) * (*(volatile f32*)&e01[0]));
    (*(volatile f32*)&scratch[22]) = (tmp);
    normal[2] = scratch[22];

    if ((float_0_8041f62c == (*(volatile f32*)&normal[0])) &&
        (float_0_8041f62c == (*(volatile f32*)&normal[1])) &&
        (float_0_8041f62c == (*(volatile f32*)&normal[2]))) {
        return 0;
    }

    tmp = ((scratch[30] = (p0)), (*(volatile f32*)&scratch[30])) - ((scratch[13] = (v00)), (*(volatile f32*)&scratch[13]));
    (*(volatile f32*)&scratch[23]) = (tmp);
    diff[0] = scratch[23];

    tmp = ((scratch[31] = (p1)), (*(volatile f32*)&scratch[31])) - ((scratch[14] = (v01)), (*(volatile f32*)&scratch[14]));
    (*(volatile f32*)&scratch[24]) = (tmp);
    diff[1] = scratch[24];

    tmp = ((scratch[32] = (p2)), (*(volatile f32*)&scratch[32])) - ((scratch[15] = (v02)), (*(volatile f32*)&scratch[15]));
    (*(volatile f32*)&scratch[25]) = (tmp);
    diff[2] = scratch[25];

    dist = ((*(volatile f32*)&normal[2]) * (*(volatile f32*)&diff[2])) +
           ((*(volatile f32*)&normal[0]) * (*(volatile f32*)&diff[0])) +
           ((*(volatile f32*)&normal[1]) * (*(volatile f32*)&diff[1]));
    (*(volatile f32*)&scratch[26]) = (dist);
    distBits = scratch[26];

    denom = ((*(volatile f32*)&normal[2]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))) +
            ((*(volatile f32*)&normal[0]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))) +
            ((*(volatile f32*)&normal[1]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34])));
    (*(volatile f32*)&scratch[27]) = (denom);
    denomBits = scratch[27];

    if ((*(volatile f32*)&distBits) <= float_0_8041f62c) {
        if ((f64)(*(volatile f32*)&denomBits) <= double_0_802bf588) {
            return 0;
        }

        tmp = ((scratch[32] = (p2)), (*(volatile f32*)&scratch[32])) - ((scratch[15] = (v02)), (*(volatile f32*)&scratch[15]));
        (*(volatile f32*)&scratch[40]) = (tmp);
        tmpVec[2] = scratch[40];
        tmp = ((scratch[30] = (p0)), (*(volatile f32*)&scratch[30])) - ((scratch[13] = (v00)), (*(volatile f32*)&scratch[13]));
        (*(volatile f32*)&scratch[38]) = (tmp);
        tmpVec[0] = scratch[38];
        tmp = ((scratch[31] = (p1)), (*(volatile f32*)&scratch[31])) - ((scratch[14] = (v01)), (*(volatile f32*)&scratch[14]));
        (*(volatile f32*)&scratch[39]) = (tmp);
        tmpVec[1] = scratch[39];

        side = ((*(volatile f32*)&tmpVec[2]) * (((*(volatile f32*)&e20[0]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))) - ((*(volatile f32*)&e20[1]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))))) +
               ((*(volatile f32*)&tmpVec[0]) * (((*(volatile f32*)&e20[1]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34]))) - ((*(volatile f32*)&e20[2]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))))) +
               ((*(volatile f32*)&tmpVec[1]) * (((*(volatile f32*)&e20[2]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))) - ((*(volatile f32*)&e20[0]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34])))));
        (*(volatile f32*)&scratch[28]) = (side);
        sideBits = scratch[28];
        if ((f64)(*(volatile f32*)&sideBits) < double_0_802bf588) {
            return 0;
        }

        tmp = ((scratch[32] = (p2)), (*(volatile f32*)&scratch[32])) - ((scratch[12] = (v12)), (*(volatile f32*)&scratch[12]));
        (*(volatile f32*)&scratch[40]) = (tmp);
        tmpVec[2] = scratch[40];
        tmp = ((scratch[30] = (p0)), (*(volatile f32*)&scratch[30])) - ((scratch[10] = (v10)), (*(volatile f32*)&scratch[10]));
        (*(volatile f32*)&scratch[38]) = (tmp);
        tmpVec[0] = scratch[38];
        tmp = ((scratch[31] = (p1)), (*(volatile f32*)&scratch[31])) - ((scratch[11] = (v11)), (*(volatile f32*)&scratch[11]));
        (*(volatile f32*)&scratch[39]) = (tmp);
        tmpVec[1] = scratch[39];

        side = ((*(volatile f32*)&tmpVec[2]) * (((*(volatile f32*)&e01[0]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))) - ((*(volatile f32*)&e01[1]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))))) +
               ((*(volatile f32*)&tmpVec[0]) * (((*(volatile f32*)&e01[1]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34]))) - ((*(volatile f32*)&e01[2]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))))) +
               ((*(volatile f32*)&tmpVec[1]) * (((*(volatile f32*)&e01[2]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))) - ((*(volatile f32*)&e01[0]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34])))));
        (*(volatile f32*)&scratch[28]) = (side);
        sideBits = scratch[28];
        if ((f64)(*(volatile f32*)&sideBits) < double_0_802bf588) {
            return 0;
        }

        tmp = ((scratch[32] = (p2)), (*(volatile f32*)&scratch[32])) - ((scratch[18] = (v22)), (*(volatile f32*)&scratch[18]));
        (*(volatile f32*)&scratch[40]) = (tmp);
        tmpVec[2] = scratch[40];
        tmp = ((scratch[30] = (p0)), (*(volatile f32*)&scratch[30])) - ((scratch[16] = (v20)), (*(volatile f32*)&scratch[16]));
        (*(volatile f32*)&scratch[38]) = (tmp);
        tmpVec[0] = scratch[38];
        tmp = ((scratch[31] = (p1)), (*(volatile f32*)&scratch[31])) - ((scratch[17] = (v21)), (*(volatile f32*)&scratch[17]));
        (*(volatile f32*)&scratch[39]) = (tmp);
        tmpVec[1] = scratch[39];

        side = ((*(volatile f32*)&tmpVec[2]) * (((*(volatile f32*)&e12[0]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))) - ((*(volatile f32*)&e12[1]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))))) +
               ((*(volatile f32*)&tmpVec[0]) * (((*(volatile f32*)&e12[1]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34]))) - ((*(volatile f32*)&e12[2]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))))) +
               ((*(volatile f32*)&tmpVec[1]) * (((*(volatile f32*)&e12[2]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))) - ((*(volatile f32*)&e12[0]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34])))));
        (*(volatile f32*)&scratch[28]) = (side);
        sideBits = scratch[28];
        if ((f64)(*(volatile f32*)&sideBits) < double_0_802bf588) {
            return 0;
        }
    } else {
        if (double_0_802bf588 <= (f64)(*(volatile f32*)&denomBits)) {
            return 0;
        }

        tmp = ((scratch[32] = (p2)), (*(volatile f32*)&scratch[32])) - ((scratch[15] = (v02)), (*(volatile f32*)&scratch[15]));
        (*(volatile f32*)&scratch[40]) = (tmp);
        tmpVec[2] = scratch[40];
        tmp = ((scratch[30] = (p0)), (*(volatile f32*)&scratch[30])) - ((scratch[13] = (v00)), (*(volatile f32*)&scratch[13]));
        (*(volatile f32*)&scratch[38]) = (tmp);
        tmpVec[0] = scratch[38];
        tmp = ((scratch[31] = (p1)), (*(volatile f32*)&scratch[31])) - ((scratch[14] = (v01)), (*(volatile f32*)&scratch[14]));
        (*(volatile f32*)&scratch[39]) = (tmp);
        tmpVec[1] = scratch[39];

        side = ((*(volatile f32*)&tmpVec[2]) * (((*(volatile f32*)&e20[0]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))) - ((*(volatile f32*)&e20[1]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))))) +
               ((*(volatile f32*)&tmpVec[0]) * (((*(volatile f32*)&e20[1]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34]))) - ((*(volatile f32*)&e20[2]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))))) +
               ((*(volatile f32*)&tmpVec[1]) * (((*(volatile f32*)&e20[2]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))) - ((*(volatile f32*)&e20[0]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34])))));
        (*(volatile f32*)&scratch[28]) = (side);
        sideBits = scratch[28];
        if (double_0_802bf588 < (f64)(*(volatile f32*)&sideBits)) {
            return 0;
        }

        tmp = ((scratch[32] = (p2)), (*(volatile f32*)&scratch[32])) - ((scratch[12] = (v12)), (*(volatile f32*)&scratch[12]));
        (*(volatile f32*)&scratch[40]) = (tmp);
        tmpVec[2] = scratch[40];
        tmp = ((scratch[30] = (p0)), (*(volatile f32*)&scratch[30])) - ((scratch[10] = (v10)), (*(volatile f32*)&scratch[10]));
        (*(volatile f32*)&scratch[38]) = (tmp);
        tmpVec[0] = scratch[38];
        tmp = ((scratch[31] = (p1)), (*(volatile f32*)&scratch[31])) - ((scratch[11] = (v11)), (*(volatile f32*)&scratch[11]));
        (*(volatile f32*)&scratch[39]) = (tmp);
        tmpVec[1] = scratch[39];

        side = ((*(volatile f32*)&tmpVec[2]) * (((*(volatile f32*)&e01[0]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))) - ((*(volatile f32*)&e01[1]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))))) +
               ((*(volatile f32*)&tmpVec[0]) * (((*(volatile f32*)&e01[1]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34]))) - ((*(volatile f32*)&e01[2]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))))) +
               ((*(volatile f32*)&tmpVec[1]) * (((*(volatile f32*)&e01[2]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))) - ((*(volatile f32*)&e01[0]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34])))));
        (*(volatile f32*)&scratch[28]) = (side);
        sideBits = scratch[28];
        if (double_0_802bf588 < (f64)(*(volatile f32*)&sideBits)) {
            return 0;
        }

        tmp = ((scratch[32] = (p2)), (*(volatile f32*)&scratch[32])) - ((scratch[18] = (v22)), (*(volatile f32*)&scratch[18]));
        (*(volatile f32*)&scratch[40]) = (tmp);
        tmpVec[2] = scratch[40];
        tmp = ((scratch[30] = (p0)), (*(volatile f32*)&scratch[30])) - ((scratch[16] = (v20)), (*(volatile f32*)&scratch[16]));
        (*(volatile f32*)&scratch[38]) = (tmp);
        tmpVec[0] = scratch[38];
        tmp = ((scratch[31] = (p1)), (*(volatile f32*)&scratch[31])) - ((scratch[17] = (v21)), (*(volatile f32*)&scratch[17]));
        (*(volatile f32*)&scratch[39]) = (tmp);
        tmpVec[1] = scratch[39];

        side = ((*(volatile f32*)&tmpVec[2]) * (((*(volatile f32*)&e12[0]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))) - ((*(volatile f32*)&e12[1]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))))) +
               ((*(volatile f32*)&tmpVec[0]) * (((*(volatile f32*)&e12[1]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34]))) - ((*(volatile f32*)&e12[2]) * ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35]))))) +
               ((*(volatile f32*)&tmpVec[1]) * (((*(volatile f32*)&e12[2]) * ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33]))) - ((*(volatile f32*)&e12[0]) * ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34])))));
        (*(volatile f32*)&scratch[28]) = (side);
        sideBits = scratch[28];
        if (double_0_802bf588 < (f64)(*(volatile f32*)&sideBits)) {
            return 0;
        }
    }

    scale = -(*(volatile f32*)&distBits) / (*(volatile f32*)&denomBits);

    tmp = ((scratch[33] = (dir0)), (*(volatile f32*)&scratch[33])) * scale;
    (*(volatile f32*)&scratch[50]) = (((scratch[30] = (p0)), (*(volatile f32*)&scratch[30])) + tmp);
    hit[0] = scratch[50];

    tmp = ((scratch[34] = (dir1)), (*(volatile f32*)&scratch[34])) * scale;
    (*(volatile f32*)&scratch[51]) = (((scratch[31] = (p1)), (*(volatile f32*)&scratch[31])) + tmp);
    hit[1] = scratch[51];

    tmp = ((scratch[35] = (dir2)), (*(volatile f32*)&scratch[35])) * scale;
    (*(volatile f32*)&scratch[52]) = (((scratch[32] = (p2)), (*(volatile f32*)&scratch[32])) + tmp);
    hit[2] = scratch[52];

    oldY = *(f32*)e;
    hitY = *(volatile f32*)&hit[1];
    if (oldY <= hitY) {
        *(f32*)e = hitY;
    }

    return oldY <= hitY;
}


#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset



s32 collisionCurve(s32 type, void* road, void* pos, void* result) {
    extern f64 __frsqrte(f64 value);
    extern f64 __fabs(f64 value);
    extern f32 __float_nan[];
    f32* point = (f32*)pos;
    f32* out = (f32*)result;
    f32* vertices;
    f32* normals;
    s32 count;
    s32 roadMode;
    f32 start;
    f32 end;
    f32 limitA;
    f32 limitB;
    f32 best = dat_8041f688;
    s32 found = 0;
    s32 i;

    if (type != 0) {
        count = *(s32*)((s32)road + 0x64) / 2;
        start = 0.0f;
        end = 1.40129846e-45f;
        limitA = 0.0f;
        limitB = 0.0f;
        roadMode = 0;
        vertices = (f32*)(*(s32*)(*(s32*)wp + 0xF8) + *(s32*)((s32)road + 0x60) * 12);
    } else {
        count = *(s32*)((s32)road + 0xA4) / 2;
        start = *(f32*)((s32)road + 0x40);
        end = *(f32*)((s32)road + 0x44);
        limitA = *(f32*)((s32)road + 0x4C);
        limitB = *(f32*)((s32)road + 0x48);
        roadMode = *(s32*)((s32)road + 0x84);
        vertices = (f32*)(*(s32*)(*(s32*)wp + 0xF8) + *(s32*)((s32)road + 0xA0) * 12);
    }
    normals = vertices + count * 3;

    for (i = 0; i < count - 1; i++, vertices += 3, normals += 3) {
        f32 dx0 = point[0] - vertices[0];
        f32 dz0 = point[2] - vertices[2];
        f32 dx1 = point[0] - vertices[3];
        f32 dz1 = point[2] - vertices[5];
        f32 side0 = normals[0] * dx0 + normals[2] * dz0;
        f32 side1 = -(normals[3] * dx1 + normals[5] * dz1);

        if (side0 >= float_0_8041f62c && side1 >= float_0_8041f62c) {
            f32 edgeX = vertices[3] - vertices[0];
            f32 edgeZ = vertices[5] - vertices[2];
            f32 t;
            f32 interp;
            f32 hitX;
            f32 hitZ;
            f32 offX;
            f32 offZ;
            f32 distanceSq;
            f32 distance;
            f32 hitY;
            f32 clampFlag = float_0_8041f62c;

            if (__fabs((f64)-edgeZ) <= __fabs((f64)edgeX)) {
                if ((f64)edgeX == 0.0) {
                    t = float_0_8041f62c;
                } else {
                    t = (f32)((((f64)-edgeZ * (f64)((vertices[2] - point[2]) / edgeX) +
                                  (f64)point[0]) - (f64)vertices[0]) /
                                -(f64)((f32)((f64)-edgeZ * (f64)(edgeZ / edgeX)) - edgeX));
                }
            } else {
                t = (f32)((((f64)edgeX * (f64)((vertices[0] - point[0]) / -edgeZ) +
                              (f64)point[2]) - (f64)vertices[2]) /
                            -(f64)((f32)((f64)edgeX * (f64)(edgeX / -edgeZ)) - edgeZ));
            }
            hitX = vertices[0] + t * edgeX;
            hitZ = vertices[2] + t * edgeZ;
            if (__fabs((f64)edgeX) <= __fabs((f64)edgeZ)) {
                if ((f64)edgeZ != 0.0) interp = (hitZ - vertices[2]) / edgeZ;
                else interp = float_0_8041f62c;
            } else {
                interp = (hitX - vertices[0]) / edgeX;
            }
            offX = point[0] - hitX;
            offZ = point[2] - hitZ;
            distanceSq = offX * offX + offZ * offZ;

            if (distanceSq > float_0_8041f62c) {
                f64 value = (f64)distanceSq;
                f64 inv = __frsqrte(value);
                f64 half = *(f64*)(vec3_802bf540 + 0x38);
                f64 three = *(f64*)(vec3_802bf540 + 0x40);
                f64 square = inv * inv;
                inv = (half * inv) * (three - value * square);
                square = inv * inv;
                inv = (half * inv) * (three - value * square);
                distance = (f32)(value * half * inv * (three - value * inv * inv));
            } else if (distanceSq < float_0_8041f62c) {
                distance = __float_nan[0];
            } else {
                distance = float_0_8041f62c;
            }
            hitY = normals[1] + interp * (normals[4] - normals[1]);

            if (distance < best) {
                if (roadMode != 0) {
                    f32 lowY = normals[(s32)start * 3 + 1];
                    f32 highY = normals[(s32)end * 3 + 1];
                    if (hitY < lowY) {
                        hitY = lowY;
                        clampFlag = 1.40129846e-45f;
                    } else if (hitY > highY) {
                        hitY = highY;
                        clampFlag = 1.40129846e-45f;
                    }
                    if (offX * edgeX + offZ * edgeZ >= float_0_8041f62c) {
                        if (limitA < distance) distance = limitA;
                    } else if (limitB < distance) distance = limitB;
                }
                best = distance;
                found = 1;
                out[0] = hitY;
                out[2] = vertices[1];
                out[3] = hitX;
                out[4] = float_0_8041f62c;
                out[5] = hitZ;
                out[6] = normals[0] + interp * (normals[3] - normals[0]);
                out[7] = float_0_8041f62c;
                out[8] = normals[2] + interp * (normals[5] - normals[2]);
                out[9] = (f32)i;
                out[10] = clampFlag;
            }
        } else {
            f32 ax = point[0] - vertices[0];
            f32 az = point[2] - vertices[2];
            f32 bx = point[0] - vertices[3];
            f32 bz = point[2] - vertices[5];
            f32 distA2 = ax * ax + az * az;
            f32 distB2 = bx * bx + bz * bz;
            f32 distance;
            f32 endpoint;
            f32 invLen;

            if (distA2 <= distB2) {
                f64 value = (f64)distA2;
                endpoint = float_0_8041f62c;
                if (value > 0.0) {
                    f64 inv = __frsqrte(value);
                    f64 half = *(f64*)(vec3_802bf540 + 0x38);
                    f64 three = *(f64*)(vec3_802bf540 + 0x40);
                    inv = half * inv * (three - value * inv * inv);
                    inv = half * inv * (three - value * inv * inv);
                    distance = (f32)(value * half * inv * (three - value * inv * inv));
                } else distance = float_0_8041f62c;
            } else {
                f64 value = (f64)distB2;
                endpoint = float_1_8041f638;
                if (value > 0.0) {
                    f64 inv = __frsqrte(value);
                    f64 half = *(f64*)(vec3_802bf540 + 0x38);
                    f64 three = *(f64*)(vec3_802bf540 + 0x40);
                    inv = half * inv * (three - value * inv * inv);
                    inv = half * inv * (three - value * inv * inv);
                    distance = (f32)(value * half * inv * (three - value * inv * inv));
                } else distance = float_0_8041f62c;
            }

            if (distance < best) {
                invLen = distance != float_0_8041f62c ? float_1_8041f638 / distance : float_0_8041f62c;
                best = distance;
                found = 1;
                out[0] = normals[1] + endpoint * (normals[4] - normals[1]);
                out[2] = vertices[1];
                out[3] = point[0] - (endpoint == float_0_8041f62c ? ax : bx);
                out[4] = float_0_8041f62c;
                out[5] = point[2] - (endpoint == float_0_8041f62c ? az : bz);
                out[6] = (endpoint == float_0_8041f62c ? ax : bx) * invLen;
                out[7] = float_0_8041f62c;
                out[8] = (endpoint == float_0_8041f62c ? az : bz) * invLen;
                out[9] = (f32)i;
                out[10] = 4.20389539e-45f;
            }
        }
    }
    return found;
}

void calcCamRoad(f32 x, f32 y, f32 z, f32 param4, f32 param5, f32 param6, f32 checkWidth, void* out, void* prev) {
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

    for (i = 0; i < *(s32*)((s32)work0 + 0xC8); i++, road = (void*)((s32)road + 0xB8)) {
        roadEntry = (void*)((s32)*(void**)((s32)work0 + 0xF0) + (*(s32*)((s32)road + 0xA8) * 0x68));
        blockCount = *(s32*)((s32)road + 0xAC);

        if ((*(u32*)((s32)work0 + 0x1D0) == 0x65) && (*(u32*)((s32)road + 0x28) != 0)) {
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
            vertexBase = (void*)((s32)*(void**)((s32)work0 + 0xFC) + (*(s32*)((s32)roadEntry + 0x58) * 0xC));
            triBase = (void*)((s32)*(void**)((s32)work0 + 0xF4) + (*(s32*)((s32)roadEntry + 0x60) * 8));
            indexBase = *(void**)((s32)work0 + 0x100);
            polyCount = *(s32*)((s32)roadEntry + 0x64);

            for (k = 0; k < polyCount; k++, triBase = (void*)((s32)triBase + 8)) {
                s32 firstIndex = *(s32*)triBase;
                indexCount = *(s32*)((s32)triBase + 4) - 1;

                firstIndexPtr = (s32*)((s32)indexBase + (firstIndex * 4));
                walkIndexPtr = firstIndexPtr + 1;

                for (tri = 1; tri < indexCount; tri++, walkIndexPtr++) {
                idx2 = walkIndexPtr[1];
                idx1 = walkIndexPtr[0];
                idx0 = firstIndexPtr[0];

                    p3 = *(Vec3*)((s32)vertexBase + (idx2 * 0xC));
                    p2 = *(Vec3*)((s32)vertexBase + (idx1 * 0xC));
                    p1 = *(Vec3*)((s32)vertexBase + (idx0 * 0xC));
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
        hit.bestY = float_neg1E06_8041f680;
        hit.nx = float_0_8041f62c;
        hit.ny = float_0_8041f62c;
        hit.nz = float_0_8041f62c;

        if (collisionCurve(0, foundRoad, &pos, &hit) == 0) {
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
                half = *(f64*)(vec3_802bf540 + 0x38);
                three = *(f64*)(vec3_802bf540 + 0x40);

                square = inv * inv;
                inv = (half * inv) * (three - ((f64)distSq * square));
                square = inv * inv;
                inv = (half * inv) * (three - ((f64)distSq * square));
                square = inv * inv;
                inv = (half * inv) * (three - ((f64)distSq * square));

                dist = (f32)((f64)distSq * inv);
                dx = dx / dist;
                dz = dz / dist;
                dirY = float_0_8041f62c;
            }
        }

        dirX = dx * float_neg1_8041f684;
        dirY = dirY * float_neg1_8041f684;
        dirZ = dz * float_neg1_8041f684;
    }

    if (*(s32*)((s32)chosenRoad + 0xB4) != 0) {
        Vec3 curvePos;

        curvePos.x = outX;
        curvePos.y = dat_8041f688;
        curvePos.z = outZ;
        hit.bestY = float_neg1E06_8041f680;

        if (collisionCurve(1,
                           (void*)((s32)*(void**)((s32)*(void**)wp + 0x104) +
                                  (*(s32*)((s32)chosenRoad + 0xB0) * 0x68)),
                           &curvePos,
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

        sidePosA.x = x + (offset * sideX);
        sidePosA.y = float_1E06_8041f694;
        sidePosA.z = z + (offset * dirX);

        hitAny = collisionCurve(0, chosenRoad, &sidePosA, &sideHitA);
        if (hitAny != 0) {
            sidePosA.x = sideHitA.x;
            sidePosA.y = outY;
            sidePosA.z = sideHitA.z;
        }

        if (*(s32*)((s32)chosenRoad + 0xB4) != 0) {
            sideHitA.bestY = float_neg1E06_8041f680;
            if (collisionCurve(1,
                               (void*)((s32)*(void**)((s32)*(void**)wp + 0x104) +
                                      (*(s32*)((s32)chosenRoad + 0xB0) * 0x68)),
                               &sidePosA,
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

        sidePosB.x = x + (offset * sideX);
        sidePosB.y = float_1E06_8041f694;
        sidePosB.z = z + (offset * dirX);

        active = collisionCurve(0, chosenRoad, &sidePosB, &sideHitB);
        flags = 0;
        if (active != 0) {
            sidePosB.x = sideHitB.x;
            sidePosB.y = outY;
            sidePosB.z = sideHitB.z;
            flags = sideHitB.flags;
        }

        if (*(s32*)((s32)chosenRoad + 0xB4) != 0) {
            sideHitB.bestY = float_neg1E06_8041f680;
            if (collisionCurve(1,
                               (void*)((s32)*(void**)((s32)*(void**)wp + 0x104) +
                                      (*(s32*)((s32)chosenRoad + 0xB0) * 0x68)),
                               &sidePosB,
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

