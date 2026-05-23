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
