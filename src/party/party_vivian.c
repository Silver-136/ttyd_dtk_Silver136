#include "party/party_vivian.h"

extern s32 vivian_cancel_req;
extern f32 float_35_804242d8;
extern f32 float_24_804242bc;
extern f32 float_10_80424270;

void L_partyVivianTailStart(void* party) {
    if (*(s8*)((s32)party + 0x31) == 6) {
        s32 flags = *(u8*)((s32)party + 0x4F);
        *(u8*)((s32)party + 0x4F) = flags & ~1;
    }
}

void L_partyVivianTailStop(void* party) {
    if (*(s8*)((s32)party + 0x31) == 6) {
        *(u8*)((s32)party + 0x4F) |= 1;
    }
}

void vivianUnhold(void* party) {
    if (party == 0) {
        return;
    }
    if (*(s8*)((s32)party + 0x31) != 6) {
        return;
    }
    if (*(u32*)party & 0x100) {
        if (*(u8*)((s32)party + 0x39) == 10) {
            *(u8*)((s32)party + 0x39) = 0x14;
        }
    }
}

void vivian_init(void* party) {
    f32 value;
    f32 offset;
    f32 scale;
    s32 six = 6;
    s32 one = 1;
    s32 zero = 0;
    s32 pos1;
    s32 pos0;

    *(u8*)((s32)party + 0x33) = six;
    scale = float_35_804242d8;
    *(u8*)((s32)party + 0x32) = one;
    offset = float_24_804242bc;
    *(f32*)((s32)party + 0xF0) = scale;
    value = float_10_80424270;
    *(f32*)((s32)party + 0xF4) = offset;
    *(f32*)((s32)party + 0x174) = value;
    *(u8*)((s32)party + 0x4F) = zero;
    pos1 = *(s32*)((s32)party + 0x5C);
    pos0 = *(s32*)((s32)party + 0x58);
    *(s32*)((s32)party + 0xB8) = pos0;
    *(s32*)((s32)party + 0xBC) = pos1;
    *(s32*)((s32)party + 0xC0) = *(s32*)((s32)party + 0x60);
    *(s32*)((s32)party + 0x184) = zero;
    vivian_cancel_req = zero;
}

