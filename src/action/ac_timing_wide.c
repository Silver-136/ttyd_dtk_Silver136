#include "action/ac_timing_wide.h"

extern f32 float_30_80426874;
extern f32 float_neg300_80426878;

f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
void actionCommandDisp(f32 x, f32 y);

s32 battleAcMain_TimingWide(void* battleWork) {
    extern void* memset(void* dest, int ch, u32 count);
    extern s32 irand(s32 range);
    extern s32 BattlePadCheckTrigger(s32 button);
    extern s32 BattlePadCheckUp(s32 button);
    extern s32 BattlePadCheckNow(s32 button);
    extern f32 float_neg300_80426878;
    extern f32 float_0_8042687c;

    s32 work;
    s32 disp;
    s32 unit;
    s32 state;
    s32 autoCommand;
    s32 value;
    s32 hit;
    s32 success;
    s32 buttonMode;
    s32 frame;
    s32 start;
    s32 end;

    work = (s32)battleWork + 0x1F4C;
    disp = (s32)battleWork + 0x1F20;
    unit = *(s32*)((s32)battleWork + 0x1C90);

    autoCommand = 0;
    if (*(u8*)(unit + 0x307) != 0) {
        autoCommand = 1;
    }

    state = *(s32*)((s32)battleWork + 0x1C9C);

    if (state == 0) {
        memset((void*)disp, 0, 0x2C);
        *(f32*)(disp + 0x14) = float_neg300_80426878;
        *(f32*)(disp + 0x18) = float_0_8042687c;
        *(s32*)(disp + 0x20) = 0x14;

        *(s32*)(work + 0xC) = 0;
        *(s32*)(work + 8) = 0;

        value = *(s32*)((s32)battleWork + 0x1CC8);
        if (value == 3) {
            *(s32*)work = 0x80000;
            *(s8*)(work + 4) = -1;
        } else if (value < 3) {
            if (value == 1) {
                *(s32*)work = 0x80000;
                *(s8*)(work + 4) = 0;
            } else if (value > 0) {
                *(s32*)work = 0x40000;
                *(s8*)(work + 4) = -1;
            } else if (value > -1) {
                *(s32*)work = 0x40000;
                *(s8*)(work + 4) = 0;
            } else {
                *(s32*)work = 0x40000;
                *(s8*)(work + 4) = 0;
            }
        } else if (value == 5) {
            *(s32*)work = 0x100;
            *(s8*)(work + 4) = 1;
        } else {
            *(s32*)work = 0x40000;
            *(s8*)(work + 4) = 0;
        }

        *(s32*)((s32)battleWork + 0x1CE8) = 0;
        *(s32*)((s32)battleWork + 0x1CEC) = 0;
        *(s32*)((s32)battleWork + 0x1CF0) = 0;
        *(s32*)((s32)battleWork + 0x1C9C) = 99;
        *(s32*)((s32)battleWork + 0x1CB8) = 1;
        return 1;
    }

    if (state == 100) {
        if ((*(u32*)((s32)battleWork + 0x1C94) & 1) != 0 &&
            (*(u32*)(unit + 0x27C) & 0x10) != 0) {
            value = irand(100);
            if (value < 0) {
                value = irand(*(s32*)((s32)battleWork + 0x1CD4) - *(s32*)((s32)battleWork + 0x1CD0));
                *(s32*)((s32)battleWork + 0x1C98) = *(s32*)((s32)battleWork + 0x1CD0) + value;
            } else {
                *(s32*)((s32)battleWork + 0x1C98) = 0;
                value = *(s32*)((s32)battleWork + 0x1CD0);
                if (value > 0) {
                    *(s32*)((s32)battleWork + 0x1C98) = irand(value);
                }
                if (*(s32*)((s32)battleWork + 0x1C98) < 1) {
                    *(s32*)((s32)battleWork + 0x1C98) = *(s32*)((s32)battleWork + 0x1CCC) + 2;
                }
            }
            *(s32*)((s32)battleWork + 0x1C9C) = 0x3EA;
            *(s32*)(work + 0xC) = 2;
            return 1;
        }

        if (*(s32*)((s32)battleWork + 0x1CC8) == 4) {
            *(s32*)((s32)battleWork + 0x1C9C) = 0x3E8;
        } else {
            *(s32*)((s32)battleWork + 0x1C9C) = 0x3E9;
        }
        *(s32*)(work + 0xC) = 1;
        return 1;
    }

    if (state == 0x3E8) {
        if (BattlePadCheckTrigger(*(s32*)work) != 0) {
            *(s32*)((s32)battleWork + 0x1CEC) = 1;
            *(s32*)((s32)battleWork + 0x1C9C) = 0x3E9;
        }
        return 1;
    }

    if (state == 0x3E9) {
        *(s32*)(work + 8) = *(s32*)(work + 8) + 1;

        if (*(s32*)((s32)battleWork + 0x1CCC) < *(s32*)(work + 8)) {
            *(s32*)((s32)battleWork + 0x1CB8) = 0;
            *(s32*)((s32)battleWork + 0x1C9C) = 0x3EC;
            *(s32*)(work + 0xC) = 4;

            if ((*(u32*)((s32)battleWork + 0x1CC4) & 8) != 0) {
                *(s32*)((s32)battleWork + 0x1C9C) = 0x3EB;
                *(s32*)(work + 0xC) = 3;
            }
            return 1;
        }

        buttonMode = *(s8*)(work + 4);
        success = 0;
        if (buttonMode != 0) {
            if (buttonMode < 0) {
                if (buttonMode > -2) {
                    if (BattlePadCheckUp(*(s32*)work) != 0) {
                        success = 1;
                    }
                } else {
                    if (BattlePadCheckNow(*(s32*)work) == 0) {
                        success = 1;
                    }
                }
            } else if (buttonMode < 2) {
                if (BattlePadCheckTrigger(*(s32*)work) != 0) {
                    success = 1;
                }
            } else {
                if (BattlePadCheckNow(*(s32*)work) == 0) {
                    success = 1;
                }
            }
        } else {
            if (BattlePadCheckNow(*(s32*)work) == 0) {
                success = 1;
            }
        }

        frame = *(s32*)(work + 8);
        start = *(s32*)((s32)battleWork + 0x1CD0);
        end = *(s32*)((s32)battleWork + 0x1CD4);
        if (frame < start || end <= frame) {
            *(s32*)((s32)battleWork + 0x1CF0) = 0;
        } else {
            *(s32*)((s32)battleWork + 0x1CF0) = 1;
        }

        if (autoCommand != 0) {
            if (*(s32*)((s32)battleWork + 0x1CF0) == 0) {
                success = 0;
            } else {
                success = 1;
            }
        }

        if (success != 0) {
            if (*(s32*)((s32)battleWork + 0x1CF0) == 0) {
                *(s32*)((s32)battleWork + 0x1CB8) = 0;
                *(s32*)((s32)battleWork + 0x1CE8) = -1;
            } else {
                *(s32*)((s32)battleWork + 0x1CB8) = 2;
                *(s32*)((s32)battleWork + 0x1CB4) = *(s32*)((s32)battleWork + 0x1CB4) + 1;
                *(s32*)((s32)battleWork + 0x1CE8) = 1;
            }

            *(s32*)((s32)battleWork + 0x1C9C) = 0x3EC;
            *(s32*)(work + 0xC) = 4;
            if ((*(u32*)((s32)battleWork + 0x1CC4) & 8) != 0) {
                *(s32*)((s32)battleWork + 0x1C9C) = 0x3EB;
                *(s32*)(work + 0xC) = 3;
            }
        }
        return 1;
    }

    if (state == 0x3EA) {
        *(s32*)(work + 8) = *(s32*)(work + 8) + 1;
        *(s32*)((s32)battleWork + 0x1C98) = *(s32*)((s32)battleWork + 0x1C98) - 1;

        value = *(s32*)(work + 8);
        if (*(s32*)((s32)battleWork + 0x1CCC) < value) {
            *(s32*)((s32)battleWork + 0x1CB8) = 0;
            return 0;
        }

        if (*(s32*)((s32)battleWork + 0x1C98) > -1) {
            return 1;
        }

        if (value < *(s32*)((s32)battleWork + 0x1CD0) ||
            *(s32*)((s32)battleWork + 0x1CD4) <= value) {
            *(s32*)((s32)battleWork + 0x1CB8) = 0;
            *(s32*)((s32)battleWork + 0x1CE8) = -1;
        } else {
            *(s32*)((s32)battleWork + 0x1CB8) = 2;
            *(s32*)((s32)battleWork + 0x1CB4) = *(s32*)((s32)battleWork + 0x1CB4) + 1;
            *(s32*)((s32)battleWork + 0x1CE8) = 1;
        }
        return 0;
    }

    if (state == 0x3EB) {
        return 1;
    }

    if (state == 0x3EC) {
        *(s32*)(work + 0x10) = 0x3C;
        *(s32*)((s32)battleWork + 0x1C9C) = 0x3ED;
        *(s32*)(work + 0xC) = 5;
    }

    if (state == 0x3ED || *(s32*)((s32)battleWork + 0x1C9C) == 0x3ED) {
        *(s32*)(work + 0x10) = *(s32*)(work + 0x10) - 1;
        if (*(s32*)(work + 0x10) > 0) {
            return 1;
        }
        *(s32*)((s32)battleWork + 0x1C9C) = 0x3EE;
        *(s32*)(work + 0xC) = 6;
        return 1;
    }

    if (state == 0x3EE) {
        *(s32*)((s32)battleWork + 0x1CA0) = 0;
        *(s32*)((s32)battleWork + 0x1CA8) = 0;
        *(s32*)((s32)battleWork + 0x1CA4) = 0;
        *(s32*)((s32)battleWork + 0x1CAC) = 0;
        return 0;
    }

    return 1;
}

s32 battleAcResult_TimingWide(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}

void battleAcDisp_TimingWide(void* camera, void* obj) {
    register s32 work;
    s32 state;
    s32 timer;
    s32 enabled;
    s32 flags;

    work = (s32)obj;
    work += 0x1F20;
    state = *(s32*)((s32)obj + 0x1F58);
    if (state == 3) {
        goto display_only;
    }
    if (state >= 3) {
        goto state_high;
    }
    if (state >= 0) {
        goto intro;
    }
    return;

state_high:
    if (state >= 6) {
        return;
    }
    goto outro;

intro:
    if (*(s32*)((s32)obj + 0x1CC4) & 1) {
        *(f32*)(work + 0x14) = float_30_80426874;
    } else {
        *(f32*)(work + 0x14) = intplGetValue(4, 0x14 - *(s32*)(work + 0x20), 0x14, float_neg300_80426878, float_30_80426874);
    }
    actionCommandDisp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    timer = *(s32*)(work + 0x20);
    if (timer > 0) {
        *(s32*)(work + 0x20) = timer - 1;
    }
    return;

display_only:
    actionCommandDisp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    return;

outro:
    flags = *(s32*)((s32)obj + 0x1CC4);
    enabled = 1;
    if ((flags & 2) && (*(s32*)((s32)obj + 0x1CB8) & 2)) {
        enabled = 0;
    }
    if ((flags & 4) && !(*(s32*)((s32)obj + 0x1CB8) & 2)) {
        enabled = 0;
    }
    timer = *(s32*)(work + 0x20);
    if (timer >= 0x28) {
        if (enabled) {
            *(f32*)(work + 0x14) = intplGetValue(4, timer - 0x28, 0x14, float_30_80426874, float_neg300_80426878);
        } else {
            *(f32*)(work + 0x14) = float_30_80426874;
        }
    } else {
        *(f32*)(work + 0x14) = float_30_80426874;
    }
    actionCommandDisp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    timer = *(s32*)(work + 0x20);
    if (timer < 0x3C) {
        *(s32*)(work + 0x20) = timer + 1;
    }
}

void battleAcDelete_TimingWide(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3EC;
}

void actionCommandDisp(f32 x, f32 y) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* _battleWorkPointer;
    extern Vec vec3_802fdf48[];
    extern f32 float_neg200_80426868;
    extern f32 float_50_8042686c;
    extern f32 float_1_80426870;
    extern void* camGetPtr(s32 cameraId);
    extern void iconDispGx(Vec* pos, s32 flags, s32 iconId, f32 scale);

    void* bw;
    void* disp;
    s32 ex;
    Vec* vecs;
    Vec local_c8;
    Vec local_bc;
    Vec local_b0;
    Vec local_a4;
    Vec local_98;
    Vec local_8c;
    Vec local_80;
    Vec local_74;
    Vec local_68;
    Vec local_5c;
    Vec local_50;
    Vec local_44;
    s32 button;
    s32 iconOn;
    s32 iconOff;
    s32 mode;
    s32 current;
    s8 flag;

    bw = _battleWorkPointer;
    vecs = vec3_802fdf48;
    disp = (void*)((s32)bw + 0x1F20);

    camGetPtr(8);

    ex = (s32)bw + 0x1F4C;
    button = *(s32*)ex;

    if (button == 0x800) {
        iconOn = 0x73;
        iconOff = 0x72;
    } else if (button < 0x800) {
        if (button == 0x200) {
            iconOn = 0x6F;
            iconOff = 0x6E;
        } else if (button < 0x200) {
            if (button == 0x100) {
                iconOn = 0x6D;
                iconOff = 0x6C;
            } else {
                iconOn = 0x80;
                iconOff = 0x81;
            }
        } else if (button == 0x400) {
            iconOn = 0x71;
            iconOff = 0x70;
        } else {
            iconOn = 0x80;
            iconOff = 0x81;
        }
    } else {
        if (button == 0x80000) {
            iconOn = 0x82;
            iconOff = 0x81;
        } else if (button < 0x80000) {
            if (button == 0x40000) {
                iconOn = 0x80;
                iconOff = 0x81;
            } else {
                iconOn = 0x80;
                iconOff = 0x81;
            }
        } else {
            iconOn = 0x80;
            iconOff = 0x81;
        }
    }

    mode = *(s32*)(ex + 0xC);
    if (mode == 1) {
        goto mode_one;
    }
    if (mode >= 1) {
        if (mode >= 7) {
            goto done;
        }
        goto mode_mid;
    }
    if (mode >= 0) {
        goto mode_zero;
    }
    goto done;

mode_zero:
    if (*(s8*)(ex + 4) < 1) {
        local_50 = vecs[1];
        local_50.x = float_neg200_80426868 + x;
        local_50.y = float_50_8042686c + y;
        iconDispGx(&local_50, 0x10, iconOn, float_1_80426870);
    } else {
        local_44 = vecs[0];
        local_44.x = float_neg200_80426868 + x;
        local_44.y = float_50_8042686c + y;
        iconDispGx(&local_44, 0x10, iconOff, float_1_80426870);
    }
    goto done;

mode_mid:
    if ((*(u32*)((s32)bw + 0x1CC4) & 0x10) == 0) {
        if (*(s8*)(ex + 4) < 1) {
            local_68 = vecs[3];
            local_68.x = float_neg200_80426868 + x;
            local_68.y = float_50_8042686c + y;
            iconDispGx(&local_68, 0x10, iconOn, float_1_80426870);
        } else {
            local_5c = vecs[2];
            local_5c.x = float_neg200_80426868 + x;
            local_5c.y = float_50_8042686c + y;
            iconDispGx(&local_5c, 0x10, iconOff, float_1_80426870);
        }
    } else {
        if (*(s8*)(ex + 4) < 1) {
            local_80 = vecs[5];
            local_80.x = float_neg200_80426868 + x;
            local_80.y = float_50_8042686c + y;
            iconDispGx(&local_80, 0x10, iconOff, float_1_80426870);
        } else {
            local_74 = vecs[4];
            local_74.x = float_neg200_80426868 + x;
            local_74.y = float_50_8042686c + y;
            iconDispGx(&local_74, 0x10, iconOn, float_1_80426870);
        }
    }
    goto done;

mode_one:
    flag = *(s8*)(ex + 4);
    if (flag == -1) {
        if (*(s32*)(ex + 8) < *(s32*)((s32)bw + 0x1CD0)) {
            local_c8 = vecs[11];
            local_c8.x = float_neg200_80426868 + x;
            local_c8.y = float_50_8042686c + y;
            iconDispGx(&local_c8, 0x10, iconOn, float_1_80426870);
        } else {
            local_bc = vecs[10];
            local_bc.x = float_neg200_80426868 + x;
            local_bc.y = float_50_8042686c + y;
            iconDispGx(&local_bc, 0x10, iconOff, float_1_80426870);
        }
    } else {
        current = *(s32*)(ex + 8);
        if (current >= *(s32*)((s32)bw + 0x1CD0) &&
            current < *(s32*)((s32)bw + 0x1CD4)) {
            if (flag > 0) {
                local_8c = vecs[6];
                local_8c.x = float_neg200_80426868 + x;
                local_8c.y = float_50_8042686c + y;
                iconDispGx(&local_8c, 0x10, iconOn, float_1_80426870);
            } else {
                local_98 = vecs[7];
                local_98.x = float_neg200_80426868 + x;
                local_98.y = float_50_8042686c + y;
                iconDispGx(&local_98, 0x10, iconOff, float_1_80426870);
            }
        } else {
            if (flag > 0) {
                local_a4 = vecs[8];
                local_a4.x = float_neg200_80426868 + x;
                local_a4.y = float_50_8042686c + y;
                iconDispGx(&local_a4, 0x10, iconOff, float_1_80426870);
            } else {
                local_b0 = vecs[9];
                local_b0.x = float_neg200_80426868 + x;
                local_b0.y = float_50_8042686c + y;
                iconDispGx(&local_b0, 0x10, iconOn, float_1_80426870);
            }
        }
    }

done:
    *(s32*)disp = *(s32*)disp + 1;
}
