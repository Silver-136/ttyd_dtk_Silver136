#include "party/party_yoshi.h"

extern s32 evt_yoshi_frm;
extern f32 evt_yoshi_spd;
extern f32 float_26_804211e0;
extern f32 float_20_804211e4;

s32 sysMsec2Frame(s32 msec);

s32 evt_get_yoshi_frm(void) {
    return evt_yoshi_frm;
}

void evt_set_yoshi_spd(f32 speed) {
    evt_yoshi_spd = speed;
}

void evt_set_yoshi_frm(s32 msec) {
    evt_yoshi_frm = sysMsec2Frame(msec);
}

void yoshi_init(void* party) {
    f32 offset;
    f32 height;

    *(u8*)((s32)party + 0x33) = 4;
    height = float_26_804211e0;
    *(u8*)((s32)party + 0x32) = 0;
    offset = float_20_804211e4;
    *(f32*)((s32)party + 0xF0) = height;
    *(f32*)((s32)party + 0xF4) = offset;
    *(s32*)((s32)party + 0x168) = -1;
}

