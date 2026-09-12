#include "action/ac_shot_target.h"

#include "system.h"

extern f32 float_neg200_80427e74;
extern f32 float_0_80427e78;
extern u32 dat_80427e58;
extern f32 float_2_80427e6c;

void _ac_disp_init(void* wp);
void _ac_rumble_param_set(u32 type, s16* frame1, s16* frame2, f32* strength);
void actionCommandDisp(f32 x, f32 y);

/* stub-fill: battleAcMain_ShotTarget | missing_definition | ghidra_signature */
s32 battleAcMain_ShotTarget(void* battleWork) {
    extern f32 vec3_80300258[];
    extern void* BattleGetUnitPtr(void*, s32);
    extern void* BtlUnit_GetPartsPtr(void*, s32);
    extern void BtlUnit_GetHitPos(void*, void*, f32*, f32*, f32*);
    extern s32 psndSFXOn(char*);
    extern void psndSFXOff(s32);
    extern f64 sinfd(f64);
    extern f64 distABf(f64, f64, f64, f64);
    extern f32 __fabsf(f32);
    extern const char str_SFX_AC_CURSOR_MOVE1_803002c4[];

    u8* bw = battleWork;
    u8* extra = bw + 0x1F4C;
    void* acUnit = *(void**)(bw + 0x1C90);
    s32 autoCommand = *(u8*)((u8*)acUnit + 0x307) != 0;
    u32 rumbleType = *(u32*)(bw + 0x1CD4);
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    s32 state;
    s32 immediateExit;
    s16 timer;
    f32 oldValue;
    f32 newValue;

    for (;;) {
        state = *(s32*)(bw + 0x1C9C);
        immediateExit = 0;

        switch (state) {
            case 0:
                _ac_disp_init(battleWork);
                *(s32*)(bw + 0x1CB8) = 1;
                acUnit = BattleGetUnitPtr(battleWork, *(s32*)(bw + 0x1CC8));
                BtlUnit_GetHitPos(acUnit,
                                  BtlUnit_GetPartsPtr(acUnit, *(s32*)(bw + 0x1CCC)),
                                  &hitX, &hitY, &hitZ);
                *(f32*)(extra + 0x08) = hitX;
                *(f32*)(extra + 0x0C) = hitY;
                *(f32*)(extra + 0x10) = hitZ;
                *(s32*)(extra + 0x20) = 0;
                *(f32*)(extra + 0x14) = vec3_80300258[0];
                *(f32*)(extra + 0x18) = vec3_80300258[1];
                *(f32*)(extra + 0x1C) = vec3_80300258[2];
                *(f32*)(extra + 0x24) = 0.0f;
                _ac_rumble_param_set(rumbleType, (s16*)(extra + 0x28),
                                     (s16*)(extra + 0x2A), (f32*)(extra + 0x2C));
                _ac_rumble_param_set(rumbleType, (s16*)(extra + 0x30),
                                     (s16*)(extra + 0x32), (f32*)(extra + 0x34));
                *(s32*)(extra + 0x38) = -1;
                *(s32*)(bw + 0x1C9C) = 99;
                break;

            case 100:
                *(s32*)extra = *(s32*)(bw + 0x1CD0);
                *(s32*)(bw + 0x1C9C) = 1000;
                *(s32*)(extra + 0x38) = psndSFXOn((char*)str_SFX_AC_CURSOR_MOVE1_803002c4);
                /* Fall through into the active cursor update. */
            case 1000:
                timer = *(s16*)(bw + 0x1D18);
                if (timer > 0) {
                    *(s16*)(bw + 0x1D18) = timer - 1;
                    immediateExit = 1;
                    *(s32*)(bw + 0x1CB8) = 2;
                    (*(s32*)(bw + 0x1CB4))++;
                    *(s32*)(bw + 0x1C9C) = 1001;
                    break;
                }

                (*(s16*)(extra + 0x2A))--;
                if (*(s16*)(extra + 0x2A) < 0) {
                    _ac_rumble_param_set(rumbleType, (s16*)(extra + 0x28),
                                         (s16*)(extra + 0x2A), (f32*)(extra + 0x2C));
                }
                *(f32*)(extra + 0x14) +=
                    *(f32*)(extra + 0x2C) *
                        (f32)sinfd((f32)(*(s16*)(extra + 0x2A) * 180 /
                                         *(s16*)(extra + 0x28))) -
                    *(f32*)(extra + 0x2C) *
                        (f32)sinfd((f32)((*(s16*)(extra + 0x2A) + 1) * 180 /
                                         *(s16*)(extra + 0x28)));

                (*(s16*)(extra + 0x32))--;
                if (*(s16*)(extra + 0x32) < 0) {
                    _ac_rumble_param_set(rumbleType, (s16*)(extra + 0x30),
                                         (s16*)(extra + 0x32), (f32*)(extra + 0x34));
                }
                *(f32*)(extra + 0x18) +=
                    *(f32*)(extra + 0x34) *
                        (f32)sinfd((f32)(*(s16*)(extra + 0x32) * 180 /
                                         *(s16*)(extra + 0x30))) -
                    *(f32*)(extra + 0x34) *
                        (f32)sinfd((f32)((*(s16*)(extra + 0x32) + 1) * 180 /
                                         *(s16*)(extra + 0x30)));

                *(f32*)(extra + 0x14) += (f32)*(s8*)(bw + 0xF2E) / 10.0f;
                *(f32*)(extra + 0x18) += (f32)*(s8*)(bw + 0xF2F) / 10.0f;

                if (autoCommand) {
                    f32 cursor = *(f32*)(extra + 0x14);
                    f32 target = *(f32*)(extra + 0x08);
                    if (__fabs((f64)(cursor - target)) >= 3.0) {
                        *(f32*)(extra + 0x14) = cursor <= target ? cursor + 3.0f : cursor - 3.0f;
                    } else {
                        *(f32*)(extra + 0x14) = target;
                    }

                    cursor = *(f32*)(extra + 0x18);
                    target = *(f32*)(extra + 0x0C);
                    if (__fabs((f64)(cursor - target)) >= 3.0) {
                        *(f32*)(extra + 0x18) = cursor <= target ? cursor + 3.0f : cursor - 3.0f;
                    } else {
                        *(f32*)(extra + 0x18) = target;
                    }
                }

                *(s32*)(extra + 0x20) =
                    __fabsf((f32)distABf(*(f32*)(extra + 0x14), *(f32*)(extra + 0x18),
                                         *(f32*)(extra + 0x08), *(f32*)(extra + 0x0C))) <= 28.0f;
                (*(s32*)extra)--;

                if ((*(u32*)(bw + 0x1C94) & 1) == 0 ||
                    (*(u32*)((u8*)acUnit + 0x27C) & 0x10) == 0) {
                    if (*(s32*)extra < 0) {
                        if (*(s32*)(extra + 0x20) == 0) {
                            *(s32*)(bw + 0x1CB8) = 0;
                        } else {
                            *(s32*)(bw + 0x1CB8) = 2;
                            (*(s32*)(bw + 0x1CB4))++;
                        }
                        *(s32*)(bw + 0x1C9C) = 1001;
                    }
                } else {
                    if (irand(100) < 0) {
                        *(s32*)(bw + 0x1CB8) = 2;
                        (*(s32*)(bw + 0x1CB4))++;
                    } else {
                        *(s32*)(bw + 0x1CB8) = 0;
                    }
                    *(s32*)(bw + 0x1C9C) = 1001;
                }
                break;

            case 1002:
                *(s32*)(bw + 0x1C9C) = 1002;
                break;

            case 1003:
                *(s32*)(extra + 4) = 60;
                *(s32*)(bw + 0x1C9C) = 1004;
                if (*(s32*)(extra + 0x38) != -1) {
                    psndSFXOff(*(s32*)(extra + 0x38));
                }
                break;

            case 1004:
                (*(s32*)(extra + 4))--;
                if (*(s32*)(extra + 4) < 1) {
                    *(s32*)(bw + 0x1C9C) = 1005;
                }
                break;

            case 1005:
                *(void**)(bw + 0x1CA0) = 0;
                *(void**)(bw + 0x1CA4) = 0;
                *(void**)(bw + 0x1CA8) = 0;
                *(void**)(bw + 0x1CAC) = 0;
                break;
        }

        if (!immediateExit) {
            return 1;
        }
    }
}
void _ac_rumble_param_set(u32 type, s16* frame1, s16* frame2, f32* strength) {
    extern f32 float_25_80427e80;
    extern f32 float_neg1_80427e84;
    extern f32 float_0p6_80427e88;
    extern f32 float_0p25_80427e8c;
    extern f32 float_0p5_80427e90;
    extern f32 float_0p8_80427e94;
    extern f32 float_1p5_80427e98;
    extern f32 float_3_80427e9c;

    *frame1 = irand(0x78) + 0x78;
    *frame2 = *frame1 - 1;
    *strength = float_25_80427e80;
    if (irand(0x64) < 0x32) {
        *strength = float_neg1_80427e84 * *strength;
    }

    switch (type) {
        case 0:
            *strength = float_0p6_80427e88 * (float_0p25_80427e8c * *strength);
            break;
        case 1:
            *strength = float_0p6_80427e88 * (float_0p5_80427e90 * *strength);
            break;
        case 2:
            *strength = float_0p6_80427e88 * (float_0p8_80427e94 * *strength);
            break;
        case 3:
            *strength = float_0p6_80427e88 * *strength;
            break;
        case 4:
            *strength = float_0p6_80427e88 * (float_1p5_80427e98 * *strength);
            break;
        case 5:
            *strength = float_0p6_80427e88 * (float_2_80427e6c * *strength);
            break;
        case 6:
        default:
            *strength = float_0p6_80427e88 * (float_3_80427e9c * *strength);
            break;
    }
}

s32 battleAcResult_ShotTarget(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_ShotTarget(s32 unused, void* wp) {
    extern f32 float_30_80427e7c;
    extern f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);

    s32 state;
    s32 timer;

    state = *(s32*)((s32)wp + 0x1C9C);
    wp = (void*)((s32)wp + 0x1F20);
    if (state >= 1000) {
        goto ge_1000;
    }
    if (state >= 101) {
        return;
    }
    if (state >= 99) {
        goto disp_in;
    }
    return;

ge_1000:
    if (state >= 1005) {
        return;
    }
    if (state >= 1003) {
        goto disp_out;
    }
disp_in:
    timer = *(s32*)((s32)wp + 0x20);
    *(f32*)((s32)wp + 0x14) = intplGetValue(4, 0x14 - timer, float_neg200_80427e74, float_30_80427e7c, 0x14);
    actionCommandDisp(*(f32*)((s32)wp + 0x14), *(f32*)((s32)wp + 0x18));
    timer = *(s32*)((s32)wp + 0x20);
    if (timer > 0) {
        *(s32*)((s32)wp + 0x20) = timer - 1;
    }
    return;

disp_out:
    timer = *(s32*)((s32)wp + 0x20);
    if (timer >= 40) {
        *(f32*)((s32)wp + 0x14) = intplGetValue(4, timer - 40, float_30_80427e7c, float_neg200_80427e74, 0x14);
    } else {
        *(f32*)((s32)wp + 0x14) = float_30_80427e7c;
    }
    actionCommandDisp(*(f32*)((s32)wp + 0x14), *(f32*)((s32)wp + 0x18));
    timer = *(s32*)((s32)wp + 0x20);
    if (timer < 60) {
        *(s32*)((s32)wp + 0x20) = timer + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void battleAcDelete_ShotTarget(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1003;
}

void _ac_disp_init(void* wp) {
    void* disp = (void*)((s32)wp + 0x1F20);
    f32 y;
    f32 zero;
    s8 alpha;

    memset(disp, 0, 0x2C);
    y = float_neg200_80427e74;
    zero = float_0_80427e78;
    alpha = -0x10;
    *(f32*)((s32)disp + 0x14) = y;
    *(f32*)((s32)disp + 0x18) = zero;
    *(s32*)((s32)disp + 0x20) = 0x14;
    *(u32*)((s32)disp + 0xC) = dat_80427e58;
    *(u8*)((s32)disp + 0x10) = 0;
    *(u8*)((s32)disp + 0x11) = alpha;
    *(u8*)((s32)disp + 0x12) = alpha;
    *(u8*)((s32)disp + 0x13) = 0;
    *(f32*)((s32)disp + 0x28) = zero;
}


/* CHATGPT STUB FILL: main/action/ac_shot_target 20260624_184929 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    typedef struct Vec3Local {
        f32 x;
        f32 y;
        f32 z;
    } Vec3Local;
    typedef void (*BtlDispTex4ByValue)(s32, Vec3Local, Vec3Local, Vec3Local, u32*);
    extern f32 vec3_80300258[];
    extern void* _battleWorkPointer;
    extern void btlGetScreenPoint(f32* in, f32* out);
    extern void btlDispTex4(s32 tex, f32* pos, f32* v0, f32* v1, u32* color);
    extern u32 dat_80427e5c;
    extern u32 dat_80427e60;
    extern u32 dat_80427e64;
    extern u32 dat_80427e68;
    extern f32 float_4_80427e70;

    void* battleWork;
    u8* work;
    f32* vectors;
    Vec3Local p0;
    Vec3Local p1;
    Vec3Local a2;
    Vec3Local b2;
    Vec3Local a3;
    Vec3Local b3;
    u32 color0;
    u32 color1;
    u32 color2;
    u32 color3;
    s32 state;

    battleWork = _battleWorkPointer;
    work = (u8*)battleWork + 0x1F4C;
    vectors = vec3_80300258;
    state = *(s32*)((s32)battleWork + 0x1C9C);
    if (state >= 1000) {
        if (state >= 1003) {
            return;
        }
    } else if (state != 100) {
        return;
    }

    *(f32*)(work + 0x24) += float_2_80427e6c;
    btlGetScreenPoint((f32*)(work + 0x14), (f32*)&p0);
    color0 = dat_80427e5c;
    ((BtlDispTex4ByValue)btlDispTex4)(0x58, p0,
                                     *(Vec3Local*)&vectors[3],
                                     *(Vec3Local*)&vectors[6], &color0);

    p0.x += float_4_80427e70;
    p0.y -= float_4_80427e70;
    p0.z -= float_2_80427e6c;
    color1 = dat_80427e60;
    ((BtlDispTex4ByValue)btlDispTex4)(0x58, p0,
                                     *(Vec3Local*)&vectors[9],
                                     *(Vec3Local*)&vectors[12], &color1);

    btlGetScreenPoint((f32*)(work + 8), (f32*)&p1);
    a2 = *(Vec3Local*)&vectors[15];
    a2.z = *(f32*)(work + 0x24);
    b2 = *(Vec3Local*)&vectors[18];
    b2.z = *(f32*)(work + 0x24);
    color2 = dat_80427e64;
    ((BtlDispTex4ByValue)btlDispTex4)(0x57, p1, a2, b2, &color2);

    p1.x += float_4_80427e70;
    p1.y -= float_4_80427e70;
    p1.z -= float_2_80427e6c;
    a3 = *(Vec3Local*)&vectors[21];
    a3.z = *(f32*)(work + 0x24);
    b3 = *(Vec3Local*)&vectors[24];
    b3.z = *(f32*)(work + 0x24);
    color3 = dat_80427e68;
    ((BtlDispTex4ByValue)btlDispTex4)(0x57, p1, a3, b3, &color3);
}

const f32 vec3_80300258[] = { 0.0f, 60.0f, 0.0f };
const f32 vec3_80300264[] = { 1.0f, 1.0f, 1.0f };
const f32 vec3_80300270[] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_8030027c[] = { 1.0f, 1.0f, 1.0f };
const f32 vec3_80300288[] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_80300294[] = { 1.0f, 1.0f, 1.0f };
const f32 vec3_803002a0[] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_803002ac[] = { 1.0f, 1.0f, 1.0f };
const f32 vec3_803002b8[] = { 0.0f, 0.0f, 0.0f };
const char str_SFX_AC_CURSOR_MOVE1_803002c4[] = "SFX_AC_CURSOR_MOVE1";
