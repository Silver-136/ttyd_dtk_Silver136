#include "effect/n64/eff_cloud_n64.h"

extern s32 rand(void);
extern const f32 float_0_80424f18;

#pragma optimize_for_size off

void* effCloudN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effCloudMain(void*);
    extern char str_CloudN64_802fad30[];
    void* entry;
    u8* work;
    s32 i;
    s32 r;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_CloudN64_802fad30;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x324);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effCloudMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (timer <= 0) {
        *(s32*)(work + 0x10) = 1000;
    } else {
        *(s32*)(work + 0x10) = timer;
    }
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x34) = scale;

    switch (type) {
        case 0:
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0xFF;
            *(s32*)(work + 0x20) = 0xFF;
            *(s32*)(work + 0x28) = 0xE1;
            *(s32*)(work + 0x2C) = 0xE1;
            *(s32*)(work + 0x30) = 0xE1;
            *(s32*)(work + 0x24) = 0xFF;
            break;
        case 1:
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0xF5;
            *(s32*)(work + 0x20) = 0xE6;
            *(s32*)(work + 0x28) = 0x2D;
            *(s32*)(work + 0x2C) = 0x23;
            *(s32*)(work + 0x30) = 0xF;
            *(s32*)(work + 0x24) = 0xFF;
            break;
        case 2:
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0;
            *(s32*)(work + 0x20) = 0;
            *(s32*)(work + 0x28) = 0x9B;
            *(s32*)(work + 0x2C) = 0x7D;
            *(s32*)(work + 0x30) = 0x7D;
            *(s32*)(work + 0x24) = 0xFF;
            break;
        case 3:
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0xFF;
            *(s32*)(work + 0x20) = 0xFF;
            *(s32*)(work + 0x28) = 0xFF;
            *(s32*)(work + 0x2C) = 0x41;
            *(s32*)(work + 0x30) = 0x14;
            *(s32*)(work + 0x24) = 0xFF;
            break;
    }

    *(f32*)(work + 0x38) = x;
    *(f32*)(work + 0x3C) = y;
    *(f32*)(work + 0x40) = z;
    for (i = 0; i < 0x7C; i += 4) {
        *(s32*)(work + 0x230 + i) = 0;
    }
    *(f32*)(work + 0x50) = x;
    *(f32*)(work + 0xC8) = y;
    *(f32*)(work + 0x140) = z;
    *(s32*)(work + 0x230) = 1;
    *(s32*)(work + 0x1B8) = 0;
    *(f32*)(work + 0x2AC) = float_0_80424f18;
    r = rand();
    *(f32*)(work + 0x44) = (f32)(r % 30 + 10);
    *(f32*)(work + 0x48) = float_0_80424f18;
    *(f32*)(work + 0x4C) = float_0_80424f18;

    return entry;
}

#pragma optimize_for_size on


#pragma optimize_for_size off

u8 effCloudMain(u32* effect) {
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 prio, s32 layer, void* callback, void* arg, f32 z);
    extern u8 effCloudDisp(void);
    extern f32 sqrtf(f32 x);
    extern u32 vec3_802fad00[];

    u8* work = (u8*)effect[3];
    u32 v0;
    u32 v1;
    u32 v2;
    f32 dispPos[3];
    f32 calcPos[3];
    s32 type;
    s32 timer;
    s32 frame;
    s32 idx;
    s32 next;
    f32 x;
    f32 y;
    f32 z;
    f32 oldX;
    f32 oldY;
    f32 oldZ;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distSq;
    f32 dist;
    f32 oldDist;

    v0 = vec3_802fad00[0];
    v1 = vec3_802fad00[1];
    v2 = vec3_802fad00[2];

    *(u32*)&dispPos[0] = v0;
    dispPos[0] = *(f32*)(work + 0x04);
    *(u32*)&dispPos[1] = v1;
    dispPos[1] = *(f32*)(work + 0x08);
    *(u32*)&dispPos[2] = v2;
    dispPos[2] = *(f32*)(work + 0x0C);

    *(u32*)&calcPos[0] = *(u32*)&dispPos[0];
    *(u32*)&calcPos[1] = *(u32*)&dispPos[1];
    *(u32*)&calcPos[2] = *(u32*)&dispPos[2];

    type = *(s32*)(work + 0x00);

    if ((*effect & 4) != 0) {
        *effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }

    timer = *(s32*)(work + 0x10);
    if (timer < 1000) {
        timer--;
        *(s32*)(work + 0x10) = timer;
    }

    frame = *(s32*)(work + 0x14) + 1;
    *(s32*)(work + 0x14) = frame;

    timer = *(s32*)(work + 0x10);
    if (timer < 0) {
        effDelete(effect);
        return 0;
    }

    if (timer < 0x10) {
        *(s32*)(work + 0x24) = timer << 4;
    }

    if (type == 3) {
        *(f32*)(work + 0x44) += (f32)((rand() % 10) - 5);
        *(f32*)(work + 0x48) += (f32)((rand() % 10) - 5);

        *(f32*)(work + 0x38) += *(f32*)(work + 0x44);
        *(f32*)(work + 0x3C) += *(f32*)(work + 0x48);
        *(f32*)(work + 0x40) += *(f32*)(work + 0x4C);
    }

    x = *(f32*)(work + 0x38);
    y = *(f32*)(work + 0x3C);
    z = *(f32*)(work + 0x40);

    idx = *(s32*)(work + 0x2A8) % 30;
    oldX = ((f32*)(work + 0x50))[idx];
    oldY = ((f32*)(work + 0xC8))[idx];
    oldZ = ((f32*)(work + 0x140))[idx];

    if (!(x == oldX && y == oldY && z == oldZ)) {
        oldDist = ((f32*)(work + 0x2AC))[idx];

        next = *(s32*)(work + 0x2A8) + 1;
        *(s32*)(work + 0x2A8) = next;
        if (*(s32*)(work + 0x2A8) >= 30) {
            *(s32*)(work + 0x2A8) = 0;
        }

        next = *(s32*)(work + 0x2A8);
        ((s32*)(work + 0x230))[next] = 1;
        ((f32*)(work + 0x50))[next] = x;
        ((f32*)(work + 0xC8))[next] = y;
        ((f32*)(work + 0x140))[next] = z;
        ((s32*)(work + 0x1B8))[next] = frame;

        dx = x - oldX;
        dy = y - oldY;
        dz = z - oldZ;
        distSq = dx * dx + dy * dy + dz * dz;

        if (distSq == float_0_80424f18) {
            dist = float_0_80424f18;
        } else {
            dist = sqrtf(distSq);
        }

        ((f32*)(work + 0x2AC))[next] = oldDist + dist;
    }

    dispEntry(4, 2, effCloudDisp, effect, dispCalcZ(calcPos));
    return 0;
}

#pragma optimize_for_size on



void effCloudDisp(int cameraId, int effectAddress) {
    typedef float Mtx[3][4];
    typedef struct SmartAllocationData { void* pMemory; } SmartAllocationData;
    typedef struct CloudRibbonVertex {
        s16 x;
        s16 z;
        s16 y;
        s16 s;
        s16 t;
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } CloudRibbonVertex;
    extern void* camGetPtr(int);
    extern void GXSetNumChans(int);
    extern void GXSetChanCtrl(int, int, int, int, int, int, int);
    extern void GXSetNumTevStages(int);
    extern void GXSetTevOrder(int, int, int, int);
    extern void GXSetTevColorOp(int, int, int, int, int, int);
    extern void GXSetTevAlphaOp(int, int, int, int, int, int);
    extern void GXSetTevColorIn(int, int, int, int, int);
    extern void GXSetTevAlphaIn(int, int, int, int, int);
    extern void GXSetNumTexGens(int);
    extern void GXSetTexCoordGen2(int, int, int, int, int, int);
    extern void PSMTXScale(Mtx, float, float, float);
    extern void GXLoadTexMtxImm(Mtx, int, int);
    extern void effGetTexObjN64(int, void*);
    extern void GXLoadTexObj(void*, int);
    extern void GXSetTevColor(int, void*);
    extern void GXLoadPosMtxImm(Mtx, int);
    extern void GXSetCurrentMtx(int);
    extern void GXSetCullMode(int);
    extern SmartAllocationData* smartAlloc(unsigned int, int);
    extern void* memset(void*, int, unsigned int);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(int, int, int);
    extern void DCFlushRange(void*, u32);
    extern void GXInvalidateVtxCache(void);
    extern void tri2(s16, s16, s16, s16, s16, s16, s16);
    extern const float float_0p015625_80424f10;
    extern const float float_0p03125_80424f14;
    extern const float float_0_80424f18;
    extern const float float_6p2832_80424f1c;
    extern const float float_360_80424f20;
    extern const float float_255_80424f24;
    extern const float float_180_80424f28;
    extern const float float_neg180_80424f2c;
    extern const float float_0p5_80424f30;
    extern const float float_16_80424f34;
    extern const float float_3_80424f38;
    extern const float float_24_80424f3c;
    extern const float float_0p8_80424f40;
    extern const float float_0p4_80424f44;
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern unsigned char unk_8042966c;
    unsigned char texObj[0x20];
    Mtx texMtx;
    unsigned char* entry = (unsigned char*)effectAddress;
    unsigned char* work = *(unsigned char**)(entry + 0xC);
    unsigned char* camera = (unsigned char*)camGetPtr(cameraId);
    SmartAllocationData* allocation;
    CloudRibbonVertex* vertices;
    unsigned int color1;
    unsigned int color2;
    int type = *(int*)work;
    int frame = *(int*)(work + 0x14);
    int alpha = *(int*)(work + 0x24);
    float scale = *(float*)(work + 0x34);
    int i;
    int strip;
    int active;

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    if (type >= 2) {
        GXSetNumTevStages(2);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 2, 4, 8, 15);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(1, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(1, 15, 15, 15, 0);
        GXSetTevAlphaIn(1, 7, 0, 1, 7);
    } else {
        GXSetNumTevStages(2);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 2, 10, 8, 15);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(1, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(1, 15, 15, 15, 0);
        GXSetTevAlphaIn(1, 7, 0, 1, 7);
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_0p015625_80424f10, float_0p03125_80424f14, float_0_80424f18);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    effGetTexObjN64(0x83, texObj);
    GXLoadTexObj(texObj, 0);

    color1 = ((unsigned int)*(unsigned char*)(work + 0x18) << 24) |
             ((unsigned int)*(unsigned char*)(work + 0x1C) << 16) |
             ((unsigned int)*(unsigned char*)(work + 0x20) << 8) |
             (unsigned char)alpha;
    color2 = ((unsigned int)*(unsigned char*)(work + 0x28) << 24) |
             ((unsigned int)*(unsigned char*)(work + 0x2C) << 16) |
             ((unsigned int)*(unsigned char*)(work + 0x30) << 8) |
             unk_8042966c;
    GXSetTevColor(1, &color1);
    GXSetTevColor(2, &color2);
    GXLoadPosMtxImm((float (*)[4])(camera + 0x11C), 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    allocation = smartAlloc(0x690, 3);
    vertices = (CloudRibbonVertex*)allocation->pMemory;
    memset(vertices, 0, 0x690);
    effSetVtxDescN64(vertices);

    active = 0;
    i = 0;
    do {
        int next = (i + *(int*)(work + 0x2A8) + 1) % 30;
        if (((int*)(work + 0x230))[next] != 0) active++;
        next = (i + *(int*)(work + 0x2A8) + 2) % 30;
        if (((int*)(work + 0x230))[next] != 0) active++;
        next = (i + *(int*)(work + 0x2A8) + 3) % 30;
        if (((int*)(work + 0x230))[next] != 0) active++;
        i += 3;
    } while (i < 30);
    if (active > 1) {
        int first = -1;
        int alphaProgress = 0;
        int vertexIndex;
        float angle;
        CloudRibbonVertex* out = vertices;

        for (i = 0; i < 30; i++, out += 2) {
            int next = (i + *(int*)(work + 0x2A8) + 1) % 30;
            int alphaAngle = alphaProgress / (active - 1);
            int alpha2 = (int)(float_255_80424f24 *
                               (float)sin((float_6p2832_80424f1c *
                                          (float)alphaAngle) /
                                         float_360_80424f20));

            if (alpha2 > 200) {
                alpha2 = 200;
            }

            if (((int*)(work + 0x230))[next] != 0) {
                int ahead;
                int behind;
                int stamp;
                int age;
                int phase;
                int fade;
                int edge;
                int center;
                s16 texS;
                float x;
                float z;
                float y;
                float radius;
                float radians;
                float sideX;
                float sideZ;

                if (first == -1) {
                    ahead = next + 1;
                    if (ahead >= 30) {
                        ahead = 0;
                    }
                    angle = -(float)angleABf(
                        ((float*)(work + 0xC8))[ahead],
                        -((float*)(work + 0x50))[ahead],
                        ((float*)(work + 0xC8))[next],
                        -((float*)(work + 0x50))[next]);
                    first = i;
                } else if (i != 29) {
                    float forwardAngle;
                    float backwardAngle;
                    float delta;

                    ahead = next + 1;
                    behind = next - 1;
                    if (ahead >= 30) {
                        ahead = 0;
                    }
                    if (behind < 0) {
                        behind = 29;
                    }

                    forwardAngle = -(float)angleABf(
                        ((float*)(work + 0xC8))[ahead],
                        -((float*)(work + 0x50))[ahead],
                        ((float*)(work + 0xC8))[next],
                        -((float*)(work + 0x50))[next]);
                    backwardAngle = -(float)angleABf(
                        ((float*)(work + 0xC8))[next],
                        -((float*)(work + 0x50))[next],
                        ((float*)(work + 0xC8))[behind],
                        -((float*)(work + 0x50))[behind]);

                    delta = backwardAngle - forwardAngle;
                    if (delta > float_180_80424f28) {
                        forwardAngle += float_360_80424f20;
                    } else if (delta < float_neg180_80424f2c) {
                        backwardAngle += float_360_80424f20;
                    }
                    angle = (forwardAngle + backwardAngle) * float_0p5_80424f30;
                }

                stamp = ((int*)(work + 0x1B8))[next];
                age = frame - stamp;
                phase = (frame - stamp * 80) * 4;
                x = ((float*)(work + 0x50))[next];
                z = ((float*)(work + 0xC8))[next];
                y = ((float*)(work + 0x140))[next];

                radius = ((float)age +
                          float_3_80424f38 *
                              (float)sin((float_6p2832_80424f1c *
                                          (float)phase) /
                                         float_360_80424f20) +
                          float_16_80424f34) *
                         scale;

                texS = (s16)((int)(float_24_80424f3c *
                                   ((float*)(work + 0x2AC))[next]) +
                             ((frame << 5) & 0x7E0));

                fade = 255 - age * 100;
                center = 255;
                edge = 255;
                if (fade < 0) {
                    int oldFade = fade;
                    fade = 0;
                    edge = (int)(float_0p8_80424f40 * (float)oldFade) + 255;
                    if (edge < 0) {
                        int oldEdge = edge;
                        edge = 0;
                        center = (int)(float_0p4_80424f44 * (float)oldEdge) + 255;
                        if (center < 0) {
                            center = 0;
                        }
                    }
                }

                radians = (float_6p2832_80424f1c * angle) /
                          float_360_80424f20;
                sideX = radius * (float)sin(radians);
                sideZ = radius * (float)cos(radians);
                alphaProgress += 180;

                out[0].x = (s16)(x + sideX);
                out[0].z = (s16)(z + sideZ);
                out[0].y = (s16)(y + float_0_80424f18);
                out[0].s = texS;
                out[0].t = 0x400;
                out[0].r = (u8)center;
                out[0].g = (u8)edge;
                out[0].b = (u8)fade;
                out[0].a = (u8)alpha2;

                out[1].x = (s16)(x - sideX);
                out[1].z = (s16)(z - sideZ);
                out[1].y = (s16)(y + float_0_80424f18);
                out[1].s = texS;
                out[1].t = 0;
                out[1].r = (u8)center;
                out[1].g = (u8)edge;
                out[1].b = (u8)fade;
                out[1].a = (u8)alpha2;
            }
        }

        DCFlushRange(vertices, 0x690);
        GXInvalidateVtxCache();

        vertexIndex = first << 1;
        for (strip = first; strip < 29; strip++, vertexIndex += 2) {
            GXBegin(0x90, 0, 6);
            tri2((s16)vertexIndex, (s16)(vertexIndex + 2),
                 (s16)(vertexIndex + 1), vertexIndex,
                 (s16)(vertexIndex + 1), (s16)(vertexIndex + 2),
                 (s16)(vertexIndex + 3));
        }
    }
}

const f32 float_0p015625_80424f10 = 0.015625f;
const f32 float_0p03125_80424f14 = 0.03125f;
const f32 float_0_80424f18 = 0.0f;
const f32 float_6p2832_80424f1c = 6.2832f;
const f32 float_360_80424f20 = 360.0f;
const f32 float_255_80424f24 = 255.0f;
const f32 float_180_80424f28 = 180.0f;
const f32 float_neg180_80424f2c = -180.0f;
const f32 float_0p5_80424f30 = 0.5f;
const f32 float_16_80424f34 = 16.0f;
const f32 float_3_80424f38 = 3.0f;
const f32 float_24_80424f3c = 24.0f;
const f32 float_0p8_80424f40 = 0.8f;
const f32 float_0p4_80424f44 = 0.4f;
