#include "window/win_item.h"

extern f32 float_190_80423848;
extern f32 float_0p25_8042384c;
extern f32 float_6_80423850;
extern f32 float_neg275_80423858;
extern f32 float_320_8042382c;
extern f32 float_neg240_804237f4;
extern char str_msg_menu_mochi_item_802f5e44[];
extern char str_msg_menu_mochi_daiji_802f5e58[];
extern s32 pouchGetHaveItemCnt(void);
extern char* msgSearch(char* key);
extern s32 pouchHaveItem(s32 index);
extern void winTexInit(void* data);
extern s32 winMgrAction(s32 entryId);
extern u8 itemDataTable[];
extern void winIconInit(void);
extern void winIconGrayInit(void);
extern void winFontInit(void);
extern void* winGetPtr(void);
extern u16 menu_skip_list[18];
void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);
u16 pouchKeyItem(s32 index);
void item_disp(f32 x, f32 y, void* pWin);
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winMakeSkipList(void* pWin) {
    s32 i;
    u16 item;
    u16* skip;
    s32 skipIndex;
    s32 loop;
    s32 count;

    i = 0;
    *(u16*)((s32)pWin + 0x3DA) = 0;
    while (i < 0x79) {
        item = pouchKeyItem(i);
        if (item != 0) {
            skipIndex = 0;
            skip = menu_skip_list;
            for (loop = 0; loop < 2; loop++) {
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
            }
            if ((u32)skipIndex >= 0x12) {
                count = *(s16*)((s32)pWin + 0x3DA);
                *(u16*)((s32)pWin + 0x2E8 + count * 2) = item;
                *(u16*)((s32)pWin + 0x3DA) = count + 1;
            }
        }
        i++;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 winItemInit(void* pWin) {
    extern f32 float_0_804237b8;
    extern void* window_desc;
    extern void* gp;
    extern char str_jp_80423870[];
    extern char str_us_80423874[];
    extern char str_PCTs_w_PCTs_mail_tpl_802f5e70[];
    extern s32 pouchCheckItem(s32 itemId);
    extern s32 pouchGetItem(s32 itemId);
    extern s32 pouchReceiveMailCount(void);
    extern void* pouchGetPtr(void);
    extern s32 winMgrEntry(void* desc);
    extern void winMgrSetPriority(s32 entryId, s32 priority);
    extern s32 getMarioStDvdRoot(void);
    extern u32 fileAsyncf(s32, s32, char*, s32, char*);
    s32 i;
    void* out;
    s32 count;
    u16 item;
    u16* skip;
    s32 skipIndex;
    s32 loop;
    s32 langRoot;
    char* lang;

    *(s32*)((s32)pWin + 0x20C) = 0;
    *(s32*)((s32)pWin + 0x210) = 0;
    *(s32*)((s32)pWin + 0x214) = 0;
    *(s32*)((s32)pWin + 0x21C) = 0;
    *(s32*)((s32)pWin + 0x218) = 0;
    *(s32*)((s32)pWin + 0x220) = 0;
    *(f32*)((s32)pWin + 0x238) = float_320_8042382c;
    *(f32*)((s32)pWin + 0x234) = float_320_8042382c;
    *(f32*)((s32)pWin + 0x240) = float_neg240_804237f4;
    *(f32*)((s32)pWin + 0x23C) = float_neg240_804237f4;
    *(f32*)((s32)pWin + 0x248) = float_320_8042382c;
    *(f32*)((s32)pWin + 0x244) = float_320_8042382c;
    *(f32*)((s32)pWin + 0x250) = float_neg240_804237f4;
    *(f32*)((s32)pWin + 0x24C) = float_neg240_804237f4;

    if (pouchCheckItem(0x36) == 0) {
        pouchGetItem(0x36);
    }

    *(s32*)((s32)pWin + 0x254) = pouchReceiveMailCount();
    *(s32*)((s32)pWin + 0x258) = 0;
    *(s32*)((s32)pWin + 0x25C) = 0;
    *(f32*)((s32)pWin + 0x264) = float_0_804237b8;
    *(f32*)((s32)pWin + 0x260) = float_0_804237b8;

    out = pWin;
    i = 0;
    while (i < *(s32*)((s32)pWin + 0x254)) {
        *(u8*)((s32)out + 0x26E) = (u8)(*(s32*)((s32)pWin + 0x254) - i - 1);
        *(u8*)((s32)out + 0x26F) = *(u8*)((s32)pouchGetPtr() + (*(s32*)((s32)pWin + 0x254) - i) + 0x519);
        i++;
        out = (void*)((s32)out + 2);
    }

    count = 0;
    *(s32*)((s32)pWin + 0x2D4) = 0;
    *(u16*)((s32)pWin + 0x3DA) = 0;
    while (count < 0x79) {
        item = pouchKeyItem(count);
        if (item != 0) {
            skipIndex = 0;
            skip = menu_skip_list;
            for (loop = 0; loop < 2; loop++) {
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
                if (item == *skip) break;
                skip++;
                skipIndex++;
            }
            if ((u32)skipIndex >= 0x12) {
                *(u16*)((s32)pWin + 0x2E8 + (*(s16*)((s32)pWin + 0x3DA) * 2)) = item;
                *(u16*)((s32)pWin + 0x3DA) = *(s16*)((s32)pWin + 0x3DA) + 1;
            }
        }
        count++;
    }

    *(s32*)((s32)pWin + 0x1210) = winMgrEntry(&window_desc);
    *(s32*)((s32)pWin + 0x1214) = winMgrEntry((void*)((s32)&window_desc + 0x28));
    winMgrSetPriority(*(s32*)((s32)pWin + 0x1210), -0xC8);
    winMgrSetPriority(*(s32*)((s32)pWin + 0x1214), -0x64);
    lang = str_us_80423874;
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        lang = str_jp_80423870;
    }
    langRoot = getMarioStDvdRoot();
    fileAsyncf(4, 0, str_PCTs_w_PCTs_mail_tpl_802f5e70, langRoot, lang);
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

void winItemExit(void* work) {
    extern void winMgrDelete(void* window);

    winMgrDelete(*(void**)((s32)work + 0x1210));
    winMgrDelete(*(void**)((s32)work + 0x1214));
}

s32 winItemMain(void* pWin) {
    extern s32 pouchGetHaveItemCnt(void);
    extern s32 pouchHaveItem(s32 index);
    extern void winMsgEntry(void* win, s32 item, char* text, s32 type);
    extern void winSortEntry(f32 x, f32 y, void* win, s32 type);
    extern char* msgSearch(char* key);
    extern u8 itemDataTable[];
    extern void psndSFXOn(s32 id);
    extern char str_msg_menu_mochi_item_802f5e44[];
    extern char str_msg_menu_mochi_daiji_802f5e58[];
    u8* w = (u8*)pWin;
    s32 state = *(s32*)(w + 0x20C);
    u32 sub = *(u32*)(w + 0x210);
    s32 count;
    s32 pages;
    s32 cursor;
    s32 page;
    s32 item;
    u32 pressed = *(u32*)(w + 4);
    u32 repeat = *(u32*)(w + 8);
    u32 dirs = *(u32*)(w + 0x10);

    if (state == 0) {
        if ((pressed & 0x200) != 0) {
            psndSFXOn(0x20013);
            return -1;
        }
        if ((pressed & 0x1000) != 0) {
            return -2;
        }
        if ((dirs & 0x3000) != 0) {
            sub = 1 - sub;
            *(s32*)(w + 0x210) = sub;
            psndSFXOn(0x20005);
        } else if ((pressed & 0x100) != 0) {
            if (sub == 0) {
                if (pouchGetHaveItemCnt() == 0) {
                    return 0;
                }
            } else if (*(s16*)(w + 0x3DA) == 0) {
                return 0;
            }
            *(s32*)(w + 0x20C) = 10;
            psndSFXOn(0x20012);
            return 0;
        } else if ((pressed & 0x10) != 0) {
            winSortEntry(-310.0f, 150.0f, pWin, sub != 0);
            *(s32*)(w + 0x124) = state;
            psndSFXOn(0x20012);
            *(s32*)(w + 0x20C) = 1000;
        }
        *(f32*)(w + 0x158) = -275.0f;
        *(f32*)(w + 0x15C) = (f32)(125 - sub * 50);
        winMsgEntry(pWin, 0,
                    sub == 0 ? str_msg_menu_mochi_item_802f5e44
                             : str_msg_menu_mochi_daiji_802f5e58,
                    0);
        return 0;
    }

    if (state == 10) {
        if (sub == 0) {
            count = pouchGetHaveItemCnt();
        } else {
            count = *(s16*)(w + 0x3DA);
        }
        pages = (count + 9) / 10;
        cursor = *(s32*)(w + 0x214 + sub * 4);
        page = *(s32*)(w + 0x21C + sub * 4);

        if ((pressed & 0x200) != 0) {
            psndSFXOn(0x20013);
            *(s32*)(w + 0x20C) = 0;
        } else if ((pressed & 0x100) == 0) {
            if ((repeat & 0x40) != 0) {
                page--;
                if (page < 0) page = 0;
                cursor = page * 10;
                psndSFXOn(0x20035);
            } else if ((repeat & 0x20) != 0) {
                page++;
                if (page >= pages) page = pages - 1;
                cursor = page * 10;
                psndSFXOn(0x20035);
            } else if ((dirs & 0xC000) != 0) {
                if ((cursor & 1) == 0) {
                    if (cursor + 1 < count) cursor++;
                } else if (cursor - 1 >= 0) {
                    cursor--;
                }
                psndSFXOn(0x20035);
            } else if ((dirs & 0x1000) != 0) {
                if (cursor - 2 >= 0) cursor -= 2;
                page = cursor / 10;
                psndSFXOn(0x20035);
            } else if ((dirs & 0x2000) != 0) {
                if (cursor + 2 < count) {
                    cursor += 2;
                } else if (cursor + (cursor & 1) < count) {
                    cursor = count - 1;
                }
                page = cursor / 10;
                psndSFXOn(0x20035);
            } else if ((pressed & 0x10) != 0) {
                winSortEntry(-310.0f, 150.0f, pWin, sub != 0);
                *(s32*)(w + 0x124) = state;
                psndSFXOn(0x20012);
                *(s32*)(w + 0x20C) = 1000;
            } else if ((pressed & 0x1000) != 0) {
                return -2;
            }
        } else if (sub == 0 && count != 0) {
            item = pouchHaveItem(cursor);
            *(s32*)(w + 0x2D4) = item;
            *(s32*)(w + 0x2D8) = cursor;
            *(s32*)(w + 0x20C) = 300;
            *(s32*)(w + 0x2DC) = 0;
            *(s32*)(w + 0x2E0) = 0;
            psndSFXOn(0x20012);
        }

        *(s32*)(w + 0x214 + sub * 4) = cursor;
        *(s32*)(w + 0x21C + sub * 4) = page;
        *(f32*)(w + 0x158) = (f32)((cursor & 1) * 204 - 155);
        *(f32*)(w + 0x15C) = (f32)(110 - ((cursor % 10) / 2) * 38);
        if (sub == 0) {
            item = pouchHaveItem(cursor);
        } else {
            item = *(s16*)(w + 0x2E8 + cursor * 2);
        }
        winMsgEntry(pWin, item,
                    *(char**)(itemDataTable + item * 0x28 + 0xC), 0);
        return 0;
    }

    if (state == 100) {
        if ((pressed & 0x100) != 0) {
            psndSFXOn(0x20012);
            *(s32*)(w + 0x2D0) += 1;
            if (*(s32*)(w + 0x2D0) >= *(s32*)(w + 0x2CC)) {
                *(s32*)(w + 0x2D0) = *(s32*)(w + 0x2CC) - 1;
                *(s32*)(w + 0x20C) = 10;
                *(f32*)(w + 0x238) = 320.0f;
                *(f32*)(w + 0x240) = -240.0f;
                *(u16*)w &= 0x7FFF;
                *(f32*)(w + 0x150) = *(f32*)(w + 0x158);
                *(f32*)(w + 0x154) = *(f32*)(w + 0x15C);
            }
        } else if ((pressed & 0x200) != 0) {
            psndSFXOn(0x20013);
            *(s32*)(w + 0x2D0) -= 1;
            if (*(s32*)(w + 0x2D0) < 0) {
                *(s32*)(w + 0x2D0) = 0;
                *(s32*)(w + 0x20C) = 10;
                *(f32*)(w + 0x238) = 320.0f;
                *(f32*)(w + 0x240) = -240.0f;
                *(u16*)w &= 0x7FFF;
                *(f32*)(w + 0x150) = *(f32*)(w + 0x158);
                *(f32*)(w + 0x154) = *(f32*)(w + 0x15C);
            }
        } else if ((pressed & 0x1000) != 0) {
            *(f32*)(w + 0x238) = 320.0f;
            *(f32*)(w + 0x240) = -240.0f;
            *(u16*)w &= 0x7FFF;
            *(f32*)(w + 0x150) = *(f32*)(w + 0x158);
            *(f32*)(w + 0x154) = *(f32*)(w + 0x15C);
            return -2;
        }
        *(f32*)(w + 0x158) = -170.0f;
        *(f32*)(w + 0x15C) = 150.0f;
    } else if (state == 1000) {
        *(s32*)(w + 0x20C) = *(s32*)(w + 0x124);
    }
    return 0;
}

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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 winItemDisp(s32 param_1, void* pWin, s32 param_3) {
    typedef struct LocalVec {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec;

    extern void winBgGX(f32 x, f32 y, void* pWin, s32 type);
    extern void winKirinukiGX(f32 x, f32 y, f32 w, f32 h, void* pWin, s32 flag);
    extern void winTexInit_x2(void* tpl);
    extern void winTexSet(s32 texId, LocalVec* pos, LocalVec* size, u32* color);
    extern void winTexSet_x2(s32 texId0, s32 texId1, LocalVec* pos, LocalVec* size, u32* color);
    extern u32 dat_80423748;
    extern u32 dat_8042374c;
    extern u32 dat_80423750;
    extern f32 float_20_804237e8;
    extern f32 float_105_804237fc;
    extern f32 float_120_80423834;
    extern f32 float_150_80423830;
    extern f32 float_210_8042383c;
    extern f32 float_215_80423840;
    extern f32 float_415_80423838;
    extern f32 float_4_80423844;
    extern LocalVec vec3_802f5ac0;
    extern LocalVec vec3_802f5acc;
    extern LocalVec vec3_802f5ad8;
    extern LocalVec vec3_802f5ae4;
    extern LocalVec vec3_802f5af0;
    extern LocalVec vec3_802f5afc;

    f32 x;
    f32 y;
    f32 baseY;
    f32 selectedY;
    f32 selectedX;
    s32 i;
    s32 yOffset;
    u32 selectedColor;

    x = *(f32*)((s32)pWin + param_3 * 0x14 + 0xC4);
    y = *(f32*)((s32)pWin + param_3 * 0x14 + 0xC8);
    selectedColor = dat_80423748;

    winBgGX(x, y, pWin, 2);
    winKirinukiGX(x - float_150_80423830,
                  float_20_804237e8 + (float_120_80423834 + y),
                  float_415_80423838,
                  float_210_8042383c,
                  pWin,
                  0);

    baseY = float_105_804237fc + y;
    selectedY = float_4_80423844 + baseY;
    selectedX = (x - float_215_80423840) - float_4_80423844;

    for (i = 0, yOffset = 0; i < 2; i++, yOffset += 0x32) {
        if (i == *(s32*)((s32)pWin + 0x210)) {
            LocalVec rawPos;
            LocalVec pos;
            LocalVec size;
            u32 color;
            LocalVec rawPos2;
            LocalVec pos2;
            LocalVec size2;

            winTexInit(*(void**)(*(s32*)(*(s32*)((s32)pWin + 0x28) + 0xA0)));
            color = dat_8042374c;
            size = vec3_802f5acc;
            rawPos.x = x - float_215_80423840;
            rawPos.y = float_20_804237e8 + (baseY - (f32)yOffset);
            rawPos.z = vec3_802f5ac0.z;
            pos = rawPos;
            winTexSet(0xB3, &pos, &size, &color);

            winTexInit_x2(*(void**)(*(s32*)(*(s32*)((s32)pWin + 0x28) + 0xA0)));
            color = selectedColor;
            size2 = vec3_802f5ae4;
            rawPos2.x = selectedX;
            rawPos2.y = float_20_804237e8 + (selectedY - (f32)yOffset);
            rawPos2.z = vec3_802f5ad8.z;
            pos2 = rawPos2;
            winTexSet_x2(i + 0x18, 0xB3, &pos2, &size2, &color);
        } else {
            LocalVec rawPos;
            LocalVec pos;
            LocalVec size;
            u32 color;

            winTexInit_x2(*(void**)(*(s32*)(*(s32*)((s32)pWin + 0x28) + 0xA0)));
            color = dat_80423750;
            size = vec3_802f5afc;
            rawPos.x = x - float_215_80423840;
            rawPos.y = float_20_804237e8 + (baseY - (f32)yOffset);
            rawPos.z = vec3_802f5af0.z;
            pos = rawPos;
            winTexSet_x2(i + 0x18, 0xB3, &pos, &size, &color);
        }
    }

    item_disp(x, y, pWin);
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
void item_disp(f32 x, f32 y, void* pWin) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;

    extern void GXSetScissor(s32, s32, s32, s32);
    extern void winIconSet(s32, Vec3*, Vec3*, void*);
    extern void winFontSet(Vec3*, Vec3*, void*, char*, ...);
    extern void winFontSetWidth(Vec3*, Vec3*, void*, f32, char*, ...);
    extern u16 FontGetMessageWidth(char*);
    extern void winTexSet(s32, Vec3*, Vec3*, void*);
    extern void winBookGX(double, double, void*, s32);
    extern char str_msg_luigi_book1_txt1_802f5520[];
    extern u32 dat_80423754;
    extern u32 dat_80423758;
    extern u32 dat_8042375c;
    extern f32 float_125_804237f8;

    u8* catalog;
    s32 count;
    s32 i;
    s32 item;
    s32 enabled;
    s32 disabled;
    s32 row;
    s32 column;
    u8* keyItemPtr;
    f32 scroll;
    f32 rowY;
    f32 drawY;
    f32 px;
    f32 py;
    f32 width;
    char* text;

    u32 normalColor;
    u32 disabledColor;
    u32 iconColor0;
    u32 iconColor1;
    u32 iconColor2;
    u32 iconColor3;
    u32 fontColor;
    u32* fontColorPtr;
    u32 sortColor;
    u32 sortIconColor;

    Vec3 itemScaleNormal;
    Vec3 itemScaleKey;
    Vec3 itemScale;
    Vec3 itemPos;
    Vec3 itemFontScale;
    Vec3 itemFontPos;
    Vec3 upTexScale;
    Vec3 upTexPos;
    Vec3 upIconScale;
    Vec3 upIconPos;
    Vec3 downTexScale;
    Vec3 downTexPos;
    Vec3 downIconScale;
    Vec3 downIconPos;
    Vec3 sortFontScale;
    Vec3 sortFontPos;
    Vec3 sortIconScale;
    Vec3 sortIconPos;

    normalColor = dat_80423758;
    disabledColor = dat_8042375c;
    iconColor0 = dat_80423754;
    iconColor1 = dat_80423754;
    iconColor2 = dat_80423754;

    if (*(s32*)((u8*)pWin + 0x210) == 0) {
        count = pouchGetHaveItemCnt();
    } else {
        count = *(s16*)((u8*)pWin + 0x3DA);
    }

    GXSetScissor(0, 0x6D, 0x260, 0xC0);

    catalog = (u8*)str_msg_luigi_book1_txt1_802f5520;
    keyItemPtr = (u8*)pWin;

    for (i = 0; i < count; i++, keyItemPtr += 2) {
        /*
         * Match the target schedule: quotient/remainder are formed before
         * item selection.  The remainder stays live and becomes the 0xCC
         * column offset later.
         */
        row = i / 2;
        column = i % 2;

        if (*(s32*)((u8*)pWin + 0x210) == 0) {
            item = pouchHaveItem(i);
        } else {
            item = *(s16*)(keyItemPtr + 0x2E8);
        }

        enabled = 0;
        scroll = *(f32*)((u8*)pWin + 0x224 +
                         *(s32*)((u8*)pWin + 0x210) * 4);

        if (*(s32*)((u8*)pWin + 0x210) != 0) {
            enabled = 1;
        } else if ((*(u16*)(itemDataTable + item * 0x28 + 0x10) & 4) != 0) {
            enabled = 1;
        }

        rowY = y + 100.0f - (f32)(row * 0x26);
        disabled = enabled == 0;
        drawY = scroll + 20.0f + (rowY - 10.0f);

        if (drawY > 240.0f) {
            continue;
        }
        if (drawY < -240.0f) {
            break;
        }


            if (disabled != 0) {
                winIconGrayInit();
            } else {
                winIconInit();
            }

            if (*(s32*)((u8*)pWin + 0x210) == 0) {
                itemScaleNormal = *(Vec3*)(catalog + 0x5F4);
                itemScale = itemScaleNormal;
            } else {
                itemScaleKey = *(Vec3*)(catalog + 0x600);
                itemScale = itemScaleKey;
            }

            column *= 0xCC;
            itemPos.x = x - float_125_804237f8 + (f32)column;
            itemPos.y = drawY;
            itemPos.z = ((Vec3*)(catalog + 0x5E8))->z;

            iconColor3 = iconColor2;
            winIconSet(
                *(s16*)(itemDataTable + item * 0x28 + 0x20),
                &itemPos,
                &itemScale,
                &iconColor3);

            winFontInit();

            fontColorPtr = &normalColor;
            if (disabled != 0) {
                fontColorPtr = &disabledColor;
            }
            fontColor = *fontColorPtr;

            itemFontPos.x = x - 105.0f + (f32)column;
            itemFontPos.y = scroll + 20.0f + rowY;
            itemFontPos.z = ((Vec3*)(catalog + 0x60C))->z;
            itemFontScale = *(Vec3*)(catalog + 0x618);

            winFontSetWidth(
                &itemFontPos,
                &itemFontScale,
                &fontColor,
                155.0f,
                msgSearch(*(char**)(itemDataTable + item * 0x28 + 0x4)));
    }

    GXSetScissor(0, 0, 0x260, 0x1E0);

    if (*(s32*)((u8*)pWin + 0x21C +
                *(s32*)((u8*)pWin + 0x210) * 4) > 0) {
        winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));

        px = x + 250.0f;
        py = y + 110.0f;
        py = 18.0f + py;
        py = 20.0f + py;

        upTexPos.x = px;
        upTexPos.y = py;
        upTexPos.z = ((Vec3*)(catalog + 0x624))->z;
        upTexScale = *(Vec3*)(catalog + 0x630);
        winTexSet(0x17, &upTexPos, &upTexScale, &iconColor0);

        winIconInit();

        upIconPos.x = px;
        upIconPos.y = 20.0f + (y + 113.0f);
        upIconPos.z = ((Vec3*)(catalog + 0x63C))->z;
        upIconScale = *(Vec3*)(catalog + 0x648);
        winIconSet(0x86, &upIconPos, &upIconScale, &iconColor1);
    }

    if (*(s32*)((u8*)pWin + 0x21C +
                *(s32*)((u8*)pWin + 0x210) * 4) * 10 + 10 < count) {
        winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));

        px = x + 250.0f;
        py = 20.0f + ((y - 90.0f) - 15.0f);

        downTexPos.x = px;
        downTexPos.y = py;
        downTexPos.z = ((Vec3*)(catalog + 0x654))->z;
        downTexScale = *(Vec3*)(catalog + 0x660);
        winTexSet(0x17, &downTexPos, &downTexScale, &iconColor2);

        winIconInit();

        downIconPos.x = px;
        downIconPos.y = 20.0f + (y - 90.0f);
        downIconPos.z = ((Vec3*)(catalog + 0x66C))->z;
        downIconScale = *(Vec3*)(catalog + 0x678);
        winIconSet(0x88, &downIconPos, &downIconScale, &iconColor2);
    }

    text = msgSearch((char*)(catalog + 0x90C));
    width = 0.7f * (f32)FontGetMessageWidth(text);

    winFontInit();

    px = x + (220.0f - width);
    sortFontPos.x = px;
    sortFontPos.y = 20.0f + ((y - 92.0f) - 10.0f);
    sortFontPos.z = ((Vec3*)(catalog + 0x684))->z;
    sortFontScale = *(Vec3*)(catalog + 0x690);
    sortColor = normalColor;

    winFontSet(
        &sortFontPos,
        &sortFontScale,
        &sortColor,
        msgSearch((char*)(catalog + 0x90C)));

    winIconInit();

    sortIconPos.x = px - 30.0f;
    sortIconPos.y = 20.0f + ((y - 100.0f) - 10.0f);
    sortIconPos.z = ((Vec3*)(catalog + 0x69C))->z;
    sortIconScale = *(Vec3*)(catalog + 0x6A8);
    sortIconColor = dat_80423754;

    winIconSet(0x219, &sortIconPos, &sortIconScale, &sortIconColor);

    if (*(f32*)((u8*)pWin + 0x234) < 320.0f &&
        *(f32*)((u8*)pWin + 0x23C) > -240.0f) {
        winBookGX(
            *(f32*)((u8*)pWin + 0x234),
            *(f32*)((u8*)pWin + 0x23C),
            pWin,
            0);
    }
}


void itemUseDisp(void* pWinMgr) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern s32 pouchGetHP(void);
    extern s32 pouchGetMaxHP(void);
    extern s32 pouchGetFP(void);
    extern s32 pouchGetMaxFP(void);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* format, ...);
    extern void winFontSetR(Vec3* pos, Vec3* scale, void* color, char* format, ...);
    extern s32 marioBgmodeChk(void);
    extern s32 marioGetParty(void);
    extern void* marioGetPtr(void);
    extern u8 winPartyDt[];
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    u32 gray = 0x7F7F7FFF;
    u8* w = (u8*)winGetPtr();
    s32 x = *(s32*)((u8*)pWinMgr + 0x18);
    s32 y = *(s32*)((u8*)pWinMgr + 0x1C);
    s32 count = *(s32*)(w + 0x1E0) + 1;
    s32 cursor = *(s32*)(w + 0x2DC);
    s32 timer = *(s32*)(w + 0x2E0);
    void* partyOrder[11];
    s32 currentParty;
    s32 partyCount;
    s32 i;

    if (winMgrAction(*(s32*)(w + 0x1210)) == 0) {
        if (marioBgmodeChk() == 0) {
            currentParty = marioGetParty();
        } else {
            currentParty = *(s8*)((u8*)marioGetPtr() + 0x247);
        }
        partyCount = 0;
        if (*(s32*)(winPartyDt + 0x00) == currentParty)
            partyOrder[partyCount++] = winPartyDt + 0x00;
        if (*(s32*)(winPartyDt + 0x24) == currentParty)
            partyOrder[partyCount++] = winPartyDt + 0x24;
        if (*(s32*)(winPartyDt + 0x48) == currentParty)
            partyOrder[partyCount++] = winPartyDt + 0x48;
        if (*(s32*)(winPartyDt + 0x6C) == currentParty)
            partyOrder[partyCount++] = winPartyDt + 0x6C;
        if (*(s32*)(winPartyDt + 0x90) == currentParty)
            partyOrder[partyCount++] = winPartyDt + 0x90;
        if (*(s32*)(winPartyDt + 0xB4) == currentParty)
            partyOrder[partyCount++] = winPartyDt + 0xB4;
        if (*(s32*)(winPartyDt + 0xD8) == currentParty)
            partyOrder[partyCount++] = winPartyDt + 0xD8;
        if (*(s32*)(winPartyDt + 0x00) != currentParty)
            partyOrder[partyCount++] = winPartyDt + 0x00;
        if (*(s32*)(winPartyDt + 0x24) != currentParty)
            partyOrder[partyCount++] = winPartyDt + 0x24;
        if (*(s32*)(winPartyDt + 0x48) != currentParty)
            partyOrder[partyCount++] = winPartyDt + 0x48;
        if (*(s32*)(winPartyDt + 0x6C) != currentParty)
            partyOrder[partyCount++] = winPartyDt + 0x6C;
        if (*(s32*)(winPartyDt + 0x90) != currentParty)
            partyOrder[partyCount++] = winPartyDt + 0x90;
        if (*(s32*)(winPartyDt + 0xB4) != currentParty)
            partyOrder[partyCount++] = winPartyDt + 0xB4;
        if (*(s32*)(winPartyDt + 0xD8) != currentParty)
            partyOrder[partyCount++] = winPartyDt + 0xD8;
        scale.x = scale.y = scale.z = 1.0f;
        pos.z = 0.0f;
        for (i = 0; i < count; i++) {
            s32 rowOffset;
            if (i == 0) {
                rowOffset = 0;
            } else if (i == 1) {
                rowOffset = 44;
            } else {
                rowOffset = (i + 1) * 28;
            }
            if (timer != 0 && i != cursor) {
                winIconGrayInit();
            } else {
                winIconInit();
            }
            pos.x = (f32)(x + 30);
            pos.y = (f32)(y - 20 - rowOffset);
            winIconSet(i == 0 ? 0x1A6 : *(s16*)((u8*)partyOrder[i - 1] + 4), &pos, &scale,
                       timer != 0 && i != cursor ? &gray : &white);

            winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
            GXSetTevColorIn(0, 0, 0, 0, 2);
            GXSetTevAlphaIn(0, 0, 4, 5, 7);
            pos.x = (f32)(x + 220);
            pos.y = (f32)(y - 19 - rowOffset);
            winTexSet(0xAF, &pos, &scale, &white);
            pos.y -= 22.0f;
            winTexSet(0xAF, &pos, &scale, &white);

            winFontInit();
            pos.x = (f32)(x + 50);
            pos.y = (f32)(y - 8 - rowOffset);
            if (i == 0) {
                winFontSet(&pos, &scale, &white, msgSearch("name_mario"));
                pos.x = (f32)(x + 110);
                pos.y -= 22.0f;
                winFontSetR(&pos, &scale, &white, "%d/%d", pouchGetHP(), pouchGetMaxHP());
                pos.x += 95.0f;
                winFontSetR(&pos, &scale, &white, "%d/%d", pouchGetFP(), pouchGetMaxFP());
            } else {
                winFontSet(&pos, &scale, &white, msgSearch("msg_menu_party_name"));
            }
        }
    }

    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
    pos.x = *(f32*)(w + 0x150) + 20.0f;
    pos.y = *(f32*)(w + 0x154) - 20.0f;
    pos.z = 0.0f;
    scale.x = scale.y = scale.z = 1.0f;
    winTexSet(0, &pos, &scale, &white);
    pos.x = *(f32*)(w + 0x150);
    pos.y = *(f32*)(w + 0x154);
    winTexSet(0, &pos, &scale, &white);
}

void itemUseDisp2(void* pWinMgr) {
    typedef struct LocalVec {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec;
    extern void* winGetPtr(void);
    extern s32 winMgrAction(s32 entryId);
    extern char* msgSearch(char* msg);
    extern u32 FontGetMessageWidthLine(char* msg, u16* width);
    extern void winMgrSetSize(s32 entryId, s32 x, s32 y, s32 width, s32 height);
    extern void winFontInit(void);
    extern void winFontSetPitch(LocalVec* position, LocalVec* scale, u32* color, char* msg, f32 pitch);
    extern u32 dat_804237b0;
    extern LocalVec vec3_802f5df0;
    extern LocalVec vec3_802f5de4;
    extern char str_msg_window_select_5_802f5dfc[];
    extern f32 float_0p5_804237b4;
    void* win;
    char* msg;
    u16 width[2];
    u32 color;
    LocalVec scale;
    LocalVec position;
    u32 measuredWidth;
    f32 msgWidth;
    f32 maxWidth;

    win = winGetPtr();
    color = dat_804237b0;
    if (winMgrAction(*(s32*)((s32)win + 0x1214)) == 0) {
        msg = msgSearch(str_msg_window_select_5_802f5dfc);
        measuredWidth = FontGetMessageWidthLine(msg, width);
        msgWidth = (f32)(u16)measuredWidth;
        winMgrSetSize(
            *(s32*)((s32)win + 0x1214),
            *(s32*)((s32)pWinMgr + 0x18),
            *(s32*)((s32)pWinMgr + 0x1C),
            *(s32*)((s32)pWinMgr + 0x20),
            *(s32*)(*(s32*)((s32)pWinMgr + 0x28) + 0x18) + width[0] * 0x16
        );
        winFontInit();
        maxWidth = (f32)(*(s32*)((s32)pWinMgr + 0x20) - 0x14);
        if (maxWidth < msgWidth) {
            msgWidth = maxWidth;
        }
        color = dat_804237b0;
        scale.x = vec3_802f5df0.x;
        scale.y = vec3_802f5df0.y;
        scale.z = vec3_802f5df0.z;
        position.x = ((f32)*(s32*)((s32)pWinMgr + 0x20) - msgWidth) * float_0p5_804237b4
            + (f32)*(s32*)((s32)pWinMgr + 0x18);
        position.y = vec3_802f5de4.y;
        position.z = vec3_802f5de4.z;
        winFontSetPitch(&position, &scale, &color, msg, maxWidth);
    }
}

u16 menu_skip_list[18] = {
    9, 10, 11, 6, 7, 8, 5, 3, 2,
    4, 50, 114, 115, 116, 117, 118, 119, 120,
};
