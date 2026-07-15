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


void itemUseDisp(void* pWinMgr) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void* winGetPtr(void);
    extern s32 winMgrAction(s32 id);
    extern s32 pouchGetHP(void);
    extern s32 pouchGetMaxHP(void);
    extern s32 pouchGetFP(void);
    extern s32 pouchGetMaxFP(void);
    extern void winIconInit(void);
    extern void winIconGrayInit(void);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern void winFontInit(void);
    extern void winFontSetR(Vec3* pos, Vec3* scale, void* color, char* format, ...);
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    void* pWin = winGetPtr();
    s32 count = *(s32*)((s32)pWin + 0x3D0);
    s32 cursor = *(s32*)((s32)pWin + 0x3D4);
    s32 i;

    if (winMgrAction(*(s32*)((s32)pWin + 0x1A0)) == 0) return;
    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;
    pos.z = 0.0f;
    for (i = 0; i <= count; i++) {
        pos.x = *(f32*)((s32)pWinMgr + 0x10) + 20.0f;
        pos.y = *(f32*)((s32)pWinMgr + 0x14) - i * 31.0f;
        if (i == cursor) winIconInit();
        else winIconGrayInit();
        winIconSet(i == 0 ? 0x1A6 : 0x120 + i, &pos, &scale, &white);
        winFontInit();
        pos.x += 52.0f;
        if (i == 0) {
            winFontSetR(&pos, &scale, &white, "%d/%d", pouchGetHP(), pouchGetMaxHP());
            pos.x += 90.0f;
            winFontSetR(&pos, &scale, &white, "%d/%d", pouchGetFP(), pouchGetMaxFP());
        }
    }
}

u8 itemUseDisp2(void* pWinMgr) {
    typedef struct LocalVec {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec;
    extern void* winGetPtr(void);
    extern s32 winMgrAction(s32 entryId);
    extern char* msgSearch(char* msg);
    extern void FontGetMessageWidthLine(char* msg, u16* width);
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
    f32 msgWidth;
    f32 maxWidth;

    win = winGetPtr();
    color = dat_804237b0;
    if (winMgrAction(*(s32*)((s32)win + 0x1214)) == 0) {
        msg = msgSearch(str_msg_window_select_5_802f5dfc);
        FontGetMessageWidthLine(msg, width);
        msgWidth = (f32)width[0];
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
    return 0;
}

s32 winItemMain(void* pWin) {
    extern s32 pouchGetHaveItemCnt(void);
    extern s32 pouchHaveItem(s32 index);
    extern void winMsgEntry(void* win, s32 item, char* text, s32 type);
    extern char* msgSearch(char* key);
    extern u8 itemDataTable[];
    extern void psndSFXOn(s32 id);
    s32 state = *(s32*)((s32)pWin + 0x10);
    s32 count;
    s32 cursor = *(s32*)((s32)pWin + 0x1C0);
    s32 item;
    u32 pressed = *(u32*)((s32)pWin + 0x8C);
    u32 repeat = *(u32*)((s32)pWin + 0x90);

    if (state == 0) {
        *(s32*)((s32)pWin + 0x1C0) = 0;
        *(s32*)((s32)pWin + 0x1C4) = 0;
        *(s32*)((s32)pWin + 0x10) = 10;
        return 0;
    }
    if (state == 10) {
        count = pouchGetHaveItemCnt();
        if ((pressed & 0x200) != 0) return -1;
        if ((repeat & 0x1000) != 0 && cursor > 0) cursor--;
        if ((repeat & 0x2000) != 0 && cursor + 1 < count) cursor++;
        if ((repeat & 0x4000) != 0 && cursor > 1) cursor -= 2;
        if ((repeat & 0x8000) != 0 && cursor + 2 < count) cursor += 2;
        *(s32*)((s32)pWin + 0x1C0) = cursor;
        if ((pressed & 0x100) != 0 && count != 0) {
            item = pouchHaveItem(cursor);
            winMsgEntry(pWin, item,
                        msgSearch(*(char**)((s32)itemDataTable + item * 0x28 + 0xC)), 0);
            psndSFXOn(0x20013);
            *(s32*)((s32)pWin + 0x10) = 78;
        }
    } else if (state == 78) {
        if ((pressed & 0x300) != 0) *(s32*)((s32)pWin + 0x10) = 10;
    } else if (state == 100) {
        if ((pressed & 0x200) != 0) return -1;
    }
    return 0;
}

void item_disp(double x, double y, void* pWin) {
    typedef struct Vec3 { f32 x,y,z; } Vec3;
    extern s32 pouchGetHaveItemCnt(void);
    extern s32 pouchHaveItem(s32);
    extern void GXSetScissor(s32,s32,s32,s32);
    extern void winIconInit(void);
    extern void winIconGrayInit(void);
    extern void winIconSet(s32,Vec3*,Vec3*,void*);
    extern void winFontInit(void);
    extern void winFontSet(Vec3*,Vec3*,void*,char*);
    extern char* msgSearch(char*);
    extern u8 itemDataTable[];
    extern u32 dat_80423758, dat_8042375c;
    s32 sub = *(s32*)((s32)pWin + 0x210);
    s32 count = sub ? *(u16*)((s32)pWin + 0x3DA) : pouchGetHaveItemCnt();
    f32 scroll = *(f32*)((s32)pWin + 0x224 + sub * 4);
    s32 i;
    u32 color = dat_80423758;
    Vec3 pos, scale;
    GXSetScissor(0,0x6D,0x260,0xC0);
    scale.x=1.0f; scale.y=1.0f; scale.z=1.0f;
    for (i=0; i<count; i++) {
        s32 item = sub ? *(u16*)((s32)pWin + 0x2E8 + i*2) : pouchHaveItem(i);
        f32 rowY = (f32)y + 100.0f - (f32)(i/2)*38.0f;
        f32 drawY = scroll + 20.0f + (rowY - 10.0f);
        if (drawY > 240.0f) continue;
        if (drawY < -240.0f) break;
        if (sub == 0 && ((*(u16*)(itemDataTable + item*0x28 + 6) & 1) == 0)) winIconGrayInit();
        else winIconInit();
        pos.x = (f32)x + 20.0f + (f32)(i&1)*190.0f;
        pos.y = drawY; pos.z = 0.0f;
        winIconSet(*(s16*)(itemDataTable + item*0x28), &pos, &scale, &color);
        winFontInit();
        pos.x += 28.0f; pos.y -= 4.0f;
        winFontSet(&pos, &scale, &color, msgSearch(*(char**)(itemDataTable + item*0x28 + 0x10)));
    }
    GXSetScissor(0,0,0x280,0x1E0);
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
    extern void winTexInit(void* tpl);
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
            LocalVec pos;
            LocalVec size;
            u32 color;
            LocalVec pos2;
            LocalVec size2;

            winTexInit(*(void**)(*(s32*)(*(s32*)((s32)pWin + 0x28) + 0xA0)));
            color = dat_8042374c;
            size = vec3_802f5acc;
            pos.x = x - float_215_80423840;
            pos.y = float_20_804237e8 + (baseY - (f32)yOffset);
            pos.z = vec3_802f5ac0.z;
            winTexSet(0xB3, &pos, &size, &color);

            winTexInit_x2(*(void**)(*(s32*)(*(s32*)((s32)pWin + 0x28) + 0xA0)));
            color = selectedColor;
            size2 = vec3_802f5ae4;
            pos2.x = selectedX;
            pos2.y = float_20_804237e8 + (selectedY - (f32)yOffset);
            pos2.z = vec3_802f5ad8.z;
            winTexSet_x2(i + 0x18, 0xB3, &pos2, &size2, &color);
        } else {
            LocalVec pos;
            LocalVec size;
            u32 color;

            winTexInit_x2(*(void**)(*(s32*)(*(s32*)((s32)pWin + 0x28) + 0xA0)));
            color = dat_80423750;
            size = vec3_802f5afc;
            pos.x = x - float_215_80423840;
            pos.y = float_20_804237e8 + (baseY - (f32)yOffset);
            pos.z = vec3_802f5af0.z;
            winTexSet_x2(i + 0x18, 0xB3, &pos, &size, &color);
        }
    }

    item_disp((s64)x, (s64)y, pWin);
    return 0;
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
