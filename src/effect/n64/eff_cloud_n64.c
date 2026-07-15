#include "effect/n64/eff_cloud_n64.h"


u8 effCloudDisp(s32 camId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void DCFlushRange(void*, u32);
    extern void* smartAlloc(u32, s32);
    extern void* memset(void*, s32, u32);
    extern void GXBegin(s32, s32, s16);
    u32* effect = (u32*)effectAddress;
    u8* work = (u8*)effect[3];
    char* cam = camGetPtr(camId);
    GXTexObj tex;
    f32* vertices;
    s32 i;
    s32 strip;

    effGetTexObj(0x83, &tex);
    GXLoadTexObj(&tex, 0);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOrder(1, 0, 0, 4);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 2);
    GXSetVtxDesc(11, 2);
    GXSetVtxDesc(13, 2);
    GXLoadPosMtxImm((f32(*)[4])(cam + 0x118), 0);
    GXSetCurrentMtx(0);
    vertices = smartAlloc(0x690, 0);
    memset(vertices, 0, 0x690);
    for (i = 0; i < 30; i++) {
        s32 next = (i + *(s32*)(work + 0x2A8)) % 30;
        f32 x = ((f32*)(work + 0x50))[next];
        f32 y = ((f32*)(work + 0xC8))[next];
        f32 z = ((f32*)(work + 0x140))[next];
        f32 width = *(f32*)(work + 0x18) * (1.0f - (f32)i / 30.0f);
        vertices[i * 6 + 0] = x - width;
        vertices[i * 6 + 1] = y;
        vertices[i * 6 + 2] = z;
        vertices[i * 6 + 3] = x + width;
        vertices[i * 6 + 4] = y;
        vertices[i * 6 + 5] = z;
    }
    DCFlushRange(vertices, 0x690);
    for (strip = 0; strip < 29; strip++) {
        GXBegin(0x90, 0, 6);
        *(volatile u16*)0xCC008000 = strip * 2;
        *(volatile u16*)0xCC008000 = strip * 2 + 1;
        *(volatile u16*)0xCC008000 = strip * 2 + 2;
        *(volatile u16*)0xCC008000 = strip * 2 + 1;
        *(volatile u16*)0xCC008000 = strip * 2 + 3;
        *(volatile u16*)0xCC008000 = strip * 2 + 2;
    }
    return 0;
}

#pragma optimize_for_size off

u8 effCloudMain(u32* effect) {
    extern void effDelete(void* effect);
    extern s32 rand(void);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 prio, s32 layer, void* callback, void* arg, f32 z);
    extern u8 effCloudDisp(void);
    extern f32 sqrtf(f32 x);
    extern f32 float_0_80424f18;
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

#pragma optimize_for_size off

void* effCloudN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effCloudMain(void*);
    extern s32 rand(void);
    extern char str_CloudN64_802fad30[];
    extern f32 float_0_80424f18;
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

