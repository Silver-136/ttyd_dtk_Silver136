#include "effect/n64/eff_timestamp_n64.h"


u8 effTimestampDisp(void) {
    return 0;
}


void effTimestampMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effTimestampDisp(void);
    extern const Vec3 vec3_802fc1e0;
    extern f32 float_0_80426438;
    extern f32 float_0p05_80426450;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 timer;
    s32 frame;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fc1e0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x40;
    }
    if (*(s32*)(work + 0x10) < 1000) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    timer = *(s32*)(work + 0x10);
    frame = *(s32*)(work + 0x14);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (timer < 0x40) {
        *(f32*)(work + 0x30) -= float_0p05_80426450;
        if (*(f32*)(work + 0x30) < float_0_80426438) {
            *(f32*)(work + 0x30) = float_0_80426438;
        }
    }
    if (timer < 0x20) {
        *(s32*)(work + 0x2C) += 0x80;
        if (*(s32*)(work + 0x2C) > 0xFF) {
            *(s32*)(work + 0x2C) = 0xFF;
        }
        *(f32*)(work + 0x30) = float_0_80426438;
    }
    if (timer < 0x10) {
        *(s32*)(work + 0x24) = timer << 4;
    }
    if (frame < 0x10) {
        *(s32*)(work + 0x24) = (frame << 4) + 0xF;
    }
    part = work;
    for (i = 0; i < 5; i++, part += 8) {
        if (*(s32*)(part + 0x14C) != 0) {
            *(s32*)(part + 0x14C) -= 1;
            if (*(s32*)(part + 0x14C) != 0) {
                goto second;
            }
        }
        *(s32*)(part + 0x124) += 8;
        if (*(s32*)(part + 0x124) > 0xFF) {
            *(s32*)(part + 0x124) = 0xFF;
        }
second:
        if (*(s32*)(part + 0x150) != 0) {
            *(s32*)(part + 0x150) -= 1;
            if (*(s32*)(part + 0x150) != 0) {
                continue;
            }
        }
        *(s32*)(part + 0x128) += 8;
        if (*(s32*)(part + 0x128) > 0xFF) {
            *(s32*)(part + 0x128) = 0xFF;
        }
    }
    dispEntry(4, 2, effTimestampDisp, effect, dispCalcZ(&dispPos));
}

void* effTimestampN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effTimestampMain(void*);
    extern char str_TimestampN64_802fc200[];
    extern s32 pos_data[];
    extern f32 float_1_80426454;
    extern f32 float_0_80426438;
    void* entry;
    u8* work;
    s32 i;
    s32* data;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_TimestampN64_802fc200;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x174);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effTimestampMain;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (lifetime <= 0) {
        *(s32*)(work + 0x10) = 1000;
    } else {
        *(s32*)(work + 0x10) = lifetime;
    }
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x28) = scale;
    *(s32*)(work + 0x18) = 0x46;
    *(s32*)(work + 0x1C) = 0xB4;
    *(s32*)(work + 0x20) = 0x78;
    *(s32*)(work + 0x2C) = 0;
    *(f32*)(work + 0x30) = float_1_80426454;
    data = pos_data;
    for (i = 0; i < 5; i++, data += 6, work += 8) {
        *(f32*)(work + 0x34) = (f32)data[0];
        *(f32*)(work + 0x5C) = (f32)data[1];
        *(f32*)(work + 0x84) = float_0_80426438;
        *(f32*)(work + 0xAC) = float_0_80426438;
        *(f32*)(work + 0xD4) = float_0_80426438;
        *(f32*)(work + 0xFC) = float_0_80426438;
        *(s32*)(work + 0x124) = 0;
        *(s32*)(work + 0x14C) = data[2];
        *(f32*)(work + 0x38) = (f32)data[3];
        *(f32*)(work + 0x60) = (f32)data[4];
        *(f32*)(work + 0x88) = float_0_80426438;
        *(f32*)(work + 0xB0) = float_0_80426438;
        *(f32*)(work + 0xD8) = float_0_80426438;
        *(f32*)(work + 0x100) = float_0_80426438;
        *(s32*)(work + 0x128) = 0;
        *(s32*)(work + 0x150) = data[5];
    }
    return entry;
}

