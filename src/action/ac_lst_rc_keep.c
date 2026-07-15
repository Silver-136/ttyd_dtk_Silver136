#include "action/ac_lst_rc_keep.h"

extern f32 float_neg300_80424968;
extern f32 float_30_8042496c;
extern f32 float_0_80424970;
f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);

typedef struct LstRcKeepDisp {
    u8 pad0[0x14];
    f32 x;
    f32 y;
    u8 pad1[4];
    s32 timer;
    s32 icon;
} LstRcKeepDisp;

s32 battleAcResult_LstRcKeep(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_LstRcKeep(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1005;
}


s32 battleAcMain_LstRcKeep(void* battleWork) {
    extern void* memset(void*, s32, u32);
    extern s32 irand(s32);
    extern u32 BattlePadCheckTrigger(u32);
    extern u32 BattlePadCheckNow(u32);
    extern u32 BattleActionCommandGetDifficulty(void*);
    extern void iconDelete(char*);
    extern s32 ac_lstrckeep_ok_frame[];
    extern char* ac_lstrckeep_iconname[];

    void* extra;
    void* disp;
    s32 autoCommand;
    s32 done;
    s32 state;
    s32 timer;
    s32 idx;
    s32 ok;
    s32 difficulty;
    s32 i;
    s16 holdA;
    s16 holdB;

    extra = (void*)((s32)battleWork + 0x1F4C);
    disp = (void*)((s32)battleWork + 0x1F20);
    autoCommand = *(u8*)((s32)*(void**)((s32)battleWork + 0x1C90) + 0x307) != 0;

    while (1) {
        done = 0;
        state = *(s32*)((s32)battleWork + 0x1C9C);

        if (state == 0) {
            *(s32*)((s32)battleWork + 0x1CB8) = 1;
            *(s32*)((s32)extra + 4) = 0;
            memset(disp, 0, 0x2C);
            *(f32*)((s32)disp + 0x14) = float_neg300_80424968;
            *(f32*)((s32)disp + 0x18) = float_0_80424970;
            *(s32*)((s32)disp + 0x20) = 0x14;
            *(s32*)((s32)battleWork + 0x1C9C) = 99;
        } else if (state == 99) {
            ;
        } else if (state >= 100 && state < 1000) {
            if (state <= 100) {
                *(s32*)extra = 0x78;
                *(s16*)((s32)extra + 8) = 0;
                *(s16*)((s32)extra + 0xA) = 0;
                *(s32*)((s32)battleWork + 0x1C9C) = 1000;
            }
            if (((*(u32*)((s32)battleWork + 0x1C94) & 1) == 0) ||
                ((*(u32*)((s32)*(void**)((s32)battleWork + 0x1C90) + 0x27C) & 0x10) == 0)) {
                holdA = *(s16*)((s32)extra + 8);
                if (holdA == 0) {
                    if (BattlePadCheckTrigger(0x40000) != 0) {
                        *(s16*)((s32)extra + 8) = 1;
                    }
                } else {
                    *(s16*)((s32)extra + 8) = holdA + 1;
                    if (BattlePadCheckNow(0x40000) == 0) {
                        *(s16*)((s32)extra + 8) = 0;
                    }
                }
                holdB = *(s16*)((s32)extra + 0xA);
                if (holdB == 0) {
                    if (BattlePadCheckTrigger(0x800000) != 0) {
                        *(s16*)((s32)extra + 0xA) = 1;
                    }
                } else {
                    *(s16*)((s32)extra + 0xA) = holdB + 1;
                    if (BattlePadCheckNow(0x800000) == 0) {
                        *(s16*)((s32)extra + 0xA) = 0;
                    }
                }
                if (((*(s16*)((s32)extra + 8) != 0) && (*(s16*)((s32)extra + 0xA) != 0)) ||
                    (*(s16*)((s32)battleWork + 0x1D18) > 0) || autoCommand) {
                    *(s32*)((s32)battleWork + 0x1C9C) = 1001;
                    *(s32*)extra = *(s32*)((s32)battleWork + 0x1CC0);
                    *(s32*)((s32)extra + 4) = 0;
                    *(s16*)((s32)extra + 8) = 0;
                    *(s16*)((s32)extra + 0xA) = 0;
                } else {
                    timer = *(s32*)extra - 1;
                    *(s32*)extra = timer;
                    if (timer < 1) {
                        *(s32*)((s32)battleWork + 0x1CB8) = 0;
                        *(s32*)((s32)battleWork + 0x1C9C) = 1003;
                        done = 1;
                    }
                }
            } else if (((*(u32*)((s32)battleWork + 0x1C94) & 2) == 0) ||
                       (--*(s32*)((s32)battleWork + 0x1C98) < 0)) {
                if (irand(100) < 0) {
                    *(s32*)((s32)battleWork + 0x1CB8) = 2;
                    *(s32*)((s32)battleWork + 0x1CB4) = *(s32*)((s32)battleWork + 0x1CB4) + 1;
                } else {
                    *(s32*)((s32)battleWork + 0x1CB8) = 0;
                }
                return 0;
            }
        } else if (state == 1001) {
            if (*(s16*)((s32)battleWork + 0x1D18) > 0) {
                *(s16*)((s32)battleWork + 0x1D18) = *(s16*)((s32)battleWork + 0x1D18) - 1;
                *(s32*)((s32)battleWork + 0x1CB8) = 2;
                *(s32*)((s32)battleWork + 0x1C9C) = 1003;
                done = 1;
            }
            *(s32*)extra = *(s32*)extra - 1;
            holdA = *(s16*)((s32)extra + 8);
            if (holdA < 1) {
                if (BattlePadCheckNow(0x40000) == 0) {
                    *(s16*)((s32)extra + 8) = 1;
                }
            } else {
                *(s16*)((s32)extra + 8) = holdA + 1;
            }
            holdB = *(s16*)((s32)extra + 0xA);
            if (holdB < 1) {
                if (BattlePadCheckNow(0x800000) == 0) {
                    *(s16*)((s32)extra + 0xA) = 1;
                }
            } else {
                *(s16*)((s32)extra + 0xA) = holdB + 1;
            }
            if ((autoCommand) || ((*(s16*)((s32)extra + 8) < 6) && (*(s16*)((s32)extra + 0xA) < 6))) {
                timer = *(s32*)extra;
                if (timer < 0) {
                    *(s32*)((s32)extra + 4) = *(s32*)((s32)extra + 4) + 1;
                    idx = *(s32*)((s32)extra + 4);
                    if (idx < 5) {
                        *(s32*)extra = *(s32*)((s32)battleWork + 0x1CC0 + idx * 4);
                    } else {
                        *(s32*)((s32)battleWork + 0x1CB8) &= ~1;
                        *(s32*)((s32)battleWork + 0x1C9C) = 1003;
                        done = 1;
                    }
                } else if (((*(s16*)((s32)extra + 8) != 0) && (*(s16*)((s32)extra + 0xA) != 0)) || autoCommand) {
                    ok = 0;
                    if (*(s32*)((s32)extra + 4) == 4) {
                        difficulty = BattleActionCommandGetDifficulty(battleWork);
                        if (*(s32*)((s32)battleWork + 0x1CD0) - timer <= ac_lstrckeep_ok_frame[difficulty]) {
                            ok = 1;
                        }
                    }
                    done = 1;
                    if (ok) {
                        *(s32*)((s32)battleWork + 0x1CB8) |= 2;
                    }
                    *(s32*)((s32)battleWork + 0x1CB8) &= ~1;
                    *(s32*)((s32)battleWork + 0x1C9C) = 1003;
                }
            } else {
                *(s32*)((s32)battleWork + 0x1CB8) &= ~1;
                *(s32*)((s32)battleWork + 0x1C9C) = 1003;
                done = 1;
            }
        } else if (state == 1003) {
            idx = *(s32*)((s32)extra + 4) - 1;
            for (i = 0; i <= idx; i++) {
                iconDelete(ac_lstrckeep_iconname[i]);
            }
            *(s32*)((s32)battleWork + 0x1C9C) = 1004;
        } else if (state == 1005) {
            *(s32*)((s32)extra + 0xC) = 0x3C;
            *(s32*)((s32)battleWork + 0x1C9C) = 1006;
        } else if (state == 1006) {
            *(s32*)((s32)extra + 0xC) = *(s32*)((s32)extra + 0xC) - 1;
            if (*(s32*)((s32)extra + 0xC) < 1) {
                *(s32*)((s32)battleWork + 0x1C9C) = 1007;
            }
        } else if (state == 1007) {
            *(void**)((s32)battleWork + 0x1CA0) = 0;
            *(void**)((s32)battleWork + 0x1CA4) = 0;
            *(void**)((s32)battleWork + 0x1CAC) = 0;
            *(void**)((s32)battleWork + 0x1CA8) = 0;
        }

        if (!done) {
            return 1;
        }
    }
}

void actionCommandDisp(f32 x, f32 y, s32 icon) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern void* camGetPtr(s32);
    extern void iconDispGx(f32, VecLocal*, s32, s32);
    extern f32 float_neg168_80424948;
    extern f32 float_70_8042494c;
    extern f32 float_1_80424950;
    extern f32 float_neg232_80424954;
    extern f32 float_neg200_80424958;
    extern f32 float_25_8042495c;
    extern f32 float_0p5_80424960;
    extern f32 float_neg151_80424964;

    VecLocal posA;
    VecLocal posB;
    VecLocal posC;
    VecLocal posD;
    VecLocal posE;
    VecLocal posLoopOn;
    VecLocal posLoopOff;
    VecLocal posEndOn;
    VecLocal posEndOff;
    s32 i;
    s32 offset;
    f32 yPos;

    camGetPtr(8);
    if (icon < 3) {
        posA.x = float_neg168_80424948 + x;
        posA.y = float_70_8042494c + y;
        posA.z = 0.0f;
        iconDispGx(float_1_80424950, &posA, 0x10, 0x79);
    } else {
        posB.x = float_neg168_80424948 + x;
        posB.y = float_70_8042494c + y;
        posB.z = 0.0f;
        iconDispGx(float_1_80424950, &posB, 0x10, 0x78);
    }

    if (icon < 3) {
        posC.x = float_neg232_80424954 + x;
        posC.y = float_70_8042494c + y;
        posC.z = 0.0f;
        iconDispGx(float_1_80424950, &posC, 0x10, 0x80);
    } else {
        posD.x = float_neg232_80424954 + x;
        posD.y = float_70_8042494c + y;
        posD.z = 0.0f;
        iconDispGx(float_1_80424950, &posD, 0x10, 0x81);
    }

    yPos = float_25_8042495c + y;
    posE.x = float_neg200_80424958 + x;
    posE.y = yPos;
    posE.z = 0.0f;
    iconDispGx(float_1_80424950, &posE, 0x10, 0x96);

    offset = 0;
    for (i = 0; i < 3; i++, offset += 0x21) {
        if (icon < i) {
            posLoopOff.x = x + (f32)(offset - 0x102);
            posLoopOff.y = yPos;
            posLoopOff.z = 0.0f;
            iconDispGx(float_0p5_80424960, &posLoopOff, 0x10, 0x99);
        } else {
            posLoopOn.x = x + (f32)(offset - 0x102);
            posLoopOn.y = yPos;
            posLoopOn.z = 0.0f;
            iconDispGx(float_1_80424950, &posLoopOn, 0x10, i + 0x9A);
        }
    }

    if (icon < 3) {
        posEndOn.x = float_neg151_80424964 + x;
        posEndOn.y = yPos;
        posEndOn.z = 0.0f;
        iconDispGx(float_1_80424950, &posEndOn, 0x10, 0x99);
    } else {
        posEndOff.x = float_neg151_80424964 + x;
        posEndOff.y = yPos;
        posEndOff.z = 0.0f;
        iconDispGx(float_1_80424950, &posEndOff, 0x10, 0x9D);
    }
}

void battleAcDisp_LstRcKeep(s32 unused, void* battleWork) {
    volatile s32* work;
    register LstRcKeepDisp* disp;
    s32 state;
    s32 timer;

    work = (volatile s32*)((s32)battleWork + 0x1F4C);
    disp = (LstRcKeepDisp*)((s32)battleWork + 0x1F20);
    state = *(s32*)((s32)battleWork + 0x1C9C);
    if (state == 1001) {
        goto disp_keep;
    }
    if (state >= 1001) {
        goto ge_1001;
    }
    if (state >= 101) {
        goto check_1000;
    }
    if (state >= 99) {
        goto disp_in;
    }
    return;

check_1000:
    if (state >= 1000) {
        goto disp_in;
    }
    return;

disp_in:
    disp->icon = -1;
    timer = disp->timer;
    disp->x = intplGetValue(4, 0x14 - timer, float_neg300_80424968, float_30_8042496c, 0x14);
    actionCommandDisp(disp->x, disp->y, disp->icon);
    timer = disp->timer;
    if (timer > 0) {
        disp->timer = timer - 1;
    }
    return;

disp_keep:
    disp->icon = work[1] - 1;
    timer = disp->timer;
    disp->x = intplGetValue(4, 0x14 - timer, float_neg300_80424968, float_30_8042496c, 0x14);
    actionCommandDisp(disp->x, disp->y, disp->icon);
    timer = disp->timer;
    if (timer > 0) {
        disp->timer = timer - 1;
    }
    return;

ge_1001:
    if (state >= 1007) {
        return;
    }
    if (state >= 1005) {
        goto disp_out;
    }
    actionCommandDisp(float_30_8042496c, float_0_80424970, disp->icon);
    return;

disp_out:
    timer = disp->timer;
    if (timer >= 40) {
        disp->x = intplGetValue(4, timer - 40, float_30_8042496c, float_neg300_80424968, 0x14);
    } else {
        disp->x = float_30_8042496c;
    }
    actionCommandDisp(disp->x, disp->y, disp->icon);
    timer = disp->timer;
    if (timer < 60) {
        disp->timer = timer + 1;
    }
}
