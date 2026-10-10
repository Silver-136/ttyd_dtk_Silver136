#include "window/win_root.h"
#include "driver/swdrv.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

u8 enemy_monoshiri_sort_table[231] = {
    0x00, 0x01, 0x02, 0x03, 0x2F, 0x30, 0x94, 0x2A, 0x62, 0x95, 0x96, 0x12, 0x24, 0x24, 0x0A, 0x0B,
    0x32, 0x14, 0x4D, 0x54, 0x33, 0x32, 0x15, 0x63, 0x43, 0x38, 0x4F, 0x3E, 0x6B, 0x6E, 0x6F, 0x79,
    0x7A, 0x7B, 0x75, 0x99, 0x01, 0x0C, 0x0D, 0x36, 0x1B, 0x1D, 0x4E, 0x56, 0x2C, 0x2D, 0x23, 0x0E,
    0x0F, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27, 0x2B, 0x18, 0x19, 0x1A, 0x5A, 0x13, 0x50, 0x51, 0x29,
    0x66, 0x67, 0x04, 0x05, 0x06, 0x3C, 0x3D, 0x52, 0x1F, 0x20, 0x40, 0x47, 0x48, 0x68, 0x9A, 0x9B,
    0x9C, 0x9D, 0x9E, 0x9F, 0x4A, 0x4B, 0x34, 0x35, 0x39, 0x21, 0x5B, 0x5C, 0x57, 0x69, 0xA0, 0xA1,
    0xA2, 0xA3, 0xA4, 0x77, 0x6B, 0x6B, 0x6B, 0x44, 0x37, 0x22, 0x49, 0x6A, 0xA5, 0xA6, 0xA7, 0xA8,
    0x45, 0x3A, 0x53, 0x70, 0x71, 0x72, 0x73, 0x6C, 0x6D, 0x76, 0xA9, 0x41, 0x4C, 0x5D, 0x5E, 0x59,
    0x5F, 0x5F, 0x16, 0x17, 0x64, 0x79, 0x7A, 0x68, 0x68, 0x68, 0x68, 0x68, 0x68, 0x68, 0x68, 0x68,
    0x29, 0x28, 0x78, 0x74, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x07, 0x08, 0x09, 0x10, 0x11, 0x2E, 0x1C,
    0x1E, 0x60, 0x3B, 0x31, 0x3F, 0x55, 0x46, 0x42, 0x58, 0x61, 0x61, 0x65, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,
};

/* Room-prefix strings used by the tattle lookup directory. */
extern const char str_nara_80423264[];
extern const char str_gor_8042326c[];
extern const char str_tik_80423270[];
extern const char str_shiga_80423274[];
extern const char str_hei_8042327c[];
extern const char str_nok_80423280[];
extern const char str_stn_80423284[];
extern const char str_gon_80423288[];
extern const char str_win_8042328c[];
extern const char str_hou_80423290[];
extern const char str_mri_80423294[];
extern const char str_tou_80423298[];
extern const char str_usu_8042329c[];
extern const char str_gra_804232a0[];
extern const char str_jin_804232a4[];
extern const char str_muj_804232a8[];
extern const char str_dou_804232ac[];
extern const char str_hom_804232b0[];
extern const char str_eki_804232b4[];
extern const char str_rsh_804232b8[];
extern const char str_pik_804232bc[];
extern const char str_sin_804232c0[];
extern const char str_bom_804232c4[];
extern const char str_moo_804232c8[];
extern const char str_aji_804232cc[];
extern const char str_las_804232d0[];

char* tbl_1614[26] = {
    (char*)str_nara_80423264,
    (char*)str_gor_8042326c,
    (char*)str_tik_80423270,
    (char*)str_shiga_80423274,
    (char*)str_hei_8042327c,
    (char*)str_nok_80423280,
    (char*)str_stn_80423284,
    (char*)str_gon_80423288,
    (char*)str_win_8042328c,
    (char*)str_hou_80423290,
    (char*)str_mri_80423294,
    (char*)str_tou_80423298,
    (char*)str_usu_8042329c,
    (char*)str_gra_804232a0,
    (char*)str_jin_804232a4,
    (char*)str_muj_804232a8,
    (char*)str_dou_804232ac,
    (char*)str_hom_804232b0,
    (char*)str_eki_804232b4,
    (char*)str_rsh_804232b8,
    (char*)str_pik_804232bc,
    (char*)str_sin_804232c0,
    (char*)str_bom_804232c4,
    (char*)str_moo_804232c8,
    (char*)str_aji_804232cc,
    (char*)str_las_804232d0,
};

extern f32 float_neg1000_804233a0;
extern void* sort_data[8];
void qqsort(void* base, s32 count, s32 size, void* compare);
s32 pouchGetHaveItemCnt(void);
s32 pouchGetHaveBadgeCnt(void);
s32 pouchHaveItem(s32 item);
s32 pouchHaveBadge(s32 badge);
void pouchSortItem(s32 type);
void winMakeSkipList(void* win);
void winMakeEquipList(void* win);
s32 N_compare_func4_1(void* a, void* b);
s32 N_compare_func4_1_r(void* a, void* b);
s32 N_compare_func4_2(void* a, void* b);
s32 N_compare_func4_2_r(void* a, void* b);
s32 compare_func4(void* a, void* b);
s32 compare_func4_r(void* a, void* b);
s32 compare_func5_1(void* a, void* b);
s32 compare_func5_1_r(void* a, void* b);
s32 compare_func5_2(void* a, void* b);
s32 compare_func5_2_r(void* a, void* b);
s32 compare_func6_1(void* a, void* b);
s32 compare_func6_1_r(void* a, void* b);
s32 compare_func6_2(void* a, void* b);
s32 compare_func6_2_r(void* a, void* b);
extern u8 itemDataTable[];
extern void* gp;
extern f32 float_0_80423338;
void* mailGetPtr(s32 id);
void winTexInit(void* data);
void winFontInit(void);
void GXSetTevColor(s32,void*);
void GXSetBlendMode(s32,s32,s32,s32);
void GXSetZCompLoc(s32);
void GXSetAlphaCompare(s32,s32,s32,s32,s32);
void GXSetZMode(s32,s32,s32);
void GXSetFog(s32,f32,f32,f32,f32,void*);
void GXSetNumChans(s32);
void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
void GXSetNumTevStages(s32);
void GXSetTevOrder(s32,s32,s32,s32);
void GXSetTevOp(s32,s32);
void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
void GXSetTevColorIn(s32,s32,s32,s32,s32);
void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
void GXSetTevSwapMode(s32,s32,s32);
void PSMTXTrans(void*,f32,f32,f32);
void PSMTXConcat(void*,void*,void*);
void PSMTXScale(void*,f32,f32,f32);
void* camGetPtr(s32);
void GXLoadPosMtxImm(void*,s32);
void GXLoadTexMtxImm(void*,s32,s32);
void GXSetCurrentMtx(s32);
void GXSetCullMode(s32);
void GXClearVtxDesc(void);
void GXSetVtxDesc(s32,s32);
void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
void TEXGetGXTexObjFromPalette(void*,void*,s32);
void GXInitTexObjLOD(void*,s32,s32,f32,f32,f32,s32,s32,s32);
void GXLoadTexObj(void*,s32);
void GXSetNumTexGens(s32);
void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
s32 GXGetTexObjHeight(void*);
s32 GXGetTexObjWidth(void*);
void GXBegin(s32,s32,s32);
extern volatile f32 DAT_cc008000;
void psndSFXOn(s32 soundId);
void winIconInit(void);
void windowDispGX_Waku_col(f32 x, f32 y, f32 w, f32 h, f32 r, s32 type, void* color);
void winTexSet(s32 id, void* pos, void* scale, void* color);
void winIconSet(s32 icon, void* pos, void* scale, void* color);
void winFontSet(void* pos, void* scale, void* color, char* text);
void winFontSetLabel(void* pos, void* scale, void* color, char* label);
extern f32 float_1_80423340;
extern f32 float_0p5_8042333c;
extern f32 float_2_80423360;
char* msgSearch(const char* msg);
void unk_800d48b0(char* msg, char* out);
s32 strcmp(const char* a, const char* b);
void FontGetMessageWidthLine(char* msg, u16* width);
char* strncpy(char* dest, const char* src, u32 n);
extern char _tmp_1042[];
void* battleGetUnitMonosiriPtr(s32 id);
s32 sprintf(char* str, const char* format, ...);
s32 evtGetValue(void* evt, s32 value);
extern char str_menu_monosiri_PCTs_802f4d00[];
extern char str_btl_un_hatena_802f4ce0[];
extern char str_btl_un_rampell_802f4cf0[];

extern char str_msg_menu_mario_802f47d8[];
extern char str_msg_menu_nakama_802f47e8[];
extern char str_msg_menu_mochimono_802f47f8[];
extern char str_msg_menu_badge_802f480c[];
extern char str_msg_menu_kiroku_802f481c[];
char* msgHelp[5] = {
    "msg_menu_mario",
    "msg_menu_nakama",
    "msg_menu_mochimono",
    "msg_menu_badge",
    "msg_menu_kiroku",
};

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
#pragma optimize_for_size off
#pragma opt_common_subs off
s32 winRootMain(void* pWin) {
    typedef struct RootBodyRecord {
        s32 type;
        f32 x, y;
        s32 state, timer;
    } RootBodyRecord;
    extern char _tmp_1042[];
    extern f32 float_185_804234b0;
    extern f32 float_neg1000_804233a0;
    extern f32 float_4_8042334c;
    extern f32 float_60_80423468;
    extern s32 winMarioMain(void* win);
    extern s32 winPartyMain(void* win);
    extern s32 winItemMain(void* win);
    extern s32 winBadgeMain(void* win);
    extern s32 winLogMain(void* win);
    extern void winMarioInit(void* win);
    extern void winPartyInit(void* win);
    extern void winItemInit(void* win);
    extern void winBadgeInit(void* win);
    extern void winLogInit(void* win);
    extern void winMarioInit2(void* win);
    extern void winPartyInit2(void* win);
    extern void winItemInit2(void* win);
    extern void winBadgeInit2(void* win);
    extern void winLogInit2(void* win);
    extern void winBgMain(void* win);
    extern void winMsgMain(void* win);
    extern void winMarioMain2(void);
    extern void winPartyMain2(void* win);
    extern void winItemMain2(void* win);
    extern void winBadgeMain2(void* win);
    extern void winLogMain2(void* win);
    extern void winSortMain(void* win);
    extern u32 swByteGet(s32 id);
    extern u32 partyChkJoin(s32 id);
    extern s32 pouchGetHaveBadgeCnt(void);
    extern void N_mapDispOn(void);
    extern void L_camDispOn(s32 camera);
    extern void smartFree(void* ptr);
    extern void winMarioExit(void* win);
    extern void winPartyExit(void* win);
    extern void winItemExit(void* win);
    extern void winBadgeExit(void* win);
    extern void winLogExit(void* win);
    extern u32 GXGetTexBufferSize(u16 width, u16 height, s32 format, s32 mipmap, s32 roundUp);
    extern void* smartAlloc(u32 size, s32 type);
    extern void unk_80152e80(s32 cameraId, void* win);
    extern double intplGetValue(double start, double end, s32 type, s32 time, s32 duration);
    extern double sin(double value);
    extern double fabs(double value);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* user, f32 priority);
    extern void unk_80152bdc(s32 cameraId, void* win);
    extern f32 float_0p125_804233d8;
    extern f32 float_2_80423360;
    extern f32 float_180_80423460;
    u8* w = (u8*)pWin;
    s32 result = 0;
    s32 duration;
    s32 i;
    char* help;
    BOOL updateHelp;
    u16 lineWidth[2];
    u8* entry;

    switch (*(s32*)(w + 0x24)) {
    case 0:
        psndSFXOn(0x2002A);
        *(s32*)(w + 0x44) = 0;

        entry = w + 0x48 + *(s32*)(w + 0x44) * 0x18;
        *(s32*)(entry + 0x00) = 0;
        *(s32*)(entry + 0x0C) = 0;
        *(f32*)(entry + 0x08) = 1000.0f;
        *(f32*)(entry + 0x04) = 1000.0f;
        *(char**)(entry + 0x14) = msgHelp[0];
        (*(s32*)(w + 0x44))++;

        if ((s32)swByteGet(0) > 6 || partyChkJoin(1) != 0) {
            entry = w + 0x48 + *(s32*)(w + 0x44) * 0x18;
            *(s32*)(entry + 0x00) = 1;
        *(s32*)(entry + 0x0C) = 0;
        *(f32*)(entry + 0x08) = 1000.0f;
        *(f32*)(entry + 0x04) = 1000.0f;
            *(char**)(entry + 0x14) = msgHelp[1];
            (*(s32*)(w + 0x44))++;
        }

        entry = w + 0x48 + *(s32*)(w + 0x44) * 0x18;
        *(s32*)(entry + 0x00) = 2;
        *(s32*)(entry + 0x0C) = 0;
        *(f32*)(entry + 0x08) = 1000.0f;
        *(f32*)(entry + 0x04) = 1000.0f;
        *(char**)(entry + 0x14) = msgHelp[2];
        (*(s32*)(w + 0x44))++;

        if ((s32)swByteGet(0) > 0x13 || pouchGetHaveBadgeCnt() != 0) {
            entry = w + 0x48 + *(s32*)(w + 0x44) * 0x18;
            *(s32*)(entry + 0x00) = 3;
        *(s32*)(entry + 0x0C) = 0;
        *(f32*)(entry + 0x08) = 1000.0f;
        *(f32*)(entry + 0x04) = 1000.0f;
            *(char**)(entry + 0x14) = msgHelp[3];
            (*(s32*)(w + 0x44))++;
        }

        entry = w + 0x48 + *(s32*)(w + 0x44) * 0x18;
        *(s32*)(entry + 0x00) = 4;
        *(s32*)(entry + 0x0C) = 0;
        *(f32*)(entry + 0x08) = 1000.0f;
        *(f32*)(entry + 0x04) = 1000.0f;
        *(char**)(entry + 0x14) = msgHelp[4];
        (*(s32*)(w + 0x44))++;

        for (i = 0; i < *(s32*)(w + 0x44); i++) {
            if (*(s32*)(w + 0x48 + i * 0x18) == *(s32*)(w + 0x28)) {
                *(s32*)(w + 0x40) = i;
            }
        }

        *(s32*)(w + 0xCC) = -1;
        *(f32*)(w + 0xC8) = 1000.0f;
        *(f32*)(w + 0xC4) = 1000.0f;
        *(s32*)(w + 0xC0) = -1;

        *(s32*)(w + 0xE0) = -1;
        *(f32*)(w + 0xDC) = 1000.0f;
        *(f32*)(w + 0xD8) = 1000.0f;
        *(s32*)(w + 0xD4) = -1;

        *(s32*)(w + 0xF4) = -1;
        *(f32*)(w + 0xF0) = 1000.0f;
        *(f32*)(w + 0xEC) = 1000.0f;
        *(s32*)(w + 0xE8) = -1;

        *(s32*)(w + 0x108) = -1;
        *(f32*)(w + 0x104) = 1000.0f;
        *(f32*)(w + 0x100) = 1000.0f;
        *(s32*)(w + 0xFC) = -1;

        *(s32*)(w + 0x11C) = -1;
        *(f32*)(w + 0x118) = 1000.0f;
        *(f32*)(w + 0x114) = 1000.0f;
        *(s32*)(w + 0x110) = -1;

        *(f32*)(w + 0xC8) = 1000.0f;
        *(f32*)(w + 0xC4) = 1000.0f;
        *(s32*)(w + 0xCC) = 0;
        *(s32*)(w + 0xC0) = *(s32*)(w + 0x48 + *(s32*)(w + 0x40) * 0x18);

        *(s32*)(w + 0x130) = 0;
        *(f32*)(w + 0x12C) = 1000.0f;
        *(f32*)(w + 0x128) = 1000.0f;
        *(void**)(w + 0x13C) = 0;
        *(void**)(w + 0x140) = 0;
        *(void**)(w + 0x144) = 0;

        winMarioInit(pWin);
        winPartyInit(pWin);
        winItemInit(pWin);
        winBadgeInit(pWin);
        winLogInit(pWin);

        *(f32*)(w + 0x150) = (f32)(*(s32*)(w + 0x40) * 105 - 270);
        *(f32*)(w + 0x158) = *(f32*)(w + 0x150);
        *(f32*)(w + 0x154) = 300.0f;
        *(f32*)(w + 0x15C) = float_185_804234b0;
        *(f32*)(w + 0x164) = float_neg1000_804233a0;
        *(f32*)(w + 0x168) = float_neg1000_804233a0;
        *(f32*)(w + 0x16C) = float_neg1000_804233a0;
        *(f32*)(w + 0x170) = float_neg1000_804233a0;
        help = *(char**)(w + 0x48 + *(s32*)(w + 0x40) * 0x18 + 0x14);
        updateHelp = FALSE;
            if (help != 0) {
                if (*(char**)(w + 0x144) == help) {
                    if (strcmp(_tmp_1042, help) != 0) updateHelp = TRUE;
                } else updateHelp = TRUE;
            }
            if (updateHelp) {
            *(s32*)(w + 0x138) = 0;
            *(char**)(w + 0x13C) = help;
            *(char**)(w + 0x140) = 0;
            FontGetMessageWidthLine(msgSearch(help), lineWidth);
            *(s32*)(w + 0x148) = 0;
            *(s32*)(w + 0x14C) = lineWidth[0] + 1;
            *(char**)(w + 0x144) = help;
            strncpy(_tmp_1042, help, 0x3C);
        }
        *(s32*)(w + 0x24) = 10;
        switch (*(s32*)(w + 0x1C)) {
            case 1:
                winPartyInit2(pWin);
                *(s32*)(w + 0x24) = 0xC;
                *(u16*)w |= 0x4000;
                break;
            case 2:
                winItemInit2(pWin);
                *(s32*)(w + 0x24) = 0xD;
                *(u16*)w |= 0x4000;
                break;
            case 3:
                winBadgeInit2(pWin);
                *(s32*)(w + 0x24) = 0xE;
                *(u16*)w |= 0x4000;
                break;
            case 4:
                winLogInit2(pWin);
                *(s32*)(w + 0x24) = 0xF;
                *(u16*)w |= 0x4000;
                break;
        }

        if ((*(u32*)gp & 0x1000) != 0) {
            *(s32*)(w + 0x44) = 0;

            entry = w + 0x48;
            *(s32*)(entry + 0x00) = 1;
            *(s32*)(entry + 0x0C) = 0;
            *(f32*)(entry + 0x08) = 1000.0f;
            *(f32*)(entry + 0x04) = 1000.0f;
            *(char**)(entry + 0x14) = msgHelp[1];

            *(s32*)(w + 0x44) = 1;
            winPartyInit2(pWin);
            *(s32*)(w + 0x24) = 0xC;
            *(s32*)(w + 0x40) = 0;
        }

        {
            u16 width = *(u16*)((u8*)gp + 0x170);
            u16 height = *(u16*)((u8*)gp + 0x172);
            u32 size = GXGetTexBufferSize(width, height, 4, 0, 0);

            *(void**)(w + 0x34) = smartAlloc(size, 1);
            *(s32*)(w + 0x38) = 0;
            dispEntry(8, 0, (void*)unk_80152e80, pWin, 1000.0f);
        }
        break;
    case 10:
        if ((*(u32*)(w + 4) & 0x1200) != 0) {
            if (*(s32*)(w + 0xCC) == 100) {
                *(s32*)(w + 0x24) = 0x14;
            }
        } else if ((*(u32*)(w + 4) & 0x100) != 0) {
            s32 menu = *(s32*)(w + 0x48 + *(s32*)(w + 0x40) * 0x18);
            if (menu != 1 || *(s32*)(w + 0x1E0) != 0) {
                psndSFXOn(0x20012);
                *(s32*)(w + 0x24) = menu + 0xB;
                if (menu == 0) winMarioInit2(pWin);
                else if (menu == 1) winPartyInit2(pWin);
                else if (menu == 2) winItemInit2(pWin);
                else if (menu == 3) winBadgeInit2(pWin);
                else winLogInit2(pWin);
            }
        } else if ((*(u32*)(w + 0x10) & 0x4000) != 0 && *(s32*)(w + 0xCC) == 100) {
            psndSFXOn(0x20032);
            psndSFXOn(0x20031);
            (*(s32*)(w + 0x40))--;
            if (*(s32*)(w + 0x40) < 0) {
                *(s32*)(w + 0x40) = *(s32*)(w + 0x44) - 1;
            }

            *(RootBodyRecord*)(w + 0x110) = *(RootBodyRecord*)(w + 0xFC);

            *(RootBodyRecord*)(w + 0xFC) = *(RootBodyRecord*)(w + 0xE8);

            *(RootBodyRecord*)(w + 0xE8) = *(RootBodyRecord*)(w + 0xD4);

            *(RootBodyRecord*)(w + 0xD4) = *(RootBodyRecord*)(w + 0xC0);

            *(f32*)(w + 0xC8) = 1000.0f;
            *(f32*)(w + 0xC4) = 1000.0f;
            *(s32*)(w + 0xCC) = 0x1E;
            *(s32*)(w + 0xC0) = *(s32*)(w + 0x48 + *(s32*)(w + 0x40) * 0x18);
            *(f32*)(w + 0x158) = (f32)(*(s32*)(w + 0x40) * 105 - 270);
            help = *(char**)(w + 0x48 + *(s32*)(w + 0x40) * 0x18 + 0x14);
            updateHelp = FALSE;
            if (help != 0) {
                if (*(char**)(w + 0x144) == help) {
                    if (strcmp(_tmp_1042, help) != 0) updateHelp = TRUE;
                } else updateHelp = TRUE;
            }
            if (updateHelp) {
                *(s32*)(w + 0x138) = 0;
                *(char**)(w + 0x13C) = help;
                *(char**)(w + 0x140) = 0;
                FontGetMessageWidthLine(msgSearch(help), lineWidth);
                *(s32*)(w + 0x148) = 0;
                *(s32*)(w + 0x14C) = lineWidth[0] + 1;
                *(char**)(w + 0x144) = help;
                strncpy(_tmp_1042, help, 0x3C);
            }
        } else if ((*(u32*)(w + 0x10) & 0x8000) != 0 && *(s32*)(w + 0xCC) == 100) {
            psndSFXOn(0x20032);
            psndSFXOn(0x20031);
            (*(s32*)(w + 0x40))++;
            if (*(s32*)(w + 0x40) >= *(s32*)(w + 0x44)) {
                *(s32*)(w + 0x40) = 0;
            }

            *(RootBodyRecord*)(w + 0x110) = *(RootBodyRecord*)(w + 0xFC);

            *(RootBodyRecord*)(w + 0xFC) = *(RootBodyRecord*)(w + 0xE8);

            *(RootBodyRecord*)(w + 0xE8) = *(RootBodyRecord*)(w + 0xD4);

            *(RootBodyRecord*)(w + 0xD4) = *(RootBodyRecord*)(w + 0xC0);

            *(f32*)(w + 0xC8) = 1000.0f;
            *(f32*)(w + 0xC4) = 1000.0f;
            *(s32*)(w + 0xCC) = 0x14;
            *(s32*)(w + 0xC0) = *(s32*)(w + 0x48 + *(s32*)(w + 0x40) * 0x18);
            *(f32*)(w + 0x158) = (f32)(*(s32*)(w + 0x40) * 105 - 270);
            help = *(char**)(w + 0x48 + *(s32*)(w + 0x40) * 0x18 + 0x14);
            updateHelp = FALSE;
            if (help != 0) {
                if (*(char**)(w + 0x144) == help) {
                    if (strcmp(_tmp_1042, help) != 0) updateHelp = TRUE;
                } else updateHelp = TRUE;
            }
            if (updateHelp) {
                *(s32*)(w + 0x138) = 0;
                *(char**)(w + 0x13C) = help;
                *(char**)(w + 0x140) = 0;
                FontGetMessageWidthLine(msgSearch(help), lineWidth);
                *(s32*)(w + 0x148) = 0;
                *(s32*)(w + 0x14C) = lineWidth[0] + 1;
                *(char**)(w + 0x144) = help;
                strncpy(_tmp_1042, help, 0x3C);
            }
        }
        break;
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
        switch (*(s32*)(w + 0x24)) {
            case 0xB:
                result = winMarioMain(pWin);
                break;
            case 0xC:
                result = winPartyMain(pWin);
                break;
            case 0xD:
                result = winItemMain(pWin);
                break;
            case 0xE:
                result = winBadgeMain(pWin);
                break;
            case 0xF:
                result = winLogMain(pWin);
                break;
        }

        if ((*(u32*)gp & 0x1000) != 0 && result == -1) {
            result = -2;
        }

        if (result == -1) {
            *(f32*)(w + 0x158) = (f32)(*(s32*)(w + 0x40) * 105 - 270);
            *(f32*)(w + 0x15C) = float_185_804234b0;
            *(s32*)(w + 0x24) = 10;

            help = *(char**)(w + 0x48 + *(s32*)(w + 0x40) * 0x18 + 0x14);
            updateHelp = FALSE;
            if (help != 0) {
                if (*(char**)(w + 0x144) == help) {
                    if (strcmp(_tmp_1042, help) != 0) updateHelp = TRUE;
                } else updateHelp = TRUE;
            }
            if (updateHelp) {
                *(s32*)(w + 0x138) = 0;
                *(char**)(w + 0x13C) = help;
                *(char**)(w + 0x140) = 0;
                FontGetMessageWidthLine(msgSearch(help), lineWidth);
                *(s32*)(w + 0x148) = 0;
                *(s32*)(w + 0x14C) = lineWidth[0] + 1;
                *(char**)(w + 0x144) = help;
                strncpy(_tmp_1042, help, 0x3C);
            }

            *(u16*)w &= (u16)~0x4000;
        } else if (result == -2) {
            *(s32*)(w + 0x24) = 0x14;
            *(u16*)w &= (u16)~0x4000;
        }
        break;
    case 0x14:
        psndSFXOn(0x2002B);
        for (i = 0; i < *(s32*)(w + 0x44); i++) {
            *(s32*)(w + 0x48 + i * 0x18 + 0x0C) = 10;
        }
        *(s32*)(w + 0xCC) = 10;
        *(s32*)(w + 0xE0) = 10;
        *(s32*)(w + 0xF4) = 10;
        *(s32*)(w + 0x108) = 10;
        *(s32*)(w + 0x11C) = 10;
        *(s32*)(w + 0x130) = 10;
        N_mapDispOn();
        L_camDispOn(4);
        *(s32*)(w + 0x24) += 1;
        break;
    case 100:
        if (*(s32*)(w + 0xCC) == 100) {
            *(s32*)(w + 0x24) = 10;
        }
        break;
    default:
        if (*(void**)(w + 0x34) != 0) {
            smartFree(*(void**)(w + 0x34));
            *(void**)(w + 0x34) = 0;
            *(s32*)(w + 0x38) = 0;
        }
        *(s32*)(w + 0x24) += 1;
        if (*(s32*)(w + 0x24) > 0x32) {
            winMarioExit(pWin);
            winPartyExit(pWin);
            winItemExit(pWin);
            winBadgeExit(pWin);
            winLogExit(pWin);
            if (*(s32*)(w + 0x19C) != 0 &&
                *(s32*)(w + 0x1DC) != *(s32*)(w + 0x1FC)) {
                return -2;
            }
            if (*(s32*)(w + 0x2D4) != 0) {
                return -3;
            }
            return -1;
        }
        break;
    }

    entry = w;
    for (i = 0; i < 5; i++, entry += 0x18) {
        switch (*(s32*)(entry + 0x54)) {
            case 0:
                *(f32*)(entry + 0x4C) = (f32)(-205 + i * 105);
                *(f32*)(entry + 0x50) = 300.0f;
                *(s32*)(entry + 0x58) = i * 4;
                *(s32*)(entry + 0x54) += 1;
                break;
            case 1:
            case 0xB:
                *(s32*)(entry + 0x58) -= 1;
                if (*(s32*)(entry + 0x58) < 0) {
                    *(s32*)(entry + 0x58) = 0;
                    *(s32*)(entry + 0x54) += 1;
                }
                break;
            case 2:
                duration = (*(s32*)((s32)gp + 4) * 10) / 60;
                *(f32*)(entry + 0x50) = (f32)intplGetValue(
                    300.0, 185.0, 0xB, *(s32*)(entry + 0x58), duration);
                *(s32*)(entry + 0x58) += 1;
                duration = (*(s32*)((s32)gp + 4) * 10) / 60;
                if (*(s32*)(entry + 0x58) > duration) {
                    *(s32*)(entry + 0x54) += 1;
                }
                break;
            case 10:
                *(s32*)(entry + 0x58) = (4 - i) * 4;
                *(s32*)(entry + 0x54) += 1;
                break;
            case 0xC:
                duration = (*(s32*)((s32)gp + 4) * 10) / 60;
                *(f32*)(entry + 0x50) = (f32)intplGetValue(
                    180.0, 300.0, 0xB, *(s32*)(entry + 0x58), duration);
                *(s32*)(entry + 0x58) += 1;
                duration = (*(s32*)((s32)gp + 4) * 10) / 60;
                if (*(s32*)(entry + 0x58) > duration) {
                    *(s32*)(entry + 0x54) += 1;
                }
                break;
            case 0xD:
                break;
        }
    }
    winBgMain(pWin);
    winMsgMain(pWin);
    winMarioMain2();
    winPartyMain2(pWin);
    winItemMain2(pWin);
    winBadgeMain2(pWin);
    winLogMain2(pWin);
    winSortMain(pWin);

    if (*(s32*)(w + 0x24) < 0x14) {
        if ((*(u16*)w & 0x4000) == 0) {
            *(f32*)(w + 0x150) += (*(f32*)(w + 0x158) - *(f32*)(w + 0x150)) / ((float_4_8042334c * (f32)*(s32*)((s32)gp + 4)) / float_60_80423468);
            *(f32*)(w + 0x154) +=
                ((*(f32*)(w + 0x15C) +
                  (*(f32*)(w + 0x48 + *(s32*)(w + 0x40) * 0x18 + 8) - float_185_804234b0)) -
                 *(f32*)(w + 0x154)) / ((float_4_8042334c * (f32)*(s32*)((s32)gp + 4)) / float_60_80423468);
        } else {
            *(f32*)(w + 0x150) += (*(f32*)(w + 0x158) - *(f32*)(w + 0x150)) / ((float_4_8042334c * (f32)*(s32*)((s32)gp + 4)) / float_60_80423468);
            *(f32*)(w + 0x154) +=
                (*(f32*)(w + 0x15C) - *(f32*)(w + 0x154)) / ((float_4_8042334c * (f32)*(s32*)((s32)gp + 4)) / float_60_80423468);
        }
    } else if (*(f32*)(w + 0x154) <= float_180_80423460) {
        *(f32*)(w + 0x150) +=
            ((*(f32*)(w + 0x158) + *(f32*)(w + 0xC4)) -
             *(f32*)(w + 0x150)) / ((float_4_8042334c * (f32)*(s32*)((s32)gp + 4)) / float_60_80423468);
        *(f32*)(w + 0x154) +=
            (*(f32*)(w + 0x15C) - *(f32*)(w + 0x154)) / ((float_4_8042334c * (f32)*(s32*)((s32)gp + 4)) / float_60_80423468);
    } else {
        *(f32*)(w + 0x150) +=
            (*(f32*)(w + 0x158) - *(f32*)(w + 0x150)) / ((float_4_8042334c * (f32)*(s32*)((s32)gp + 4)) / float_60_80423468);
        *(f32*)(w + 0x154) +=
            ((*(f32*)(w + 0x15C) +
              (*(f32*)(w + 0x48 + *(s32*)(w + 0x40) * 0x18 + 8) - float_185_804234b0)) -
             *(f32*)(w + 0x154)) / ((float_4_8042334c * (f32)*(s32*)((s32)gp + 4)) / float_60_80423468);
    }

    {
        double phase = (double)((f32)*(u32*)((u8*)gp + 0x1C) *
                       float_0p125_804233d8);
        f32 wave = (f32)sin(phase);
        wave *= float_2_80423360;
        *(f32*)(w + 0x150) += (f32)fabs((double)wave);
    }

    {
        *(f32*)(w + 0x164) += (*(f32*)(w + 0x16C) - *(f32*)(w + 0x164)) / ((float_4_8042334c * (f32)*(s32*)((s32)gp + 4)) / float_60_80423468);
        *(f32*)(w + 0x168) += (*(f32*)(w + 0x170) - *(f32*)(w + 0x168)) / ((float_4_8042334c * (f32)*(s32*)((s32)gp + 4)) / float_60_80423468);
    }

    dispEntry(8, 0, (void*)unk_80152bdc, pWin, 1000.0f);
    return 0;
}
#pragma opt_common_subs reset
#pragma optimize_for_size reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winRootDisp(s32 cameraId, void* pWin) {
    extern void winMarioDisp(s32 cameraId, void* win, s32 index);
    extern void winPartyDisp(s32 cameraId, void* win, s32 index);
    extern void winItemDisp(s32 cameraId, void* win, s32 index);
    extern void winBadgeDisp(s32 cameraId, void* win, s32 index);
    extern void winLogDisp(s32 cameraId, void* win, s32 index);
    extern void winTexInit_x2(void* fileData);
    extern void winTexSet_x2(s32 texId0, s32 texId1, void* pos, void* size, void* color);
    extern void winMsgDisp(f32 x, f32 y, void* win);
    extern char str_msg_menu_mario_802f47d8[];
    extern u32 dat_804231a8;
    extern u32 dat_804231ac;
    extern u32 dat_804231b0;
    extern u32 dat_804231b4;

    char* base = str_msg_menu_mario_802f47d8;
    u32 color;
    u32 headerColor;
    u32 cursorColor;
    u32 normalColor;
    Vec3 headerRawPos;
    Vec3 headerPos;
    Vec3 headerSize;
    Vec3 cursorRawPos;
    Vec3 cursorPos;
    Vec3 cursorSize;
    Vec3 normalRawPos;
    Vec3 normalPos;
    Vec3 normalSize;
    s32 i;
    void* body;
    void* header;
    void* fileData;
    s32 type;
    s32 state;

    color = dat_804231a8;
    if (*(s32*)((s32)pWin + 0x24) == 0) {
        return;
    }

    for (i = 4; i >= 0; i--) {
        body = (void*)((s32)pWin + 0xC0 + i * 0x14);
        state = *(s32*)((s32)body + 0xC);
        if (state != -1 &&
            (i <= 0 || *(s32*)((s32)body - 8) != 100) &&
            (i <= 0 || (u32)(state - 10) > 1)) {
            type = *(s32*)body;
            switch (type) {
                case 0:
                    winMarioDisp(cameraId, pWin, i);
                    break;
                case 1:
                    winPartyDisp(cameraId, pWin, i);
                    break;
                case 2:
                    winItemDisp(cameraId, pWin, i);
                    break;
                case 3:
                    winBadgeDisp(cameraId, pWin, i);
                    break;
                case 4:
                    winLogDisp(cameraId, pWin, i);
                    break;
            }
        }
    }

    header = (void*)((s32)pWin + 0x48);
    for (i = 0; i < *(s32*)((s32)pWin + 0x44); i++) {
        if (*(s32*)((s32)pWin + 0x40) == i) {
            headerColor = dat_804231b0;
        } else {
            headerColor = dat_804231b4;
        }

        fileData = **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0);
        winTexInit_x2(fileData);

        headerSize = *(Vec3*)(base + 0x90);
        headerRawPos = *(Vec3*)(base + 0x84);
        headerRawPos.x = *(f32*)((s32)header + 4);
        headerRawPos.y = *(f32*)((s32)header + 8);
        headerPos = headerRawPos;
        winTexSet_x2(*(s32*)header + 1, 0xB2, &headerPos, &headerSize, &headerColor);
        header = (void*)((s32)header + 0x18);
    }

    winMailDisp(pWin);
    winMsgDisp(*(f32*)((s32)pWin + 0x128), *(f32*)((s32)pWin + 0x12C), pWin);
    winSortGX(pWin);

    if ((*(u16*)pWin & 0x8000) == 0) {
        fileData = **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0);
        winTexInit(fileData);

        cursorColor = dat_804231ac;
        cursorSize = *(Vec3*)(base + 0x60);
        cursorRawPos = *(Vec3*)(base + 0x54);
        cursorRawPos.x = *(f32*)((s32)pWin + 0x150) + float_2_80423360;
        cursorRawPos.y = *(f32*)((s32)pWin + 0x154) - float_2_80423360;
        cursorPos = cursorRawPos;
        winTexSet(0, &cursorPos, &cursorSize, &cursorColor);

        normalColor = color;
        normalSize = *(Vec3*)(base + 0x78);
        normalRawPos = *(Vec3*)(base + 0x6C);
        normalRawPos.x = *(f32*)((s32)pWin + 0x150);
        normalRawPos.y = *(f32*)((s32)pWin + 0x154);
        normalPos = normalRawPos;
        winTexSet(0, &normalPos, &normalSize, &normalColor);
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off
void winBgMain(void* pWin) {
    extern f32 intplGetValue(s32 type, s32 time, s32 duration, f32 start, f32 end);
    extern f32 float_800_804234a4;
    extern f32 float_640_804234a8;
    extern f32 float_neg640_804234ac;

    f32 start, end;
    s32 i = 0;
    s32 state;
    s32 timer;
    s32 duration;
    s32 scaled;
    void* entry = pWin;

    do {
        switch (*(s32*)((s32)entry + 0xCC)) {
            case 0:
                *(f32*)((s32)entry + 0xC4) = float_800_804234a4;
                *(f32*)((s32)entry + 0xC8) = float_0_80423338;
                *(s32*)((s32)entry + 0xD0) = 0;
                (*(s32*)((s32)entry + 0xCC))++;
                break;
            case 1:
                start = float_800_804234a4; end = float_0_80423338;
                *(f32*)((s32)entry + 0xC4) = intplGetValue(0xB, (*(s32*)((s32)entry + 0xD0))++, (*(s32*)((s32)gp + 4) << 4) / 60, start, end);
                scaled = *(s32*)((s32)gp + 4) << 4;
                duration = scaled / 60;
                if (*(s32*)((s32)entry + 0xD0) > duration) {
                    *(s32*)((s32)entry + 0xCC) = 100;
                }
                break;
            case 10:
                *(f32*)((s32)entry + 0xC8) = float_0_80423338;
                *(s32*)((s32)entry + 0xD0) = 0;
                (*(s32*)((s32)entry + 0xCC))++;
                break;
            case 11:
                start = float_0_80423338; end = float_800_804234a4;
                *(f32*)((s32)entry + 0xC4) = intplGetValue(0xB, (*(s32*)((s32)entry + 0xD0))++, (*(s32*)((s32)gp + 4) << 4) / 60, start, end);
                scaled = *(s32*)((s32)gp + 4) << 4;
                duration = scaled / 60;
                if (*(s32*)((s32)entry + 0xD0) > duration) {
                    *(s32*)((s32)entry + 0xCC) = -1;
                }
                break;
            case 20:
                *(f32*)((s32)entry + 0xC8) = float_0_80423338;
                *(s32*)((s32)entry + 0xD0) = 0;
                (*(s32*)((s32)entry + 0xCC))++;
                break;
            case 21:
                start = float_640_804234a8; end = float_0_80423338;
                *(f32*)((s32)entry + 0xC4) = intplGetValue(0xB, (*(s32*)((s32)entry + 0xD0))++, (*(s32*)((s32)gp + 4) * 0x18) / 60, start, end);
                scaled = *(s32*)((s32)gp + 4) * 0x18;
                duration = scaled / 60;
                if (*(s32*)((s32)entry + 0xD0) > duration) {
                    *(s32*)((s32)entry + 0xCC) = 100;
                }
                break;
            case 30:
                *(f32*)((s32)entry + 0xC8) = float_0_80423338;
                *(s32*)((s32)entry + 0xD0) = 0;
                (*(s32*)((s32)entry + 0xCC))++;
                break;
            case 31:
                start = float_neg640_804234ac; end = float_0_80423338;
                *(f32*)((s32)entry + 0xC4) = intplGetValue(0xB, (*(s32*)((s32)entry + 0xD0))++, (*(s32*)((s32)gp + 4) * 0x18) / 60, start, end);
                scaled = *(s32*)((s32)gp + 4) * 0x18;
                duration = scaled / 60;
                if (*(s32*)((s32)entry + 0xD0) > duration) {
                    *(s32*)((s32)entry + 0xCC) = 100;
                }
                break;
            case 40:
                *(f32*)((s32)entry + 0xC8) = float_0_80423338;
                *(s32*)((s32)entry + 0xD0) = 0;
                (*(s32*)((s32)entry + 0xCC))++;
                break;
            case 41:
                start = float_0_80423338; end = float_neg640_804234ac;
                *(f32*)((s32)entry + 0xC4) = intplGetValue(0xB, (*(s32*)((s32)entry + 0xD0))++, (*(s32*)((s32)gp + 4) * 0x18) / 60, start, end);
                scaled = *(s32*)((s32)gp + 4) * 0x18;
                duration = scaled / 60;
                if (*(s32*)((s32)entry + 0xD0) > duration) {
                    *(s32*)((s32)entry + 0xCC) = -1;
                }
                break;
            case 100:
                break;
        }
        i++;
        entry = (void*)((s32)entry + 0x14);
    } while (i < 5);
}
#pragma optimize_for_size reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


void winBgGX(f32 x,f32 y,void* pWin,s32 type){
    extern void GXSetTevColor(s32,void*);extern void GXSetBlendMode(s32,s32,s32,s32);extern void GXSetZCompLoc(s32);extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);extern void GXSetZMode(s32,s32,s32);extern void GXSetFog(s32,f32,f32,f32,f32,void*);extern void GXSetNumChans(s32);extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetTevSwapMode(s32,s32,s32);
    extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXConcat(void*,void*,void*);extern void PSMTXScale(void*,f32,f32,f32);extern void* camGetPtr(s32);extern void GXLoadPosMtxImm(void*,s32);extern void GXLoadTexMtxImm(void*,s32,s32);extern void GXSetCurrentMtx(s32);extern void GXSetCullMode(s32);extern void GXClearVtxDesc(void);extern void GXSetVtxDesc(s32,s32);extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void TEXGetGXTexObjFromPalette(void*,void*,s32);extern void GXInitTexObjLOD(void*,s32,s32,f32,f32,f32,s32,s32,s32);extern void GXLoadTexObj(void*,s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern u16 GXGetTexObjHeight(void*);extern u16 GXGetTexObjWidth(void*);extern void GXBegin(s32,s32,s32);extern volatile f32 DAT_cc008000;
    extern u32 dat_804231b8,dat_804231bc,dat_804231c0,dat_804231c4,dat_804231c8,dat_804231cc,dat_804231d0,dat_804231d4,dat_804231d8,dat_804231dc,dat_804231e0,dat_804231e4,dat_804231e8,dat_804231ec,dat_804231f0,dat_804231f4;
    extern f32 float_288_80423440, float_285_80423498;
    extern f32 float_144_8042349c, float_neg144_804234a0;
    extern f32 float_neg285_8042348c, float_160_80423490, float_neg160_80423494;
    extern f32 float_18_80423484, float_570_80423488, float_320_80423344;
    u32 texObj[8];f32 mtx[3][4];void* cam;void* file;void* tpl;f32 w,h;
    switch(type) {
    case 0: {u32 c1,c2,c3;c1=dat_804231b8;GXSetTevColor(1,&c1);c2=dat_804231bc;GXSetTevColor(2,&c2);c3=dat_804231c0;GXSetTevColor(3,&c3);break;}
    case 1: {u32 c1,c2,c3;c1=dat_804231c4;GXSetTevColor(1,&c1);c2=dat_804231c8;GXSetTevColor(2,&c2);c3=dat_804231cc;GXSetTevColor(3,&c3);break;}
    case 2: {u32 c1,c2,c3;c1=dat_804231d0;GXSetTevColor(1,&c1);c2=dat_804231d4;GXSetTevColor(2,&c2);c3=dat_804231d8;GXSetTevColor(3,&c3);break;}
    case 3: {u32 c1,c2,c3;c1=dat_804231dc;GXSetTevColor(1,&c1);c2=dat_804231e0;GXSetTevColor(2,&c2);c3=dat_804231e4;GXSetTevColor(3,&c3);break;}
    case 4: {u32 c1,c2,c3;c1=dat_804231e8;GXSetTevColor(1,&c1);c2=dat_804231ec;GXSetTevColor(2,&c2);c3=dat_804231f0;GXSetTevColor(3,&c3);break;}
    }
    GXSetBlendMode(0,1,0,0);GXSetZCompLoc(0);GXSetAlphaCompare(6,0x80,1,0,0);GXSetZMode(0,7,0);{u32 fog=dat_804231f4;GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&fog);}GXSetNumChans(0);GXSetChanCtrl(4,0,0,0,0,0,2);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,15,15,15,2);GXSetTevAlphaIn(0,7,7,7,4);GXSetTevSwapMode(0,0,0);
    PSMTXTrans(mtx,x,y+18.0f,0.0f);cam=camGetPtr(8);PSMTXConcat((void*)((s32)cam+0x11C),mtx,mtx);GXLoadPosMtxImm(mtx,0);GXSetCurrentMtx(0);GXSetCullMode(0);GXClearVtxDesc();GXSetVtxDesc(9,1);GXSetVtxDesc(13,1);GXSetVtxAttrFmt(0,9,1,4,0);GXSetVtxAttrFmt(0,13,1,4,0);
    file=*(void**)((s32)pWin+0x28);tpl=**(void***)((s32)file+0xA0);TEXGetGXTexObjFromPalette(tpl,texObj,7);GXInitTexObjLOD(texObj,0,0,0.0f,0.0f,0.0f,0,0,0);GXLoadTexObj(texObj,0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);h=(f32)GXGetTexObjHeight(texObj);w=(f32)GXGetTexObjWidth(texObj);PSMTXScale(mtx,570.0f/w,320.0f/h,1.0f);GXLoadTexMtxImm(mtx,0x1E,1);
    GXBegin(0x80,0,4);*(volatile f32*)0xCC008000=-285.0f;*(volatile f32*)0xCC008000=160.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=-285.0f;*(volatile f32*)0xCC008000=-160.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=-160.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=160.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=0.0f;
    GXBegin(0x80,0,4);*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=160.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=-160.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=float_285_80423498;*(volatile f32*)0xCC008000=-160.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=float_285_80423498;*(volatile f32*)0xCC008000=160.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=0.0f;
    TEXGetGXTexObjFromPalette(tpl,texObj,6);GXInitTexObjLOD(texObj,0,0,0.0f,0.0f,0.0f,0,0,0);GXLoadTexObj(texObj,0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);h=(f32)GXGetTexObjHeight(texObj);w=(f32)GXGetTexObjWidth(texObj);PSMTXScale(mtx,570.0f/w,float_288_80423440/h,1.0f);GXLoadTexMtxImm(mtx,0x1E,1);
    GXSetTevColorIn(0,4,6,8,15);GXSetTevAlphaIn(0,7,7,7,3);
    GXBegin(0x80,0,4);*(volatile f32*)0xCC008000=-285.0f;*(volatile f32*)0xCC008000=float_144_8042349c;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=-285.0f;*(volatile f32*)0xCC008000=float_neg144_804234a0;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=float_285_80423498;*(volatile f32*)0xCC008000=float_neg144_804234a0;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=float_285_80423498;*(volatile f32*)0xCC008000=float_144_8042349c;*(volatile f32*)0xCC008000=0.0f;*(volatile f32*)0xCC008000=1.0f;*(volatile f32*)0xCC008000=0.0f;
}

#pragma optimize_for_size off
void winMsgMain(void* pWin) {
    extern f32 intplGetValue(s32 type, s32 time, s32 duration, f32 start, f32 end);
    extern u32 keyGetDirTrg(s32 controller);
    extern f32 float_neg800_8042347c;
    extern f32 float_neg170_80423480;
    s32 duration;
    s32 scaled;
    s32 timer;
    s32 line;
    s32 max;

    switch (*(s32*)((s32)pWin + 0x130)) {
        case 0: {
            f32 x = float_neg800_8042347c;
            f32 y = float_neg170_80423480;
            *(f32*)((s32)pWin + 0x128) = x;
            *(f32*)((s32)pWin + 0x12C) = y;
            *(s32*)((s32)pWin + 0x134) = 0;
            (*(s32*)((s32)pWin + 0x130))++;
            break;
        }
        case 1:
            timer = *(s32*)((s32)pWin + 0x134);
            (*(s32*)((s32)pWin + 0x134))++;
            scaled = *(s32*)((s32)gp + 4) << 4;
            duration = scaled / 60;
            *(f32*)((s32)pWin + 0x128) = intplGetValue(0xB, timer, duration, float_neg800_8042347c, float_0_80423338);
            scaled = *(s32*)((s32)gp + 4) << 4;
            duration = scaled / 60;
            if (*(s32*)((s32)pWin + 0x134) > duration) {
                (*(s32*)((s32)pWin + 0x130))++;
            }
            break;
        case 2:
            if (*(s32*)((s32)pWin + 0x20C) != 0xCA) {
                if ((keyGetDirTrg(0) & 0x100) != 0) {
                    *(s32*)((s32)pWin + 0x148) -= 2;
                    if (*(s32*)((s32)pWin + 0x148) < 0) {
                        *(s32*)((s32)pWin + 0x148) = 0;
                    } else {
                        psndSFXOn(0x20006);
                    }
                } else if ((keyGetDirTrg(0) & 0x200) != 0) {
                    line = *(s32*)((s32)pWin + 0x148);
                    max = *(s32*)((s32)pWin + 0x14C);
                    if (line / 2 < ((max + 1) / 2) - 1) {
                        *(s32*)((s32)pWin + 0x148) = line + 2;
                        psndSFXOn(0x20006);
                    }
                }
            }
            break;
        case 10:
            *(s32*)((s32)pWin + 0x134) = 0;
            (*(s32*)((s32)pWin + 0x130))++;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 12:
            break;
        case 11:
            timer = *(s32*)((s32)pWin + 0x134);
            (*(s32*)((s32)pWin + 0x134))++;
            scaled = *(s32*)((s32)gp + 4) << 4;
            duration = scaled / 60;
            *(f32*)((s32)pWin + 0x128) = intplGetValue(0xB, timer, duration, float_0_80423338, float_neg800_8042347c);
            scaled = *(s32*)((s32)gp + 4) << 4;
            duration = scaled / 60;
            if (*(s32*)((s32)pWin + 0x134) > duration) {
                (*(s32*)((s32)pWin + 0x130))++;
            }
            break;
    }
}
#pragma optimize_for_size reset

void winMsgDisp(double x, double y, void* pWin) {
    extern void winNameGX(f32, f32, f32, f32, void*, s32);
    extern u16 FontGetMessageWidth(char*);
    extern void winFontSetEdgeWidth(Vec3*, Vec3*, void*, char*, f32);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern u32 dat_804231f8, dat_80423200;
    f32 px = (f32)x - 250.0f;
    f32 py = (f32)y + 40.0f;
    u32 frameColor = dat_80423200;
    u32 color = dat_804231f8;
    Vec3 pos, scale;
    s32 item = *(s32*)((s32)pWin + 0x138);

    windowDispGX_Waku_col(px, py, 500.0f, 80.0f, 20.0f, 0, &frameColor);
    if (item > 0) {
        void* file = *(void**)((s32)pWin + 0x28);
        winTexInit(**(void***)((s32)file + 0xA0));
        pos.x = px; pos.y = py; pos.z = 0.0f;
        scale.x = 1.0f; scale.y = 1.0f; scale.z = 1.0f;
        winTexSet(0x25, &pos, &scale, &color);
        winIconInit();
        pos.x = px; pos.y = py;
        winIconSet(*(s16*)(itemDataTable + item * 0x28), &pos, &scale, &color);
    }
    if (*(char**)((s32)pWin + 0x140) != 0) {
        f32 offset = item == -2 ? -130.0f : 0.0f;
        char* msg = msgSearch(*(char**)((s32)pWin + 0x140));
        f32 width = (f32)FontGetMessageWidth(msg);
        f32 nameY = (f32)y + 56.0f;
        winNameGX((f32)x - 120.0f + offset, nameY, 240.0f, 32.0f, pWin, 1);
        if (width > 210.0f) width = 210.0f;
        winFontInit();
        pos.x = (f32)x - 120.0f + offset + (240.0f - width) * 0.5f;
        pos.y = nameY - 4.0f; pos.z = 0.0f;
        scale.x = 1.0f; scale.y = 1.0f; scale.z = 1.0f;
        winFontSetEdgeWidth(&pos, &scale, &color, msg, 210.0f);
    }
    if (*(char**)((s32)pWin + 0x13C) != 0) {
        winFontInit();
        GXSetScissor(0, (s32)(240.0f - ((f32)y + 26.0f)), 0x260, 0x38);
        pos.x = px + 20.0f; pos.y = py - 20.0f; pos.z = 0.0f;
        scale.x = 1.0f; scale.y = 1.0f; scale.z = 1.0f;
        winFontSetLabel(&pos, &scale, &color, *(char**)((s32)pWin + 0x13C));
        GXSetScissor(0, 0, 0x260, 0x1E0);
    }
    if (*(s32*)((s32)pWin + 0x20C) != 0xCA &&
        *(s32*)((s32)pWin + 0x14C) > 2) {
        s32 line = *(s32*)((s32)pWin + 0x148);
        s32 maxLines = *(s32*)((s32)pWin + 0x14C);
        winIconInit();
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
        pos.x = (f32)x + 240.0f;
        pos.z = 0.0f;
        if (line == 0) {
            if ((*(u32*)((s32)gp + 0x1C) & 0x1F) < 0x14) {
                pos.y = (f32)y - 24.0f;
                winIconSet(0x1BE, &pos, &scale, &color);
            }
        } else if (line / 2 == (maxLines + 1) / 2 - 1) {
            if ((*(u32*)((s32)gp + 0x1C) & 0x1F) < 0x14) {
                pos.y = (f32)y + 24.0f;
                winIconSet(0x1BD, &pos, &scale, &color);
            }
        } else if ((*(u32*)((s32)gp + 0x1C) & 0x1F) < 0x14) {
                pos.y = (f32)y + 24.0f;
                winIconSet(0x1BD, &pos, &scale, &color);
                pos.y = (f32)y - 24.0f;
                winIconSet(0x1BE, &pos, &scale, &color);
        }
        pos.x = (f32)x + 240.0f;
        pos.y = (f32)y;
        winIconSet(0x78, &pos, &scale, &color);
    }
}


void winMsgEntry(void* pWin, s32 param_2, char* param_3, s32 param_4) {
    u16 width;
    s32 changed = 0;

    if (param_3 != NULL) {
        if (*(char**)((s32)pWin + 0x144) != param_3) {
            changed = 1;
        } else if (strcmp(_tmp_1042, param_3) != 0) {
            changed = 1;
        }

        if (changed != 0) {
            *(s32*)((s32)pWin + 0x138) = param_2;
            *(char**)((s32)pWin + 0x13C) = param_3;
            *(s32*)((s32)pWin + 0x140) = param_4;
            FontGetMessageWidthLine(msgSearch(*(char**)((s32)pWin + 0x13C)), &width);
            *(s32*)((s32)pWin + 0x148) = 0;
            *(s32*)((s32)pWin + 0x14C) = width + 1;
            *(char**)((s32)pWin + 0x144) = param_3;
            strncpy(_tmp_1042, param_3, 0x3C);
        }
    }
}


void winKirinukiGX(double x, double y, double w, double h, void* pWin, s32 style) {
    extern f32 float_8_80423370;
    extern f32 float_16_804233b8;
    extern f32 float_60_80423468;
    extern u32 dat_80423218;
    extern char str_msg_menu_badge_nokor_802f4da8[];
    extern s32 FontGetMessageWidth(char* msg);
    u32 texObj0[8], texObj1[8], texObj2[8], texObj3[8];
    u32 texObj4[8], texObj5[8], texObj6[8], texObj7[8];
    u32 texObjB0[8];
    u32 texObjB1[8];
    u32 texObjB2[8];
    u32 texObjB3[8];
    u32 texObjB4[8];
    u32 texObjB5[8];
    u32 texObjB6[8];
    u32 texObjB7[8];
    u32 texObjB8[8];
    u32 texObjB9[8];
    u32 texObjB10[8];
    u32 texObjB11[8];
    u32 texObjB12[8];
    u32 texObjB13[8];
    f32 texMtx0[3][4], texMtx1[3][4], texMtx2[3][4], texMtx3[3][4];
    f32 texMtx4[3][4], texMtx5[3][4], texMtx6[3][4], texMtx7[3][4];
    f32 texMtxB0[3][4], texMtxB1[3][4], texMtxB2[3][4], texMtxB3[3][4];
    f32 texMtxB4[3][4], texMtxB5[3][4], texMtxB6[3][4], texMtxB7[3][4];
    f32 texMtxB8[3][4], texMtxB9[3][4], texMtxB10[3][4], texMtxB11[3][4];
    f32 texMtxB12[3][4], texMtxB13[3][4];
    u32 fog = dat_80423218;
    f32 fx;
    f32 fy;
    f32 fw;
    f32 fh;
    f32 right;
    f32 bottom;
    f32 innerLeft;
    f32 innerTop;
    f32 innerBottom;
    f32 notchStart;
    f32 cutout;

    GXSetBlendMode(0, 1, 0, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0);
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fog);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevOp(0, 3);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(8) + 0x11C, 0);
    GXSetCurrentMtx(0);
    if (style == 0) {
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj0, 0xB);
        GXInitTexObjLOD(texObj0, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj0, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx0,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj0),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj0), 1.0f);
        GXLoadTexMtxImm(texMtx0, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = (f32)x;
        (*(volatile f32*)0xCC008000) = (f32)y;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = (f32)x;
        (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj1, 9);
        GXInitTexObjLOD(texObj1, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj1, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx1,
                   ((f32)w - float_16_804233b8) / (f32)GXGetTexObjWidth(texObj1),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj1), 1.0f);
        GXLoadTexMtxImm(texMtx1, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w - float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w - float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj2, 0xD);
        GXInitTexObjLOD(texObj2, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj2, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx2,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj2),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj2), 1.0f);
        GXLoadTexMtxImm(texMtx2, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w - float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w - float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w;
        (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w;
        (*(volatile f32*)0xCC008000) = (f32)y;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj3, 10);
        GXInitTexObjLOD(texObj3, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj3, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx3,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj3),
                   ((f32)h - float_16_804233b8) / (f32)GXGetTexObjHeight(texObj3), 1.0f);
        GXLoadTexMtxImm(texMtx3, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = (f32)x;
        (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = (f32)x;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h + float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h + float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj4, 8);
        GXInitTexObjLOD(texObj4, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj4, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx4,
                   ((f32)w - float_8_80423370) / (f32)GXGetTexObjWidth(texObj4),
                   ((f32)h - float_16_804233b8) / (f32)GXGetTexObjHeight(texObj4), 1.0f);
        GXLoadTexMtxImm(texMtx4, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h + float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h + float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w;
        (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj5, 0xC);
        GXInitTexObjLOD(texObj5, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj5, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx5,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj5),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj5), 1.0f);
        GXLoadTexMtxImm(texMtx5, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = (f32)x;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h + float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = (f32)x;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h + float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj6, 8);
        GXInitTexObjLOD(texObj6, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj6, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx6,
                   ((f32)w - float_16_804233b8) / (f32)GXGetTexObjWidth(texObj6),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj6), 1.0f);
        GXLoadTexMtxImm(texMtx6, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h + float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w - float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w - float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h + float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj7, 0xE);
        GXInitTexObjLOD(texObj7, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj7, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx7,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj7),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj7), 1.0f);
        GXLoadTexMtxImm(texMtx7, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w - float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h + float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w - float_8_80423370;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + (f32)w;
        (*(volatile f32*)0xCC008000) = (f32)y - (f32)h + float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
    } else {
        /*
         * Reconstruct the target's complete message-dependent style-1 frame.
         * The lower border is split around the localized badge-count label.
         */
        fx = (f32)x;
        fy = (f32)y;
        fw = (f32)w;
        fh = (f32)h;
        right = fx + fw;
        bottom = fy - fh;
        innerLeft = fx + float_8_80423370;
        innerTop = fy - float_8_80423370;
        innerBottom = bottom + float_8_80423370;
        cutout = float_0p5_8042333c *
                 (f32)(u16)FontGetMessageWidth(
                     msgSearch(str_msg_menu_badge_nokor_802f4da8)) +
                 float_60_80423468;
        notchStart = right - cutout;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB0, 0xB);
        GXInitTexObjLOD(texObjB0, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB0, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB0,
                   (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjB0),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB0),
                   1.0f);
        GXLoadTexMtxImm(texMtxB0, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = fx;
        (*(volatile f32*)0xCC008000) = fy;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = fx;
        (*(volatile f32*)0xCC008000) = innerTop;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = innerTop;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = fy;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB1, 9);
        GXInitTexObjLOD(texObjB1, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB1, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB1,
                   (fw - float_16_804233b8) / (f32)(u16)GXGetTexObjWidth(texObjB1),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB1),
                   1.0f);
        GXLoadTexMtxImm(texMtxB1, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = fy;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = innerTop;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right - float_8_80423370;
        (*(volatile f32*)0xCC008000) = innerTop;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right - float_8_80423370;
        (*(volatile f32*)0xCC008000) = fy;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB2, 0xD);
        GXInitTexObjLOD(texObjB2, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB2, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB2,
                   (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjB2),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB2),
                   1.0f);
        GXLoadTexMtxImm(texMtxB2, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = right - float_8_80423370;
        (*(volatile f32*)0xCC008000) = fy;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = right - float_8_80423370;
        (*(volatile f32*)0xCC008000) = innerTop;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = innerTop;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = fy;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB3, 10);
        GXInitTexObjLOD(texObjB3, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB3, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB3,
                   (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjB3),
                   (fh - float_16_804233b8) / (f32)(u16)GXGetTexObjHeight(texObjB3),
                   1.0f);
        GXLoadTexMtxImm(texMtxB3, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = fx;
        (*(volatile f32*)0xCC008000) = innerTop;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = fx;
        (*(volatile f32*)0xCC008000) = innerBottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = innerBottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = innerTop;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB4, 8);
        GXInitTexObjLOD(texObjB4, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB4, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB4,
                   (fw - float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjB4),
                   (fh - float_16_804233b8) / (f32)(u16)GXGetTexObjHeight(texObjB4),
                   1.0f);
        GXLoadTexMtxImm(texMtxB4, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = innerTop;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = innerBottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = innerBottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = innerTop;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB5, 0xC);
        GXInitTexObjLOD(texObjB5, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB5, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB5,
                   (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjB5),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB5),
                   1.0f);
        GXLoadTexMtxImm(texMtxB5, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = fx;
        (*(volatile f32*)0xCC008000) = innerBottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = fx;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = innerBottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB6, 8);
        GXInitTexObjLOD(texObjB6, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB6, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB6,
                   (fw - float_16_804233b8) / (f32)(u16)GXGetTexObjWidth(texObjB6),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB6),
                   1.0f);
        GXLoadTexMtxImm(texMtxB6, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = innerBottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = innerLeft;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right - float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right - float_8_80423370;
        (*(volatile f32*)0xCC008000) = innerBottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB7, 0xF);
        GXInitTexObjLOD(texObjB7, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB7, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB7,
                   (float_16_804233b8) / (f32)(u16)GXGetTexObjWidth(texObjB7),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB7),
                   1.0f);
        GXLoadTexMtxImm(texMtxB7, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = notchStart;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = notchStart;
        (*(volatile f32*)0xCC008000) = bottom - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB8, 8);
        GXInitTexObjLOD(texObjB8, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB8, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB8,
                   (cutout - float_16_804233b8) / (f32)(u16)GXGetTexObjWidth(texObjB8),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB8),
                   1.0f);
        GXLoadTexMtxImm(texMtxB8, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = bottom - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB9, 10);
        GXInitTexObjLOD(texObjB9, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB9, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB9,
                   (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjB9),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB9),
                   1.0f);
        GXLoadTexMtxImm(texMtxB9, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = notchStart + float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB10, 8);
        GXInitTexObjLOD(texObjB10, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB10, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB10,
                   (cutout - float_16_804233b8) / (f32)(u16)GXGetTexObjWidth(texObjB10),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB10),
                   1.0f);
        GXLoadTexMtxImm(texMtxB10, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = bottom - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB11, 0xC);
        GXInitTexObjLOD(texObjB11, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB11, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB11,
                   (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjB11),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB11),
                   1.0f);
        GXLoadTexMtxImm(texMtxB11, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = notchStart + float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8 - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8 - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB12, 8);
        GXInitTexObjLOD(texObjB12, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB12, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB12,
                   (cutout - float_16_804233b8 - float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjB12),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB12),
                   1.0f);
        GXLoadTexMtxImm(texMtxB12, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = notchStart + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8 - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right - float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8 - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right - float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

        TEXGetGXTexObjFromPalette(
            **(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),
            texObjB13, 0xE);
        GXInitTexObjLOD(texObjB13, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjB13, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxB13,
                   (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjB13),
                   (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjB13),
                   1.0f);
        GXLoadTexMtxImm(texMtxB13, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        (*(volatile f32*)0xCC008000) = right - float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = right - float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8 - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8 - float_8_80423370;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = bottom - float_16_804233b8;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;

    }
}

void winNameGX(double x, double y, double width, double unusedHeight, void* pWin, s32 type) {
    extern u32 dat_8042321c;
    extern f32 float_8_80423370;
    extern f32 float_16_804233b8;
    extern f32 float_24_8042339c;
    extern f32 float_32_80423378;
    extern f32 float_0_80423338, float_1_80423340;
    extern const double double_16_802f4cd8, double_32_802f4d18, double_8_802f4d08, double_24_802f4cc8;

    u32 color;
    u32 tex0[8], tex1[8], tex2[8], tex3[8], tex4[8], tex5[8];
    f32 mtx0[3][4], mtx1[3][4], mtx2[3][4], mtx3[3][4], mtx4[3][4], mtx5[3][4];
    void* camera;
    f32 split, texHeight, texWidth, scaleX, scaleY;
    double sizeX, sizeY;

    (void)unusedHeight;
    GXSetBlendMode(0, 1, 0, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0);
    color = dat_8042321c;
    GXSetFog(0, float_0_80423338, float_0_80423338, float_0_80423338, float_0_80423338, &color);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
camera = camGetPtr(8);
GXLoadPosMtxImm((u8*)camera + 0x11C, 0);
    GXSetCurrentMtx(0);

    if (type == 0) {
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), tex0, 0x13);
        GXInitTexObjLOD(tex0, 1, 1, float_0_80423338, float_0_80423338, float_0_80423338, 0, 0, 0);
        GXLoadTexObj(tex0, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        sizeY = __fabs(double_32_802f4d18);
        sizeX = __fabsf((f32)(width - (double)float_16_804233b8));
        texHeight = (f32)(u16)GXGetTexObjHeight(tex0);
        scaleY = (f32)sizeY / texHeight;
        texWidth = (f32)(u16)GXGetTexObjWidth(tex0);
        scaleX = (f32)sizeX / texWidth;
        PSMTXScale(mtx0, scaleX, scaleY, float_1_80423340);
        GXLoadTexMtxImm(mtx0, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        split = (f32)(x + (double)(f32)(width - (double)float_16_804233b8));
        (*(volatile f32*)0xCC008000)=(f32)x; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338;
        (*(volatile f32*)0xCC008000)=(f32)x; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_32_80423378); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_32_80423378); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_0_80423338;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), tex1, 0x14);
        GXInitTexObjLOD(tex1, 1, 1, float_0_80423338, float_0_80423338, float_0_80423338, 0, 0, 0);
        GXLoadTexObj(tex1, 0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        sizeY = __fabs(double_32_802f4d18); sizeX = __fabs(double_16_802f4cd8);
        texHeight = (f32)(u16)GXGetTexObjHeight(tex1);
        scaleY = (f32)sizeY / texHeight;
        texWidth = (f32)(u16)GXGetTexObjWidth(tex1);
        PSMTXScale(mtx1, 16.0f/texWidth, scaleY, float_1_80423340);
        GXLoadTexMtxImm(mtx1,0x1E,1); GXBegin(0x80,0,4);
        split=(f32)(x+width)-float_16_804233b8;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_32_80423378); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split+float_16_804233b8; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_32_80423378); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split+float_16_804233b8; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_0_80423338;
    } else if (type == 1) {
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), tex2, 0x15);
        GXInitTexObjLOD(tex2,1,1,float_0_80423338,float_0_80423338,float_0_80423338,0,0,0); GXLoadTexObj(tex2,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        sizeY = __fabs(double_32_802f4d18); sizeX = __fabsf((f32)(width-(double)float_16_804233b8));
        texHeight = (f32)(u16)GXGetTexObjHeight(tex2);
        scaleY = (f32)sizeY / texHeight;
        texWidth = (f32)(u16)GXGetTexObjWidth(tex2);
        PSMTXScale(mtx2,(f32)sizeX/texWidth,scaleY,float_1_80423340);
        GXLoadTexMtxImm(mtx2,0x1E,1); GXBegin(0x80,0,4); split=(f32)(x+(double)(f32)(width-(double)float_16_804233b8));
        (*(volatile f32*)0xCC008000)=(f32)x; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338;
        (*(volatile f32*)0xCC008000)=(f32)x; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_32_80423378); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_32_80423378); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_0_80423338;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),tex3,0x16); GXInitTexObjLOD(tex3,1,1,float_0_80423338,float_0_80423338,float_0_80423338,0,0,0); GXLoadTexObj(tex3,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        sizeY = __fabs(double_32_802f4d18); sizeX = __fabs(double_16_802f4cd8);
        texHeight = (f32)(u16)GXGetTexObjHeight(tex3);
        scaleY = (f32)sizeY / texHeight;
        texWidth = (f32)(u16)GXGetTexObjWidth(tex3);
        PSMTXScale(mtx3,(f32)sizeX/texWidth,scaleY,float_1_80423340); GXLoadTexMtxImm(mtx3,0x1E,1); GXBegin(0x80,0,4); split=(f32)(x+width)-float_16_804233b8;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_32_80423378); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split+float_16_804233b8; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_32_80423378); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split+float_16_804233b8; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_0_80423338;
    } else if (type == 2) {
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),tex4,0x78); GXInitTexObjLOD(tex4,1,1,float_0_80423338,float_0_80423338,float_0_80423338,0,0,0); GXLoadTexObj(tex4,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        sizeY = __fabs(double_24_802f4cc8); sizeX = __fabsf((f32)(width-(double)float_8_80423370));
        texHeight = (f32)(u16)GXGetTexObjHeight(tex4);
        scaleY = (f32)sizeY / texHeight;
        texWidth = (f32)(u16)GXGetTexObjWidth(tex4);
        PSMTXScale(mtx4,(f32)sizeX/texWidth,scaleY,float_1_80423340); GXLoadTexMtxImm(mtx4,0x1E,1); GXBegin(0x80,0,4); split=(f32)(x+(double)(f32)(width-(double)float_8_80423370));
        (*(volatile f32*)0xCC008000)=(f32)x; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338;
        (*(volatile f32*)0xCC008000)=(f32)x; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_24_8042339c); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_24_8042339c); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_0_80423338;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0),tex5,0x79); GXInitTexObjLOD(tex5,1,1,float_0_80423338,float_0_80423338,float_0_80423338,0,0,0); GXLoadTexObj(tex5,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        sizeY = __fabs(double_24_802f4cc8); sizeX = __fabs(double_8_802f4d08);
        texHeight = (f32)(u16)GXGetTexObjHeight(tex5);
        scaleY = (f32)sizeY / texHeight;
        texWidth = (f32)(u16)GXGetTexObjWidth(tex5);
        PSMTXScale(mtx5,(f32)sizeX/texWidth,scaleY,float_1_80423340); GXLoadTexMtxImm(mtx5,0x1E,1); GXBegin(0x80,0,4); split=(f32)(x+width)-float_8_80423370;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338;
        (*(volatile f32*)0xCC008000)=split; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_24_8042339c); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split+float_8_80423370; (*(volatile f32*)0xCC008000)=(f32)(y-(double)float_24_8042339c); (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_1_80423340;
        (*(volatile f32*)0xCC008000)=split+float_8_80423370; (*(volatile f32*)0xCC008000)=(f32)y; (*(volatile f32*)0xCC008000)=float_0_80423338; (*(volatile f32*)0xCC008000)=float_1_80423340; (*(volatile f32*)0xCC008000)=float_0_80423338;
    }
}

void winBookGX(double x, double y, void* pWin, s32 page) {
    extern u32 dat_80423220;
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern char str_msg_menu_sort_narabi_802f4da8[];
    extern f32 float_16_804233b8;
    extern f32 float_8_80423370;
    extern f32 float_24_8042339c;
    extern f32 float_32_80423378;
    extern f32 float_48_804233f0;
    extern f32 float_184_8042344c;
    extern f32 float_248_80423424;
    extern f32 float_256_80423438;
    extern f32 float_432_80423448;
    Vec3 pos;
    Vec3 scale;
    u32 texObj[8];
    f32 texMtx[3][4];
    u32 white = 0xFFFFFFFF;
    u32 fog = dat_80423220;
    u32 gray = 0xA0A0A0FF;
    s32 row;
    s32 column;

    GXSetBlendMode(0, 1, 0, 0); GXSetZCompLoc(0); GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0); GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fog);
    GXSetNumChans(0); GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1); GXSetTevOp(0, 3); GXSetCullMode(0);
    GXClearVtxDesc(); GXSetVtxDesc(9, 1); GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0); GXSetVtxAttrFmt(0, 13, 1, 4, 0);
GXLoadPosMtxImm((u8*)camGetPtr(8) + 0x11C, 0); GXSetCurrentMtx(0);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;
    pos.z = 0.0f;
    {
        f32 pageOffset = page == 0 ? 0.0f : float_32_80423378;
        f32 height = float_256_80423438 - pageOffset;
        f32 left = (f32)x + float_8_80423370;
        f32 top = (f32)y - float_8_80423370;
        f32 bottom = top - height;
        f32 right = left + float_432_80423448;

        TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                                  texObj, 0x33);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_432_80423448 / (f32)GXGetTexObjWidth(texObj),
                   height / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = left;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = left;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = right;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
    }
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                              texObj, 0x28);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx,
               float_16_804233b8 / (f32)GXGetTexObjWidth(texObj),
               float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    (*(volatile f32*)0xCC008000) = (f32)x;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = (f32)x;
    (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8;
    (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x2C);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_184_8042344c / (f32)GXGetTexObjWidth(texObj),
               float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8;
    (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c;
    (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x26);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_48_804233f0 / (f32)GXGetTexObjWidth(texObj),
               float_16_804233b8 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c;
    (*(volatile f32*)0xCC008000) = (f32)y - float_16_804233b8;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0;
    (*(volatile f32*)0xCC008000) = (f32)y - float_16_804233b8;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x2D);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_184_8042344c / (f32)GXGetTexObjWidth(texObj),
               float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0;
    (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c;
    (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x2A);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_16_804233b8 / (f32)GXGetTexObjWidth(texObj),
               float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c;
    (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c + float_16_804233b8;
    (*(volatile f32*)0xCC008000) = (f32)y - float_8_80423370;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c + float_16_804233b8;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    {
        f32 pageOffset = page == 0 ? 0.0f : float_32_80423378;
        f32 height = float_256_80423438 - pageOffset;
        f32 top = (f32)y - float_8_80423370;
        f32 bottom = top - height;
        TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x30);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx, float_8_80423370 / (f32)GXGetTexObjWidth(texObj),
                   height / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = (f32)x;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = (f32)x;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
    }
    {
        f32 pageOffset = page == 0 ? 0.0f : float_32_80423378;
        f32 height = float_248_80423424 - pageOffset;
        f32 left = (f32)x + float_16_804233b8 + float_184_8042344c + float_8_80423370;
        f32 top = (f32)y - float_16_804233b8;
        f32 bottom = top - height;
        TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x32);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx, float_16_804233b8 / (f32)GXGetTexObjWidth(texObj),
                   height / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = left;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = left;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
    }
    {
        f32 pageOffset = page == 0 ? 0.0f : float_32_80423378;
        f32 height = float_256_80423438 - pageOffset;
        f32 left = (f32)x + float_432_80423448 + float_8_80423370;
        f32 top = (f32)y - float_8_80423370;
        f32 bottom = top - height;
        TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x31);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx, float_8_80423370 / (f32)GXGetTexObjWidth(texObj),
                   height / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = left;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = left;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_8_80423370;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_8_80423370;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
    }
    {
        f32 pageOffset = page == 0 ? 0.0f : float_32_80423378;
        f32 top = (f32)y - float_8_80423370 - float_256_80423438 + pageOffset;
        f32 bottom = top - float_16_804233b8;
        TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x29);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx, float_16_804233b8 / (f32)GXGetTexObjWidth(texObj),
                   float_16_804233b8 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = (f32)x;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = (f32)x;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = (f32)x + float_16_804233b8;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
    }
    {
        f32 pageOffset = page == 0 ? 0.0f : float_32_80423378;
        f32 top = (f32)y - float_8_80423370 - float_256_80423438 + pageOffset;
        f32 bottom = top - float_16_804233b8;
        f32 left = (f32)x + float_16_804233b8;
        TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x2E);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx, float_184_8042344c / (f32)GXGetTexObjWidth(texObj),
                   float_16_804233b8 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = left;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = left;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_184_8042344c;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_184_8042344c;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
    }
    {
        f32 pageOffset = page == 0 ? 0.0f : float_32_80423378;
        f32 top = (f32)y - float_8_80423370 - float_256_80423438 + pageOffset;
        f32 bottom = top - float_24_8042339c;
        f32 left = (f32)x + float_16_804233b8 + float_184_8042344c;
        TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x27);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx, float_48_804233f0 / (f32)GXGetTexObjWidth(texObj),
                   float_24_8042339c / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = left;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = left;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_48_804233f0;
        (*(volatile f32*)0xCC008000) = bottom;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_48_804233f0;
        (*(volatile f32*)0xCC008000) = top;
        (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = 0.0f;
    }
    {
        f32 pageOffset = page == 0 ? 0.0f : float_32_80423378;
        f32 top = (f32)y - float_8_80423370 - float_256_80423438 + pageOffset;
        f32 bottom = top - float_16_804233b8;
        f32 left = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0;
        TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x2F);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx, float_184_8042344c / (f32)GXGetTexObjWidth(texObj),
                   float_16_804233b8 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = left; (*(volatile f32*)0xCC008000) = top; (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f; (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = left; (*(volatile f32*)0xCC008000) = bottom; (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f; (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_184_8042344c; (*(volatile f32*)0xCC008000) = bottom; (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f; (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_184_8042344c; (*(volatile f32*)0xCC008000) = top; (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f; (*(volatile f32*)0xCC008000) = 0.0f;
    }
    {
        f32 pageOffset = page == 0 ? 0.0f : float_32_80423378;
        f32 top = (f32)y - float_8_80423370 - float_256_80423438 + pageOffset;
        f32 bottom = top - float_16_804233b8;
        f32 left = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c;
        TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x2B);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx, float_16_804233b8 / (f32)GXGetTexObjWidth(texObj),
                   float_16_804233b8 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        (*(volatile f32*)0xCC008000) = left; (*(volatile f32*)0xCC008000) = top; (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f; (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = left; (*(volatile f32*)0xCC008000) = bottom; (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 0.0f; (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_16_804233b8; (*(volatile f32*)0xCC008000) = bottom; (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f; (*(volatile f32*)0xCC008000) = 1.0f;
        (*(volatile f32*)0xCC008000) = left + float_16_804233b8; (*(volatile f32*)0xCC008000) = top; (*(volatile f32*)0xCC008000) = 0.0f;
        (*(volatile f32*)0xCC008000) = 1.0f; (*(volatile f32*)0xCC008000) = 0.0f;
    }
    for (row = 0; row < 4; row++) {
        for (column = 0; column < 4; column++) {
            pos.x = (f32)x + column * 56.0f;
            pos.y = (f32)y - row * 56.0f;
            winTexSet(0x27 + row * 4 + column, &pos, &scale,
                      page == 0 ? &white : &gray);
        }
    }
    winFontInit();
    pos.x = (f32)x + 80.0f;
    pos.y = (f32)y + 24.0f;
    scale.x = 0.8f;
    scale.y = 0.8f;
    winFontSetLabel(&pos, &scale, &white,
                    msgSearch(str_msg_menu_sort_narabi_802f4da8));
}

#pragma optimize_for_size off
void winHalfBookGX(double x,double y,void* file,s32 side) {
    extern f32 float_0_80423338;
    extern f32 float_16_804233b8;
    extern f32 float_1_80423340;
    extern f32 float_200_8042341c;
    extern f32 float_240_80423380;
    extern f32 float_248_80423424;
    extern f32 float_24_8042339c;
    extern f32 float_256_80423438;
    extern f32 float_272_80423428;
    extern f32 float_288_80423440;
    extern f32 float_320_80423344;
    extern f32 float_328_80423444;
    extern f32 float_32_80423378;
    extern f32 float_40_804233bc;
    extern f32 float_48_804233f0;
    extern f32 float_8_80423370;
    extern f32 float_neg16_8042343c;
    extern f32 float_neg8_8042342c;
    extern const double double_16_802f4cd8;
    extern const double double_240_802f4d60;
    extern const double double_248_802f4d58;
    extern const double double_24_802f4cc8;
    extern const double double_320_802f4d70;
    extern const double double_40_802f4cd0;
    extern const double double_48_802f4d30;
    extern const double double_8_802f4d20;
    extern const double double_neg16_802f4d78;
    extern u32 dat_8042322c,dat_80423230,dat_80423234;
    extern double fabs(double);
    extern void GXSetChanMatColor(s32,void*);
    f32 mtx[3][4];
    void* camera;
    u32 fog=dat_8042322c,color0,color1;
    s32 pass;
    f32 fVar1,fVar2,fVar3,fVar4,fVar5,fVar6,fVar7,fVar8,fVar9,fVar10,fVar11,fVar12,fVar13,fVar14,fVar15;
    double dVar19,dVar20;
    f32 local_200,local_204,local_208,local_20c,local_210,local_214,local_218,local_21c,local_220,local_224,local_228,local_22c,local_230,local_234,local_238;
    double local_168,local_188,local_198,local_1a8,local_1b8,local_1d8,local_1e8,local_1f0,local_1f8;
    u32 texObj0[8];
    f32 texMtx0[3][4];
    u32 texObj1[8];
    f32 texMtx1[3][4];
    u32 texObj2[8];
    f32 texMtx2[3][4];
    u32 texObj3[8];
    f32 texMtx3[3][4];
    u32 texObj4[8];
    f32 texMtx4[3][4];
    u32 texObj5[8];
    f32 texMtx5[3][4];
    u32 texObj6[8];
    f32 texMtx6[3][4];
    u32 texObj7[8];
    f32 texMtx7[3][4];
    u32 texObj8[8];
    f32 texMtx8[3][4];
    u32 texObj9[8];
    f32 texMtx9[3][4];
    u32 texObj10[8];
    f32 texMtx10[3][4];
    u32 texObj11[8];
    f32 texMtx11[3][4];
    u32 texObj12[8];
    f32 texMtx12[3][4];
    u32 texObj13[8];
    f32 texMtx13[3][4];
    u32 texObj14[8];
    f32 texMtx14[3][4];
    u32 texObj15[8];
    f32 texMtx15[3][4];
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fog);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9,1); GXSetVtxDesc(13,1);
    GXSetVtxAttrFmt(0,9,1,4,0); GXSetVtxAttrFmt(0,13,1,4,0);
  dVar19 = (double)float_16_804233b8;
  dVar20 = (double)float_8_80423370;
  fVar1 = (f32)(dVar19 + x);
  fVar9 = (f32)(y - dVar20);
  fVar10 = (f32)(y - dVar19);
  fVar14 = fVar1 + float_248_80423424;
  fVar2 = (f32)((double)float_32_80423378 + x);
  fVar11 = (f32)(y - (double)float_256_80423438);
  local_22c = (f32)(dVar20 + (double)fVar14);
  local_1f8 = fabs(double_24_802f4cc8);
  fVar3 = (f32)((double)float_320_80423344 + x);
  local_230 = (f32)((double)float_272_80423428 + x);
  fVar4 = (f32)(dVar20 + x);
  fVar5 = (f32)((double)fVar14 + (double)float_48_804233f0);
  local_234 = (f32)((double)float_288_80423440 + x);
  local_200 = (f32)((double)fVar4 + (double)float_320_80423344);
  local_1e8 = (double)(f32)(y - (double)float_200_8042341c);
  local_238 = (f32)((double)float_328_80423444 + x);
  local_1f0 = fabs(double_48_802f4d30);
  fVar15 = fVar9 - float_248_80423424;
  fVar12 = (f32)((double)fVar10 - (double)float_240_80423380);
  local_214 = (f32)((double)fVar2 + (double)float_240_80423380);
  fVar13 = (f32)((double)fVar11 - dVar20);
  local_204 = fVar5 + float_24_8042339c;
  fVar6 = (f32)fabs(double_248_802f4d58);
  fVar7 = (f32)fabs(double_8_802f4d20);
  local_218 = (f32)((double)local_230 + dVar19);
  fVar8 = (f32)fabs(double_16_802f4cd8);
  local_208 = (f32)((double)local_22c + dVar19);
  local_21c = (f32)((double)local_234 + dVar19);
  local_220 = (f32)((double)fVar3 + (double)float_neg16_8042343c);
  local_20c = (f32)(local_1e8 - (double)float_40_804233bc);
  local_224 = (f32)((double)fVar3 + dVar19);
  local_210 = (f32)((double)fVar2 + (double)float_neg16_8042343c);
  local_228 = (f32)((double)local_238 + dVar20);
  local_1d8 = (double)(f32)fabs(double_320_802f4d70);
  local_1b8 = (double)(f32)local_1f0;
  local_1a8 = (double)(f32)local_1f8;
  local_198 = (double)(f32)fabs(double_240_802f4d60);
  local_188 = (double)(f32)fabs(double_40_802f4cd0);
  local_168 = (double)(f32)fabs(double_neg16_802f4d78);
    for (pass = 0; pass < 2; pass++) {
        if (pass == 0) {
            color0=dat_80423230;
            GXSetChanMatColor(4, &color0);
            GXSetBlendMode(1, 4, 5, 0);
            GXSetZCompLoc(1);
            GXSetAlphaCompare(7, 0, 0, 7, 0);
            GXSetZMode(0, 7, 0);
            PSMTXTrans(mtx, 8.0f, float_neg8_8042342c, 0.0f);
            camera = camGetPtr(8);
            PSMTXConcat((u8*)camera + 0x11C, mtx, mtx);
            GXLoadPosMtxImm(mtx, 0);
            GXSetCurrentMtx(0);
        } else {
            color1=dat_80423234;
            GXSetChanMatColor(4, &color1);
            GXSetBlendMode(0, 1, 0, 0);
            GXSetZCompLoc(0);
            GXSetAlphaCompare(6, 0x80, 1, 0, 0);
            GXSetZMode(0, 7, 0);
            camera = camGetPtr(8);
            GXLoadPosMtxImm((u8*)camera + 0x11C, 0);
            GXSetCurrentMtx(0);
        }
        TEXGetGXTexObjFromPalette(file,texObj0,0x33);
        GXInitTexObjLOD(texObj0,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj0,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx0,local_1d8/(f32)(u16)GXGetTexObjWidth(texObj0),fVar6/(f32)(u16)GXGetTexObjHeight(texObj0),1.0f);
        GXLoadTexMtxImm(texMtx0,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = fVar4;
    (*(volatile f32*)0xCC008000) = fVar9;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = fVar4;
    (*(volatile f32*)0xCC008000) = fVar15;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_200;
    (*(volatile f32*)0xCC008000) = fVar15;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_200;
    (*(volatile f32*)0xCC008000) = fVar9;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj1,0x28);
        GXInitTexObjLOD(texObj1,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj1,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx1,fVar8/(f32)(u16)GXGetTexObjWidth(texObj1),fVar7/(f32)(u16)GXGetTexObjHeight(texObj1),1.0f);
        GXLoadTexMtxImm(texMtx1,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = (f32)x;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = (f32)x;
    (*(volatile f32*)0xCC008000) = fVar9;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar1;
    (*(volatile f32*)0xCC008000) = fVar9;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar1;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj2,0x35);
        GXInitTexObjLOD(texObj2,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj2,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx2,fVar6/(f32)(u16)GXGetTexObjWidth(texObj2),fVar7/(f32)(u16)GXGetTexObjHeight(texObj2),1.0f);
        GXLoadTexMtxImm(texMtx2,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = fVar1;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = fVar1;
    (*(volatile f32*)0xCC008000) = fVar9;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar14;
    (*(volatile f32*)0xCC008000) = fVar9;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar14;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj3,0x26);
        GXInitTexObjLOD(texObj3,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj3,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx3,local_1b8/(f32)(u16)GXGetTexObjWidth(texObj3),fVar8/(f32)(u16)GXGetTexObjHeight(texObj3),1.0f);
        GXLoadTexMtxImm(texMtx3,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = fVar14;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = fVar14;
    (*(volatile f32*)0xCC008000) = fVar10;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar5;
    (*(volatile f32*)0xCC008000) = fVar10;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar5;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj4,0x3b);
        GXInitTexObjLOD(texObj4,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj4,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx4,local_1a8/(f32)(u16)GXGetTexObjWidth(texObj4),fVar8/(f32)(u16)GXGetTexObjHeight(texObj4),1.0f);
        GXLoadTexMtxImm(texMtx4,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = fVar5;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = fVar5;
    (*(volatile f32*)0xCC008000) = fVar10;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_204;
    (*(volatile f32*)0xCC008000) = fVar10;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_204;
    (*(volatile f32*)0xCC008000) = (f32)y;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj5,0x30);
        GXInitTexObjLOD(texObj5,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj5,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx5,fVar7/(f32)(u16)GXGetTexObjWidth(texObj5),fVar6/(f32)(u16)GXGetTexObjHeight(texObj5),1.0f);
        GXLoadTexMtxImm(texMtx5,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = (f32)x;
    (*(volatile f32*)0xCC008000) = fVar9;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = (f32)x;
    (*(volatile f32*)0xCC008000) = fVar15;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar4;
    (*(volatile f32*)0xCC008000) = fVar15;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar4;
    (*(volatile f32*)0xCC008000) = fVar9;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj6,0x32);
        GXInitTexObjLOD(texObj6,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj6,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx6,fVar8/(f32)(u16)GXGetTexObjWidth(texObj6),local_198/(f32)(u16)GXGetTexObjHeight(texObj6),1.0f);
        GXLoadTexMtxImm(texMtx6,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = local_22c;
    (*(volatile f32*)0xCC008000) = fVar10;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = local_22c;
    (*(volatile f32*)0xCC008000) = fVar12;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_208;
    (*(volatile f32*)0xCC008000) = fVar12;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_208;
    (*(volatile f32*)0xCC008000) = fVar10;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj7,0x36);
        GXInitTexObjLOD(texObj7,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj7,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx7,fVar6/(f32)(u16)GXGetTexObjWidth(texObj7),local_188/(f32)(u16)GXGetTexObjHeight(texObj7),1.0f);
        GXLoadTexMtxImm(texMtx7,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = fVar1;
    (*(volatile f32*)0xCC008000) = (f32)local_1e8;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = fVar1;
    (*(volatile f32*)0xCC008000) = local_20c;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar14;
    (*(volatile f32*)0xCC008000) = local_20c;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar14;
    (*(volatile f32*)0xCC008000) = (f32)local_1e8;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj8,0x3a);
        GXInitTexObjLOD(texObj8,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj8,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx8,fVar8/(f32)(u16)GXGetTexObjWidth(texObj8),fVar7/(f32)(u16)GXGetTexObjHeight(texObj8),1.0f);
        GXLoadTexMtxImm(texMtx8,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = (f32)x;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = (f32)x;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar1;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = fVar1;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj9,0x38);
        GXInitTexObjLOD(texObj9,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj9,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx9,fVar8/(f32)(u16)GXGetTexObjWidth(texObj9),fVar7/(f32)(u16)GXGetTexObjHeight(texObj9),1.0f);
        GXLoadTexMtxImm(texMtx9,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = fVar2;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = fVar2;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_210;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_210;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj10,0x38);
        GXInitTexObjLOD(texObj10,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj10,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx10,local_198/(f32)(u16)GXGetTexObjWidth(texObj10),fVar7/(f32)(u16)GXGetTexObjHeight(texObj10),1.0f);
        GXLoadTexMtxImm(texMtx10,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = fVar2;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = fVar2;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_214;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_214;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj11,0x39);
        GXInitTexObjLOD(texObj11,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj11,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx11,fVar8/(f32)(u16)GXGetTexObjWidth(texObj11),fVar7/(f32)(u16)GXGetTexObjHeight(texObj11),1.0f);
        GXLoadTexMtxImm(texMtx11,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = local_230;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = local_230;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_218;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_218;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj12,0x38);
        GXInitTexObjLOD(texObj12,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj12,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx12,fVar8/(f32)(u16)GXGetTexObjWidth(texObj12),fVar7/(f32)(u16)GXGetTexObjHeight(texObj12),1.0f);
        GXLoadTexMtxImm(texMtx12,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = local_234;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = local_234;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_21c;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_21c;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj13,0x38);
        GXInitTexObjLOD(texObj13,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj13,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx13,fVar8/(f32)(u16)GXGetTexObjWidth(texObj13),fVar7/(f32)(u16)GXGetTexObjHeight(texObj13),1.0f);
        GXLoadTexMtxImm(texMtx13,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = fVar3;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = fVar3;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_220;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_220;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj14,0x38);
        GXInitTexObjLOD(texObj14,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj14,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx14,fVar8/(f32)(u16)GXGetTexObjWidth(texObj14),fVar7/(f32)(u16)GXGetTexObjHeight(texObj14),1.0f);
        GXLoadTexMtxImm(texMtx14,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = fVar3;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = fVar3;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_224;
    (*(volatile f32*)0xCC008000) = fVar13;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_224;
    (*(volatile f32*)0xCC008000) = fVar11;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
        TEXGetGXTexObjFromPalette(file,texObj15,0x3c);
        GXInitTexObjLOD(texObj15,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj15,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(texMtx15,fVar7/(f32)(u16)GXGetTexObjWidth(texObj15),local_198/(f32)(u16)GXGetTexObjHeight(texObj15),1.0f);
        GXLoadTexMtxImm(texMtx15,0x1E,1);
        GXBegin(0x80,0,4);
    (*(volatile f32*)0xCC008000) = local_238;
    (*(volatile f32*)0xCC008000) = fVar10;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = local_238;
    (*(volatile f32*)0xCC008000) = fVar12;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_228;
    (*(volatile f32*)0xCC008000) = fVar12;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = local_228;
    (*(volatile f32*)0xCC008000) = fVar10;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    (*(volatile f32*)0xCC008000) = float_1_80423340;
    (*(volatile f32*)0xCC008000) = float_0_80423338;
    }
}
#pragma optimize_for_size reset

void winMailGX(double x, double y, void* pWin) {
    extern u32 dat_80423238;
    extern f32 float_8_80423370;
    extern f32 float_16_804233b8;
    extern f32 float_24_8042339c;
    extern f32 float_32_80423378;
    extern f32 float_40_804233bc;
    extern f32 float_72_80423358;
    extern f32 float_200_8042341c;
    extern f32 float_240_80423380;
    extern f32 float_248_80423424;
    extern f32 float_272_80423428;
    extern f32 float_344_80423418;
    f32 pos[3];
    f32 scale[3];
    u32 texObj[8];
    f32 texMtx[3][4];
    u32 white = 0xFFFFFFFF;
    u32 fog = dat_80423238;
    s32 page = *(s32*)((s32)pWin + 0x258);
    s32 row;
    volatile u8 stackShape[0x620];

    stackShape[0] = (u8)page;

    GXSetBlendMode(0, 1, 0, 0); GXSetZCompLoc(0); GXSetAlphaCompare(6, 0x80, 1, 0, 0); GXSetZMode(0, 7, 0);
    GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&fog); GXSetNumChans(0); GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2); GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF); GXSetTevOp(0, 3); GXSetCullMode(0);
    GXClearVtxDesc(); GXSetVtxDesc(9, 1); GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0); GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(8) + 0x11C, 0); GXSetCurrentMtx(0);
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x4D);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_32_80423378 / (f32)GXGetTexObjWidth(texObj),
               float_24_8042339c / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    DAT_cc008000 = (f32)x; DAT_cc008000 = (f32)y; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f;
    DAT_cc008000 = (f32)x; DAT_cc008000 = (f32)y - float_24_8042339c; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f; DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_32_80423378;
    DAT_cc008000 = (f32)y - float_24_8042339c; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f; DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_32_80423378;
    DAT_cc008000 = (f32)y; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f; DAT_cc008000 = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 99);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_344_80423418 / (f32)GXGetTexObjWidth(texObj),
               float_24_8042339c / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    DAT_cc008000 = (f32)x + float_32_80423378;
    DAT_cc008000 = (f32)y; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f;
    DAT_cc008000 = (f32)x + float_32_80423378;
    DAT_cc008000 = (f32)y - float_24_8042339c; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f; DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_32_80423378 + float_344_80423418;
    DAT_cc008000 = (f32)y - float_24_8042339c; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f; DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_32_80423378 + float_344_80423418;
    DAT_cc008000 = (f32)y; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f; DAT_cc008000 = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 100);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0); GXSetNumTexGens(1); GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_16_804233b8 / (f32)GXGetTexObjWidth(texObj), float_24_8042339c / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1); GXBegin(0x80, 0, 4);
    DAT_cc008000=(f32)x+float_200_8042341c; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x+float_200_8042341c; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_200_8042341c+float_16_804233b8; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_200_8042341c+float_16_804233b8; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x55);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0); GXSetNumTexGens(1); GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_344_80423418 / (f32)GXGetTexObjWidth(texObj), float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1); GXBegin(0x80, 0, 4);
    DAT_cc008000=(f32)x+float_32_80423378; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x+float_32_80423378; DAT_cc008000=(f32)y-float_24_8042339c-float_8_80423370; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418; DAT_cc008000=(f32)y-float_24_8042339c-float_8_80423370; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x4E);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0); GXSetNumTexGens(1); GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_40_804233bc / (f32)GXGetTexObjWidth(texObj), float_24_8042339c / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1); GXBegin(0x80, 0, 4);
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_40_804233bc; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_40_804233bc; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x51);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0); GXSetNumTexGens(1); GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_16_804233b8 / (f32)GXGetTexObjWidth(texObj), float_248_80423424 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1); GXBegin(0x80, 0, 4);
    DAT_cc008000=(f32)x; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x; DAT_cc008000=(f32)y-float_24_8042339c-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_16_804233b8; DAT_cc008000=(f32)y-float_24_8042339c-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_16_804233b8; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),texObj,0x55); GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(texObj,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(texMtx,float_16_804233b8/(f32)GXGetTexObjWidth(texObj),float_248_80423424/(f32)GXGetTexObjHeight(texObj),1.0f); GXLoadTexMtxImm(texMtx,0x1E,1); GXBegin(0x80,0,4);
    DAT_cc008000=(f32)x+float_16_804233b8; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x+float_16_804233b8; DAT_cc008000=(f32)y-float_24_8042339c-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378; DAT_cc008000=(f32)y-float_24_8042339c-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),texObj,0x55); GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(texObj,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(texMtx,float_16_804233b8/(f32)GXGetTexObjWidth(texObj),float_248_80423424/(f32)GXGetTexObjHeight(texObj),1.0f); GXLoadTexMtxImm(texMtx,0x1E,1); GXBegin(0x80,0,4);
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418; DAT_cc008000=(f32)y-float_24_8042339c-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_16_804233b8; DAT_cc008000=(f32)y-float_24_8042339c-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_16_804233b8; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),texObj,0x51); GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(texObj,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(texMtx,float_16_804233b8/(f32)GXGetTexObjWidth(texObj),float_248_80423424/(f32)GXGetTexObjHeight(texObj),1.0f); GXLoadTexMtxImm(texMtx,0x1E,1); GXBegin(0x80,0,4);
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_16_804233b8; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_16_804233b8; DAT_cc008000=(f32)y-float_24_8042339c-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_32_80423378; DAT_cc008000=(f32)y-float_24_8042339c-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_32_80423378; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),texObj,0x52); GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(texObj,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(texMtx,float_8_80423370/(f32)GXGetTexObjWidth(texObj),float_248_80423424/(f32)GXGetTexObjHeight(texObj),1.0f); GXLoadTexMtxImm(texMtx,0x1E,1); GXBegin(0x80,0,4);
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_32_80423378; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_32_80423378; DAT_cc008000=(f32)y-float_24_8042339c-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_32_80423378+float_8_80423370; DAT_cc008000=(f32)y-float_24_8042339c-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418+float_32_80423378+float_8_80423370; DAT_cc008000=(f32)y-float_24_8042339c; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin+0x28)+0xA0),texObj,0x5C); GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(texObj,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(texMtx,float_344_80423418/(f32)GXGetTexObjWidth(texObj),float_240_80423380/(f32)GXGetTexObjHeight(texObj),1.0f); GXLoadTexMtxImm(texMtx,0x1E,1); GXBegin(0x80,0,4);
    DAT_cc008000=(f32)x+float_32_80423378; DAT_cc008000=(f32)y-float_32_80423378; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x+float_32_80423378; DAT_cc008000=(f32)y-float_272_80423428; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418; DAT_cc008000=(f32)y-float_272_80423428; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418; DAT_cc008000=(f32)y-float_32_80423378; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin+0x28)+0xA0),texObj,0x55); GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(texObj,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(texMtx,float_344_80423418/(f32)GXGetTexObjWidth(texObj),float_24_8042339c/(f32)GXGetTexObjHeight(texObj),1.0f); GXLoadTexMtxImm(texMtx,0x1E,1); GXBegin(0x80,0,4);
    DAT_cc008000=(f32)x+float_32_80423378; DAT_cc008000=(f32)y-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x+float_32_80423378; DAT_cc008000=(f32)y-float_272_80423428; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418; DAT_cc008000=(f32)y-float_272_80423428; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378+float_344_80423418; DAT_cc008000=(f32)y-float_248_80423424; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin+0x28)+0xA0),texObj,0x4F); GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(texObj,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(texMtx,float_32_80423378/(f32)GXGetTexObjWidth(texObj),float_32_80423378/(f32)GXGetTexObjHeight(texObj),1.0f); GXLoadTexMtxImm(texMtx,0x1E,1); GXBegin(0x80,0,4);
    DAT_cc008000=(f32)x; DAT_cc008000=(f32)y-float_272_80423428; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
    DAT_cc008000=(f32)x; DAT_cc008000=(f32)y-float_272_80423428-float_32_80423378; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378; DAT_cc008000=(f32)y-float_272_80423428-float_32_80423378; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
    DAT_cc008000=(f32)x+float_32_80423378; DAT_cc008000=(f32)y-float_272_80423428; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    pos[2] = 0.0f;
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    scale[2] = 1.0f;
    for (row = 0; row < 9; row++) {
        pos[0] = (f32)x;
        pos[1] = (f32)y - row * 32.0f;
        winTexSet(0xB0 + ((page + row) & 7), pos, scale, &white);
        pos[0] += 220.0f;
        winTexSet(0xB8 + ((page + row) & 7), pos, scale, &white);
    }
    pos[0] = (f32)x + 110.0f;
    pos[1] = (f32)y + 24.0f;
    scale[0] = 2.0f;
    scale[1] = 1.2f;
    winTexSet(0xAE, pos, scale, &white);
}

void winHakoGX(double x, double y, void* pWin, s32 type) {
    extern u32 dat_8042323c;
    extern s32 swGet(s32 id);
    extern f32 float_neg400_804233ec;
    extern f32 float_4_8042334c;
    extern f32 float_8_80423370;
    extern f32 float_15_804233fc;
    extern f32 float_24_8042339c;
    extern f32 float_28_804233f4;
    extern f32 float_30_804233d4;
    extern f32 float_48_804233f0;
    extern f32 float_56_80423354;
    extern f32 float_220_80423404;
    extern f32 float_224_804233e0;
    extern f32 float_228_80423410;
    extern f32 float_235_80423400;
    extern f32 float_237_80423414;
    extern f32 float_240_80423380;
    extern f32 float_304_8042337c;
    extern f32 float_380_8042340c;
    extern f32 float_392_804233e4;
    extern f32 float_395_804233f8;
    extern f32 float_400_804233e8;
    extern f32 float_0p7_80423408;
    extern char str_msg_menu_sort_narabi_802f4d38[];
    extern s32 FontGetMessageWidth(char* msg);
    extern void GXSetScissor(u32 x, u32 y, u32 width, u32 height);
    u32 texObj0[8];
    f32 texMtx0[3][4];
    u32 texObj1[8];
    f32 texMtx1[3][4];
    u32 texObj2[8];
    f32 texMtx2[3][4];
    u32 texObj3[8];
    f32 texMtx3[3][4];
    u32 texObj4[8];
    f32 texMtx4[3][4];
    u32 texObj5[8];
    f32 texMtx5[3][4];
    u32 texObj6[8];
    f32 texMtx6[3][4];
    u32 texObj7[8];
    f32 texMtx7[3][4];
    u32 texObj8[8];
    f32 texMtx8[3][4];
    u32 texObj9[8];
    f32 texMtx9[3][4];
    u32 texObj10[8];
    f32 texMtx10[3][4];
    u32 texObj11[8];
    f32 texMtx11[3][4];
    extern const Vec3 vec3_802f4994, vec3_802f49a0;
    extern const u32 dat_80423240;
    Vec3 helperPos0, helperScale0;
    u32 helperColor0;
    extern const Vec3 vec3_802f49ac, vec3_802f49b8;
    extern const u32 dat_80423244;
    Vec3 helperPos1, helperScale1;
    u32 helperColor1;
    extern const Vec3 vec3_802f49c4, vec3_802f49d0;
    extern const u32 dat_80423248;
    Vec3 helperPos2, helperScale2;
    u32 helperColor2;
    extern const Vec3 vec3_802f49dc, vec3_802f49e8;
    extern const u32 dat_8042324c;
    Vec3 helperPos3, helperScale3;
    u32 helperColor3;
    extern const Vec3 vec3_802f49f4, vec3_802f4a00;
    extern const u32 dat_80423250;
    Vec3 helperPos4, helperScale4;
    u32 helperColor4;
    extern const Vec3 vec3_802f4a0c, vec3_802f4a18;
    extern const u32 dat_80423254;
    Vec3 helperPos5, helperScale5;
    u32 helperColor5;
    extern const Vec3 vec3_802f4a24, vec3_802f4a30;
    extern const u32 dat_80423258;
    Vec3 helperPos6, helperScale6;
    u32 helperColor6;
    extern const Vec3 vec3_802f4a3c, vec3_802f4a48;
    extern const u32 dat_8042325c;
    Vec3 helperPos7, helperScale7;
    u32 helperColor7;
    u32 fog = dat_8042323c;
    char* sortText;
    f32 scrollY;
    f32 pageTop;
    f32 pageBottom;
    f32 left;
    f32 right;
    f32 top;
    f32 bottom;
    f32 sortX;
    s32 count;
    s32 pageNum;
    s32 pageCount;
    s32 texBase;
    s32 page;
    s32 row;
    s32 column;
    s32 index;
    s32 texId;
    volatile u8 stackShape[0x170];

    stackShape[0] = (u8)type;

    /*
     * Match the target's ABI/data selection before GX setup.  The two log
     * catalogs share the renderer but use separate page, scroll, count, and
     * item-id arrays.
     */
    if (type == 0) {
        pageNum = *(s32*)((s32)pWin + 0xEA0);
        scrollY = *(f32*)((s32)pWin + 0xEA4);
        count = *(s32*)((s32)pWin + 0xE94);
    } else {
        pageNum = *(s32*)((s32)pWin + 0xFA0);
        scrollY = *(f32*)((s32)pWin + 0xFA4);
        count = *(s32*)((s32)pWin + 0xF94);
    }

    GXSetBlendMode(0, 1, 0, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0);
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fog);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(8) + 0x11C, 0);
    GXSetCurrentMtx(0);
    GXSetScissor((u32)(float_304_8042337c + (f32)x),
                 (u32)(float_240_80423380 - (f32)y),
                 0x190, 0xE4);

    texBase = type * 13;
    pageCount = (count + 27) / 28;

    /*
     * Reconstruct the complete target per-page panel family.  The special
     * catalog pages use a compact 64x64 six-piece panel; ordinary pages use
     * the target 7x4 checker body plus the alternating bottom and right edge
     * pieces.  Each target primitive family keeps its own GXTexObj/Mtx local.
     */
    for (page = 0; page < pageCount; page++) {
        pageTop = (f32)y + scrollY - (f32)(page * 232);
        pageBottom = pageTop - float_224_804233e0;

        if (pageTop <= float_400_804233e8 &&
            pageTop >= float_neg400_804233ec) {
            if (((texBase != 0) && (page == 2)) ||
                ((texBase != 1) && (page == 3))) {
            TEXGetGXTexObjFromPalette(
                **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                texObj0, texBase + 0x7B);
            GXInitTexObjLOD(texObj0, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
            GXLoadTexObj(texObj0, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
            PSMTXScale(texMtx0,
                       (float_56_80423354) / (f32)(u16)GXGetTexObjWidth(texObj0),
                       (float_56_80423354) / (f32)(u16)GXGetTexObjHeight(texObj0),
                       1.0f);
            GXLoadTexMtxImm(texMtx0, 0x1E, 1);
            GXBegin(0x80, 0, 4);

            (*(volatile f32*)0xCC008000) = (f32)x;
            (*(volatile f32*)0xCC008000) = pageTop;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            (*(volatile f32*)0xCC008000) = (f32)x;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354;
            (*(volatile f32*)0xCC008000) = pageTop;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            TEXGetGXTexObjFromPalette(
                **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                texObj1, texBase + 0x7D);
            GXInitTexObjLOD(texObj1, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
            GXLoadTexObj(texObj1, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
            PSMTXScale(texMtx1,
                       (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObj1),
                       (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObj1),
                       1.0f);
            GXLoadTexMtxImm(texMtx1, 0x1E, 1);
            GXBegin(0x80, 0, 4);

            (*(volatile f32*)0xCC008000) = (f32)x;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            (*(volatile f32*)0xCC008000) = (f32)x;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354 - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354 - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            TEXGetGXTexObjFromPalette(
                **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                texObj2, texBase + 0x7E);
            GXInitTexObjLOD(texObj2, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
            GXLoadTexObj(texObj2, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
            PSMTXScale(texMtx2,
                       (float_48_804233f0) / (f32)(u16)GXGetTexObjWidth(texObj2),
                       (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObj2),
                       1.0f);
            GXLoadTexMtxImm(texMtx2, 0x1E, 1);
            GXBegin(0x80, 0, 4);

            (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_8_80423370;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354 - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354 - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            TEXGetGXTexObjFromPalette(
                **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                texObj3, texBase + 0x85);
            GXInitTexObjLOD(texObj3, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
            GXLoadTexObj(texObj3, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
            PSMTXScale(texMtx3,
                       (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObj3),
                       (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObj3),
                       1.0f);
            GXLoadTexMtxImm(texMtx3, 0x1E, 1);
            GXBegin(0x80, 0, 4);

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354;
            (*(volatile f32*)0xCC008000) = pageTop;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354;
            (*(volatile f32*)0xCC008000) = pageTop - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354 + float_8_80423370;
            (*(volatile f32*)0xCC008000) = pageTop - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354 + float_8_80423370;
            (*(volatile f32*)0xCC008000) = pageTop;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            TEXGetGXTexObjFromPalette(
                **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                texObj4, texBase + 0x84);
            GXInitTexObjLOD(texObj4, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
            GXLoadTexObj(texObj4, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
            PSMTXScale(texMtx4,
                       (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObj4),
                       (float_48_804233f0) / (f32)(u16)GXGetTexObjHeight(texObj4),
                       1.0f);
            GXLoadTexMtxImm(texMtx4, 0x1E, 1);
            GXBegin(0x80, 0, 4);

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354;
            (*(volatile f32*)0xCC008000) = pageTop - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354 + float_8_80423370;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354 + float_8_80423370;
            (*(volatile f32*)0xCC008000) = pageTop - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            TEXGetGXTexObjFromPalette(
                **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                texObj5, texBase + 0x81);
            GXInitTexObjLOD(texObj5, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
            GXLoadTexObj(texObj5, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
            PSMTXScale(texMtx5,
                       (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObj5),
                       (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObj5),
                       1.0f);
            GXLoadTexMtxImm(texMtx5, 0x1E, 1);
            GXBegin(0x80, 0, 4);

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354 - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354 + float_8_80423370;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354 - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_56_80423354 + float_8_80423370;
            (*(volatile f32*)0xCC008000) = pageTop - float_56_80423354;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            } else {
                for (column = 0; column < 7; column++) {
                    left = (f32)x + (f32)(column * 56);
                    right = left + float_56_80423354;

                    for (row = 0; row < 4; row++) {
                        top = pageTop - (f32)(row * 56);
                        bottom = top - float_56_80423354;
                        texId = texBase + 0x7B +
                                ((column + row * 7) & 1);
                        TEXGetGXTexObjFromPalette(
                            **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                            texObj6, texId);
                        GXInitTexObjLOD(texObj6, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
                        GXLoadTexObj(texObj6, 0);
                        GXSetNumTexGens(1);
                        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
                        PSMTXScale(texMtx6,
                                   (float_56_80423354) / (f32)(u16)GXGetTexObjWidth(texObj6),
                                   (float_56_80423354) / (f32)(u16)GXGetTexObjHeight(texObj6),
                                   1.0f);
                        GXLoadTexMtxImm(texMtx6, 0x1E, 1);
                        GXBegin(0x80, 0, 4);

                        (*(volatile f32*)0xCC008000) = left;
                        (*(volatile f32*)0xCC008000) = top;
                        (*(volatile f32*)0xCC008000) = 0.0f;
                        (*(volatile f32*)0xCC008000) = 0.0f;
                        (*(volatile f32*)0xCC008000) = 0.0f;

                        (*(volatile f32*)0xCC008000) = left;
                        (*(volatile f32*)0xCC008000) = bottom;
                        (*(volatile f32*)0xCC008000) = 0.0f;
                        (*(volatile f32*)0xCC008000) = 0.0f;
                        (*(volatile f32*)0xCC008000) = 1.0f;

                        (*(volatile f32*)0xCC008000) = right;
                        (*(volatile f32*)0xCC008000) = bottom;
                        (*(volatile f32*)0xCC008000) = 0.0f;
                        (*(volatile f32*)0xCC008000) = 1.0f;
                        (*(volatile f32*)0xCC008000) = 1.0f;

                        (*(volatile f32*)0xCC008000) = right;
                        (*(volatile f32*)0xCC008000) = top;
                        (*(volatile f32*)0xCC008000) = 0.0f;
                        (*(volatile f32*)0xCC008000) = 1.0f;
                        (*(volatile f32*)0xCC008000) = 0.0f;

                    }
                }

                top = pageBottom;
                bottom = top - float_8_80423370;
                for (column = 0; column < 7; column++) {
                    left = (f32)x + (f32)(column * 56);

                    if (column == 0) {
                        texId = texBase + 0x7D;
                    } else if ((column & 1) != 0) {
                        texId = texBase + 0x86;
                    } else {
                        texId = texBase + 0x7F;
                    }
                    TEXGetGXTexObjFromPalette(
                        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                        texObj7, texId);
                    GXInitTexObjLOD(texObj7, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
                    GXLoadTexObj(texObj7, 0);
                    GXSetNumTexGens(1);
                    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
                    PSMTXScale(texMtx7,
                               (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObj7),
                               (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObj7),
                               1.0f);
                    GXLoadTexMtxImm(texMtx7, 0x1E, 1);
                    GXBegin(0x80, 0, 4);

                    (*(volatile f32*)0xCC008000) = left;
                    (*(volatile f32*)0xCC008000) = top;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;

                    (*(volatile f32*)0xCC008000) = left;
                    (*(volatile f32*)0xCC008000) = bottom;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;

                    (*(volatile f32*)0xCC008000) = left + float_8_80423370;
                    (*(volatile f32*)0xCC008000) = bottom;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;

                    (*(volatile f32*)0xCC008000) = left + float_8_80423370;
                    (*(volatile f32*)0xCC008000) = top;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;

                    if ((column & 1) == 0) {
                        texId = texBase + 0x7E;
                    } else {
                        texId = texBase + 0x80;
                    }
                    TEXGetGXTexObjFromPalette(
                        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                        texObj8, texId);
                    GXInitTexObjLOD(texObj8, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
                    GXLoadTexObj(texObj8, 0);
                    GXSetNumTexGens(1);
                    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
                    PSMTXScale(texMtx8,
                               (float_48_804233f0) / (f32)(u16)GXGetTexObjWidth(texObj8),
                               (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObj8),
                               1.0f);
                    GXLoadTexMtxImm(texMtx8, 0x1E, 1);
                    GXBegin(0x80, 0, 4);

                    (*(volatile f32*)0xCC008000) = left + float_8_80423370;
                    (*(volatile f32*)0xCC008000) = top;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;

                    (*(volatile f32*)0xCC008000) = left + float_8_80423370;
                    (*(volatile f32*)0xCC008000) = bottom;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;

                    (*(volatile f32*)0xCC008000) = left + float_56_80423354;
                    (*(volatile f32*)0xCC008000) = bottom;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;

                    (*(volatile f32*)0xCC008000) = left + float_56_80423354;
                    (*(volatile f32*)0xCC008000) = top;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;

                }

                left = (f32)x + float_392_804233e4;
                right = left + float_8_80423370;
                for (row = 0; row < 4; row++) {
                    top = pageTop - (f32)(row * 56);

                    if (row == 0) {
                        texId = texBase + 0x85;
                    } else if ((row & 1) != 0) {
                        texId = texBase + 0x87;
                    } else {
                        texId = texBase + 0x83;
                    }
                    TEXGetGXTexObjFromPalette(
                        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                        texObj9, texId);
                    GXInitTexObjLOD(texObj9, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
                    GXLoadTexObj(texObj9, 0);
                    GXSetNumTexGens(1);
                    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
                    PSMTXScale(texMtx9,
                               (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObj9),
                               (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObj9),
                               1.0f);
                    GXLoadTexMtxImm(texMtx9, 0x1E, 1);
                    GXBegin(0x80, 0, 4);

                    (*(volatile f32*)0xCC008000) = left;
                    (*(volatile f32*)0xCC008000) = top;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;

                    (*(volatile f32*)0xCC008000) = left;
                    (*(volatile f32*)0xCC008000) = top - float_8_80423370;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;

                    (*(volatile f32*)0xCC008000) = right;
                    (*(volatile f32*)0xCC008000) = top - float_8_80423370;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;

                    (*(volatile f32*)0xCC008000) = right;
                    (*(volatile f32*)0xCC008000) = top;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;

                    if ((row & 1) == 0) {
                        texId = texBase + 0x84;
                    } else {
                        texId = texBase + 0x82;
                    }
                    TEXGetGXTexObjFromPalette(
                        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                        texObj10, texId);
                    GXInitTexObjLOD(texObj10, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
                    GXLoadTexObj(texObj10, 0);
                    GXSetNumTexGens(1);
                    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
                    PSMTXScale(texMtx10,
                               (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObj10),
                               (float_48_804233f0) / (f32)(u16)GXGetTexObjHeight(texObj10),
                               1.0f);
                    GXLoadTexMtxImm(texMtx10, 0x1E, 1);
                    GXBegin(0x80, 0, 4);

                    (*(volatile f32*)0xCC008000) = left;
                    (*(volatile f32*)0xCC008000) = top - float_8_80423370;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;

                    (*(volatile f32*)0xCC008000) = left;
                    (*(volatile f32*)0xCC008000) = top - float_56_80423354;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;

                    (*(volatile f32*)0xCC008000) = right;
                    (*(volatile f32*)0xCC008000) = top - float_56_80423354;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;

                    (*(volatile f32*)0xCC008000) = right;
                    (*(volatile f32*)0xCC008000) = top - float_8_80423370;
                    (*(volatile f32*)0xCC008000) = 0.0f;
                    (*(volatile f32*)0xCC008000) = 1.0f;
                    (*(volatile f32*)0xCC008000) = 0.0f;

                }
            TEXGetGXTexObjFromPalette(
                **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                texObj11, texBase + 0x81);
            GXInitTexObjLOD(texObj11, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
            GXLoadTexObj(texObj11, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
            PSMTXScale(texMtx11,
                       (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObj11),
                       (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObj11),
                       1.0f);
            GXLoadTexMtxImm(texMtx11, 0x1E, 1);
            GXBegin(0x80, 0, 4);

            (*(volatile f32*)0xCC008000) = (f32)x + float_392_804233e4;
            (*(volatile f32*)0xCC008000) = pageBottom;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_392_804233e4;
            (*(volatile f32*)0xCC008000) = pageBottom - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_400_804233e8;
            (*(volatile f32*)0xCC008000) = pageBottom - float_8_80423370;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;

            (*(volatile f32*)0xCC008000) = (f32)x + float_400_804233e8;
            (*(volatile f32*)0xCC008000) = pageBottom;
            (*(volatile f32*)0xCC008000) = 0.0f;
            (*(volatile f32*)0xCC008000) = 1.0f;
            (*(volatile f32*)0xCC008000) = 0.0f;

            }
        }
    }

    /*
     * Preserve the retained helper-based badge/recipe contents after the
     * target panel family.  Their positions and filtering already follow the
     * target's 28-entry, 7x4 page layout.
     */

    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    for (page = 0; page < pageCount; page++) {
        f32 baseY = scrollY + (f32)y - (f32)(page * 232);
        if (baseY <= float_400_804233e8 &&
            baseY >= float_neg400_804233ec) {
            for (row = 0; row < 4; row++) {
                for (column = 0; column < 7; column++) {
                    index = page * 28 + row * 7 + column;
                    if (index < count &&
                        swGet(*(s16*)((s32)pWin +
                                     (type == 0 ? 0xEBC : 0xFBC) +
                                     index * 2) +
                              (type == 0 ? 0x80 : 0x41)) == 0) {
    helperPos0 = vec3_802f4994;
    helperScale0 = vec3_802f49a0;
    helperColor0 = dat_80423240;
                        helperPos0.x = float_4_8042334c +
                                 float_24_8042339c +
                                 (f32)x +
                                 (f32)(column * 56);
                        helperPos0.y = baseY -
                                 (f32)(row * 56) -
                                 float_28_804233f4;
                        winTexSet(0x97, &helperPos0, &helperScale0, &helperColor0);
                    }
                }
            }
        }
    }

    winIconInit();
    for (page = 0; page < pageCount; page++) {
        f32 baseY = scrollY + (f32)y - (f32)(page * 232);
        if (baseY <= float_400_804233e8 &&
            baseY >= float_neg400_804233ec) {
            for (row = 0; row < 4; row++) {
                for (column = 0; column < 7; column++) {
                    s16 itemId;
                    s32 iconId;

                    index = page * 28 + row * 7 + column;
                    if (index < count) {
                        itemId = *(s16*)((s32)pWin +
                                         (type == 0 ? 0xEBC : 0xFBC) +
                                         index * 2);
                        if (swGet(itemId +
                                  (type == 0 ? 0x80 : 0x41)) != 0) {
                            iconId = *(s16*)(itemDataTable +
                                             (itemId +
                                              (type == 0 ? 0xF0 : 0xB3)) *
                                                 0x28);
    helperPos1 = vec3_802f49ac;
    helperScale1 = vec3_802f49b8;
    helperColor1 = dat_80423244;
                            helperPos1.x = float_4_8042334c +
                                     float_24_8042339c +
                                     (f32)x +
                                     (f32)(column * 56);
                            helperPos1.y = baseY -
                                     (f32)(row * 56) -
                                     float_28_804233f4;
                            winIconSet(iconId, &helperPos1, &helperScale1, &helperColor1);
                        }
                    }
                }
            }
        }
    }

    GXSetScissor(0, 0, 0x260, 0x1E0);

    if (pageNum > 0) {
        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    helperPos2 = vec3_802f49c4;
    helperScale2 = vec3_802f49d0;
    helperColor2 = dat_80423248;
        helperPos2.x = (f32)x + float_395_804233f8;
        helperPos2.y = (f32)y + float_15_804233fc;
        winTexSet(0x17, &helperPos2, &helperScale2, &helperColor2);

        winIconInit();
    helperPos3 = vec3_802f49dc;
    helperScale3 = vec3_802f49e8;
    helperColor3 = dat_8042324c;
    helperPos3.x = (f32)(x + (double)float_395_804233f8);
        helperPos3.y = (f32)y;
        winIconSet(0x86, &helperPos3, &helperScale3, &helperColor3);
    }

    if (pageNum * 28 + 28 < count) {
        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    helperPos4 = vec3_802f49f4;
    helperScale4 = vec3_802f4a00;
    helperColor4 = dat_80423250;
        helperPos4.x = (f32)x + float_395_804233f8;
        helperPos4.y = (f32)y - float_235_80423400;
        winTexSet(0x17, &helperPos4, &helperScale4, &helperColor4);

        winIconInit();
    helperPos5 = vec3_802f4a0c;
    helperScale5 = vec3_802f4a18;
    helperColor5 = dat_80423254;
    helperPos5.x = (f32)(x + (double)float_395_804233f8);
        helperPos5.y = (f32)y - float_220_80423404;
        winIconSet(0x88, &helperPos5, &helperScale5, &helperColor5);
    }

    /*
     * Restore the target's sort label and icon tail.  The old provisional
     * 4x5 texture grid and large type icon do not exist in the target body.
     */
    sortText = msgSearch(str_msg_menu_sort_narabi_802f4d38);
    sortX = (f32)x +
            (float_380_8042340c -
             float_0p7_80423408 *
                 (f32)(u16)FontGetMessageWidth(sortText));

    winFontInit();
    helperPos6 = vec3_802f4a24;
    helperScale6 = vec3_802f4a30;
    helperColor6 = dat_80423258;
    helperPos6.x = sortX;
    helperPos6.y = (f32)y - float_228_80423410;
    winFontSet(&helperPos6, &helperScale6, &helperColor6, sortText);

    winIconInit();
    helperPos7 = vec3_802f4a3c;
    helperScale7 = vec3_802f4a48;
    helperColor7 = dat_8042325c;
    helperPos7.x = sortX - float_30_804233d4;
    helperPos7.y = (f32)y - float_237_80423414;
    winIconSet(0x219, &helperPos7, &helperScale7, &helperColor7);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winWazaGX(double x, double y, double w, double h, void* pWin) {
    extern u32 dat_80423260;
    extern f32 float_0p75_804233dc;
    extern f32 float_8_80423370;
    extern f32 float_16_804233b8;
    extern f32 float_32_80423378;
    u32 texObj98[8];
    f32 texMtx98[3][4];
    u32 texObj9A[8];
    f32 texMtx9A[3][4];
    u32 texObj9B[8];
    f32 texMtx9B[3][4];
    u32 texObjA2[8];
    f32 texMtxA2[3][4];
    u32 texObj9E1[8];
    f32 texMtx9E1[3][4];
    u32 texObjA11[8];
    f32 texMtxA11[3][4];
    u32 texObj9E2[8];
    f32 texMtx9E2[3][4];
    u32 texObjA12[8];
    f32 texMtxA12[3][4];
    u32 texObj9C[8];
    f32 texMtx9C[3][4];
    u32 texObj9F[8];
    f32 texMtx9F[3][4];
    u32 texObj9D[8];
    f32 texMtx9D[3][4];
    u32 fog = dat_80423260;
    f32 fx = (f32)x;
    f32 fy = (f32)y;
    f32 fw = (f32)w;
    f32 fh = (f32)h;
    f32 leftWidth;
    f32 topMidWidth;
    f32 centerWidth;
    f32 innerHeight;
    f32 topY;
    f32 innerBottom;
    f32 lowerBottom;
    f32 finalBottom;

    GXSetBlendMode(0, 1, 0, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0);
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fog);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(8) + 0x11C, 0);
    GXSetCurrentMtx(0);

    /*
     * The target is one fixed eleven-primitive frame, not the provisional
     * helper loop/selection overlay.  Keep each texture object and matrix
     * distinct so MWCC can approach the target's 0x530-byte stack layout.
     */
    leftWidth = (f32)((f64)float_0p75_804233dc * w);
    topMidWidth = (f32)(w - (f64)leftWidth) - float_16_804233b8;
    centerWidth = (fw - leftWidth) - float_8_80423370;
    innerHeight = (fh - float_32_80423378) - float_16_804233b8;
    topY = (f32)(y - (f64)float_32_80423378);
    innerBottom = topY - innerHeight;
    lowerBottom = innerBottom - float_8_80423370;
    finalBottom = fy - fh;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObj98, 0x98);
    GXInitTexObjLOD(texObj98, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj98, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx98,
               (leftWidth) / (f32)(u16)GXGetTexObjWidth(texObj98),
               (float_32_80423378) / (f32)(u16)GXGetTexObjHeight(texObj98),
               1.0f);
    GXLoadTexMtxImm(texMtx98, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx;
    (*(volatile f32*)0xCC008000) = fy;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = fy;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObj9A, 0x9A);
    GXInitTexObjLOD(texObj9A, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj9A, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx9A,
               (topMidWidth) / (f32)(u16)GXGetTexObjWidth(texObj9A),
               (float_32_80423378) / (f32)(u16)GXGetTexObjHeight(texObj9A),
               1.0f);
    GXLoadTexMtxImm(texMtx9A, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = fy;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth + topMidWidth;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth + topMidWidth;
    (*(volatile f32*)0xCC008000) = fy;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObj9B, 0x9B);
    GXInitTexObjLOD(texObj9B, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj9B, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx9B,
               (float_16_804233b8) / (f32)(u16)GXGetTexObjWidth(texObj9B),
               (float_32_80423378) / (f32)(u16)GXGetTexObjHeight(texObj9B),
               1.0f);
    GXLoadTexMtxImm(texMtx9B, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx + fw - float_16_804233b8;
    (*(volatile f32*)0xCC008000) = fy;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx + fw - float_16_804233b8;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + fw;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + fw;
    (*(volatile f32*)0xCC008000) = fy;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObjA2, 0xA2);
    GXInitTexObjLOD(texObjA2, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObjA2, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtxA2,
               (leftWidth) / (f32)(u16)GXGetTexObjWidth(texObjA2),
               (innerHeight) / (f32)(u16)GXGetTexObjHeight(texObjA2),
               1.0f);
    GXLoadTexMtxImm(texMtxA2, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObj9E1, 0x9E);
    GXInitTexObjLOD(texObj9E1, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj9E1, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx9E1,
               (centerWidth) / (f32)(u16)GXGetTexObjWidth(texObj9E1),
               (innerHeight) / (f32)(u16)GXGetTexObjHeight(texObj9E1),
               1.0f);
    GXLoadTexMtxImm(texMtx9E1, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth + centerWidth;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth + centerWidth;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObjA11, 0xA1);
    GXInitTexObjLOD(texObjA11, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObjA11, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtxA11,
               (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjA11),
               (innerHeight) / (f32)(u16)GXGetTexObjHeight(texObjA11),
               1.0f);
    GXLoadTexMtxImm(texMtxA11, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx + fw - float_8_80423370;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx + fw - float_8_80423370;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + fw;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + fw;
    (*(volatile f32*)0xCC008000) = topY;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObj9E2, 0x9E);
    GXInitTexObjLOD(texObj9E2, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj9E2, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx9E2,
               (centerWidth) / (f32)(u16)GXGetTexObjWidth(texObj9E2),
               (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObj9E2),
               1.0f);
    GXLoadTexMtxImm(texMtx9E2, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = lowerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth + centerWidth;
    (*(volatile f32*)0xCC008000) = lowerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth + centerWidth;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObjA12, 0xA1);
    GXInitTexObjLOD(texObjA12, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObjA12, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtxA12,
               (float_8_80423370) / (f32)(u16)GXGetTexObjWidth(texObjA12),
               (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObjA12),
               1.0f);
    GXLoadTexMtxImm(texMtxA12, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx + fw - float_8_80423370;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx + fw - float_8_80423370;
    (*(volatile f32*)0xCC008000) = lowerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + fw;
    (*(volatile f32*)0xCC008000) = lowerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + fw;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObj9C, 0x9C);
    GXInitTexObjLOD(texObj9C, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj9C, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx9C,
               (leftWidth) / (f32)(u16)GXGetTexObjWidth(texObj9C),
               (float_16_804233b8) / (f32)(u16)GXGetTexObjHeight(texObj9C),
               1.0f);
    GXLoadTexMtxImm(texMtx9C, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx;
    (*(volatile f32*)0xCC008000) = finalBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = finalBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObj9F, 0x9F);
    GXInitTexObjLOD(texObj9F, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj9F, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx9F,
               (topMidWidth) / (f32)(u16)GXGetTexObjWidth(texObj9F),
               (float_8_80423370) / (f32)(u16)GXGetTexObjHeight(texObj9F),
               1.0f);
    GXLoadTexMtxImm(texMtx9F, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = lowerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth;
    (*(volatile f32*)0xCC008000) = finalBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth + topMidWidth;
    (*(volatile f32*)0xCC008000) = finalBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + leftWidth + topMidWidth;
    (*(volatile f32*)0xCC008000) = lowerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObj9D, 0x9D);
    GXInitTexObjLOD(texObj9D, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj9D, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx9D,
               (float_16_804233b8) / (f32)(u16)GXGetTexObjWidth(texObj9D),
               (float_16_804233b8) / (f32)(u16)GXGetTexObjHeight(texObj9D),
               1.0f);
    GXLoadTexMtxImm(texMtx9D, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    (*(volatile f32*)0xCC008000) = fx + fw - float_16_804233b8;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

    (*(volatile f32*)0xCC008000) = fx + fw - float_16_804233b8;
    (*(volatile f32*)0xCC008000) = finalBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + fw;
    (*(volatile f32*)0xCC008000) = finalBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;

    (*(volatile f32*)0xCC008000) = fx + fw;
    (*(volatile f32*)0xCC008000) = innerBottom;
    (*(volatile f32*)0xCC008000) = 0.0f;
    (*(volatile f32*)0xCC008000) = 1.0f;
    (*(volatile f32*)0xCC008000) = 0.0f;

}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off
void sort_1_1_func(void* pWin) {
    s32 item = *(s32*)((s32)pWin + 0x214);
    s32 type;
    pouchGetHaveItemCnt();
    pouchHaveItem(item);
    type = 7;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 0;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeSkipList(pWin);
    *(s32*)((s32)pWin + 0x214) = 0;
    *(s32*)((s32)pWin + 0x21C) = *(s32*)((s32)pWin + 0x214) / 10;
}
#pragma optimize_for_size reset

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off
void sort_1_2_func(void* pWin) {
    s32 item = *(s32*)((s32)pWin + 0x214);
    s32 type;
    pouchGetHaveItemCnt();
    pouchHaveItem(item);
    type = 8;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 1;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeSkipList(pWin);
    *(s32*)((s32)pWin + 0x214) = 0;
    *(s32*)((s32)pWin + 0x21C) = *(s32*)((s32)pWin + 0x214) / 10;
}
#pragma optimize_for_size reset

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma optimize_for_size off
void sort_2_1_func(void* pWin) {
    s32 type = 9;

    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 2;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeSkipList(pWin);
    *(s32*)((s32)pWin + 0x218) = 0;
    *(s32*)((s32)pWin + 0x220) = *(s32*)((s32)pWin + 0x218) / 10;
}
#pragma optimize_for_size reset

#pragma optimize_for_size off
void sort_2_2_func(void* pWin) {
    s32 type = 10;

    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 3;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeSkipList(pWin);
    *(s32*)((s32)pWin + 0x218) = 0;
    *(s32*)((s32)pWin + 0x220) = *(s32*)((s32)pWin + 0x218) / 10;
}
#pragma optimize_for_size reset
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void sort_3_1_func(void* pWin) {
    s32 badge = *(s32*)((s32)pWin + 0x3E4);
    s32 type;
    pouchGetHaveBadgeCnt();
    pouchHaveBadge(badge);
    type = 0xB;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 4;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeEquipList(pWin);
    *(s32*)((s32)pWin + 0x3E4) = 0;
    *(s32*)((s32)pWin + 0x3EC) = *(s32*)((s32)pWin + 0x3E4) / 8;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void sort_3_2_func(void* pWin) {
    s32 badge = *(s32*)((s32)pWin + 0x3E4);
    s32 type;
    pouchGetHaveBadgeCnt();
    pouchHaveBadge(badge);
    type = 0xC;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 5;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeEquipList(pWin);
    *(s32*)((s32)pWin + 0x3E4) = 0;
    *(s32*)((s32)pWin + 0x3EC) = *(s32*)((s32)pWin + 0x3E4) / 8;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void sort_3_3_func(void* pWin) {
    s32 badge = *(s32*)((s32)pWin + 0x3E4);
    s32 type;
    pouchGetHaveBadgeCnt();
    pouchHaveBadge(badge);
    type = 0xD;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 6;
    }
    pouchSortItem(type);
    type = 0xD;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 6;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeEquipList(pWin);
    *(s32*)((s32)pWin + 0x3E4) = 0;
    *(s32*)((s32)pWin + 0x3EC) = *(s32*)((s32)pWin + 0x3E4) / 8;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 N_compare_func4_1(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = *(s32*)((s32)param_1 + 8);
    s32 idxB = (s16)*(s32*)((s32)param_2 + 8);
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[(s16)idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[(s16)idxB * 0xA]), bufB);
    return strcmp(bufA, bufB);
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 N_compare_func4_1_r(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = *(s32*)((s32)param_1 + 8);
    s32 idxB = (s16)*(s32*)((s32)param_2 + 8);
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[(s16)idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[(s16)idxB * 0xA]), bufB);
    return strcmp(bufB, bufA);
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

s32 N_compare_func4_2(void* param_1, void* param_2) {
    s32 idxA = *(s32*)((s32)param_1 + 8);
    s32 idxB = *(s32*)((s32)param_2 + 8);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[(s16)idxA * 0x14];
    s32 valueB = values[(s16)idxB * 0x14];

    if (valueA > valueB) {
        return 1;
    }
    if (valueA < valueB) {
        return -1;
    }
    return 0;
}

s32 N_compare_func4_2_r(void* param_1, void* param_2) {
    s32 idxA = *(s32*)((s32)param_1 + 8);
    s32 idxB = *(s32*)((s32)param_2 + 8);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[(s16)idxA * 0x14];
    s32 valueB = values[(s16)idxB * 0x14];

    if (valueA < valueB) {
        return 1;
    }
    if (valueA > valueB) {
        return -1;
    }
    return 0;
}


void sort_4_1_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x404), *(s32*)((s32)pWin + 0xD64), 0xC, N_compare_func4_1);
    } else {
        qqsort((void*)((s32)pWin + 0x404), *(s32*)((s32)pWin + 0xD64), 0xC, N_compare_func4_1_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x3E8) = 0;
    *(s32*)((s32)pWin + 0x3F0) = *(s32*)((s32)pWin + 0x3E8) / 8;
}


void sort_4_2_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x404), *(s32*)((s32)pWin + 0xD64), 0xC, N_compare_func4_2);
    } else {
        qqsort((void*)((s32)pWin + 0x404), *(s32*)((s32)pWin + 0xD64), 0xC, N_compare_func4_2_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x3E8) = 0;
    *(s32*)((s32)pWin + 0x3F0) = *(s32*)((s32)pWin + 0x3E8) / 8;
}

#pragma optimize_for_size off
u8 sort_4_3_func(void* pWin) {
    typedef struct { s32 words[3]; } WindowRecord;
    typedef struct { u8 prefix[0x12]; s16 value; u8 gap[8]; s8 rank; u8 tail[11]; } SortRecord;
    SortRecord* recA;
    SortRecord* recB;
    WindowRecord temp;
    s32 i;
    s32 j;
    char* a;
    char* b;
    s32 swap;
    s32 idA;
    s32 idB;

    if (*(s32*)((s32)pWin + 0x180) == 0) {
        for (i = 0; i < *(s32*)((s32)pWin + 0xD64) - 1; i++) {
            a = (char*)pWin + i * 12;
            b = (char*)pWin + (i + 1) * 12;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xD64); j++) {
                swap = 0;
                idA = (s16)*(s32*)(a + 0x40C);
                idB = (s16)*(s32*)(b + 0x40C);
                recA = (SortRecord*)(itemDataTable + idA * 40);
                recB = (SortRecord*)(itemDataTable + idB * 40);
                if (recA->rank > recB->rank) {
                    swap = 1;
                } else if (recA->rank == recB->rank &&
                           recA->value > recB->value) {
                    swap = 1;
                }
                if (swap != 0) {
                    temp = *(WindowRecord*)(a + 0x404);
                    *(WindowRecord*)(a + 0x404) = *(WindowRecord*)(b + 0x404);
                    *(WindowRecord*)(b + 0x404) = temp;
                }
                b += 12;
            }
        }
    } else {
        for (i = 0; i < *(s32*)((s32)pWin + 0xD64) - 1; i++) {
            a = (char*)pWin + i * 12;
            b = (char*)pWin + (i + 1) * 12;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xD64); j++) {
                swap = 0;
                idA = (s16)*(s32*)(a + 0x40C);
                idB = (s16)*(s32*)(b + 0x40C);
                recA = (SortRecord*)(itemDataTable + idA * 40);
                recB = (SortRecord*)(itemDataTable + idB * 40);
                if (recA->rank < recB->rank) {
                    swap = 1;
                } else if (recA->rank == recB->rank &&
                           recA->value < recB->value) {
                    swap = 1;
                }
                if (swap != 0) {
                    temp = *(WindowRecord*)(a + 0x404);
                    *(WindowRecord*)(a + 0x404) = *(WindowRecord*)(b + 0x404);
                    *(WindowRecord*)(b + 0x404) = temp;
                }
                b += 12;
            }
        }
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xD68) = 0;
    *(s32*)((s32)pWin + 0xD6C) = *(s32*)((s32)pWin + 0xD68) / 8;

}
#pragma optimize_for_size reset
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func5_1(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = (s16)*(s16*)param_1 + 0xF0;
    s32 idxB = (s16)*(s16*)param_2 + 0xF0;
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[idxB * 0xA]), bufB);
    return strcmp(bufA, bufB);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func5_1_r(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = (s16)*(s16*)param_1 + 0xF0;
    s32 idxB = (s16)*(s16*)param_2 + 0xF0;
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[idxB * 0xA]), bufB);
    return strcmp(bufB, bufA);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 compare_func5_2(void* param_1, void* param_2) {
    s32 idxA = (s16)((s16)*(s16*)param_1 + 0xF0);
    s32 idxB = (s16)((s16)*(s16*)param_2 + 0xF0);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[idxA * 0x14];
    s32 valueB = values[idxB * 0x14];

    if (valueA > valueB) {
        return 1;
    }
    if (valueA < valueB) {
        return -1;
    }
    return 0;
}

s32 compare_func5_2_r(void* param_1, void* param_2) {
    s32 idxA = (s16)((s16)*(s16*)param_1 + 0xF0);
    s32 idxB = (s16)((s16)*(s16*)param_2 + 0xF0);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[idxA * 0x14];
    s32 valueB = values[idxB * 0x14];

    if (valueA < valueB) {
        return 1;
    }
    if (valueA > valueB) {
        return -1;
    }
    return 0;
}


#pragma optimize_for_size off
void sort_5_1_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0xEBC), *(s32*)((s32)pWin + 0xE94), 2, compare_func5_1);
    } else {
        qqsort((void*)((s32)pWin + 0xEBC), *(s32*)((s32)pWin + 0xE94), 2, compare_func5_1_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xE9C) = 0;
    *(s32*)((s32)pWin + 0xEA0) = *(s32*)((s32)pWin + 0xE9C) / 28;
}
#pragma optimize_for_size reset


#pragma optimize_for_size off
void sort_5_2_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0xEBC), *(s32*)((s32)pWin + 0xE94), 2, compare_func5_2);
    } else {
        qqsort((void*)((s32)pWin + 0xEBC), *(s32*)((s32)pWin + 0xE94), 2, compare_func5_2_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xE9C) = 0;
    *(s32*)((s32)pWin + 0xEA0) = *(s32*)((s32)pWin + 0xE9C) / 28;
}
#pragma optimize_for_size reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off
u8 sort_5_3_func(void* pWin) {
    typedef struct { u8 prefix[0x12]; s16 value; u8 gap[8]; s8 rank; u8 tail[11]; } SortRecord;
    SortRecord* recA;
    SortRecord* recB;
    s32 i;
    s32 j;
    char* a;
    char* b;
    s16 tmp;
    s16 other;
    s32 swap;
    s32 idA;
    s32 idB;

    if (*(s32*)((s32)pWin + 0x180) == 0) {
        for (i = 0; i < *(s32*)((s32)pWin + 0xE94) - 1; i++) {
            a = (char*)pWin + i * 2;
            b = (char*)pWin + (i + 1) * 2;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xE94); j++) {
                tmp = *(s16*)(a + 0xEBC);
                other = *(s16*)(b + 0xEBC);
                swap = 0;
                idA = (s16)(tmp + 0xF0);
                idB = (s16)(other + 0xF0);
                recA = (SortRecord*)(itemDataTable + idA * 0x28);
                recB = (SortRecord*)(itemDataTable + idB * 0x28);
                if (recA->rank > recB->rank) {
                    swap = 1;
                } else if (recA->rank == recB->rank &&
                           recA->value > recB->value) {
                    swap = 1;
                }
                if (swap != 0) {
                    *(s16*)(a + 0xEBC) = other;
                    *(s16*)(b + 0xEBC) = tmp;
                }
                b += 2;
            }
        }
    } else {
        for (i = 0; i < *(s32*)((s32)pWin + 0xE94) - 1; i++) {
            a = (char*)pWin + i * 2;
            b = (char*)pWin + (i + 1) * 2;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xE94); j++) {
                tmp = *(s16*)(a + 0xEBC);
                other = *(s16*)(b + 0xEBC);
                swap = 0;
                idA = (s16)(tmp + 0xF0);
                idB = (s16)(other + 0xF0);
                recA = (SortRecord*)(itemDataTable + idA * 0x28);
                recB = (SortRecord*)(itemDataTable + idB * 0x28);
                if (recA->rank < recB->rank) {
                    swap = 1;
                } else if (recA->rank == recB->rank &&
                           recA->value < recB->value) {
                    swap = 1;
                }
                if (swap != 0) {
                    *(s16*)(a + 0xEBC) = other;
                    *(s16*)(b + 0xEBC) = tmp;
                }
                b += 2;
            }
        }
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xE9C) = 0;
    i = *(s32*)((s32)pWin + 0xE9C);
    j = i / 28 + (i >> 31);
    *(s32*)((s32)pWin + 0xEA0) = j - (j >> 31);

}
#pragma optimize_for_size reset
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
#pragma optimize_for_size reset
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func6_1(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = (s16)*(s16*)param_1 + 0xB3;
    s32 idxB = (s16)*(s16*)param_2 + 0xB3;
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[idxB * 0xA]), bufB);
    return strcmp(bufA, bufB);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func6_1_r(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = (s16)*(s16*)param_1 + 0xB3;
    s32 idxB = (s16)*(s16*)param_2 + 0xB3;
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[idxB * 0xA]), bufB);
    return strcmp(bufB, bufA);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 compare_func6_2(void* param_1, void* param_2) {
    s32 idxA = (s16)((s16)*(s16*)param_1 + 0xB3);
    s32 idxB = (s16)((s16)*(s16*)param_2 + 0xB3);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[idxA * 0x14];
    s32 valueB = values[idxB * 0x14];

    if (valueA > valueB) {
        return 1;
    }
    if (valueA < valueB) {
        return -1;
    }
    return 0;
}

s32 compare_func6_2_r(void* param_1, void* param_2) {
    s32 idxA = (s16)((s16)*(s16*)param_1 + 0xB3);
    s32 idxB = (s16)((s16)*(s16*)param_2 + 0xB3);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[idxA * 0x14];
    s32 valueB = values[idxB * 0x14];

    if (valueA < valueB) {
        return 1;
    }
    if (valueA > valueB) {
        return -1;
    }
    return 0;
}


#pragma optimize_for_size off
void sort_6_1_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0xFBC), *(s32*)((s32)pWin + 0xF94), 2, compare_func6_1);
    } else {
        qqsort((void*)((s32)pWin + 0xFBC), *(s32*)((s32)pWin + 0xF94), 2, compare_func6_1_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xF9C) = 0;
    *(s32*)((s32)pWin + 0xFA0) = *(s32*)((s32)pWin + 0xF9C) / 28;
}
#pragma optimize_for_size reset


#pragma optimize_for_size off
void sort_6_2_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0xFBC), *(s32*)((s32)pWin + 0xF94), 2, compare_func6_2);
    } else {
        qqsort((void*)((s32)pWin + 0xFBC), *(s32*)((s32)pWin + 0xF94), 2, compare_func6_2_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xF9C) = 0;
    *(s32*)((s32)pWin + 0xFA0) = *(s32*)((s32)pWin + 0xF9C) / 28;
}
#pragma optimize_for_size reset

s32 compare_func1(void* a, void* b) {
    if (*(u8*)a > *(u8*)b) {
        return 1;
    }
    if (*(u8*)a < *(u8*)b) {
        return -1;
    }
    return 0;
}

s32 compare_func1_r(void* a, void* b) {
    if (*(u8*)a < *(u8*)b) {
        return 1;
    }
    if (*(u8*)a > *(u8*)b) {
        return -1;
    }
    return 0;
}

void sort_7_1_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1);
    } else {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x1048) = 0;
    *(s32*)((s32)pWin + 0x104C) = *(s32*)((s32)pWin + 0x1048) / 16;
}


#pragma optimize_for_size off
#pragma opt_common_subs off
s32 compare_func2(void* param_1, void* param_2) {
    char bufA[0x40];
    char bufB[0x40];
    void* unitA;
    void* unitB;
    s32 idxA;
    char* nameA;
    s32 idxB;
    char** tblB;
    char** tblA;

    unitA = battleGetUnitMonosiriPtr(*(u8*)((s32)param_1 + 1));
    unitB = battleGetUnitMonosiriPtr(*(u8*)((s32)param_2 + 1));
    nameA = *(char**)((s32)unitA + 0x14);
    unitA = unitB;
    tblA = tbl_1614;
    tblB = tblA;
    idxA = 0;
    while (1) {
        sprintf(bufA, str_menu_monosiri_PCTs_802f4d00, *tblB);
        if (strcmp(bufA, nameA) == 0) {
            break;
        }
        idxA++;
        tblB++;
        if ((u32)idxA >= 0x1A) {
            break;
        }
    }
    if ((u32)idxA >= 0x1A) {
        idxA = 0;
    }

    unitA = *(void**)((s32)unitA + 0x14);
    idxB = 0;
    while (1) {
        sprintf(bufB, str_menu_monosiri_PCTs_802f4d00, *tblA);
        if (strcmp(bufB, (char*)unitA) == 0) {
            break;
        }
        idxB++;
        tblA++;
        if ((u32)idxB >= 0x1A) {
            break;
        }
    }
    if ((u32)idxB >= 0x1A) {
        idxB = 0;
    }
    if (idxA > idxB) {
        return 1;
    }
    if (idxA < idxB) {
        return -1;
    }
    return 0;
}
#pragma optimize_for_size reset
#pragma opt_common_subs reset
#pragma optimize_for_size off
#pragma opt_common_subs off
s32 compare_func2_r(void* param_1, void* param_2) {
    char bufA[0x40];
    char bufB[0x40];
    void* unitA;
    void* unitB;
    s32 idxA;
    char* nameA;
    s32 idxB;
    char** tblB;
    char** tblA;

    unitA = battleGetUnitMonosiriPtr(*(u8*)((s32)param_1 + 1));
    unitB = battleGetUnitMonosiriPtr(*(u8*)((s32)param_2 + 1));
    nameA = *(char**)((s32)unitA + 0x14);
    unitA = unitB;
    tblA = tbl_1614;
    tblB = tblA;
    idxA = 0;
    while (1) {
        sprintf(bufA, str_menu_monosiri_PCTs_802f4d00, *tblB);
        if (strcmp(bufA, nameA) == 0) {
            break;
        }
        idxA++;
        tblB++;
        if ((u32)idxA >= 0x1A) {
            break;
        }
    }
    if ((u32)idxA >= 0x1A) {
        idxA = 0;
    }

    unitA = *(void**)((s32)unitA + 0x14);
    idxB = 0;
    while (1) {
        sprintf(bufB, str_menu_monosiri_PCTs_802f4d00, *tblA);
        if (strcmp(bufB, (char*)unitA) == 0) {
            break;
        }
        idxB++;
        tblA++;
        if ((u32)idxB >= 0x1A) {
            break;
        }
    }
    if ((u32)idxB >= 0x1A) {
        idxB = 0;
    }
    if (idxA < idxB) {
        return 1;
    }
    if (idxA > idxB) {
        return -1;
    }
    return 0;
}
#pragma optimize_for_size reset
#pragma opt_common_subs reset


#pragma opt_common_subs off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void sort_7_2_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1);
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func2);
    } else {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1_r);
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func2_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x1048) = 0;
    *(s32*)((s32)pWin + 0x104C) = *(s32*)((s32)pWin + 0x1048) / 16;
}
#pragma opt_common_subs reset
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func3(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    void* unitA;
    void* unitB;
    void* ptrB = param_2;
    void* ptrA = param_1;

    unitA = battleGetUnitMonosiriPtr(*(u8*)((s32)ptrA + 1));
    unitB = battleGetUnitMonosiriPtr(*(u8*)((s32)ptrB + 1));
    unk_800d48b0(msgSearch(*(char**)unitA), bufA);
    unk_800d48b0(msgSearch(*(char**)unitB), bufB);
    if (*(u8*)((s32)ptrA + 1) == 0x4D || *(u8*)((s32)ptrA + 1) == 0x87) {
        if (evtGetValue(NULL, 0xF5DE0180) < 0xD2) {
            unk_800d48b0(msgSearch(str_btl_un_hatena_802f4ce0), bufA);
        } else {
            unk_800d48b0(msgSearch(str_btl_un_rampell_802f4cf0), bufA);
        }
    }
    if (*(u8*)((s32)ptrB + 1) == 0x4D || *(u8*)((s32)ptrB + 1) == 0x87) {
        if (evtGetValue(NULL, 0xF5DE0180) < 0xD2) {
            unk_800d48b0(msgSearch(str_btl_un_hatena_802f4ce0), bufB);
        } else {
            unk_800d48b0(msgSearch(str_btl_un_rampell_802f4cf0), bufB);
        }
    }
    return strcmp(bufA, bufB);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func3_r(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    void* unitA;
    void* unitB;
    void* ptrB = param_2;
    void* ptrA = param_1;

    unitA = battleGetUnitMonosiriPtr(*(u8*)((s32)ptrA + 1));
    unitB = battleGetUnitMonosiriPtr(*(u8*)((s32)ptrB + 1));
    unk_800d48b0(msgSearch(*(char**)unitA), bufA);
    unk_800d48b0(msgSearch(*(char**)unitB), bufB);
    if (*(u8*)((s32)ptrA + 1) == 0x4D || *(u8*)((s32)ptrA + 1) == 0x87) {
        if (evtGetValue(NULL, 0xF5DE0180) < 0xD2) {
            unk_800d48b0(msgSearch(str_btl_un_hatena_802f4ce0), bufA);
        } else {
            unk_800d48b0(msgSearch(str_btl_un_rampell_802f4cf0), bufA);
        }
    }
    if (*(u8*)((s32)ptrB + 1) == 0x4D || *(u8*)((s32)ptrB + 1) == 0x87) {
        if (evtGetValue(NULL, 0xF5DE0180) < 0xD2) {
            unk_800d48b0(msgSearch(str_btl_un_hatena_802f4ce0), bufB);
        } else {
            unk_800d48b0(msgSearch(str_btl_un_rampell_802f4cf0), bufB);
        }
    }
    return strcmp(bufB, bufA);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma opt_common_subs off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void sort_7_3_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1);
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func3);
    } else {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1_r);
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func3_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x1048) = 0;
    *(s32*)((s32)pWin + 0x104C) = *(s32*)((s32)pWin + 0x1048) / 16;
}
#pragma opt_common_subs reset
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func4(void* param_1, void* param_2) {
    void* mailA;
    void* mailB;
    char* msgA;
    char* msgB;

    mailA = mailGetPtr(*(u8*)((s32)param_1 + 1));
    mailB = mailGetPtr(*(u8*)((s32)param_2 + 1));
    msgA = msgSearch(*(char**)((s32)mailA + 8));
    msgB = msgSearch(*(char**)((s32)mailB + 8));
    return strcmp(msgA, msgB);
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func4_r(void* param_1, void* param_2) {
    void* mailA;
    void* mailB;
    char* msgA;
    char* msgB;

    mailA = mailGetPtr(*(u8*)((s32)param_1 + 1));
    mailB = mailGetPtr(*(u8*)((s32)param_2 + 1));
    msgA = msgSearch(*(char**)((s32)mailA + 8));
    msgB = msgSearch(*(char**)((s32)mailB + 8));
    return strcmp(msgB, msgA);
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset


#pragma optimize_for_size off
void sort_8_1_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x26E), *(s32*)((s32)pWin + 0x254), 2, compare_func4);
    } else {
        qqsort((void*)((s32)pWin + 0x26E), *(s32*)((s32)pWin + 0x254), 2, compare_func4_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x258) = 0;
    *(s32*)((s32)pWin + 0x25C) = *(s32*)((s32)pWin + 0x258) / 5;
}

#pragma optimize_for_size reset

s32 compare_func5(void* a, void* b) {
    if (*(u8*)a > *(u8*)b) {
        return 1;
    }
    if (*(u8*)a < *(u8*)b) {
        return -1;
    }
    return 0;
}

s32 compare_func5_r(void* a, void* b) {
    if (*(u8*)a < *(u8*)b) {
        return 1;
    }
    if (*(u8*)a > *(u8*)b) {
        return -1;
    }
    return 0;
}


#pragma optimize_for_size off
void sort_8_2_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x26E), *(s32*)((s32)pWin + 0x254), 2, compare_func5);
    } else {
        qqsort((void*)((s32)pWin + 0x26E), *(s32*)((s32)pWin + 0x254), 2, compare_func5_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x258) = 0;
    *(s32*)((s32)pWin + 0x25C) = *(s32*)((s32)pWin + 0x258) / 5;
}
#pragma optimize_for_size reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 winSortMain(void* pWin) {
    extern void FontGetMessageWidthLine(char* msg, u16* width);
    extern s32 strcmp(char* a, char* b);
    extern char* strncpy(char* dst, char* src, u32 n);
    extern void* sort_data[8];
    extern char _tmp_1042[];
    extern f32 float_neg600_804233d0;
    extern f32 float_30_804233d4;
    extern f32 float_40_804233bc;
    extern f32 float_0p125_804233d8;
    u32 buttons;
    u32 dirs;
    s32 changed;
    char* msg;
    f32 targetValue,currentValue,interpolation;
    u16 width[4];

    buttons = *(u32*)((s32)pWin + 4);
    dirs = *(u32*)((s32)pWin + 0x10);
    if (*(s32*)((s32)pWin + 0x17C) == 0) {
        buttons = 0;
        dirs = 0;
    }
    if ((buttons & 0x1000) != 0) {
        *(s32*)((s32)pWin + 0x17C) = 0;
        *(f32*)((s32)pWin + 0x16C) = float_neg600_804233d0;
    } else if ((buttons & 0x200) != 0) {
        psndSFXOn(0x20013);
        *(s32*)((s32)pWin + 0x17C) = 0;
        *(f32*)((s32)pWin + 0x16C) = float_neg600_804233d0;
    } else if ((dirs & 0x1000) != 0) {
        psndSFXOn(0x20005);
        (*(s32*)((s32)pWin + 0x174))--;
        if (*(s32*)((s32)pWin + 0x174) < 0) {
            *(s32*)((s32)pWin + 0x174) = *(s32*)((s32)pWin + 0x178) - 1;
        }
    } else if ((dirs & 0x2000) != 0) {
        psndSFXOn(0x20005);
        (*(s32*)((s32)pWin + 0x174))++;
        if (*(s32*)((s32)pWin + 0x174) >= *(s32*)((s32)pWin + 0x178)) {
            *(s32*)((s32)pWin + 0x174) = 0;
        }
    } else if ((buttons & 0x100) != 0) {
        void (**entry)(void*);
        psndSFXOn(0x20012);
        entry = (void (**)(void*))((s32)sort_data[*(s32*)((s32)pWin + 0x184)] + (*(s32*)((s32)pWin + 0x174) << 3));
        entry[1](pWin);
    }

    if (*(s32*)((s32)pWin + 0x17C) != 0) {
        changed = 0;
        *(f32*)((s32)pWin + 0x158) = float_30_804233d4 + *(f32*)((s32)pWin + 0x16C);
        *(f32*)((s32)pWin + 0x15C) = (*(f32*)((s32)pWin + 0x170) - float_40_804233bc) - (f32)(*(s32*)((s32)pWin + 0x174) * 0x28);
        msg = *(char**)((s32)sort_data[*(s32*)((s32)pWin + 0x184)] + (*(s32*)((s32)pWin + 0x174) << 3));
        if (msg != 0) {
            if (*(char**)((s32)pWin + 0x144) != msg) {
                changed = 1;
            } else if (strcmp(_tmp_1042, msg) != 0) {
                changed = 1;
            }
            if (changed != 0) {
                *(s32*)((s32)pWin + 0x138) = 0;
                *(char**)((s32)pWin + 0x13C) = msg;
                *(s32*)((s32)pWin + 0x140) = 0;
                FontGetMessageWidthLine(msgSearch(*(char**)((s32)pWin + 0x13C)), width);
                *(s32*)((s32)pWin + 0x148) = 0;
                *(s32*)((s32)pWin + 0x14C) = width[0] + 1;
                *(char**)((s32)pWin + 0x144) = msg;
                strncpy(_tmp_1042, msg, 0x3C);
            }
        }
    }

    targetValue=*(f32*)((s32)pWin+0x16C);
    currentValue=*(f32*)((s32)pWin+0x164);
    interpolation=float_0p125_804233d8;
    *(f32*)((s32)pWin+0x164)=(targetValue-currentValue)*interpolation+currentValue;
    targetValue=*(f32*)((s32)pWin+0x170);
    currentValue=*(f32*)((s32)pWin+0x168);
    *(f32*)((s32)pWin+0x168)=(targetValue-currentValue)*interpolation+currentValue;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

void winSortGX(void* pWin) {
    extern void GXSetScissor(u32 x, u32 y, u32 width, u32 height);
    extern f32 float_8_80423370;
    extern f32 float_16_804233b8;
    extern f32 float_24_8042339c;
    extern f32 float_40_804233bc;
    extern f32 float_128_804233c0;
    extern f32 float_136_804233c4;
    extern f32 float_240_80423380;
    extern f32 float_304_8042337c;
    extern u32 dat_804232d4;
    f32 pos[3];
    f32 scale[3];
    u32 texObj[8];
    f32 texMtx[3][4];
    u32 texObjA6[8];
    f32 texMtxA6[3][4];
    u32 texObjA4[8];
    f32 texMtxA4[3][4];
    u32 texObjA7[8];
    f32 texMtxA7[3][4];
    u32 texObjA8[8];
    f32 texMtxA8[3][4];
    u32 white = 0xFFFFFFFF;
    u32 fog = dat_804232d4;
    f32 x = *(f32*)((s32)pWin + 0x164);
    f32 y = *(f32*)((s32)pWin + 0x168);
    s32 type = *(s32*)((s32)pWin + 0x184);
    s32 cursor = *(s32*)((s32)pWin + 0x174);
    s32 rowOffset;
    f32 frameTop;
    f32 frameBottom;
    f32 frameLeft;
    f32 frameRight;
    f32 listBottom;
    f32 listHeight;
    s32 i;

    GXSetBlendMode(0, 1, 0, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0);
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fog);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevOp(0, 3);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1); GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0); GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(8) + 0x11C, 0); GXSetCurrentMtx(0);
    if (type == 7) {
        GXSetScissor((u32)(float_16_804233b8 + float_16_804233b8 +
                           float_304_8042337c + *(f32*)((s32)pWin + 0x244)),
                     (u32)(float_40_804233bc + float_240_80423380 -
                           *(f32*)((s32)pWin + 0x24C)), 0x200, 0xC0);
    }
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                              texObj, 0xA5);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx,
               float_128_804233c0 / (f32)GXGetTexObjWidth(texObj),
               float_24_8042339c / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    DAT_cc008000 = x;
    DAT_cc008000 = y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = x;
    DAT_cc008000 = y - float_24_8042339c;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = x + float_128_804233c0;
    DAT_cc008000 = y - float_24_8042339c;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = x + float_128_804233c0;
    DAT_cc008000 = y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
                              texObj, 0xA3);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx,
               float_24_8042339c / (f32)GXGetTexObjWidth(texObj),
               float_24_8042339c / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    DAT_cc008000 = x + float_128_804233c0;
    DAT_cc008000 = y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = x + float_128_804233c0;
    DAT_cc008000 = y - float_24_8042339c;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = x + float_128_804233c0 + float_24_8042339c;
    DAT_cc008000 = y - float_24_8042339c;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = x + float_128_804233c0 + float_24_8042339c;
    DAT_cc008000 = y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

    /* Target's next adjacent primitive family: repeated 0xA6 cursor-column quads. */
    rowOffset = 0;
    for (i = 0; i < *(s32*)((s32)pWin + 0x178); i++) {
        TEXGetGXTexObjFromPalette(
            **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
            texObjA6, 0xA6);
        GXInitTexObjLOD(texObjA6, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObjA6, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtxA6,
                   float_16_804233b8 / (f32)(u16)GXGetTexObjWidth(texObjA6),
                   float_40_804233bc / (f32)(u16)GXGetTexObjHeight(texObjA6),
                   1.0f);
        GXLoadTexMtxImm(texMtxA6, 0x1E, 1);
        GXBegin(0x80, 0, 4);

        frameTop = (y - float_24_8042339c) - (f32)rowOffset;
        frameBottom = frameTop - float_40_804233bc;
        frameLeft = x + float_128_804233c0 + float_8_80423370;
        frameRight = frameLeft + float_16_804233b8;

        DAT_cc008000 = frameLeft;
        DAT_cc008000 = frameTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = frameLeft;
        DAT_cc008000 = frameBottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = frameRight;
        DAT_cc008000 = frameBottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = frameRight;
        DAT_cc008000 = frameTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

        rowOffset += 40;
    }

    /*
     * Continue the target's fixed border primitive sequence after the repeated
     * 0xA6 column: 0xA4 lower-right cap, 0xA7 lower horizontal strip, and
     * 0xA8 full-height background strip.
     */
    listHeight = (f32)(*(s32*)((s32)pWin + 0x178) * 40);
    listBottom = (y - float_24_8042339c) - listHeight;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObjA4, 0xA4);
    GXInitTexObjLOD(texObjA4, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObjA4, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtxA4,
               float_24_8042339c / (f32)(u16)GXGetTexObjWidth(texObjA4),
               float_24_8042339c / (f32)(u16)GXGetTexObjHeight(texObjA4),
               1.0f);
    GXLoadTexMtxImm(texMtxA4, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    frameLeft = x + float_128_804233c0;
    frameRight = frameLeft + float_24_8042339c;
    frameTop = listBottom;
    frameBottom = frameTop - float_24_8042339c;

    DAT_cc008000 = frameLeft;
    DAT_cc008000 = frameTop;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = frameLeft;
    DAT_cc008000 = frameBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = frameRight;
    DAT_cc008000 = frameBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = frameRight;
    DAT_cc008000 = frameTop;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObjA7, 0xA7);
    GXInitTexObjLOD(texObjA7, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObjA7, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtxA7,
               float_128_804233c0 / (f32)(u16)GXGetTexObjWidth(texObjA7),
               float_24_8042339c / (f32)(u16)GXGetTexObjHeight(texObjA7),
               1.0f);
    GXLoadTexMtxImm(texMtxA7, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    frameLeft = x;
    frameRight = x + float_128_804233c0;
    frameTop = listBottom;
    frameBottom = frameTop - float_24_8042339c;

    DAT_cc008000 = frameLeft;
    DAT_cc008000 = frameTop;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = frameLeft;
    DAT_cc008000 = frameBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = frameRight;
    DAT_cc008000 = frameBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = frameRight;
    DAT_cc008000 = frameTop;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

    TEXGetGXTexObjFromPalette(
        **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0),
        texObjA8, 0xA8);
    GXInitTexObjLOD(texObjA8, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObjA8, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtxA8,
               float_136_804233c4 / (f32)(u16)GXGetTexObjWidth(texObjA8),
               listHeight / (f32)(u16)GXGetTexObjHeight(texObjA8),
               1.0f);
    GXLoadTexMtxImm(texMtxA8, 0x1E, 1);
    GXBegin(0x80, 0, 4);

    frameLeft = x;
    frameRight = x + float_136_804233c4;
    frameTop = y - float_24_8042339c;
    frameBottom = frameTop - listHeight;

    DAT_cc008000 = frameLeft;
    DAT_cc008000 = frameTop;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = frameLeft;
    DAT_cc008000 = frameBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = frameRight;
    DAT_cc008000 = frameBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = frameRight;
    DAT_cc008000 = frameTop;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    scale[2] = 1.0f;
    pos[2] = 0.0f;
    for (i = 0; i < *(s32*)((s32)pWin + 0x178); i++) {
        pos[0] = x + i * 42.0f;
        pos[1] = y;
        winTexSet(0xA3 + i, pos, scale, &white);
    }
    pos[0] = x + cursor * 42.0f;
    pos[1] = y + 4.0f;
    scale[0] = 1.15f;
    scale[1] = 1.15f;
    winTexSet(type == 7 ? 0xA7 : 0xA5, pos, scale, &white);
    if (type == 7) {
        GXSetScissor(0, 0, 0x260, 0x1E0);
    }
}

void winSortEntry(void* win, s32 type, f32 a, f32 b) {
    void* entry = sort_data[type];
    s32 index = 0;
    f32 neg = float_neg1000_804233a0;

    while (*(void**)entry != 0) {
        entry = (void*)((s32)entry + 8);
        index++;
    }

    *(s32*)((s32)win + 0x184) = type;
    *(s32*)((s32)win + 0x178) = index;
    *(s32*)((s32)win + 0x174) = 0;
    *(s32*)((s32)win + 0x180) = 0;
    *(f32*)((s32)win + 0x164) = neg;
    *(f32*)((s32)win + 0x16C) = a;
    *(f32*)((s32)win + 0x168) = b;
    *(f32*)((s32)win + 0x170) = b;
    *(s32*)((s32)win + 0x17C) = 1;
}

s32 winSortWait(void* win) {
    s32 value = *(s32*)((s32)win + 0x17C);
    return (u32)(-value | value) >> 31;
}

void winKageGX(f32 x, f32 y, f32 z, f32 scale, void* pWin, u32* colorPtr) {
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 angle);
    extern u32 dat_804232e4;
    extern f32 float_neg1p3963_804233b0;
    extern f32 float_neg0p5_804233b4;

    u32 fogColor = dat_804232e4;
    u32 color;
    u32 texObj[8];
    f32 modelMtx[3][4];
    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    f32 rotMtx[3][4];
    f32 texH;
    f32 texW;
    void* cam;

    GXSetFog(0, float_0_80423338, float_0_80423338, float_0_80423338, float_0_80423338, &fogColor);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 7, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);

    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x7A);
    GXInitTexObjLOD(texObj, 1, 1, float_0_80423338, float_0_80423338, float_0_80423338, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    color=*colorPtr;
    GXSetTevColor(1, &color);

    PSMTXTrans(transMtx, x, y, z);
    texH = scale * (float_2_80423360 * (f32)(u16)GXGetTexObjHeight(texObj));
    texW = float_2_80423360 * (f32)(u16)GXGetTexObjWidth(texObj);
    PSMTXScale(scaleMtx, scale * texW, texH, float_1_80423340);
    PSMTXRotRad(rotMtx, 0x78, float_neg1p3963_804233b0);
    PSMTXConcat(transMtx, rotMtx, modelMtx);
    PSMTXConcat(modelMtx, scaleMtx, modelMtx);
    cam = camGetPtr(8);
    PSMTXConcat((void*)((s32)cam + 0x11C), modelMtx, modelMtx);
    GXLoadPosMtxImm(modelMtx, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXBegin(0x80, 0, 4);

    *(volatile f32*)0xCC008000 = float_neg0p5_804233b4;
    *(volatile f32*)0xCC008000 = float_0p5_8042333c;
    *(volatile f32*)0xCC008000 = float_0_80423338;
    *(volatile f32*)0xCC008000 = float_0_80423338;
    *(volatile f32*)0xCC008000 = float_0_80423338;
    *(volatile f32*)0xCC008000 = float_neg0p5_804233b4;
    *(volatile f32*)0xCC008000 = float_neg0p5_804233b4;
    *(volatile f32*)0xCC008000 = float_0_80423338;
    *(volatile f32*)0xCC008000 = float_0_80423338;
    *(volatile f32*)0xCC008000 = float_1_80423340;
    *(volatile f32*)0xCC008000 = float_0p5_8042333c;
    *(volatile f32*)0xCC008000 = float_neg0p5_804233b4;
    *(volatile f32*)0xCC008000 = float_0_80423338;
    *(volatile f32*)0xCC008000 = float_1_80423340;
    *(volatile f32*)0xCC008000 = float_1_80423340;
    *(volatile f32*)0xCC008000 = float_0p5_8042333c;
    *(volatile f32*)0xCC008000 = float_0p5_8042333c;
    *(volatile f32*)0xCC008000 = float_0_80423338;
    *(volatile f32*)0xCC008000 = float_1_80423340;
    *(volatile f32*)0xCC008000 = float_0_80423338;
}

u8 winZClear(void) {
    extern void* camGetCurPtr(void);
    extern void GXSetAlphaUpdate(s32 update);
    extern void GXSetColorUpdate(s32 update);
    extern f32 float_neg1000_804233a0;
    extern f32 float_neg500_804233a4;
    extern f32 float_300_804233a8;
    extern f32 float_neg100_804233ac;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 mtx[3][4];

    PSMTXTrans(mtx, float_0_80423338, float_0_80423338, float_neg1000_804233a0);
    PSMTXConcat((void*)((s32)camGetCurPtr() + 0x11C), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXSetAlphaUpdate(0);
    GXSetColorUpdate(0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevOp(0, 4);
    GXSetNumTexGens(0);
    GXSetZMode(1, 7, 1);
    GXSetBlendMode(0, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxDesc(9, 1);
    GXBegin(0x80, 0, 4);
    *fifo = float_neg500_804233a4;
    *fifo = float_300_804233a8;
    *fifo = float_0_80423338;
    *fifo = float_0_80423338;
    *fifo = float_300_804233a8;
    *fifo = float_0_80423338;
    *fifo = float_0_80423338;
    *fifo = float_neg100_804233ac;
    *fifo = float_0_80423338;
    *fifo = float_neg500_804233a4;
    *fifo = float_neg100_804233ac;
    *fifo = float_0_80423338;
    GXSetColorUpdate(1);
    return 0;
}

#pragma optimize_for_size off
void winMailDisp(void* pWin) {
    extern s32 pouchCheckMail(s32 id);
    extern void winMailGX(f32 x, f32 y, void* pWin);
    extern void GXSetScissor(s32 x, s32 y, s32 width, s32 height);
    extern void winFontSetWidth(Vec3*, Vec3*, void*, f32, const char*, ...);
    extern char str_msg_menu_mario_802f47d8[];

    extern u32 dat_804232e8;
    extern u32 dat_804232ec;
    extern u32 dat_804232f0;
    extern u32 dat_804232f4;
    extern u32 dat_804232f8;
    extern u32 dat_804232fc;

    extern u32 dat_80423300;
    extern u32 dat_80423304;
    extern u32 dat_80423308;
    extern u32 dat_8042330c;
    extern u32 dat_80423310;
    extern u32 dat_80423314;
    extern u32 dat_80423318;
    extern u32 dat_8042331c;
    extern u32 dat_80423320;
    extern u32 dat_80423324;
    extern u32 dat_80423328;
    extern u32 dat_8042332c;
    extern u32 dat_80423330;

    extern f32 float_2_80423360;
    extern f32 float_3_8042335c;
    extern f32 float_4_8042334c;
    extern f32 float_7_80423390;
    extern f32 float_8_80423370;
    extern f32 float_20_80423398;
    extern f32 float_24_8042339c;
    extern f32 float_32_80423378;
    extern f32 float_56_80423354;
    extern f32 float_72_80423358;
    extern f32 float_80_80423384;
    extern f32 float_88_80423364;
    extern f32 float_104_80423368;
    extern f32 float_120_8042336c;
    extern f32 float_208_80423350;
    extern f32 float_210_8042338c;
    extern f32 float_230_80423374;
    extern f32 float_240_80423380;
    extern f32 float_280_80423388;
    extern f32 float_304_8042337c;
    extern f32 float_320_80423344;
    extern f32 float_360_80423394;
    extern f32 float_neg240_80423348;


    f32 c0, c1, c2, c3, c4, c5, c6, c7, c8, c9;
    Vec3 listBgBase;
    Vec3 listBgPos;
    Vec3 listBgScale;
    Vec3 listStatusBase;
    Vec3 listStatusPos;
    Vec3 listStatusScale;
    Vec3 listTensBase;
    Vec3 listTensPos;
    Vec3 listTensScale;
    Vec3 listOnesBase;
    Vec3 listOnesPos;
    Vec3 listOnesScale;
    Vec3 listSubjectBase;
    Vec3 listSubjectPos;
    Vec3 listSubjectScale;
    Vec3 selBgBase;
    Vec3 selBgPos;
    Vec3 selBgScale;
    Vec3 selStatusBase;
    Vec3 selStatusPos;
    Vec3 selStatusScale;
    Vec3 selTensBase;
    Vec3 selTensPos;
    Vec3 selTensScale;
    Vec3 selOnesBase;
    Vec3 selOnesPos;
    Vec3 selOnesScale;
    Vec3 selSubjectBase;
    Vec3 selSubjectPos;
    Vec3 selSubjectScale;
    Vec3 bodyBgBase;
    Vec3 bodyBgPos;
    Vec3 bodyBgScale;
    Vec3 bodyTextBase;
    Vec3 bodyTextPos;
    Vec3 bodyTextScale;
    Vec3 attachmentBase;
    Vec3 attachmentPos;
    Vec3 attachmentScale;
    Vec3 arrowUpOnlyBase;
    Vec3 arrowUpOnlyPos;
    Vec3 arrowUpOnlyScale;
    Vec3 arrowDownOnlyBase;
    Vec3 arrowDownOnlyPos;
    Vec3 arrowDownOnlyScale;
    Vec3 arrowDownBothBase;
    Vec3 arrowDownBothPos;
    Vec3 arrowDownBothScale;
    Vec3 arrowUpBothBase;
    Vec3 arrowUpBothPos;
    Vec3 arrowUpBothScale;
    Vec3 scrollIndicatorBase;
    Vec3 scrollIndicatorPos;
    Vec3 scrollIndicatorScale;

    u32 drawColor0;
    u32 drawColor1;
    u32 drawColor2;
    u32 drawColor3;
    u32 drawColor4;
    u32 drawColor5;
    u32 drawColor6;
    u32 drawColor7;
    u32 drawColor8;
    u32 drawColor9;
    u32 drawColor10;
    u32 drawColor11;
    u32 drawColor12;
    u32 drawColor13;
    u32 drawColor14;
    u32 drawColor15;
    u32 drawColor16;
    u32 drawColor17;
    u32 listFontColor;
    u32 listBgColor;
    u32 listIconColor;

    u32 selectedNumberColor;
    u32 selectedSubjectColor;
    u32 selectedBgColor;
    u32 selectedBodyColor;
    u32 attachmentColor;
    u32 arrowUpOnlyColor;
    u32 arrowDownOnlyColor;
    u32 arrowDownBothColor;
    u32 arrowUpBothColor;
    u32 scrollIndicatorColor;

    u8* win = (u8*)pWin;
    u8* mailCatalog = (u8*)str_msg_menu_mario_802f47d8;
    u8* mail;
    u8* rowPtr;
    s32 mailId;
    s32 displayNumber;
    s32 count;
    s32 cursor;
    s32 i;
    s32 rowOffset;
    s32 currentLine;
    u16 maxLines;
    u16 flags;
    char* text;

    if (*(f32*)(win + 0x244) < float_320_80423344 &&
        *(f32*)(win + 0x24C) > float_neg240_80423348) {
        winMailGX(*(f32*)(win + 0x244), *(f32*)(win + 0x24C), pWin);

        if (*(s32*)(win + 0x20C) != 0xCA) {
            c0=float_4_8042334c;
            c1=float_208_80423350;
            c2=float_56_80423354;
            c3=float_72_80423358;
            c4=float_3_8042335c;
            c5=float_2_80423360;
            c6=float_88_80423364;
            c7=float_104_80423368;
            c8=float_120_8042336c;
            c9=float_8_80423370;
            rowOffset = 0;
            rowPtr = win;
            count = *(s32*)(win + 0x254);

            for (i = 0; i < count; i++, rowOffset += 0x28, rowPtr += 2) {
                mailId = *(u8*)(rowPtr + 0x26F);
                displayNumber = *(u8*)(rowPtr + 0x26E);
                mail = (u8*)mailGetPtr(mailId);
                flags = *(u16*)mail;

                listFontColor = dat_804232e8;
                if ((flags & 1) != 0) {
                    listBgColor = dat_804232ec;
                } else if ((flags & 2) != 0) {
                    listBgColor = dat_804232f0;
                } else if ((flags & 4) != 0) {
                    listBgColor = dat_804232f4;
                } else {
                    listBgColor = dat_804232f8;
                }

                winTexInit(**(void***)((s32)*(void**)(win + 0x28) + 0xA0));
            listBgScale = *(Vec3*)(mailCatalog + 0x338);
            listBgBase = *(Vec3*)(mailCatalog + 0x32C);
            listBgBase.x = c0 + (c1 + *(f32*)(win + 0x244));
            listBgBase.y = *(f32*)(win + 0x260) + (*(f32*)(win + 0x24C) - c2 - (f32)rowOffset);
            listBgPos = listBgBase;

                drawColor0=listBgColor;

                winTexSet(0x77, &listBgPos, &listBgScale, &drawColor0);

                listIconColor = dat_804232fc;
            listStatusScale = *(Vec3*)(mailCatalog + 0x350);
            listStatusBase = *(Vec3*)(mailCatalog + 0x344);
            listStatusBase.x = c3 + *(f32*)(win + 0x244);
            listStatusBase.y = *(f32*)(win + 0x260) + (*(f32*)(win + 0x24C) - c2 - (f32)rowOffset - c4);
            listStatusPos = listStatusBase;

                drawColor1=listIconColor;

                winTexSet(pouchCheckMail(mailId) == 2 ? 0x5E : 0x5D,
                          &listStatusPos, &listStatusScale, &drawColor1);

            listTensScale = *(Vec3*)(mailCatalog + 0x368);
            listTensBase = *(Vec3*)(mailCatalog + 0x35C);
            listTensBase.x = c5 + (c6 + *(f32*)(win + 0x244));
            listTensBase.y = *(f32*)(win + 0x260) + (*(f32*)(win + 0x24C) - c2 - (f32)rowOffset - c4);
            listTensPos = listTensBase;

                drawColor2=listIconColor;

                winTexSet((displayNumber + 1) / 10 + 0x65,
                          &listTensPos, &listTensScale, &drawColor2);

            listOnesScale = *(Vec3*)(mailCatalog + 0x380);
            listOnesBase = *(Vec3*)(mailCatalog + 0x374);
            listOnesBase.x = c5 + (c7 + *(f32*)(win + 0x244));
            listOnesBase.y = *(f32*)(win + 0x260) + (*(f32*)(win + 0x24C) - c2 - (f32)rowOffset - c4);
            listOnesPos = listOnesBase;

                drawColor3=listIconColor;

                winTexSet((displayNumber + 1) % 10 + 0x65,
                          &listOnesPos, &listOnesScale, &drawColor3);

                winFontInit();
            listSubjectScale = *(Vec3*)(mailCatalog + 0x398);
            listSubjectBase = *(Vec3*)(mailCatalog + 0x38C);
            listSubjectBase.x = c8 + *(f32*)(win + 0x244);
            listSubjectBase.y = *(f32*)(win + 0x260) + c9 + (*(f32*)(win + 0x24C) - c2 - (f32)rowOffset);
            listSubjectPos = listSubjectBase;

                text = msgSearch(*(char**)(mail + 8));
                drawColor15=listFontColor;
                winFontSetWidth(&listSubjectPos, &listSubjectScale,
                                &drawColor15, float_230_80423374, text);
            }

            GXSetScissor(0, 0, 0x260, 0x1E0);
            } else {
            cursor = *(s32*)(win + 0x258);
            displayNumber = *(u8*)(win + 0x26E + cursor * 2);
            mailId = *(u8*)(win + 0x26F + cursor * 2);
            mail = (u8*)mailGetPtr(mailId);
            flags = *(u16*)mail;

            selectedNumberColor = dat_80423300;
            selectedSubjectColor = dat_80423304;

            if ((flags & 1) != 0) {
                selectedBgColor = dat_80423308;
            } else if ((flags & 2) != 0) {
                selectedBgColor = dat_8042330c;
            } else if ((flags & 4) != 0) {
                selectedBgColor = dat_80423310;
            } else {
                selectedBgColor = dat_80423314;
            }

            winTexInit(**(void***)((s32)*(void**)(win + 0x28) + 0xA0));
            selBgScale = *(Vec3*)(mailCatalog + 0x3B0);
            selBgBase = *(Vec3*)(mailCatalog + 0x3A4);
            selBgBase.x = float_4_8042334c + float_208_80423350 + *(f32*)(win + 0x244);
            selBgBase.y = *(f32*)(win + 0x24C) - float_56_80423354;
            selBgPos = selBgBase;

            drawColor4=selectedBgColor;

            winTexSet(0x77, &selBgPos, &selBgScale, &drawColor4);

            selStatusScale = *(Vec3*)(mailCatalog + 0x3C8);
            selStatusBase = *(Vec3*)(mailCatalog + 0x3BC);
            selStatusBase.x = float_72_80423358 + *(f32*)(win + 0x244);
            selStatusBase.y = *(f32*)(win + 0x24C) - float_56_80423354;
            selStatusPos = selStatusBase;

            drawColor5=selectedNumberColor;

            winTexSet(0x5E, &selStatusPos, &selStatusScale, &drawColor5);

            selTensScale = *(Vec3*)(mailCatalog + 0x3E0);
            selTensBase = *(Vec3*)(mailCatalog + 0x3D4);
            selTensBase.x = float_2_80423360 + float_88_80423364 + *(f32*)(win + 0x244);
            selTensBase.y = *(f32*)(win + 0x24C) - float_56_80423354 - float_3_8042335c;
            selTensPos = selTensBase;

            drawColor6=selectedNumberColor;

            winTexSet((displayNumber + 1) / 10 + 0x65,
                      &selTensPos, &selTensScale, &drawColor6);

            selOnesScale = *(Vec3*)(mailCatalog + 0x3F8);
            selOnesBase = *(Vec3*)(mailCatalog + 0x3EC);
            selOnesBase.x = float_2_80423360 + float_104_80423368 + *(f32*)(win + 0x244);
            selOnesBase.y = *(f32*)(win + 0x24C) - float_56_80423354 - float_3_8042335c;
            selOnesPos = selOnesBase;

            drawColor7=selectedNumberColor;

            winTexSet((displayNumber + 1) % 10 + 0x65,
                      &selOnesPos, &selOnesScale, &drawColor7);

            winFontInit();
            selSubjectScale = *(Vec3*)(mailCatalog + 0x410);
            selSubjectBase = *(Vec3*)(mailCatalog + 0x404);
            selSubjectBase.x = float_120_8042336c + *(f32*)(win + 0x244);
            selSubjectBase.y = *(f32*)(win + 0x24C) - float_56_80423354 + float_8_80423370;
            selSubjectPos = selSubjectBase;

            text = msgSearch(*(char**)(mail + 8));
            drawColor16=selectedSubjectColor;
            winFontSetWidth(&selSubjectPos, &selSubjectScale,
                            &drawColor16, float_230_80423374, text);

            GXSetScissor(
                (u32)(*(f32*)(win + 0x244) +
                      float_304_8042337c + float_32_80423378),
                (u32)(float_72_80423358 +
                      (float_240_80423380 - *(f32*)(win + 0x24C))),
                0x158, 0xB1);

            winTexInit(**(void***)((s32)*(void**)(win + 0x28) + 0xA0));
            bodyBgScale = *(Vec3*)(mailCatalog + 0x428);
            bodyBgBase = *(Vec3*)(mailCatalog + 0x41C);
            bodyBgBase.x = float_4_8042334c + float_208_80423350 + *(f32*)(win + 0x244);
            bodyBgBase.y = *(f32*)(win + 0x24C) - float_56_80423354 - float_80_80423384;
            bodyBgPos = bodyBgBase;

            attachmentColor = selectedBgColor;
            drawColor8=attachmentColor;
            winTexSet(0x77, &bodyBgPos, &bodyBgScale, &drawColor8);

            winFontInit();
            selectedBodyColor = dat_80423318;
            currentLine = *(s32*)(win + 0x268);
            bodyTextScale = *(Vec3*)(mailCatalog + 0x440);
            bodyTextBase = *(Vec3*)(mailCatalog + 0x434);
            bodyTextBase.x = float_80_80423384 + *(f32*)(win + 0x244);
            bodyTextBase.y = *(f32*)(win + 0x24C) - float_80_80423384 + (f32)(currentLine * 0x15);
            bodyTextPos = bodyTextBase;

            text = msgSearch(*(char**)(mail + 0x0C));
            drawColor17=selectedBodyColor;
            winFontSetWidth(&bodyTextPos, &bodyTextScale,
                            &drawColor17, float_280_80423388, text);

            if ((flags & 0x1000) != 0) {
                maxLines = *(u16*)(win + 0x26C);
                winTexInit(**(void***)((s32)*(void**)(win + 0x2C) + 0xA0));
                attachmentColor = dat_8042331c;
            attachmentScale = *(Vec3*)(mailCatalog + 0x458);
            attachmentBase = *(Vec3*)(mailCatalog + 0x44C);
            attachmentBase.x = float_210_8042338c + *(f32*)(win + 0x244);
            attachmentBase.y = float_7_80423390 + (*(f32*)(win + 0x24C) - (f32)((maxLines - 1) * 0x15)) + (f32)(currentLine * 0x15);
            attachmentPos = attachmentBase;

                drawColor9=attachmentColor;

                winTexSet(*(s32*)(mail + 0x10), &attachmentPos,
                          &attachmentScale, &drawColor9);
            }

            GXSetScissor(0, 0, 0x260, 0x1E0);

            maxLines = *(u16*)(win + 0x26C);
            if (maxLines > 8) {
                s32 blink = ((*(u32*)((s32)gp + 0x1C) & 0x1F) < 0x14);
                winIconInit();

                if (currentLine == 0) {
                    if (blink != 0) {
                        arrowUpOnlyColor = dat_80423320;
            arrowUpOnlyScale = *(Vec3*)(mailCatalog + 0x470);
            arrowUpOnlyBase = *(Vec3*)(mailCatalog + 0x464);
            arrowUpOnlyBase.x = float_360_80423394 + *(f32*)(win + 0x244);
            arrowUpOnlyBase.y = float_20_80423398 + (*(f32*)(win + 0x24C) - float_230_80423374) - float_24_8042339c;
            arrowUpOnlyPos = arrowUpOnlyBase;

                        drawColor10=arrowUpOnlyColor;

                        winIconSet(0x1BE, &arrowUpOnlyPos,
                                   &arrowUpOnlyScale, &drawColor10);
                    }
                } else if ((u32)(currentLine + 6) == (u32)maxLines) {
                    if (blink != 0) {
                        arrowDownOnlyColor = dat_80423324;
            arrowDownOnlyScale = *(Vec3*)(mailCatalog + 0x488);
            arrowDownOnlyBase = *(Vec3*)(mailCatalog + 0x47C);
            arrowDownOnlyBase.x = float_360_80423394 + *(f32*)(win + 0x244);
            arrowDownOnlyBase.y = float_24_8042339c + float_20_80423398 + (*(f32*)(win + 0x24C) - float_230_80423374);
            arrowDownOnlyPos = arrowDownOnlyBase;

                        drawColor11=arrowDownOnlyColor;

                        winIconSet(0x1BD, &arrowDownOnlyPos,
                                   &arrowDownOnlyScale, &drawColor11);
                    }
                } else if (blink != 0) {
                    arrowDownBothColor = dat_80423328;
            arrowDownBothScale = *(Vec3*)(mailCatalog + 0x4A0);
            arrowDownBothBase = *(Vec3*)(mailCatalog + 0x494);
            arrowDownBothBase.x = float_360_80423394 + *(f32*)(win + 0x244);
            arrowDownBothBase.y = float_24_8042339c + float_20_80423398 + (*(f32*)(win + 0x24C) - float_230_80423374);
            arrowDownBothPos = arrowDownBothBase;

                    drawColor12=arrowDownBothColor;

                    winIconSet(0x1BD, &arrowDownBothPos,
                               &arrowDownBothScale, &drawColor12);

                    arrowUpBothColor = dat_8042332c;
            arrowUpBothScale = *(Vec3*)(mailCatalog + 0x4B8);
            arrowUpBothBase = *(Vec3*)(mailCatalog + 0x4AC);
            arrowUpBothBase.x = float_360_80423394 + *(f32*)(win + 0x244);
            arrowUpBothBase.y = float_20_80423398 + (*(f32*)(win + 0x24C) - float_230_80423374) - float_24_8042339c;
            arrowUpBothPos = arrowUpBothBase;

                    drawColor13=arrowUpBothColor;

                    winIconSet(0x1BE, &arrowUpBothPos,
                               &arrowUpBothScale, &drawColor13);
                }

                scrollIndicatorColor = dat_80423330;
            scrollIndicatorScale = *(Vec3*)(mailCatalog + 0x4D0);
            scrollIndicatorBase = *(Vec3*)(mailCatalog + 0x4C4);
            scrollIndicatorBase.x = float_360_80423394 + *(f32*)(win + 0x244);
            scrollIndicatorBase.y = float_20_80423398 + (*(f32*)(win + 0x24C) - float_230_80423374);
            scrollIndicatorPos = scrollIndicatorBase;

                drawColor14=scrollIndicatorColor;

                winIconSet(0x78, &scrollIndicatorPos,
                           &scrollIndicatorScale, &drawColor14);
            }
            }
    }
}
#pragma optimize_for_size reset

void unk_80152e80(s32 cameraId, void* win) {
    extern void GXSetTexCopySrc(s32 x, s32 y, s32 width, s32 height);
    extern void GXSetTexCopyDst(s32 width, s32 height, s32 format, s32 mipmap);
    extern void GXCopyTex(void* dst, s32 clear);
    extern void GXTexModeSync(void);
    extern void N_mapDispOff(void);
    extern void L_camDispOff(s32 cameraId);
    void* gpWork;

    gpWork = gp;
    GXSetTexCopySrc(0, 0, *(u16*)((s32)gpWork + 0x170), *(u16*)((s32)gpWork + 0x172));
    gpWork = gp;
    GXSetTexCopyDst(*(u16*)((s32)gpWork + 0x170), *(u16*)((s32)gpWork + 0x172), 4, 0);
    GXCopyTex(**(void***)((s32)win + 0x34), 0);
    GXTexModeSync();
    *(s32*)((s32)win + 0x38) = 1;
    N_mapDispOff();
    L_camDispOff(4);
}


/* CHATGPT FALLBACK MISSING STUBS: main/window/win_root 20260624_191429 */

/* fallback stub-fill: map=unk_80152bdc addr=0x80152bdc size=0x000002a4 */
void unk_80152bdc(s32 cameraId, void* win) {
    extern void GXInitTexObj(void* texObj, void* image, u16 width, u16 height, s32 fmt, s32 wrapS, s32 wrapT, s32 mipmap);
    extern u32 dat_80423334;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    u32 color;
    s32 texObj[8];
    f32 halfW;
    f32 halfH;

    if (*(s32*)((s32)win + 0x38) != 0) {
        color = dat_80423334;
        GXSetFog(0, float_0_80423338, float_0_80423338, float_0_80423338, float_0_80423338, &color);
        GXSetNumChans(0);
        GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevOp(0, 3);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetZMode(0, 7, 0);
        GXLoadPosMtxImm((void*)((s32)camGetPtr(8) + 0x11C), 0);
        GXSetCurrentMtx(0);
        GXInitTexObj(texObj, **(void***)((s32)win + 0x34), *(u16*)((s32)gp + 0x170), *(u16*)((s32)gp + 0x172), 4, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetCullMode(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxDesc(0xD, 1);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        GXBegin(0x80, 0, 4);
        halfW = (f32)*(u16*)((s32)gp + 0x170) * float_0p5_8042333c;
        halfH = (f32)*(u16*)((s32)gp + 0x172) * float_0p5_8042333c;
        *fifo = -halfW;
        *fifo = -halfH;
        *fifo = float_0_80423338;
        *fifo = float_0_80423338;
        *fifo = float_1_80423340;
        *fifo = halfW;
        *fifo = -halfH;
        *fifo = float_0_80423338;
        *fifo = float_1_80423340;
        *fifo = float_1_80423340;
        *fifo = halfW;
        *fifo = halfH;
        *fifo = float_0_80423338;
        *fifo = float_1_80423340;
        *fifo = float_0_80423338;
        *fifo = -halfW;
        *fifo = halfH;
        *fifo = float_0_80423338;
        *fifo = float_0_80423338;
        *fifo = float_0_80423338;
    }
}

char _tmp_1042[64];

/*
 * Sort-label strings remain target-defined symbolic references in A2.
 * The separate read-only catalog is not part of this transaction.
 */
extern const char str_msg_menu_sort_aiueo_802f4a54[];
extern const char str_msg_menu_sort_kind_802f4a68[];
extern const char str_msg_menu_sort_bp_802f4a7c[];
extern const char str_msg_menu_sort_number_802f4a90[];
extern const char str_msg_menu_sort_place_802f4aa8[];

typedef struct WinRootSortOption {
    const char* message;
    void (*callback)(void*);
} WinRootSortOption;

WinRootSortOption sort_1[3] = {
    { str_msg_menu_sort_aiueo_802f4a54, (void (*)(void*))sort_1_1_func },
    { str_msg_menu_sort_kind_802f4a68, (void (*)(void*))sort_1_2_func },
    { 0, 0 },
};

WinRootSortOption sort_2[3] = {
    { str_msg_menu_sort_aiueo_802f4a54, (void (*)(void*))sort_2_1_func },
    { str_msg_menu_sort_kind_802f4a68, (void (*)(void*))sort_2_2_func },
    { 0, 0 },
};

WinRootSortOption sort_3[4] = {
    { str_msg_menu_sort_aiueo_802f4a54, (void (*)(void*))sort_3_1_func },
    { str_msg_menu_sort_kind_802f4a68, (void (*)(void*))sort_3_2_func },
    { str_msg_menu_sort_bp_802f4a7c, (void (*)(void*))sort_3_3_func },
    { 0, 0 },
};

WinRootSortOption sort_4[4] = {
    { str_msg_menu_sort_aiueo_802f4a54, (void (*)(void*))sort_4_1_func },
    { str_msg_menu_sort_kind_802f4a68, (void (*)(void*))sort_4_2_func },
    { str_msg_menu_sort_bp_802f4a7c, (void (*)(void*))sort_4_3_func },
    { 0, 0 },
};

WinRootSortOption sort_5[4] = {
    { str_msg_menu_sort_aiueo_802f4a54, (void (*)(void*))sort_5_1_func },
    { str_msg_menu_sort_kind_802f4a68, (void (*)(void*))sort_5_2_func },
    { str_msg_menu_sort_bp_802f4a7c, (void (*)(void*))sort_5_3_func },
    { 0, 0 },
};

WinRootSortOption sort_6[3] = {
    { str_msg_menu_sort_aiueo_802f4a54, (void (*)(void*))sort_6_1_func },
    { str_msg_menu_sort_kind_802f4a68, (void (*)(void*))sort_6_2_func },
    { 0, 0 },
};

WinRootSortOption sort_7[4] = {
    { str_msg_menu_sort_number_802f4a90, (void (*)(void*))sort_7_1_func },
    { str_msg_menu_sort_place_802f4aa8, (void (*)(void*))sort_7_2_func },
    { str_msg_menu_sort_aiueo_802f4a54, (void (*)(void*))sort_7_3_func },
    { 0, 0 },
};

WinRootSortOption sort_8[3] = {
    { str_msg_menu_sort_aiueo_802f4a54, (void (*)(void*))sort_8_1_func },
    { str_msg_menu_sort_number_802f4a90, (void (*)(void*))sort_8_2_func },
    { 0, 0 },
};

void* sort_data[8] = { sort_1, sort_2, sort_3, sort_4, sort_5, sort_6, sort_7, sort_8 };

/*
 * Data-pass A5: physical .sdata2 root-window color/control catalog.
 * Retail places these 47 four-byte GLOBAL entries at +0x000..+0x0BB,
 * immediately before the room-prefix string family at +0x0BC.
 * Define them after all executable consumers to preserve declaration-only
 * visibility in the earlier function bodies. No padding/pool objects.
 */
const u32 dat_804231a8 = 0xFFFFFFFF;
const u32 dat_804231ac = 0x00000080;
const u32 dat_804231b0 = 0xFFFFFFFE;
const u32 dat_804231b4 = 0x808080FE;
const u32 dat_804231b8 = 0xC86660FF;
const u32 dat_804231bc = 0xE1A5A1FF;
const u32 dat_804231c0 = 0xF0CDCBFF;
const u32 dat_804231c4 = 0xC86B96FF;
const u32 dat_804231c8 = 0xDBABC6FF;
const u32 dat_804231cc = 0xECD1E1FF;
/* Target auto-name implies float; physical value is packed RGBA. */
const u32 float_neg0p0047648_804231d0 = 0xBB9C21FF;
const u32 dat_804231d4 = 0xD7CD76FF;
const u32 dat_804231d8 = 0xEAE5ADFF;
const u32 dat_804231dc = 0x2FA530FF;
const u32 dat_804231e0 = 0xA2D26EFF;
const u32 dat_804231e4 = 0xCBE7A7FF;
const u32 dat_804231e8 = 0x287EC5FF;
const u32 dat_804231ec = 0x78B3D6FF;
const u32 dat_804231f0 = 0xAFD6EAFF;
const u32 dat_804231f4 = 0xFFFFFFFF;
const u32 dat_804231f8 = 0xFFFFFFFF;
const u32 dat_804231fc = 0x000000FF;
const u32 dat_80423200 = 0xFFFFFFFF;
const u32 dat_80423204 = 0xFFFFFFFF;
const u32 dat_80423208 = 0xFFFFFFFF;
const u32 dat_8042320c = 0xFFFFFFFF;
const u32 dat_80423210 = 0xFFFFFFFF;
const u32 dat_80423214 = 0xFFFFFFFF;
const u32 dat_80423218 = 0xFFFFFFFF;
const u32 dat_8042321c = 0xFFFFFFFF;
const u32 dat_80423220 = 0xFFFFFFFF;
const u32 dat_80423224 = 0x000000FF;
const u32 dat_80423228 = 0x000000FF;
const u32 dat_8042322c = 0xFFFFFFFF;
const u32 dat_80423230 = 0x00000080;
const u32 dat_80423234 = 0xFFFFFFFF;
const u32 dat_80423238 = 0xFFFFFFFF;
const u32 dat_8042323c = 0xFFFFFFFF;
const u32 dat_80423240 = 0xFFFFFFFF;
const u32 dat_80423244 = 0xFFFFFFFF;
const u32 dat_80423248 = 0xFFFFFFFF;
const u32 dat_8042324c = 0xFFFFFFFF;
const u32 dat_80423250 = 0xFFFFFFFF;
const u32 dat_80423254 = 0xFFFFFFFF;
const u32 dat_80423258 = 0x000000FF;
const u32 dat_8042325c = 0xFFFFFFFF;
const u32 dat_80423260 = 0xFFFFFFFF;

/* Read-only room prefixes referenced by the 26-entry tattle directory. */
const char str_nara_80423264[] __attribute__((aligned(4))) = "nara";
const char str_gor_8042326c[] __attribute__((aligned(4))) = "gor";
const char str_tik_80423270[] __attribute__((aligned(4))) = "tik";
const char str_shiga_80423274[] __attribute__((aligned(4))) = "shiga";
const char str_hei_8042327c[] __attribute__((aligned(4))) = "hei";
const char str_nok_80423280[] __attribute__((aligned(4))) = "nok";
const char str_stn_80423284[] __attribute__((aligned(4))) = "stn";
const char str_gon_80423288[] __attribute__((aligned(4))) = "gon";
const char str_win_8042328c[] __attribute__((aligned(4))) = "win";
const char str_hou_80423290[] __attribute__((aligned(4))) = "hou";
const char str_mri_80423294[] __attribute__((aligned(4))) = "mri";
const char str_tou_80423298[] __attribute__((aligned(4))) = "tou";
const char str_usu_8042329c[] __attribute__((aligned(4))) = "usu";
const char str_gra_804232a0[] __attribute__((aligned(4))) = "gra";
const char str_jin_804232a4[] __attribute__((aligned(4))) = "jin";
const char str_muj_804232a8[] __attribute__((aligned(4))) = "muj";
const char str_dou_804232ac[] __attribute__((aligned(4))) = "dou";
const char str_hom_804232b0[] __attribute__((aligned(4))) = "hom";
const char str_eki_804232b4[] __attribute__((aligned(4))) = "eki";
const char str_rsh_804232b8[] __attribute__((aligned(4))) = "rsh";
const char str_pik_804232bc[] __attribute__((aligned(4))) = "pik";
const char str_sin_804232c0[] __attribute__((aligned(4))) = "sin";
const char str_bom_804232c4[] __attribute__((aligned(4))) = "bom";
const char str_moo_804232c8[] __attribute__((aligned(4))) = "moo";
const char str_aji_804232cc[] __attribute__((aligned(4))) = "aji";
const char str_las_804232d0[] __attribute__((aligned(4))) = "las";

/*
 * Data-pass A9: complete target-backed root-menu Vec3/sorting-label catalog.
 * Preserve the five A8 menu labels as natural msgHelp string literals: defining
 * that common catalog base explicitly regressed winRootDisp/winMailDisp in A7.
 * This late block contains only 88 real vectors and five sort-label strings;
 * no function body, jump table, compiler pool or gap is hand-authored.
 */
const Vec3 vec3_802f482c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4838 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4844 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4850 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f485c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4868 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4874 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4880 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f488c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4898 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f48a4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f48b0 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f48bc = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f48c8 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f48d4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f48e0 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f48ec = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f48f8 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f4904 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4910 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f491c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4928 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f4934 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4940 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f494c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4958 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f4964 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4970 = { 0.8f, 0.8f, 0.8f };
const Vec3 vec3_802f497c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4988 = { 0.8f, 0.8f, 0.8f };
const Vec3 vec3_802f4994 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f49a0 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f49ac = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f49b8 = { 0.8f, 0.8f, 0.8f };
const Vec3 vec3_802f49c4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f49d0 = { -1.0f, -1.0f, -1.0f };
const Vec3 vec3_802f49dc = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f49e8 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f49f4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4a00 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4a0c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4a18 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f4a24 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4a30 = { 0.7f, 0.7f, 0.7f };
const Vec3 vec3_802f4a3c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4a48 = { 0.6f, 0.6f, 0.6f };
const char str_msg_menu_sort_aiueo_802f4a54[] __attribute__((aligned(4))) = "msg_menu_sort_aiueo";
const char str_msg_menu_sort_kind_802f4a68[] __attribute__((aligned(4))) = "msg_menu_sort_kind";
const char str_msg_menu_sort_bp_802f4a7c[] __attribute__((aligned(4))) = "msg_menu_sort_bp";
const char str_msg_menu_sort_number_802f4a90[] __attribute__((aligned(4))) = "msg_menu_sort_number";
const char str_msg_menu_sort_place_802f4aa8[] __attribute__((aligned(4))) = "msg_menu_sort_place";
const Vec3 vec3_802f4abc = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4ac8 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4ad4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4ae0 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4aec = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4af8 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4b04 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4b10 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4b1c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4b28 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4b34 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4b40 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4b4c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4b58 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4b64 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4b70 = { 0.75f, 0.75f, 0.75f };
const Vec3 vec3_802f4b7c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4b88 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4b94 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4ba0 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4bac = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4bb8 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4bc4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4bd0 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4bdc = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4be8 = { 0.75f, 0.75f, 0.75f };
const Vec3 vec3_802f4bf4 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4c00 = { 1.0f, 7.5f, 1.0f };
const Vec3 vec3_802f4c0c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4c18 = { 0.75f, 0.75f, 0.75f };
const Vec3 vec3_802f4c24 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4c30 = { 1.0f, 1.0f, 1.0f };
const Vec3 vec3_802f4c3c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4c48 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f4c54 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4c60 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f4c6c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4c78 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f4c84 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4c90 = { 0.6f, 0.6f, 0.6f };
const Vec3 vec3_802f4c9c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f4ca8 = { 0.6f, 0.6f, 0.6f };
