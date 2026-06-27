#include "action/ac_button_down.h"

extern f32 float_neg300_8042685c;
extern f32 float_0_80426860;

f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
void actionCommandDisp(s32 type, f32 x, f32 y);

s32 battleAcResult_ButtonDown(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}

void battleAcDelete_ButtonDown(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3EB;
}

void battleAcDisp_ButtonDown(void* camera, void* obj) {
    s32 state;
    s32 timer;
    f32 x;
    f32 y;

    state = *(s32*)((s32)obj + 0x1C9C);
    obj = (void*)((s32)obj + 0x1F20);
    if (state == 0x63) {
        goto active;
    }
    if (state >= 0x63) {
        goto inactive;
    }
    if (state == 0) {
        goto active;
    }
    goto inactive;

active:
    timer = *(s32*)((s32)obj + 0x20);
    if (timer > 0) {
        *(s32*)((s32)obj + 0x20) = timer - 1;
    }
    x = intplGetValue(4, 0x14 - *(s32*)((s32)obj + 0x20), 0x14, float_neg300_8042685c, float_0_80426860);
    y = float_0_80426860;
    goto disp;

inactive:
    y = float_0_80426860;
    x = y;

disp:
    actionCommandDisp(0, x, y);
}

void actionCommandDisp(s32 type, f32 x, f32 y) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* _battleWorkPointer;
    extern Vec vec3_802fdee8[];
    extern f32 float_neg200_80426848;
    extern f32 float_40_8042684c;
    extern f32 float_70_80426850;
    extern f32 float_1_80426854;
    extern f32 float_16_80426858;
    extern void iconDispGx(f32 scale, Vec* pos, s32 flags, s32 iconId);

    void* battleWork;
    void* extraWork;
    Vec* vecs;
    Vec pos;
    f32 baseX;
    f32 drawX;
    f32 drawY;
    f32 step;
    s32 count;
    s32 limit;
    s32 shown;
    s32 i;
    s32 offset;
    s32 xOffset;
    s32 button;
    s32 icon;
    s32 iconOff;
    s32 remaining;

    (void)type;

    vecs = vec3_802fdee8;
    battleWork = _battleWorkPointer;
    baseX = float_neg200_80426848 + x;
    drawX = baseX - float_40_8042684c;
    drawY = float_70_80426850 + y;
    extraWork = (void*)((s32)battleWork + 0x1F4C);
    count = *(s32*)((s32)battleWork + 0x1CD0);
    limit = count * 3 + 2;

    pos = vecs[0];
    pos.x = drawX;
    pos.y = drawY;
    iconDispGx(float_1_80426854, &pos, 0x10, 0x97);

    step = float_16_80426858;
    for (i = 1, drawX += step; i < limit; i++, drawX += step) {
        pos = vecs[1];
        pos.x = drawX;
        pos.y = drawY;
        iconDispGx(float_1_80426854, &pos, 0x10, 0x96);
    }

    pos = vecs[2];
    pos.x = drawX;
    pos.y = drawY;
    iconDispGx(float_1_80426854, &pos, 0x10, 0x98);

    if ((*(u32*)((s32)battleWork + 0x1CC4) & 1) != 0) {
        shown = 0;
        offset = 0;
        remaining = count;
        if (count > 0) {
            do {
                if (*(s32*)((s32)extraWork + offset + 0x3C) == 0) {
                    break;
                }
                shown++;
                offset += 4;
                remaining--;
            } while (remaining != 0);
        }
        if (shown < count) {
            shown++;
        }
    } else {
        shown = count;
    }

    for (i = 0, xOffset = 0, offset = 0; i < shown; i++, xOffset += 0x30, offset += 4) {
        button = *(s32*)((s32)extraWork + offset + 8);
        if (button == 0) {
            continue;
        }

        if (button == 0x200) {
            icon = 0x6F;
            iconOff = 0x6E;
        } else if (button < 0x200) {
            if (button == 0x40) {
                icon = 0x87;
                iconOff = 0x86;
            } else if (button < 0x40) {
                if (button == 0x20) {
                    icon = 0x89;
                    iconOff = 0x88;
                } else if (button < 0x20 && button == 0x10) {
                    icon = 0x8B;
                    iconOff = 0x8A;
                } else {
                    icon = 0x80;
                    iconOff = 0x81;
                }
            } else if (button == 0x100) {
                icon = 0x6D;
                iconOff = 0x6C;
            } else {
                icon = 0x80;
                iconOff = 0x81;
            }
        } else if (button == 0x800) {
            icon = 0x73;
            iconOff = 0x72;
        } else if (button < 0x800 && button == 0x400) {
            icon = 0x71;
            iconOff = 0x70;
        } else {
            icon = 0x80;
            iconOff = 0x81;
        }

        if (*(s32*)((s32)extraWork + offset + 0x3C) != 0) {
            pos = vecs[3];
            pos.x = baseX + (f32)xOffset;
            pos.y = drawY;
            iconDispGx(float_1_80426854, &pos, 0x10, icon);
        } else {
            pos = vecs[4];
            pos.x = baseX + (f32)xOffset;
            pos.y = drawY;
            iconDispGx(float_1_80426854, &pos, 0x10, iconOff);
        }
    }
}

s32 battleAcMain_ButtonDown(void* battleWork) {
    extern void* memset(void* dest, int ch, u32 count);
    extern s32 irand(s32 range);
    extern s32 BattlePadGetTrigger(void);
    extern void psndSFXOn(const char* name);
    extern const char str_SFX_AC_BUTTON_PUSH1_802fdf30[];
    extern s32 _button_table[];

    void* dispWork;
    void* extraWork;
    void* unit;
    s32 autoCommand;
    s32 state;
    s32 loop;
    s32 count;
    s32 i;
    s32 offset;
    s32 button;
    s32 ok;
    s32 complete;
    s32 tmp;
    s32 picked;
    s32 flag;
    s32 setup;
    s16 delay;

    extraWork = (void*)((s32)battleWork + 0x1F4C);
    dispWork = (void*)((s32)battleWork + 0x1F20);
    unit = *(void**)((s32)battleWork + 0x1C90);
    autoCommand = 0;
    if (*(u8*)((s32)unit + 0x307) != 0) {
        autoCommand = 1;
    }

    do {
        state = *(s32*)((s32)battleWork + 0x1C9C);
        loop = 0;

        if (state == 0) {
            *(s32*)((s32)battleWork + 0x1CB8) = 1;

            memset(dispWork, 0, 0x2C);
            *(f32*)((s32)dispWork + 0x14) = float_neg300_8042685c;
            *(f32*)((s32)dispWork + 0x18) = float_0_80426860;
            *(s32*)((s32)dispWork + 0x20) = 0x14;

            memset((void*)((s32)extraWork + 8), 0, 0x34);
            memset((void*)((s32)extraWork + 0x3C), 1, 0x34);

            i = 0;
            offset = 0;
            while (i < *(s32*)((s32)battleWork + 0x1CD0)) {
                setup = *(s32*)((s32)battleWork + 0x1CD4);

                if (setup == -2) {
                    tmp = irand(100);
                    button = 0x200;
                    if (tmp < 0x32) {
                        button = 0x100;
                    }
                    *(s32*)((s32)extraWork + offset + 8) = button;
                    *(s32*)((s32)extraWork + offset + 0x3C) = 0;
                } else if (setup == -4) {
                    tmp = irand(100);
                    button = 0x400;
                    if (tmp < 0x32) {
                        button = 0x200;
                    }
                    *(s32*)((s32)extraWork + offset + 8) = button;
                    *(s32*)((s32)extraWork + offset + 0x3C) = 0;
                } else if (setup == -3) {
                    if ((*(u32*)((s32)battleWork + 0x1CC4) & 4) != 0) {
                        tmp = irand(4);
                        button = _button_table[tmp];
                        *(s32*)((s32)extraWork + offset + 8) = button;
                        *(s32*)((s32)extraWork + offset + 0x3C) = 0;
                    } else {
                        ok = 0;
                        while (ok == 0) {
                            tmp = irand(4);
                            picked = _button_table[tmp];
                            flag = 1;
                            count = i - 1;
                            tmp = count * 4;
                            while (count >= 0) {
                                if (*(s32*)((s32)extraWork + tmp + 8) == picked) {
                                    flag = 0;
                                }
                                count--;
                                tmp -= 4;
                            }
                            if (flag != 0) {
                                ok = 1;
                            }
                        }
                        *(s32*)((s32)extraWork + offset + 8) = picked;
                        *(s32*)((s32)extraWork + offset + 0x3C) = 0;
                    }
                } else {
                    ok = 0;
                    while (ok == 0) {
                        tmp = irand(7);
                        picked = _button_table[tmp];
                        flag = 1;
                        count = i - 1;
                        tmp = count * 4;
                        while (count >= 0) {
                            if (*(s32*)((s32)extraWork + tmp + 8) == picked) {
                                flag = 0;
                            }
                            count--;
                            tmp -= 4;
                        }
                        if (flag != 0) {
                            ok = 1;
                        }
                    }
                    *(s32*)((s32)extraWork + offset + 8) = picked;
                    *(s32*)((s32)extraWork + offset + 0x3C) = 0;
                }

                i++;
                offset += 4;
            }

            *(s32*)((s32)battleWork + 0x1C9C) = 0x63;
        } else if (state == 0x64) {
            *(s32*)((s32)battleWork + 0x1C9C) = 0x3E8;
            *(s32*)((s32)extraWork + 4) = 0;
            *(s32*)extraWork = *(s32*)((s32)battleWork + 0x1CC8);

            state = 0x3E8;
        }

        if (state == 0x3E8) {
            setup = *(s32*)((s32)battleWork + 0x1C94);
            if (((setup & 1) != 0) && ((*(u32*)((s32)unit + 0x27C) & 0x10) != 0)) {
                if (((setup & 2) == 0) || (--*(s32*)((s32)battleWork + 0x1C98), *(s32*)((s32)battleWork + 0x1C98) < 0)) {
                    tmp = irand(100);
                    if (tmp < 0) {
                        *(s32*)((s32)battleWork + 0x1CB8) = 2;
                        *(s32*)((s32)battleWork + 0x1CB4) = *(s32*)((s32)battleWork + 0x1CB4) + 1;
                    } else {
                        *(s32*)((s32)battleWork + 0x1CB8) = 0;
                    }
                    return 0;
                }
            } else {
                delay = *(s16*)((s32)battleWork + 0x1D18);
                if (delay > 0) {
                    *(s16*)((s32)battleWork + 0x1D18) = delay - 1;
                    *(s32*)((s32)battleWork + 0x1CB8) = 2;
                    *(s32*)((s32)battleWork + 0x1C9C) = 0x3E9;
                    loop = 1;
                } else {
                    i = 0;
                    offset = 0;
                    complete = 1;
                    *(s32*)((s32)battleWork + 0x1CE8) = 0;

                    while (i < *(s32*)((s32)battleWork + 0x1CD0)) {
                        void* entry = (void*)((s32)extraWork + offset);
                        button = *(s32*)((s32)entry + 8);
                        if (button != 0) {
                            if (*(s32*)((s32)entry + 0x3C) != 0) {
                                *(s32*)((s32)battleWork + 0x1CE8) = *(s32*)((s32)battleWork + 0x1CE8) + 1;
                            } else {
                                tmp = BattlePadGetTrigger();
                                if (button == tmp || autoCommand != 0) {
                                    *(s32*)((s32)entry + 0x3C) = 1;
                                    *(s32*)((s32)battleWork + 0x1CE8) = *(s32*)((s32)battleWork + 0x1CE8) + 1;
                                    if (*(s32*)((s32)battleWork + 0x1CCC) == 1) {
                                        if (i + 1 < *(s32*)((s32)battleWork + 0x1CD0)) {
                                            complete = 0;
                                        }
                                        break;
                                    }
                                } else {
                                    tmp = BattlePadGetTrigger();
                                    if (tmp != 0) {
                                        *(s32*)((s32)battleWork + 0x1CB8) = 0;
                                        *(s32*)((s32)battleWork + 0x1C9C) = 0x3E9;
                                        if (((*(u32*)((s32)battleWork + 0x1CC4) & 2) == 0) ||
                                            (*(s32*)((s32)battleWork + 0x1CE8) <= 0)) {
                                            return 1;
                                        }
                                        loop = 1;
                                        *(s32*)((s32)battleWork + 0x1CB8) = (*(s32*)((s32)battleWork + 0x1CB8) | 2) & ~1;
                                        *(s32*)((s32)battleWork + 0x1C9C) = 0x3E9;
                                        break;
                                    }
                                    complete = 0;
                                    if (*(s32*)((s32)battleWork + 0x1CCC) == 1) {
                                        break;
                                    }
                                }
                            }
                        }

                        i++;
                        offset += 4;
                    }

                    if (*(s32*)((s32)extraWork + 4) != *(s32*)((s32)battleWork + 0x1CE8)) {
                        psndSFXOn(str_SFX_AC_BUTTON_PUSH1_802fdf30);
                        *(s32*)((s32)extraWork + 4) = *(s32*)((s32)battleWork + 0x1CE8);
                    }

                    if (complete != 0) {
                        loop = 1;
                        *(s32*)((s32)battleWork + 0x1CB8) = (*(s32*)((s32)battleWork + 0x1CB8) | 2) & ~1;
                        *(s32*)((s32)battleWork + 0x1C9C) = 0x3E9;
                    } else {
                        *(s32*)extraWork = *(s32*)extraWork - 1;
                        if (*(s32*)extraWork < 0) {
                            *(s32*)((s32)battleWork + 0x1CB8) = 0;
                            loop = 1;
                            *(s32*)((s32)battleWork + 0x1C9C) = 0x3E9;
                            if (((*(u32*)((s32)battleWork + 0x1CC4) & 2) != 0) &&
                                (*(s32*)((s32)battleWork + 0x1CE8) > 0)) {
                                *(s32*)((s32)battleWork + 0x1CB8) = (*(s32*)((s32)battleWork + 0x1CB8) | 2) & ~1;
                                *(s32*)((s32)battleWork + 0x1C9C) = 0x3E9;
                            }
                        }
                    }
                }
            }
        } else if (state == 0x3E9) {
            *(s32*)((s32)battleWork + 0x1C9C) = 0x3EA;
            return 0;
        } else if (state == 0x3EA) {
            return 0;
        } else if (state == 0x3EB) {
            *(s32*)((s32)extraWork + 0x70) = 0x3C;
            *(s32*)((s32)battleWork + 0x1C9C) = 0x3EC;
        } else if (state == 0x3EC) {
            *(s32*)((s32)extraWork + 0x70) = *(s32*)((s32)extraWork + 0x70) - 1;
            if (*(s32*)((s32)extraWork + 0x70) <= 0) {
                *(s32*)((s32)battleWork + 0x1C9C) = 0x3ED;
            }
        } else if (state == 0x3ED) {
            *(s32*)((s32)battleWork + 0x1CA0) = 0;
            *(s32*)((s32)battleWork + 0x1CA8) = 0;
            *(s32*)((s32)battleWork + 0x1CA4) = 0;
            *(s32*)((s32)battleWork + 0x1CAC) = 0;
        }
    } while (loop != 0);

    return 1;
}
