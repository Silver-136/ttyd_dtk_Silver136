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
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern s32 marioBgmodeChk(void);
    extern f32 toMovedir(f32 angle);
    extern void cloudGetAt(void* pos);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern void PSVECNormalize(void* src, void* dst);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSVECAdd(void* a, void* b, void* out);
    extern s32 evt_cam3d_evt_set(void* event);
    extern f32 float_0_80424aa4;
    extern f32 float_3p5_80424aac;
    extern f32 float_5_80424aa0;
    extern f32 float_320_80424aa8;
    extern f32 float_450_80424a9c;
    extern f32 float_750_80424ab0;
    extern f32 float_neg70_80424ab4;
    extern f32 float_70_80424ab8;
    extern f32 float_180_80424abc;
    extern f32 float_550_80424ac0;
    extern const u32 vec3_802fa760[];

    void* mario;
    void* party;
    void* cam;
    f32 dir[3];
    f32 pos[3];
    f32 target[3];
    s32 args[8];
    u8 event[0x1B0];
    const u32* vec;
    f32 distance;
    f32 xOff;
    f32 yOff;
    f32 movedir;

    mario = marioGetPtr();
    party = partyGetPtr(marioGetPartyId());

    if (marioBgmodeChk() == 1) {
        return 0;
    }

    if (party == 0 || *(s8*)((s32)party + 0x31) != 5 || (*(u32*)party & 0x100) == 0) {
        cam = camGetPtr(4);
        if (*(u16*)((s32)cam + 4) != 0) {
            return 0;
        }
        if ((*(u32*)((s32)mario + 4) & 0x100000) != 0) {
            return 1;
        }
    } else {
        camGetPtr(4);
        cam = camGetPtr(4);
        if (*(u16*)((s32)cam + 4) != 0 && (*(u32*)((s32)mario + 4) & 0x100000) != 0) {
            return 1;
        }
    }

    *(u32*)((s32)mario + 4) |= 0x100000;
    *(s16*)((s32)mario + 0x5E) = 0;

    if (*(s8*)((s32)mario + 0x3C) == 2) {
        distance = float_750_80424ab0;
    } else {
        distance = float_450_80424a9c;
    }

    xOff = (f32)(*(s32*)((s32)mario + 0x168) - 0x12F) / float_5_80424aa0;
    yOff = float_0_80424aa4;
    if ((f32)*(s32*)((s32)mario + 0x16C) > float_320_80424aa8) {
        yOff = (float_320_80424aa8 - (f32)*(s32*)((s32)mario + 0x16C)) / float_3p5_80424aac;
    }

    if (party != 0 && *(s8*)((s32)party + 0x31) == 5 && (*(u32*)party & 0x100) != 0) {
        movedir = toMovedir(*(f32*)((s32)mario + 0x1AC));
        xOff = float_70_80424ab8;
        if (movedir >= float_180_80424abc) {
            xOff = float_neg70_80424ab4;
        }
        xOff += (f32)(*(s32*)((s32)mario + 0x168) - 0x12F) / float_5_80424aa0;
        distance = float_550_80424ac0;
    }

    cam = camGetPtr(4);
    vec = vec3_802fa760;
    *(u32*)&pos[0] = vec[0];
    *(u32*)&pos[1] = vec[1];
    *(u32*)&pos[2] = vec[2];
    pos[0] = *(f32*)((s32)cam + 0x18) + xOff;
    pos[1] = *(f32*)((s32)cam + 0x1C) + yOff;
    pos[2] = *(f32*)((s32)cam + 0x20);
    cloudGetAt(pos);

    target[0] = pos[0];
    target[1] = pos[1];
    target[2] = pos[2];

    PSVECSubtract((void*)((s32)cam + 0xC), (void*)((s32)cam + 0x18), dir);
    PSVECNormalize(dir, dir);
    PSVECScale(dir, dir, distance);
    PSVECAdd(target, dir, target);

    args[0] = (s16)(s32)target[0];
    args[1] = (s16)(s32)target[1];
    args[2] = (s16)(s32)target[2];
    args[3] = (s16)(s32)pos[0];
    args[4] = (s16)(s32)pos[1];
    args[5] = (s16)(s32)pos[2];
    args[6] = 0x5DC;
    args[7] = 3;
    *(s32**)((s32)event + 0x18) = args;
    evt_cam3d_evt_set(event);
    return 1;
}

u8 marioCamZoomUpLevelMain(void) {
    extern s32 marioBgmodeChk(void);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern void PSVECNormalize(void* src, void* dst);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSVECAdd(void* a, void* b, void* out);
    extern s32 evt_cam3d_evt_set(void* event);
    extern f32 float_0_80424aa4;
    extern f32 float_3p5_80424aac;
    extern f32 float_5_80424aa0;
    extern f32 float_320_80424aa8;
    extern f32 float_440_80424a98;
    extern f32 float_450_80424a9c;
    extern f32 float_475_80424a94;
    extern f32 float_520_80424a90;
    extern const u32 vec3_802fa76c[];
    void* mario;
    void* cam;
    f32 pos[3];
    f32 target[3];
    f32 dir[3];
    s32 args[8];
    u8 event[0x1B0];
    f32 distance;
    f32 xOff;
    f32 yOff;

    mario = marioGetPtr();
    if ((*(u32*)((s32)mario + 0xC) & 0x10) == 0) {
        return 0;
    }
    *(u32*)((s32)mario + 0xC) &= ~0x10;

    if (marioBgmodeChk() == 1) {
        return 0;
    }

    *(u32*)((s32)mario + 4) |= 0x100000;
    *(s16*)((s32)mario + 0x5E) = 0;

    switch (*(s16*)((s32)mario + 0x60)) {
        case 1:
            distance = float_520_80424a90;
            break;
        case 2:
            distance = float_475_80424a94;
            break;
        case 3:
            distance = float_440_80424a98;
            break;
        default:
            distance = float_450_80424a9c;
            break;
    }

    xOff = (f32)(*(s32*)((s32)mario + 0x168) - 0x12F) / float_5_80424aa0;
    yOff = float_0_80424aa4;
    if ((f32)*(s32*)((s32)mario + 0x16C) > float_320_80424aa8) {
        yOff = (float_320_80424aa8 - (f32)*(s32*)((s32)mario + 0x16C)) / float_3p5_80424aac;
    }

    cam = camGetPtr(4);
    pos[0] = *(f32*)((s32)cam + 0x18) + xOff;
    pos[1] = *(f32*)((s32)cam + 0x1C) + yOff;
    pos[2] = *(f32*)((s32)cam + 0x20);

    target[0] = pos[0];
    target[1] = pos[1];
    target[2] = pos[2];
    dir[0] = pos[0];
    dir[1] = pos[1];
    dir[2] = pos[2];

    PSVECSubtract((void*)((s32)cam + 0xC), (void*)((s32)cam + 0x18), dir);
    PSVECNormalize(dir, dir);
    PSVECScale(dir, dir, distance);
    PSVECAdd(target, dir, target);

    args[0] = (s16)(s32)target[0];
    args[1] = (s16)(s32)target[1];
    args[2] = (s16)(s32)target[2];
    args[3] = (s16)(s32)pos[0];
    args[4] = (s16)(s32)pos[1];
    args[5] = (s16)(s32)pos[2];
    args[6] = 0x5DC;
    args[7] = 3;
    *(s32**)((s32)event + 0x18) = args;
    evt_cam3d_evt_set(event);
    return 1;
}

u8 marioUpdateCam(void) {
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern const f32 float_1_80424ac4;
    extern const f32 float_10_80424acc;
    extern const f32 float_90_80424ac8;
    void* mario;
    void* cam;
    f32 sinVal;
    f32 cosVal;
    f32 cur;
    f32 target;
    f32 rate;

    mario = marioGetPtr();

    cur = *(f32*)((s32)mario + 0x13C);
    target = *(f32*)((s32)mario + 0x138);
    rate = *(f32*)((s32)mario + 0x150);
    if (cur > target) {
        cur += ((target - float_1_80424ac4) - cur) * rate;
        *(f32*)((s32)mario + 0x13C) = cur;
        if (*(f32*)((s32)mario + 0x13C) < target) {
            *(f32*)((s32)mario + 0x13C) = target;
        }
    } else if (cur < target) {
        cur += ((target + float_1_80424ac4) - cur) * rate;
        *(f32*)((s32)mario + 0x13C) = cur;
        if (*(f32*)((s32)mario + 0x13C) > target) {
            *(f32*)((s32)mario + 0x13C) = target;
        }
    }

    cur = *(f32*)((s32)mario + 0x144);
    target = *(f32*)((s32)mario + 0x140);
    rate = *(f32*)((s32)mario + 0x154);
    if (cur > target) {
        cur += ((target - float_1_80424ac4) - cur) * rate;
        *(f32*)((s32)mario + 0x144) = cur;
        if (*(f32*)((s32)mario + 0x144) < target) {
            *(f32*)((s32)mario + 0x144) = target;
        }
    } else if (cur < target) {
        cur += ((target + float_1_80424ac4) - cur) * rate;
        *(f32*)((s32)mario + 0x144) = cur;
        if (*(f32*)((s32)mario + 0x144) > target) {
            *(f32*)((s32)mario + 0x144) = target;
        }
    }

    cur = *(f32*)((s32)mario + 0x14C);
    target = *(f32*)((s32)mario + 0x148);
    rate = *(f32*)((s32)mario + 0x158);
    if (cur > target) {
        cur += ((target - float_1_80424ac4) - cur) * rate;
        *(f32*)((s32)mario + 0x14C) = cur;
        if (*(f32*)((s32)mario + 0x14C) < target) {
            *(f32*)((s32)mario + 0x14C) = target;
        }
    } else if (cur < target) {
        cur += ((target + float_1_80424ac4) - cur) * rate;
        *(f32*)((s32)mario + 0x14C) = cur;
        if (*(f32*)((s32)mario + 0x14C) > target) {
            *(f32*)((s32)mario + 0x14C) = target;
        }
    }

    if ((*(u32*)((s32)mario + 4) & 0x10000) == 0) {
        camGetPtr(4);
        cam = camGetPtr(4);
        sincosf(*(f32*)((s32)cam + 0x114) - float_90_80424ac8, &sinVal, &cosVal);
        if ((*(u32*)((s32)mario + 4) & 0x20000) == 0) {
            *(f32*)((s32)mario + 0xEC) =
                (*(f32*)((s32)mario + 0x13C) * sinVal) + *(f32*)((s32)mario + 0x8C);
            *(f32*)((s32)mario + 0xF4) =
                (*(f32*)((s32)mario + 0x13C) * cosVal) + *(f32*)((s32)mario + 0x94);
            *(f32*)((s32)mario + 0xF0) =
                *(f32*)((s32)mario + 0x90) + *(f32*)((s32)mario + 0x9C) +
                *(f32*)((s32)mario + 0x144);
            if (*(u16*)((s32)mario + 0x2E) == 0x19) {
                *(f32*)((s32)mario + 0xF0) += float_10_80424acc;
            }
            *(f32*)((s32)mario + 0x64) = *(f32*)((s32)mario + 0x14C);
        } else {
            *(f32*)((s32)mario + 0xEC) =
                (*(f32*)((s32)mario + 0x13C) * sinVal) +
                (*(f32*)((s32)mario + 0x14C) * sinVal) +
                *(f32*)((s32)mario + 0x8C);
            *(f32*)((s32)mario + 0xF4) =
                (*(f32*)((s32)mario + 0x13C) * cosVal) +
                (*(f32*)((s32)mario + 0x14C) * cosVal) +
                *(f32*)((s32)mario + 0x94);
            *(f32*)((s32)mario + 0x64) = *(f32*)((s32)mario + 0x14C);
        }
    }

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
