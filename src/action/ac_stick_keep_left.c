#include "action/ac_stick_keep_left.h"

void actionCommandDisp(s32 unused, f32 x, f32 y);

s32 battleAcMain_StickKeepLeft(void* battleWork) {
    typedef struct StickKeepLeftExtra {
        s32 timer;
        s32 index;
        s32 wait;
        s32 sound[12];
    } StickKeepLeftExtra;
    extern s32 irand(s32 max);
    extern u32 BattlePadCheckNow(u32 buttons);
    extern s32 psndSFXOn(const char* name);
    extern u32 BattleActionCommandGetDifficulty(void* battleWork);
    extern s32 ac_stickkeepleft_ok_frame[];
    extern char str_SFX_AC_PI1_802f01a8[];
    extern char str_SFX_AC_PONE1_802f01b4[];

    register char* sfxPi;
    register char* sfxPone;
    StickKeepLeftExtra* extra;
    void* disp;
    s32 autoCommand;
    s32 done;
    s32 success;
    s32 state;
    s32 index;
    s32 timer;
    s32 difficulty;
    s32 i;

    extra = (StickKeepLeftExtra*)((s32)battleWork + 0x1F4C);
    disp = (void*)((s32)battleWork + 0x1F20);
    autoCommand = 0;
    if (*(u8*)((s32)*(void**)((s32)battleWork + 0x1C90) + 0x307) != 0) {
        autoCommand = 1;
    }

    *(s32*)((s32)battleWork + 0x1CD4) = 0;
    if (*(s32*)((s32)battleWork + 0x1C9C) != 0) {
        if (*(s32*)((s32)battleWork + 0x1CD0) != 0) {
            *(s32*)((s32)battleWork + 0x1CD4) = 1;
        } else {
            *(s32*)((s32)battleWork + 0x1CD4) = 0;
        }
    }

    sfxPi = str_SFX_AC_PI1_802f01a8;
    sfxPone = str_SFX_AC_PONE1_802f01b4;

    while (1) {
        done = 0;
        if (*(s32*)((s32)battleWork + 0x1CD4) != 0) {
            return 1;
        }

        state = *(s32*)((s32)battleWork + 0x1C9C);
        switch (state) {
            case 0:
                goto state_0;
            case 99:
                goto state_done;
            case 100:
                goto state_100;
            case 1001:
                goto state_1001;
            case 1003:
                goto state_1003;
            case 1005:
                goto state_1005;
            case 1006:
                goto state_1006;
            case 1007:
                goto state_1007;
            default:
                goto state_done;
        }

state_0:
        {
            *(s32*)((s32)battleWork + 0x1CB8) = 1;
            extra->index = 0;
            memset(disp, 0, 0x2C);
            *(f32*)((s32)disp + 0x14) = -300.0f;
            *(f32*)((s32)disp + 0x18) = 0.0f;
            *(s32*)((s32)disp + 0x20) = 0x14;
            *(s32*)((s32)battleWork + 0x1CE8) = 0;
            *(s32*)((s32)battleWork + 0x1CD0) = 0;
            *(s32*)((s32)battleWork + 0x1CD4) = 0;
            *(s32*)((s32)battleWork + 0x1CD8) = 0;
            *(s32*)((s32)battleWork + 0x1CF0) = 0;
            for (i = 0; i < 12; i++) {
                extra->sound[i] = -1;
            }
            *(s32*)((s32)battleWork + 0x1C9C) = 99;
        }
        goto state_done;

state_100:
        {
            extra->timer = 0x78;
            *(s32*)((s32)battleWork + 0x1C9C) = 1000;
            if (((*(u32*)((s32)battleWork + 0x1C94) & 1) != 0) &&
                ((*(u32*)((s32)*(void**)((s32)battleWork + 0x1C90) + 0x27C) & 0x10) != 0)) {
                if (((*(u32*)((s32)battleWork + 0x1C94) & 2) == 0) ||
                    (--*(s32*)((s32)battleWork + 0x1C98) < 0)) {
                    if (irand(100) < 0) {
                        *(s32*)((s32)battleWork + 0x1CB8) = 2;
                        *(s32*)((s32)battleWork + 0x1CB4) = *(s32*)((s32)battleWork + 0x1CB4) + 1;
                    } else {
                        *(s32*)((s32)battleWork + 0x1CB8) = 0;
                    }
                    return 0;
                }
            } else if (BattlePadCheckNow(0x40000) == 0 &&
                       *(s16*)((s32)battleWork + 0x1D18) <= 0 &&
                       !autoCommand) {
                timer = extra->timer - 1;
                extra->timer = timer;
                if (timer < 1) {
                    *(s32*)((s32)battleWork + 0x1CB8) = 0;
                    done = 1;
                    *(s32*)((s32)battleWork + 0x1C9C) = 1003;
                }
            } else {
                *(s32*)((s32)battleWork + 0x1CE8) = 1;
                *(s32*)((s32)battleWork + 0x1C9C) = 1001;
                extra->timer = 1;
                extra->index = 1;
                *(s32*)((s32)battleWork + 0x1CEC) = 0;
            }
        }
        goto state_done;

state_1001:
        {
            if (*(s16*)((s32)battleWork + 0x1D18) > 0) {
                *(s16*)((s32)battleWork + 0x1D18) = *(s16*)((s32)battleWork + 0x1D18) - 1;
                *(s32*)((s32)battleWork + 0x1CB8) = 2;
                *(s32*)((s32)battleWork + 0x1C9C) = 1003;
                done = 1;
            }
            extra->timer = extra->timer - 1;
            if (extra->timer < 0) {
                extra->index = extra->index + 1;
                *(s32*)((s32)battleWork + 0x1CEC) = *(s32*)((s32)battleWork + 0x1CEC) + 1;
                index = extra->index;
                if (index < *(s32*)((s32)battleWork + 0x1CC8) + 2) {
                    if ((u32)(index - 2) < (u32)(*(s32*)((s32)battleWork + 0x1CC8) - 1)) {
                        if (extra->sound[index - 2] == -1) {
                            extra->sound[index - 2] = psndSFXOn(sfxPi);
                        }
                    } else if (extra->sound[index - 2] == -1) {
                        extra->sound[index - 2] = psndSFXOn(sfxPone);
                        *(s32*)((s32)battleWork + 0x1CD8) = 1;
                    }
                    extra->timer = *(s32*)((s32)battleWork + 0x1CCC);
                } else {
                    *(s32*)((s32)battleWork + 0x1CB8) &= ~1;
                    *(s32*)((s32)battleWork + 0x1C9C) = 1003;
                    *(s32*)((s32)battleWork + 0x1CF0) = 2;
                    if ((*(u32*)((s32)battleWork + 0x1CC4) & 2) != 0) {
                        *(s32*)((s32)battleWork + 0x1CB8) |= 2;
                        *(s32*)((s32)battleWork + 0x1CB8) &= ~1;
                    }
                    done = 1;
                }
            } else if (BattlePadCheckNow(0x40000) == 0 || autoCommand) {
                success = 0;
                if (extra->index == *(s32*)((s32)battleWork + 0x1CC8) + 1) {
                    difficulty = BattleActionCommandGetDifficulty(battleWork);
                    if (ac_stickkeepleft_ok_frame[difficulty] < *(s32*)((s32)battleWork + 0x1CCC) - extra->timer) {
                        if ((*(u32*)((s32)battleWork + 0x1CC4) & 2) == 0) {
                            *(s32*)((s32)battleWork + 0x1CF0) = 1;
                        }
                    } else {
                        success = 1;
                        *(s32*)((s32)battleWork + 0x1CF0) = 3;
                    }
                } else if ((*(u32*)((s32)battleWork + 0x1CC4) & 2) == 0) {
                    *(s32*)((s32)battleWork + 0x1CF0) = 1;
                }
                if (success) {
                    done = 1;
                    *(s32*)((s32)battleWork + 0x1CB8) |= 2;
                    *(s32*)((s32)battleWork + 0x1CB8) &= ~1;
                    *(s32*)((s32)battleWork + 0x1C9C) = 1003;
                } else if (!autoCommand && ((*(u32*)((s32)battleWork + 0x1CC4) & 2) == 0)) {
                    done = 1;
                    *(s32*)((s32)battleWork + 0x1CB8) &= ~1;
                    *(s32*)((s32)battleWork + 0x1C9C) = 1003;
                }
            }
        }
        goto state_done;

state_1003:
        {
            index = extra->index;
            if (index - 2 >= 0) {
                index--;
                if (index - 1 >= 0) {
                    do {
                        index--;
                    } while (index != 0);
                }
            }
            *(s32*)((s32)battleWork + 0x1C9C) = 1004;
        }
        goto state_done;

state_1005:
        {
            extra->wait = 0x3C;
            *(s32*)((s32)battleWork + 0x1C9C) = 1006;
        }
        goto state_done;

state_1006:
        {
            extra->wait = extra->wait - 1;
            if (extra->wait < 1) {
                *(s32*)((s32)battleWork + 0x1C9C) = 1007;
            }
        }
        goto state_done;

state_1007:
        {
            *(void**)((s32)battleWork + 0x1CA0) = 0;
            *(void**)((s32)battleWork + 0x1CA4) = 0;
            *(void**)((s32)battleWork + 0x1CAC) = 0;
            *(void**)((s32)battleWork + 0x1CA8) = 0;
        }

state_done:
        if (!done) {
            return 1;
        }
    }
}

s32 battleAcResult_StickKeepLeft(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}

void battleAcDisp_StickKeepLeft(s32 param_1, void* battleWork) {
    typedef struct AcDisp {
        char pad_00[0x14];
        f32 x;
        f32 y;
        char pad_1C[4];
        s32 timer;
        s32 icon;
    } AcDisp;
    typedef struct AcData {
        s32 unk0;
        s32 count;
    } AcData;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    AcData* data = (AcData*)((s32)battleWork + 0x1F4C);
    register AcDisp* disp = (AcDisp*)((s32)battleWork + 0x1F20);
    s32 state = *(s32*)((s32)battleWork + 0x1C9C);
    s32 timer;

    if (state == 1001) {
        goto selected;
    }
    if (state >= 1001) {
        if (state >= 1007) {
            return;
        }
        if (state >= 1005) {
            goto closing;
        }
        goto static_disp;
    }
    if (state >= 101) {
        if (state >= 1000) {
            goto opening;
        }
        return;
    }
    if (state < 99) {
        return;
    }

opening:
    disp->icon = -1;
    disp->x = intplGetValue(4, 20 - disp->timer, 20, -300.0f, 30.0f);
    actionCommandDisp(disp->icon, disp->x, disp->y);
    if (*(s32*)((s32)battleWork + 0x1CD4) == 0) {
        timer = disp->timer;
        if (timer > 0) {
            disp->timer = timer - 1;
        }
    }
    return;

selected:
    disp->icon = data->count - 1;
    disp->x = intplGetValue(4, 20 - disp->timer, 20, -300.0f, 30.0f);
    actionCommandDisp(disp->icon, disp->x, disp->y);
    if (*(s32*)((s32)battleWork + 0x1CD4) == 0) {
        timer = disp->timer;
        if (timer > 0) {
            disp->timer = timer - 1;
        }
    }
    return;

static_disp:
    actionCommandDisp(disp->icon, 30.0f, 0.0f);
    return;

closing:
    timer = disp->timer;
    if (timer >= 40) {
        disp->x = intplGetValue(4, timer - 40, 20, 30.0f, -300.0f);
    } else {
        disp->x = 30.0f;
    }
    actionCommandDisp(disp->icon, disp->x, disp->y);
    if (*(s32*)((s32)battleWork + 0x1CD4) == 0) {
        timer = disp->timer;
        if (timer < 60) {
            disp->timer = timer + 1;
        }
    }
}

void battleAcDelete_StickKeepLeft(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3ED;
}


void actionCommandDisp(s32 unused, f32 x, f32 y) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* _battleWorkPointer;
    extern void* gp;
    extern void camGetPtr(s32 id);
    extern void iconDispGx(f32 scale, Vec* pos, s32 flags, s32 iconId);
    extern char str_AcSkl1_802f00d8[];

    Vec pos;
    void* battleWork;
    s32 state;
    s32 count;
    s32 current;
    s32 limit;
    s32 i;
    f32 yPos;
    f32 step;

    (void)unused;

    battleWork = _battleWorkPointer;
    camGetPtr(8);
    state = *(s32*)((s32)battleWork + 0x1C9C);
    count = *(s32*)((s32)battleWork + 0x1CC8);
    current = *(s32*)((s32)battleWork + 0x1F50) - 1;

    if (state < 0x65) {
        if (state == 0 || (state >= 0 && state >= 0x63)) {
            pos = *(Vec*)(str_AcSkl1_802f00d8 + 0x28);
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            iconDispGx(1.0f, &pos, 0x10, 0x81);
            goto draw_meter;
        }
    } else if (state == 1000) {
        if ((*(u32*)((s32)gp + 0x1C) & 8) == 0) {
            pos = *(Vec*)(str_AcSkl1_802f00d8 + 0x40);
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            iconDispGx(1.0f, &pos, 0x10, 0x81);
        } else {
            pos = *(Vec*)(str_AcSkl1_802f00d8 + 0x34);
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            iconDispGx(1.0f, &pos, 0x10, 0x80);
        }
        goto draw_meter;
    }

    if (current < count) {
        pos = *(Vec*)(str_AcSkl1_802f00d8 + 0x4C);
        pos.x = -200.0f + x;
        pos.y = 70.0f + y;
        iconDispGx(1.0f, &pos, 0x10, 0x80);
    } else {
        pos = *(Vec*)(str_AcSkl1_802f00d8 + 0x58);
        pos.x = -200.0f + x;
        pos.y = 70.0f + y;
        iconDispGx(1.0f, &pos, 0x10, 0x81);
    }

draw_meter:
    limit = 10;
    if (count > 3) {
        limit = count * 2 + 3;
        if ((*(u32*)((s32)battleWork + 0x1CC4) & 1) != 0) {
            limit += 2;
        }
    }

    step = 16.0f;
    if ((*(u32*)((s32)battleWork + 0x1CC4) & 1) != 0) {
        step = 12.0f;
    }
    yPos = 25.0f + y;

    pos = *(Vec*)(str_AcSkl1_802f00d8 + 0x64);
    pos.x = -288.0f + x;
    pos.y = yPos;
    iconDispGx(1.0f, &pos, 0x10, 0x97);

    for (i = 1; i < limit; i++) {
        pos = *(Vec*)(str_AcSkl1_802f00d8 + 0x70);
        pos.x = x + (step * (f32)i + -288.0f);
        pos.y = yPos;
        iconDispGx(1.0f, &pos, 0x10, 0x96);
    }

    pos = *(Vec*)(str_AcSkl1_802f00d8 + 0x7C);
    pos.x = x + (step * (f32)i + -288.0f);
    pos.y = yPos;
    iconDispGx(1.0f, &pos, 0x10, 0x98);

    step = 33.0f;
    if ((*(u32*)((s32)battleWork + 0x1CC4) & 1) != 0) {
        step = 24.75f;
    }

    for (i = 0; i < count - 1; i++) {
        if (i < current) {
            if (i < 3) {
                pos = *(Vec*)(str_AcSkl1_802f00d8 + 0x88);
                pos.x = x + (step * (f32)i + -258.0f);
                pos.y = yPos;
                iconDispGx(1.0f, &pos, 0x10, i + 0x9A);
            } else {
                pos = *(Vec*)(str_AcSkl1_802f00d8 + 0x94);
                pos.x = x + (step * (f32)i + -258.0f);
                pos.y = yPos;
                iconDispGx(1.0f, &pos, 0x10, 0x9C);
            }
        } else {
            pos = *(Vec*)(str_AcSkl1_802f00d8 + 0xA0);
            pos.x = x + (step * (f32)i + -258.0f);
            pos.y = yPos;
            iconDispGx(0.5f, &pos, 0x10, 0x99);
        }
    }

    if (current < count) {
        pos = *(Vec*)(str_AcSkl1_802f00d8 + 0xAC);
        pos.x = x + (step * (f32)i + -250.0f);
        pos.y = yPos;
        iconDispGx(1.0f, &pos, 0x10, 0x99);
    } else {
        pos = *(Vec*)(str_AcSkl1_802f00d8 + 0xB8);
        pos.x = x + (step * (f32)i + -250.0f);
        pos.y = yPos;
        iconDispGx(1.0f, &pos, 0x10, 0x9D);
    }
}
