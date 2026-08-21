#include "effect/eff_fall.h"
#include "driver/camdrv.h"

extern f64 sin(f64 x);
extern f64 cos(f64 x);
void* effFallEntry(f32 x, s32 type, f32 y, s32 count, f32 z, s32 param, f32 rot, f32 arg5, f32 arg6) {
    typedef struct EffEntryRaw {
        u32 flags;
        u32 inBattle;
        s32 count;
        void* work;
        void* main;
        char* name;
        char pad[0x10];
    } EffEntryRaw;
    typedef struct VecRaw { f32 x; f32 y; f32 z; } VecRaw;

    extern EffEntryRaw* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 irand(s32 max);
    extern u32 psndSFXOn_3D(char* name, VecRaw* pos);

    extern char str_Fall_804275a8;
    extern char str_SFX_BTL_STAGE_FALL_W_802feedc[];
    extern VecRaw vec3_802fed70;
    extern f64 double_to_int_802fedd8;
    extern f32 float_3p1416_8042758c;
    extern f32 float_180_80427510;
    extern f32 float_300_804275b0;
    extern f32 float_0_804274d4;
    extern f32 float_0p2_80427564;
    extern f32 float_0p5_804274cc;
    extern f32 float_1p2_804275b4;
    extern f32 float_1p6_804275b8;
    extern f32 float_0p1_804275bc;
    extern f32 float_200_80427518;
    extern f32 float_1_804274d0;
    extern f32 float_0p6_804275c0;
    extern f32 float_30_80427530;
    extern f32 float_1p3_804275c4;
    extern f32 float_400_804275c8;
    extern f32 float_0p25_804274f0;

    EffEntryRaw* eff;
    void* base;
    void* child;
    VecRaw pos;
    s32 effCount;
    s32 mode;
    s32 i;
    s32 rotInt;
    f32 zero;
    f32 pi;
    f32 deg180;
    f32 heightBase;
    f32 angle;
    f32 radius;
    f32 temp;

    eff = effEntry();
    effCount = count + 1;
    mode = count;
    if (type != 0 && type > -1 && type < 9) {
        mode = 1;
        effCount = 2;
    }

    eff->name = &str_Fall_804275a8;
    eff->count = effCount;
    base = __memAlloc(3, eff->count * 0x68);
    eff->work = base;
    eff->main = (void*)effFallMain;
    eff->flags |= 2;

    rotInt = (s32)rot;
    pi = float_3p1416_8042758c;
    deg180 = float_180_80427510;
    heightBase = float_300_804275b0;
    zero = float_0_804274d4;

    *(s32*)((s32)base + 0x00) = type;
    *(f32*)((s32)base + 0x04) = x;
    *(f32*)((s32)base + 0x08) = y;
    *(f32*)((s32)base + 0x0C) = z;
    *(f32*)((s32)base + 0x10) = rot;
    *(f32*)((s32)base + 0x14) = arg5;
    *(f32*)((s32)base + 0x18) = arg6;
    *(s32*)((s32)base + 0x1C) = mode;
    *(s32*)((s32)base + 0x24) = 0;
    *(s32*)((s32)base + 0x28) = 0;
    *(s32*)((s32)base + 0x20) = param;
    *(s32*)((s32)base + 0x5C) = -1;
    *(s32*)((s32)base + 0x60) = -1;

    child = (void*)((s32)base + 0x68);
    for (i = 1; i < eff->count; i++, child = (void*)((s32)child + 0x68)) {
        angle = (pi * (f32)irand(0x168)) / deg180;
        radius = (f32)irand(rotInt);
        *(f32*)((s32)child + 0x04) = radius * (f32)sin((double)angle);
        *(f32*)((s32)child + 0x08) = heightBase + (f32)irand(eff->count);
        *(f32*)((s32)child + 0x0C) = radius * (f32)cos((double)angle);
        *(s32*)((s32)child + 0x24) = 0;
        *(s32*)((s32)child + 0x28) = 0;
        *(s32*)((s32)child + 0x3C) = 0xFF;
        *(f32*)((s32)child + 0x48) = zero;
        if ((u32)irand(100) <= 50U) {
            *(f32*)((s32)child + 0x18) = 180.0f;
        } else {
            *(f32*)((s32)child + 0x18) = zero;
        }
        *(s32*)((s32)child + 0x38) = 0;
        *(s32*)((s32)child + 0x5C) = -1;
        *(s32*)((s32)child + 0x60) = -1;

        switch (type) {
            case 0:
            case 1:
                *(f32*)((s32)child + 0x34) = zero;
                *(f32*)((s32)child + 0x30) = float_0p2_80427564;
                if (i == 1) {
                    *(f32*)((s32)child + 0x04) = zero;
                    *(f32*)((s32)child + 0x0C) = zero;
                    *(s32*)((s32)child + 0x24) = 0;
                } else {
                    *(s32*)((s32)child + 0x24) = irand(eff->count) + 0x3C;
                }
                if (type == 0) {
                    *(f32*)((s32)child + 0x2C) = float_0p5_804274cc;
                } else {
                    *(f32*)((s32)child + 0x2C) = float_1p2_804275b4;
                }
                break;
            case 2:
                *(f32*)((s32)child + 0x34) = zero;
                *(f32*)((s32)child + 0x30) = float_1p6_804275b8;
                *(f32*)((s32)child + 0x04) = zero;
                *(f32*)((s32)child + 0x0C) = zero;
                *(s32*)((s32)child + 0x24) = 0;
                *(f32*)((s32)child + 0x44) = zero;
                *(f32*)((s32)child + 0x2C) = float_0p5_804274cc;
                break;
            case 3:
                *(f32*)((s32)child + 0x34) = zero;
                *(f32*)((s32)child + 0x30) = float_0p1_804275bc;
                *(f32*)((s32)child + 0x04) = zero;
                *(f32*)((s32)child + 0x08) = float_200_80427518;
                *(f32*)((s32)child + 0x0C) = zero;
                *(f32*)((s32)child + 0x50) = zero;
                *(f32*)((s32)child + 0x54) = zero;
                *(f32*)((s32)child + 0x58) = zero;
                *(f32*)((s32)child + 0x2C) = float_1_804274d0;
                pos = vec3_802fed70;
                pos.x = x;
                pos.y = y;
                pos.z = z;
                *(u32*)((s32)child + 0x64) = psndSFXOn_3D(str_SFX_BTL_STAGE_FALL_W_802feedc, &pos);
                break;
            case 4:
            case 5:
                *(f32*)((s32)child + 0x34) = zero;
                *(f32*)((s32)child + 0x30) = float_0p6_804275c0;
                *(f32*)((s32)child + 0x04) = zero;
                *(f32*)((s32)child + 0x0C) = zero;
                *(s32*)((s32)child + 0x24) = 0;
                *(f32*)((s32)child + 0x2C) = float_1_804274d0;
                break;
            case 6:
                *(f32*)((s32)child + 0x04) = zero;
                *(f32*)((s32)child + 0x08) = float_200_80427518;
                *(f32*)((s32)child + 0x0C) = zero;
                temp = float_30_80427530;
                *(f32*)((s32)child + 0x50) = -*(f32*)((s32)child + 0x04) / temp;
                *(f32*)((s32)child + 0x54) = -*(f32*)((s32)child + 0x08) / temp;
                *(f32*)((s32)child + 0x58) = -*(f32*)((s32)child + 0x0C) / temp;
                *(f32*)((s32)child + 0x2C) = float_1p3_804275c4;
                *(f32*)((s32)child + 0x48) = temp;
                *(f32*)((s32)child + 0x4C) = zero;
                break;
            case 7:
                *(f32*)((s32)child + 0x04) = float_400_804275c8;
                *(f32*)((s32)child + 0x08) = float_200_80427518;
                *(f32*)((s32)child + 0x0C) = zero;
                *(f32*)((s32)child + 0x2C) = float_0p5_804274cc;
                break;
            case 8:
                *(f32*)((s32)child + 0x34) = zero;
                *(f32*)((s32)child + 0x30) = float_0p25_804274f0;
                *(f32*)((s32)child + 0x04) = zero;
                *(f32*)((s32)child + 0x08) = float_400_804275c8;
                *(f32*)((s32)child + 0x0C) = zero;
                *(f32*)((s32)child + 0x2C) = float_1_804274d0;
                break;
        }
    }

    return eff;
}

void effFallMain(void* effect) {
    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void psndSFXOn(s32);
    extern void psndSFXOff(s32);
    extern void animPoseRelease(s32);
    extern s32 animGroupBaseAsync(void*, s32, s32);
    extern s32 animPoseEntry(void*, s32);
    extern void animPoseSetAnim(s32, void*, s32);
    extern u8 animPoseMain(s32);
    extern f64 animPoseGetLoopTimes(s32);
    extern void effKemuri1N64Entry(f64, f64, f64, f64, s32);
    extern void* evtEntry(void*, s32, u32);
    extern u32 psndSFXOn_3D(char*, void*);
    extern s32 evt_shake[];
    extern void* gp;
    extern f64 distABf(f64, f64, f64, f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern void effWaterDamageN64Entry(f64, f64, f64, f64, s32, s32);
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    u8* base = *(u8**)((u8*)effect + 0xC);
    s32 count = *(s32*)((u8*)effect + 8);
    s32 type = *(s32*)base;
    s32 alive = 0;
    s32 animMode;
    s32 i;
    f32 pos[3];

    animMode = (*(s32*)((u8*)gp + 0x14) != 0);
    pos[0] = *(f32*)(base + 4);
    pos[1] = *(f32*)(base + 8);
    pos[2] = *(f32*)(base + 0xC);
    for (i = 1; i < count; i++) {
        u8* p = base + i * 0x68;
        s32* state = (s32*)(p + 0x38);
        s32* timer = (s32*)(p + 0x24);
        f32* y = (f32*)(p + 8);
        f32* vy = (f32*)(p + 0x34);
        f32* gravity = (f32*)(p + 0x30);
        s32* alpha = (s32*)(p + 0x3C);
        if (*state < 0) {
            continue;
        }
        alive++;
        if (type == 8) {
            s32 pose = *(s32*)(p + 0x5C);

            switch (*state) {
            case 0:
                if (animGroupBaseAsync(
                        "EFF_light", animMode, 0) != 0) {
                    pose = animPoseEntry(
                        "EFF_light", animMode);
                    *(s32*)(p + 0x5C) = pose;
                    animPoseSetAnim(pose, "Z_1", 1);
                    (*state)++;
                }
                break;

            case 1:
            case 2:
                *y += *vy;
                *vy -= *gravity;

                if (*y < *(f32*)(base + 8)) {
                    *y = *(f32*)(base + 8);
                    *timer = 0;
                    (*state)++;
                }

                if (*state == 1 &&
                    *y < *(f32*)(base + 8) + 30.0f) {
                    effKemuri1N64Entry(
                        (f64)(pos[0] + *(f32*)(p + 4)),
                        (f64)(pos[1] + *(f32*)(base + 8)),
                        (f64)(pos[2] + *(f32*)(p + 0xC)),
                        0.0,
                        4);
                    evtEntry(evt_shake, 0, 0);
                    animPoseSetAnim(
                        *(s32*)(p + 0x5C),
                        "A_1",
                        1);
                    (*state)++;
                    psndSFXOn_3D(
                        "SFX_BTL_STAGE_DAMAGE",
                        pos);
                }
                break;

            case 3:
                if (animPoseGetLoopTimes(
                        *(s32*)(p + 0x5C)) >= 1.0) {
                    (*timer)++;
                    if (*(s32*)(p + 0x20) < *timer) {
                        (*state)++;
                    }
                }
                break;

            case 4:
                *alpha -= 10;
                if (*alpha < 0) {
                    *alpha = 0;
                    (*state)++;
                }
                break;

            case 5:
                animPoseRelease(*(s32*)(p + 0x5C));
                *(s32*)(p + 0x5C) = -1;
                (*state)++;
                break;

            case 6:
                alive--;
                break;
            }

            if (*(s32*)(p + 0x5C) != -1) {
                animPoseMain(*(s32*)(p + 0x5C));
            }
        } else if (type == 7 && *state >= 8) {
            if (*state == 8) {
                psndSFXOff(*(s32*)(p + 0x64));
                if (*(s32*)(p + 0x5C) != -1) {
                    animPoseRelease(*(s32*)(p + 0x5C));
                }
                if (*(s32*)(p + 0x60) != -1) {
                    animPoseRelease(*(s32*)(p + 0x60));
                }
                *(s32*)(p + 0x5C) = -1;
                *(s32*)(p + 0x60) = -1;
                (*state)++;
            } else if (*state == 9) {
                alive--;
            }
        } else if (type == 4 || type == 5) {
            s32 pose = *(s32*)(p + 0x5C);
            const char* animName =
                type == 5 ? "EFF_Baketu" : "EFF_Tarai";

            switch (*state) {
            case 0:
                if (animGroupBaseAsync((void*)animName, animMode, 0) != 0) {
                    if (pose == -1) {
                        pose = animPoseEntry((void*)animName, animMode);
                        *(s32*)(p + 0x5C) = pose;
                        animPoseSetAnim(pose, "Z_1", 1);
                    }
                    if (*timer != 0) {
                        (*timer)--;
                    }
                    if (*timer == 0) {
                        (*state)++;
                    }
                }
                break;

            case 1:
                *y += *vy;
                *vy -= *gravity;
                if (distABf(
                        *(f32*)(p + 4),
                        *(f32*)(p + 0xC),
                        0.0,
                        0.0) >=
                    (f64)(0.5f * *(f32*)(base + 0x10))) {
                    if (*y < *(f32*)(base + 8)) {
                        *y = *(f32*)(base + 8);
                        *vy *= -0.3f;
                        (*state)++;
                    }
                } else if (
                    *y < *(f32*)(base + 8) +
                             *(f32*)(base + 0x14)) {
                    *y = *(f32*)(base + 8) +
                         *(f32*)(base + 0x14);
                    *vy *= -0.3f;
                    (*state)++;
                }
                break;

            case 2: {
                f32 angle;

                *y += *vy;
                *vy -= *gravity;

                angle =
                    6.2832f *
                    (f32)angleABf(
                        0.0,
                        0.0,
                        *(f32*)(p + 4),
                        *(f32*)(p + 0xC));
                if (i == 1) {
                    angle =
                        6.2832f * *(f32*)(base + 0x18);
                }
                angle /= 360.0f;

                *(f32*)(p + 4) +=
                    2.0f * (f32)sin((f64)angle);
                *(f32*)(p + 0xC) -=
                    2.0f * (f32)cos((f64)angle);

                if (*y < *(f32*)(base + 8)) {
                    *y = *(f32*)(base + 8);
                    *vy *= -0.5f;
                    *y = *(f32*)(base + 8);
                    *timer = 0;
                    if (*vy < 1.0f) {
                        (*state)++;
                    }
                }
                break;
            }

            case 3:
                (*timer)++;
                if (*timer > 30) {
                    (*state)++;
                }
                break;

            case 4:
                *alpha -= 10;
                if (*alpha < 0) {
                    *alpha = 0;
                    (*state)++;
                    animPoseRelease(*(s32*)(p + 0x5C));
                    *(s32*)(p + 0x5C) = -1;
                }
                break;

            case 5:
                alive--;
                break;
            }

            if (*state > 1) {
                f32 target =
                    type == 5 ? 430.0f : 360.0f;

                *(f32*)(p + 0x48) +=
                    (target - *(f32*)(p + 0x48)) /
                    10.0f;
            }

            if (*(s32*)(p + 0x5C) != -1) {
                animPoseMain(*(s32*)(p + 0x5C));
            }
        } else if (type == 6) {
            s32 pose = *(s32*)(p + 0x5C);

            switch (*state) {
            case 0:
                if (animGroupBaseAsync(
                        "EFF_koopa", animMode, 0) != 0) {
                    if (pose == -1) {
                        pose = animPoseEntry(
                            "EFF_koopa", animMode);
                        *(s32*)(p + 0x5C) = pose;
                        animPoseSetAnim(pose, "Z_1", 1);
                        psndSFXOn_3D(
                            "SFX_BTL_STAGE_FALL_K", pos);
                    }

                    *(f32*)(p + 4) += *(f32*)(p + 0x50);
                    *(f32*)(p + 8) += *(f32*)(p + 0x54);
                    *(f32*)(p + 0xC) += *(f32*)(p + 0x58);

                    (*timer)++;
                    if (*timer > 30) {
                        *timer = 0;
                        (*state)++;

                        effKemuri1N64Entry(
                            (f64)(pos[0] + *(f32*)(p + 4)),
                            (f64)(pos[1] + *(f32*)(p + 8)),
                            (f64)(pos[2] + *(f32*)(p + 0xC)),
                            0.0,
                            4);
                        evtEntry(evt_shake, 0, 0);
                    }
                }
                break;

            case 1:
                *(f32*)(p + 0x48) =
                    (f32)intplGetValue(
                        30.0, -27.0, 11, *timer, 180);
                if (*(f32*)(p + 0x48) < 0.0f) {
                    *(f32*)(p + 0x4C) =
                        *(f32*)(p + 0x48);
                    *(f32*)(p + 0x48) = 0.0f;
                }

                (*timer)++;
                if (*timer > 180) {
                    *timer = 0;
                    (*state)++;
                }

                if (*timer == 100) {
                    psndSFXOn_3D(
                        "SFX_BTL_STAGE_FALL_K", pos);
                }
                break;

            case 2:
                *(f32*)(p + 0x48) =
                    (f32)intplGetValue(
                        -27.0, 80.0, 11, *timer, 200);
                if (*(f32*)(p + 0x48) < 0.0f) {
                    *(f32*)(p + 0x4C) =
                        *(f32*)(p + 0x48);
                    *(f32*)(p + 0x48) = 0.0f;
                }

                (*timer)++;
                if (*timer > 200) {
                    animPoseRelease(
                        *(s32*)(p + 0x5C));
                    alive--;
                    pose = -1;
                    *(s32*)(p + 0x5C) = -1;
                }

                if (*timer == 70) {
                    psndSFXOn_3D(
                        "SFX_BTL_STAGE_FALL_K", pos);
                }
                break;
            }

            if (pose != -1) {
                animPoseMain(pose);
            }
        } else if (type == 3) {
            switch (*state) {
            case 0:
                *y += *vy;
                *vy -= *gravity;
                if (*y < *(f32*)(base + 8)) {
                    *y = *(f32*)(base + 8);
                    *timer = 0;
                    (*state)++;
                }
                break;
            case 1:
                if ((*timer % 10) == 0) {
                    effWaterDamageN64Entry(pos[0], pos[1], pos[2], 1.0, 4, 20);
                }
                (*timer)++;
                if (*(s32*)(base + 0x20) < *timer) {
                    (*state)++;
                }
                break;
            case 2:
            case 3:
                *alpha = (s32)((f32)*alpha * 0.9f);
                *(f32*)(p + 0x2C) *= 0.9f;
                if (*(f32*)(p + 0x2C) < 0.01f) {
                    effDelete(effect);
                    return;
                }
                break;
            }
            *(f32*)(p + 0x5C) -= 0.2f;
        } else {
        switch (*state) {
            case 0:
                if (*timer != 0) {
                    (*timer)--;
                }
                if (*timer == 0) {
                    (*state)++;
                    if (type == 1 || i == 1) {
                        psndSFXOn(0x8CF);
                    }
                }
                break;
            case 1:
                *y += *vy;
                *vy -= *gravity;
                if (distABf(*(f32*)(p + 4), *(f32*)(p + 0xC), 0.0, 0.0) >=
                    (f64)(0.5f * *(f32*)(base + 0x10))) {
                    if (*y > *(f32*)(base + 8)) {
                        break;
                    }
                    *y = *(f32*)(base + 8);
                } else {
                    if (*y > *(f32*)(base + 8) + *(f32*)(base + 0x14)) {
                        break;
                    }
                    *y = *(f32*)(base + 8) + *(f32*)(base + 0x14);
                }
                    *vy *= -0.3f;
                    (*state)++;
                break;
            case 2:
                *y += *vy;
                *vy -= *gravity;
                {
                    f32 angle = (f32)angleABf(0.0, 0.0,
                                              *(f32*)(p + 4), *(f32*)(p + 0xC));
                    if (i == 1) {
                        angle = 6.2832f * *(f32*)(base + 0x18);
                    } else {
                        angle = 6.2832f * angle / 360.0f;
                    }
                    *(f32*)(p + 4) += 0.5f * (f32)sin((f64)angle);
                    *(f32*)(p + 0xC) -= 0.5f * (f32)cos((f64)angle);
                }
                if (*y <= *(f32*)(base + 8)) {
                    *y = *(f32*)(base + 8);
                    *timer = 0;
                    (*state)++;
                }
                break;
            case 3:
                (*timer)++;
                if (*timer > 30) {
                    (*state)++;
                }
                break;
            case 4:
                *alpha -= 10;
                if (*alpha < 0) {
                    *alpha = 0;
                    (*state)++;
                }
                break;
            case 5:
                alive--;
                break;
        }
        }
        if (type < 2) {
            if (*(s32*)(p + 0x28) > 2) {
                *(s32*)(p + 0x28) = 0;
            }
            *(f32*)(p + 0x48) = 35.0f *
                (f32)sin((f64)(0.5f * (f32)(*timer + i)));
        }
    }

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        for (i = 0; i < count; i++) {
            u8* p = base + i * 0x68;
            if (*(s32*)(p + 0x5C) != -1) {
                animPoseRelease(*(s32*)(p + 0x5C));
            }
            if (*(s32*)(p + 0x60) != -1) {
                animPoseRelease(*(s32*)(p + 0x60));
            }
            psndSFXOff(*(s32*)(p + 0x64));
        }
        effDelete(effect);
    } else if (alive == 0) {
        effDelete(effect);
    } else {
        if (type == 3) {
            dispEntry(4, 2, effFallDispWater, effect, dispCalcZ(pos));
        } else if (type == 7) {
            dispEntry(4, 2, effFallDispInseki, effect, dispCalcZ(pos));
        } else if (type == 4 || type == 5) {
            dispEntry(4, 0, effFallDisp, effect, dispCalcZ(pos));
        } else {
            dispEntry(4, 2, effFallDisp, effect, dispCalcZ(pos));
        }
    }
}

void effFallDisp(s32 cameraId, void* effect) {
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXBegin(s32, s32, s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void PSMTXRotAxisRad(void*, void*, f32);
    extern void animPoseDrawMtx(s32, void*, s32, f64, f64);
    u8* base = *(u8**)((u8*)effect + 0xC);
    s32 count = *(s32*)((u8*)effect + 8);
    s32 type = *(s32*)base;
    u8 tex[64];
    f32 trans[3][4];
    f32 scale[3][4];
    f32 model[3][4];
    f32 baseMtx[3][4];
    f32 rot[3][4];
    f32 texMtx[3][4];
    f32 poseAxis[3];
    f32 poseAxisAngle;
    u8* camera;
    u8* rotationCamera;
    s32 i;

    camera = camGetPtr(cameraId);
    PSMTXTrans(trans, *(f32*)(base + 4), *(f32*)(base + 8), *(f32*)(base + 0xC));
    rotationCamera = camGetPtr(cameraId);
    PSMTXRotRad(rot, 0x79,
                0.017453292f * -*(f32*)(rotationCamera + 0x114));
    PSMTXConcat(trans, rot, baseMtx);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    poseAxisAngle = 6.2832f * *(f32*)(base + 0x18);
    if (type == 2) {
        effGetTexObj(0x33, tex);
        GXLoadTexObj(tex, 0);
    } else if (type >= 0 && type < 2) {
        effGetTexObj(0x32, tex);
        GXLoadTexObj(tex, 0);
    }
    for (i = 1; i < count; i++) {
        u8* p = base + i * 0x68;
        u32 color;

        if (type == 4 || type == 5) {
            s32 pose = *(s32*)(p + 0x5C);
            f32 scaleValue;
            f32 centerOffset;

            poseAxis[0] =
                (f32)cos((f64)(poseAxisAngle / 360.0f));
            poseAxis[1] = 0.0f;
            poseAxis[2] =
                (f32)sin((f64)(poseAxisAngle / 360.0f));

            PSMTXTrans(
                trans,
                *(f32*)(p + 4),
                *(f32*)(p + 8),
                *(f32*)(p + 0xC));

            scaleValue = *(f32*)(p + 0x2C);
            PSMTXScale(
                scale,
                scaleValue,
                scaleValue,
                scaleValue);
            PSMTXConcat(trans, scale, trans);

            centerOffset = type == 4 ? 8.0f : 16.0f;
            PSMTXTrans(
                scale,
                0.0f,
                centerOffset,
                0.0f);
            PSMTXConcat(trans, scale, trans);

            PSMTXRotAxisRad(
                rot,
                poseAxis,
                0.017453292f * -*(f32*)(p + 0x48));
            PSMTXConcat(trans, rot, trans);

            PSMTXTrans(
                scale,
                0.0f,
                -centerOffset,
                0.0f);
            PSMTXConcat(trans, scale, trans);
            PSMTXConcat(baseMtx, trans, trans);

            if (pose != -1) {
                animPoseDrawMtx(
                    pose, trans, 1, 0.0, 10.0);
                animPoseDrawMtx(
                    pose, trans, 2, 0.0, 10.0);
                animPoseDrawMtx(
                    pose, trans, 3, 0.0, 10.0);
            }
            continue;
        }

        PSMTXTrans(trans, *(f32*)(p + 4), *(f32*)(p + 8), *(f32*)(p + 0xC));
        if (type < 2) {
            PSMTXRotRad(rot, 0x79, 0.017453292f * *(f32*)(p + 0x18));
            PSMTXConcat(trans, rot, trans);
            PSMTXTrans(rot, 0.0f, 10.66675f * *(f32*)(p + 0x2C), 0.0f);
            PSMTXConcat(trans, rot, trans);
            PSMTXRotRad(rot, 0x7A, 0.017453292f * *(f32*)(p + 0x48));
            PSMTXConcat(trans, rot, trans);
            PSMTXTrans(rot, 0.0f, -10.66675f * *(f32*)(p + 0x2C), 0.0f);
            PSMTXConcat(trans, rot, trans);
        }
        PSMTXScale(scale, *(f32*)(p + 0x2C), *(f32*)(p + 0x2C), *(f32*)(p + 0x2C));
        PSMTXConcat(trans, scale, model);
        PSMTXConcat(baseMtx, model, model);
        PSMTXConcat(camera + 0x118, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        color = *(u32*)(p + 0x3C);
        GXSetChanMatColor(4, &color);
        if (type < 2) {
            PSMTXScale(texMtx, 1.0f, 0.33333f, 1.0f);
            PSMTXTrans(rot, 0.0f, (f32)*(s32*)(p + 0x28), 0.0f);
            PSMTXConcat(texMtx, rot, texMtx);
            GXLoadTexMtxImm(texMtx, 0x1E, 1);
        }
        GXBegin(0x80, 0, 4);
        *(volatile f32*)0xCC008000 = -16.0f;
        *(volatile f32*)0xCC008000 = 21.3335f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 16.0f;
        *(volatile f32*)0xCC008000 = 21.3335f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 1.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 16.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 1.0f;
        *(volatile f32*)0xCC008000 = 1.0f;
        *(volatile f32*)0xCC008000 = -16.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 1.0f;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effFallDispInseki(s32 cameraId, void* eff) {
    typedef f32 MtxRaw[3][4];
    typedef struct VecRaw { f32 x; f32 y; f32 z; } VecRaw;

    extern void PSMTXTrans(MtxRaw m, double x, double y, double z);
    extern void PSMTXRotRad(MtxRaw m, s32 axis, double angle);
    extern void PSMTXRotAxisRad(MtxRaw m, VecRaw* axis, double angle);
    extern void PSMTXScale(MtxRaw m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(MtxRaw a, MtxRaw b, MtxRaw out);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseDrawMtx(s32 poseId, MtxRaw mtx, s32 mode, double rot, double scale);

    extern f32 float_deg2rad_804274c0;
    extern f32 float_6p2832_804274f4;
    extern f32 float_360_804274f8;
    extern f32 float_2p05_804274fc;
    extern f32 float_2_804274d8;
    extern f32 float_0_804274d4;
    extern f32 float_10_80427500;
    extern f32 float_2p1_80427504;
    extern VecRaw vec3_802fedc4[];
    extern u32 dat_804274ac;
    extern u32 dat_804274b0;

    MtxRaw mtxA;
    MtxRaw mtxD;
    MtxRaw mtxB;
    MtxRaw mtxC;
    VecRaw axis;
    VecRaw axis0;
    u32 color0;
    u32 color1;
    void* cam;
    void* work;
    f32 camAngle;
    f32 deg;
    f32 scale;
    s32 i;

    work = *(void**)((s32)eff + 0xC);

    camGetPtr(cameraId);
    PSMTXTrans(mtxA,
        (double)*(f32*)((s32)work + 0x4),
        (double)*(f32*)((s32)work + 0x8),
        (double)*(f32*)((s32)work + 0xC));

    cam = camGetPtr(cameraId);
    camAngle = *(f32*)((s32)cam + 0x114);
    deg = float_deg2rad_804274c0;
    camAngle = -camAngle;
    PSMTXRotRad(mtxB, 'y', (double)(deg * camAngle));
    PSMTXConcat(mtxA, mtxB, mtxC);

    if (*(s32*)((s32)work + 0xC4) != -1 && *(s32*)((s32)work + 0xC8) != -1) {
        axis0 = vec3_802fedc4[0];
        axis0.x = (f32)cos((double)((float_6p2832_804274f4 * *(f32*)((s32)work + 0x80)) / float_360_804274f8));
        axis0.z = (f32)sin((double)((float_6p2832_804274f4 * *(f32*)((s32)work + 0x80)) / float_360_804274f8));
        axis = axis0;

        for (i = 0; i < 2; i++) {
            if (i == 0) {
                PSMTXTrans(mtxA,
                    (double)*(f32*)((s32)work + 0x6C),
                    (double)*(f32*)((s32)work + 0x70),
                    (double)*(f32*)((s32)work + 0x74));
                camAngle = *(f32*)((s32)work + 0xB0);
                deg = float_deg2rad_804274c0;
                camAngle = -camAngle;
                PSMTXRotAxisRad(mtxB, &axis, (double)(deg * camAngle));
                scale = float_2p05_804274fc * *(f32*)((s32)work + 0x94);
                PSMTXScale(mtxD, scale, scale, scale);
                PSMTXConcat(mtxC, mtxA, mtxA);
                PSMTXConcat(mtxA, mtxB, mtxA);
                PSMTXConcat(mtxA, mtxD, mtxA);
                animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0xC4), 0x40);
                color0 = dat_804274ac;
                animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0xC4), &color0);
            } else {
                PSMTXTrans(mtxA,
                    (double)*(f32*)((s32)work + 0x6C),
                    (double)*(f32*)((s32)work + 0x70),
                    (double)*(f32*)((s32)work + 0x74));
                camAngle = *(f32*)((s32)work + 0xB0);
                deg = float_deg2rad_804274c0;
                camAngle = -camAngle;
                PSMTXRotAxisRad(mtxB, &axis, (double)(deg * camAngle));
                scale = float_2_804274d8 * *(f32*)((s32)work + 0x94);
                PSMTXScale(mtxD, scale, scale, scale);
                PSMTXConcat(mtxC, mtxA, mtxA);
                PSMTXConcat(mtxA, mtxB, mtxA);
                PSMTXConcat(mtxA, mtxD, mtxA);
                animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0xC4), 0x40);
                color1 = dat_804274b0;
                animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0xC4), &color1);
            }

            animPoseDrawMtx(*(s32*)((s32)work + 0xC4), mtxA, 1, (double)float_0_804274d4, (double)float_10_80427500);
            animPoseDrawMtx(*(s32*)((s32)work + 0xC4), mtxA, 2, (double)float_0_804274d4, (double)float_10_80427500);
            animPoseDrawMtx(*(s32*)((s32)work + 0xC4), mtxA, 3, (double)float_0_804274d4, (double)float_10_80427500);
        }

        PSMTXTrans(mtxA,
            (double)*(f32*)((s32)work + 0x6C),
            (double)*(f32*)((s32)work + 0x70),
            (double)*(f32*)((s32)work + 0x74));
        scale = float_2p1_80427504 * *(f32*)((s32)work + 0x94);
        PSMTXScale(mtxD, scale, scale, scale);
        PSMTXConcat(mtxC, mtxA, mtxA);
        PSMTXConcat(mtxA, mtxD, mtxA);
        animPoseDrawMtx(*(s32*)((s32)work + 0xC8), mtxA, 1, (double)float_0_804274d4, (double)float_10_80427500);
        animPoseDrawMtx(*(s32*)((s32)work + 0xC8), mtxA, 2, (double)float_0_804274d4, (double)float_10_80427500);
        animPoseDrawMtx(*(s32*)((s32)work + 0xC8), mtxA, 3, (double)float_0_804274d4, (double)float_10_80427500);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
void effFallDispWater(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; typedef u8 TexObj[0x20];
    extern void PSMTXTrans(Mtx,f64,f64,f64); extern void PSMTXScale(Mtx,f32,f32,f32); extern void PSMTXRotRad(Mtx,f64,char); extern void PSMTXConcat(void*,void*,void*);
    extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevOp(s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetTevColor(s32,void*); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXSetCullMode(s32); extern void GXClearVtxDesc(void); extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void effGetTexObj(s32,void*); extern void GXLoadTexObj(void*,s32); extern u16 GXGetTexObjHeight(void*); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXLoadPosMtxImm(void*,s32); extern void GXBegin(s32,s32,s32); extern void GXSetArray(s32,void*,s32); extern void GXCallDisplayList(void*,u32); extern volatile f32 DAT_cc008000;
    extern u8 shibuki_dl_1_0[], shibuki_dl_1_1[], shibuki_dl_1_2[], shibuki_dl_1_3[];
    extern u8 shibuki_dl_0_0[], shibuki_dl_0_1[], shibuki_dl_0_2[], shibuki_dl_0_3[];
    extern u8 dat_803b11a0[], dat_8041bce0[], dat_8041bd00[], dat_803b12a0[];
    extern f32 float_0p125_804274e8; extern f32 float_0p7_804274ec; extern f32 float_0p25_804274f0;
    u8* w=*(u8**)((u8*)effect+0xC); void* cam=camGetPtr(cameraId); Mtx trans,scale,rot,base; TexObj tex; u32 color; f32 halfW=32.0f; f32 height=352.0f;
    PSMTXTrans(trans,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC));PSMTXRotRad(rot,-0.017453292f**(f32*)((u8*)camGetPtr(cameraId)+0x114),'y');
    PSMTXConcat(trans,rot,base);PSMTXConcat((u8*)cam+0x11C,base,base);GXSetNumTevStages(2);GXSetTevOrder(0,0,0,4);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevOp(0,0);
    GXSetTevOrder(1,0xFF,0xFF,0xFF);GXSetTevColorOp(1,0,0,0,1,0);GXSetTevAlphaOp(1,0,0,0,1,0);GXSetTevColorIn(1,15,2,0,15);GXSetTevAlphaIn(1,7,1,0,7);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);GXSetNumChans(1);GXSetChanCtrl(4,0,0,1,0,0,2);GXSetCullMode(2);
    GXClearVtxDesc();GXSetVtxDesc(9,1);GXSetVtxDesc(11,1);GXSetVtxDesc(13,1);GXSetVtxAttrFmt(0,9,1,4,0);GXSetVtxAttrFmt(0,11,1,5,0);GXSetVtxAttrFmt(0,13,1,4,0);
    effGetTexObj(0x35,tex);GXLoadTexObj(tex,0);PSMTXTrans(trans,*(f32*)(w+0xB8),*(f32*)(w+0xBC),*(f32*)(w+0xC0));PSMTXScale(scale,1.0f,height/704.0f,1.0f);PSMTXConcat(trans,scale,trans);GXLoadTexMtxImm(trans,0x1E,1);
    PSMTXTrans(trans,*(f32*)(w+0x6C),*(f32*)(w+0x70),*(f32*)(w+0x74));PSMTXScale(scale,*(f32*)(w+0x94),1.0f,1.0f);PSMTXConcat(trans,scale,trans);PSMTXConcat(base,trans,trans);GXLoadPosMtxImm(trans,0);color=0xFFFFFFFF;GXSetTevColor(1,&color);
    GXBegin(0x80,0,4);DAT_cc008000=-halfW;DAT_cc008000=height;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=0;
    DAT_cc008000=halfW;DAT_cc008000=height;DAT_cc008000=0;DAT_cc008000=2;DAT_cc008000=0;
    DAT_cc008000=halfW;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=2;DAT_cc008000=1;
    DAT_cc008000=-halfW;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=1;
    if(*(s32*)(w+0xA0)>0){
        PSMTXTrans(trans,*(f32*)(w+0x6C),*(f32*)(w+0x70),*(f32*)(w+0x74));PSMTXScale(scale,0.56f,0.4f,0.8f);PSMTXConcat(base,trans,trans);PSMTXConcat(trans,scale,trans);GXLoadPosMtxImm(trans,0);
        PSMTXTrans(trans,*(f32*)(w+0xBC)*float_0p125_804274e8,0.0f,0.0f);GXLoadTexMtxImm(trans,0x1E,1);color=(0xFFFFFF00|*(u8*)(w+0xA4));GXSetTevColor(1,&color);effGetTexObj(0x37,tex);GXLoadTexObj(tex,0);
        GXClearVtxDesc();GXSetVtxDesc(9,2);GXSetVtxAttrFmt(0,9,1,3,8);GXSetArray(9,dat_803b11a0,6);GXSetVtxDesc(10,2);GXSetVtxAttrFmt(0,10,0,1,6);GXSetArray(10,dat_8041bce0,3);GXSetVtxDesc(11,2);GXSetVtxAttrFmt(0,11,1,5,0);GXSetArray(11,dat_8041bd00,4);GXSetVtxDesc(13,2);GXSetVtxAttrFmt(0,13,1,3,14);GXSetArray(13,dat_803b12a0,4);
        GXCallDisplayList(shibuki_dl_1_0,0x20);GXCallDisplayList(shibuki_dl_1_1,0x20);GXCallDisplayList(shibuki_dl_1_2,0x20);GXCallDisplayList(shibuki_dl_1_3,0x20);
        PSMTXTrans(trans,*(f32*)(w+0x6C),*(f32*)(w+0x70),*(f32*)(w+0x74));halfW=*(f32*)(w+0x94)*0.8f;PSMTXScale(scale,float_0p7_804274ec*halfW,0.5f*halfW,halfW);PSMTXConcat(base,trans,trans);PSMTXConcat(trans,scale,trans);GXLoadPosMtxImm(trans,0);PSMTXTrans(trans,0.0f,-*(f32*)(w+0xBC)*float_0p25_804274f0,0.0f);GXLoadTexMtxImm(trans,0x1E,1);color=0xFFFFFFFF;GXSetTevColor(1,&color);effGetTexObj(0x36,tex);GXLoadTexObj(tex,0);
        GXClearVtxDesc();GXSetVtxDesc(9,2);GXSetVtxAttrFmt(0,9,1,3,8);GXSetArray(9,dat_803b11a0,6);GXSetVtxDesc(10,2);GXSetVtxAttrFmt(0,10,0,1,6);GXSetArray(10,dat_8041bce0,3);GXSetVtxDesc(11,2);GXSetVtxAttrFmt(0,11,1,5,0);GXSetArray(11,dat_8041bd00,4);GXSetVtxDesc(13,2);GXSetVtxAttrFmt(0,13,1,3,14);GXSetArray(13,dat_803b12a0,4);
        GXCallDisplayList(shibuki_dl_0_0,0x40);GXCallDisplayList(shibuki_dl_0_1,0x40);GXCallDisplayList(shibuki_dl_0_2,0x40);GXCallDisplayList(shibuki_dl_0_3,0x40);
    }
}

s32 effFallCheckHit(void* eff) {
    void* work = *(void**)((s32)eff + 0xC);

    switch (*(s32*)work) {
        case 0:
        case 1:
        case 4:
        case 5:
            if (*(s32*)((s32)work + 0xA0) >= 2) {
                return 1;
            }
            break;
        case 2:
        case 3:
            if (*(s32*)((s32)work + 0xA0) >= 1) {
                return 1;
            }
            break;
        case 6:
            if (*(s32*)((s32)work + 0xA0) < 2) {
                if (*(s32*)((s32)work + 0xA0) != 1) {
                    break;
                }
                if (*(s32*)((s32)work + 0x8C) <= 90) {
                    break;
                }
            }
            return 1;
        case 7:
            if (*(s32*)((s32)work + 0xA0) >= 4) {
                return 1;
            }
            break;
        case 8:
            if (*(s32*)((s32)work + 0xA0) >= 3) {
                return 1;
            }
            break;
    }
    return 0;
}
