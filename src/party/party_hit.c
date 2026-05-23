#include "party/party_hit.h"

s32 unk_800c27c0(void* flags, void* mario) {
    extern f32 float_6_80421518;
    extern s32 marioChkWallAround(void* mario, s32 arg, f32 a, f32 b, f32 radius);
    f32 a;
    f32 b;
    f32 radius;

    if (*(u32*)flags & 0x1000) {
        radius = float_6_80421518;
    }
    return marioChkWallAround(mario, 1, a, b, radius);
}
