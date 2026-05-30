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


u8 yoshi_use(s32 pParty) {
    return 0;
}


u8 searchGround(s32 pParty) {
    return 0;
}


u8 checkWall(s64 speed, s64 direction, s32 pParty) {
    return 0;
}


u8 searchGround2(void* pParty) {
    return 0;
}


u8 inertia(void* pParty) {
    return 0;
}


u8 yoshi_exit(void* pParty) {
    return 0;
}


u8 marioYoshiForceCancel(void) {
    return 0;
}


u8 L_getStick(void* pParty, s32 param_2, s32 param_3) {
    return 0;
}


u8 yoshi_bye(void* pParty) {
    return 0;
}


u8 L_yoshiFlyMove(void* pParty) {
    return 0;
}


u32 marioGetoffYoshi(void) {
    return 0;
}


u8 L_sweatEntry(void* pParty, void* hitobjStandOn) {
    return 0;
}


s32 yoshiGetStatus(void) {
    return 0;
}


u8 marioRideYoshi(void) {
    return 0;
}


u8 L_sweatMain(void* pParty) {
    return 0;
}


u8 yoshi_jumpStand(void) {
    return 0;
}
