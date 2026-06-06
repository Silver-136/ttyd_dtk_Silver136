#include "mario/mario_cam.h"

void* marioGetPtr(void);
void* camGetPtr(s32 id);
extern s32 evt_cam3d_evt_off(void* event);
extern f32 revise360(f32 angle);
extern f32 float_0p2_80424adc;
extern f32 float_0_80424aa4;
extern f32 float_0p5_80424ad0;
extern f32 float_neg0p5_80424ad4;
extern f32 float_1000_80424ad8;
extern f32 float_0p03_80424ae0;
extern f32 float_0p06_80424ae4;
extern f32 float_2_80424ae8;
extern u32 vec3_802fa748[];
extern u32 vec3_802fa754[];

typedef struct MarioCamEventWork {
    s32 args[2];
    u8 pad[0x20];
    u8 event[0x1B0];
} MarioCamEventWork;

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
    s32 x = *(s32*)((s32)pos + 0);
    s32 y = *(s32*)((s32)pos + 4);
    *(s32*)((s32)mario + 0xF8) = x;
    x = *(s32*)((s32)pos + 8);
    *(s32*)((s32)mario + 0xFC) = y;
    *(s32*)((s32)mario + 0x100) = x;
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
    u32* vec = vec3_802fa754;
    *(f32*)((s32)mario + 0x150) = a;
    {
        f32 zero = float_0_80424aa4;
        *(f32*)((s32)mario + 0x154) = b;
        {
            f32 two = float_2_80424ae8;
            *(f32*)((s32)mario + 0x158) = b;
            *(f32*)((s32)mario + 0x64) = zero;
            *(f32*)((s32)mario + 0x68) = two;
        }
    }
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioCamMain(void) {
    void* firstMario;
    void* mario;
    void* cam;
    u32 flags;
    s32 timer;
    MarioCamEventWork work;

    firstMario = marioGetPtr();
    if (*(u16*)((s32)firstMario + 0x2E) != 0x1D) {
        marioUpdateCam();
    }

    marioCamZoomUpLevelMain();

    mario = marioGetPtr();
    flags = *(u32*)((s32)mario + 4);
    if ((flags & 0x100000) == 0) {
        *(u32*)((s32)mario + 4) = flags & ~0x200000;
    } else if ((flags & 0x200000) != 0) {
        timer = *(s16*)((s32)mario + 0x5C) - 1;
        *(s16*)((s32)mario + 0x5C) = timer;
        if ((s16)timer <= 0) {
            s32 frames;
            *(s16*)((s32)mario + 0x5C) = 0;
            *(s16*)((s32)mario + 0x60) = 0;
            *(u32*)((s32)mario + 4) &= ~(0x100000 | 0x200000);
            frames = *(s16*)((s32)mario + 0x62) * 10;
            *(s16*)((s32)mario + 0x5E) = (frames * 60) / 100;
            work.args[0] = frames;
            work.args[1] = 0xB;
            *(s32**)((s32)work.event + 0x18) = work.args;
            evt_cam3d_evt_off(work.event);
        }
    }

    cam = camGetPtr(4);
    mario = marioGetPtr();
    if (*(f32*)((s32)mario + 0x194) != float_0_80424aa4) {
        if (*(f32*)((s32)cam + 0xC) == *(f32*)((s32)mario + 0x160)) {
            *(u32*)((s32)mario + 4) |= 0x20;
        } else {
            *(u32*)((s32)mario + 4) &= ~0x20;
        }
    }
    *(f32*)((s32)mario + 0x160) = *(f32*)((s32)cam + 0xC);
    *(u32*)((s32)firstMario + 4) &= ~0x40000;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


s32 marioCamZoomOff(void) {
    void* mario;
    u32 flags;
    s32 timer;
    MarioCamEventWork work;

    mario = marioGetPtr();
    flags = *(u32*)((s32)mario + 4);
    if ((flags & 0x100000) == 0) {
        *(u32*)((s32)mario + 4) = flags & ~0x200000;
        return 0;
    }
    if ((flags & 0x200000) == 0) {
        return 0;
    }

    timer = *(s16*)((s32)mario + 0x5C) - 1;
    *(s16*)((s32)mario + 0x5C) = timer;
    if ((s16)timer > 0) {
        return 0;
    }

    *(s16*)((s32)mario + 0x5C) = 0;
    *(s16*)((s32)mario + 0x60) = 0;
    *(u32*)((s32)mario + 4) &= ~(0x100000 | 0x200000);
    {
        s32 frames = *(s16*)((s32)mario + 0x62) * 10;
        *(s16*)((s32)mario + 0x5E) = (frames * 60) / 100;
        work.args[0] = frames;
        work.args[1] = 0xB;
    }
    *(s32**)((s32)work.event + 0x18) = work.args;
    evt_cam3d_evt_off(work.event);
    return 1;
}


void marioPreCamera(void) {
    f32 add;
    f32 angle;

    if (*(f32*)((s32)camGetPtr(4) + 0x114) >= float_0_80424aa4) {
        add = float_0p5_80424ad0;
    } else {
        add = float_neg0p5_80424ad4;
    }

    angle = *(f32*)((s32)camGetPtr(4) + 0x114);
    angle = revise360((f32)(s32)((float_1000_80424ad8 * angle) + add) / float_1000_80424ad8);
    *(f32*)((s32)marioGetPtr() + 0x19C) = angle;
}
