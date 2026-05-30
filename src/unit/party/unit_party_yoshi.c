#include "unit/party/unit_party_yoshi.h"

void _btlYoshiDisp(void* camera, void* work) {
    extern f32 float_0_804240b4;
    extern f32 float_0p4_804240ac;
    extern f32 float_neg0p4_804240b0;
    extern f32 float_20_804240b8;
    extern s32 dat_804240a8;
    extern u8 _yoshig_tpl_no_tbl;
    extern void btlDispTexPlane(s32 tpl, void* color, s32 flags, f32 x, f32 y, f32 z, f32 rx, f32 scale);
    s32 color;
    f32 rx;

    if (*(f32*)((s32)work + 0xC) < float_0_804240b4) {
        rx = float_0p4_804240ac;
    } else {
        rx = float_neg0p4_804240b0;
    }
    color = dat_804240a8;
    btlDispTexPlane(((u8*)&_yoshig_tpl_no_tbl)[*(u8*)((s32)work + 0x25)], &color, 0,
                    *(f32*)work, float_20_804240b8 + *(f32*)((s32)work + 4),
                    *(f32*)((s32)work + 8), rx, float_0p4_804240ac);
}


u8 _gundan_yoshi_run_effect(void) {
    return 0;
}


u8 btl_yoshi_yoroyoro_jump_move(void) {
    return 0;
}


void __makeTechMenuFunc(void* commandWork, s32 param_2) {
}


u8 btl_yoshi_yoroyoro_jump_calc_param(void) {
    return 0;
}


s32 yoshi_original_color_anim_set(int param_1, int param_2) {
    return 0;
}


s32 _get_nomikomi_hit_position(int param_1) {
    return 0;
}


s32 _check_swallow_attribute(int param_1) {
    return 0;
}


s32 _wait_yoshig_run(int param_1) {
    return 0;
}


s32 _get_swallow_param(int param_1) {
    return 0;
}


u32 _wait_yoshig_complete(int param_1) {
    return 0;
}


void _yoshi_slide_move_sound(void* unit, s32 param_2) {
}
