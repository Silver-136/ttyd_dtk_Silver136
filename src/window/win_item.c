#include "window/win_item.h"

extern f32 float_190_80423848;
extern f32 float_0p25_8042384c;
extern f32 float_6_80423850;
extern f32 float_neg275_80423858;
extern f32 float_320_8042382c;
extern f32 float_neg240_804237f4;
extern char str_msg_menu_mochi_item_802f5e44[];
extern char str_msg_menu_mochi_daiji_802f5e58[];
extern u16 menu_skip_list[18];
void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);
u16 pouchKeyItem(s32 index);

void winItemExit(void* work) {
    extern void winMgrDelete(void* window);

    winMgrDelete(*(void**)((s32)work + 0x1210));
    winMgrDelete(*(void**)((s32)work + 0x1214));
}


u8 itemUseDisp(void* pWinMgr) {
    return 0;
}


u8 itemUseDisp2(void* pWinMgr) {
    return 0;
}


s32 winItemMain(void* pWin) {
    return 0;
}


u8 item_disp(s64 param_1, s64 param_2, void* pWin) {
    return 0;
}


u8 winItemDisp(s32 param_1, void* pWin, s32 param_3) {
    return 0;
}


u8 winItemInit(void* pWin) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winItemInit2(void* pWin) {
    char* msg = str_msg_menu_mochi_daiji_802f5e58;
    f32 scale = float_190_80423848;
    f32 pos = float_320_8042382c;
    f32 neg = float_neg240_804237f4;
    f32 value;

    *(f32*)((s32)pWin + 0x158) = float_neg275_80423858;
    *(f32*)((s32)pWin + 0x15C) = (f32)(0x7D - *(s32*)((s32)pWin + 0x210) * 0x28);
    value = scale * (f32)*(s32*)((s32)pWin + 0x21C);
    *(f32*)((s32)pWin + 0x22C) = value;
    *(f32*)((s32)pWin + 0x224) = value;
    value = scale * (f32)*(s32*)((s32)pWin + 0x220);
    *(f32*)((s32)pWin + 0x230) = value;
    *(f32*)((s32)pWin + 0x228) = value;
    *(f32*)((s32)pWin + 0x238) = pos;
    *(f32*)((s32)pWin + 0x234) = pos;
    *(f32*)((s32)pWin + 0x240) = neg;
    *(f32*)((s32)pWin + 0x23C) = neg;
    *(f32*)((s32)pWin + 0x248) = pos;
    *(f32*)((s32)pWin + 0x244) = pos;
    *(f32*)((s32)pWin + 0x250) = neg;
    *(f32*)((s32)pWin + 0x24C) = neg;
    value = (f32)(*(s32*)((s32)pWin + 0x258) * 0xC8);
    *(f32*)((s32)pWin + 0x264) = value;
    *(f32*)((s32)pWin + 0x260) = value;
    if (*(s32*)((s32)pWin + 0x210) == 0) {
        msg = str_msg_menu_mochi_item_802f5e44;
    }
    winMsgEntry(pWin, 0, msg, 0);
    *(s32*)((s32)pWin + 0x2D4) = 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winMakeSkipList(void* pWin) {
    s32 i = 0;
    u16 item;
    s32 j;
    u16* skip;
    s32 count;

    *(u16*)((s32)pWin + 0x3DA) = 0;
    for (; i < 0x79; i++) {
        item = pouchKeyItem(i);
        if (item != 0) {
            skip = menu_skip_list;
            j = 0;
            while (j < 0x12) {
                if (item == *skip) {
                    break;
                }
                skip++;
                j++;
            }
            if ((u32)j >= 0x12) {
                count = *(s16*)((s32)pWin + 0x3DA);
                *(u16*)((s32)pWin + 0x2E8 + count * 2) = item;
                *(u16*)((s32)pWin + 0x3DA) = count + 1;
            }
        }
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void winItemMain2(void* pWin) {
    f32 scale = float_190_80423848;
    f32 rate = float_0p25_8042384c;
    f32 div = float_6_80423850;
    f32 target;
    f32 current;
    f32 diff;

    target = scale * (f32)*(s32*)((s32)pWin + 0x21C);
    *(f32*)((s32)pWin + 0x22C) = target;
    target = *(f32*)((s32)pWin + 0x22C);
    current = *(f32*)((s32)pWin + 0x224);
    diff = target - current;
    *(f32*)((s32)pWin + 0x224) = diff * rate + current;

    target = scale * (f32)*(s32*)((s32)pWin + 0x220);
    *(f32*)((s32)pWin + 0x230) = target;
    target = *(f32*)((s32)pWin + 0x230);
    current = *(f32*)((s32)pWin + 0x228);
    diff = target - current;
    *(f32*)((s32)pWin + 0x228) = diff * rate + current;

    current = *(f32*)((s32)pWin + 0x234);
    target = *(f32*)((s32)pWin + 0x238);
    *(f32*)((s32)pWin + 0x234) = current + (target - current) / div;

    current = *(f32*)((s32)pWin + 0x23C);
    target = *(f32*)((s32)pWin + 0x240);
    *(f32*)((s32)pWin + 0x23C) = current + (target - current) / div;

    current = *(f32*)((s32)pWin + 0x244);
    target = *(f32*)((s32)pWin + 0x248);
    *(f32*)((s32)pWin + 0x244) = current + (target - current) / div;

    current = *(f32*)((s32)pWin + 0x24C);
    target = *(f32*)((s32)pWin + 0x250);
    *(f32*)((s32)pWin + 0x24C) = current + (target - current) / div;

    target = (f32)(*(s32*)((s32)pWin + 0x25C) * 0xC8);
    *(f32*)((s32)pWin + 0x264) = target;
    target = *(f32*)((s32)pWin + 0x264);
    current = *(f32*)((s32)pWin + 0x260);
    diff = target - current;
    *(f32*)((s32)pWin + 0x260) = diff * rate + current;
}
