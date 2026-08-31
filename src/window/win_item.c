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
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

/* Target-named item-use renderer data. */
const Vec3 vec3_802f5bd4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5be0 = { 0.65f, 0.65f, 0.65f };
const Vec3 vec3_802f5bec = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5bf8 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f5c04 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5c10 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f5c1c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5c28 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f5c34 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5c40 = { 0.9f, 0.9f, 0.9f };
const Vec3 vec3_802f5c4c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5c58 = { 0.9f, 0.9f, 0.9f };
const Vec3 vec3_802f5c64 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5c70 = { 0.8f, 0.8f, 0.8f };
const Vec3 vec3_802f5c7c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5c88 = { 0.8f, 0.8f, 0.8f };
const Vec3 vec3_802f5c94 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5ca0 = { 0.9f, 0.9f, 0.9f };
const Vec3 vec3_802f5cac = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5cb8 = { 0.9f, 0.9f, 0.9f };
const Vec3 vec3_802f5cc4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5cd0 = { 0.8f, 0.8f, 0.8f };
const Vec3 vec3_802f5cdc = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5ce8 = { 0.8f, 0.8f, 0.8f };
const Vec3 vec3_802f5cf4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5d00 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f5d0c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5d18 = { 0.65f, 0.65f, 0.65f };
const Vec3 vec3_802f5d24 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5d30 = { 0.0f, 1.0f, 1.0f };
const Vec3 vec3_802f5d3c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5d48 = { 0.9f, 0.9f, 0.9f };
const Vec3 vec3_802f5d54 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5d60 = { 0.9f, 0.9f, 0.9f };
const Vec3 vec3_802f5d6c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5d78 = { 0.8f, 0.8f, 0.8f };
const Vec3 vec3_802f5d84 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5d90 = { 0.8f, 0.8f, 0.8f };
const Vec3 vec3_802f5d9c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5da8 = { 0.15f, 0.15f, 0.15f };
const Vec3 vec3_802f5db4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5dc0 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f5dcc = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f5dd8 = { 1.0f, 1.0f, 1.0f };

const u32 dat_8042378c[1] = { 0x000000FF };
const u32 dat_80423790[1] = { 0xFFFFFFFF };
const u32 dat_80423794[1] = { 0x000000FF };
const u32 dat_80423798[1] = { 0x808080FF };
const u32 dat_8042379c[1] = { 0xCB8CA2FE };
const u32 dat_804237a0[1] = { 0xCBBE89FE };
const u32 dat_804237a4[1] = { 0x000000FF };
const u32 dat_804237a8[1] = { 0xCB8CA2F4 };
const u32 dat_804237ac[1] = { 0x00000080 };
const u32 dat_804237b0[1] = { 0x000000FF };
const f32 float_0p5_804237b4[1] = { 0.5f };
const f32 float_0_804237b8[1] = { 0.0f };
const f32 float_neg16_804237bc[1] = { -16.0f };
const f32 float_neg28_804237c0[1] = { -28.0f };
const f32 float_0p9_804237c4[1] = { 0.9f };
const char str_PCTd_804237c8[3] = "%d";
const char str__804237cc[2] = "/";
const f32 float_14_804237d4[1] = { 14.0f };
const f32 float_1_804237d8[1] = { 1.0f };
const f32 float_160_804237dc[1] = { 160.0f };
const f32 float_2_804237e0[1] = { 2.0f };

const char str_name_mario_802f5e20[11] = "name_mario";

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
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

u8 winItemInit(void* pWin) {
    extern const f32 float_0_804237b8[1];
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
    *(f32*)((s32)pWin + 0x264) = float_0_804237b8[0];
    *(f32*)((s32)pWin + 0x260) = float_0_804237b8[0];

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

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

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
    extern s32 winSortWait(void* win);
    extern void psndSFXOn(s32 id);
    extern u8 itemDataTable[];
    extern char str_msg_luigi_book1_txt1_802f5520[];
    extern void* luigi_book_list[5];
    extern void* winMgrGetPtr(s32 id);
    extern void winMgrSetSize(s32 id, s32 x, s32 y, s32 width, s32 height);
    extern void winMgrOpen(s32 id);
    extern void winMgrClose(s32 id);
    extern s32 pouchReceiveMailCount(void);
    extern s32 pouchCheckMail(s32 id);
    extern void pouchOpenMail(s32 id);
    extern void swSet(s32 flag);
    extern void* mailGetPtr(s32 id);
    extern char* msgSearch(char* key);
    extern u32 FontGetMessageWidthLine(char* msg, u16* width);
    extern s32 getMarioStDvdRoot(void);
    extern u32 fileAsyncf(s32, s32, char*, s32, char*);
    extern void* fileAllocf(s32 id, const char* fmt, ...);
    extern void fileFree(void* file);
    extern u32 keyGetDirRep(s32 controller);
    extern void* gp;
    extern char str_jp_80423870[];
    extern char str_us_80423874[];
    extern void* marioGetPtr(void);
    extern s32 unk_JP_US_EU_05_800a1748(void);
    extern s32 U_marioSlitContinueChk(void);
    extern s32 marioBgmodeChk(void);
    extern s32 marioGetParty(void);
    extern u8 winPartyDt[];
    extern s32 pouchGetHP(void);
    extern void pouchSetHP(u16 hp);
    extern s32 pouchGetFP(void);
    extern void pouchSetFP(u16 fp);
    extern s32 pouchGetAP(void);
    extern void pouchSetAP(s16 ap);
    extern s32 pouchGetPartyHP(s32 party);
    extern void pouchSetPartyHP(s32 party, u16 hp);
    extern s32 pouchRemoveItemIndex(s32 item, s32 index);

    extern const f32 float_160_804237dc[1];
    extern f32 float_neg144_80423860;
    extern f32 float_180_80423864;
    extern f32 float_320_8042382c;
    extern f32 float_neg240_804237f4;
    extern f32 float_60_80423868;
    extern f32 float_neg96_8042386c;
    extern f32 float_120_80423834;
    extern f32 float_neg80_80423878;
    extern const f32 float_0_804237b8[1];
    extern const f32 float_neg16_804237bc[1];
    extern const f32 float_neg28_804237c0[1];
    extern f32 float_28_80423880;
    extern f32 float_130_8042387c;

    typedef union {
        f64 d;
        struct {
            u32 hi;
            u32 lo;
        } w;
    } IntDouble;

    u8* rodata = (u8*)str_msg_luigi_book1_txt1_802f5520;
    s32 state = *(s32*)((u8*)pWin + 0x20C);
    s32 sub;
    s32 count;
    s32 pages;
    s32 cursor;
    s32 page;
    s32 item;
    s32 old;
    s32 i;
    s32 party;
    s32 invalid;
    s32 id;
    s32 lines;
    u32 tmp;
    u8* mail;
    char* lang;
    char* msg;
    void* mgr;
    IntDouble cvt1;
    IntDouble cvt2;
    void* partyOrder[8];
    f32 adjust;


    switch (state) {
    case 0: {
        if ((*(u32*)((u8*)pWin + 4)) & 0x200) {
            psndSFXOn(0x20013);
            return -1;
        }
        if ((*(u32*)((u8*)pWin + 4)) & 0x1000) {
            return -2;
        }
        if ((*(u32*)((u8*)pWin + 0x10)) & 0x3000) {
            *(s32*)((u8*)pWin + 0x210) = 1 - *(s32*)((u8*)pWin + 0x210);
            psndSFXOn(0x20005);
        } else if ((*(u32*)((u8*)pWin + 4)) & 0x100) {
            if (*(s32*)((u8*)pWin + 0x210) == 0) {
                if (pouchGetHaveItemCnt() == 0)
                    return 0;
            } else if (*(s16*)((u8*)pWin + 0x3DA) == 0) {
                return 0;
            }
            *(s32*)((u8*)pWin + 0x20C) = 10;
            psndSFXOn(0x20012);
            return 0;
        } else if ((*(u32*)((u8*)pWin + 4)) & 0x10) {
            sub = *(s32*)((u8*)pWin + 0x210);
            winSortEntry(-310.0f, 150.0f, pWin, sub != 0);
            *(s32*)((u8*)pWin + 0x124) = *(s32*)((u8*)pWin + 0x20C);
            psndSFXOn(0x20012);
            *(s32*)((u8*)pWin + 0x20C) = 1000;
        }

        cvt1.w.hi = 0x43300000;
        cvt1.w.lo = (u32)(125 - *(s32*)((u8*)pWin + 0x210) * 50) ^ 0x80000000U;
        *(f32*)((u8*)pWin + 0x158) = -275.0f;
        *(f32*)((u8*)pWin + 0x15C) =
            (f32)(cvt1.d - *(f64*)(rodata + 0x8F8));
        msg = (char*)(rodata + 0x938);
        if (*(s32*)((u8*)pWin + 0x210) == 0)
            msg = (char*)(rodata + 0x924);
        winMsgEntry(pWin, 0, msg, 0);
        return 0;
    }

    case 10: {
        if (*(s32*)((u8*)pWin + 0x210) == 0) {
            count = pouchGetHaveItemCnt();
        } else {
            count = *(s16*)((u8*)pWin + 0x3DA);
        }

        sub = *(s32*)((u8*)pWin + 0x210);
        pages = (count + 9) / 10;

        if ((*(u32*)((u8*)pWin + 4)) & 0x200) {
            psndSFXOn(0x20013);
            *(s32*)((u8*)pWin + 0x20C) = 0;
        }

        tmp = *(u32*)((u8*)pWin + 4);
        if (tmp & 0x100) {
            if (*(s32*)((u8*)pWin + 0x210) == 0) {
                cursor = *(s32*)((u8*)pWin + 0x214);
                item = pouchHaveItem(cursor);
                if ((*(u16*)(itemDataTable + item * 0x28 + 0x10) & 4) != 0) {
                    *(s32*)((u8*)pWin + 0x2D4) = item;
                    *(s32*)((u8*)pWin + 0x2D8) = cursor;
                    *(s32*)((u8*)pWin + 0x20C) = 300;
                    *(s32*)((u8*)pWin + 0x2DC) = 0;
                    *(s32*)((u8*)pWin + 0x2E0) = 0;
                    psndSFXOn(0x20012);

                    mgr = winMgrGetPtr(*(s32*)((u8*)pWin + 0x1210));
                    winMgrSetSize(*(s32*)((u8*)pWin + 0x1210),
                                  *(s32*)((u8*)mgr + 0x18),
                                  *(s32*)((u8*)mgr + 0x1C),
                                  *(s32*)((u8*)mgr + 0x20),
                                  *(s32*)((u8*)pWin + 0x1E0) * 0x1C + 0x44);
                    winMgrOpen(*(s32*)((u8*)pWin + 0x1210));
                    winMgrOpen(*(s32*)((u8*)pWin + 0x1214));
                }
            } else {
                item = *(s16*)((u8*)pWin + 0x2E8 +
                               *(s32*)((u8*)pWin + 0x218) * 2);

                if ((u32)(item - 0x37) < 4 || item == 0x3B) {
                    *(s32*)((u8*)pWin + 0x20C) = 100;
                    *(f32*)((u8*)pWin + 0x238) = -170.0f;
                    *(f32*)((u8*)pWin + 0x240) = float_160_804237dc[0];
                    *(u16*)pWin |= 0x8000;

                    msg = (char*)luigi_book_list[item - 0x37];
                    *(s32*)((u8*)pWin + 0x2C8) = item - 0x37;
                    *(s32*)((u8*)pWin + 0x2D0) = 0;
                    *(s32*)((u8*)pWin + 0x2CC) = 0;
                    while (((void**)msg)[*(s32*)((u8*)pWin + 0x2CC)] != 0) {
                        *(s32*)((u8*)pWin + 0x2CC) += 1;
                    }
                    *(s32*)((u8*)pWin + 0x2CC) =
                        (*(s32*)((u8*)pWin + 0x2CC) + 1) / 2;
                    psndSFXOn(0x20012);
                } else if (item == 0x36 &&
                           pouchReceiveMailCount() != 0) {
                    *(s32*)((u8*)pWin + 0x20C) = 200;
                    *(f32*)((u8*)pWin + 0x248) = float_neg144_80423860;
                    *(f32*)((u8*)pWin + 0x250) = float_180_80423864;
                    psndSFXOn(0x20012);
                } else if (item == 0x4E) {
                    *(s32*)((u8*)pWin + 0x2D4) = 0x4E;
                    psndSFXOn(0x20012);
                    return -2;
                }
            }
        } else if ((*(u32*)((u8*)pWin + 8)) & 0x40) {
            *(s32*)((u8*)pWin + 0x21C + sub * 4) =
                *(s32*)((u8*)pWin + 0x21C + sub * 4) - 1;
            if (*(s32*)((u8*)pWin + 0x21C + sub * 4) < 0) {
                *(s32*)((u8*)pWin + 0x21C + sub * 4) = 0;
            }
            *(s32*)((u8*)pWin + 0x214 + sub * 4) =
                *(s32*)((u8*)pWin + 0x21C + sub * 4) * 10;
            psndSFXOn(0x20035);
        } else if ((*(u32*)((u8*)pWin + 8)) & 0x20) {
            *(s32*)((u8*)pWin + 0x21C + sub * 4) =
                *(s32*)((u8*)pWin + 0x21C + sub * 4) + 1;
            if (pages <= *(s32*)((u8*)pWin + 0x21C + sub * 4)) {
                *(s32*)((u8*)pWin + 0x21C + sub * 4) = pages - 1;
            }
            *(s32*)((u8*)pWin + 0x214 + sub * 4) =
                *(s32*)((u8*)pWin + 0x21C + sub * 4) * 10;
            psndSFXOn(0x20035);
        } else {
            old = *(s32*)((u8*)pWin + 0x10);
            if (old & 0xC000) {
                tmp = (u32)*(s32*)((u8*)pWin + 0x214 + sub * 4);
                if (((tmp & 1) ^ (tmp >> 31)) == (tmp >> 31)) {
                    if ((s32)(tmp + 1) > -1 &&
                        (s32)(tmp + 1) < count) {
                        *(s32*)((u8*)pWin + 0x214 + sub * 4) =
                            (s32)tmp + 1;
                    }
                } else if ((s32)(tmp - 1) > -1 &&
                           (s32)(tmp - 1) < count) {
                    *(s32*)((u8*)pWin + 0x214 + sub * 4) =
                        (s32)tmp - 1;
                }
                psndSFXOn(0x20035);
            } else if (old & 0x1000) {
                cursor = *(s32*)((u8*)pWin + 0x214 + sub * 4) - 2;
                if (cursor > -1 && cursor < count) {
                    *(s32*)((u8*)pWin + 0x214 + sub * 4) =
                        *(s32*)((u8*)pWin + 0x214 + sub * 4) - 2;
                }

                cursor = *(s32*)((u8*)pWin + 0x214 + sub * 4);
                *(s32*)((u8*)pWin + 0x21C + sub * 4) = cursor / 10;
                psndSFXOn(0x20035);
            } else if (old & 0x2000) {
                cursor = *(s32*)((u8*)pWin + 0x214 + sub * 4);
                if ((cursor + 2 < 0) || (count <= cursor + 2)) {
                    if (cursor + (cursor & 1) < count) {
                        *(s32*)((u8*)pWin + 0x214 + sub * 4) = count - 1;
                    }
                } else {
                    *(s32*)((u8*)pWin + 0x214 + sub * 4) = cursor + 2;
                }

                cursor = *(s32*)((u8*)pWin + 0x214 + sub * 4);
                *(s32*)((u8*)pWin + 0x21C + sub * 4) = cursor / 10;
                psndSFXOn(0x20035);
            } else if (tmp & 0x10) {
                winSortEntry(-310.0f, 150.0f,
                             pWin,
                             *(s32*)((u8*)pWin + 0x210) != 0);
                *(s32*)((u8*)pWin + 0x124) = *(s32*)((u8*)pWin + 0x20C);
                psndSFXOn(0x20012);
                *(s32*)((u8*)pWin + 0x20C) = 1000;
            } else if (tmp & 0x1000) {
                return -2;
            }
        }

        tmp = (u32)*(s32*)((u8*)pWin + 0x214 + sub * 4);
        old = (s32)tmp / 10 + ((s32)tmp >> 31);
        cursor = (s32)(tmp + (old - (old >> 31)) * -10);
        pages = cursor >> 31;

        cvt1.w.hi = 0x43300000;
        cvt2.w.hi = 0x43300000;
        cvt1.w.lo =
            (u32)(((cursor & 1) ^ -pages) + pages) * 0xCC -
            0x9B ^ 0x80000000U;

        tmp = (u32)*(s32*)((u8*)pWin + 0x214 + sub * 4);
        old = (s32)tmp / 10 + ((s32)tmp >> 31);
        cursor = (s32)(tmp + (old - (old >> 31)) * -10);
        cvt2.w.lo =
            (u32)((cursor / 2) * -0x26 + 0x6E) ^
            0x80000000U;

        *(f32*)((u8*)pWin + 0x158) =
            (f32)(cvt1.d - *(f64*)(rodata + 0x8F8));
        *(f32*)((u8*)pWin + 0x15C) =
            (f32)(cvt2.d - *(f64*)(rodata + 0x8F8));

        if (*(s32*)((u8*)pWin + 0x210) == 0) {
            item = pouchHaveItem(
                *(s32*)((u8*)pWin + 0x214));
        } else {
            item = *(s16*)((u8*)pWin + 0x2E8 +
                           *(s32*)((u8*)pWin + 0x218) * 2);
        }

        winMsgEntry(pWin, item,
                    *(char**)(itemDataTable + item * 0x28 + 0x0C), 0);
        return 0;
    }

    case 100: {
        tmp = *(u32*)((u8*)pWin + 4);

        if (tmp & 0x100) {
            psndSFXOn(0x20012);
            *(s32*)((u8*)pWin + 0x2D0) += 1;

            if (*(s32*)((u8*)pWin + 0x2D0) >=
                *(s32*)((u8*)pWin + 0x2CC)) {
                *(s32*)((u8*)pWin + 0x2D0) =
                    *(s32*)((u8*)pWin + 0x2CC) - 1;
                *(s32*)((u8*)pWin + 0x20C) = 10;
                *(f32*)((u8*)pWin + 0x238) = float_320_8042382c;
                *(f32*)((u8*)pWin + 0x240) = float_neg240_804237f4;
                *(u16*)pWin &= 0x7FFF;
                *(f32*)((u8*)pWin + 0x150) =
                    *(f32*)((u8*)pWin + 0x158);
                *(f32*)((u8*)pWin + 0x154) =
                    *(f32*)((u8*)pWin + 0x15C);
            }
        } else if (tmp & 0x200) {
            psndSFXOn(0x20013);
            *(s32*)((u8*)pWin + 0x2D0) -= 1;

            if (*(s32*)((u8*)pWin + 0x2D0) < 0) {
                *(s32*)((u8*)pWin + 0x2D0) = 0;
                *(s32*)((u8*)pWin + 0x20C) = 10;
                *(f32*)((u8*)pWin + 0x238) = float_320_8042382c;
                *(f32*)((u8*)pWin + 0x240) = float_neg240_804237f4;
                *(u16*)pWin &= 0x7FFF;
                *(f32*)((u8*)pWin + 0x150) =
                    *(f32*)((u8*)pWin + 0x158);
                *(f32*)((u8*)pWin + 0x154) =
                    *(f32*)((u8*)pWin + 0x15C);
            }
        } else if (tmp & 0x1000) {
            *(f32*)((u8*)pWin + 0x238) = float_320_8042382c;
            *(f32*)((u8*)pWin + 0x240) = float_neg240_804237f4;
            *(u16*)pWin &= 0x7FFF;
            *(f32*)((u8*)pWin + 0x150) =
                *(f32*)((u8*)pWin + 0x158);
            *(f32*)((u8*)pWin + 0x154) =
                *(f32*)((u8*)pWin + 0x15C);
            return -2;
        }

        *(f32*)((u8*)pWin + 0x158) = -170.0f;
        *(f32*)((u8*)pWin + 0x15C) = 150.0f;
        return 0;
    }

    case 200: {
        if ((*(u32*)((u8*)pWin + 4)) & 0x100) {
            id = *(u8*)((u8*)pWin + 0x26F + *(s32*)((u8*)pWin + 0x258) * 2);
            if (pouchCheckMail(id) == 1)
                pouchOpenMail(id);
            swSet(0x26);
            psndSFXOn(0x20036);
            *(s32*)((u8*)pWin + 0x20C) = 201;
            mail = (u8*)mailGetPtr(id);
            msg = msgSearch(*(char**)(mail + 0x0C));
            FontGetMessageWidthLine(msg, (u16*)((u8*)pWin + 0x26C));
            *(u16*)((u8*)pWin + 0x26C) += 1;
            *(s32*)((u8*)pWin + 0x268) = 0;
            if ((*(u16*)mail & 0x1000) != 0)
                *(u16*)((u8*)pWin + 0x26C) += 10;
        } else if ((*(u32*)((u8*)pWin + 4)) & 0x200) {
            psndSFXOn(0x20013);
            *(s32*)((u8*)pWin + 0x20C) = 10;
            *(f32*)((u8*)pWin + 0x248) = float_320_8042382c;
            *(f32*)((u8*)pWin + 0x250) = float_neg240_804237f4;
        } else if ((*(u32*)((u8*)pWin + 0x10)) & 0x1000) {
            old = *(s32*)((u8*)pWin + 0x258);
            *(s32*)((u8*)pWin + 0x258) = old - 1;
            if (*(s32*)((u8*)pWin + 0x258) < 0)
                *(s32*)((u8*)pWin + 0x258) = 0;
            *(s32*)((u8*)pWin + 0x25C) = *(s32*)((u8*)pWin + 0x258) / 5;
            *(s32*)((u8*)pWin + 0x268) = 0;
            if (old != *(s32*)((u8*)pWin + 0x258))
                psndSFXOn(0x20036);
        } else if ((*(u32*)((u8*)pWin + 0x10)) & 0x2000) {
            old = *(s32*)((u8*)pWin + 0x258);
            *(s32*)((u8*)pWin + 0x258) = old + 1;
            if (*(s32*)((u8*)pWin + 0x258) >= *(s32*)((u8*)pWin + 0x254))
                *(s32*)((u8*)pWin + 0x258) = *(s32*)((u8*)pWin + 0x254) - 1;
            *(s32*)((u8*)pWin + 0x25C) = *(s32*)((u8*)pWin + 0x258) / 5;
            *(s32*)((u8*)pWin + 0x268) = 0;
            if (old != *(s32*)((u8*)pWin + 0x258))
                psndSFXOn(0x20036);
        } else if ((*(u32*)((u8*)pWin + 8)) & 0x40) {
            old = *(s32*)((u8*)pWin + 0x25C);
            *(s32*)((u8*)pWin + 0x25C) = old - 1;
            if (*(s32*)((u8*)pWin + 0x25C) < 0)
                *(s32*)((u8*)pWin + 0x25C) = 0;
            *(s32*)((u8*)pWin + 0x258) = *(s32*)((u8*)pWin + 0x25C) * 5;
            if (old != *(s32*)((u8*)pWin + 0x25C))
                psndSFXOn(0x20036);
        } else if ((*(u32*)((u8*)pWin + 8)) & 0x20) {
            old = *(s32*)((u8*)pWin + 0x25C);
            pages = (*(s32*)((u8*)pWin + 0x254) + 4) / 5;
            *(s32*)((u8*)pWin + 0x25C) = old + 1;
            if (*(s32*)((u8*)pWin + 0x25C) >= pages)
                *(s32*)((u8*)pWin + 0x25C) = pages - 1;
            *(s32*)((u8*)pWin + 0x258) = *(s32*)((u8*)pWin + 0x25C) * 5;
            if (old != *(s32*)((u8*)pWin + 0x25C))
                psndSFXOn(0x20036);
        } else if ((*(u32*)((u8*)pWin + 4)) & 0x10) {
            winSortEntry(*(f32*)((u8*)pWin + 0x248),
                         *(f32*)((u8*)pWin + 0x250) - float_60_80423868,
                         pWin, 7);
            *(s32*)((u8*)pWin + 0x124) = *(s32*)((u8*)pWin + 0x20C);
            psndSFXOn(0x20012);
            *(s32*)((u8*)pWin + 0x20C) = 1000;
        } else if ((*(u32*)((u8*)pWin + 4)) & 0x1000) {
            *(f32*)((u8*)pWin + 0x248) = float_320_8042382c;
            *(f32*)((u8*)pWin + 0x250) = float_neg240_804237f4;
            return -2;
        }

        cursor = *(s32*)((u8*)pWin + 0x258);
        cvt1.w.hi = 0x43300000;
        cvt1.w.lo = (u32)((cursor % 5) * 40) ^ 0x80000000U;
        *(f32*)((u8*)pWin + 0x158) = float_neg96_8042386c;
        *(f32*)((u8*)pWin + 0x15C) =
            float_120_80423834 -
            (f32)(cvt1.d - *(f64*)(rodata + 0x8F8));
        return 0;
    }

    case 201: {
        id = *(u8*)((u8*)pWin + 0x26F + *(s32*)((u8*)pWin + 0x258) * 2);
        mail = (u8*)mailGetPtr(id);
        if ((*(u16*)mail & 0x1000) == 0) {
            *(void**)((u8*)pWin + 0x2C) = 0;
            *(s32*)((u8*)pWin + 0x20C) += 1;
        } else {
            lang = str_us_80423874;
            if (*(u32*)((u8*)gp + 0x16C) == 0)
                lang = str_jp_80423870;
            *(void**)((u8*)pWin + 0x2C) =
                (void*)fileAsyncf(4, 0, (char*)(rodata + 0x950),
                                  getMarioStDvdRoot(), lang);
            if (*(void**)((u8*)pWin + 0x2C) != 0) {
                lang = str_us_80423874;
                if (*(u32*)((u8*)gp + 0x16C) == 0)
                    lang = str_jp_80423870;
                *(void**)((u8*)pWin + 0x2C) =
                    fileAllocf(4, (char*)(rodata + 0x950),
                               getMarioStDvdRoot(), lang);
                *(s32*)((u8*)pWin + 0x20C) += 1;
            }
        }
        return 0;
    }

    case 202: {
        if (*(u16*)((u8*)pWin + 0x26C) > 8) {
            tmp = keyGetDirRep(0);
            if (tmp & 0x100) {
                if (*(s32*)((u8*)pWin + 0x268) > 0)
                    *(s32*)((u8*)pWin + 0x268) -= 1;
            } else {
                tmp = keyGetDirRep(0);
                if ((tmp & 0x200) &&
                    *(s32*)((u8*)pWin + 0x268) + 6 < (s32)*(u16*)((u8*)pWin + 0x26C))
                    *(s32*)((u8*)pWin + 0x268) += 1;
            }
        }
        if ((*(u32*)((u8*)pWin + 4)) & 0x200) {
            psndSFXOn(0x20036);
            if (*(void**)((u8*)pWin + 0x2C) != 0)
                fileFree(*(void**)((u8*)pWin + 0x2C));
            *(s32*)((u8*)pWin + 0x20C) = 200;
        } else if ((*(u32*)((u8*)pWin + 4)) & 0x1000) {
            if (*(void**)((u8*)pWin + 0x2C) != 0)
                fileFree(*(void**)((u8*)pWin + 0x2C));
            *(f32*)((u8*)pWin + 0x248) = float_320_8042382c;
            *(f32*)((u8*)pWin + 0x250) = float_neg240_804237f4;
            return -2;
        }
        *(f32*)((u8*)pWin + 0x158) = float_neg96_8042386c;
        *(f32*)((u8*)pWin + 0x15C) = float_120_80423834;
        return 0;
    }

    case 300: {
        if ((*(u32*)((u8*)pWin + 4)) & 0x200) {
            psndSFXOn(0x20013);
            *(s32*)((u8*)pWin + 0x20C) = 10;
            *(s32*)((u8*)pWin + 0x2D4) = 0;
            winMgrClose(*(s32*)((u8*)pWin + 0x1210));
            winMgrClose(*(s32*)((u8*)pWin + 0x1214));
        } else if ((*(u32*)((u8*)pWin + 4)) & 0x100) {
            void* player = marioGetPtr();
            invalid = 0;
            psndSFXOn(0x20012);
            if (*(u16*)((u8*)player + 0x2E) == 0x19 ||
                *(u16*)((u8*)player + 0x2E) == 0x18)
                invalid = 1;
            if (unk_JP_US_EU_05_800a1748() != 0 ||
                U_marioSlitContinueChk() != 0)
                invalid = 1;
            if (*(s32*)((u8*)pWin + 0x2DC) > 0 && marioBgmodeChk() != 0)
                invalid = 1;
            if (*(s32*)((u8*)pWin + 0x2DC) > 1 && marioBgmodeChk() == 0)
                invalid = 1;

            if (invalid) {
                if (marioBgmodeChk() == 0)
                    party = marioGetParty();
                else
                    party = *(s8*)((u8*)marioGetPtr() + 0x247);

                count = 0;
                if (*(s32*)(winPartyDt + 0x00) == party)
                    partyOrder[count++] = winPartyDt + 0x00;
                if (*(s32*)(winPartyDt + 0x24) == party)
                    partyOrder[count++] = winPartyDt + 0x24;
                if (*(s32*)(winPartyDt + 0x48) == party)
                    partyOrder[count++] = winPartyDt + 0x48;
                if (*(s32*)(winPartyDt + 0x6C) == party)
                    partyOrder[count++] = winPartyDt + 0x6C;
                if (*(s32*)(winPartyDt + 0x90) == party)
                    partyOrder[count++] = winPartyDt + 0x90;
                if (*(s32*)(winPartyDt + 0xB4) == party)
                    partyOrder[count++] = winPartyDt + 0xB4;
                if (*(s32*)(winPartyDt + 0xD8) == party)
                    partyOrder[count++] = winPartyDt + 0xD8;

                if (*(s32*)(winPartyDt + 0x00) != party)
                    partyOrder[count++] = winPartyDt + 0x00;
                if (*(s32*)(winPartyDt + 0x24) != party)
                    partyOrder[count++] = winPartyDt + 0x24;
                if (*(s32*)(winPartyDt + 0x48) != party)
                    partyOrder[count++] = winPartyDt + 0x48;
                if (*(s32*)(winPartyDt + 0x6C) != party)
                    partyOrder[count++] = winPartyDt + 0x6C;
                if (*(s32*)(winPartyDt + 0x90) != party)
                    partyOrder[count++] = winPartyDt + 0x90;
                if (*(s32*)(winPartyDt + 0xB4) != party)
                    partyOrder[count++] = winPartyDt + 0xB4;
                if (*(s32*)(winPartyDt + 0xD8) != party)
                    partyOrder[count++] = winPartyDt + 0xD8;

                item = *(s32*)((u8*)pWin + 0x2D4);
                if (*(s32*)((u8*)pWin + 0x2DC) < 1) {
                    pouchSetHP((u16)(pouchGetHP() +
                               *(u8*)(itemDataTable + item * 0x28 + 0x1D)));
                    pouchSetFP((u16)(pouchGetFP() +
                               *(u8*)(itemDataTable + item * 0x28 + 0x1E)));
                    pouchSetAP((s16)(pouchGetAP() +
                               *(u8*)(itemDataTable + item * 0x28 + 0x1F)));
                } else {
                    id = *(s32*)partyOrder[
                        *(s32*)((u8*)pWin + 0x2DC) - 1];
                    pouchSetPartyHP(id, (u16)(pouchGetPartyHP(id) +
                               *(u8*)(itemDataTable + item * 0x28 + 0x1D)));
                    pouchSetFP((u16)(pouchGetFP() +
                               *(u8*)(itemDataTable + item * 0x28 + 0x1E)));
                    pouchSetAP((s16)(pouchGetAP() +
                               *(u8*)(itemDataTable + item * 0x28 + 0x1F)));
                }
                *(s32*)((u8*)pWin + 0x2E0) = 0;
                *(s32*)((u8*)pWin + 0x20C) = 301;
                return 0;
            }

            winMgrClose(*(s32*)((u8*)pWin + 0x1210));
            winMgrClose(*(s32*)((u8*)pWin + 0x1214));
            return -2;
        } else if ((*(u32*)((u8*)pWin + 0x10)) & 0x1000) {
            old = *(s32*)((u8*)pWin + 0x2DC);
            *(s32*)((u8*)pWin + 0x2DC) = old - 1;
            if (*(s32*)((u8*)pWin + 0x2DC) < 0)
                *(s32*)((u8*)pWin + 0x2DC) = *(s32*)((u8*)pWin + 0x1E0);
            if (old != *(s32*)((u8*)pWin + 0x2DC))
                psndSFXOn(0x20005);
        } else if ((*(u32*)((u8*)pWin + 0x10)) & 0x2000) {
            old = *(s32*)((u8*)pWin + 0x2DC);
            *(s32*)((u8*)pWin + 0x2DC) = old + 1;
            if (*(s32*)((u8*)pWin + 0x2DC) >= *(s32*)((u8*)pWin + 0x1E0) + 1)
                *(s32*)((u8*)pWin + 0x2DC) = 0;
            if (old != *(s32*)((u8*)pWin + 0x2DC))
                psndSFXOn(0x20005);
        } else if ((*(u32*)((u8*)pWin + 4)) & 0x1000) {
            *(s32*)((u8*)pWin + 0x2D4) = 0;
            winMgrClose(*(s32*)((u8*)pWin + 0x1210));
            winMgrClose(*(s32*)((u8*)pWin + 0x1214));
            return -2;
        }

        *(f32*)((u8*)pWin + 0x158) = float_neg80_80423878;
        adjust = float_0_804237b8[0];
        cursor = *(s32*)((u8*)pWin + 0x2DC);
        if (cursor != 0) {
            adjust = float_neg28_804237c0[0];
            if (cursor == 1)
                adjust = float_neg16_804237bc[0];
        }
        cvt1.w.hi = 0x43300000;
        cvt1.w.lo = (u32)cursor ^ 0x80000000U;
        *(f32*)((u8*)pWin + 0x15C) =
            -(float_28_80423880 *
              (f32)(cvt1.d - *(f64*)(rodata + 0x8F8)) -
              float_130_8042387c) + adjust;
        return 0;
    }

    case 301: {
        *(s32*)((u8*)pWin + 0x2E0) += 1;
        if (*(s32*)((u8*)pWin + 0x2E0) > (*(s32*)((u8*)gp + 4) << 1)) {
            *(s32*)((u8*)pWin + 0x2E0) = 0;
            pouchRemoveItemIndex(*(s32*)((u8*)pWin + 0x2D4), *(s32*)((u8*)pWin + 0x2D8));
            *(s32*)((u8*)pWin + 0x20C) = 10;
            *(s32*)((u8*)pWin + 0x2D4) = 0;
            if (pouchGetHaveItemCnt() == 0) {
                *(s32*)((u8*)pWin + 0x20C) = 0;
            } else if (*(s32*)((u8*)pWin + 0x214) >= pouchGetHaveItemCnt()) {
                *(s32*)((u8*)pWin + 0x214) = pouchGetHaveItemCnt() - 1;
            }
            winMgrClose(*(s32*)((u8*)pWin + 0x1210));
            winMgrClose(*(s32*)((u8*)pWin + 0x1214));
        }
        return 0;
    }

    case 1000: {
        if (winSortWait(pWin) == 0)
            *(s32*)((u8*)pWin + 0x20C) = *(s32*)((u8*)pWin + 0x124);
        if (*(u32*)((u8*)pWin + 4) & 0x1000)
            return -2;
        break;
    }
    default:
        break;
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
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

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
    extern s32 pouchGetHP(void);
    extern s32 pouchGetMaxHP(void);
    extern s32 pouchGetFP(void);
    extern s32 pouchGetMaxFP(void);
    extern char* pouchGetYoshiName(void);
    extern void* pouchGetPtr(void);
    extern u16 FontGetMessageWidth(const char*);
    extern void winTexSet(s32, Vec3*, Vec3*, void*);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void winIconSet(s32, Vec3*, Vec3*, void*);
    extern void winFontSet(Vec3*, Vec3*, void*, const char*, ...);
    extern void winFontSetR(Vec3*, Vec3*, void*, const char*, ...);
    extern s32 marioBgmodeChk(void);
    extern s32 marioGetParty(void);
    extern void* marioGetPtr(void);
    extern u8 winPartyDt[];
    extern u8 itemDataTable[];
    extern void* winGetPtr(void);

    extern char str_slash_jp_804237d0[3];
    extern char* lbl_803776B4[];
    extern void* gp;


    u8* w;
    s32 currentParty;
    s32 partyCount;
    u8* partyOrder[11];
    s32 row22;
    s32 row28;
    s32 i;
    s32 partnerId;
    s32 j;
    f32 rowAdjust;
    f32 nameScale;
    const char* text;
    u32 currentColor;
    u32 baseWhite;
    u32 tailWhite;
    u32 marioIconColor;
    u32 marioHpTexColor;
    u32 marioFpTexColor;
    u32 marioNameColor;
    u32 marioHpLabelColor;
    u32 marioHpCurrentColor;
    u32 marioHpSlashColor;
    u32 marioHpMaxColor;
    u32 marioFpLabelColor;
    u32 marioFpCurrentColor;
    u32 marioFpSlashColor;
    u32 marioFpMaxColor;
    u32 partnerTexColor;
    u32 partnerIconColor;
    u32 partnerNameColor;
    u32 partnerHpLabelColor;
    u32 partnerHpCurrentColor;
    u32 partnerHpSlashColor;
    u32 partnerHpMaxColor;
    u32 markerColor;
    u32 cursorColorA;
    u32 cursorColorB;

    Vec3 marioIconScale;
    Vec3 marioIconPos;
    Vec3 marioIconPosWork;
    Vec3 marioHpTexScale;
    Vec3 marioHpTexPos;
    Vec3 marioHpTexPosWork;
    Vec3 marioFpTexScale;
    Vec3 marioFpTexPos;
    Vec3 marioFpTexPosWork;
    Vec3 marioNameScale;
    Vec3 marioNamePos;
    Vec3 marioNamePosWork;
    Vec3 marioHpLabelScale;
    Vec3 marioHpLabelPos;
    Vec3 marioHpLabelPosWork;
    Vec3 marioHpCurrentScale;
    Vec3 marioHpCurrentPos;
    Vec3 marioHpCurrentPosWork;
    Vec3 marioHpSlashScale;
    Vec3 marioHpSlashPos;
    Vec3 marioHpSlashPosWork;
    Vec3 marioHpMaxScale;
    Vec3 marioHpMaxPos;
    Vec3 marioHpMaxPosWork;
    Vec3 marioFpLabelScale;
    Vec3 marioFpLabelPos;
    Vec3 marioFpLabelPosWork;
    Vec3 marioFpCurrentScale;
    Vec3 marioFpCurrentPos;
    Vec3 marioFpCurrentPosWork;
    Vec3 marioFpSlashScale;
    Vec3 marioFpSlashPos;
    Vec3 marioFpSlashPosWork;
    Vec3 marioFpMaxScale;
    Vec3 marioFpMaxPos;
    Vec3 marioFpMaxPosWork;

    Vec3 partnerTexScale;
    Vec3 partnerTexPos;
    Vec3 partnerTexPosWork;
    Vec3 partnerIconScale;
    Vec3 partnerIconPos;
    Vec3 partnerIconPosWork;
    Vec3 partnerNameScale;
    Vec3 partnerNamePos;
    Vec3 partnerNamePosWork;
    Vec3 partnerHpLabelScale;
    Vec3 partnerHpLabelPos;
    Vec3 partnerHpLabelPosWork;
    Vec3 partnerHpCurrentScale;
    Vec3 partnerHpCurrentPos;
    Vec3 partnerHpCurrentPosWork;
    Vec3 partnerHpSlashScale;
    Vec3 partnerHpSlashPos;
    Vec3 partnerHpSlashPosWork;
    Vec3 partnerHpMaxScale;
    Vec3 partnerHpMaxPos;
    Vec3 partnerHpMaxPosWork;

    Vec3 markerScale;
    Vec3 markerPos;
    Vec3 markerPosWork;
    Vec3 cursorScaleA;
    Vec3 cursorPosA;
    Vec3 cursorPosAWork;
    Vec3 cursorScaleB;
    Vec3 cursorPosB;
    Vec3 cursorPosBWork;

    w = (u8*)winGetPtr();
    currentColor = dat_8042378c[0];
    baseWhite = dat_80423790[0];
    tailWhite = dat_80423790[0];

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

        row22 = 0;
        row28 = 0;

        for (i = 0; i < *(s32*)(w + 0x1E0) + 1; i++) {
            rowAdjust = float_0_804237b8[0];
            if (i != 0) {
                rowAdjust = float_neg28_804237c0[0];
                if (i == 1)
                    rowAdjust = float_neg16_804237bc[0];
            }

            if (*(s32*)(w + 0x2E0) != 0) {
                if (i == *(s32*)(w + 0x2DC)) {
                    currentColor = dat_80423794[0];
                } else {
                    currentColor = dat_80423798[0];
                }
            }

            if (i == 0) {
                if (((*(s32*)(w + 0x2E0)) % 20) < 16 || (*(s32*)(w + 0x2DC)) != 0) {
                    if ((*(s32*)(w + 0x2E0)) != 0 && i != (*(s32*)(w + 0x2DC)))
                        winIconGrayInit();
                    else
                        winIconInit();

                    marioIconColor = baseWhite;
                    marioIconScale = vec3_802f5be0;
                    marioIconPosWork = vec3_802f5bd4;
                    marioIconPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 30);
                    marioIconPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 20 - row22);
                    marioIconPos = marioIconPosWork;
                    winIconSet(0x1A6, &marioIconPos, &marioIconScale, &marioIconColor);

                    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
                    GXSetTevColorIn(0, 15, 15, 15, 2);
                    GXSetTevAlphaIn(0, 7, 1, 4, 7);

                    marioHpTexColor = dat_8042379c[0];
                    marioHpTexScale = vec3_802f5bf8;
                    marioHpTexPosWork = vec3_802f5bec;
                    marioHpTexPosWork.x =
                        float_0p9_804237c4[0] *
                        (f32)(u16)FontGetMessageWidth(lbl_803776B4[(*(s32*)((u8*)gp + 0x16C)) * 2]) *
                        float_0p5_804237b4[0] + (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 220);
                    marioHpTexPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 19 - row22);
                    marioHpTexPos = marioHpTexPosWork;
                    winTexSet(0xAF, &marioHpTexPos, &marioHpTexScale, &marioHpTexColor);

                    marioFpTexColor = dat_804237a0[0];
                    marioFpTexScale = vec3_802f5c10;
                    marioFpTexPosWork = vec3_802f5c04;
                    marioFpTexPosWork.x =
                        float_0p9_804237c4[0] *
                        (f32)(u16)FontGetMessageWidth(lbl_803776B4[(*(s32*)((u8*)gp + 0x16C)) * 2]) *
                        float_0p5_804237b4[0] + (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 220);
                    marioFpTexPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 41);
                    marioFpTexPos = marioFpTexPosWork;
                    winTexSet(0xAF, &marioFpTexPos, &marioFpTexScale, &marioFpTexColor);

                    winFontInit();

                    marioNameColor = currentColor;
                    marioNameScale = vec3_802f5c28;
                    marioNamePosWork = vec3_802f5c1c;
                    marioNamePosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 50);
                    marioNamePosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 8 - row22);
                    text = msgSearch((char*)str_name_mario_802f5e20);
                    marioNamePos = marioNamePosWork;
                    winFontSet(&marioNamePos, &marioNameScale, &marioNameColor, text);

                    marioHpLabelColor = currentColor;
                    marioHpLabelScale = vec3_802f5c40;
                    marioHpLabelPosWork = vec3_802f5c34;
                    marioHpLabelPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 220);
                    marioHpLabelPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 8 - row22);
                    marioHpLabelPos = marioHpLabelPosWork;
                    winFontSet(&marioHpLabelPos, &marioHpLabelScale, &marioHpLabelColor, lbl_803776B4[(*(s32*)((u8*)gp + 0x16C)) * 2]);

                    marioHpCurrentColor = currentColor;
                    marioHpCurrentScale = vec3_802f5c58;
                    marioHpCurrentPosWork = vec3_802f5c4c;
                    marioHpCurrentPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 245);
                    marioHpCurrentPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 8 - row22);
                    marioHpCurrentPos = marioHpCurrentPosWork;
                    winFontSetR(&marioHpCurrentPos, &marioHpCurrentScale, &marioHpCurrentColor,
                                str_PCTd_804237c8, pouchGetHP());

                    text = (*(s32*)((u8*)gp + 0x16C)) == 0 ? str_slash_jp_804237d0 : str__804237cc;
                    marioHpSlashColor = currentColor;
                    marioHpSlashScale = vec3_802f5c70;
                    marioHpSlashPosWork = vec3_802f5c64;
                    marioHpSlashPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 267);
                    marioHpSlashPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 8 - row22);
                    marioHpSlashPos = marioHpSlashPosWork;
                    winFontSetR(&marioHpSlashPos, &marioHpSlashScale, &marioHpSlashColor, text);

                    marioHpMaxColor = currentColor;
                    marioHpMaxScale = vec3_802f5c88;
                    marioHpMaxPosWork = vec3_802f5c7c;
                    marioHpMaxPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 310);
                    marioHpMaxPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 10 - row22);
                    marioHpMaxPos = marioHpMaxPosWork;
                    winFontSet(&marioHpMaxPos, &marioHpMaxScale, &marioHpMaxColor,
                               str_PCTd_804237c8, pouchGetMaxHP());

                    if (*(u8*)(itemDataTable + *(s32*)(w + 0x2D4) * 0x28 + 0x26) != 0) {
                        if (((*(s32*)(w + 0x2E0)) % 20) > 15)
                            goto item_use_row_done;
                        currentColor = dat_804237a4[0];
                    }

                    winFontInit();

                    marioFpLabelColor = currentColor;
                    marioFpLabelScale = vec3_802f5ca0;
                    marioFpLabelPosWork = vec3_802f5c94;
                    marioFpLabelPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 220);
                    marioFpLabelPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 30);
                    marioFpLabelPos = marioFpLabelPosWork;
                    winFontSet(&marioFpLabelPos, &marioFpLabelScale, &marioFpLabelColor, lbl_803776B4[(*(s32*)((u8*)gp + 0x16C)) * 2 + 1]);

                    marioFpCurrentColor = currentColor;
                    marioFpCurrentScale = vec3_802f5cb8;
                    marioFpCurrentPosWork = vec3_802f5cac;
                    marioFpCurrentPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 245);
                    marioFpCurrentPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 30);
                    marioFpCurrentPos = marioFpCurrentPosWork;
                    winFontSetR(&marioFpCurrentPos, &marioFpCurrentScale, &marioFpCurrentColor,
                                str_PCTd_804237c8, pouchGetFP());

                    text = (*(s32*)((u8*)gp + 0x16C)) == 0 ? str_slash_jp_804237d0 : str__804237cc;
                    marioFpSlashColor = currentColor;
                    marioFpSlashScale = vec3_802f5cd0;
                    marioFpSlashPosWork = vec3_802f5cc4;
                    marioFpSlashPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 267);
                    marioFpSlashPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 30);
                    marioFpSlashPos = marioFpSlashPosWork;
                    winFontSetR(&marioFpSlashPos, &marioFpSlashScale, &marioFpSlashColor, text);

                    marioFpMaxColor = currentColor;
                    marioFpMaxScale = vec3_802f5ce8;
                    marioFpMaxPosWork = vec3_802f5cdc;
                    marioFpMaxPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 310);
                    marioFpMaxPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 32);
                    marioFpMaxPos = marioFpMaxPosWork;
                    winFontSet(&marioFpMaxPos, &marioFpMaxScale, &marioFpMaxColor,
                               str_PCTd_804237c8, pouchGetMaxFP());
                }
            } else {
                partnerId = *(s32*)partyOrder[i - 1];
                if (partnerId == 4)
                    text = pouchGetYoshiName();
                else
                    text = msgSearch(*(char**)(partyOrder[i - 1] + 8));

                if (((*(s32*)(w + 0x2E0)) % 20) < 16 || i != (*(s32*)(w + 0x2DC))) {
                    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
                    GXSetTevColorIn(0, 15, 15, 15, 2);
                    GXSetTevAlphaIn(0, 7, 1, 4, 7);

                    partnerTexColor = dat_804237a8[0];
                    partnerTexScale = vec3_802f5d00;
                    partnerTexPosWork = vec3_802f5cf4;
                    partnerTexPosWork.x =
                        float_0p9_804237c4[0] *
                        (f32)(u16)FontGetMessageWidth(lbl_803776B4[0]) *
                        float_0p5_804237b4[0] + (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 220);
                    partnerTexPosWork.y = rowAdjust + (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 8 - row28) - float_14_804237d4[0];
                    partnerTexPos = partnerTexPosWork;
                    winTexSet(0xAF, &partnerTexPos, &partnerTexScale, &partnerTexColor);

                    if ((*(s32*)(w + 0x2E0)) != 0 && i != (*(s32*)(w + 0x2DC)))
                        winIconGrayInit();
                    else
                        winIconInit();

                    partnerIconColor = baseWhite;
                    partnerIconScale = vec3_802f5d18;
                    partnerIconPosWork = vec3_802f5d0c;
                    partnerIconPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 30);
                    partnerIconPosWork.y = rowAdjust + (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 20 - row28);
                    partnerIconPos = partnerIconPosWork;
                    winIconSet(*(s16*)(partyOrder[i - 1] + 4),
                               &partnerIconPos, &partnerIconScale, &partnerIconColor);

                    winFontInit();

                    partnerNameColor = currentColor;
                    partnerNameScale = vec3_802f5d30;
                    nameScale = float_1_804237d8[0];
                    if (float_160_804237dc[0] <= (f32)(u16)FontGetMessageWidth(text))
                        nameScale = float_160_804237dc[0] / (f32)(u16)FontGetMessageWidth(text);
                    partnerNameScale.x = nameScale;
                    partnerNamePosWork = vec3_802f5d24;
                    partnerNamePosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 50);
                    partnerNamePosWork.y = rowAdjust + (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 8 - row28);
                    partnerNamePos = partnerNamePosWork;
                    winFontSet(&partnerNamePos, &partnerNameScale, &partnerNameColor, text);

                    partnerHpLabelColor = currentColor;
                    partnerHpLabelScale = vec3_802f5d48;
                    partnerHpLabelPosWork = vec3_802f5d3c;
                    partnerHpLabelPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 220);
                    partnerHpLabelPosWork.y = rowAdjust + (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 8 - row28);
                    partnerHpLabelPos = partnerHpLabelPosWork;
                    winFontSet(&partnerHpLabelPos, &partnerHpLabelScale, &partnerHpLabelColor, lbl_803776B4[(*(s32*)((u8*)gp + 0x16C)) * 2]);

                    partnerHpCurrentColor = currentColor;
                    partnerHpCurrentScale = vec3_802f5d60;
                    partnerHpCurrentPosWork = vec3_802f5d54;
                    partnerHpCurrentPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 245);
                    partnerHpCurrentPosWork.y = rowAdjust + (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 8 - row28);
                    partnerHpCurrentPos = partnerHpCurrentPosWork;
                    winFontSetR(
                        &partnerHpCurrentPos, &partnerHpCurrentScale, &partnerHpCurrentColor,
                        str_PCTd_804237c8,
                        *(s16*)((u8*)pouchGetPtr() + partnerId * 14 + 6));

                    text = (*(s32*)((u8*)gp + 0x16C)) == 0 ? str_slash_jp_804237d0 : str__804237cc;
                    partnerHpSlashColor = currentColor;
                    partnerHpSlashScale = vec3_802f5d78;
                    partnerHpSlashPosWork = vec3_802f5d6c;
                    partnerHpSlashPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 267);
                    partnerHpSlashPosWork.y = rowAdjust + (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 8 - row28);
                    partnerHpSlashPos = partnerHpSlashPosWork;
                    winFontSetR(&partnerHpSlashPos, &partnerHpSlashScale, &partnerHpSlashColor, text);

                    partnerHpMaxColor = currentColor;
                    partnerHpMaxScale = vec3_802f5d90;
                    partnerHpMaxPosWork = vec3_802f5d84;
                    partnerHpMaxPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + 310);
                    partnerHpMaxPosWork.y =
                        rowAdjust + (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 8 - row28) - float_2_804237e0[0];
                    partnerHpMaxPos = partnerHpMaxPosWork;
                    winFontSet(
                        &partnerHpMaxPos, &partnerHpMaxScale, &partnerHpMaxColor,
                        str_PCTd_804237c8,
                        *(s16*)((u8*)pouchGetPtr() + partnerId * 14 + 2));
                }
            }

item_use_row_done:
            row22 += 22;
            row28 += 28;
        }

        if (*(s32*)(w + 0x1E0) > 1) {
            winIconInit();
            j = 2;
            do {
                markerColor = currentColor;
                markerScale = vec3_802f5da8;
                markerPosWork = vec3_802f5d9c;
                markerPosWork.x = (f32)((*(s32*)((u8*)pWinMgr + 0x18)) + ((*(s32*)((u8*)pWinMgr + 0x20)) / 31) * j);
                markerPosWork.y = (f32)((*(s32*)((u8*)pWinMgr + 0x1C)) - 84);
                markerPos = markerPosWork;
                winIconSet(0x1BF, &markerPos, &markerScale, &markerColor);
                j++;
            } while (j < 30);
        }
    }

    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));

    cursorColorA = dat_804237ac[0];
    cursorScaleA = vec3_802f5dc0;
    cursorPosAWork = vec3_802f5db4;
    cursorPosAWork.x = float_2_804237e0[0] + *(f32*)(w + 0x150);
    cursorPosAWork.y = *(f32*)(w + 0x154) - float_2_804237e0[0];
    cursorPosA = cursorPosAWork;
    winTexSet(0, &cursorPosA, &cursorScaleA, &cursorColorA);

    cursorColorB = tailWhite;
    cursorScaleB = vec3_802f5dd8;
    cursorPosBWork = vec3_802f5dcc;
    cursorPosBWork.x = *(f32*)(w + 0x150);
    cursorPosBWork.y = *(f32*)(w + 0x154);
    cursorPosB = cursorPosBWork;
    winTexSet(0, &cursorPosB, &cursorScaleB, &cursorColorB);
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
    extern const u32 dat_804237b0[1];
    extern LocalVec vec3_802f5df0;
    extern LocalVec vec3_802f5de4;
    extern char str_msg_window_select_5_802f5dfc[];
    extern const f32 float_0p5_804237b4[1];
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
    color = dat_804237b0[0];
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
        color = dat_804237b0[0];
        scale.x = vec3_802f5df0.x;
        scale.y = vec3_802f5df0.y;
        scale.z = vec3_802f5df0.z;
        position.x = ((f32)*(s32*)((s32)pWinMgr + 0x20) - msgWidth) * float_0p5_804237b4[0]
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
