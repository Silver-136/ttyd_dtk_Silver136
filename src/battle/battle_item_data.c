#include "battle/battle_item_data.h"

void _disp_item_icon(void* unused, void* work) {
    extern u8 itemDataTable[];
    extern f32 float_1_80422800;
    extern void iconDispGx(void* pos, s32 flags, s32 iconId, f32 scale);

    void* pos = (void*)((s32)*(void**)((s32)work + 0x78) + 0xC);
    s32 item = *(s32*)((s32)work + 0x7C);
    u8* data = itemDataTable;
    s32 icon = *(u16*)(data + item * 0x28 + 0x20);
    f32 scale = float_1_80422800;

    iconDispGx(pos, 0, icon, scale);
}


u8 _throw_item_icon(void) {
    return 0;
}


u32 _naniga_okorukana_check(int param_1, int param_2) {
    return 0;
}


u8 _nancy_disp(void) {
    return 0;
}


s32 _nancy_mirage(int param_1, int param_2) {
    return 0;
}


s32 _get_throw_first_pos(int param_1) {
    return 0;
}


s32 _sampling_random_target(int param_1) {
    return 0;
}


s32 _hpfp_change_declare_2(int param_1) {
    return 0;
}


u8 _cam_rot(void) {
    return 0;
}


s32 _hpfp_change_declare_1(int param_1) {
    return 0;
}


s32 _rank_up(int param_1) {
    return 0;
}


s32 _full_ap_recover(int param_1) {
    return 0;
}
