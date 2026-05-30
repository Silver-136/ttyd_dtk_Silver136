#include "mario/mario_cam.h"

void* marioGetPtr(void);
void* camGetPtr(s32 id);
extern f32 float_0p2_80424adc;
extern f32 float_0_80424aa4;
extern f32 float_0p03_80424ae0;
extern f32 float_0p06_80424ae4;
extern f32 float_2_80424ae8;
extern u32 vec3_802fa748[];
extern u32 vec3_802fa754[];

f32 marioGetCamFollowRate(void) {
    return *(f32*)((s32)marioGetPtr() + 0x15C);
}

s32 marioGetCamId(void) {
    return *(s32*)((s32)marioGetPtr() + 0x134);
}

void marioNoUpdateCamPos(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 4) |= 0x10000;
}

void marioUpdateCamPos(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 4) &= ~0x10000;
}

void marioInitCamId(void) {
    *(s32*)((s32)marioGetPtr() + 0x134) = 4;
}

void marioSetCamFollowRate(f32 rate) {
    *(f32*)((s32)marioGetPtr() + 0x15C) = rate;
}

void marioSetCamId(s32 id) {
    *(s32*)((s32)marioGetPtr() + 0x134) = id;
}

void marioResetCamFollowRate(void) {
    f32 rate = float_0p2_80424adc;
    *(f32*)((s32)marioGetPtr() + 0x15C) = rate;
}

void camFollowYOff(void) {
    void* cam = camGetPtr(4);
    *(u16*)cam |= 0x10;
    *(u32*)((s32)marioGetPtr() + 4) |= 0x20000;
}

void camFollowYOn(void) {
    void* cam = camGetPtr(4);
    *(u16*)cam &= ~0x10;
    *(u32*)((s32)marioGetPtr() + 4) &= ~0x20000;
}

void marioCamZoomOffReq(void) {
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 4);
    if ((flags & 0x100000) != 0 && (flags & 0x200000) == 0) {
        *(u32*)((s32)mario + 4) = flags | 0x200000;
        *(s16*)((s32)mario + 0x62) = 0xDC;
        *(s16*)((s32)mario + 0x5C) = 0x20;
    }
}

void marioCamZoomOffReq2(s32 value) {
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 4);
    if ((flags & 0x100000) != 0) {
        *(u32*)((s32)mario + 4) = flags | 0x200000;
        *(s16*)((s32)mario + 0x62) = value / 10;
        *(s16*)((s32)mario + 0x5C) = 0x20;
    }
}

void marioUpdateCamCollision(void* pos) {
    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0xF8) = *(s32*)((s32)pos + 0);
    *(s32*)((s32)mario + 0xFC) = *(s32*)((s32)pos + 4);
    *(s32*)((s32)mario + 0x100) = *(s32*)((s32)pos + 8);
    *(u32*)((s32)mario + 4) |= 0x40000;
}

void marioResetCamShift(void) {
    void* mario = marioGetPtr();
    f32 zero = float_0_80424aa4;
    f32 two = float_2_80424ae8;
    u32* vec = vec3_802fa748;
    *(f32*)((s32)mario + 0x138) = zero;
    *(f32*)((s32)mario + 0x140) = zero;
    *(f32*)((s32)mario + 0x148) = zero;
    *(f32*)((s32)mario + 0x64) = zero;
    *(f32*)((s32)mario + 0x68) = two;
    *(s32*)((s32)mario + 0x6C) = vec[0];
    *(s32*)((s32)mario + 0x70) = vec[1];
    *(s32*)((s32)mario + 0x74) = vec[2];
}

void marioResetCamShiftRate(void) {
    void* mario = marioGetPtr();
    f32 a = float_0p03_80424ae0;
    f32 b = float_0p06_80424ae4;
    f32 zero = float_0_80424aa4;
    f32 two = float_2_80424ae8;
    u32* vec = vec3_802fa754;
    *(f32*)((s32)mario + 0x150) = a;
    *(f32*)((s32)mario + 0x154) = b;
    *(f32*)((s32)mario + 0x158) = b;
    *(f32*)((s32)mario + 0x64) = zero;
    *(f32*)((s32)mario + 0x68) = two;
    *(s32*)((s32)mario + 0x6C) = vec[0];
    *(s32*)((s32)mario + 0x70) = vec[1];
    *(s32*)((s32)mario + 0x74) = vec[2];
}


u8 marioCamZoomUp(void) {
    return 0;
}


u8 marioCamZoomUpLevelMain(void) {
    return 0;
}


u8 marioUpdateCam(void) {
    return 0;
}


u8 marioCamMain(void) {
    return 0;
}


s32 marioCamZoomOff(void) {
    return 0;
}


u8 marioPreCamera(void) {
    return 0;
}
