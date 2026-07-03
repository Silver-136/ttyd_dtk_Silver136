#include "effect/n64/eff_bomb_n64.h"

void effBombN64SetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x40) = camId;
}


u8 effBombDisp(int param_1, void* effEntry) {
    return 0;
}


void effBombMain(void* effEntry) {
    extern void effDelete(void* effect);
    extern void* effKemuri8N64Entry(s32 type, f32 x, f32 y, f32 z);
    extern void effKemuri8N64SetCamId(void* effect, s32 camId);
    extern void* effConfettiN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 scale);
    extern void effConfettiN64SetCamId(void* effect, s32 camId);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 camId, s32 layer, void* callback, void* param, f32 z);
    extern void effBombDisp(void);
    extern f32 float_1_80424d5c;
    extern f32 float_0p4_80424d60;
    extern f32 float_0p6_80424d64;
    extern f32 float_255_80424d68;
    extern f32 float_0p9_80424d6c;
    extern f32 float_0p1_80424d70;
    extern f32 float_0p5_80424d74;
    f32 pos[3];
    void* spawned;
    u8* work;
    s32 frame;
    s32 type;
    s32 camId;

    work = *(u8**)((s32)effEntry + 0xC);
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);

    camId = *(s32*)(work + 0x40);
    *(s32*)(work + 0x30) = *(s32*)(work + 0x30) - 1;
    if (*(s32*)(work + 0x30) < 0) {
        effDelete(effEntry);
        return;
    }

    *(s32*)(work + 0x34) = *(s32*)(work + 0x34) + 1;
    frame = *(s32*)(work + 0x34);
    type = *(s32*)work;

    if (frame == 7) {
        if (*(s32*)(work + 0x3C) == 0) {
            spawned = effKemuri8N64Entry(type, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
        } else {
            spawned = effKemuri8N64Entry(type + 3, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
        }
        effKemuri8N64SetCamId(spawned, camId);
    } else if (frame == 1) {
        spawned = effConfettiN64Entry(type + 4, 0x32, *(f32*)(work + 4), *(f32*)(work + 8),
                                      *(f32*)(work + 0xC), float_1_80424d5c);
        effConfettiN64SetCamId(spawned, camId);
    }

    *(f32*)(work + 0x64) += *(f32*)(work + 0x68);
    if (frame < 8) {
        if (type == 2) {
            *(f32*)(work + 0x68) += float_1_80424d5c;
        } else {
            *(f32*)(work + 0x68) += float_0p4_80424d60;
        }
        *(s32*)(work + 0x7C) =
            (s32)((float_255_80424d68 - (f32)*(s32*)(work + 0x7C)) * float_0p6_80424d64);
    } else {
        *(f32*)(work + 0x68) *= float_0p6_80424d64;
        *(s32*)(work + 0x7C) = (s32)((f32)*(s32*)(work + 0x7C) * float_0p9_80424d6c);
    }

    if (frame >= 3) {
        if (frame == 3) {
            *(f32*)(work + 0xA8) = float_1_80424d5c;
        }
        *(f32*)(work + 0xA8) += *(f32*)(work + 0xAC);
    }

    if (frame >= 3) {
        if (frame < 6) {
            *(f32*)(work + 0xAC) += float_0p1_80424d70;
        } else {
            *(f32*)(work + 0xAC) *= float_0p5_80424d74;
            *(s32*)(work + 0xC0) = (s32)((f32)*(s32*)(work + 0xC0) * float_0p9_80424d6c);
        }
    }

    *(f32*)(work + 0xB0) += *(f32*)(work + 0xB4);
    dispEntry(camId, 2, effBombDisp, effEntry, dispCalcZ(pos));
}

void* effBombN64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effBombMain(void);
    extern char str_BombN64_802fabd0[];
    extern f32 float_0_80424d58;
    extern f32 float_1_80424d5c;
    extern f32 float_3_80424d78;
    extern f32 float_9p9_80424d7c;
    extern f32 float_1p6_80424d80;
    extern f32 float_13p9_80424d84;
    extern f32 float_2p4_80424d88;
    extern f32 float_17p8_80424d8c;
    extern f32 float_3p2_80424d90;
    extern f32 float_30_80424d94;
    void* entry;
    u8* work;
    s32 big;

    big = 0;
    if (type >= 3) {
        big = 1;
        type -= 3;
    }

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_BombN64_802fabd0;
    *(s32*)((s32)entry + 8) = 3;
    work = __memAlloc(3, 0xCC);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBombMain;

    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x30) = 0x3C;
    *(s32*)work = type;
    *(s32*)(work + 0x3C) = big;
    *(s32*)(work + 0x40) = 4;
    *(s32*)(work + 0x44) = (type != 0) ? type : -1;
    *(s32*)(work + 0x7C) = 0;
    if (type == 1) {
        *(f32*)(work + 0x64) = float_1_80424d5c;
        *(f32*)(work + 0x68) = float_1_80424d5c;
    } else {
        *(f32*)(work + 0x64) = float_3_80424d78;
        *(f32*)(work + 0x68) = float_3_80424d78;
    }
    *(s32*)(work + 0x88) = type;
    *(s32*)(work + 0xC0) = 0xFF;

    switch (type) {
        case 0:
            *(f32*)(work + 0xA8) = float_9p9_80424d7c;
            *(f32*)(work + 0xAC) = float_1p6_80424d80;
            break;
        case 1:
            *(f32*)(work + 0xA8) = float_13p9_80424d84;
            *(f32*)(work + 0xAC) = float_2p4_80424d88;
            break;
        case 2:
            *(f32*)(work + 0xA8) = float_17p8_80424d8c;
            *(f32*)(work + 0xAC) = float_3p2_80424d90;
            break;
    }

    *(f32*)(work + 0xB0) = float_0_80424d58;
    *(f32*)(work + 0xB4) = float_30_80424d94;
    return entry;
}

