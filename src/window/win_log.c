#include "window/win_log.h"

extern void* mapalloc_base_ptr;
extern const char str_msg_menu_kiroku_map_802f6708[];
extern const f64 double_to_int_802f6b70;
extern f32 float_neg128_80423c34;
extern f32 float_500_80423b40;
extern f32 float_neg300_80423b44;
extern f32 float_45_80423bdc;
extern f32 float_0p5_80423af8;
extern f32 float_8_80423b04;
extern f32 float_320_80423c0c;
extern f32 float_neg240_80423bf0;
extern f32 float_neg200_80423c30;
extern f32 float_60_80423b58;
extern s16 tubuDt[];
extern void* gp;

void fileFree(void* ptr);
void animPoseRelease(s32 poseId);
void smartFree(void* ptr);
void _mapFree(void* heap, void* ptr);
void* _mapAlloc(void* heap, u32 size);
s32 swByteGet(s32 index);
s32 swGet(s32 index);
void* battleGetUnitMonosiriPtr(s32 unit);
void winTexInit(void* data);
void winFontInit(void);
void winIconInit(void);
s32 sprintf(char* buffer, const char* format, ...);
void* animPoseGetAnimDataPtr(s32 poseId);
void animPoseMain(s32 poseId);
void TEXGetGXTexObjFromPalette(void* tpl, void* texObj, s32 id);
s32 GXGetTexObjWidth(void* texObj);
s32 GXGetTexObjHeight(void* texObj);
void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);

struct MonosiriWork {
    s32 state;
    s32 param;
    void* file0;
    void* file1;
    f32 closeX;
    f32 closeY;
    f32 x;
    f32 y;
    s32 pose0;
    s32 pose1;
    void* buffer;
    s32 flags;
};
void winLogInit(void* pWin) {
    extern s32 pouchCheckItem(s32 item);
    extern void qqsort(void* base, s32 count, s32 size, void* compare);
    extern s32 compare_func5_2(void* a, void* b);
    extern s32 compare_func6_2(void* a, void* b);
    s32 i;
    s32 badgeCount = 0;
    s32 recipeCount = 0;
    s32 tattleCount = 0;
    s32 submenuCount = 0;
    u8* submenu;
    u16* badgeIds = (u16*)((s32)pWin + 0xEBC);
    u16* recipeIds = (u16*)((s32)pWin + 0xFBC);
    u16* tattleIds = (u16*)((s32)pWin + 0x1058);

    *(s32*)((s32)pWin + 0xD74) = 0;
    *(s32*)((s32)pWin + 0xD70) = 0;
    *(s32*)((s32)pWin + 0xD6C) = 0;
    submenu = (u8*)pWin + 0xD7C;
    *(s32*)(submenu + submenuCount * 0x1C) = 0;
    submenuCount++;
    for (i = 0; i < submenuCount; i++) {
        u8* item = submenu + i * 0x1C;
        *(f32*)(item + 4) = 0.0f;
        *(f32*)(item + 0xC) = 0.0f;
        *(f32*)(item + 8) = 45.0f * (f32)submenuCount * 0.5f - 45.0f * (f32)i;
        *(f32*)(item + 0x10) = *(f32*)(item + 8);
    }
    if (swByteGet(0) > 0x12) {
        *(s32*)(submenu + submenuCount * 0x1C) = 1;
        submenuCount++;
        for (i = 0; i < submenuCount; i++) {
            u8* item = submenu + i * 0x1C;
            *(f32*)(item + 4) = 0.0f;
            *(f32*)(item + 0xC) = 0.0f;
            *(f32*)(item + 8) = 45.0f * (f32)submenuCount * 0.5f - 45.0f * (f32)i;
            *(f32*)(item + 0x10) = *(f32*)(item + 8);
        }
    }
    if (swByteGet(0) > 6) {
        *(s32*)(submenu + submenuCount * 0x1C) = 4;
        submenuCount++;
        for (i = 0; i < submenuCount; i++) {
            u8* item = submenu + i * 0x1C;
            *(f32*)(item + 4) = 0.0f;
            *(f32*)(item + 0xC) = 0.0f;
            *(f32*)(item + 8) = 45.0f * (f32)submenuCount * 0.5f - 45.0f * (f32)i;
            *(f32*)(item + 0x10) = *(f32*)(item + 8);
        }
    }
    if (swByteGet(0) > 0x13) {
        *(s32*)(submenu + submenuCount * 0x1C) = 2;
        submenuCount++;
        for (i = 0; i < submenuCount; i++) {
            u8* item = submenu + i * 0x1C;
            *(f32*)(item + 4) = 0.0f;
            *(f32*)(item + 0xC) = 0.0f;
            *(f32*)(item + 8) = 45.0f * (f32)submenuCount * 0.5f - 45.0f * (f32)i;
            *(f32*)(item + 0x10) = *(f32*)(item + 8);
        }
    }
    if (swGet(0x40) != 0) {
        *(s32*)(submenu + submenuCount * 0x1C) = 3;
        submenuCount++;
        for (i = 0; i < submenuCount; i++) {
            u8* item = submenu + i * 0x1C;
            *(f32*)(item + 4) = 0.0f;
            *(f32*)(item + 0xC) = 0.0f;
            *(f32*)(item + 8) = 45.0f * (f32)submenuCount * 0.5f - 45.0f * (f32)i;
            *(f32*)(item + 0x10) = *(f32*)(item + 8);
        }
    }
    *(s32*)((s32)pWin + 0xD78) = submenuCount;
    for (i = 0; i < submenuCount; i++) {
        u8* item = submenu + i * 0x1C;
        *(f32*)(item + 4) = 0.0f;
        *(f32*)(item + 0xC) = 0.0f;
        *(f32*)(item + 8) = 45.0f * (f32)submenuCount * 0.5f - 45.0f * (f32)i;
        *(f32*)(item + 0x10) = *(f32*)(item + 8);
        *(s32*)(item + 0x14) = 0;
        *(s32*)(item + 0x18) = 0;
    }
    for (i = 0; i < 0x153; i++) {
        if (swGet(i + 0x80) != 0) badgeIds[badgeCount++] = (u16)i;
    }
    for (i = 0; i < 0xEC; i++) {
        if (swGet(i + 0x41) != 0) recipeIds[recipeCount++] = (u16)i;
    }
    for (i = 0; i < 0xD8; i++) {
        void* data = battleGetUnitMonosiriPtr(i);
        if (*(void**)((s32)data + 8) != 0) {
            tattleIds[tattleCount] = (u16)i;
            tattleCount++;
        }
    }
    for (i = 0; i < 7; i++) if (pouchCheckItem(0x70 + i) != 0) (*(s32*)((s32)pWin + 0xDA0))++;
    *(s32*)((s32)pWin + 0xE94) = badgeCount;
    *(s32*)((s32)pWin + 0xF94) = recipeCount;
    *(s32*)((s32)pWin + 0x1040) = tattleCount;
    *(s32*)((s32)pWin + 0x1044) = 0;
    for (i = 0; i < tattleCount; i++) {
        if (swGet((tattleIds[i] & 0xFF) + 0x117A) != 0) {
            *(s32*)((s32)pWin + 0x1044) += 1;
        }
    }
    *(void**)((s32)pWin + 0x30) = 0;
    *(s32*)((s32)pWin + 0xE1C) = -1;
    *(s32*)((s32)pWin + 0xE14) = -1;
    *(f32*)((s32)pWin + 0xE38) = 0.0f;
    *(f32*)((s32)pWin + 0xE3C) = 0.0f;
    qqsort(badgeIds, badgeCount, 2, compare_func5_2);
    qqsort(recipeIds, recipeCount, 2, compare_func6_2);
}

void winLogInit2(void* pWin) {
    f32 f45;
    f32 f05;
    f32 f8;
    f32 f320;
    f32 f240;
    f32 f300;
    f32 f200;
    f32 a;
    f32 b;
    f32 value;

    *(f32*)((s32)pWin + 0x158) = float_neg128_80423c34;
    f45 = float_45_80423bdc;
    a = f45 * (f32)*(s32*)((s32)pWin + 0xD70);
    b = f45 * (f32)*(s32*)((s32)pWin + 0xD6C);
    f05 = float_0p5_80423af8;
    f8 = float_8_80423b04;
    f320 = float_320_80423c0c;
    f240 = float_neg240_80423bf0;
    f300 = float_neg300_80423b44;
    f200 = float_neg200_80423c30;
    value = (a * f05) - b;
    *(f32*)((s32)pWin + 0x15C) = f8 + value;

    *(f32*)((s32)pWin + 0xE88) = f320;
    *(f32*)((s32)pWin + 0xE84) = f320;
    *(f32*)((s32)pWin + 0xE90) = f240;
    *(f32*)((s32)pWin + 0xE8C) = f240;
    value = (f32)(*(s32*)((s32)pWin + 0xEA0) * 0xE8);
    *(f32*)((s32)pWin + 0xEA8) = value;
    *(f32*)((s32)pWin + 0xEA4) = value;
    *(f32*)((s32)pWin + 0xEB0) = f200;
    *(f32*)((s32)pWin + 0xEAC) = f200;
    *(f32*)((s32)pWin + 0xEB8) = f300;
    *(f32*)((s32)pWin + 0xEB4) = f300;

    *(f32*)((s32)pWin + 0xF88) = f320;
    *(f32*)((s32)pWin + 0xF84) = f320;
    *(f32*)((s32)pWin + 0xF90) = f240;
    *(f32*)((s32)pWin + 0xF8C) = f240;
    value = (f32)(*(s32*)((s32)pWin + 0xFA0) * 0xE8);
    *(f32*)((s32)pWin + 0xFA8) = value;
    *(f32*)((s32)pWin + 0xFA4) = value;
    *(f32*)((s32)pWin + 0xFB0) = f200;
    *(f32*)((s32)pWin + 0xFAC) = f200;
    *(f32*)((s32)pWin + 0xFB8) = f300;
    *(f32*)((s32)pWin + 0xFB4) = f300;
    winMsgEntry(pWin, 0, *(char**)((s32)pWin + 0xD90 + (*(s32*)((s32)pWin + 0xD6C) << 5)), 0);
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 winLogExit(void* pWin) {
    extern void effSoftDelete(s32 id);
    s32 i;

    if (*(void**)((s32)pWin + 0x30) != NULL) {
        fileFree(*(void**)((s32)pWin + 0x30));
        *(void**)((s32)pWin + 0x30) = NULL;
    }
    if (*(s32*)((s32)pWin + 0xE1C) != -1) {
        animPoseRelease(*(s32*)((s32)pWin + 0xE1C));
        *(s32*)((s32)pWin + 0xE1C) = -1;
    }
    for (i = 0; i < 7; i++) {
        effSoftDelete(*(s32*)((s32)pWin + 0xE44));
        pWin = (void*)((s32)pWin + 4);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



s32 winLogMain(void* pWin) {
    extern void psndSFXOn(s32 id);
    extern void winMsgEntry(void* win, s32 item, char* msg, s32 type);
    extern void winSortEntry(f32 x, f32 y, void* win, s32 type);
    extern s32 winSortWait(void* win);
    extern void* monosiriInit(s32 enemy, f32 x, f32 y);
    extern s32 monosiriMain(void* work);
    extern u32 keyGetButton(s32 channel);
    extern f32 float_255_80423c28;
    extern f32 float_120_80423bcc;
    extern f32 float_neg130_80423c2c;
    extern f32 float_20_80423c08;
    s32 state = *(s32*)((s32)pWin + 0xD68);
    s32 cursor = *(s32*)((s32)pWin + 0xD6C);
    s32 page = *(s32*)((s32)pWin + 0x104C);
    s32 count = *(s32*)((s32)pWin + 0xD70);
    u32 pressed = *(u32*)((s32)pWin + 4);
    u32 repeat = *(u32*)((s32)pWin + 0x10);
    u32 directions = *(u32*)((s32)pWin + 0xC);
    s32 i;

    if (state == 0) {
        s32 oldCursor = cursor;
        if ((pressed & 0x200) != 0) {
            psndSFXOn(0x20013);
            return -1;
        }
        if ((pressed & 0x1000) != 0) return -2;
        if ((repeat & 0x2000) != 0) {
            cursor++;
            if (cursor >= count) cursor = 0;
        } else if ((repeat & 0x1000) != 0) {
            cursor--;
            if (cursor < 0) cursor = count - 1;
        }
        *(s32*)((s32)pWin + 0xD6C) = cursor;
        if (oldCursor != cursor) psndSFXOn(0x20005);
        if ((pressed & 0x100) != 0) {
            for (i = 0; i < count; i++) {
                *(s32*)((s32)pWin + 0xD88 + i * 0x20) = 30;
            }
            *(s32*)((s32)pWin + 0xD88 + cursor * 0x20) = 10;
            state = *(s32*)((s32)pWin + 0xD74 + cursor * 0x20) + 10;
            *(s32*)((s32)pWin + 0xD68) = state;
            switch (*(s32*)((s32)pWin + 0xD7C + cursor * 0x1C)) {
            case 0:
                *(f32*)((s32)pWin + 0xE3C) = float_20_80423c08;
                break;
            case 1:
                *(f32*)((s32)pWin + 0xE68) = float_20_80423c08;
                *(f32*)((s32)pWin + 0xE70) = float_255_80423c28;
                break;
            case 2:
                *(f32*)((s32)pWin + 0xE88) = float_neg130_80423c2c;
                *(f32*)((s32)pWin + 0xE90) = float_120_80423bcc;
                *(f32*)((s32)pWin + 0xEB0) = float_neg200_80423c30;
                *(f32*)((s32)pWin + 0xEB8) = float_60_80423b58;
                break;
            case 3:
                *(f32*)((s32)pWin + 0xF88) = float_neg130_80423c2c;
                *(f32*)((s32)pWin + 0xF90) = float_120_80423bcc;
                *(f32*)((s32)pWin + 0xFB0) = float_neg200_80423c30;
                *(f32*)((s32)pWin + 0xFB8) = float_60_80423b58;
                break;
            }
            psndSFXOn(0x20012);
        }
        *(f32*)((s32)pWin + 0x158) = float_neg128_80423c34;
        *(f32*)((s32)pWin + 0x15C) =
            float_8_80423b04 + float_45_80423bdc * (f32)count *
            float_0p5_80423af8 - float_45_80423bdc * (f32)cursor;
    } else if (state == 10) {
        f32 moveX = 0.0f;
        f32 moveY = 0.0f;
        if ((keyGetButton(0) & 1) != 0) moveX = -8.0f;
        if ((keyGetButton(0) & 2) != 0) moveX = 8.0f;
        if ((keyGetButton(0) & 4) != 0) moveY = 8.0f;
        if ((keyGetButton(0) & 8) != 0) moveY = -8.0f;
        if (moveX == 0.0f && moveY == 0.0f &&
            *(s32*)((s32)pWin + 0xE14) != -1) {
            s32 area = *(s32*)((s32)pWin + 0xE14);
            moveX = (304.0f + (f32)tubuDt[area * 6] -
                     *(f32*)((s32)pWin + 0xE20)) * 0.2f;
            moveY = (240.0f + (f32)tubuDt[area * 6 + 1] -
                     *(f32*)((s32)pWin + 0xE24)) * 0.2f;
        }
        *(f32*)((s32)pWin + 0xE20) += moveX;
        *(f32*)((s32)pWin + 0xE24) += moveY;
        if (*(f32*)((s32)pWin + 0xE20) < 16.0f)
            *(f32*)((s32)pWin + 0xE20) = 16.0f;
        if (*(f32*)((s32)pWin + 0xE24) < 8.0f)
            *(f32*)((s32)pWin + 0xE24) = 8.0f;
        if (*(f32*)((s32)pWin + 0xE20) >= 604.0f)
            *(f32*)((s32)pWin + 0xE20) = 604.0f;
        if (*(f32*)((s32)pWin + 0xE24) >= 468.0f)
            *(f32*)((s32)pWin + 0xE24) = 468.0f;
    } else if (state == 11 || state == 12 || state == 13) {
        if ((pressed & 0x200) != 0) {
            for (i = 0; i < count; i++) {
                *(s32*)((s32)pWin + 0xD88 + i * 0x20) = 40;
            }
            *(s32*)((s32)pWin + 0xD88 + cursor * 0x20) = 20;
            *(s32*)((s32)pWin + 0xD68) = 0;
            *(f32*)((s32)pWin + 0x1034) = float_320_80423c0c;
            *(f32*)((s32)pWin + 0x103C) = float_neg240_80423bf0;
            psndSFXOn(0x20013);
        } else if ((repeat & 0x1000) != 0 && cursor > 0) {
            *(s32*)((s32)pWin + 0xD6C) = cursor - 1;
        } else if ((repeat & 0x2000) != 0 && cursor + 1 < count) {
            *(s32*)((s32)pWin + 0xD6C) = cursor + 1;
        } else if ((pressed & 0x400) != 0) {
            *(s32*)((s32)pWin + 0x14) = state;
            winSortEntry(-310.0f, 150.0f, pWin, state - 7);
            psndSFXOn(0x20012);
            *(s32*)((s32)pWin + 0xD68) = 1000;
        }
    } else if (state == 14) {
        if ((pressed & 0x200) != 0) {
            *(s32*)((s32)pWin + 0xD68) = 0;
        } else if ((repeat & 0x40) != 0) {
            page--;
            if (page < 0) page = 0;
            cursor = page << 4;
            *(s32*)((s32)pWin + 0xD6C) = page;
            *(s32*)((s32)pWin + 0xD70) = cursor;
            psndSFXOn(0x20035);
        } else if ((repeat & 0x20) != 0) {
            page++;
            if ((page << 4) >= count) page--;
            cursor = page << 4;
            *(s32*)((s32)pWin + 0xD6C) = page;
            *(s32*)((s32)pWin + 0xD70) = cursor;
            psndSFXOn(0x20035);
        } else if ((directions & 0xC000) != 0) {
            if ((cursor & 0xF) < 8) cursor += 8;
            else cursor -= 8;
            if (cursor >= count) cursor = count - 1;
            if (cursor < 0) cursor = 0;
            *(s32*)((s32)pWin + 0xD70) = cursor;
            psndSFXOn(0x20035);
        } else if ((directions & 0x1000) != 0) {
            cursor = ((cursor & 7) == 0) ? cursor - 9 : cursor - 1;
            if (cursor < 0) cursor = 0;
            *(s32*)((s32)pWin + 0xD70) = cursor;
            *(s32*)((s32)pWin + 0xD6C) = cursor >> 4;
            psndSFXOn(0x20035);
        } else if ((directions & 0x2000) != 0) {
            cursor = ((cursor & 7) == 7) ? cursor + 9 : cursor + 1;
            if (cursor >= count) cursor = count - 1;
            *(s32*)((s32)pWin + 0xD70) = cursor;
            *(s32*)((s32)pWin + 0xD6C) = cursor >> 4;
            psndSFXOn(0x20035);
        } else if ((pressed & 0x100) != 0 && count > 0 &&
                   swGet(*(u8*)((s32)pWin + 0x1000 + cursor * 2 + 1) + 0x117A) != 0) {
            psndSFXOn(0x20012);
            *(void**)((s32)pWin + 0x102C) =
                monosiriInit(*(u8*)((s32)pWin + 0x1000 + cursor * 2 + 1),
                             500.0f, -300.0f);
            *(s32*)((s32)pWin + 0xD68) = 21;
        }
    } else if (state == 21) {
        monosiriMain(*(void**)((s32)pWin + 0x1208));
    } else if (state == 22) {
        void* work = *(void**)((s32)pWin + 0x1208);
        if (work == 0 ||
            (*(f32*)((s32)work + 0x10) > 500.0f &&
             *(f32*)((s32)work + 0x14) < -300.0f)) {
            *(s32*)((s32)pWin + 0xD68) = 14;
        } else {
            monosiriMain(work);
        }
    } else if (state == 1000 || state == 1001) {
        if (winSortWait(pWin) == 0) {
            *(s32*)((s32)pWin + 0xD68) = *(s32*)((s32)pWin + 0x124);
        }
        if ((pressed & 0x1000) != 0) {
            return -2;
        }
    }
    return 0;
}

void winLogMain2(void* pWin) {
    extern f32 float_0_80423aec;
    extern f32 float_140_80423c20, float_neg160_80423c24;
    extern f32 float_6_80423b54, float_4_80423be8;
    s32 i;
    s32 count = *(s32*)((s32)pWin + 0xD70);
    u8* entry = (u8*)pWin + 0xD78;
    #define FPS_FLOAT ((f32)*(s32*)((s32)gp + 4))
    #define DENOM8 ((float_8_80423b04 * FPS_FLOAT) / float_60_80423b58)
    #define DENOM6 ((float_6_80423b54 * FPS_FLOAT) / float_60_80423b58)
    #define DENOM4 ((float_4_80423be8 * FPS_FLOAT) / float_60_80423b58)

    for (i = 0; i < count; i++, entry += 0x20) {
        s32 state = *(s32*)(entry + 0x10);
        switch (state) {
            case 10:
            case 20:
                *(f32*)(entry + 8) = float_0_80423aec;
                *(f32*)(entry + 0xC) = float_140_80423c20;
                *(s32*)(entry + 0x14) = 0;
                *(s32*)(entry + 0x10) = state + 1;
                break;
            case 11:
                if (++*(s32*)(entry + 0x14) > 8) {
                    *(f32*)(entry + 8) = float_neg160_80423c24;
                    *(f32*)(entry + 0xC) = float_140_80423c20;
                    *(s32*)(entry + 0x14) = 0;
                    *(s32*)(entry + 0x10) = state + 1;
                }
                break;
            case 21:
                if (++*(s32*)(entry + 0x14) > 8) {
                    *(f32*)(entry + 8) = float_0_80423aec;
                    *(f32*)(entry + 0xC) = float_45_80423bdc * (f32)count * float_0p5_80423af8 - float_45_80423bdc * (f32)i;
                    *(s32*)(entry + 0x14) = 0;
                    *(s32*)(entry + 0x10) = state + 1;
                }
                break;
            case 30:
                *(f32*)(entry + 8) = float_0_80423aec;
                *(f32*)(entry + 0xC) = float_neg160_80423c24;
                *(s32*)(entry + 0x14) = 0;
                *(s32*)(entry + 0x10) = state + 1;
                break;
            case 40:
                *(f32*)(entry + 8) = float_0_80423aec;
                *(f32*)(entry + 0xC) = float_45_80423bdc * (f32)count * float_0p5_80423af8 - float_45_80423bdc * (f32)i;
                *(s32*)(entry + 0x14) = 0;
                *(s32*)(entry + 0x10) = state + 1;
                break;
        }
        *(f32*)entry += (*(f32*)(entry + 8) - *(f32*)entry) / DENOM8;
        *(f32*)(entry + 4) += (*(f32*)(entry + 0xC) - *(f32*)(entry + 4)) / DENOM8;
    }

    #define STEP(cur, tgt, d) (*(f32*)((s32)pWin + (cur)) += (*(f32*)((s32)pWin + (tgt)) - *(f32*)((s32)pWin + (cur))) / (d))
    STEP(0xE2C, 0xE28, DENOM8);
    STEP(0xE34, 0xE30, DENOM8);
    STEP(0xE68, 0xE64, DENOM8);
    STEP(0xE70, 0xE6C, DENOM8);
    STEP(0xE78, 0xE74, DENOM8);
    STEP(0xE80, 0xE7C, DENOM4);
    *(f32*)((s32)pWin + 0xEA8) = (f32)(*(s32*)((s32)pWin + 0xEA0) * 0xE8);
    STEP(0xEA4, 0xEA8, DENOM6);
    STEP(0xE84, 0xE88, DENOM6);
    STEP(0xE8C, 0xE90, DENOM6);
    STEP(0xEAC, 0xEB0, DENOM6);
    STEP(0xEB4, 0xEB8, DENOM6);
    *(f32*)((s32)pWin + 0xFA8) = (f32)(*(s32*)((s32)pWin + 0xFA0) * 0xE8);
    STEP(0xFA4, 0xFA8, DENOM6);
    STEP(0xF84, 0xF88, DENOM6);
    STEP(0xF8C, 0xF90, DENOM6);
    STEP(0xFAC, 0xFB0, DENOM6);
    STEP(0xFB4, 0xFB8, DENOM6);
    #undef STEP
    #undef DENOM4
    #undef DENOM6
    #undef DENOM8
    #undef FPS_FLOAT
}

void winLogDisp(s32 cameraId, void* pWin, s32 index) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void winBgGX(f32 x, f32 y, void* win, s32 type);
    extern void mapGX(f32 x, f32 y, void* win);
    extern void monoshiriGX(f32 x, f32 y, void* win);
    extern void winHakoGX(f32 x, f32 y, void* win, s32 type);
    extern void winKirinukiGX(f32 x, f32 y, f32 width, f32 height,
                              void* win, s32 type);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void winTexInit_x2(void* data);
    extern void winTexSet_x2(s32 id, s32 frame, Vec3* pos,
                             Vec3* scale, void* color);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* text);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern void effStarStoneDraw(void* camera, void* effect);
    extern u16 FontGetMessageWidth(char* text);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 radians);
    extern void PSMTXMultVec(void* mtx, Vec3* src, Vec3* dst);
    extern void winKageGX(f32 x, f32 y, f32 z, f32 scale,
                          void* win, void* color);
    extern u32 dat_80423aa4;
    extern u32 dat_80423aa8;
    extern u32 dat_80423ab0;
    extern u32 dat_80423ab4;
    extern u32 dat_80423ab8;
    extern u32 dat_80423abc;
    extern f32 float_50_80423b64;
    extern f32 float_120_80423bcc;
    extern f32 float_100_80423b60;
    extern f32 float_0p75_80423b10;
    extern f32 float_128_80423c10;
    extern f32 float_4_80423be8;
    extern f32 float_10_80423c14;
    extern f32 float_0p43633_80423bfc;
    extern f32 float_deg2rad_80423c00;
    extern f32 float_51p429_80423c04;
    extern f32 float_20_80423c08;
    extern f32 float_30_80423bc0;
    extern f32 float_1_80423af0;
    extern u32 dat_80423aac;
    extern u8 itemDataTable[];
    Vec3 pos, scale;
    u8* catalog = (u8*)str_msg_menu_kiroku_map_802f6708;
    volatile u32 white = dat_80423aa4;
    volatile u32 submenuWhite = dat_80423aa4;
    f32 x = *(f32*)((s32)pWin + 0xC4 + index * 0x14);
    f32 y = *(f32*)((s32)pWin + 0xC8 + index * 0x14);
    s32 i;
    s32 width;
    char badgeText[256];
    char recipeText[256];
    Vec3 badgeFontPos, badgeFontCallPos, badgeFontScale;
    Vec3 badgeTexPos, badgeTexCallPos, badgeTexScale;
    Vec3 badgeIconPos, badgeIconCallPos, badgeIconScale;
    Vec3 recipeFontPos, recipeFontCallPos, recipeFontScale;
    Vec3 recipeTexPos, recipeTexCallPos, recipeTexScale;
    Vec3 recipeIconPos, recipeIconCallPos, recipeIconScale;
    u32 badgeFontColor;
    u32 badgeTexColor;
    u32 badgeIconColor;
    u32 recipeFontColor;
    u32 recipeTexColor;
    u32 recipeIconColor;
    u16 messageWidth;
    u16 scaledWidth;
    Vec3 submenuSelectedPos, submenuSelectedCallPos, submenuSelectedScale;
    Vec3 submenuSelectedIconPos, submenuSelectedIconCallPos, submenuSelectedIconScale;
    Vec3 submenuNormalPos, submenuNormalCallPos, submenuNormalScale;
    u32 submenuSelectedColor;
    u32 submenuSelectedIconColor;
    u32 submenuNormalColor;

    winBgGX(x, y, pWin, 4);
    if (y + *(f32*)((s32)pWin + 0xE34) > -480.0f) {
        mapGX(x + *(f32*)((s32)pWin + 0xE2C),
              y + *(f32*)((s32)pWin + 0xE34), pWin);
    }
    if (y + *(f32*)((s32)pWin + 0xE70) > -480.0f) {
        Vec3 crystalPos;
        Vec3 crystalScale;
        u32 crystalColor;
        s32 crystalAlpha;

        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));

        crystalAlpha = (s32)*(f32*)((s32)pWin + 0xE78);
        crystalColor = dat_80423aa8;
        ((u8*)&crystalColor)[3] = (u8)crystalAlpha;

        crystalScale = *(Vec3*)(catalog + 0x80);
        crystalPos = *(Vec3*)(catalog + 0x74);
        crystalPos.x = x + *(f32*)((s32)pWin + 0xE68);
        crystalPos.y = y + *(f32*)((s32)pWin + 0xE70);
        winTexSet(0x24, &crystalPos, &crystalScale, &crystalColor);

        {
            f32 rotateX[3][4];
            f32 rotateY[3][4];
            Vec3 orbit;
            u32 shadowColor;
            void* effect;
            void* allocation;
            u8* effectWork;
            u8* drawWork;
            s32 stoneIndex;
            s32 drawIndex;
            f32 degreeToRad;
            f32 stoneAngleStep;
            f32 shadowYOffset;
            f32 effectYOffset;

            degreeToRad = float_deg2rad_80423c00;
            stoneAngleStep = float_51p429_80423c04;
            shadowYOffset = float_30_80423bc0;
            effectYOffset = float_20_80423c08;
            effectWork = (u8*)pWin;

            for (stoneIndex = 0; stoneIndex < 7; stoneIndex++) {
                orbit = *(Vec3*)(catalog + 0x8C);
                PSMTXRotRad(rotateX, 'x', float_0p43633_80423bfc);
                PSMTXRotRad(rotateY, 'y',
                    degreeToRad *
                    (stoneAngleStep * (f32)stoneIndex -
                     *(f32*)((s32)pWin + 0xE80)));
                PSMTXMultVec(rotateY, &orbit, &orbit);
                PSMTXMultVec(rotateX, &orbit, &orbit);

                shadowColor = dat_80423aac;
                winKageGX(orbit.x + x + *(f32*)((s32)pWin + 0xE68),
                          orbit.y + y + *(f32*)((s32)pWin + 0xE70) -
                              shadowYOffset,
                          orbit.z, float_1_80423af0, pWin, &shadowColor);

                effect = *(void**)(effectWork + 0xE44);
                allocation = *(void**)((s32)effect + 0xC);
                *(f32*)((s32)allocation + 8) =
                    orbit.x + x + *(f32*)((s32)pWin + 0xE68);

                effect = *(void**)(effectWork + 0xE44);
                allocation = *(void**)((s32)effect + 0xC);
                *(f32*)((s32)allocation + 0xC) =
                    effectYOffset + orbit.y + y +
                    *(f32*)((s32)pWin + 0xE70);

                effect = *(void**)(effectWork + 0xE44);
                allocation = *(void**)((s32)effect + 0xC);
                *(f32*)((s32)allocation + 0x10) = orbit.z;

                effectWork += 4;
            }

            drawWork = (u8*)pWin;
            for (drawIndex = 0; drawIndex < 7; drawIndex++, drawWork += 4) {
                effStarStoneDraw((void*)8, *(void**)(drawWork + 0xE44));
            }
        }
    }
    if (*(f32*)((s32)pWin + 0xE84) < 320.0f &&
        *(f32*)((s32)pWin + 0xE8C) > float_neg240_80423bf0) {
        char* format = (char*)catalog + 0x4C8;

        winHakoGX(x + *(f32*)((s32)pWin + 0xE84),
                  y + *(f32*)((s32)pWin + 0xE8C), pWin, 0);

        badgeFontColor = dat_80423ab0;
        winKirinukiGX(x + *(f32*)((s32)pWin + 0xEAC) - 50.0f,
                      y + *(f32*)((s32)pWin + 0xEB4) - float_120_80423bcc,
                      float_100_80423b60, float_30_80423bc0, pWin, 0);

        if (*(s32*)((s32)gp + 0x16C) != 0) {
            format = "%d/%d";
        }
        sprintf(badgeText, format, *(s32*)((s32)pWin + 0xE98),
                *(s32*)((s32)pWin + 0xE94));

        messageWidth = FontGetMessageWidth(badgeText);
        scaledWidth = (u16)(float_0p75_80423b10 * (f32)messageWidth);

        winFontInit();
        badgeFontScale = *(Vec3*)(catalog + 0xA4);
        badgeFontPos = *(Vec3*)(catalog + 0x98);
        badgeFontPos.x =
            x + *(f32*)((s32)pWin + 0xEAC) - float_50_80423b64 +
            (f32)((100 - scaledWidth) / 2);
        badgeFontPos.y =
            y + *(f32*)((s32)pWin + 0xEB4) - 128.0f;
        badgeFontCallPos = badgeFontPos;
        winFontSet(&badgeFontCallPos, &badgeFontScale,
                   &badgeFontColor, badgeText);

        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
        badgeTexColor = white;
        badgeTexScale = *(Vec3*)(catalog + 0xBC);
        badgeTexPos = *(Vec3*)(catalog + 0xB0);
        badgeTexPos.x = x + *(f32*)((s32)pWin + 0xEAC);
        badgeTexPos.y = y + *(f32*)((s32)pWin + 0xEB4);
        badgeTexCallPos = badgeTexPos;
        winTexSet(0x95, &badgeTexCallPos, &badgeTexScale, &badgeTexColor);

        {
            s16 badgeId = *(s16*)((s32)pWin + 0xEBC +
                                  *(s32*)((s32)pWin + 0xE9C) * 2);
            if (swGet(badgeId + 0x80) != 0) {
                winIconInit();
                badgeIconColor = white;
                badgeIconScale = *(Vec3*)(catalog + 0xD4);
                badgeIconPos = *(Vec3*)(catalog + 0xC8);
                badgeIconPos.x = x + *(f32*)((s32)pWin + 0xEAC);
                badgeIconPos.y =
                    y + *(f32*)((s32)pWin + 0xEB4) - 4.0f;
                badgeIconCallPos = badgeIconPos;
                winIconSet(*(u16*)(itemDataTable +
                                   (badgeId + 0xF0) * 0x28 + 0x20),
                           &badgeIconCallPos, &badgeIconScale, &badgeIconColor);
            }
        }
    }
    if (*(f32*)((s32)pWin + 0xF84) < float_320_80423c0c &&
        *(f32*)((s32)pWin + 0xF8C) > float_neg240_80423bf0) {
        char* format = (char*)catalog + 0x4C8;

        winHakoGX(x + *(f32*)((s32)pWin + 0xF84),
                  y + *(f32*)((s32)pWin + 0xF8C), pWin, 1);

        recipeFontColor = dat_80423ab4;
        winKirinukiGX(x + *(f32*)((s32)pWin + 0xFAC) - float_50_80423b64,
                      y + *(f32*)((s32)pWin + 0xFB4) - float_120_80423bcc,
                      float_100_80423b60, float_30_80423bc0, pWin, 0);

        if (*(s32*)((s32)gp + 0x16C) != 0) {
            format = "%d/%d";
        }
        sprintf(recipeText, format, *(s32*)((s32)pWin + 0xF98),
                *(s32*)((s32)pWin + 0xF94));

        messageWidth = FontGetMessageWidth(recipeText);
        scaledWidth = (u16)(float_0p75_80423b10 * (f32)messageWidth);

        winFontInit();
        recipeFontScale = *(Vec3*)(catalog + 0xEC);
        recipeFontPos = *(Vec3*)(catalog + 0xE0);
        recipeFontPos.x =
            x + *(f32*)((s32)pWin + 0xFAC) - float_50_80423b64 +
            (f32)((100 - scaledWidth) / 2);
        recipeFontPos.y =
            y + *(f32*)((s32)pWin + 0xFB4) - float_128_80423c10;
        recipeFontCallPos = recipeFontPos;
        winFontSet(&recipeFontCallPos, &recipeFontScale,
                   &recipeFontColor, recipeText);

        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
        recipeTexColor = white;
        recipeTexScale = *(Vec3*)(catalog + 0x104);
        recipeTexPos = *(Vec3*)(catalog + 0xF8);
        recipeTexPos.x = x + *(f32*)((s32)pWin + 0xFAC);
        recipeTexPos.y = y + *(f32*)((s32)pWin + 0xFB4);
        recipeTexCallPos = recipeTexPos;
        winTexSet(0x96, &recipeTexCallPos, &recipeTexScale, &recipeTexColor);

        {
            s16 recipeId = *(s16*)((s32)pWin + 0xFBC +
                                   *(s32*)((s32)pWin + 0xF9C) * 2);
            if (swGet(recipeId + 0x41) != 0) {
                winIconInit();
                recipeIconColor = white;
                recipeIconScale = *(Vec3*)(catalog + 0x11C);
                recipeIconPos = *(Vec3*)(catalog + 0x110);
                recipeIconPos.x = x + *(f32*)((s32)pWin + 0xFAC);
                recipeIconPos.y =
                    y + *(f32*)((s32)pWin + 0xFB4) - float_10_80423c14;
                recipeIconCallPos = recipeIconPos;
                winIconSet(*(u16*)(itemDataTable +
                                   (recipeId + 0xB3) * 0x28 + 0x20),
                           &recipeIconCallPos, &recipeIconScale, &recipeIconColor);
            }
        }
    }
    {
        s32 state = *(s32*)((s32)pWin + 0xD68);
        if (state == 14 || (u32)(state - 20) < 3 || state == 1001) {
            monoshiriGX(x - 170.0f, y + 130.0f, pWin);
        }
    }
    {
        u8* entry = (u8*)pWin + 0xD74;
        for (i = 0; i < *(s32*)((s32)pWin + 0xD70); i++, entry += 0x20) {
            if (i == *(s32*)((s32)pWin + 0xD6C)) {
                winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
                submenuSelectedColor = dat_80423ab8;
                submenuSelectedScale = *(Vec3*)(catalog + 0x134);
                submenuSelectedPos = *(Vec3*)(catalog + 0x128);
                submenuSelectedPos.x = x + *(f32*)(entry + 4);
                submenuSelectedPos.y = y + *(f32*)(entry + 8);
                submenuSelectedCallPos = submenuSelectedPos;
                winTexSet(0xB5, &submenuSelectedCallPos, &submenuSelectedScale,
                          &submenuSelectedColor);

                winTexInit_x2(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
                submenuSelectedIconColor = submenuWhite;
                submenuSelectedIconScale = *(Vec3*)(catalog + 0x14C);
                submenuSelectedIconPos = *(Vec3*)(catalog + 0x140);
                submenuSelectedIconPos.x = x + *(f32*)(entry + 4) - 8.0f;
                submenuSelectedIconPos.y = y + *(f32*)(entry + 8) + 8.0f;
                submenuSelectedIconCallPos = submenuSelectedIconPos;
                winTexSet_x2(*(s32*)entry + 0x1F, 0xB5,
                             &submenuSelectedIconCallPos, &submenuSelectedIconScale,
                             &submenuSelectedIconColor);
            } else {
                winTexInit_x2(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
                submenuNormalColor = dat_80423abc;
                submenuNormalScale = *(Vec3*)(catalog + 0x164);
                submenuNormalPos = *(Vec3*)(catalog + 0x158);
                submenuNormalPos.x = x + *(f32*)(entry + 4);
                submenuNormalPos.y = y + *(f32*)(entry + 8);
                submenuNormalCallPos = submenuNormalPos;
                winTexSet_x2(*(s32*)entry + 0x1F, 0xB5,
                             &submenuNormalCallPos, &submenuNormalScale,
                             &submenuNormalColor);
            }
        }
    }
    (void)cameraId;
}

char* winGetMapTplName(void) {
    char* base = (char*)str_msg_menu_kiroku_map_802f6708;

    if (swByteGet(0) < 0x13) {
        return base + 0x4F0;
    }
    if (swByteGet(0) < 0x41) {
        return base + 0x4FC;
    }
    if (swByteGet(0) < 0x79) {
        return base + 0x508;
    }
    if (swByteGet(0) < 0xAF) {
        return base + 0x514;
    }
    if (swByteGet(0) < 0xDE) {
        return base + 0x520;
    }
    if (swByteGet(0) < 0x110) {
        return base + 0x52C;
    }
    if (swByteGet(0) >= 0x15C) {
        return base + 0x544;
    }
    return base + 0x538;
}

void mapGX(double x, double y, void* pWin) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef f32 Mtx[3][4];
    extern void GXSetScissor(u32 x, u32 y, u32 w, u32 h);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern double sin(double angle);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern void PSMTXTrans(Mtx matrix, f32 x, f32 y, f32 z);
    extern void animPoseSetLocalTime(f32 time, s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, Mtx matrix, s32 mode,
                                f32 rotation, f32 scale);
    Vec3 pos, scale;
    Vec3 mapPos, mapScale;
    Vec3 selectedPos, selectedScale;
    Vec3 normalPos, normalScale;
    Vec3 iconPos, iconScale;
    Vec3 borderLeftPos, borderLeftScale;
    Vec3 borderRightPos, borderRightScale;
    Vec3 borderTopPos, borderTopScale;
    Vec3 borderBottomPos, borderBottomScale;
    Vec3 arrowTopPos, arrowTopScale;
    Vec3 arrowRightPos, arrowRightScale;
    Vec3 arrowBottomPos, arrowBottomScale;
    Vec3 arrowLeftPos, arrowLeftScale;
    u32 white = 0xFFFFFFFF;
    s32 i;
    s16* area;
    s32 available;
    s32 texture;
    f32 pulse;
    Mtx matrix;

    if (*(void**)((s32)pWin + 0x30) == 0) return;
    GXSetScissor((u32)((f32)x + 72.0f), (u32)(118.0f - (f32)y),
                 0x1D0, 0xF4);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x30) + 0xA0));
    mapPos.x = (f32)x + 74.0f + *(f32*)((s32)pWin + 0xE38);
    mapPos.y = (f32)y + 120.0f + *(f32*)((s32)pWin + 0xE3C);
    mapPos.z = 0.0f;
    mapScale.x = 1.0f;
    mapScale.y = 1.0f;
    mapScale.z = 1.0f;
    winTexSet(0, &mapPos, &mapScale, &white);
    scale = mapScale;
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    area = tubuDt;
    for (i = 0; i < 0x5D; i++, area += 6) {
        available = swByteGet(0) >= (u16)area[3];
        if (!available) {
            switch (*((u8*)area + 5)) {
                case 1: available = swGet(0xFD) != 0; break;
                case 2: available = swGet(0xFE) != 0; break;
                case 3: available = swGet(0xFF) != 0; break;
                case 4: available = swGet(0x100) != 0; break;
                default: break;
            }
        }
        if (!available) {
            continue;
        }
        if (*(s32*)((s32)pWin + 0xE14) == i) {
            selectedPos.x = (f32)x + 74.0f + *(f32*)((s32)pWin + 0xE38) + (f32)area[0];
            selectedPos.y = (f32)y + 120.0f + *(f32*)((s32)pWin + 0xE3C) + (f32)area[1];
            selectedPos.z = 0.0f;
            selectedScale = mapScale;
            texture = ((*(u32*)((s32)gp + 0x1C) >> 4) & 1) ? 0xB9 : 0xBA;
            winTexSet(texture, &selectedPos, &selectedScale, &white);
        } else {
            normalPos.x = (f32)x + 74.0f + *(f32*)((s32)pWin + 0xE38) + (f32)area[0];
            normalPos.y = (f32)y + 120.0f + *(f32*)((s32)pWin + 0xE3C) + (f32)area[1];
            normalPos.z = 0.0f;
            normalScale = mapScale;
            texture = 0xB8;
            if (*((u8*)area + 4) != 0) {
                texture = 0xBA;
            }
            winTexSet(texture, &normalPos, &normalScale, &white);
        }
    }
    winIconInit();
    iconPos.x = (f32)x;
    iconPos.y = (f32)y;
    iconPos.z = 0.0f;
    iconScale = mapScale;
    winIconSet(0x1A6, &iconPos, &iconScale, &white);
    if (*(s32*)((s32)pWin + 0xE1C) != -1) {
        s32 poseId = *(s32*)((s32)pWin + 0x188);
        s32 mapPoseId = *(s32*)((s32)pWin + 0xE1C);
        s32 currentArea = *(s32*)((s32)pWin + 0xE18);
        void* animData;
        u32 duration;
        f32 drawScale = (*(u16*)pWin & 0x2000) != 0 ? 3.0f : 1.5f;

        animPoseGetAnimPosePtr(poseId);
        animData = animPoseGetAnimDataPtr(poseId);
        PSMTXTrans(matrix,
                   (f32)x + 74.0f + *(f32*)((s32)pWin + 0xE38) +
                       (f32)tubuDt[currentArea * 6],
                   (f32)y + 120.0f + *(f32*)((s32)pWin + 0xE3C) +
                       (f32)tubuDt[currentArea * 6 + 1], 0.0f);
        duration = (u32)*(f32*)(*(s32*)((s32)animData + 0x24) + 8);
        animPoseSetLocalTime((f32)(*(u32*)((s32)gp + 0x1C) % duration),
                             mapPoseId);
        animPoseMain(mapPoseId);
        animPoseDrawMtx(mapPoseId, matrix, 1, 0.0f, drawScale);
        animPoseDrawMtx(mapPoseId, matrix, 2, 0.0f, drawScale);
        animPoseDrawMtx(mapPoseId, matrix, 3, 0.0f, drawScale);
    }
    GXSetScissor(0, 0, 0x260, 0x1E0);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    borderLeftPos.x = (f32)x - 234.0f;
    borderLeftPos.y = (f32)y;
    borderLeftPos.z = 0.0f;
    borderLeftScale = mapScale;
    winTexSet(0xBF, &borderLeftPos, &borderLeftScale, &white);
    borderRightPos.x = (f32)x + 234.0f;
    borderRightPos.y = (f32)y;
    borderRightPos.z = 0.0f;
    borderRightScale = mapScale;
    winTexSet(0xBF, &borderRightPos, &borderRightScale, &white);
    borderTopPos.x = (f32)x;
    borderTopPos.y = (f32)y + 124.0f;
    borderTopPos.z = 0.0f;
    borderTopScale = mapScale;
    winTexSet(0xBF, &borderTopPos, &borderTopScale, &white);
    borderBottomPos.x = (f32)x;
    borderBottomPos.y = (f32)y - 124.0f;
    borderBottomPos.z = 0.0f;
    borderBottomScale = mapScale;
    winTexSet(0xBF, &borderBottomPos, &borderBottomScale, &white);

    pulse = 2.0f * (f32)sin((double)((f32)*(u32*)((s32)gp + 0x1C) * 0.125f));
    if (pulse < 0.0f) pulse = -pulse;
    arrowTopPos.x = (f32)x;
    arrowTopPos.y = (f32)y + 120.0f +
            (*(f32*)((s32)pWin + 0xE3C) == -240.0f ? 0.0f : pulse);
    arrowTopPos.z = 0.0f;
    arrowTopScale = mapScale;
    winTexSet(0xBD, &arrowTopPos, &arrowTopScale, &white);
    arrowRightPos.x = (f32)x + 220.0f +
            (*(f32*)((s32)pWin + 0xE38) == -148.0f ? 0.0f : pulse);
    arrowRightPos.y = (f32)y;
    arrowRightPos.z = 0.0f;
    arrowRightScale = mapScale;
    winTexSet(0xBC, &arrowRightPos, &arrowRightScale, &white);
    arrowBottomPos.x = (f32)x;
    arrowBottomPos.y = (f32)y - 120.0f -
            (*(f32*)((s32)pWin + 0xE3C) == 0.0f ? 0.0f : pulse);
    arrowBottomPos.z = 0.0f;
    arrowBottomScale = mapScale;
    winTexSet(0xBE, &arrowBottomPos, &arrowBottomScale, &white);
    arrowLeftPos.x = (f32)x - 220.0f -
            (*(f32*)((s32)pWin + 0xE38) == 0.0f ? 0.0f : pulse);
    arrowLeftPos.y = (f32)y;
    arrowLeftPos.z = 0.0f;
    arrowLeftScale = mapScale;
    winTexSet(0xBB, &arrowLeftPos, &arrowLeftScale, &white);
}

void monoshiriGX(double x, double y, void* pWin) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void winBookGX(double x, double y, void* win, s32 page);
    extern void winKirinukiGX(double x, double y, double width, double height,
                              void* win, s32 type);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* text);
    extern void winFontSetWidth(Vec3* pos, Vec3* scale, void* color,
                                f32 width, char* text, ...);
    extern u16 FontGetMessageWidth(char* text);
    extern void winTexSet(s32 texture, Vec3* pos, Vec3* scale, void* color);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern s32 evtGetValue(void* evt, s32 value);
    extern char* msgSearch(char* key);
    extern char str_msg_menu_monosiri_ka_802f6bb8[];
    extern char str_msg_menu_sort_narabi_802f6be0[];
    extern u8 enemy_monoshiri_sort_table[];
    extern char str_btl_un_hatena_802f6b28[];
    extern char str_btl_un_rampell_802f6b38[];
    Vec3 pos, scale;
    Vec3 titlePos, titleScale;
    Vec3 countPos, countScale;
    u32 white = 0xFFFFFFFF;
    s32 page = *(s32*)((s32)pWin + 0x104C);
    s32 count = *(s32*)((s32)pWin + 0x1040);
    s32 i, idx, unit;
    char countText[256];
    char numberText[32];
    char* name;
    char* title;
    f32 titleWidth;
    f32 countWidth;

    winBookGX(x, y, pWin, 1);
    winKirinukiGX((double)((f32)x - 110.0f), (double)((f32)y - 160.0f),
                  100.0, 50.0, pWin, 0);
    titleScale.x = 0.75f; titleScale.y = 0.75f; titleScale.z = 1.0f;
    titlePos.z = 0.0f;
    title = msgSearch(str_msg_menu_monosiri_ka_802f6bb8);
    titleWidth = 0.75f * (f32)FontGetMessageWidth(title);
    if (titleWidth > 80.0f) {
        titleScale.x *= 80.0f / titleWidth;
        titleWidth = 80.0f;
    }
    winFontInit();
    titlePos.x = (f32)x - 108.0f + (100.0f - titleWidth) * 0.5f;
    titlePos.y = (f32)y - 166.0f;
    winFontSetWidth(&titlePos, &titleScale, &white, 80.0f, title);
    sprintf(countText, "%d/%d", *(s32*)((s32)pWin + 0x1044), count);
    countScale.x = 0.75f; countScale.y = 0.75f; countScale.z = 1.0f;
    countPos.z = 0.0f;
    countWidth = 0.75f * (f32)FontGetMessageWidth(countText);
    countPos.x = (f32)x - 108.0f + (100.0f - countWidth) * 0.5f;
    countPos.y = (f32)y - 184.0f;
    winFontSet(&countPos, &countScale, &white, countText);
    scale.x = 0.75f; scale.y = 0.75f; scale.z = 1.0f; pos.z = 0.0f;
    winFontInit();
    for (i = 0; i < 16; i++) {
        idx = page * 16 + i;
        if (idx >= count) break;
        unit = *(u8*)((s32)pWin + 0x1058 + idx * 2 + 1);
        if (swGet(unit + 0x117A) == 0) {
            name = "---";
        } else if (unit == 0x4D || unit == 0x87) {
            if (evtGetValue(0, -170000000) < 0xD2) {
                name = msgSearch(str_btl_un_hatena_802f6b28);
            } else {
                name = msgSearch(str_btl_un_rampell_802f6b38);
            }
        } else {
            name = msgSearch(*(char**)battleGetUnitMonosiriPtr(unit));
        }
        sprintf(numberText, "%d", enemy_monoshiri_sort_table[unit]);
        pos.x = (f32)x + 19.0f + (i >> 3) * 216.0f;
        pos.y = (f32)y - 16.0f - (i & 7) * 24.0f;
        winFontSetWidth(&pos, &scale, &white, 40.0f, numberText);
        pos.x = (f32)x + 56.0f + (i >> 3) * 216.0f;
        winFontSetWidth(&pos, &scale, &white, 145.0f, name);
    }
    winIconInit();
    if (page > 0) {
        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
        pos.x = (f32)x + 440.0f;
        pos.y = (f32)y + 18.0f;
        winTexSet(0x17, &pos, &scale, &white);
        winIconInit();
        pos.y = (f32)y;
        winIconSet(0x86, &pos, &scale, &white);
    }
    if ((page + 1) * 16 < count) {
        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
        pos.x = (f32)x + 440.0f;
        pos.y = (f32)y - 255.0f;
        winTexSet(0x17, &pos, &scale, &white);
        winIconInit();
        pos.y = (f32)y - 240.0f;
        winIconSet(0x88, &pos, &scale, &white);
    }
    pos.x = (f32)x + 390.0f;
    pos.y = (f32)y - 210.0f;
    scale.x = 0.7f;
    scale.y = 0.7f;
    winFontInit();
    winFontSet(&pos, &scale, &white,
               msgSearch(str_msg_menu_sort_narabi_802f6be0));
    pos.x -= 30.0f;
    pos.y -= 10.0f;
    winIconInit();
    winIconSet(0x219, &pos, &scale, &white);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
MonosiriWork* monosiriInit(s32 param, f32 x, f32 y) {
    MonosiriWork* work = _mapAlloc(mapalloc_base_ptr, sizeof(MonosiriWork));
    f32 closeX;
    f32 closeY;

    memset(work, 0, sizeof(MonosiriWork));
    closeX = float_500_80423b40;
    closeY = float_neg300_80423b44;
    work->state = 0;
    work->param = param;
    work->x = x;
    work->y = y;
    work->closeX = closeX;
    work->closeY = closeY;
    work->flags = 0;
    work->pose0 = -1;
    work->pose1 = -1;
    return work;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 monosiriMain(void* param_1) {
    extern char str_jp_80423acc[];
    extern char str_us_80423ad0[];
    extern char str_ge_80423ad4[];
    extern char str_fr_80423ad8[];
    extern char str_sp_80423adc[];
    extern char str_it_80423ae0[];
    extern char str_S_1_80423b48[];
    extern char str_c_n_moa_a_802f6bac[];
    extern char str_PCTs_w_PCTs_win_tpl_802f6b88[];
    extern char str_PCTs_w_PCTs_mono_dat_802f6b98[];
    extern f32 float_0_80423aec;
    extern f32 float_600_80423b4c;
    extern f32 float_neg400_80423b50;
    extern f32 float_6_80423b54;
    extern s32 getMarioStDvdRoot(void);
    extern s32 fileAsyncf(s32 kind, s32 unused, char* fmt, s32 root, char* lang, ...);
    extern void* fileAllocf(s32 kind, char* fmt, s32 root, char* lang, ...);
    extern s32 GXGetTexBufferSize(s32 width, s32 height, s32 format, s32 mipmap, s32 maxLod);
    extern void* smartAlloc(s32 size, s32 heap);
    extern s32 animGroupBaseAsync(char* name, s32 battle, s32 unused);
    extern s32 animPoseEntry(char* name, s32 battle);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 loop);
    extern void animPoseWorldPositionEvalOn(s32 poseId, s32 flag);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void capture(void);
    extern void monosiri_disp(s32 param_1, void* monosiriWork);

    void* work = param_1;
    char* langs[6];
    s32 inBattle;
    s32 state;
    f32 targetY;
    f32 denom;
    void* monoData;

    targetY = float_neg400_80423b50;
    langs[0] = str_jp_80423acc;
    langs[1] = str_us_80423ad0;
    langs[2] = str_ge_80423ad4;
    langs[3] = str_fr_80423ad8;
    langs[4] = str_sp_80423adc;
    langs[5] = str_it_80423ae0;
    inBattle = *(s32*)((s32)gp + 0x14) != 0;

    if (work != NULL) {
        state = *(s32*)work;
        if (state == 0) {
            s32 dvdRoot;
            char* lang;
            u32 texObj[8];
            s32 height;
            s32 width;
            s32 size;

            dvdRoot = getMarioStDvdRoot();
            if (fileAsyncf(4, 0, str_PCTs_w_PCTs_win_tpl_802f6b88, dvdRoot,
                           langs[*(s32*)((s32)gp + 0x16C)]) != 0) {
                lang = str_us_80423ad0;
                if (*(s32*)((s32)gp + 0x16C) == 0) {
                    lang = str_jp_80423acc;
                }
                dvdRoot = getMarioStDvdRoot();
                if (fileAsyncf(0, 0, str_PCTs_w_PCTs_mono_dat_802f6b98, dvdRoot, lang) != 0) {
                    dvdRoot = getMarioStDvdRoot();
                    *(void**)((s32)work + 8) =
                        fileAllocf(4, str_PCTs_w_PCTs_win_tpl_802f6b88, dvdRoot,
                                   langs[*(s32*)((s32)gp + 0x16C)]);
                    lang = str_us_80423ad0;
                    if (*(s32*)((s32)gp + 0x16C) == 0) {
                        lang = str_jp_80423acc;
                    }
                    dvdRoot = getMarioStDvdRoot();
                    *(void**)((s32)work + 0xC) =
                        fileAllocf(0, str_PCTs_w_PCTs_mono_dat_802f6b98, dvdRoot, lang);
                    *(s32*)work = *(s32*)work + 1;
                    TEXGetGXTexObjFromPalette(*(void**)(*(s32*)(*(s32*)((s32)work + 8) + 0xA0)),
                                              texObj, 0x37);
                    height = GXGetTexObjHeight(texObj);
                    width = GXGetTexObjWidth(texObj);
                    size = GXGetTexBufferSize(width, height, 5, 0, 0);
                    *(void**)((s32)work + 0x28) = smartAlloc(size, 0);
                }
            }
        } else if (state == 1) {
            monoData = battleGetUnitMonosiriPtr(*(s32*)((s32)work + 4));
            if (animGroupBaseAsync(*(char**)((s32)monoData + 0xC), inBattle, 0) != 0 &&
                (*(s32*)((s32)work + 4) != 0x6B ||
                 animGroupBaseAsync(str_c_n_moa_a_802f6bac, inBattle, 0) != 0)) {
                *(s32*)((s32)work + 0x2C) += 0x14;
                if (*(s32*)((s32)work + 0x2C) > 0xFF) {
                    *(s32*)((s32)work + 0x2C) = 0xFF;
                }
                if (*(s32*)((s32)work + 0x20) == -1) {
                    *(s32*)((s32)work + 0x20) =
                        animPoseEntry(*(char**)((s32)monoData + 0xC), inBattle);
                    animPoseSetAnim(*(s32*)((s32)work + 0x20),
                                    *(char**)((s32)monoData + 0x10), 1);
                    if (*(s32*)((s32)work + 4) == 0x6B) {
                        animPoseWorldPositionEvalOn(*(s32*)((s32)work + 0x20), 0);
                        animPoseMain(*(s32*)((s32)work + 0x20));
                        *(s32*)((s32)work + 0x24) =
                            animPoseEntry(str_c_n_moa_a_802f6bac, inBattle);
                        animPoseSetAnim(*(s32*)((s32)work + 0x24), str_S_1_80423b48, 1);
                        animPoseMain(*(s32*)((s32)work + 0x24));
                    } else {
                        animPoseMain(*(s32*)((s32)work + 0x20));
                    }
                    dispEntry(7, 6, capture, work, float_0_80423aec);
                }
            }
        } else if (state < 3) {
            *(f32*)((s32)work + 0x18) = float_600_80423b4c;
            *(f32*)((s32)work + 0x1C) = targetY;
        }

        denom = (float_6_80423b54 * (f32)*(s32*)((s32)gp + 4)) / float_60_80423b58;
        *(f32*)((s32)work + 0x10) +=
            (*(f32*)((s32)work + 0x18) - *(f32*)((s32)work + 0x10)) / denom;
        *(f32*)((s32)work + 0x14) +=
            (*(f32*)((s32)work + 0x1C) - *(f32*)((s32)work + 0x14)) / denom;
        if (*(s32*)work > 0) {
            dispEntry(8, 0, monosiri_disp, work, float_0_80423aec);
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void monosiriClose(void* work) {
    if (work != NULL) {
        *(s32*)work = 2;
    }
}

s32 monosiriCloseWait(void* work) {
    if (work == NULL) {
        return 1;
    }
    if (*(f32*)((s32)work + 0x10) > float_500_80423b40 &&
        *(f32*)((s32)work + 0x14) < float_neg300_80423b44) {
        return 1;
    }
    return 0;
}


void monosiriExit(void* work) {
    if (work != NULL) {
        if (*(void**)((s32)work + 0x8) != NULL) {
            fileFree(*(void**)((s32)work + 0x8));
            *(void**)((s32)work + 0x8) = NULL;
        }
        if (*(void**)((s32)work + 0xC) != NULL) {
            fileFree(*(void**)((s32)work + 0xC));
            *(void**)((s32)work + 0xC) = NULL;
        }
        if (*(s32*)((s32)work + 0x20) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x20));
            *(s32*)((s32)work + 0x20) = -1;
        }
        if (*(s32*)((s32)work + 0x24) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x24));
            *(s32*)((s32)work + 0x24) = -1;
        }
        if (*(void**)((s32)work + 0x28) != NULL) {
            smartFree(*(void**)((s32)work + 0x28));
            *(void**)((s32)work + 0x28) = NULL;
        }
        _mapFree(mapalloc_base_ptr, work);
    }
}

void capture(s32 cameraId, void* work) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void GXGetViewportv(f32*);
    extern void GXGetProjectionv(f32*);
    extern void GXGetScissor(s32*, s32*, s32*, s32*);
    extern void GXSetViewport(f32, f32, f32, f32, f32, f32);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern void C_MTXPerspective(void*, f32, f32, f32, f32);
    extern void C_MTXLookAt(void*, Vec3*, Vec3*, Vec3*);
    extern f64 tan(f64);
    extern void GXSetProjection(void*, s32);
    extern void GXSetProjectionv(f32*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void* camGetPtr(s32);
    extern void* camGetCurPtr(void);
    extern void PSMTXCopy(void*, void*);
    extern s32 animPoseGetVivianType(s32);
    extern void animPoseVivianMain(s32, Vec3*);
    extern void animPoseSetMaterialFlagOn(s32, u32);
    extern void animPosePeraOff(s32);
    extern void PSMTXIdentity(void*);
    extern void animPoseDrawMtx(s32, void*, s32, f32, f32);
    extern void animPosePeraOn(s32);
    extern void* animPoseGetAnimPosePtr(s32);
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void GXReadBoundingBox(u16*, u16*, u16*, u16*);
    extern void GXSetTexCopySrc(s32, s32, s32, s32);
    extern void GXSetTexCopyDst(s16, s16, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXCopyTex(void*, s32);
    extern void GXPixModeSync(void);

    f32 viewport[6];
    f32 projection[7];
    f32 perspective[4][4];
    f32 view[3][4];
    f32 model[3][4];
    f32 scaleMtx[3][4];
    f32 drawMtx[3][4];
    u32 savedCamera[152];
    u32 texObj[8];
    Vec3 center, up, camera, vivianOffset;
    s32 sx, sy, sw, sh;
    s32 width, height;
    s32 poseId = *(s32*)((s32)work + 0x20);
    u8* animData = animPoseGetAnimDataPtr(poseId);
    f32 minX = *(f32*)(animData + 0x44);
    f32 minY = *(f32*)(animData + 0x48);
    f32 maxX = *(f32*)(animData + 0x50);
    f32 maxY = *(f32*)(animData + 0x54);
    f32 centerX = (minX + maxX) * 0.5f;
    f32 centerY = (minY + maxY) * 0.5f;
    s32 i;
    u16 left, right, top, bottom;
    void* file = *(void**)((s32)work + 8);
    s32* table;
    s32 count;
    f32 tx = 0.0f;
    f32 ty = 0.0f;
    f32 scale = 1.0f;
    void* activeCamera;

    GXGetViewportv(viewport);
    GXGetProjectionv(projection);
    GXGetScissor(&sx, &sy, &sw, &sh);
    TEXGetGXTexObjFromPalette(**(void***)((s32)file + 0xA0), texObj, 0x37);
    width = GXGetTexObjWidth(texObj);
    height = GXGetTexObjHeight(texObj);
    GXSetViewport(0.0f, 0.0f, (f32)width, (f32)height, 0.0f, 1.0f);
    GXSetScissor(1, 1, width - 2, height - 2);
    C_MTXPerspective(perspective, 25.0f, 1.2667f, 1.0f, 1000.0f);
    GXSetProjection(perspective, 0);

    center.x = centerX; center.y = centerY; center.z = 0.0f;
    up.x = 0.0f; up.y = 1.0f; up.z = 0.0f;
    camera.x = centerX; camera.y = centerY;
    if ((maxX - minX) / (f32)width <= (maxY - minY) / (f32)height) {
        camera.z = ((608.0f * (maxY - centerY)) / 480.0f) /
                   (f32)tan(0.2181661564992912);
    } else {
        camera.z = (maxX - centerX) / (f32)tan(0.2181661564992912);
    }
    C_MTXLookAt(view, &camera, &up, &center);

    table = **(s32***)((s32)*(void**)((s32)work + 0xC) + 0xA0);
    count = *(s32*)(*(s32*)((s32)work + 0xC) + 0xA4) >> 4;
    for (i = 0; i < count; i++, table += 4) {
        if (table[0] == *(s32*)((s32)work + 4)) {
            tx = (f32)table[1]; ty = (f32)table[2]; scale = (f32)table[3];
            break;
        }
    }
    PSMTXTrans(model, tx, ty, 0.0f);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(model, scaleMtx, model);
    PSMTXConcat(view, model, view);

    sysWaitDrawSync();
    GXClearBoundingBox();
    activeCamera = camGetPtr(cameraId);
    for (i = 0; i < 152; i++) savedCamera[i] = *(u32*)((s32)activeCamera + i * 4);
    activeCamera = camGetCurPtr();
    PSMTXCopy(view, (void*)((s32)activeCamera + 0x18));
    if (animPoseGetVivianType(poseId) != 0) {
        vivianOffset.x = 0.0f; vivianOffset.y = 0.0f; vivianOffset.z = 0.0f;
        animPoseVivianMain(poseId, &vivianOffset);
    }
    animPoseSetMaterialFlagOn(poseId, 0x1800);
    animPosePeraOff(poseId);
    PSMTXIdentity(drawMtx);
    animPoseDrawMtx(poseId, drawMtx, 1, 0.0f, 1.0f);
    animPoseDrawMtx(poseId, drawMtx, 2, 0.0f, 1.0f);
    animPoseDrawMtx(poseId, drawMtx, 3, 0.0f, 1.0f);
    animPosePeraOn(poseId);

    if (*(s32*)((s32)work + 4) == 0x6B) {
        extern s32 dat_8041fb08;
        extern char* g_moamoa_chain_tbl[];
        extern void* smartAlloc(u32, s32);
        extern s32 animPoseGetGroupIdx(s32, char*);
        extern void PSVECScale(Vec3*, Vec3*, f32);
        extern void PSVECAdd(Vec3*, Vec3*, Vec3*);
        extern s32 irand(s32);
        void* pose = animPoseGetAnimPosePtr(poseId);
        Vec3* worldPos = **(Vec3***)((s32)pose + 0x15C);
        s32* chainCounts = *(s32**)smartAlloc(dat_8041fb08 * 4, 3);
        s32* chainIndices = *(s32**)smartAlloc(500 * 4, 3);
        s32 secondaryPose = *(s32*)((s32)work + 0x24);
        s32 chain;
        Vec3 first, second, blended;
        f32 ratio;

        memset(chainCounts, 0, dat_8041fb08 * 4);
        i = 0;
        count = 62;
        do {
            chain = i % dat_8041fb08;
            chainIndices[i] = chainCounts[chain]++;
            chain = (i + 1) % dat_8041fb08;
            chainIndices[i + 1] = chainCounts[chain]++;
            chain = (i + 2) % dat_8041fb08;
            chainIndices[i + 2] = chainCounts[chain]++;
            chain = (i + 3) % dat_8041fb08;
            chainIndices[i + 3] = chainCounts[chain]++;
            chain = (i + 4) % dat_8041fb08;
            chainIndices[i + 4] = chainCounts[chain]++;
            chain = (i + 5) % dat_8041fb08;
            chainIndices[i + 5] = chainCounts[chain]++;
            chain = (i + 6) % dat_8041fb08;
            chainIndices[i + 6] = chainCounts[chain]++;
            chain = (i + 7) % dat_8041fb08;
            chainIndices[i + 7] = chainCounts[chain]++;
            i += 8;
            count--;
        } while (count != 0);
        while (i < 500) {
            chain = i % dat_8041fb08;
            chainIndices[i] = chainCounts[chain]++;
            i++;
        }
        animPoseSetMaterialFlagOn(secondaryPose, 0x1800);
        animPosePeraOff(secondaryPose);
        for (i = 0; i < 500; i++) {
            chain = i % dat_8041fb08;
            ratio = (f32)chainIndices[i] / (f32)chainCounts[chain];
            PSVECScale(&worldPos[animPoseGetGroupIdx(poseId, g_moamoa_chain_tbl[chain * 2])],
                       &first, ratio);
            PSVECScale(&worldPos[animPoseGetGroupIdx(poseId, g_moamoa_chain_tbl[chain * 2 + 1])],
                       &second, 1.0f - ratio);
            PSVECAdd(&first, &second, &blended);
            PSMTXTrans(drawMtx, blended.x + (f32)(5 - irand(10)),
                       blended.y + (f32)(5 - irand(10)),
                       blended.z + (f32)(5 - irand(10)));
            animPoseDrawMtx(secondaryPose, drawMtx, 1, 0.0f, 1.0f);
            animPoseDrawMtx(secondaryPose, drawMtx, 2, 0.0f, 1.0f);
            animPoseDrawMtx(secondaryPose, drawMtx, 3, 0.0f, 1.0f);
        }
        animPosePeraOn(secondaryPose);
    }

    activeCamera = camGetCurPtr();
    PSMTXCopy(savedCamera, (void*)((s32)activeCamera + 0x18));
    sysWaitDrawSync();
    GXReadBoundingBox(&right, &left, &bottom, &top);
    GXSetTexCopySrc(0, 0, width, height);
    GXSetTexCopyDst((s16)width, (s16)height, 5, 0);
    GXSetZMode(1, 3, 1);
    GXCopyTex(**(void***)((s32)work + 0x28), 1);
    GXPixModeSync();
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    GXSetProjectionv(projection);
    GXSetScissor(sx, sy, sw, sh);
}

void monosiri_disp(s32 param_1,void* work){
    extern void winHalfBookGX(f32, f32, void*, s32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXInitTexObj(void*, void*, s16, s16, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevKAlphaSel(s32, s32);
    extern void GXBegin(s32, s32, s32);
    extern volatile f32 DAT_cc008000;
    extern u32 dat_80423ae8;
    extern u32 unk_804295f8;
    u32 texObj[8],tev=dat_80423ae8,alpha;s32 i,w,h;void* file=*(void**)((s32)work+8);f32 x=*(f32*)((s32)work+0x18),y=*(f32*)((s32)work+0x1C);
    if(file==0)return;winHalfBookGX(x,y,**(void***)((s32)file+0xA0),0);TEXGetGXTexObjFromPalette(**(void***)((s32)file+0xA0),texObj,0x37);w=GXGetTexObjWidth(texObj);h=GXGetTexObjHeight(texObj);GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0);GXLoadTexObj(texObj,0);GXInitTexObj(texObj,**(void***)((s32)work+0x28),(s16)w,(s16)h,5,0,0,0);GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0);GXLoadTexObj(texObj,1);GXSetNumTexGens(8);GXSetTevColor(1,&tev);alpha=(unk_804295f8&0xFFFFFF00)|*(u8*)((s32)work+0x2C);GXSetTevColor(2,&alpha);GXSetNumTevStages(12);
    for(i=0;i<8;i++){GXSetTevOrder(i,i,1,0xFF);GXSetTevColorOp(i,0,0,0,1,0);GXSetTevAlphaOp(i,0,0,0,1,0);GXSetTevColorIn(i,15,15,15,2);GXSetTevAlphaIn(i,7,4,6,i?0:7);GXSetTevKAlphaSel(i,7);}
    GXSetTevOrder(8,7,1,0xFF);GXSetTevColorOp(8,0,0,0,1,0);GXSetTevAlphaOp(8,0,0,0,1,0);GXSetTevColorIn(8,0,8,9,15);GXSetTevAlphaIn(8,0,7,4,4);GXSetTevOrder(9,0xFF,0xFF,0xFF);GXSetTevColorOp(9,0,0,0,1,0);GXSetTevAlphaOp(9,0,0,0,1,0);GXSetTevColorIn(9,15,15,15,0);GXSetTevAlphaIn(9,7,0,6,7);GXSetTevOrder(10,7,0,0xFF);GXSetTevColorOp(10,0,0,0,1,0);GXSetTevAlphaOp(10,0,0,0,1,0);GXSetTevColorIn(10,15,8,0,15);GXSetTevAlphaIn(10,7,7,7,0);GXSetTevOrder(11,7,0,0xFF);GXSetTevColorOp(11,0,0,0,1,0);GXSetTevAlphaOp(11,0,0,0,1,0);GXSetTevColorIn(11,8,0,1,15);GXSetTevAlphaIn(11,7,7,7,4);
    GXBegin(0x80,0,4);DAT_cc008000=x+32.0f;DAT_cc008000=y-8.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=x+32.0f;DAT_cc008000=y-168.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=x+248.0f;DAT_cc008000=y-168.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=1.0f;DAT_cc008000=x+248.0f;DAT_cc008000=y-8.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=0.0f;
}
