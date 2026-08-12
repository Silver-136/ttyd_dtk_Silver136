#include "window/win_root.h"

extern f32 float_neg1000_804233a0;
void* sort_data[8];
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
extern char* tbl_1614[];
extern char str_menu_monosiri_PCTs_802f4d00[];
extern char str_btl_un_hatena_802f4ce0[];
extern char str_btl_un_rampell_802f4cf0[];

s32 winRootMain(void* pWin) {
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
    extern double intplGetValue(double start, double end, s32 type, s32 time, s32 duration);
    u8* w = (u8*)pWin;
    s32 state = *(s32*)(w + 0x24);
    s32 cursor = *(s32*)(w + 0x40);
    s32 count = *(s32*)(w + 0x44);
    s32 result = 0;
    s32 i;
    u8* entry;
    u8* body;

    switch (state) {
    case 0:
        psndSFXOn(0x2002A);
        count = 0;
        *(s32*)(w + 0x44) = count;

        entry = w + 0x48 + count * 0x18;
        *(s32*)(entry + 0x00) = 0;
        *(f32*)(entry + 0x04) = 1000.0f;
        *(f32*)(entry + 0x08) = 1000.0f;
        *(s32*)(entry + 0x0C) = 0;
        *(void**)(entry + 0x14) = 0;
        count++;

        if (swByteGet(0) > 6 || partyChkJoin(1) != 0) {
            entry = w + 0x48 + count * 0x18;
            *(s32*)(entry + 0x00) = 1;
            *(f32*)(entry + 0x04) = 1000.0f;
            *(f32*)(entry + 0x08) = 1000.0f;
            *(s32*)(entry + 0x0C) = 0;
            *(void**)(entry + 0x14) = 0;
            count++;
        }

        entry = w + 0x48 + count * 0x18;
        *(s32*)(entry + 0x00) = 2;
        *(f32*)(entry + 0x04) = 1000.0f;
        *(f32*)(entry + 0x08) = 1000.0f;
        *(s32*)(entry + 0x0C) = 0;
        *(void**)(entry + 0x14) = 0;
        count++;

        if (swByteGet(0) > 0x13 || pouchGetHaveBadgeCnt() != 0) {
            entry = w + 0x48 + count * 0x18;
            *(s32*)(entry + 0x00) = 3;
            *(f32*)(entry + 0x04) = 1000.0f;
            *(f32*)(entry + 0x08) = 1000.0f;
            *(s32*)(entry + 0x0C) = 0;
            *(void**)(entry + 0x14) = 0;
            count++;
        }

        entry = w + 0x48 + count * 0x18;
        *(s32*)(entry + 0x00) = 4;
        *(f32*)(entry + 0x04) = 1000.0f;
        *(f32*)(entry + 0x08) = 1000.0f;
        *(s32*)(entry + 0x0C) = 0;
        *(void**)(entry + 0x14) = 0;
        count++;
        *(s32*)(w + 0x44) = count;

        for (i = 0; i < count; i++) {
            if (*(s32*)(w + 0x48 + i * 0x18) == *(s32*)(w + 0x28)) {
                cursor = i;
            }
        }
        *(s32*)(w + 0x40) = cursor;

        for (i = 0; i < 5; i++) {
            body = w + 0xCC + i * 0x14;
            *(s32*)(body + 0x00) = -1;
            *(f32*)(body + 0x04) = 1000.0f;
            *(f32*)(body + 0x08) = 1000.0f;
            *(s32*)(body + 0x0C) = -1;
        }
        body = w + 0xCC;
        *(s32*)(body + 0x00) = *(s32*)(w + 0x48 + cursor * 0x18);
        *(s32*)(body + 0x0C) = 0;

        winMarioInit(pWin);
        winPartyInit(pWin);
        winItemInit(pWin);
        winBadgeInit(pWin);
        winLogInit(pWin);
        *(s32*)(w + 0x24) = 10;
        state = 10;
        break;
    case 10:
        body = w + 0xCC;
        if ((*(u32*)(w + 4) & 0x1200) != 0) {
            if (*(s32*)(body + 0x0C) == 100) {
                *(s32*)(w + 0x24) = 0x14;
            }
        } else if ((*(u32*)(w + 4) & 0x100) != 0) {
            s32 menu = *(s32*)(w + 0x48 + cursor * 0x18);
            if (menu != 1 || *(s32*)(w + 0x1E0) != 0) {
                psndSFXOn(0x20012);
                *(s32*)(w + 0x24) = menu + 0xB;
                if (menu == 0) winMarioInit2(pWin);
                else if (menu == 1) winPartyInit2(pWin);
                else if (menu == 2) winItemInit2(pWin);
                else if (menu == 3) winBadgeInit2(pWin);
                else winLogInit2(pWin);
            }
        } else if ((*(u32*)(w + 0x10) & 0x4000) != 0 && *(s32*)(body + 0x0C) == 100) {
            s32 j;
            psndSFXOn(0x20032);
            psndSFXOn(0x20031);
            cursor--;
            if (cursor < 0) cursor = count - 1;
            for (j = 4; j > 0; j--) {
                u8* dst = w + 0xCC + j * 0x14;
                u8* src = dst - 0x14;
                *(u32*)(dst + 0x00) = *(u32*)(src + 0x00);
                *(u32*)(dst + 0x04) = *(u32*)(src + 0x04);
                *(u32*)(dst + 0x08) = *(u32*)(src + 0x08);
                *(u32*)(dst + 0x0C) = *(u32*)(src + 0x0C);
                *(u32*)(dst + 0x10) = *(u32*)(src + 0x10);
            }
            *(s32*)(w + 0x40) = cursor;
            *(s32*)(body + 0x00) = *(s32*)(w + 0x48 + cursor * 0x18);
            *(s32*)(body + 0x0C) = 0x1E;
        } else if ((*(u32*)(w + 0x10) & 0x8000) != 0 && *(s32*)(body + 0x0C) == 100) {
            s32 j;
            psndSFXOn(0x20032);
            psndSFXOn(0x20031);
            cursor++;
            if (cursor >= count) cursor = 0;
            for (j = 0; j < 4; j++) {
                u8* dst = w + 0xCC + j * 0x14;
                u8* src = dst + 0x14;
                *(u32*)(dst + 0x00) = *(u32*)(src + 0x00);
                *(u32*)(dst + 0x04) = *(u32*)(src + 0x04);
                *(u32*)(dst + 0x08) = *(u32*)(src + 0x08);
                *(u32*)(dst + 0x0C) = *(u32*)(src + 0x0C);
                *(u32*)(dst + 0x10) = *(u32*)(src + 0x10);
            }
            *(s32*)(w + 0x40) = cursor;
            *(s32*)(body + 0x00) = *(s32*)(w + 0x48 + cursor * 0x18);
            *(s32*)(body + 0x0C) = 0x14;
        }
        break;
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
    case 0x14:
        psndSFXOn(0x2002B);
        for (i = 0; i < count; i++) {
            *(s32*)(w + 0x48 + i * 0x18 + 0x0C) = 10;
        }
        for (i = 0; i < 5; i++) {
            *(s32*)(w + 0xCC + i * 0x14 + 0x0C) = 10;
        }
        N_mapDispOn();
        L_camDispOn(4);
        *(s32*)(w + 0x24) = state + 1;
        break;
    case 100:
        if (*(s32*)(w + 0xD8) == 100) {
            *(s32*)(w + 0x24) = 10;
        }
        break;
    }

    if (result == -1) {
        *(s32*)(w + 0x24) = 10;
    }

    for (i = 0; i < 5; i++) {
        entry = w + 0x48 + i * 0x18;
        switch (*(s32*)(entry + 0x0C)) {
            case 0:
                *(f32*)(entry + 0x04) = (f32)(-205 + i * 105);
                *(f32*)(entry + 0x08) = 300.0f;
                *(s32*)(entry + 0x10) = i * 4;
                *(s32*)(entry + 0x0C) = 1;
                break;
            case 1:
            case 0xB:
                *(s32*)(entry + 0x10) -= 1;
                if (*(s32*)(entry + 0x10) < 0) {
                    *(s32*)(entry + 0x10) = 0;
                    *(s32*)(entry + 0x0C) += 1;
                }
                break;
            case 2:
                *(f32*)(entry + 0x08) = (f32)intplGetValue(
                    300.0, 185.0, 0xB, *(s32*)(entry + 0x10), 10);
                *(s32*)(entry + 0x10) += 1;
                if (*(s32*)(entry + 0x10) > 10) {
                    *(s32*)(entry + 0x0C) += 1;
                }
                break;
            case 10:
                *(s32*)(entry + 0x10) = (4 - i) * 4;
                *(s32*)(entry + 0x0C) = 0xB;
                break;
            case 0xC:
                *(f32*)(entry + 0x08) = (f32)intplGetValue(
                    180.0, 300.0, 0xB, *(s32*)(entry + 0x10), 10);
                *(s32*)(entry + 0x10) += 1;
                if (*(s32*)(entry + 0x10) > 10) {
                    *(s32*)(entry + 0x0C) += 1;
                }
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
    return 0;
}
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
    extern void winMailDisp(void* win);
    extern void winMsgDisp(f32 x, f32 y, void* win);
    extern void winSortGX(void* win);
    extern char str_msg_menu_mario_802f47d8[];
    extern u32 dat_804231a8;
    extern u32 dat_804231ac;
    extern u32 dat_804231b0;
    extern u32 dat_804231b4;

    char* base = str_msg_menu_mario_802f47d8;
    u32 color;
    u32 pos[3];
    u32 size[3];
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
            color = dat_804231b0;
        } else {
            color = dat_804231b4;
        }

        fileData = **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0);
        winTexInit_x2(fileData);

        size[0] = *(u32*)(base + 0x90);
        size[1] = *(u32*)(base + 0x94);
        size[2] = *(u32*)(base + 0x98);
        pos[0] = *(u32*)(base + 0x84);
        pos[1] = *(u32*)(base + 0x88);
        pos[2] = *(u32*)(base + 0x8C);
        *(f32*)&pos[0] = *(f32*)((s32)header + 4);
        *(f32*)&pos[1] = *(f32*)((s32)header + 8);
        winTexSet_x2(*(s32*)header + 1, 0xB2, pos, size, &color);
        header = (void*)((s32)header + 0x18);
    }

    winMailDisp(pWin);
    winMsgDisp(*(f32*)((s32)pWin + 0x128), *(f32*)((s32)pWin + 0x12C), pWin);
    winSortGX(pWin);

    if ((*(u16*)pWin & 0x8000) == 0) {
        fileData = **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0);
        winTexInit(fileData);

        color = dat_804231ac;
        size[0] = *(u32*)(base + 0x60);
        size[1] = *(u32*)(base + 0x64);
        size[2] = *(u32*)(base + 0x68);
        pos[0] = *(u32*)(base + 0x54);
        pos[1] = *(u32*)(base + 0x58);
        pos[2] = *(u32*)(base + 0x5C);
        *(f32*)&pos[0] = *(f32*)((s32)pWin + 0x150) + float_2_80423360;
        *(f32*)&pos[1] = *(f32*)((s32)pWin + 0x154) - float_2_80423360;
        winTexSet(0, pos, size, &color);

        color = dat_804231a8;
        size[0] = *(u32*)(base + 0x78);
        size[1] = *(u32*)(base + 0x7C);
        size[2] = *(u32*)(base + 0x80);
        pos[0] = *(u32*)(base + 0x6C);
        pos[1] = *(u32*)(base + 0x70);
        pos[2] = *(u32*)(base + 0x74);
        *(f32*)&pos[0] = *(f32*)((s32)pWin + 0x150);
        *(f32*)&pos[1] = *(f32*)((s32)pWin + 0x154);
        winTexSet(0, pos, size, &color);
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winBgMain(void* pWin) {
    extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 time, s32 duration);
    extern f32 float_800_804234a4;
    extern f32 float_640_804234a8;
    extern f32 float_neg640_804234ac;

    s32 i = 0;
    s32 state;
    s32 timer;
    s32 duration;
    void* entry = pWin;

    do {
        state = *(s32*)((s32)entry + 0xCC);
        switch (state) {
            case 0:
                *(f32*)((s32)entry + 0xC4) = float_800_804234a4;
                *(f32*)((s32)entry + 0xC8) = float_0_80423338;
                *(s32*)((s32)entry + 0xD0) = 0;
                *(s32*)((s32)entry + 0xCC) = state + 1;
                break;
            case 1:
                timer = *(s32*)((s32)entry + 0xD0);
                *(s32*)((s32)entry + 0xD0) = timer + 1;
                duration = (*(s32*)((s32)gp + 4) << 4) / 60;
                *(f32*)((s32)entry + 0xC4) =
                    intplGetValue(float_800_804234a4, float_0_80423338, 0xB, timer, duration);
                duration = (*(s32*)((s32)gp + 4) << 4) / 60;
                if (*(s32*)((s32)entry + 0xD0) > duration) {
                    *(s32*)((s32)entry + 0xCC) = 100;
                }
                break;
            case 10:
                *(f32*)((s32)entry + 0xC8) = float_0_80423338;
                *(s32*)((s32)entry + 0xD0) = 0;
                *(s32*)((s32)entry + 0xCC) = state + 1;
                break;
            case 11:
                timer = *(s32*)((s32)entry + 0xD0);
                *(s32*)((s32)entry + 0xD0) = timer + 1;
                duration = (*(s32*)((s32)gp + 4) << 4) / 60;
                *(f32*)((s32)entry + 0xC4) =
                    intplGetValue(float_0_80423338, float_800_804234a4, 0xB, timer, duration);
                duration = (*(s32*)((s32)gp + 4) << 4) / 60;
                if (*(s32*)((s32)entry + 0xD0) > duration) {
                    *(s32*)((s32)entry + 0xCC) = -1;
                }
                break;
            case 20:
                *(f32*)((s32)entry + 0xC8) = float_0_80423338;
                *(s32*)((s32)entry + 0xD0) = 0;
                *(s32*)((s32)entry + 0xCC) = state + 1;
                break;
            case 21:
                timer = *(s32*)((s32)entry + 0xD0);
                *(s32*)((s32)entry + 0xD0) = timer + 1;
                duration = (*(s32*)((s32)gp + 4) * 0x18) / 60;
                *(f32*)((s32)entry + 0xC4) =
                    intplGetValue(float_640_804234a8, float_0_80423338, 0xB, timer, duration);
                duration = (*(s32*)((s32)gp + 4) * 0x18) / 60;
                if (*(s32*)((s32)entry + 0xD0) > duration) {
                    *(s32*)((s32)entry + 0xCC) = 100;
                }
                break;
            case 30:
                *(f32*)((s32)entry + 0xC8) = float_0_80423338;
                *(s32*)((s32)entry + 0xD0) = 0;
                *(s32*)((s32)entry + 0xCC) = state + 1;
                break;
            case 31:
                timer = *(s32*)((s32)entry + 0xD0);
                *(s32*)((s32)entry + 0xD0) = timer + 1;
                duration = (*(s32*)((s32)gp + 4) * 0x18) / 60;
                *(f32*)((s32)entry + 0xC4) =
                    intplGetValue(float_neg640_804234ac, float_0_80423338, 0xB, timer, duration);
                duration = (*(s32*)((s32)gp + 4) * 0x18) / 60;
                if (*(s32*)((s32)entry + 0xD0) > duration) {
                    *(s32*)((s32)entry + 0xCC) = 100;
                }
                break;
            case 40:
                *(f32*)((s32)entry + 0xC8) = float_0_80423338;
                *(s32*)((s32)entry + 0xD0) = 0;
                *(s32*)((s32)entry + 0xCC) = state + 1;
                break;
            case 41:
                timer = *(s32*)((s32)entry + 0xD0);
                *(s32*)((s32)entry + 0xD0) = timer + 1;
                duration = (*(s32*)((s32)gp + 4) * 0x18) / 60;
                *(f32*)((s32)entry + 0xC4) =
                    intplGetValue(float_0_80423338, float_neg640_804234ac, 0xB, timer, duration);
                duration = (*(s32*)((s32)gp + 4) * 0x18) / 60;
                if (*(s32*)((s32)entry + 0xD0) > duration) {
                    *(s32*)((s32)entry + 0xCC) = -1;
                }
                break;
        }
        i++;
        entry = (void*)((s32)entry + 0x14);
    } while (i < 5);
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


void winBgGX(f32 x,f32 y,void* pWin,s32 type){
    extern u32 dat_804231b8,dat_804231bc,dat_804231c0,dat_804231c4,dat_804231c8,dat_804231cc,dat_804231d0,dat_804231d4,dat_804231d8,dat_804231dc,dat_804231e0,dat_804231e4,dat_804231e8,dat_804231ec,dat_804231f0,dat_804231f4;
    u32 colors[3],texObj[8];f32 mtx[3][4];void* cam;void* file;void* tpl;f32 w,h;
    if(type==0){colors[0]=dat_804231b8;colors[1]=dat_804231bc;colors[2]=dat_804231c0;}else if(type==1){colors[0]=dat_804231c4;colors[1]=dat_804231c8;colors[2]=dat_804231cc;}else if(type==2){colors[0]=dat_804231d0;colors[1]=dat_804231d4;colors[2]=dat_804231d8;}else if(type==3){colors[0]=dat_804231dc;colors[1]=dat_804231e0;colors[2]=dat_804231e4;}else{colors[0]=dat_804231e8;colors[1]=dat_804231ec;colors[2]=dat_804231f0;}
    GXSetTevColor(1,&colors[0]);GXSetTevColor(2,&colors[1]);GXSetTevColor(3,&colors[2]);GXSetBlendMode(0,1,0,0);GXSetZCompLoc(0);GXSetAlphaCompare(6,0x80,1,0,0);GXSetZMode(0,7,0);GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&dat_804231f4);GXSetNumChans(0);GXSetChanCtrl(4,0,0,0,0,0,2);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,15,15,15,2);GXSetTevAlphaIn(0,7,7,7,4);GXSetTevSwapMode(0,0,0);
    PSMTXTrans(mtx,x,y+18.0f,0.0f);cam=camGetPtr(1);PSMTXConcat((void*)((s32)cam+0x30),mtx,mtx);GXLoadPosMtxImm(mtx,0);GXSetCurrentMtx(0);GXSetCullMode(0);GXClearVtxDesc();GXSetVtxDesc(9,1);GXSetVtxDesc(13,1);GXSetVtxAttrFmt(0,9,1,4,0);GXSetVtxAttrFmt(0,13,1,4,0);
    file=*(void**)((s32)pWin+0x28);tpl=**(void***)((s32)file+0xA0);TEXGetGXTexObjFromPalette(tpl,texObj,7);GXInitTexObjLOD(texObj,0,0,0.0f,0.0f,0.0f,0,0,0);GXLoadTexObj(texObj,0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);h=(f32)GXGetTexObjHeight(texObj);w=(f32)GXGetTexObjWidth(texObj);PSMTXScale(mtx,570.0f/w,320.0f/h,1.0f);GXLoadTexMtxImm(mtx,0x1E,1);
    GXBegin(0x80,0,4);DAT_cc008000=-285.0f;DAT_cc008000=160.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=-285.0f;DAT_cc008000=-160.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=0.0f;DAT_cc008000=-160.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=1.0f;DAT_cc008000=0.0f;DAT_cc008000=160.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=0.0f;
}

u8 winMsgMain(void* pWin) {
    extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 time, s32 duration);
    extern u32 keyGetDirTrg(s32 controller);
    extern f32 float_neg800_8042347c;
    extern f32 float_neg170_80423480;
    s32 duration;
    s32 timer;
    s32 line;
    s32 max;

    switch (*(s32*)((s32)pWin + 0x130)) {
        case 0:
            *(f32*)((s32)pWin + 0x128) = float_neg800_8042347c;
            *(f32*)((s32)pWin + 0x12C) = float_neg170_80423480;
            *(s32*)((s32)pWin + 0x134) = 0;
            (*(s32*)((s32)pWin + 0x130))++;
            break;
        case 1:
            timer = *(s32*)((s32)pWin + 0x134);
            (*(s32*)((s32)pWin + 0x134))++;
            duration = (*(s32*)((s32)gp + 4) << 4) / 60;
            *(f32*)((s32)pWin + 0x128) = intplGetValue(float_neg800_8042347c, float_0_80423338, 0xB, timer, duration);
            duration = (*(s32*)((s32)gp + 4) << 4) / 60;
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
        case 11:
            timer = *(s32*)((s32)pWin + 0x134);
            (*(s32*)((s32)pWin + 0x134))++;
            duration = (*(s32*)((s32)gp + 4) << 4) / 60;
            *(f32*)((s32)pWin + 0x128) = intplGetValue(float_0_80423338, float_neg800_8042347c, 0xB, timer, duration);
            duration = (*(s32*)((s32)gp + 4) << 4) / 60;
            if (*(s32*)((s32)pWin + 0x134) > duration) {
                (*(s32*)((s32)pWin + 0x130))++;
            }
            break;
    }
    return 0;
}

void winMsgDisp(double x, double y, void* pWin) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
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
        char* msg = msgSearch(*(char**)((s32)pWin + 0x140));
        f32 width = (f32)FontGetMessageWidth(msg);
        f32 offset = item == -2 ? -130.0f : 0.0f;
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
            pos.y = (f32)y - 24.0f;
            winIconSet(0x1BE, &pos, &scale, &color);
        } else if (line / 2 == (maxLines + 1) / 2 - 1) {
            pos.y = (f32)y + 24.0f;
            winIconSet(0x1BD, &pos, &scale, &color);
        } else {
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
    u32 texObj[8];
    f32 texMtx[3][4];
    u32 texObjB0[8];
    f32 texMtxB0[3][4];
    u32 texObjB1[8];
    f32 texMtxB1[3][4];
    u32 texObjB2[8];
    f32 texMtxB2[3][4];
    u32 texObjB3[8];
    f32 texMtxB3[3][4];
    u32 texObjB4[8];
    f32 texMtxB4[3][4];
    u32 texObjB5[8];
    f32 texMtxB5[3][4];
    u32 texObjB6[8];
    f32 texMtxB6[3][4];
    u32 texObjB7[8];
    f32 texMtxB7[3][4];
    u32 texObjB8[8];
    f32 texMtxB8[3][4];
    u32 texObjB9[8];
    f32 texMtxB9[3][4];
    u32 texObjB10[8];
    f32 texMtxB10[3][4];
    u32 texObjB11[8];
    f32 texMtxB11[3][4];
    u32 texObjB12[8];
    f32 texMtxB12[3][4];
    u32 texObjB13[8];
    f32 texMtxB13[3][4];
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
    GXLoadPosMtxImm((u8*)camGetPtr(1) + 0x30, 0);
    GXSetCurrentMtx(0);
    if (style == 0) {
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj, 0xB);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj, 9);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   ((f32)w - float_16_804233b8) / (f32)GXGetTexObjWidth(texObj),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + (f32)w - float_8_80423370;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + (f32)w - float_8_80423370;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj, 0xD);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + (f32)w - float_8_80423370;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + (f32)w - float_8_80423370;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + (f32)w;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + (f32)w;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj, 10);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj),
                   ((f32)h - float_16_804233b8) / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y - (f32)h + float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - (f32)h + float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj, 8);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   ((f32)w - float_8_80423370) / (f32)GXGetTexObjWidth(texObj),
                   ((f32)h - float_16_804233b8) / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - (f32)h + float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + (f32)w;
        DAT_cc008000 = (f32)y - (f32)h + float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + (f32)w;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj, 0xC);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y - (f32)h + float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y - (f32)h;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - (f32)h;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - (f32)h + float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj, 8);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   ((f32)w - float_16_804233b8) / (f32)GXGetTexObjWidth(texObj),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - (f32)h + float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - (f32)h;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + (f32)w - float_8_80423370;
        DAT_cc008000 = (f32)y - (f32)h;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + (f32)w - float_8_80423370;
        DAT_cc008000 = (f32)y - (f32)h + float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0), texObj, 0xE);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + (f32)w - float_8_80423370;
        DAT_cc008000 = (f32)y - (f32)h + float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + (f32)w - float_8_80423370;
        DAT_cc008000 = (f32)y - (f32)h;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + (f32)w;
        DAT_cc008000 = (f32)y - (f32)h;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + (f32)w;
        DAT_cc008000 = (f32)y - (f32)h + float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
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

        DAT_cc008000 = fx;
        DAT_cc008000 = fy;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = fx;
        DAT_cc008000 = innerTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = innerLeft;
        DAT_cc008000 = innerTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = innerLeft;
        DAT_cc008000 = fy;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = innerLeft;
        DAT_cc008000 = fy;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = innerLeft;
        DAT_cc008000 = innerTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right - float_8_80423370;
        DAT_cc008000 = innerTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right - float_8_80423370;
        DAT_cc008000 = fy;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = right - float_8_80423370;
        DAT_cc008000 = fy;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = right - float_8_80423370;
        DAT_cc008000 = innerTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = innerTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = fy;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = fx;
        DAT_cc008000 = innerTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = fx;
        DAT_cc008000 = innerBottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = innerLeft;
        DAT_cc008000 = innerBottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = innerLeft;
        DAT_cc008000 = innerTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = innerLeft;
        DAT_cc008000 = innerTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = innerLeft;
        DAT_cc008000 = innerBottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = innerBottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = innerTop;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = fx;
        DAT_cc008000 = innerBottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = fx;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = innerLeft;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = innerLeft;
        DAT_cc008000 = innerBottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = innerLeft;
        DAT_cc008000 = innerBottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = innerLeft;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right - float_8_80423370;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right - float_8_80423370;
        DAT_cc008000 = innerBottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = notchStart;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = notchStart;
        DAT_cc008000 = bottom - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = bottom - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = notchStart + float_8_80423370;
        DAT_cc008000 = bottom - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = notchStart + float_8_80423370;
        DAT_cc008000 = bottom - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = bottom - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = bottom - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = notchStart + float_8_80423370;
        DAT_cc008000 = bottom - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = notchStart + float_8_80423370;
        DAT_cc008000 = bottom - float_16_804233b8 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom - float_16_804233b8 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = notchStart + float_16_804233b8;
        DAT_cc008000 = bottom - float_16_804233b8 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right - float_8_80423370;
        DAT_cc008000 = bottom - float_16_804233b8 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right - float_8_80423370;
        DAT_cc008000 = bottom - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

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

        DAT_cc008000 = right - float_8_80423370;
        DAT_cc008000 = bottom - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = right - float_8_80423370;
        DAT_cc008000 = bottom - float_16_804233b8 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = bottom - float_16_804233b8 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = bottom - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

    }
}

void winNameGX(double x, double y, double width, double unusedHeight, void* pWin, s32 type) {
    extern u32 dat_8042321c;
    extern f32 float_8_80423370;
    extern f32 float_16_804233b8;
    extern f32 float_24_8042339c;
    extern f32 float_32_80423378;
    u32 color = dat_8042321c;
    u32 tex0[8], tex1[8], tex2[8], tex3[8], tex4[8], tex5[8];
    f32 mtx0[3][4], mtx1[3][4], mtx2[3][4], mtx3[3][4], mtx4[3][4], mtx5[3][4];
    void* fileData = **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0);
    void* camera;
    f32 split;

    (void)unusedHeight;
    GXSetBlendMode(0, 1, 0, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0);
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &color);
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
    camera = camGetPtr(1);
    GXLoadPosMtxImm((u8*)camera + 0x30, 0);
    GXSetCurrentMtx(0);

    if (type == 0) {
        TEXGetGXTexObjFromPalette(fileData, tex0, 0x13);
        GXInitTexObjLOD(tex0, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(tex0, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(mtx0, ((f32)width - 16.0f) / (f32)GXGetTexObjWidth(tex0),
                    32.0f / (f32)GXGetTexObjHeight(tex0), 1.0f);
        GXLoadTexMtxImm(mtx0, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        split = (f32)(x + (double)((f32)width - 16.0f));
        DAT_cc008000=(f32)x; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
        DAT_cc008000=(f32)x; DAT_cc008000=(f32)(y-(double)float_32_80423378); DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)(y-(double)float_32_80423378); DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
        TEXGetGXTexObjFromPalette(fileData, tex1, 0x14);
        GXInitTexObjLOD(tex1, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(tex1, 0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(mtx1, 16.0f/(f32)GXGetTexObjWidth(tex1), 32.0f/(f32)GXGetTexObjHeight(tex1), 1.0f);
        GXLoadTexMtxImm(mtx1,0x1E,1); GXBegin(0x80,0,4);
        split=(f32)(x+width)-16.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)(y-(double)float_32_80423378); DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split+16.0f; DAT_cc008000=(f32)(y-(double)float_32_80423378); DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split+16.0f; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    } else if (type == 1) {
        TEXGetGXTexObjFromPalette(fileData, tex2, 0x15);
        GXInitTexObjLOD(tex2,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(tex2,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(mtx2,((f32)width-16.0f)/(f32)GXGetTexObjWidth(tex2),32.0f/(f32)GXGetTexObjHeight(tex2),1.0f);
        GXLoadTexMtxImm(mtx2,0x1E,1); GXBegin(0x80,0,4); split=(f32)(x+(double)((f32)width-16.0f));
        DAT_cc008000=(f32)x; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
        DAT_cc008000=(f32)x; DAT_cc008000=(f32)(y-(double)float_32_80423378); DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)(y-(double)float_32_80423378); DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
        TEXGetGXTexObjFromPalette(fileData,tex3,0x16); GXInitTexObjLOD(tex3,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(tex3,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(mtx3,16.0f/(f32)GXGetTexObjWidth(tex3),32.0f/(f32)GXGetTexObjHeight(tex3),1.0f); GXLoadTexMtxImm(mtx3,0x1E,1); GXBegin(0x80,0,4); split=(f32)(x+width)-16.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)(y-(double)float_32_80423378); DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split+16.0f; DAT_cc008000=(f32)(y-(double)float_32_80423378); DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split+16.0f; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
    } else if (type == 2) {
        TEXGetGXTexObjFromPalette(fileData,tex4,0x78); GXInitTexObjLOD(tex4,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(tex4,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(mtx4,((f32)width-8.0f)/(f32)GXGetTexObjWidth(tex4),24.0f/(f32)GXGetTexObjHeight(tex4),1.0f); GXLoadTexMtxImm(mtx4,0x1E,1); GXBegin(0x80,0,4); split=(f32)(x+(double)((f32)width-8.0f));
        DAT_cc008000=(f32)x; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
        DAT_cc008000=(f32)x; DAT_cc008000=(f32)(y-(double)float_24_8042339c); DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)(y-(double)float_24_8042339c); DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
        TEXGetGXTexObjFromPalette(fileData,tex5,0x79); GXInitTexObjLOD(tex5,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(tex5,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        PSMTXScale(mtx5,8.0f/(f32)GXGetTexObjWidth(tex5),24.0f/(f32)GXGetTexObjHeight(tex5),1.0f); GXLoadTexMtxImm(mtx5,0x1E,1); GXBegin(0x80,0,4); split=(f32)(x+width)-8.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
        DAT_cc008000=split; DAT_cc008000=(f32)(y-(double)float_24_8042339c); DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split+8.0f; DAT_cc008000=(f32)(y-(double)float_24_8042339c); DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=1.0f;
        DAT_cc008000=split+8.0f; DAT_cc008000=(f32)y; DAT_cc008000=0.0f; DAT_cc008000=1.0f; DAT_cc008000=0.0f;
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
    GXLoadPosMtxImm((u8*)camGetPtr(1) + 0x30, 0); GXSetCurrentMtx(0);
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
        DAT_cc008000 = left;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = left;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = right;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
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
    DAT_cc008000 = (f32)x;
    DAT_cc008000 = (f32)y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = (f32)x;
    DAT_cc008000 = (f32)y - float_8_80423370;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8;
    DAT_cc008000 = (f32)y - float_8_80423370;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8;
    DAT_cc008000 = (f32)y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x2C);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_184_8042344c / (f32)GXGetTexObjWidth(texObj),
               float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    DAT_cc008000 = (f32)x + float_16_804233b8;
    DAT_cc008000 = (f32)y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8;
    DAT_cc008000 = (f32)y - float_8_80423370;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c;
    DAT_cc008000 = (f32)y - float_8_80423370;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c;
    DAT_cc008000 = (f32)y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x26);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_48_804233f0 / (f32)GXGetTexObjWidth(texObj),
               float_16_804233b8 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c;
    DAT_cc008000 = (f32)y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c;
    DAT_cc008000 = (f32)y - float_16_804233b8;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0;
    DAT_cc008000 = (f32)y - float_16_804233b8;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0;
    DAT_cc008000 = (f32)y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x2D);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_184_8042344c / (f32)GXGetTexObjWidth(texObj),
               float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0;
    DAT_cc008000 = (f32)y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0;
    DAT_cc008000 = (f32)y - float_8_80423370;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c;
    DAT_cc008000 = (f32)y - float_8_80423370;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c;
    DAT_cc008000 = (f32)y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0), texObj, 0x2A);
    GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_16_804233b8 / (f32)GXGetTexObjWidth(texObj),
               float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c;
    DAT_cc008000 = (f32)y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c;
    DAT_cc008000 = (f32)y - float_8_80423370;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c + float_16_804233b8;
    DAT_cc008000 = (f32)y - float_8_80423370;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = (f32)x + float_16_804233b8 + float_184_8042344c + float_48_804233f0 + float_184_8042344c + float_16_804233b8;
    DAT_cc008000 = (f32)y;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;
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
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
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
        DAT_cc008000 = left;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = left;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = left + float_16_804233b8;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = left + float_16_804233b8;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
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
        DAT_cc008000 = left;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = left;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = left + float_8_80423370;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = left + float_8_80423370;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
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
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
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
        DAT_cc008000 = left;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = left;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = left + float_184_8042344c;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = left + float_184_8042344c;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
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
        DAT_cc008000 = left;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = left;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = left + float_48_804233f0;
        DAT_cc008000 = bottom;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = left + float_48_804233f0;
        DAT_cc008000 = top;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
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

void winHalfBookGX(double x, double y, void* file, s32 side) {
    extern u32 dat_80423230;
    extern u32 dat_80423234;
    extern void GXSetChanMatColor(s32 channel, void* color);
    extern u32 dat_8042322c;
    extern f32 float_neg8_8042342c;
    f32 mtx[3][4];
    void* camera;
    extern f32 float_8_80423370;
    extern f32 float_248_80423424;
    extern f32 float_256_80423438;
    extern f32 float_320_80423344;
    extern f32 float_16_804233b8;
    extern f32 float_328_80423444;
    extern f32 float_48_804233f0;
    extern f32 float_24_8042339c;
    extern f32 float_40_804233bc;
    extern f32 float_200_8042341c;
    extern f32 float_240_80423380;
    extern f32 float_32_80423378;
    extern f32 float_272_80423428;
    extern f32 float_288_80423440;
    extern f32 float_neg16_8042343c;
    u32 texObj[8];
    f32 texMtx[3][4];
    u32 texObj32[8];
    f32 texMtx32[3][4];
    u32 texObj36[8];
    f32 texMtx36[3][4];
    u32 texObj3A[8];
    f32 texMtx3A[3][4];
    u32 texObj38A[8];
    f32 texMtx38A[3][4];
    u32 texObj38B[8];
    f32 texMtx38B[3][4];
    u32 texObj39[8];
    f32 texMtx39[3][4];
    u32 texObj38C[8];
    f32 texMtx38C[3][4];
    u32 texObj38D[8];
    f32 texMtx38D[3][4];
    u32 texObj38E[8];
    f32 texMtx38E[3][4];
    u32 texObj3C[8];
    f32 texMtx3C[3][4];
    u32 fog = dat_8042322c;
    s32 pass;

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
    for (pass = 0; pass < 2; pass++) {
        if (pass == 0) {
            GXSetChanMatColor(4, &dat_80423230);
            GXSetBlendMode(1, 4, 5, 0);
            GXSetZCompLoc(1);
            GXSetAlphaCompare(7, 0, 0, 7, 0);
            GXSetZMode(0, 7, 0);
            PSMTXTrans(mtx, 8.0f, float_neg8_8042342c, 0.0f);
            camera = camGetPtr(1);
            PSMTXConcat((u8*)camera + 0x30, mtx, mtx);
            GXLoadPosMtxImm(mtx, 0);
            GXSetCurrentMtx(0);
        } else {
            GXSetChanMatColor(4, &dat_80423234);
            GXSetBlendMode(0, 1, 0, 0);
            GXSetZCompLoc(0);
            GXSetAlphaCompare(6, 0x80, 1, 0, 0);
            GXSetZMode(0, 7, 0);
            camera = camGetPtr(1);
            GXLoadPosMtxImm((u8*)camera + 0x30, 0);
            GXSetCurrentMtx(0);
        }
        TEXGetGXTexObjFromPalette(file, texObj, 0x33);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_320_80423344 / (f32)GXGetTexObjWidth(texObj),
                   float_248_80423424 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_328_80423444;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_328_80423444;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(file, texObj, 0x28);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_16_804233b8 / (f32)GXGetTexObjWidth(texObj),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(file, texObj, 0x35);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_248_80423424 / (f32)GXGetTexObjWidth(texObj),
                   float_8_80423370 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 + float_248_80423424;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 + float_248_80423424;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(file, texObj, 0x26);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_48_804233f0 / (f32)GXGetTexObjWidth(texObj),
                   float_16_804233b8 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_16_804233b8 + float_248_80423424;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 + float_248_80423424;
        DAT_cc008000 = (f32)y - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 + float_248_80423424 + float_48_804233f0;
        DAT_cc008000 = (f32)y - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 + float_248_80423424 + float_48_804233f0;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(file, texObj, 0x3B);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_24_8042339c / (f32)GXGetTexObjWidth(texObj),
                   float_16_804233b8 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_16_804233b8 + float_248_80423424 + float_48_804233f0;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 + float_248_80423424 + float_48_804233f0;
        DAT_cc008000 = (f32)y - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 + float_248_80423424 + float_48_804233f0 + float_24_8042339c;
        DAT_cc008000 = (f32)y - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 + float_248_80423424 + float_48_804233f0 + float_24_8042339c;
        DAT_cc008000 = (f32)y;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        TEXGetGXTexObjFromPalette(file, texObj, 0x30);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   float_8_80423370 / (f32)GXGetTexObjWidth(texObj),
                   float_248_80423424 / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_8_80423370;
        DAT_cc008000 = (f32)y - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;
        /*
         * Continue the target's adjacent fixed primitive family after the
         * retained 0x30 left edge: lower-right edge (0x32), center lower
         * strip (0x36), and lower-left cap (0x3A).
         */
        TEXGetGXTexObjFromPalette(file, texObj32, 0x32);
        GXInitTexObjLOD(texObj32, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj32, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx32,
                   float_16_804233b8 / (f32)(u16)GXGetTexObjWidth(texObj32),
                   float_240_80423380 / (f32)(u16)GXGetTexObjHeight(texObj32),
                   1.0f);
        GXLoadTexMtxImm(texMtx32, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_16_804233b8 +
                       float_248_80423424 + float_8_80423370;
        DAT_cc008000 = (f32)y - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 +
                       float_248_80423424 + float_8_80423370;
        DAT_cc008000 = (f32)y - float_16_804233b8 -
                       float_240_80423380;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 +
                       float_248_80423424 + float_8_80423370 +
                       float_16_804233b8;
        DAT_cc008000 = (f32)y - float_16_804233b8 -
                       float_240_80423380;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 +
                       float_248_80423424 + float_8_80423370 +
                       float_16_804233b8;
        DAT_cc008000 = (f32)y - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

        TEXGetGXTexObjFromPalette(file, texObj36, 0x36);
        GXInitTexObjLOD(texObj36, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj36, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx36,
                   float_248_80423424 / (f32)(u16)GXGetTexObjWidth(texObj36),
                   float_40_804233bc / (f32)(u16)GXGetTexObjHeight(texObj36),
                   1.0f);
        GXLoadTexMtxImm(texMtx36, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_200_8042341c;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_200_8042341c -
                       float_40_804233bc;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 +
                       float_248_80423424;
        DAT_cc008000 = (f32)y - float_200_8042341c -
                       float_40_804233bc;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8 +
                       float_248_80423424;
        DAT_cc008000 = (f32)y - float_200_8042341c;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

        TEXGetGXTexObjFromPalette(file, texObj3A, 0x3A);
        GXInitTexObjLOD(texObj3A, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj3A, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx3A,
                   float_16_804233b8 / (f32)(u16)GXGetTexObjWidth(texObj3A),
                   float_8_80423370 / (f32)(u16)GXGetTexObjHeight(texObj3A),
                   1.0f);
        GXLoadTexMtxImm(texMtx3A, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x;
        DAT_cc008000 = (f32)y - float_256_80423438 -
                       float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_256_80423438 -
                       float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

        /* Complete the target's remaining fixed bottom/right primitive family. */
        TEXGetGXTexObjFromPalette(file, texObj38A, 0x38);
        GXInitTexObjLOD(texObj38A, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj38A, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx38A,
                   float_16_804233b8 / (f32)(u16)GXGetTexObjWidth(texObj38A),
                   float_8_80423370 / (f32)(u16)GXGetTexObjHeight(texObj38A),
                   1.0f);
        GXLoadTexMtxImm(texMtx38A, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_32_80423378;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_32_80423378;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

        TEXGetGXTexObjFromPalette(file, texObj38B, 0x38);
        GXInitTexObjLOD(texObj38B, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj38B, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx38B,
                   float_240_80423380 / (f32)(u16)GXGetTexObjWidth(texObj38B),
                   float_8_80423370 / (f32)(u16)GXGetTexObjHeight(texObj38B),
                   1.0f);
        GXLoadTexMtxImm(texMtx38B, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_32_80423378;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_32_80423378;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_272_80423428;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_272_80423428;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

        TEXGetGXTexObjFromPalette(file, texObj39, 0x39);
        GXInitTexObjLOD(texObj39, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj39, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx39,
                   float_16_804233b8 / (f32)(u16)GXGetTexObjWidth(texObj39),
                   float_8_80423370 / (f32)(u16)GXGetTexObjHeight(texObj39),
                   1.0f);
        GXLoadTexMtxImm(texMtx39, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_272_80423428;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_272_80423428;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_272_80423428 + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_272_80423428 + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

        TEXGetGXTexObjFromPalette(file, texObj38C, 0x38);
        GXInitTexObjLOD(texObj38C, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj38C, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx38C,
                   float_16_804233b8 / (f32)(u16)GXGetTexObjWidth(texObj38C),
                   float_8_80423370 / (f32)(u16)GXGetTexObjHeight(texObj38C),
                   1.0f);
        GXLoadTexMtxImm(texMtx38C, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_288_80423440;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_288_80423440;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_288_80423440 + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_288_80423440 + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

        TEXGetGXTexObjFromPalette(file, texObj38D, 0x38);
        GXInitTexObjLOD(texObj38D, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj38D, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx38D,
                   float_16_804233b8 / (f32)(u16)GXGetTexObjWidth(texObj38D),
                   float_8_80423370 / (f32)(u16)GXGetTexObjHeight(texObj38D),
                   1.0f);
        GXLoadTexMtxImm(texMtx38D, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_320_80423344;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_320_80423344;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_320_80423344 + float_neg16_8042343c;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_320_80423344 + float_neg16_8042343c;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

        TEXGetGXTexObjFromPalette(file, texObj38E, 0x38);
        GXInitTexObjLOD(texObj38E, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj38E, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx38E,
                   float_16_804233b8 / (f32)(u16)GXGetTexObjWidth(texObj38E),
                   float_8_80423370 / (f32)(u16)GXGetTexObjHeight(texObj38E),
                   1.0f);
        GXLoadTexMtxImm(texMtx38E, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_320_80423344;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_320_80423344;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_320_80423344 + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_256_80423438 - float_8_80423370;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_320_80423344 + float_16_804233b8;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

        TEXGetGXTexObjFromPalette(file, texObj3C, 0x3C);
        GXInitTexObjLOD(texObj3C, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj3C, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx3C,
                   float_8_80423370 / (f32)(u16)GXGetTexObjWidth(texObj3C),
                   float_240_80423380 / (f32)(u16)GXGetTexObjHeight(texObj3C),
                   1.0f);
        GXLoadTexMtxImm(texMtx3C, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        DAT_cc008000 = (f32)x + float_328_80423444;
        DAT_cc008000 = (f32)y - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = (f32)x + float_328_80423444;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_328_80423444 + float_8_80423370;
        DAT_cc008000 = (f32)y - float_256_80423438;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = (f32)x + float_328_80423444 + float_8_80423370;
        DAT_cc008000 = (f32)y - float_16_804233b8;
        DAT_cc008000 = 0.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 0.0f;

    }
}

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

    GXSetBlendMode(0, 1, 0, 0); GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&fog); GXSetZCompLoc(0); GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0); GXSetNumChans(0); GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1); GXSetTevOp(0, 3); GXSetCullMode(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2); GXClearVtxDesc(); GXSetVtxDesc(9, 1); GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0); GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(4)+0x48,0); GXSetCurrentMtx(0);
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
    f32 pos[3];
    f32 scale[3];
    f32 fontPos[3];
    f32 fontScale[3];
    f32 iconPos[3];
    f32 iconScale[3];
    u32 white = 0xFFFFFFFF;
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

            DAT_cc008000 = (f32)x;
            DAT_cc008000 = pageTop;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;

            DAT_cc008000 = (f32)x;
            DAT_cc008000 = pageTop - float_56_80423354;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_56_80423354;
            DAT_cc008000 = pageTop - float_56_80423354;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_56_80423354;
            DAT_cc008000 = pageTop;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 0.0f;

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

            DAT_cc008000 = (f32)x;
            DAT_cc008000 = pageTop - float_56_80423354;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;

            DAT_cc008000 = (f32)x;
            DAT_cc008000 = pageTop - float_56_80423354 - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_8_80423370;
            DAT_cc008000 = pageTop - float_56_80423354 - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_8_80423370;
            DAT_cc008000 = pageTop - float_56_80423354;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 0.0f;

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

            DAT_cc008000 = (f32)x + float_8_80423370;
            DAT_cc008000 = pageTop - float_56_80423354;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;

            DAT_cc008000 = (f32)x + float_8_80423370;
            DAT_cc008000 = pageTop - float_56_80423354 - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_56_80423354;
            DAT_cc008000 = pageTop - float_56_80423354 - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_56_80423354;
            DAT_cc008000 = pageTop - float_56_80423354;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 0.0f;

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

            DAT_cc008000 = (f32)x + float_56_80423354;
            DAT_cc008000 = pageTop;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;

            DAT_cc008000 = (f32)x + float_56_80423354;
            DAT_cc008000 = pageTop - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_56_80423354 + float_8_80423370;
            DAT_cc008000 = pageTop - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_56_80423354 + float_8_80423370;
            DAT_cc008000 = pageTop;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 0.0f;

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

            DAT_cc008000 = (f32)x + float_56_80423354;
            DAT_cc008000 = pageTop - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;

            DAT_cc008000 = (f32)x + float_56_80423354;
            DAT_cc008000 = pageTop - float_56_80423354;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_56_80423354 + float_8_80423370;
            DAT_cc008000 = pageTop - float_56_80423354;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_56_80423354 + float_8_80423370;
            DAT_cc008000 = pageTop - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 0.0f;

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

            DAT_cc008000 = (f32)x + float_56_80423354;
            DAT_cc008000 = pageTop - float_56_80423354;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;

            DAT_cc008000 = (f32)x + float_56_80423354;
            DAT_cc008000 = pageTop - float_56_80423354 - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_56_80423354 + float_8_80423370;
            DAT_cc008000 = pageTop - float_56_80423354 - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_56_80423354 + float_8_80423370;
            DAT_cc008000 = pageTop - float_56_80423354;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 0.0f;

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

                        DAT_cc008000 = left;
                        DAT_cc008000 = top;
                        DAT_cc008000 = 0.0f;
                        DAT_cc008000 = 0.0f;
                        DAT_cc008000 = 0.0f;

                        DAT_cc008000 = left;
                        DAT_cc008000 = bottom;
                        DAT_cc008000 = 0.0f;
                        DAT_cc008000 = 0.0f;
                        DAT_cc008000 = 1.0f;

                        DAT_cc008000 = right;
                        DAT_cc008000 = bottom;
                        DAT_cc008000 = 0.0f;
                        DAT_cc008000 = 1.0f;
                        DAT_cc008000 = 1.0f;

                        DAT_cc008000 = right;
                        DAT_cc008000 = top;
                        DAT_cc008000 = 0.0f;
                        DAT_cc008000 = 1.0f;
                        DAT_cc008000 = 0.0f;

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

                    DAT_cc008000 = left;
                    DAT_cc008000 = top;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;

                    DAT_cc008000 = left;
                    DAT_cc008000 = bottom;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;

                    DAT_cc008000 = left + float_8_80423370;
                    DAT_cc008000 = bottom;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;
                    DAT_cc008000 = 1.0f;

                    DAT_cc008000 = left + float_8_80423370;
                    DAT_cc008000 = top;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;
                    DAT_cc008000 = 0.0f;

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

                    DAT_cc008000 = left + float_8_80423370;
                    DAT_cc008000 = top;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;

                    DAT_cc008000 = left + float_8_80423370;
                    DAT_cc008000 = bottom;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;

                    DAT_cc008000 = left + float_56_80423354;
                    DAT_cc008000 = bottom;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;
                    DAT_cc008000 = 1.0f;

                    DAT_cc008000 = left + float_56_80423354;
                    DAT_cc008000 = top;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;
                    DAT_cc008000 = 0.0f;

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

                    DAT_cc008000 = left;
                    DAT_cc008000 = top;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;

                    DAT_cc008000 = left;
                    DAT_cc008000 = top - float_8_80423370;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;

                    DAT_cc008000 = right;
                    DAT_cc008000 = top - float_8_80423370;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;
                    DAT_cc008000 = 1.0f;

                    DAT_cc008000 = right;
                    DAT_cc008000 = top;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;
                    DAT_cc008000 = 0.0f;

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

                    DAT_cc008000 = left;
                    DAT_cc008000 = top - float_8_80423370;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;

                    DAT_cc008000 = left;
                    DAT_cc008000 = top - float_56_80423354;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;

                    DAT_cc008000 = right;
                    DAT_cc008000 = top - float_56_80423354;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;
                    DAT_cc008000 = 1.0f;

                    DAT_cc008000 = right;
                    DAT_cc008000 = top - float_8_80423370;
                    DAT_cc008000 = 0.0f;
                    DAT_cc008000 = 1.0f;
                    DAT_cc008000 = 0.0f;

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

            DAT_cc008000 = (f32)x + float_392_804233e4;
            DAT_cc008000 = pageBottom;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;

            DAT_cc008000 = (f32)x + float_392_804233e4;
            DAT_cc008000 = pageBottom - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_400_804233e8;
            DAT_cc008000 = pageBottom - float_8_80423370;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 1.0f;

            DAT_cc008000 = (f32)x + float_400_804233e8;
            DAT_cc008000 = pageBottom;
            DAT_cc008000 = 0.0f;
            DAT_cc008000 = 1.0f;
            DAT_cc008000 = 0.0f;

            }
        }
    }

    /*
     * Preserve the retained helper-based badge/recipe contents after the
     * target panel family.  Their positions and filtering already follow the
     * target's 28-entry, 7x4 page layout.
     */
    pos[2] = 0.0f;
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    scale[2] = 1.0f;

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
                        pos[0] = float_4_8042334c +
                                 float_24_8042339c +
                                 (f32)x +
                                 (f32)(column * 56);
                        pos[1] = baseY -
                                 (f32)(row * 56) -
                                 float_28_804233f4;
                        winTexSet(0x97, pos, scale, &white);
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
                            pos[0] = float_4_8042334c +
                                     float_24_8042339c +
                                     (f32)x +
                                     (f32)(column * 56);
                            pos[1] = baseY -
                                     (f32)(row * 56) -
                                     float_28_804233f4;
                            winIconSet(iconId, pos, scale, &white);
                        }
                    }
                }
            }
        }
    }

    GXSetScissor(0, 0, 0x260, 0x1E0);

    if (pageNum > 0) {
        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
        pos[0] = (f32)x + float_395_804233f8;
        pos[1] = (f32)y + float_15_804233fc;
        winTexSet(0x17, pos, scale, &white);

        winIconInit();
        pos[1] = (f32)y;
        winIconSet(0x86, pos, scale, &white);
    }

    if (pageNum * 28 + 28 < count) {
        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
        pos[0] = (f32)x + float_395_804233f8;
        pos[1] = (f32)y - float_235_80423400;
        winTexSet(0x17, pos, scale, &white);

        winIconInit();
        pos[1] = (f32)y - float_220_80423404;
        winIconSet(0x88, pos, scale, &white);
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
    fontPos[0] = sortX;
    fontPos[1] = (f32)y - float_228_80423410;
    fontPos[2] = 0.0f;
    fontScale[0] = float_0p7_80423408;
    fontScale[1] = float_0p7_80423408;
    fontScale[2] = 1.0f;
    winFontSet(fontPos, fontScale, &white, sortText);

    winIconInit();
    iconPos[0] = sortX - float_30_804233d4;
    iconPos[1] = (f32)y - float_237_80423414;
    iconPos[2] = 0.0f;
    iconScale[0] = 1.0f;
    iconScale[1] = 1.0f;
    iconScale[2] = 1.0f;
    winIconSet(0x219, iconPos, iconScale, &white);
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
    leftWidth = float_0p75_804233dc * fw;
    topMidWidth = (fw - leftWidth) - float_16_804233b8;
    centerWidth = (fw - leftWidth) - float_8_80423370;
    innerHeight = (fh - float_32_80423378) - float_16_804233b8;
    topY = fy - float_32_80423378;
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

    DAT_cc008000 = fx;
    DAT_cc008000 = fy;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = fy;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

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

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = fy;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth + topMidWidth;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth + topMidWidth;
    DAT_cc008000 = fy;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

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

    DAT_cc008000 = fx + fw - float_16_804233b8;
    DAT_cc008000 = fy;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx + fw - float_16_804233b8;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + fw;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + fw;
    DAT_cc008000 = fy;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

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

    DAT_cc008000 = fx;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

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

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth + centerWidth;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth + centerWidth;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

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

    DAT_cc008000 = fx + fw - float_8_80423370;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx + fw - float_8_80423370;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + fw;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + fw;
    DAT_cc008000 = topY;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

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

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = lowerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth + centerWidth;
    DAT_cc008000 = lowerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth + centerWidth;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

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

    DAT_cc008000 = fx + fw - float_8_80423370;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx + fw - float_8_80423370;
    DAT_cc008000 = lowerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + fw;
    DAT_cc008000 = lowerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + fw;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

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

    DAT_cc008000 = fx;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx;
    DAT_cc008000 = finalBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = finalBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

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

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = lowerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx + leftWidth;
    DAT_cc008000 = finalBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth + topMidWidth;
    DAT_cc008000 = finalBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + leftWidth + topMidWidth;
    DAT_cc008000 = lowerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

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

    DAT_cc008000 = fx + fw - float_16_804233b8;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;

    DAT_cc008000 = fx + fw - float_16_804233b8;
    DAT_cc008000 = finalBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + fw;
    DAT_cc008000 = finalBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 1.0f;

    DAT_cc008000 = fx + fw;
    DAT_cc008000 = innerBottom;
    DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f;
    DAT_cc008000 = 0.0f;

}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


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


s32 N_compare_func4_1(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = *(s32*)((s32)param_1 + 8);
    s32 idxB = *(s32*)((s32)param_2 + 8);
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[(s16)idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[(s16)idxB * 0xA]), bufB);
    return strcmp(bufA, bufB);
}

s32 N_compare_func4_1_r(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = *(s32*)((s32)param_1 + 8);
    s32 idxB = *(s32*)((s32)param_2 + 8);
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[(s16)idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[(s16)idxB * 0xA]), bufB);
    return strcmp(bufB, bufA);
}

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

u8 sort_4_3_func(void* pWin) {
    s32 i;
    s32 j;
    s32* a;
    s32* b;
    s32 t0;
    s32 t1;
    s32 t2;
    s32 swap;
    s32 idA;
    s32 idB;

    if (*(s32*)((s32)pWin + 0x180) == 0) {
        for (i = 0; i < *(s32*)((s32)pWin + 0xD64) - 1; i++) {
            a = (s32*)((s32)pWin + 0x404 + (i * 0xC));
            b = a + 3;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xD64); j++) {
                swap = 0;
                idA = (s16)a[2];
                idB = (s16)b[2];
                if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] > *(s8*)&itemDataTable[idB * 0x28 + 0x1C]) {
                    swap = 1;
                } else if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] == *(s8*)&itemDataTable[idB * 0x28 + 0x1C] &&
                           *(s16*)&itemDataTable[idA * 0x28 + 0x12] > *(s16*)&itemDataTable[idB * 0x28 + 0x12]) {
                    swap = 1;
                }
                if (swap != 0) {
                    t0 = a[0];
                    t1 = a[1];
                    t2 = a[2];
                    a[0] = b[0];
                    a[1] = b[1];
                    a[2] = b[2];
                    b[0] = t0;
                    b[1] = t1;
                    b[2] = t2;
                }
                b += 3;
            }
        }
    } else {
        for (i = 0; i < *(s32*)((s32)pWin + 0xD64) - 1; i++) {
            a = (s32*)((s32)pWin + 0x404 + (i * 0xC));
            b = a + 3;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xD64); j++) {
                swap = 0;
                idA = (s16)a[2];
                idB = (s16)b[2];
                if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] < *(s8*)&itemDataTable[idB * 0x28 + 0x1C]) {
                    swap = 1;
                } else if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] == *(s8*)&itemDataTable[idB * 0x28 + 0x1C] &&
                           *(s16*)&itemDataTable[idA * 0x28 + 0x12] < *(s16*)&itemDataTable[idB * 0x28 + 0x12]) {
                    swap = 1;
                }
                if (swap != 0) {
                    t0 = a[0];
                    t1 = a[1];
                    t2 = a[2];
                    a[0] = b[0];
                    a[1] = b[1];
                    a[2] = b[2];
                    b[0] = t0;
                    b[1] = t1;
                    b[2] = t2;
                }
                b += 3;
            }
        }
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xD68) = 0;
    *(s32*)((s32)pWin + 0xD6C) = *(s32*)((s32)pWin + 0xD68) / 8;
    return 0;
}
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

u8 sort_5_3_func(void* pWin) {
    s32 i;
    s32 j;
    s16* a;
    s16* b;
    s16 tmp;
    s32 swap;
    s32 idA;
    s32 idB;

    if (*(s32*)((s32)pWin + 0x180) == 0) {
        for (i = 0; i < *(s32*)((s32)pWin + 0xE94) - 1; i++) {
            a = (s16*)((s32)pWin + 0xEBC + (i * 2));
            b = a + 1;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xE94); j++) {
                tmp = *a;
                swap = 0;
                idA = (s16)(tmp + 0xF0);
                idB = (s16)(*b + 0xF0);
                if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] > *(s8*)&itemDataTable[idB * 0x28 + 0x1C]) {
                    swap = 1;
                } else if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] == *(s8*)&itemDataTable[idB * 0x28 + 0x1C] &&
                           *(s16*)&itemDataTable[idA * 0x28 + 0x12] > *(s16*)&itemDataTable[idB * 0x28 + 0x12]) {
                    swap = 1;
                }
                if (swap != 0) {
                    *a = *b;
                    *b = tmp;
                }
                b++;
            }
        }
    } else {
        for (i = 0; i < *(s32*)((s32)pWin + 0xE94) - 1; i++) {
            a = (s16*)((s32)pWin + 0xEBC + (i * 2));
            b = a + 1;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xE94); j++) {
                tmp = *a;
                swap = 0;
                idA = (s16)(tmp + 0xF0);
                idB = (s16)(*b + 0xF0);
                if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] < *(s8*)&itemDataTable[idB * 0x28 + 0x1C]) {
                    swap = 1;
                } else if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] == *(s8*)&itemDataTable[idB * 0x28 + 0x1C] &&
                           *(s16*)&itemDataTable[idA * 0x28 + 0x12] < *(s16*)&itemDataTable[idB * 0x28 + 0x12]) {
                    swap = 1;
                }
                if (swap != 0) {
                    *a = *b;
                    *b = tmp;
                }
                b++;
            }
        }
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xE9C) = 0;
    *(s32*)((s32)pWin + 0xEA0) = *(s32*)((s32)pWin + 0xE9C) / 28;
    return 0;
}
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


s32 compare_func2(void* param_1, void* param_2) {
    char bufA[0x40];
    char bufB[0x40];
    void* unitA;
    void* unitB;
    char** tblA;
    char** tblB;
    char* nameA;
    char* nameB;
    u32 idxA;
    u32 idxB;

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
        if (idxA >= 0x1A) {
            break;
        }
    }
    if (idxA >= 0x1A) {
        idxA = 0;
    }

    nameB = *(char**)((s32)unitA + 0x14);
    idxB = 0;
    while (1) {
        sprintf(bufB, str_menu_monosiri_PCTs_802f4d00, *tblA);
        if (strcmp(bufB, nameB) == 0) {
            break;
        }
        idxB++;
        tblA++;
        if (idxB >= 0x1A) {
            break;
        }
    }
    if (idxB >= 0x1A) {
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
s32 compare_func2_r(void* param_1, void* param_2) {
    char bufA[0x40];
    char bufB[0x40];
    void* unitA;
    void* unitB;
    char** tblA;
    char** tblB;
    char* nameA;
    char* nameB;
    u32 idxA;
    u32 idxB;

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
        if (idxA >= 0x1A) {
            break;
        }
    }
    if (idxA >= 0x1A) {
        idxA = 0;
    }

    nameB = *(char**)((s32)unitA + 0x14);
    idxB = 0;
    while (1) {
        sprintf(bufB, str_menu_monosiri_PCTs_802f4d00, *tblA);
        if (strcmp(bufB, nameB) == 0) {
            break;
        }
        idxB++;
        tblA++;
        if (idxB >= 0x1A) {
            break;
        }
    }
    if (idxB >= 0x1A) {
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
    char* msg;
    s32 changed;
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
        *(f32*)((s32)pWin + 0x158) = *(f32*)((s32)pWin + 0x16C) + float_30_804233d4;
        *(f32*)((s32)pWin + 0x15C) = (*(f32*)((s32)pWin + 0x170) - float_40_804233bc) - (f32)(*(s32*)((s32)pWin + 0x174) * 0x28);
        msg = *(char**)((s32)sort_data[*(s32*)((s32)pWin + 0x184)] + (*(s32*)((s32)pWin + 0x174) << 3));
        if (msg != 0) {
            changed = 0;
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

    *(f32*)((s32)pWin + 0x164) = ((*(f32*)((s32)pWin + 0x16C) - *(f32*)((s32)pWin + 0x164)) * float_0p125_804233d8) + *(f32*)((s32)pWin + 0x164);
    *(f32*)((s32)pWin + 0x168) = ((*(f32*)((s32)pWin + 0x170) - *(f32*)((s32)pWin + 0x168)) * float_0p125_804233d8) + *(f32*)((s32)pWin + 0x168);
    return 0;
}

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
    u32 color = *colorPtr;
    u32 texObj[8];
    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    f32 rotMtx[3][4];
    f32 modelMtx[3][4];
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
    GXSetTevColor(1, &color);

    PSMTXTrans(transMtx, x, y, z);
    texH = float_2_80423360 * (f32)(u16)GXGetTexObjHeight(texObj);
    texW = float_2_80423360 * (f32)(u16)GXGetTexObjWidth(texObj);
    PSMTXScale(scaleMtx, scale * texW, scale * texH, float_1_80423340);
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

    DAT_cc008000 = float_neg0p5_804233b4;
    DAT_cc008000 = float_0p5_8042333c;
    DAT_cc008000 = float_0_80423338;
    DAT_cc008000 = float_0_80423338;
    DAT_cc008000 = float_0_80423338;
    DAT_cc008000 = float_neg0p5_804233b4;
    DAT_cc008000 = float_neg0p5_804233b4;
    DAT_cc008000 = float_0_80423338;
    DAT_cc008000 = float_0_80423338;
    DAT_cc008000 = float_1_80423340;
    DAT_cc008000 = float_0p5_8042333c;
    DAT_cc008000 = float_neg0p5_804233b4;
    DAT_cc008000 = float_0_80423338;
    DAT_cc008000 = float_1_80423340;
    DAT_cc008000 = float_1_80423340;
    DAT_cc008000 = float_0p5_8042333c;
    DAT_cc008000 = float_0p5_8042333c;
    DAT_cc008000 = float_0_80423338;
    DAT_cc008000 = float_1_80423340;
    DAT_cc008000 = float_0_80423338;
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

void winMailDisp(void* pWin) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;

    extern s32 pouchCheckMail(s32 id);
    extern void winMailGX(double x, double y, void* pWin);
    extern void GXSetScissor(s32 x, s32 y, s32 width, s32 height);
    extern void winFontSetWidth(Vec3*, Vec3*, void*, f32, const char*);
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
            listBgBase.x = *(f32*)(win + 0x244) + float_208_80423350 + float_4_8042334c;
            listBgBase.y = *(f32*)(win + 0x260) + (*(f32*)(win + 0x24C) - float_56_80423354 - (f32)rowOffset);
            listBgPos = listBgBase;

                winTexSet(0x77, &listBgPos, &listBgScale, &listBgColor);

                listIconColor = dat_804232fc;
            listStatusScale = *(Vec3*)(mailCatalog + 0x350);
            listStatusBase = *(Vec3*)(mailCatalog + 0x344);
            listStatusBase.x = *(f32*)(win + 0x244) + float_72_80423358;
            listStatusBase.y = *(f32*)(win + 0x260) + (*(f32*)(win + 0x24C) - float_56_80423354 - (f32)rowOffset - float_3_8042335c);
            listStatusPos = listStatusBase;

                winTexSet(pouchCheckMail(mailId) == 2 ? 0x5E : 0x5D,
                          &listStatusPos, &listStatusScale, &listIconColor);

            listTensScale = *(Vec3*)(mailCatalog + 0x368);
            listTensBase = *(Vec3*)(mailCatalog + 0x35C);
            listTensBase.x = *(f32*)(win + 0x244) + float_88_80423364 + float_2_80423360;
            listTensBase.y = *(f32*)(win + 0x260) + (*(f32*)(win + 0x24C) - float_56_80423354 - (f32)rowOffset - float_3_8042335c);
            listTensPos = listTensBase;

                winTexSet((displayNumber + 1) / 10 + 0x65,
                          &listTensPos, &listTensScale, &listIconColor);

            listOnesScale = *(Vec3*)(mailCatalog + 0x380);
            listOnesBase = *(Vec3*)(mailCatalog + 0x374);
            listOnesBase.x = *(f32*)(win + 0x244) + float_104_80423368 + float_2_80423360;
            listOnesBase.y = *(f32*)(win + 0x260) + (*(f32*)(win + 0x24C) - float_56_80423354 - (f32)rowOffset - float_3_8042335c);
            listOnesPos = listOnesBase;

                winTexSet((displayNumber + 1) % 10 + 0x65,
                          &listOnesPos, &listOnesScale, &listIconColor);

                winFontInit();
            listSubjectScale = *(Vec3*)(mailCatalog + 0x398);
            listSubjectBase = *(Vec3*)(mailCatalog + 0x38C);
            listSubjectBase.x = *(f32*)(win + 0x244) + float_120_8042336c;
            listSubjectBase.y = *(f32*)(win + 0x260) + float_8_80423370 + (*(f32*)(win + 0x24C) - float_56_80423354 - (f32)rowOffset);
            listSubjectPos = listSubjectBase;

                text = msgSearch(*(char**)(mail + 8));
                winFontSetWidth(&listSubjectPos, &listSubjectScale,
                                &listFontColor, float_230_80423374, text);
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
            selBgBase.x = *(f32*)(win + 0x244) + float_208_80423350 + float_4_8042334c;
            selBgBase.y = *(f32*)(win + 0x24C) - float_56_80423354;
            selBgPos = selBgBase;

            winTexSet(0x77, &selBgPos, &selBgScale, &selectedBgColor);

            selStatusScale = *(Vec3*)(mailCatalog + 0x3C8);
            selStatusBase = *(Vec3*)(mailCatalog + 0x3BC);
            selStatusBase.x = *(f32*)(win + 0x244) + float_72_80423358;
            selStatusBase.y = *(f32*)(win + 0x24C) - float_56_80423354;
            selStatusPos = selStatusBase;

            winTexSet(0x5E, &selStatusPos, &selStatusScale, &selectedNumberColor);

            selTensScale = *(Vec3*)(mailCatalog + 0x3E0);
            selTensBase = *(Vec3*)(mailCatalog + 0x3D4);
            selTensBase.x = *(f32*)(win + 0x244) + float_88_80423364 + float_2_80423360;
            selTensBase.y = *(f32*)(win + 0x24C) - float_56_80423354 - float_3_8042335c;
            selTensPos = selTensBase;

            winTexSet((displayNumber + 1) / 10 + 0x65,
                      &selTensPos, &selTensScale, &selectedNumberColor);

            selOnesScale = *(Vec3*)(mailCatalog + 0x3F8);
            selOnesBase = *(Vec3*)(mailCatalog + 0x3EC);
            selOnesBase.x = *(f32*)(win + 0x244) + float_104_80423368 + float_2_80423360;
            selOnesBase.y = *(f32*)(win + 0x24C) - float_56_80423354 - float_3_8042335c;
            selOnesPos = selOnesBase;

            winTexSet((displayNumber + 1) % 10 + 0x65,
                      &selOnesPos, &selOnesScale, &selectedNumberColor);

            winFontInit();
            selSubjectScale = *(Vec3*)(mailCatalog + 0x410);
            selSubjectBase = *(Vec3*)(mailCatalog + 0x404);
            selSubjectBase.x = *(f32*)(win + 0x244) + float_120_8042336c;
            selSubjectBase.y = *(f32*)(win + 0x24C) - float_56_80423354 + float_8_80423370;
            selSubjectPos = selSubjectBase;

            text = msgSearch(*(char**)(mail + 8));
            winFontSetWidth(&selSubjectPos, &selSubjectScale,
                            &selectedSubjectColor, float_230_80423374, text);

            GXSetScissor(
                (u32)(*(f32*)(win + 0x244) +
                      float_304_8042337c + float_32_80423378),
                (u32)(float_72_80423358 +
                      (float_240_80423380 - *(f32*)(win + 0x24C))),
                0x158, 0xB1);

            winTexInit(**(void***)((s32)*(void**)(win + 0x28) + 0xA0));
            bodyBgScale = *(Vec3*)(mailCatalog + 0x428);
            bodyBgBase = *(Vec3*)(mailCatalog + 0x41C);
            bodyBgBase.x = *(f32*)(win + 0x244) + float_208_80423350 + float_4_8042334c;
            bodyBgBase.y = *(f32*)(win + 0x24C) - float_56_80423354 - float_80_80423384;
            bodyBgPos = bodyBgBase;

            attachmentColor = selectedBgColor;
            winTexSet(0x77, &bodyBgPos, &bodyBgScale, &attachmentColor);

            winFontInit();
            selectedBodyColor = dat_80423318;
            currentLine = *(s32*)(win + 0x268);
            bodyTextScale = *(Vec3*)(mailCatalog + 0x440);
            bodyTextBase = *(Vec3*)(mailCatalog + 0x434);
            bodyTextBase.x = *(f32*)(win + 0x244) + float_80_80423384;
            bodyTextBase.y = *(f32*)(win + 0x24C) - float_80_80423384 + (f32)(currentLine * 0x15);
            bodyTextPos = bodyTextBase;

            text = msgSearch(*(char**)(mail + 0x0C));
            winFontSetWidth(&bodyTextPos, &bodyTextScale,
                            &selectedBodyColor, float_280_80423388, text);

            if ((flags & 0x1000) != 0) {
                maxLines = *(u16*)(win + 0x26C);
                winTexInit(**(void***)((s32)*(void**)(win + 0x2C) + 0xA0));
                attachmentColor = dat_8042331c;
            attachmentScale = *(Vec3*)(mailCatalog + 0x458);
            attachmentBase = *(Vec3*)(mailCatalog + 0x44C);
            attachmentBase.x = *(f32*)(win + 0x244) + float_210_8042338c;
            attachmentBase.y = float_7_80423390 + (*(f32*)(win + 0x24C) - (f32)((maxLines - 1) * 0x15)) + (f32)(currentLine * 0x15);
            attachmentPos = attachmentBase;

                winTexSet(*(s32*)(mail + 0x10), &attachmentPos,
                          &attachmentScale, &attachmentColor);
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
            arrowUpOnlyBase.x = *(f32*)(win + 0x244) + float_360_80423394;
            arrowUpOnlyBase.y = float_20_80423398 + (*(f32*)(win + 0x24C) - float_230_80423374) - float_24_8042339c;
            arrowUpOnlyPos = arrowUpOnlyBase;

                        winIconSet(0x1BE, &arrowUpOnlyPos,
                                   &arrowUpOnlyScale, &arrowUpOnlyColor);
                    }
                } else if ((u32)(currentLine + 6) == (u32)maxLines) {
                    if (blink != 0) {
                        arrowDownOnlyColor = dat_80423324;
            arrowDownOnlyScale = *(Vec3*)(mailCatalog + 0x488);
            arrowDownOnlyBase = *(Vec3*)(mailCatalog + 0x47C);
            arrowDownOnlyBase.x = *(f32*)(win + 0x244) + float_360_80423394;
            arrowDownOnlyBase.y = float_24_8042339c + float_20_80423398 + (*(f32*)(win + 0x24C) - float_230_80423374);
            arrowDownOnlyPos = arrowDownOnlyBase;

                        winIconSet(0x1BD, &arrowDownOnlyPos,
                                   &arrowDownOnlyScale, &arrowDownOnlyColor);
                    }
                } else if (blink != 0) {
                    arrowDownBothColor = dat_80423328;
            arrowDownBothScale = *(Vec3*)(mailCatalog + 0x4A0);
            arrowDownBothBase = *(Vec3*)(mailCatalog + 0x494);
            arrowDownBothBase.x = *(f32*)(win + 0x244) + float_360_80423394;
            arrowDownBothBase.y = float_24_8042339c + float_20_80423398 + (*(f32*)(win + 0x24C) - float_230_80423374);
            arrowDownBothPos = arrowDownBothBase;

                    winIconSet(0x1BD, &arrowDownBothPos,
                               &arrowDownBothScale, &arrowDownBothColor);

                    arrowUpBothColor = dat_8042332c;
            arrowUpBothScale = *(Vec3*)(mailCatalog + 0x4B8);
            arrowUpBothBase = *(Vec3*)(mailCatalog + 0x4AC);
            arrowUpBothBase.x = *(f32*)(win + 0x244) + float_360_80423394;
            arrowUpBothBase.y = float_20_80423398 + (*(f32*)(win + 0x24C) - float_230_80423374) - float_24_8042339c;
            arrowUpBothPos = arrowUpBothBase;

                    winIconSet(0x1BE, &arrowUpBothPos,
                               &arrowUpBothScale, &arrowUpBothColor);
                }

                scrollIndicatorColor = dat_80423330;
            scrollIndicatorScale = *(Vec3*)(mailCatalog + 0x4D0);
            scrollIndicatorBase = *(Vec3*)(mailCatalog + 0x4C4);
            scrollIndicatorBase.x = *(f32*)(win + 0x244) + float_360_80423394;
            scrollIndicatorBase.y = float_20_80423398 + (*(f32*)(win + 0x24C) - float_230_80423374);
            scrollIndicatorPos = scrollIndicatorBase;

                winIconSet(0x78, &scrollIndicatorPos,
                           &scrollIndicatorScale, &scrollIndicatorColor);
            }
            }
    }
}

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
