#include "action/ac_signal_timing.h"

typedef struct SignalTimingVec {
    f32 x;
    f32 y;
    f32 z;
} SignalTimingVec;

extern void* _battleWorkPointer;

u32 _button_table_4dir[4] = {
    0x00010000, 0x00020000, 0x00040000, 0x00080000
};
u32 _button_table_abxy[4] = {
    0x00000100, 0x00000200, 0x00000400, 0x00000800
};
s32 ac_signaltiming_ok_frame_range[4];
s32 ac_signaltiming_ok_frame_range_2[4];

extern const SignalTimingVec vec3_802ff6d0;
extern const SignalTimingVec vec3_802ff6dc;
extern const SignalTimingVec vec3_802ff6e8;
extern const SignalTimingVec vec3_802ff6f4;
extern const SignalTimingVec vec3_802ff700;
extern const SignalTimingVec vec3_802ff70c;
extern const SignalTimingVec vec3_802ff718;
extern const SignalTimingVec vec3_802ff724;
extern const SignalTimingVec vec3_802ff730;
extern const SignalTimingVec vec3_802ff73c;
extern const SignalTimingVec vec3_802ff748;
extern const SignalTimingVec vec3_802ff754;
extern const SignalTimingVec vec3_802ff760;
extern const SignalTimingVec vec3_802ff76c;

extern const u32 dat_80427c30;
extern const u32 dat_80427c34;
extern const u32 dat_80427c38;
extern const u32 dat_80427c3c;
extern const f32 float_neg200_80427c40;
extern const f32 float_70_80427c44;
extern const f32 float_1_80427c48;
extern const f32 float_neg288_80427c4c;
extern const f32 float_25_80427c50;
extern const f32 float_0_80427c54;
extern const f32 float_1p5_80427c58;
extern const f32 float_0p5_80427c5c;
extern const f32 float_15_80427c60;
extern const f32 float_0p8_80427c64;
extern const f32 float_0p6_80427c68;
extern const f32 float_20p8_80427c6c;
extern const f32 float_neg300_80427c70;
extern const f32 float_30_80427c74;

static void actionCommandDisp(f32 x, f32 y);

s32 _get_ok_frame_range(s32 idx) {
    void* wp = _battleWorkPointer;

    switch (*(s32*)((s32)wp + 0x1CCC)) {
        case 1:
            return *(s32*)((s32)wp + 0x1CE4) + ac_signaltiming_ok_frame_range_2[idx];
        default:
            return *(s32*)((s32)wp + 0x1CE4) + ac_signaltiming_ok_frame_range[idx];
    }
}

/* stub-fill: battleAcMain_SignalTiming | missing_definition | ghidra_signature */
s32 battleAcMain_SignalTiming(int work) {
    extern s32 BattleActionCommandGetDifficulty(void*);
    extern s32 BattlePadCheckTrigger(u32);
    extern s32 BattlePadGetTrigger(void);
    extern s32 irand(s32);
    extern void psndSFXOn(char*);
    extern u32 _button_table_4dir[4];
    extern u32 _button_table_abxy[4];
    extern const char str_SFX_AC_COMMAND_NG1_802ff7b0[];
    extern const char str_SFX_AC_COMMAND_OK1_802ff79c[];
    extern const char str_SFX_AC_PI1_802ff780[];
    extern const char str_SFX_AC_PONE1_802ff78c[];
    u8* wp = (u8*)work;
    s32* extra = (s32*)(wp + 0x1F4C);
    u8* disp = wp + 0x1F20;
    char* sfxNg = (char*)str_SFX_AC_COMMAND_NG1_802ff7b0;
    s32 state;
    s32 i;
    s32 j;
    s32 button;
    s32 frame;
    s32 index;
    s32 first;
    s32 count;
    s32 done;
    s32 unique;
    s32 valid;
    s32 pressed;
    s32 demo = *(u8*)(*(s32*)(wp + 0x1C90) + 0x307) != 0;

    do {
        state = *(s32*)(wp + 0x1C9C);
        done = FALSE;
        switch (state) {
            case 0:
                *(s32*)(wp + 0x1CB8) = 1;
                memset(disp, 0, 0x2C);
                *(f32*)(disp + 0x14) = float_neg300_80427c70;
                *(f32*)(disp + 0x18) = float_0_80427c54;
                *(s32*)(disp + 0x20) = 20;
                extra[0] = 0;
                extra[1] = 0;
                extra[4] = 0;
                extra[2] = 0;
                extra[3] = 0;
                extra[11] = BattleActionCommandGetDifficulty(wp);
                extra[12] = 0;
                *(s32*)(wp + 0x1CEC) = 0;
                for (i = 0; i < 5; i++) {
                    ((u8*)extra)[0x4C + i] = 0;
                }
                if (*(s32*)(wp + 0x1CD8) > 0 && *(s32*)(wp + 0x1CD8) < 3) {
                    for (i = 0; i < *(s32*)(wp + 0x1CC8); i++) {
                        do {
                            if (*(s32*)(wp + 0x1CD8) == 1) {
                                button = _button_table_4dir[irand(4)];
                            } else {
                                button = _button_table_abxy[irand(4)];
                            }
                            unique = TRUE;
                            for (j = i - 1; j >= 0; j--) {
                                if (extra[0xE + j] == button) {
                                    unique = FALSE;
                                }
                            }
                        } while (!unique);
                        extra[0xE + i] = button;
                    }
                    *(s32*)(wp + 0x1F80) = *(s32*)(wp + 0x1F84);
                } else {
                    *(s32*)(wp + 0x1F80) = 0x100;
                    for (i = 0; i < *(s32*)(wp + 0x1CC8); i++) {
                        extra[0xE + i] = 0x100;
                    }
                    *(s32*)(wp + 0x1F80) = *(s32*)(wp + 0x1F84);
                }
                *(s32*)(wp + 0x1C9C) = 99;
                break;

            case 99:
                if (*(s16*)(wp + 0x1D18) < 1) {
                    index = *(s32*)(wp + 0x1F54);
                    *(s32*)(wp + 0x1F60) = index * *(s32*)(wp + 0x1CD0);
                    *(s32*)(wp + 0x1F64) = (index + 1) * *(s32*)(wp + 0x1CD0) - 1;
                    *(s32*)(wp + 0x1F68) = *(s32*)(wp + 0x1F60);
                    *(s32*)(wp + 0x1F6C) = *(s32*)(wp + 0x1F68) + _get_ok_frame_range(*(s32*)(wp + 0x1F78)) - 1;
                    *(s32*)(wp + 0x1C9C) = 1000;
                } else {
                    (*(s16*)(wp + 0x1D18))--;
                    *(s32*)(wp + 0x1CB8) = 2;
                    *(s32*)(wp + 0x1C9C) = 1002;
                    done = TRUE;
                }
                break;

            case 1000:
                *(s32*)(wp + 0x1F74) = (*(s32*)(wp + 0x1F58) + 1) * *(s32*)(wp + 0x1CD0) - 1;
                (*(s32*)(wp + 0x1F58))++;
                if ((*(u32*)(wp + 0x1C94) & 1) == 0 ||
                    (*(u32*)(*(s32*)(wp + 0x1C90) + 0x27C) & 0x10) == 0) {
                    if (*(s32*)(wp + 0x1F58) < *(s32*)(wp + 0x1CE0) + 1) {
                        psndSFXOn((char*)str_SFX_AC_PI1_802ff780);
                    } else {
                        psndSFXOn((char*)str_SFX_AC_PONE1_802ff78c);
                    }
                }
                *(s32*)(wp + 0x1F70) = 0;
                *(s32*)(wp + 0x1C9C) = 1001;
                /* fall through */

            case 1001:
                (*(s32*)(wp + 0x1F5C))++;
                frame = *(s32*)(wp + 0x1F5C);
                if (*(s32*)(wp + 0x1F70) == 0 &&
                    *(s32*)(wp + 0x1F54) >= *(s32*)(wp + 0x1CE0) &&
                    frame >= *(s32*)(wp + 0x1F68) && frame <= *(s32*)(wp + 0x1F6C)) {
                    *(s32*)(wp + 0x1F7C) = 1;
                } else {
                    *(s32*)(wp + 0x1F7C) = 0;
                }

                pressed = BattlePadCheckTrigger(*(u32*)(wp + 0x1F80)) != 0;
                valid = pressed;
                if (pressed && *(s32*)(wp + 0x1CD8) == 2) {
                    valid = (*(s32*)(wp + 0x1F80) == BattlePadGetTrigger());
                }
                if (demo && *(s32*)(wp + 0x1F7C)) {
                    pressed = TRUE;
                    valid = TRUE;
                }
                if (*(s32*)(wp + 0x1CC8) <= extra[0] + *(s32*)(wp + 0x1F50)) {
                    pressed = FALSE;
                }

                index = *(s32*)(wp + 0x1F54);
                first = *(s32*)(wp + 0x1CE0);
                count = *(s32*)(wp + 0x1CC8);
                if (index >= first && pressed && *(s32*)(wp + 0x1F70) == 0) {
                    *(s32*)(wp + 0x1F70) = 1;
                    if (frame < *(s32*)(wp + 0x1F68) || frame > *(s32*)(wp + 0x1F6C) || !valid) {
                        if (index < first + count) {
                            (*(s32*)(wp + 0x1F50))++;
                            if (frame < *(s32*)(wp + 0x1F60)) {
                                *(s32*)(wp + 0x1F70) = 0;
                            } else {
                                ((u8*)extra)[0x4C + index - first] = 0xFF;
                                if ((*(u32*)(wp + 0x1C94) & 1) == 0 ||
                                    (*(u32*)(*(s32*)(wp + 0x1C90) + 0x27C) & 0x10) == 0) {
                                    psndSFXOn(sfxNg);
                                }
                                *(s32*)(wp + 0x1CEC) = -1;
                            }
                        }
                    } else {
                        extra[0]++;
                        *(s32*)(wp + 0x1CEC) = 1;
                        if (index < first + count) {
                            ((u8*)extra)[0x4C + index - first] = 1;
                            if ((*(u32*)(wp + 0x1C94) & 1) == 0 ||
                                (*(u32*)(*(s32*)(wp + 0x1C90) + 0x27C) & 0x10) == 0) {
                                psndSFXOn((char*)str_SFX_AC_COMMAND_OK1_802ff79c);
                            }
                        }
                        if (*(u32*)(wp + 0x1CC4) & 0x10) {
                            *(s32*)(wp + 0x1CE8) = extra[0];
                            *(u32*)(wp + 0x1CB8) &= ~1;
                            if (extra[0] >= *(s32*)(wp + 0x1CD4)) {
                                *(u32*)(wp + 0x1CB8) |= 2;
                            }
                            done = TRUE;
                            *(s32*)(wp + 0x1C9C) = 1002;
                        }
                    }
                }

                if (*(s32*)(wp + 0x1F70) == 0 && frame > *(s32*)(wp + 0x1F6C)) {
                    *(s32*)(wp + 0x1F70) = 1;
                    *(s32*)(wp + 0x1CEC) = -1;
                    if (index >= first && index < first + count) {
                        (*(s32*)(wp + 0x1F50))++;
                        ((u8*)extra)[0x4C + index - first] = 0xFF;
                        if ((*(u32*)(wp + 0x1C94) & 1) == 0 ||
                            (*(u32*)(*(s32*)(wp + 0x1C90) + 0x27C) & 0x10) == 0) {
                            psndSFXOn(sfxNg);
                        }
                    }
                }

                if (*(s32*)(wp + 0x1F70) && index < first + count) {
                    index = ++*(s32*)(wp + 0x1F54);
                    if (index < first + count) {
                        *(s32*)(wp + 0x1F60) = index * *(s32*)(wp + 0x1CD0) - 10;
                        *(s32*)(wp + 0x1F64) = (index + 1) * *(s32*)(wp + 0x1CD0) - 1;
                        *(s32*)(wp + 0x1F68) = index * *(s32*)(wp + 0x1CD0);
                        *(s32*)(wp + 0x1F6C) = *(s32*)(wp + 0x1F68) + _get_ok_frame_range(*(s32*)(wp + 0x1F78)) - 1;
                        *(s32*)(wp + 0x1F70) = 0;
                        if (index >= first) {
                            *(s32*)(wp + 0x1F80) = extra[0xE + index - first];
                        }
                    }
                }
                if (*(s32*)(wp + 0x1F58) * *(s32*)(wp + 0x1CD0) <= frame) {
                    if (*(s32*)(wp + 0x1F58) < first + count) {
                        *(s32*)(wp + 0x1C9C) = 1000;
                    } else {
                        *(s32*)(wp + 0x1CE8) = extra[0];
                        *(u32*)(wp + 0x1CB8) &= ~1;
                        if (extra[0] >= *(s32*)(wp + 0x1CD4)) {
                            *(u32*)(wp + 0x1CB8) |= 2;
                        }
                        done = TRUE;
                        *(s32*)(wp + 0x1C9C) = 1002;
                    }
                }
                break;

            case 1002:
                *(s32*)(wp + 0x1C9C) = 1003;
                break;
            case 1003:
                *(u32*)(wp + 0x1CC0) |= 1;
                *(s32*)(wp + 0x1F7C) = 0;
                *(s32*)(wp + 0x1FA0) = 60;
                *(s32*)(wp + 0x1C9C) = 1005;
                break;
            case 1005:
                if (--*(s32*)(wp + 0x1FA0) < 1) {
                    *(s32*)(wp + 0x1C9C) = 1006;
                }
                break;
            case 1006:
                *(s32*)(wp + 0x1CA0) = 0;
                *(s32*)(wp + 0x1CA8) = 0;
                *(s32*)(wp + 0x1CA4) = 0;
                *(s32*)(wp + 0x1CAC) = 0;
                break;
        }
        if (!done) {
            return 1;
        }
    } while (TRUE);
}

s32 battleAcResult_SignalTiming(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_SignalTiming(void* camera, void* wp) {
    typedef struct SignalTimingDispWork {
        u8 pad_00[0x14];
        f32 x;
        f32 y;
        u8 pad_1C[4];
        s32 timer;
    } SignalTimingDispWork;
    SignalTimingDispWork* disp;
    s32 state;
    s32 timer;

    extern const f32 float_neg300_80427c70;
    extern const f32 float_30_80427c74;
    extern const f32 float_0_80427c54;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    disp = (SignalTimingDispWork*)((s32)wp + 0x1F20);
    state = *(s32*)((s32)wp + 0x1C9C);

    switch (state) {
        case 99:
        case 100:
            disp->x = intplGetValue(
                4,
                0x14 - disp->timer,
                0x14,
                float_neg300_80427c70,
                float_30_80427c74);
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer > 0) {
                disp->timer = timer - 1;
            }
            break;
        case 1000:
        case 1001:
        case 1002:
        case 1003:
            actionCommandDisp(float_30_80427c74, float_0_80427c54);
            break;
        case 1004:
        case 1005:
            timer = disp->timer;
            if (timer >= 0x28) {
                disp->x = intplGetValue(
                    4,
                    timer - 0x28,
                    0x14,
                    float_30_80427c74,
                    float_neg300_80427c70);
            } else {
                disp->x = float_30_80427c74;
            }
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer < 0x3C) {
                disp->timer = timer + 1;
            }
            break;
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void battleAcDelete_SignalTiming(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1004;
}

/* CHATGPT STUB FILL: main/action/ac_signal_timing 20260624_184823 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
static void actionCommandDisp(f32 x, f32 y) {
    extern void* camGetPtr(s32);
    extern void iconDispGx(f32, SignalTimingVec*, s32, s32);
    extern void iconNumberDispGx(void*, s32, s32, void*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern s32 BattleACGetButtonIcon(u32, s32);
    extern void btlDispTexPlane(f32, f32, f32, f32, f32, s32, void*, s32);

    u8* battle;
    u8* extra;
    s32 kind;
    s32 count;
    s32 filled;
    s32 span;
    s32 i;
    SignalTimingVec prompt0, prompt1;
    SignalTimingVec barStart, barMiddle, barEnd;
    SignalTimingVec filledPos, filledFallback, emptyPos;
    SignalTimingVec passiveHit, passiveMiss;
    SignalTimingVec timedHit, timedMiss;
    SignalTimingVec holdHit, holdMiss;
    const SignalTimingVec* templates = &vec3_802ff6d0;
    f32 baseY;
    f32 trans[3][4];
    f32 scale[3][4];
    f32 model[3][4];
    u32 numberColor;
    u32 guideColor;
    u32 goodColor;
    u32 badColor;
    s32 yOffset;

    battle = (u8*)_battleWorkPointer;
    camGetPtr(8);
    extra = battle + 0x1F4C;
    kind = *(s32*)(battle + 0x1CC8);
    filled = *(s32*)(battle + 0x1F58);
    if ((*(u32*)(battle + 0x1CC4) & 1) == 0) {
        if (*(s32*)(extra + 0x30) != 0) {
            prompt0 = templates[0];
            prompt0.x = x + float_neg200_80427c40;
            prompt0.y = y + float_70_80427c44;
            iconDispGx(float_1_80427c48, &prompt0, 0x10, 0x6D);
        } else {
            prompt1 = templates[1];
            prompt1.x = x + float_neg200_80427c40;
            prompt1.y = y + float_70_80427c44;
            iconDispGx(float_1_80427c48, &prompt1, 0x10, 0x6C);
        }
    }

    switch (kind) {
        case 1:
            span = *(s32*)(battle + 0x1CE0) * 2 + 5;
            break;
        case 3:
            span = *(s32*)(battle + 0x1CE0) * 2 + 0xB;
            break;
        case 4:
            span = *(s32*)(battle + 0x1CE0) * 2 + 0xD;
            break;
        case 5:
            span = *(s32*)(battle + 0x1CE0) * 2 + 0xF;
            break;
        default:
            span = *(s32*)(battle + 0x1CE0) * 2 + 0xB;
            break;
    }
    baseY = y + float_25_80427c50;
    barStart = templates[2];
    barStart.x = x + float_neg288_80427c4c;
    barStart.y = baseY;
    iconDispGx(float_1_80427c48, &barStart, 0x10, 0x97);
    for (i = 1; i < span; i++) {
        barMiddle = templates[3];
        barMiddle.x = x + (f32)(i * 16 - 0x120);
        barMiddle.y = baseY;
        iconDispGx(float_1_80427c48, &barMiddle, 0x10, 0x96);
    }
    barEnd = templates[4];
    barEnd.x = x + (f32)(span * 16 - 0x120);
    barEnd.y = baseY;
    iconDispGx(float_1_80427c48, &barEnd, 0x10, 0x98);

    if ((*(u32*)(battle + 0x1CC4) & 4) == 0) {
        s32 numberValue;
        s32 numberOffset;

        numberValue = *(s32*)extra + *(s32*)(battle + 0x1CDC);

        if ((*(u32*)(battle + 0x1CC4) & 1) == 0) {
            numberOffset =
                kind * 0x30 + *(s32*)(battle + 0x1CE0) * 0x20 - 0x1A;
        } else {
            numberOffset =
                kind * 0x30 + *(s32*)(battle + 0x1CE0) * 0x20 - 0x1A;
        }

        PSMTXTrans(
            trans,
            x + float_neg200_80427c40 + (f32)numberOffset,
            baseY,
            float_0_80427c54);

        if ((*(u32*)(battle + 0x1CC4) & 8) != 0) {
            PSMTXScale(
                scale,
                float_1p5_80427c58,
                float_1p5_80427c58,
                float_1p5_80427c58);
        } else {
            PSMTXScale(
                scale,
                float_1_80427c48,
                float_1_80427c48,
                float_1_80427c48);
        }

        PSMTXConcat(trans, scale, model);
        numberColor = dat_80427c30;
        iconNumberDispGx(model, numberValue, 0, &numberColor);
    }

    {
        s32 slot = 0;
        s32 slotOffset = 0;

        while (slot < *(s32*)(battle + 0x1CE0)) {
            if (slot < *(s32*)(extra + 0xC)) {
                /*
                 * Keep the target's second count check.  It looks redundant
                 * semantically, but the count lives in mutable battle work and
                 * the emitted target keeps a separate fallback Vec/icon path.
                 */
                if (slot < *(s32*)(battle + 0x1CE0)) {
                    filledPos = templates[5];
                    filledPos.x = x + (f32)(slotOffset - 0x102);
                    filledPos.y = baseY;
                    iconDispGx(
                        float_1_80427c48,
                        &filledPos,
                        0x10,
                        (u16)(slot + 0x9A));
                } else {
                    filledFallback = templates[6];
                    filledFallback.x = x + (f32)(slotOffset - 0x102);
                    filledFallback.y = baseY;
                    iconDispGx(
                        float_1_80427c48,
                        &filledFallback,
                        0x10,
                        0x9C);
                }
            } else {
                emptyPos = templates[7];
                emptyPos.x = x + (f32)(slotOffset - 0x102);
                emptyPos.y = baseY;
                iconDispGx(
                    float_0p5_80427c5c,
                    &emptyPos,
                    0x10,
                    0x99);
            }

            slot++;
            slotOffset += 0x21;
        }
    }
    if ((*(u32*)(battle + 0x1CC4) & 1) == 0) {
        for (i = 0; i < kind; i++) {
            if (i + *(s32*)(battle + 0x1CE0) < filled) {
                passiveHit = templates[8];
                passiveHit.x = x + (f32)(*(s32*)(battle + 0x1CE0) * 0x21 + i * 0x30 - 0xFA);
                passiveHit.y = baseY;
                iconDispGx(float_1_80427c48, &passiveHit, 0x10, 0x9D);
            } else {
                passiveMiss = templates[9];
                passiveMiss.x = x + (f32)(*(s32*)(battle + 0x1CE0) * 0x21 + i * 0x30 - 0xFA);
                passiveMiss.y = baseY;
                iconDispGx(float_1_80427c48, &passiveMiss, 0x10, 0x99);
            }
        }
    } else if ((*(u32*)(battle + 0x1CC4) & 2) == 0) {
        s32* buttons = (s32*)(extra + 0x38);
        s32 tick = *(s32*)(extra + 0x10);
        f32 timingY = y + float_15_80427c60;
        for (i = 0; i < kind; i++) {
            s32 index = i + *(s32*)(battle + 0x1CE0);
            s32 target = *(s32*)(battle + 0x1CD0) * index;
            s32 pressed;
            f32 iconScale;
            if (filled == index && tick >= target - 5 && tick < target) {
                pressed = 0;
                iconScale = float_0p8_80427c64;
                timedHit = templates[10];
                timedHit.x = x + (f32)(*(s32*)(battle + 0x1CE0) * 0x21 + i * 0x30 - 0xFA);
                timedHit.y = timingY;
                iconDispGx(iconScale, &timedHit, 0x10,
                           BattleACGetButtonIcon(buttons[i], pressed));
            } else {
                pressed = 1;
                timedMiss = templates[11];
                if (index < filled) {
                    yOffset = -10;
                    iconScale = float_0p8_80427c64;
                } else {
                    yOffset = -2;
                    iconScale = float_0p6_80427c68;
                }
                timedMiss.y = y + (f32)(yOffset + 25);
                timedMiss.x =
                    x + (f32)(*(s32*)(battle + 0x1CE0) * 0x21 + i * 0x30 - 0xFA);
                iconDispGx(iconScale, &timedMiss, 0x10,
                           BattleACGetButtonIcon(buttons[i], pressed));
            }
        }
    } else {
        s32* buttons = (s32*)(extra + 0x38);
        s32 tick = *(s32*)(extra + 0x10);
        for (i = 0; i < kind; i++) {
            s32 index = i + *(s32*)(battle + 0x1CE0);
            s32 target = *(s32*)(battle + 0x1CD0) * index;
            if (filled == index && tick >= target - 20 && tick <= target) {
                holdHit = templates[12];
                holdHit.x = x + (f32)(*(s32*)(battle + 0x1CE0) * 0x21 + i * 0x30 - 0xFA);
                holdHit.y = baseY;
                iconDispGx(float_0p8_80427c64, &holdHit, 0x10,
                           BattleACGetButtonIcon(buttons[i], 0));
            } else if (index < filled) {
                holdMiss = templates[13];
                holdMiss.x = x + (f32)(*(s32*)(battle + 0x1CE0) * 0x21 + i * 0x30 - 0xFA);
                holdMiss.y = baseY;
                iconDispGx(float_0p8_80427c64, &holdMiss, 0x10,
                           BattleACGetButtonIcon(buttons[i], 1));
            } else if (i >= kind - 1) {
                guideColor = dat_80427c34;
                btlDispTexPlane(
                    x + (f32)(*(s32*)(battle + 0x1CE0) * 0x21 + i * 0x30 - 0xFA),
                    baseY + float_20p8_80427c6c,
                    float_0_80427c54,
                    float_0p8_80427c64,
                    float_0p8_80427c64,
                    0x56, &guideColor, 0);
            }
        }
    }

    for (i = 0; i < kind; i++) {
        s8 state = *(s8*)(extra + 0x4C + i);
        f32 markerX = x + (f32)(*(s32*)(battle + 0x1CE0) * 0x21 + i * 0x30 - 0xFA);
        if (state == 1) {
            goodColor = dat_80427c38;
            btlDispTexPlane(markerX,
                            baseY - float_25_80427c50,
                            float_0_80427c54,
                            float_1_80427c48, float_1_80427c48,
                            0x54, &goodColor, 0);
        } else if (state == -1) {
            badColor = dat_80427c3c;
            btlDispTexPlane(markerX,
                            baseY - float_25_80427c50,
                            float_0_80427c54,
                            float_1_80427c48, float_1_80427c48,
                            0x55, &badColor, 0);
        }
    }
}

const SignalTimingVec vec3_802ff6d0 = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff6dc = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff6e8 = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff6f4 = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff700 = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff70c = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff718 = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff724 = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff730 = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff73c = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff748 = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff754 = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff760 = { 0.0f, 0.0f, 0.0f };
const SignalTimingVec vec3_802ff76c = { 0.0f, 0.0f, 0.0f };

const char str_SFX_AC_PI1_802ff780[] = "SFX_AC_PI1";
const char str_SFX_AC_PONE1_802ff78c[] = "SFX_AC_PONE1";
const char str_SFX_AC_COMMAND_OK1_802ff79c[] = "SFX_AC_COMMAND_OK1";
const char str_SFX_AC_COMMAND_NG1_802ff7b0[] = "SFX_AC_COMMAND_NG1";

const u32 dat_80427c30 = 0xFFFFFFFF;
const u32 dat_80427c34 = 0xFFFFFFFF;
const u32 dat_80427c38 = 0xFFFFFFFF;
const u32 dat_80427c3c = 0xFFFFFFFF;
const f32 float_neg200_80427c40 = -200.0f;
const f32 float_70_80427c44 = 70.0f;
const f32 float_1_80427c48 = 1.0f;
const f32 float_neg288_80427c4c = -288.0f;
const f32 float_25_80427c50 = 25.0f;
const f32 float_0_80427c54 = 0.0f;
const f32 float_1p5_80427c58 = 1.5f;
const f32 float_0p5_80427c5c = 0.5f;
const f32 float_15_80427c60 = 15.0f;
const f32 float_0p8_80427c64 = 0.8f;
const f32 float_0p6_80427c68 = 0.6f;
const f32 float_20p8_80427c6c = 20.8f;
const f32 float_neg300_80427c70 = -300.0f;
const f32 float_30_80427c74 = 30.0f;
