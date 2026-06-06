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
    return 0;
}


void actionCommandDisp(f32 x, f32 y, s32 icon) {
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
