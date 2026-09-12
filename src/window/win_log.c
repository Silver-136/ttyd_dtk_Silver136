#include "window/win_log.h"

extern void* mapalloc_base_ptr;
extern const char str_msg_menu_kiroku_map_802f6708[];
extern const f64 double_to_int_802f6b70;
extern f32 float_neg128_80423c34;
extern f32 float_500_80423b40;
extern f32 float_neg300_80423b44;
extern f32 float_45_80423bdc;
extern f32 float_1_80423af0;
extern f32 float_0p04_80423af4;
extern f32 float_0p5_80423af8;
extern f32 float_neg0p5_80423afc;
extern f32 float_8_80423b04;
extern f32 float_320_80423c0c;
extern f32 float_neg240_80423bf0;
extern f32 float_neg200_80423c30;
extern f32 float_60_80423b58;
const f32 float_100_80423b60 = 100.0f;
extern s16 tubuDt[];
extern void* gp;

f32 chapterPos[14] = {
    165.0f, 68.0f, 0.0f, 75.0f, 97.0f, 150.0f, -139.0f,
    -33.0f, 41.0f, -163.0f, 211.0f, -91.0f, -209.0f, 159.0f
};

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
extern const char str_msg_menu_kiroku_ston_802f671c[];
extern const char str_msg_menu_kiroku_badg_802f6734[];
extern const char str_msg_menu_kiroku_ryor_802f674c[];
extern const char str_msg_menu_kiroku_mono_802f6764[];

const char* msg_help[] = {
    str_msg_menu_kiroku_map_802f6708,
    str_msg_menu_kiroku_ston_802f671c,
    str_msg_menu_kiroku_badg_802f6734,
    str_msg_menu_kiroku_ryor_802f674c,
    str_msg_menu_kiroku_mono_802f6764,
};

void winLogInit(void* pWin) {
    extern s32 pouchCheckItem(s32 item);
    extern void qqsort(void* base, s32 count, s32 size, void* compare);
    extern s32 compare_func5_2(void* a, void* b);
    extern s32 compare_func6_2(void* a, void* b);
    extern void* effStarStoneEntry(f32 x, f32 y, f32 z, f32 scale, s32 type);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 strncmp(const char* a, const char* b, u32 n);
    extern s32 evtGetValue(void* evt, s32 value);
    extern s32 stone_tbl[7];
    extern u8 itemDataTable[];
    extern u8 enemy_monoshiri_sort_table[];

    extern f32 float_0_80423aec;
    extern f32 float_neg1000_80423c9c;
    extern f32 float_2_80423bd8;
    extern f32 float_neg480_80423bf8;
    extern f32 float_304_80423bd0;
    extern f32 float_240_80423bb4;
    extern f32 float_230_80423bd4;
    extern f32 float_120_80423bcc;
    extern f32 float_neg148_80423bf4;
    extern f32 float_51p429_80423c04;

    extern char str_sin_80423a90[4];
    extern char str_hou_80423a6c[4];
    extern char str_gor_80423a50[4];
    extern char str_eki_80423a88[4];
    extern char str_pik_80423a8c[4];
    extern char str_tik_80423a54[4];
    extern char str_jin_80423a7c[4];
    extern char str_dou_80423a84[4];
    extern char str_rsh_80423ca0[4];
    extern char str_hom_80423ca4[4];
    extern char str_jon_80423ca8[4];

    u8* entry;
    u8* eff;
    u8* effWork;
    u8* item;
    u8* tattle;
    s16* area;
    s32* stone;
    u8* starSlot;
    u8* sortPtr;
    u8* sortOuter;
    u8* sortInner;
    char* areaName;

    const char* catalog = str_msg_menu_kiroku_map_802f6708;
    s32 i;
    s32 j;
    s32 count;
    s32 unit;
    s32 chapter;
    s32 itemId;
    s32 tmpi;
    s16 sx;
    s16 sy;
    u16 tmp16;
    f32 pos;
    f32 fZero;
    f32 fStep;
    f32 fHalf;
    f64 dMagic;
    union {
        f64 d;
        struct {
            u32 hi;
            u32 lo;
        } w;
    } cvtX, cvtY;

    /* Log submenu state and the always-present map entry. */
    fStep = float_45_80423bdc;
    fHalf = float_0p5_80423af8;
    fZero = float_0_80423aec;
    dMagic = *(const f64*)(catalog + 0x468);

    *(s32*)((u8*)pWin + 0xD68) = 0;
    *(s32*)((u8*)pWin + 0xD6C) = 0;
    *(s32*)((u8*)pWin + 0xD70) = 0;

    entry = (u8*)pWin +
            *(s32*)((u8*)pWin + 0xD70) * 0x20 + 0xD74;
    *(s32*)(entry + 0x00) = 0;
    *(s32*)(entry + 0x18) = 0;
    *(s32*)(entry + 0x14) = 0;
    *(const char**)(entry + 0x1C) = msg_help[0];
    *(s32*)((u8*)pWin + 0xD70) =
        *(s32*)((u8*)pWin + 0xD70) + 1;

    entry = (u8*)pWin;
    for (i = 0; i < *(s32*)((u8*)pWin + 0xD70);
         i++, entry += 0x20) {
        *(f32*)(entry + 0xD78) = fZero;
        *(f32*)(entry + 0xD80) = fZero;
        cvtX.w.hi = 0x43300000;
        cvtX.w.lo =
            ((u32)*(s32*)((u8*)pWin + 0xD70)) ^ 0x80000000U;
        cvtY.w.hi = 0x43300000;
        cvtY.w.lo = ((u32)i) ^ 0x80000000U;
        pos = fStep * (f32)(cvtX.d - dMagic) * fHalf -
              fStep * (f32)(cvtY.d - dMagic);
        *(f32*)(entry + 0xD7C) = pos;
        *(f32*)(entry + 0xD84) = pos;
    }

    tmpi = swByteGet(0);
    fStep = float_45_80423bdc;
    fZero = float_0_80423aec;
    dMagic = *(const f64*)(catalog + 0x468);
    if (tmpi >= 0x13) {
        fHalf = float_0p5_80423af8;
        entry = (u8*)pWin +
                *(s32*)((u8*)pWin + 0xD70) * 0x20 + 0xD74;
        *(s32*)(entry + 0x00) = 1;
        *(s32*)(entry + 0x18) = 0;
        *(s32*)(entry + 0x14) = 0;
        *(const char**)(entry + 0x1C) = msg_help[1];
        *(s32*)((u8*)pWin + 0xD70) =
            *(s32*)((u8*)pWin + 0xD70) + 1;

        entry = (u8*)pWin;
        for (i = 0; i < *(s32*)((u8*)pWin + 0xD70);
             i++, entry += 0x20) {
            *(f32*)(entry + 0xD78) = fZero;
            *(f32*)(entry + 0xD80) = fZero;
            cvtX.w.hi = 0x43300000;
            cvtX.w.lo =
                ((u32)*(s32*)((u8*)pWin + 0xD70)) ^ 0x80000000U;
            cvtY.w.hi = 0x43300000;
            cvtY.w.lo = ((u32)i) ^ 0x80000000U;
            pos = fStep * (f32)(cvtX.d - dMagic) * fHalf -
                  fStep * (f32)(cvtY.d - dMagic);
            *(f32*)(entry + 0xD7C) = pos;
            *(f32*)(entry + 0xD84) = pos;
        }
    }

    tmpi = swByteGet(0);
    fStep = float_45_80423bdc;
    fZero = float_0_80423aec;
    dMagic = *(const f64*)(catalog + 0x468);
    if (tmpi >= 7) {
        fHalf = float_0p5_80423af8;
        entry = (u8*)pWin +
                *(s32*)((u8*)pWin + 0xD70) * 0x20 + 0xD74;
        *(s32*)(entry + 0x00) = 4;
        *(s32*)(entry + 0x18) = 0;
        *(s32*)(entry + 0x14) = 0;
        *(const char**)(entry + 0x1C) = msg_help[4];
        *(s32*)((u8*)pWin + 0xD70) =
            *(s32*)((u8*)pWin + 0xD70) + 1;

        entry = (u8*)pWin;
        for (i = 0; i < *(s32*)((u8*)pWin + 0xD70);
             i++, entry += 0x20) {
            *(f32*)(entry + 0xD78) = fZero;
            *(f32*)(entry + 0xD80) = fZero;
            cvtX.w.hi = 0x43300000;
            cvtX.w.lo =
                ((u32)*(s32*)((u8*)pWin + 0xD70)) ^ 0x80000000U;
            cvtY.w.hi = 0x43300000;
            cvtY.w.lo = ((u32)i) ^ 0x80000000U;
            pos = fStep * (f32)(cvtX.d - dMagic) * fHalf -
                  fStep * (f32)(cvtY.d - dMagic);
            *(f32*)(entry + 0xD7C) = pos;
            *(f32*)(entry + 0xD84) = pos;
        }
    }

    tmpi = swByteGet(0);
    fStep = float_45_80423bdc;
    fZero = float_0_80423aec;
    dMagic = *(const f64*)(catalog + 0x468);
    if (tmpi >= 0x14) {
        fHalf = float_0p5_80423af8;
        entry = (u8*)pWin +
                *(s32*)((u8*)pWin + 0xD70) * 0x20 + 0xD74;
        *(s32*)(entry + 0x00) = 2;
        *(s32*)(entry + 0x18) = 0;
        *(s32*)(entry + 0x14) = 0;
        *(const char**)(entry + 0x1C) = msg_help[2];
        *(s32*)((u8*)pWin + 0xD70) =
            *(s32*)((u8*)pWin + 0xD70) + 1;

        entry = (u8*)pWin;
        for (i = 0; i < *(s32*)((u8*)pWin + 0xD70);
             i++, entry += 0x20) {
            *(f32*)(entry + 0xD78) = fZero;
            *(f32*)(entry + 0xD80) = fZero;
            cvtX.w.hi = 0x43300000;
            cvtX.w.lo =
                ((u32)*(s32*)((u8*)pWin + 0xD70)) ^ 0x80000000U;
            cvtY.w.hi = 0x43300000;
            cvtY.w.lo = ((u32)i) ^ 0x80000000U;
            pos = fStep * (f32)(cvtX.d - dMagic) * fHalf -
                  fStep * (f32)(cvtY.d - dMagic);
            *(f32*)(entry + 0xD7C) = pos;
            *(f32*)(entry + 0xD84) = pos;
        }
    }

    tmpi = swGet(0x40);
    fStep = float_45_80423bdc;
    fZero = float_0_80423aec;
    dMagic = *(const f64*)(catalog + 0x468);
    if (tmpi != 0) {
        fHalf = float_0p5_80423af8;
        entry = (u8*)pWin +
                *(s32*)((u8*)pWin + 0xD70) * 0x20 + 0xD74;
        *(s32*)(entry + 0x00) = 3;
        *(s32*)(entry + 0x18) = 0;
        *(s32*)(entry + 0x14) = 0;
        *(const char**)(entry + 0x1C) = msg_help[3];
        *(s32*)((u8*)pWin + 0xD70) =
            *(s32*)((u8*)pWin + 0xD70) + 1;

        entry = (u8*)pWin;
        for (i = 0; i < *(s32*)((u8*)pWin + 0xD70);
             i++, entry += 0x20) {
            *(f32*)(entry + 0xD78) = fZero;
            *(f32*)(entry + 0xD80) = fZero;
            cvtX.w.hi = 0x43300000;
            cvtX.w.lo =
                ((u32)*(s32*)((u8*)pWin + 0xD70)) ^ 0x80000000U;
            cvtY.w.hi = 0x43300000;
            cvtY.w.lo = ((u32)i) ^ 0x80000000U;
            pos = fStep * (f32)(cvtX.d - dMagic) * fHalf -
                  45.0f * (f32)(cvtY.d - dMagic);
            *(f32*)(entry + 0xD7C) = pos;
            *(f32*)(entry + 0xD84) = pos;
        }
    }

    /* Crystal-star count and seven persistent star-stone effects. */
    *(s32*)((u8*)pWin + 0xE40) = 0;
    stone = stone_tbl;
    for (i = 0; i < 7; i++, stone++) {
        if (pouchCheckItem(*stone) != 0) {
            *(s32*)((u8*)pWin + 0xE40) += 1;
        }
    }

    starSlot = (u8*)pWin;
    for (i = 0; i < 7; i++, starSlot += 4) {
        if (i < *(s32*)((u8*)pWin + 0xE40)) {
            eff = (u8*)effStarStoneEntry(float_0_80423aec, float_neg1000_80423c9c,
                                         float_0_80423aec, float_2_80423bd8, i);
        } else {
            eff = (u8*)effStarStoneEntry(float_0_80423aec, float_neg1000_80423c9c,
                                         float_0_80423aec, float_2_80423bd8, i + 7);
            effWork = *(u8**)(eff + 0x0C);
            effWork[0x5C] = 0x20;
            effWork = *(u8**)(eff + 0x0C);
            effWork[0x5D] = 0x20;
            effWork = *(u8**)(eff + 0x0C);
            effWork[0x5E] = 0x20;
        }
        *(u8**)(starSlot + 0xE44) = eff;
        effWork = *(u8**)(eff + 0x0C);
        *(s32*)(effWork + 0x38) = 8;
        effWork = *(u8**)(eff + 0x0C);
        *(u16*)(effWork + 0x04) &= (u16)~4;
        effWork = *(u8**)(eff + 0x0C);
        *(u16*)(effWork + 0x04) |= 0x20;
    }

    /* Map-log initial state and the area corresponding to the current map. */
    *(void**)((u8*)pWin + 0x30) = 0;
    *(s32*)((u8*)pWin + 0xE1C) = -1;
    *(s32*)((u8*)pWin + 0xE14) = -1;
    *(s32*)((u8*)pWin + 0xE18) = 0;
    *(f32*)((u8*)pWin + 0xE24) = float_0_80423aec;
    *(f32*)((u8*)pWin + 0xE20) = float_0_80423aec;
    *(f32*)((u8*)pWin + 0xE2C) = float_0_80423aec;
    *(f32*)((u8*)pWin + 0xE28) = float_0_80423aec;
    *(f32*)((u8*)pWin + 0xE34) = float_neg480_80423bf8;
    *(f32*)((u8*)pWin + 0xE30) = float_neg480_80423bf8;
    *(f32*)((u8*)pWin + 0xE3C) = float_0_80423aec;
    *(f32*)((u8*)pWin + 0xE38) = float_0_80423aec;

    areaName = (char*)gp + 0x13C;
    if (strcmp((char*)gp + 0x12C, catalog + 0x600) == 0 ||
        strcmp((char*)gp + 0x12C, catalog + 0x608) == 0) {
        areaName = str_sin_80423a90;
    }
    if (strcmp((char*)gp + 0x12C, catalog + 0x610) == 0 ||
        strcmp((char*)gp + 0x12C, catalog + 0x618) == 0) {
        areaName = str_hou_80423a6c;
    }
    if (strncmp((char*)gp + 0x12C, str_rsh_80423ca0, 3) == 0) {
        if (swByteGet(0) < 0x136) {
            areaName = str_gor_80423a50;
        } else if (swByteGet(0) < 0x14E || swGet(0xDFB) != 0) {
            areaName = str_eki_80423a88;
        } else if (swGet(0xDFC) != 0) {
            areaName = str_pik_80423a8c;
        } else {
            tmpi = swByteGet(0x366);
            if (tmpi == 2) {
                areaName = str_pik_80423a8c;
            } else if (tmpi < 2) {
                if (tmpi == 0) {
                    areaName = str_gor_80423a50;
                } else if (tmpi > -1) {
                    areaName = str_eki_80423a88;
                }
            } else if (tmpi < 4) {
                areaName = str_gor_80423a50;
            }
        }
    }
    if (strncmp((char*)gp + 0x12C, str_hom_80423ca4, 3) == 0) {
        areaName = str_eki_80423a88;
    }
    if (strncmp((char*)gp + 0x12C, str_jon_80423ca8, 3) == 0) {
        areaName = str_tik_80423a54;
    }
    if (strcmp((char*)gp + 0x12C, catalog + 0x620) == 0) {
        areaName = str_jin_80423a7c;
    }
    if (strcmp((char*)gp + 0x12C, catalog + 0x628) == 0 ||
        strcmp((char*)gp + 0x12C, catalog + 0x630) == 0 ||
        strcmp((char*)gp + 0x12C, catalog + 0x638) == 0) {
        areaName = str_dou_80423a84;
    }

    area = (s16*)tubuDt;
    for (i = 0; i < 0x5D; i++, area += 6) {
        if (areaName == 0 || strcmp(areaName, *(char**)((u8*)area + 0x08)) == 0) {
            break;
        }
    }
    if (i < 0x5D) {
        *(s32*)((u8*)pWin + 0xE18) = i;
    }

    area = (s16*)tubuDt + *(s32*)((u8*)pWin + 0xE18) * 6;
    sx = area[0];
    sy = area[1];
    cvtX.w.hi = 0x43300000;
    cvtX.w.lo = ((u32)(s32)sx) ^ 0x80000000U;
    cvtY.w.hi = 0x43300000;
    cvtY.w.lo = ((u32)(s32)sy) ^ 0x80000000U;
    *(f32*)((u8*)pWin + 0xE20) =
        float_304_80423bd0 +
        (f32)(cvtX.d - *(const f64*)(catalog + 0x468));
    *(f32*)((u8*)pWin + 0xE24) =
        float_240_80423bb4 +
        (f32)(cvtY.d - *(const f64*)(catalog + 0x468));
    *(f32*)((u8*)pWin + 0xE38) -= (f32)(s32)((*(f32*)((u8*)pWin + 0xE20) + *(f32*)((u8*)pWin + 0xE38)) - float_230_80423bd4);
    *(f32*)((u8*)pWin + 0xE3C) -= (f32)(s32)((*(f32*)((u8*)pWin + 0xE24) + *(f32*)((u8*)pWin + 0xE3C)) - float_120_80423bcc);
    if (*(f32*)((u8*)pWin + 0xE38) > float_0_80423aec) {
        *(f32*)((u8*)pWin + 0xE38) = float_0_80423aec;
    }
    if (*(f32*)((u8*)pWin + 0xE3C) > float_0_80423aec) {
        *(f32*)((u8*)pWin + 0xE3C) = float_0_80423aec;
    }
    if (*(f32*)((u8*)pWin + 0xE38) < float_neg148_80423bf4) {
        *(f32*)((u8*)pWin + 0xE38) = float_neg148_80423bf4;
    }
    if (*(f32*)((u8*)pWin + 0xE3C) < float_neg240_80423bf0) {
        *(f32*)((u8*)pWin + 0xE3C) = float_neg240_80423bf0;
    }

    /* Crystal-star selector/window animation state. */
    *(s32*)((u8*)pWin + 0xE60) = 0;
    pos = float_51p429_80423c04 * (f32)*(s32*)((u8*)pWin + 0xE60);
    *(f32*)((u8*)pWin + 0xE7C) = pos;
    *(f32*)((u8*)pWin + 0xE80) = pos;
    *(f32*)((u8*)pWin + 0xE68) = float_0_80423aec;
    *(f32*)((u8*)pWin + 0xE64) = float_0_80423aec;
    *(f32*)((u8*)pWin + 0xE70) = float_neg480_80423bf8;
    *(f32*)((u8*)pWin + 0xE6C) = float_neg480_80423bf8;
    *(f32*)((u8*)pWin + 0xE78) = float_0_80423aec;
    *(f32*)((u8*)pWin + 0xE74) = float_0_80423aec;

    /* Badge log: include only real item-table rows and count obtained badges separately. */
    *(s32*)((u8*)pWin + 0xE94) = 0;
    *(s32*)((u8*)pWin + 0xE98) = 0;
    item = itemDataTable + 0xF0 * 0x28;
    for (itemId = 0xF0; itemId < 0x153; itemId++, item += 0x28) {
        if (*(s16*)(item + 0x12) != -1) {
            count = *(s32*)((u8*)pWin + 0xE94);
            *(u16*)((u8*)pWin + 0xEBC + count * 2) = (u16)(itemId - 0xF0);
            *(s32*)((u8*)pWin + 0xE94) = count + 1;
            if (swGet(itemId - 0x70) != 0) {
                *(s32*)((u8*)pWin + 0xE98) += 1;
            }
        }
    }
    qqsort((u8*)pWin + 0xEBC, *(s32*)((u8*)pWin + 0xE94), 2, compare_func5_2);
    *(s32*)((u8*)pWin + 0xE9C) = 0;
    fStep = float_320_80423c0c;
    fHalf = float_neg240_80423bf0;
    fZero = float_0_80423aec;
    pos = float_neg200_80423c30;
    *(f32*)((u8*)pWin + 0xE88) = fStep;
    *(f32*)((u8*)pWin + 0xE84) = fStep;
    *(f32*)((u8*)pWin + 0xE90) = fHalf;
    *(f32*)((u8*)pWin + 0xE8C) = fHalf;
    *(s32*)((u8*)pWin + 0xEA0) = 0;
    *(f32*)((u8*)pWin + 0xEA8) = fZero;
    *(f32*)((u8*)pWin + 0xEA4) = fZero;
    *(f32*)((u8*)pWin + 0xEB0) = pos;
    *(f32*)((u8*)pWin + 0xEAC) = pos;
    fStep = float_neg300_80423b44;
    *(f32*)((u8*)pWin + 0xEB8) = fStep;
    *(f32*)((u8*)pWin + 0xEB4) = fStep;

    /* Recipe log. */
    *(s32*)((u8*)pWin + 0xF94) = 0;
    *(s32*)((u8*)pWin + 0xF98) = 0;
    item = itemDataTable + 0xB3 * 0x28;
    for (itemId = 0xB3; itemId < 0xEC; itemId++, item += 0x28) {
        if (*(s16*)(item + 0x12) != -1) {
            count = *(s32*)((u8*)pWin + 0xF94);
            *(u16*)((u8*)pWin + 0xFBC + count * 2) = (u16)(itemId - 0xB3);
            *(s32*)((u8*)pWin + 0xF94) = count + 1;
            if (swGet(itemId - 0x72) != 0) {
                *(s32*)((u8*)pWin + 0xF98) += 1;
            }
        }
    }
    qqsort((u8*)pWin + 0xFBC, *(s32*)((u8*)pWin + 0xF94), 2, compare_func6_2);
    *(s32*)((u8*)pWin + 0xF9C) = 0;
    fStep = float_320_80423c0c;
    fHalf = float_neg240_80423bf0;
    fZero = float_0_80423aec;
    pos = float_neg200_80423c30;
    *(f32*)((u8*)pWin + 0xF88) = fStep;
    *(f32*)((u8*)pWin + 0xF84) = fStep;
    *(f32*)((u8*)pWin + 0xF90) = fHalf;
    *(f32*)((u8*)pWin + 0xF8C) = fHalf;
    *(s32*)((u8*)pWin + 0xFA0) = 0;
    *(f32*)((u8*)pWin + 0xFA8) = fZero;
    *(f32*)((u8*)pWin + 0xFA4) = fZero;
    *(f32*)((u8*)pWin + 0xFB0) = pos;
    *(f32*)((u8*)pWin + 0xFAC) = pos;
    fStep = float_neg300_80423b44;
    *(f32*)((u8*)pWin + 0xFB8) = fStep;
    *(f32*)((u8*)pWin + 0xFB4) = fStep;

    /* Tattle log list.  The first byte is target sort order, the second is unit id. */
    *(s32*)((u8*)pWin + 0x1040) = 0;
    sortPtr = enemy_monoshiri_sort_table + 1;
    for (unit = 1; unit < 0xD8; unit++, sortPtr++) {
        if (*(void**)((u8*)battleGetUnitMonosiriPtr(unit) + 0x08) == 0) {
            continue;
        }

        chapter = evtGetValue(0, -170000000);
        if (chapter < 0x187) {
            if ((u32)(unit - 0x85) <= 1 || unit == 0x87) {
                continue;
            }
        } else {
            if ((u32)(unit - 0x1F) <= 1 || unit == 0x4D) {
                continue;
            }
        }

        chapter = evtGetValue(0, -170000000);
        if (chapter < 0x190) {
            if (unit == 0x90) {
                continue;
            }
        } else if (unit == 0x3F) {
            continue;
        }

        *(u8*)((u8*)pWin + 0x1058 +
               *(s32*)((u8*)pWin + 0x1040) * 2) = *sortPtr;
        count = *(s32*)((u8*)pWin + 0x1040);
        *(s32*)((u8*)pWin + 0x1040) = count + 1;
        *(u8*)((u8*)pWin + 0x1059 + count * 2) = (u8)unit;
    }

    sortOuter = (u8*)pWin + 0x1058;
    for (i = 0; i < *(s32*)((u8*)pWin + 0x1040) - 1;
         i++, sortOuter += 2) {
        sortInner = sortOuter + 2;
        for (j = i + 1; j < *(s32*)((u8*)pWin + 0x1040);
             j++, sortInner += 2) {
            if (sortOuter[0] > sortInner[0]) {
                tmp16 = *(u16*)sortOuter;
                *(u16*)sortOuter = *(u16*)sortInner;
                *(u16*)sortInner = tmp16;
            }
        }
    }

    *(s32*)((u8*)pWin + 0x1048) = 0;
    fZero = float_0_80423aec;
    *(f32*)((u8*)pWin + 0x1034) = fZero;
    *(f32*)((u8*)pWin + 0x1030) = fZero;
    *(f32*)((u8*)pWin + 0x103C) = fZero;
    *(f32*)((u8*)pWin + 0x1038) = fZero;
    *(s32*)((u8*)pWin + 0x104C) = 0;
    *(f32*)((u8*)pWin + 0x1054) = fZero;
    *(f32*)((u8*)pWin + 0x1050) = fZero;
    *(s32*)((u8*)pWin + 0x1044) = 0;

    for (unit = 1; unit < 0xD8; unit++) {
        if (*(void**)((u8*)battleGetUnitMonosiriPtr(unit) + 0x08) != 0 &&
            (u32)(unit - 0x1F) > 1 && unit != 0x4D && unit != 0x3F &&
            swGet(unit + 0x117A) != 0) {
            *(s32*)((u8*)pWin + 0x1044) += 1;
        }
    }
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

char name[64];
char help[64];

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



s32 winLogMain(void* pWin) {
    typedef union WinLogI2D {
        f64 value;
        struct {
            u32 hi;
            u32 lo;
        } words;
    } WinLogI2D;

    extern void psndSFXOn(s32 id);
    extern void winMsgEntry(void* win, s32 item, char* msg, s32 type);
    extern void winSortEntry(f32 x, f32 y, void* win, s32 type);
    extern s32 winSortWait(void* win);
    extern u32 keyGetButton(s32 channel);
    extern u32 keyGetStickX(s32 channel);
    extern u32 keyGetStickY(s32 channel);
    extern f64 distABf(f64 ax, f64 ay, f64 bx, f64 by);
    extern s32 getMarioStDvdRoot(void);
    extern s32 fileAsyncf(s32 kind, s32 unused, char* fmt, s32 root, char* lang, ...);
    extern void* fileAllocf(s32 kind, char* fmt, s32 root, char* lang, ...);
    extern s32 marioGetColor(void);
    extern s32 animPoseEntry(char* name, s32 battle);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 loop);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern u8 itemDataTable[];
    extern s32 stone_tbl[];
    extern char str_us_80423ad0[];
    extern char str_jp_80423acc[];
    extern char str_W_1_80423c38[];
    extern char str_W_15_80423c3c[];
    extern char str_W_17_80423c44[];
    extern char str_W_18_80423c4c[];
    extern char str_M_W_1_80423c54[];

    extern f32 float_0_80423aec;
    extern f32 float_0p125_80423bec;
    extern f32 float_0p2_80423c6c;
    extern f32 float_0p5_80423af8;
    extern f32 float_10000_80423c5c;
    extern f32 float_120_80423bcc;
    extern f32 float_150_80423c7c;
    extern f32 float_166_80423b70;
    extern f32 float_16_80423b14;
    extern f32 float_20_80423c08;
    extern f32 float_230_80423bd4;
    extern f32 float_240_80423bb4;
    extern f32 float_24_80423b80;
    extern f32 float_255_80423c28;
    extern f32 float_304_80423bd0;
    extern f32 float_320_80423c0c;
    extern f32 float_330_80423c8c;
    extern f32 float_360_80423c84;
    extern f32 float_45_80423bdc;
    extern f32 float_468_80423c74;
    extern f32 float_500_80423b40;
    extern f32 float_50_80423b64;
    extern f32 float_51p429_80423c04;
    extern f32 float_604_80423c70;
    extern f32 float_60_80423b58;
    extern f32 float_74_80423bc8;
    extern f32 float_80_80423b68;
    extern f32 float_8_80423b04;
    extern f32 float_neg120_80423c64;
    extern f32 float_neg128_80423c34;
    extern f32 float_neg130_80423c2c;
    extern f32 float_neg148_80423bf4;
    extern f32 float_neg150_80423c78;
    extern f32 float_neg200_80423c30;
    extern f32 float_neg230_80423c60;
    extern f32 float_neg240_80423bf0;
    extern f32 float_neg250_80423c90;
    extern f32 float_neg300_80423b44;
    extern f32 float_neg310_80423c94;
    extern f32 float_neg480_80423bf8;
    extern f32 float_neg50_80423c88;
    extern f32 float_neg60_80423c98;
    extern f32 float_neg80_80423c80;
    extern f32 float_neg8_80423c68;

    u8* catalog = (u8*)str_msg_menu_kiroku_map_802f6708;
    s32 state = *(s32*)((u8*)pWin + 0xD68);
    s32 i;
    s32 j;
    s32 count;
    s32 cursor;
    s32 page;
    s32 pages;
    s32 oldCursor;
    s32 selected;
    s32 nearest;
    s32 areaX;
    s32 areaY;
    s32 tmpi;
    u32 pressed;
    u32 buttonsRep;
    u32 dpad;
    u32 flags;
    u32 progress;
    u32 kind;
    u32 unlocked;
    s16* area;
    u8* rawArea;
    char* lang;
    char* mapName;
    char* poseName;
    void* work;
    void* data;
    f32 moveX;
    f32 moveY;
    f32 dist;
    f32 bestDist;
    f32 targetX;
    f32 targetY;
    f32 currentRot;
    f32 targetRot;
    f32 ftmp;
    WinLogI2D cvt0;
    WinLogI2D cvt1;

#define W_S32(off) (*(s32*)((u8*)pWin + (off)))
#define W_U32(off) (*(u32*)((u8*)pWin + (off)))
#define W_F32(off) (*(f32*)((u8*)pWin + (off)))
#define W_PTR(off) (*(void**)((u8*)pWin + (off)))
#define W_U8(off) (*(u8*)((u8*)pWin + (off)))
#define W_S16(off) (*(s16*)((u8*)pWin + (off)))
#define SUB_TYPE(idx) W_S32(0xD74 + (idx) * 0x20)
#define SUB_STATE(idx) W_S32(0xD88 + (idx) * 0x20)
#define SUB_HELP(idx) (*(char**)((u8*)pWin + 0xD90 + (idx) * 0x20))
#define MAKE_FLOAT(dst, value_) \
    do { \
        cvt0.words.hi = 0x43300000; \
        cvt0.words.lo = ((u32)(s32)(value_)) ^ 0x80000000; \
        (dst) = (f32)(cvt0.value - double_to_int_802f6b70); \
    } while (0)
#define CLOSE_SUBMENU_STATES() \
    do { \
        for (i = 0; i < W_S32(0xD70); i++) { \
            SUB_STATE(i) = 40; \
        } \
        SUB_STATE(W_S32(0xD6C)) = 20; \
    } while (0)
#define FREE_MONOSIRI_WORK(ptr_) \
    do { \
        struct MonosiriWork* _mw = (struct MonosiriWork*)(ptr_); \
        if (_mw != NULL) { \
            if (_mw->file0 != NULL) { \
                fileFree(_mw->file0); \
                _mw->file0 = NULL; \
            } \
            if (_mw->file1 != NULL) { \
                fileFree(_mw->file1); \
                _mw->file1 = NULL; \
            } \
            if (_mw->pose0 != -1) { \
                animPoseRelease(_mw->pose0); \
                _mw->pose0 = -1; \
            } \
            if (_mw->pose1 != -1) { \
                animPoseRelease(_mw->pose1); \
                _mw->pose1 = -1; \
            } \
            if (_mw->buffer != NULL) { \
                smartFree(_mw->buffer); \
                _mw->buffer = NULL; \
            } \
            _mapFree(mapalloc_base_ptr, _mw); \
        } \
    } while (0)

    /*
     * Target dispatch begins with state 14, then the 0/10/11/12/13 family,
     * then tattle-detail states 21/22, and finally sort wait 1000/1001.
     */
        /*
     * A2: reproduce the target's initial decision tree and physical case order.
     * The target dispatches first, then lays out bodies as
     * 0, 10, 11, 12, 13, 14, 21, 22, 1000/1001.
     */
    if (state == 14) {
        goto winlog_state_14;
    }
    if (state >= 14) {
        if (state == 22) {
            goto winlog_state_22;
        }
        if (state >= 22) {
            if (state >= 1002) {
                goto winlog_end;
            }
            if (state >= 1000) {
                goto winlog_state_sort;
            }
            goto winlog_end;
        }
        if (state >= 21) {
            goto winlog_state_21;
        }
        goto winlog_end;
    }

    if (state == 10) {
        goto winlog_state_10;
    }
    if (state >= 10) {
        if (state == 12) {
            goto winlog_state_12;
        }
        if (state >= 12) {
            goto winlog_state_13;
        }
        goto winlog_state_11;
    }
    if (state == 0) {
        goto winlog_state_0;
    }
    goto winlog_end;

winlog_state_0:
    pressed = W_U32(0x4);
    oldCursor = W_S32(0xD6C);
    if ((pressed & 0x200) != 0) {
        psndSFXOn(0x20013);
        return -1;
    }
    if ((pressed & 0x1000) != 0) {
        return -2;
    }

    dpad = W_U32(0x10);
    if ((dpad & 0x2000) != 0) {
        W_S32(0xD6C)++;
        if (W_S32(0xD6C) >= W_S32(0xD70))
            W_S32(0xD6C) = 0;
        if (oldCursor != W_S32(0xD6C))
            psndSFXOn(0x20005);
    } else if ((dpad & 0x1000) != 0) {
        W_S32(0xD6C)--;
        if (W_S32(0xD6C) < 0)
            W_S32(0xD6C) = W_S32(0xD70) - 1;
        if (oldCursor != W_S32(0xD6C))
            psndSFXOn(0x20005);
    } else if ((pressed & 0x100) != 0) {
        count = W_S32(0xD70);
        for (i = 0; i < count; i++) {
            SUB_STATE(i) = 30;
        }
        SUB_STATE(W_S32(0xD6C)) = 10;
        state = SUB_TYPE(W_S32(0xD6C)) + 10;
        W_S32(0xD68) = state;

        selected = SUB_TYPE(W_S32(0xD6C));
        if (selected == 2) {
            W_F32(0xE88) = float_neg130_80423c2c;
            W_F32(0xE90) = float_120_80423bcc;
            W_F32(0xEB0) = float_neg200_80423c30;
            W_F32(0xEB8) = float_60_80423b58;
        } else if (selected < 2) {
            if (selected == 0) {
                W_F32(0xE30) = float_20_80423c08;
            } else if (selected > -1) {
                W_F32(0xE6C) = float_20_80423c08;
                W_F32(0xE74) = float_255_80423c28;
            }
        } else if (selected != 4 && selected < 4) {
            W_F32(0xF88) = float_neg130_80423c2c;
            W_F32(0xF90) = float_120_80423bcc;
            W_F32(0xFB0) = float_neg200_80423c30;
            W_F32(0xFB8) = float_60_80423b58;
        }
        psndSFXOn(0x20012);
    }

    W_F32(0x158) = float_neg128_80423c34;
    cvt0.words.hi = 0x43300000;
    cvt0.words.lo = ((u32)W_S32(0xD70)) ^ 0x80000000;
    targetX = (f32)(cvt0.value - double_to_int_802f6b70);
    cvt1.words.hi = 0x43300000;
    cvt1.words.lo = ((u32)W_S32(0xD6C)) ^ 0x80000000;
    targetY = (f32)(cvt1.value - double_to_int_802f6b70);
    W_F32(0x15C) =
        float_8_80423b04 +
        (float_45_80423bdc * targetX * float_0p5_80423af8 -
         float_45_80423bdc * targetY);
    winMsgEntry(pWin, 0, SUB_HELP(W_S32(0xD6C)), 0);

    goto winlog_end;

winlog_state_10:
    lang = str_us_80423ad0;
    if (*(u32*)((u8*)gp + 0x16C) == 0) {
        lang = str_jp_80423acc;
    }

    progress = swByteGet(0);
    if ((s32)progress < 0x13) {
        mapName = (char*)(catalog + 0x4F0);
    } else if ((s32)swByteGet(0) < 0x41) {
        mapName = (char*)(catalog + 0x4FC);
    } else if ((s32)swByteGet(0) < 0x79) {
        mapName = (char*)(catalog + 0x508);
    } else if ((s32)swByteGet(0) < 0xAF) {
        mapName = (char*)(catalog + 0x514);
    } else if ((s32)swByteGet(0) < 0xDE) {
        mapName = (char*)(catalog + 0x520);
    } else if ((s32)swByteGet(0) < 0x110) {
        mapName = (char*)(catalog + 0x52C);
    } else if ((s32)swByteGet(0) < 0x15C) {
        mapName = (char*)(catalog + 0x538);
    } else {
        mapName = (char*)(catalog + 0x544);
    }

    if (fileAsyncf(4, 0, (char*)(catalog + 0x550),
                   getMarioStDvdRoot(), lang, mapName) != 0) {
        if (W_PTR(0x30) == NULL) {
            lang = str_us_80423ad0;
            if (*(u32*)((u8*)gp + 0x16C) == 0) {
                lang = str_jp_80423acc;
            }

            progress = swByteGet(0);
            if ((s32)progress < 0x13) {
                mapName = (char*)(catalog + 0x4F0);
            } else if ((s32)swByteGet(0) < 0x41) {
                mapName = (char*)(catalog + 0x4FC);
            } else if ((s32)swByteGet(0) < 0x79) {
                mapName = (char*)(catalog + 0x508);
            } else if ((s32)swByteGet(0) < 0xAF) {
                mapName = (char*)(catalog + 0x514);
            } else if ((s32)swByteGet(0) < 0xDE) {
                mapName = (char*)(catalog + 0x520);
            } else if ((s32)swByteGet(0) < 0x110) {
                mapName = (char*)(catalog + 0x52C);
            } else if ((s32)swByteGet(0) < 0x15C) {
                mapName = (char*)(catalog + 0x538);
            } else {
                mapName = (char*)(catalog + 0x544);
            }

            W_PTR(0x30) = fileAllocf(4, (char*)(catalog + 0x550),
                                    getMarioStDvdRoot(), lang, mapName);
        }

        if (W_S32(0xE1C) == -1) {
            if ((*(u16*)pWin & 0x2000) != 0) {
                poseName = str_W_1_80423c38;
                W_S32(0xE1C) = animPoseEntry((char*)(catalog + 0x55C), 0);

                tmpi = marioGetColor();
                if (tmpi == 2) {
                    goto winlog_pose_w17;
                }
                if (tmpi >= 2) {
                    if (tmpi >= 4) {
                        goto winlog_pose_w_done;
                    }
                    goto winlog_pose_w18;
                }
                if (tmpi == 0) {
                    goto winlog_pose_w1;
                }
                if (tmpi >= 0) {
                    goto winlog_pose_w15;
                }
                goto winlog_pose_w_done;

winlog_pose_w1:
                poseName = str_W_1_80423c38;
                goto winlog_pose_w_done;
winlog_pose_w15:
                poseName = str_W_15_80423c3c;
                goto winlog_pose_w_done;
winlog_pose_w17:
                poseName = str_W_17_80423c44;
                goto winlog_pose_w_done;
winlog_pose_w18:
                poseName = str_W_18_80423c4c;
winlog_pose_w_done:
                animPoseSetAnim(W_S32(0xE1C), poseName, 1);
                goto winlog_pose_material;
            }

            poseName = (char*)(catalog + 0x564);
            tmpi = marioGetColor();
            if (tmpi == 2) {
                goto winlog_pose_mario_w;
            }
            if (tmpi >= 2) {
                if (tmpi >= 4) {
                    goto winlog_pose_mario_done;
                }
                goto winlog_pose_mario_wl;
            }
            if (tmpi == 0) {
                goto winlog_pose_mario_done;
            }
            if (tmpi >= 0) {
                goto winlog_pose_mario_l;
            }
            goto winlog_pose_mario_done;

winlog_pose_mario_l:
            poseName = (char*)(catalog + 0x56C);
            goto winlog_pose_mario_done;
winlog_pose_mario_w:
            poseName = (char*)(catalog + 0x578);
            goto winlog_pose_mario_done;
winlog_pose_mario_wl:
            poseName = (char*)(catalog + 0x584);
winlog_pose_mario_done:
            W_S32(0xE1C) = animPoseEntry(poseName, 0);
            animPoseSetAnim(W_S32(0xE1C), str_M_W_1_80423c54, 1);

winlog_pose_material:
            animPoseSetMaterialFlagOn(W_S32(0xE1C), 0x1800);
        }

        pressed = W_U32(0x4);
        if ((pressed & 0x200) != 0) {
            psndSFXOn(0x20013);

            data = pWin;
            for (i = 0; i < W_S32(0xD70); i++) {
                *(s32*)((u8*)data + 0xD88) = 40;
                data = (u8*)data + 0x20;
            }

            SUB_STATE(W_S32(0xD6C)) = 20;
            W_S32(0xD68) = 0;
            W_F32(0xE30) = float_neg480_80423bf8;

            fileFree(W_PTR(0x30));
            W_PTR(0x30) = NULL;

            if (W_S32(0xE1C) != -1) {
                animPoseRelease(W_S32(0xE1C));
            }
            W_S32(0xE1C) = -1;
        } else if ((pressed & 0x1000) != 0) {
            return -2;
        }

        area = tubuDt;
        nearest = 0;
        bestDist = float_10000_80423c5c;
        for (i = 0; i < 93; i++, area += 6) {
            rawArea = (u8*)area;
            progress = swByteGet(0);
            flags = 0;

            if ((s32)progress < (s32)(u16)area[3]) {
                kind = rawArea[5];
                if (kind == 2) {
                    if (swGet(0xFE) != 0) flags = 1;
                } else if (kind < 2) {
                    if (kind == 1 && swGet(0xFD) != 0) flags = 1;
                } else if (kind == 4) {
                    if (swGet(0x100) != 0) flags = 1;
                } else if (kind < 4) {
                    if (swGet(0xFF) != 0) flags = 1;
                }
                if (flags == 0) {
                    continue;
                }
            }

            if (rawArea[4] == 0) {
                continue;
            }

            MAKE_FLOAT(targetX, (s32)area[0]);
            MAKE_FLOAT(targetY, (s32)area[1]);
            dist = (f32)distABf(
                (f64)(float_74_80423bc8 + W_F32(0xE38) + targetX),
                (f64)(float_120_80423bcc + W_F32(0xE3C) + targetY),
                (f64)(float_neg230_80423c60 + W_F32(0xE38) + W_F32(0xE20)),
                (f64)(float_neg120_80423c64 + W_F32(0xE3C) + W_F32(0xE24)));

            if (dist < bestDist) {
                nearest = i;
                bestDist = dist;
            }
        }

        if (bestDist >= float_50_80423b64) {
            W_S32(0xE14) = -1;
            winMsgEntry(pWin, 0, (char*)catalog, 0);
        } else {
            rawArea = (u8*)&tubuDt[nearest * 6];

            sprintf(name, (char*)(catalog + 0x590),
                    *(s32*)(rawArea += 8));
            sprintf(help, (char*)(catalog + 0x5A4),
                    *(s32*)rawArea);

            if (W_S32(0xE14) != nearest) {
                psndSFXOn(0x20035);
            }
            W_S32(0xE14) = nearest;
            winMsgEntry(pWin, 0, help, (s32)name);
        }

        cvt0.words.hi = 0x43300000;
        cvt0.words.lo = ((u32)(s32)(s8)keyGetStickX(0)) ^ 0x80000000;
        moveX = (f32)(cvt0.value - double_to_int_802f6b70) *
                float_0p125_80423bec;

        cvt1.words.hi = 0x43300000;
        cvt1.words.lo = ((u32)(s32)(s8)keyGetStickY(0)) ^ 0x80000000;
        moveY = (f32)(cvt1.value - double_to_int_802f6b70) *
                float_0p125_80423bec;

        if (moveX == float_0_80423aec &&
            moveY == float_0_80423aec) {
            if ((keyGetButton(0) & 1) != 0) {
                moveX = -8.0f;
            } else if ((keyGetButton(0) & 2) != 0) {
                moveX = float_8_80423b04;
            }

            if ((keyGetButton(0) & 8) != 0) {
                moveY = float_8_80423b04;
            } else if ((keyGetButton(0) & 4) != 0) {
                moveY = -8.0f;
            }
        }

        if (moveX == float_0_80423aec &&
            moveY == float_0_80423aec &&
            W_S32(0xE14) != -1) {
            area = &tubuDt[W_S32(0xE14) * 6];

            cvt0.words.hi = 0x43300000;
            cvt0.words.lo = ((u32)(s32)area[0]) ^ 0x80000000;
            targetX = (f32)(cvt0.value - double_to_int_802f6b70);

            cvt1.words.hi = 0x43300000;
            cvt1.words.lo = ((u32)(s32)area[1]) ^ 0x80000000;
            targetY = (f32)(cvt1.value - double_to_int_802f6b70);

            moveX = ((304.0f + targetX) - W_F32(0xE20)) *
                    0.2f;
            moveY = ((float_240_80423bb4 + targetY) - W_F32(0xE24)) *
                    0.2f;
        }

        W_F32(0xE20) += moveX;
        W_F32(0xE24) += moveY;

        if (W_F32(0xE20) < float_16_80423b14)
            W_F32(0xE20) = float_16_80423b14;
        if (W_F32(0xE24) < float_8_80423b04)
            W_F32(0xE24) = float_8_80423b04;
        if (W_F32(0xE20) >= 604.0f)
            W_F32(0xE20) = 604.0f;
        if (W_F32(0xE24) >= 468.0f)
            W_F32(0xE24) = 468.0f;

        areaX = (s32)((W_F32(0xE20) + W_F32(0xE38)) - float_230_80423bd4);
        areaY = (s32)((W_F32(0xE24) + W_F32(0xE3C)) - float_120_80423bcc);

        MAKE_FLOAT(ftmp, areaX);
        if (float_166_80423b70 <= ftmp)
            W_F32(0xE38) -= ftmp - float_166_80423b70;

        MAKE_FLOAT(ftmp, areaX);
        if (ftmp <= float_neg150_80423c78)
            W_F32(0xE38) -= float_150_80423c7c + ftmp;

        MAKE_FLOAT(ftmp, areaY);
        if (float_80_80423b68 <= ftmp)
            W_F32(0xE3C) -= ftmp - float_80_80423b68;

        MAKE_FLOAT(ftmp, areaY);
        if (ftmp <= float_neg80_80423c80)
            W_F32(0xE3C) -= float_80_80423b68 + ftmp;

        if (float_0_80423aec < W_F32(0xE38))
            W_F32(0xE38) = float_0_80423aec;
        if (float_0_80423aec < W_F32(0xE3C))
            W_F32(0xE3C) = float_0_80423aec;
        if (W_F32(0xE38) < float_neg148_80423bf4)
            W_F32(0xE38) = float_neg148_80423bf4;
        if (W_F32(0xE3C) < float_neg240_80423bf0)
            W_F32(0xE3C) = float_neg240_80423bf0;

        W_F32(0x158) =
            float_neg230_80423c60 + W_F32(0xE38) + W_F32(0xE20);
        W_F32(0x15C) =
            float_neg120_80423c64 + W_F32(0xE3C) + W_F32(0xE24);
        W_F32(0x158) -= float_24_80423b80;
        W_F32(0x15C) += float_20_80423c08;
    }

    goto winlog_end;

winlog_state_11:
    pressed = W_U32(0x4);

    if ((pressed & 0x200) != 0) {
        psndSFXOn(0x20013);

        data = pWin;
        for (i = 0; i < W_S32(0xD70); i++) {
            *(s32*)((u8*)data + 0xD88) = 40;
            data = (u8*)data + 0x20;
        }
        SUB_STATE(W_S32(0xD6C)) = 20;
        W_S32(0xD68) = 0;
        W_F32(0xE6C) = float_neg480_80423bf8;
        W_F32(0xE74) = float_0_80423aec;
    } else {
        dpad = W_U32(0x10);

        if ((dpad & 0x4000) != 0) {
            W_S32(0xE60)--;
            while (W_S32(0xE60) < 0) {
                W_S32(0xE60) += 7;
            }

            MAKE_FLOAT(ftmp, W_S32(0xE60));
            W_F32(0xE7C) = float_51p429_80423c04 * ftmp;

            while (W_F32(0xE80) < W_F32(0xE7C)) {
                W_F32(0xE80) += float_360_80423c84;
            }

            psndSFXOn(0x209D6);
            psndSFXOn(0x20037);
        } else if ((dpad & 0x8000) != 0) {
            W_S32(0xE60)++;
            while (W_S32(0xE60) > 6) {
                W_S32(0xE60) -= 7;
            }

            MAKE_FLOAT(ftmp, W_S32(0xE60));
            W_F32(0xE7C) = float_51p429_80423c04 * ftmp;

            while (W_F32(0xE7C) < W_F32(0xE80)) {
                W_F32(0xE80) -= float_360_80423c84;
            }

            psndSFXOn(0x209D6);
            psndSFXOn(0x20037);
        } else if ((pressed & 0x1000) != 0) {
            return -2;
        }
    }

    W_F32(0x158) = float_neg50_80423c88;
    W_F32(0x15C) = float_neg50_80423c88;

    if (W_S32(0xE60) < W_S32(0xE40)) {
        tmpi = stone_tbl[W_S32(0xE60)] * 0x28;
        winMsgEntry(pWin, -1,
                    *(char**)(itemDataTable + tmpi + 8),
                    (s32)*(char**)(itemDataTable + tmpi + 4));
    } else {
        winMsgEntry(pWin, -1, (char*)(catalog + 0x5B0),
                    (s32)(catalog + 0x5CC));
    }

    goto winlog_end;

winlog_state_12:
    count = W_S32(0xE94);
    pages = (count + 27) / 28;
    pressed = W_U32(0x4);

    if ((pressed & 0x200) != 0) {
        psndSFXOn(0x20013);
        CLOSE_SUBMENU_STATES();
        W_S32(0xD68) = 0;
        W_F32(0xE88) = float_330_80423c8c;
        W_F32(0xE90) = float_neg250_80423c90;
        W_F32(0xEB0) = float_neg200_80423c30;
        W_F32(0xEB8) = float_neg300_80423b44;
    } else {
        buttonsRep = W_U32(0x8);
        if ((buttonsRep & 0x40) != 0) {
            W_S32(0xEA0)--;
            if (W_S32(0xEA0) < 0) {
                W_S32(0xEA0) = 0;
            }
            W_S32(0xE9C) = W_S32(0xEA0) * 28;
            psndSFXOn(0x20035);
        } else if ((buttonsRep & 0x20) != 0) {
            W_S32(0xEA0)++;
            if (W_S32(0xEA0) >= pages) {
                W_S32(0xEA0) = pages - 1;
            }
            W_S32(0xE9C) = W_S32(0xEA0) * 28;
            psndSFXOn(0x20035);
        } else {
            dpad = W_U32(0x10);
            if ((dpad & 0x8000) != 0) {
                cursor = W_S32(0xE9C);
                if ((cursor % 7) < 6 && cursor + 1 < count) {
                    W_S32(0xE9C) = cursor + 1;
                }
                psndSFXOn(0x20035);
            } else if ((dpad & 0x4000) != 0) {
                cursor = W_S32(0xE9C);
                if ((cursor % 7) > 0) {
                    W_S32(0xE9C) = cursor - 1;
                }
                psndSFXOn(0x20035);
            } else if ((dpad & 0x1000) != 0) {
                cursor = W_S32(0xE9C);
                tmpi = cursor - 7;
                if (tmpi >= 0 && tmpi < count) {
                    W_S32(0xE9C) = cursor - 7;
                }
                W_S32(0xEA0) = W_S32(0xE9C) / 28;
                psndSFXOn(0x20035);
            } else if ((dpad & 0x2000) != 0) {
                cursor = W_S32(0xE9C);
                tmpi = cursor + 7;
                if (tmpi >= 0 && tmpi < count) {
                    W_S32(0xE9C) = cursor + 7;
                } else {
                    j = cursor % 7;
                    if (cursor + (7 - j) < count) {
                        W_S32(0xE9C) = count - 1;
                    }
                }
                W_S32(0xEA0) = W_S32(0xE9C) / 28;
                psndSFXOn(0x20035);
            } else if ((pressed & 0x10) != 0) {
                winSortEntry(-310.0f, 150.0f, pWin, 4);
                W_S32(0x124) = W_S32(0xD68);
                psndSFXOn(0x20012);
                W_S32(0xD68) = 1000;
            } else if ((pressed & 0x1000) != 0) {
                return -2;
            }
        }
    }

    cursor = W_S32(0xE9C);
    tmpi = cursor % 28;
    MAKE_FLOAT(targetX, (tmpi % 7) * 56 - 140);
    W_F32(0x158) = targetX;
    MAKE_FLOAT(targetY, 90 - (tmpi / 7) * 56);
    W_F32(0x15C) = targetY;

    tmpi = (s32)W_S16(0xEBC + cursor * 2);
    if (swGet(tmpi + 0x80) != 0) {
        tmpi = (tmpi + 0xF0) * 0x28;
        winMsgEntry(pWin, -2,
                    *(char**)(itemDataTable + tmpi + 8),
                    (s32)*(char**)(itemDataTable + tmpi + 4));
    } else {
        winMsgEntry(pWin, -2, (char*)(catalog + 0x5B0),
                    (s32)(catalog + 0x5CC));
    }

    goto winlog_end;

winlog_state_13:
    count = W_S32(0xF94);
    pages = (count + 27) / 28;
    pressed = W_U32(0x4);

    if ((pressed & 0x200) != 0) {
        psndSFXOn(0x20013);
        CLOSE_SUBMENU_STATES();
        W_S32(0xD68) = 0;
        W_F32(0xF88) = float_330_80423c8c;
        W_F32(0xF90) = float_neg250_80423c90;
        W_F32(0xFB0) = float_neg200_80423c30;
        W_F32(0xFB8) = float_neg300_80423b44;
    } else {
        buttonsRep = W_U32(0x8);
        if ((buttonsRep & 0x40) != 0) {
            W_S32(0xFA0)--;
            if (W_S32(0xFA0) < 0) {
                W_S32(0xFA0) = 0;
            }
            W_S32(0xF9C) = W_S32(0xFA0) * 28;
            psndSFXOn(0x20035);
        } else if ((buttonsRep & 0x20) != 0) {
            W_S32(0xFA0)++;
            if (W_S32(0xFA0) >= pages) {
                W_S32(0xFA0) = pages - 1;
            }
            W_S32(0xF9C) = W_S32(0xFA0) * 28;
            psndSFXOn(0x20035);
        } else {
            dpad = W_U32(0x10);
            if ((dpad & 0x8000) != 0) {
                cursor = W_S32(0xF9C);
                if ((cursor % 7) < 6 && cursor + 1 < count) {
                    W_S32(0xF9C) = cursor + 1;
                }
                psndSFXOn(0x20035);
            } else if ((dpad & 0x4000) != 0) {
                cursor = W_S32(0xF9C);
                if ((cursor % 7) > 0) {
                    W_S32(0xF9C) = cursor - 1;
                }
                psndSFXOn(0x20035);
            } else if ((dpad & 0x1000) != 0) {
                cursor = W_S32(0xF9C);
                tmpi = cursor - 7;
                if (tmpi >= 0 && tmpi < count) {
                    W_S32(0xF9C) = cursor - 7;
                }
                W_S32(0xFA0) = W_S32(0xF9C) / 28;
                psndSFXOn(0x20035);
            } else if ((dpad & 0x2000) != 0) {
                cursor = W_S32(0xF9C);
                tmpi = cursor + 7;
                if (tmpi >= 0 && tmpi < count) {
                    W_S32(0xF9C) = cursor + 7;
                } else {
                    j = cursor % 7;
                    if (cursor + (7 - j) < count) {
                        W_S32(0xF9C) = count - 1;
                    }
                }
                W_S32(0xFA0) = W_S32(0xF9C) / 28;
                psndSFXOn(0x20035);
            } else if ((pressed & 0x10) != 0) {
                winSortEntry(float_neg310_80423c94,
                             float_150_80423c7c,
                             pWin, 5);
                W_S32(0x124) = W_S32(0xD68);
                psndSFXOn(0x20012);
                W_S32(0xD68) = 1000;
            } else if ((pressed & 0x1000) != 0) {
                return -2;
            }
        }
    }

    cursor = W_S32(0xF9C);
    tmpi = cursor % 28;
    MAKE_FLOAT(targetX, (tmpi % 7) * 56 - 140);
    W_F32(0x158) = targetX;
    MAKE_FLOAT(targetY, 90 - (tmpi / 7) * 56);
    W_F32(0x15C) = targetY;

    tmpi = (s32)W_S16(0xFBC + cursor * 2);
    if (swGet(tmpi + 0x41) != 0) {
        tmpi = (tmpi + 0xB3) * 0x28;
        winMsgEntry(pWin, -2,
                    *(char**)(itemDataTable + tmpi + 8),
                    (s32)*(char**)(itemDataTable + tmpi + 4));
    } else {
        winMsgEntry(pWin, -2, (char*)(catalog + 0x5B0),
                    (s32)(catalog + 0x5CC));
    }

    goto winlog_end;

winlog_state_14:
    count = W_S32(0x1040);
    pressed = W_U32(0x4);
    pages = (count + 15) / 16;

    if ((pressed & 0x200) != 0) {
        psndSFXOn(0x20013);
        CLOSE_SUBMENU_STATES();
        W_S32(0xD68) = 0;
        W_F32(0x1034) = float_320_80423c0c;
        W_F32(0x103C) = float_neg240_80423bf0;
    } else if ((W_U32(0x8) & 0x40) != 0) {
        page = W_S32(0x104C) - 1;
        if (page < 0) {
            page = 0;
        }
        W_S32(0x104C) = page;
        W_S32(0x1048) = page << 4;
        psndSFXOn(0x20035);
    } else if ((W_U32(0x8) & 0x20) != 0) {
        page = W_S32(0x104C) + 1;
        if (page >= pages) {
            page = pages - 1;
        }
        W_S32(0x104C) = page;
        W_S32(0x1048) = page << 4;
        psndSFXOn(0x20035);
    } else if ((W_U32(0x10) & 0xC000) != 0) {
        cursor = W_S32(0x1048);
        if ((cursor % 16) < 8) {
            W_S32(0x1048) = cursor + 8;
        } else {
            W_S32(0x1048) = cursor - 8;
        }
        if (W_S32(0x1048) >= count) {
            W_S32(0x1048) = count - 1;
        }
        if (W_S32(0x1048) < 0) {
            W_S32(0x1048) = 0;
        }
        psndSFXOn(0x20035);
    } else if ((W_U32(0x10) & 0x1000) != 0) {
        cursor = W_S32(0x1048);
        if ((cursor % 8) == 0) {
            W_S32(0x1048) = cursor - 9;
        } else {
            W_S32(0x1048) = cursor - 1;
        }
        if (W_S32(0x1048) < 0) {
            W_S32(0x1048) = 0;
        }
        W_S32(0x104C) = W_S32(0x1048) / 16;
        psndSFXOn(0x20035);
    } else if ((W_U32(0x10) & 0x2000) != 0) {
        cursor = W_S32(0x1048);
        if ((cursor % 8) == 7) {
            W_S32(0x1048) = cursor + 9;
        } else {
            W_S32(0x1048) = cursor + 1;
        }
        if (W_S32(0x1048) >= count) {
            W_S32(0x1048) = count - 1;
        }
        W_S32(0x104C) = W_S32(0x1048) / 16;
        psndSFXOn(0x20035);
    } else if ((pressed & 0x100) != 0) {
        cursor = W_S32(0x1048);
        if (swGet((s32)W_U8(0x1059 + cursor * 2) + 0x117A) != 0) {
            psndSFXOn(0x20012);
            W_S32(0xD68) = 21;

            cursor = W_S32(0x1048);
            kind = W_U8(0x1059 + cursor * 2);
            work = _mapAlloc(mapalloc_base_ptr, 0x30);
            memset(work, 0, 0x30);

            *(s32*)((u8*)work + 0x0) = 0;
            *(s32*)((u8*)work + 0x4) = (s32)kind;
            *(f32*)((u8*)work + 0x18) = float_neg60_80423c98;
            *(f32*)((u8*)work + 0x1C) = float_150_80423c7c;
            *(f32*)((u8*)work + 0x10) = 500.0f;
            *(f32*)((u8*)work + 0x14) = -300.0f;
            *(s32*)((u8*)work + 0x2C) = 0;
            *(s32*)((u8*)work + 0x20) = -1;
            *(s32*)((u8*)work + 0x24) = -1;
            W_PTR(0x1208) = work;
        }
    } else if ((pressed & 0x10) != 0) {
        winSortEntry(float_neg310_80423c94,
                     float_150_80423c7c,
                     pWin, 6);
        W_S32(0x124) = W_S32(0xD68);
        psndSFXOn(0x20012);
        W_S32(0xD68) = 1001;
    } else if ((pressed & 0x1000) != 0) {
        return -2;
    }

    cursor = W_S32(0x1048);
    tmpi = cursor % 16;
    MAKE_FLOAT(targetX, (tmpi / 8) * 216 - 180);
    W_F32(0x158) = targetX;

    tmpi = cursor % 16;
    MAKE_FLOAT(targetY, 106 - (tmpi % 8) * 24);
    W_F32(0x15C) = targetY;

    cursor = W_S32(0x1048);
    kind = W_U8(0x1059 + cursor * 2);
    if (swGet((s32)kind + 0x117A) != 0) {
        data = battleGetUnitMonosiriPtr((s32)kind);
        winMsgEntry(pWin, 0, *(char**)((u8*)data + 8), 0);
    } else {
        winMsgEntry(pWin, 0, (char*)(catalog + 0x5E8), 0);
    }

    goto winlog_end;

winlog_state_21:
    pressed = W_U32(0x4);
    if ((pressed & 0x200) != 0) {
        psndSFXOn(0x20013);
        W_S32(0xD68) = 22;
        work = W_PTR(0x1208);
        if (work != NULL) {
            *(s32*)((u8*)work + 0x0) = 2;
        }
    } else if ((pressed & 0x1000) != 0) {
        work = W_PTR(0x1208);
        FREE_MONOSIRI_WORK(work);
        return -2;
    } else {
        monosiriMain(W_PTR(0x1208));
    }

    goto winlog_end;

winlog_state_22:
    work = W_PTR(0x1208);
    if (work == NULL ||
        (*(f32*)((u8*)work + 0x10) > 500.0f &&
         *(f32*)((u8*)work + 0x14) < -300.0f)) {
        W_S32(0xD68) = 14;
        work = W_PTR(0x1208);
        FREE_MONOSIRI_WORK(work);
    } else {
        monosiriMain(work);
    }

    goto winlog_end;

winlog_state_sort:
    if (winSortWait(pWin) == 0) {
        W_S32(0xD68) = W_S32(0x124);
    }
    if ((W_U32(0x4) & 0x1000) != 0) {
        return -2;
    }

    goto winlog_end;

winlog_end:


#undef FREE_MONOSIRI_WORK
#undef CLOSE_SUBMENU_STATES
#undef MAKE_FLOAT
#undef SUB_HELP
#undef SUB_STATE
#undef SUB_TYPE
#undef W_S16
#undef W_U8
#undef W_PTR
#undef W_F32
#undef W_U32
#undef W_S32

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
    s32 chapter;
    s32 chapterIcon;
    s32 chapterIndex;
    f32 mapX;
    f32 mapY;
    f32 pulse;
    Mtx matrix;

    mapX = (f32)x + 74.0f + *(f32*)((s32)pWin + 0xE38);
    mapY = (f32)y + 120.0f + *(f32*)((s32)pWin + 0xE3C);
    if (*(void**)((s32)pWin + 0x30) == 0) return;
    GXSetScissor((u32)((f32)x + 72.0f), (u32)(118.0f - (f32)y),
                 0x1D0, 0xF4);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x30) + 0xA0));
    mapPos.x = mapX;
    mapPos.y = mapY;
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
            selectedPos.x = mapX + (f32)area[0];
            selectedPos.y = mapY + (f32)area[1];
            selectedPos.z = 0.0f;
            selectedScale = mapScale;
            texture = ((*(u32*)((s32)gp + 0x1C) >> 4) & 1) ? 0xB9 : 0xBA;
            winTexSet(texture, &selectedPos, &selectedScale, &white);
        } else {
            normalPos.x = mapX + (f32)area[0];
            normalPos.y = mapY + (f32)area[1];
            normalPos.z = 0.0f;
            normalScale = mapScale;
            texture = 0xB8;
            if (*((u8*)area + 4) != 0) {
                texture = 0xBA;
            }
            winTexSet(texture, &normalPos, &normalScale, &white);
        }
    }
    chapter = swByteGet(0);
    chapterIcon = -1;
    if (chapter < 0x13 || chapter > 0x36) {
        if (chapter < 0x41 || chapter > 0x6F) {
            if (chapter < 0x79 || chapter > 0xA3) {
                if (chapter < 0xAF || chapter > 0xD4) {
                    if (chapter < 0xDE || chapter > 0x104) {
                        if (chapter < 0x110 || chapter > 0x151) {
                            if (chapter > 0x15B && chapter < 0x175) {
                                chapterIcon = 0x1A3; chapterIndex = 6;
                            }
                        } else {
                            chapterIcon = 0x1A1; chapterIndex = 5;
                        }
                    } else {
                        chapterIcon = 0x1A5; chapterIndex = 4;
                    }
                } else {
                    chapterIcon = 0x1A4; chapterIndex = 3;
                }
            } else {
                chapterIcon = 0x1A2; chapterIndex = 2;
            }
        } else {
            chapterIcon = 0x1A0; chapterIndex = 1;
        }
    } else {
        chapterIcon = 0x19F; chapterIndex = 0;
    }
    if (chapterIcon != -1) {
        winIconInit();
        iconPos.x = mapX + chapterPos[chapterIndex * 2];
        iconPos.y = float_45_80423bdc + mapY + chapterPos[chapterIndex * 2 + 1];
        iconPos.z = 0.0f;
        iconScale.x = 1.0f; iconScale.y = 1.0f; iconScale.z = 1.0f;
        winIconSet(chapterIcon, &iconPos, &iconScale, &white);
    }
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
                   mapX + (f32)tubuDt[currentArea * 6],
                   mapY + (f32)tubuDt[currentArea * 6 + 1], 0.0f);
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
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    arrowTopPos.x = (f32)x;
    arrowTopPos.y = (f32)y + 120.0f +
            (*(f32*)((s32)pWin + 0xE3C) == -240.0f ? 0.0f : pulse);
    arrowTopPos.z = 0.0f;
    arrowTopScale = mapScale;
    winTexSet(0xBD, &arrowTopPos, &arrowTopScale, &white);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    arrowRightPos.x = (f32)x + 220.0f +
            (*(f32*)((s32)pWin + 0xE38) == -148.0f ? 0.0f : pulse);
    arrowRightPos.y = (f32)y;
    arrowRightPos.z = 0.0f;
    arrowRightScale = mapScale;
    winTexSet(0xBC, &arrowRightPos, &arrowRightScale, &white);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    arrowBottomPos.x = (f32)x;
    arrowBottomPos.y = (f32)y - 120.0f -
            (*(f32*)((s32)pWin + 0xE3C) == 0.0f ? 0.0f : pulse);
    arrowBottomPos.z = 0.0f;
    arrowBottomScale = mapScale;
    winTexSet(0xBE, &arrowBottomPos, &arrowBottomScale, &white);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    arrowLeftPos.x = (f32)x - 220.0f -
            (*(f32*)((s32)pWin + 0xE38) == 0.0f ? 0.0f : pulse);
    arrowLeftPos.y = (f32)y;
    arrowLeftPos.z = 0.0f;
    arrowLeftScale = mapScale;
    winTexSet(0xBB, &arrowLeftPos, &arrowLeftScale, &white);
}

void monoshiriGX(f32 x, f32 y, void* pWin) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;

    extern void winBookGX(double x, double y, void* win, s32 page);
    extern void winKirinukiGX(double x, double y, double width, double height,
                              void* win, s32 type);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* text, ...);
    extern void winFontSetWidth(Vec3* pos, Vec3* scale, void* color,
                                f32 width, char* text, ...);
    extern u16 FontGetMessageWidth(char* text);
    extern void winTexSet(s32 texture, Vec3* pos, Vec3* scale, void* color);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern s32 evtGetValue(void* evt, s32 value);
    extern char* msgSearch(char* key);

    extern u8 enemy_monoshiri_sort_table[];
    extern char str_PCTd_PCTd_80423b74[];
    extern char str__80423b8c[];
    extern char str_PCTd_80423b90[];

    extern u32 dat_80423ac4;
    extern u32 dat_80423ac8;

    u8* catalog = (u8*)str_msg_menu_kiroku_map_802f6708;
    volatile u32 titleColorSeed;
    volatile u32 textureColorSeed;
    volatile u32 countColorSeed;
    volatile u32 listColorSeed;

    Vec3 titlePos, titleCallPos, titleScale;
    Vec3 countPos, countCallPos, countScale;
    Vec3 numberPos, numberCallPos, numberScale;
    Vec3 namePos, nameCallPos, nameScale;
    Vec3 upTexPos, upTexCallPos, upTexScale;
    Vec3 upIconPos, upIconCallPos, upIconScale;
    Vec3 downTexPos, downTexCallPos, downTexScale;
    Vec3 downIconPos, downIconCallPos, downIconScale;
    Vec3 sortPos, sortCallPos, sortScale;
    Vec3 sortIconPos, sortIconCallPos, sortIconScale;

    u32 titleColor;
    u32 countColor;
    u32 numberColor;
    u32 nameColor;
    u32 upTexColor;
    u32 upIconColor;
    u32 downTexColor;
    u32 downIconColor;
    u32 sortColor;
    u32 sortIconColor;

    s32 i;
    s32 unit;
    char countText[256];
    char numberText[32];
    char* format;
    char* name;
    char* title;
    char* sortText;
    void** mono;
    u16 messageWidth;
    u16 scaledWidth;
    f32 numberBaseX;
    f32 nameBaseX;
    f32 baseY;
    f32 sortWidth;

    titleColorSeed = dat_80423ac8;
    textureColorSeed = dat_80423ac4;
    countColorSeed = dat_80423ac8;
    listColorSeed = dat_80423ac8;

    winBookGX(x, y, pWin, 1);
    winKirinukiGX((double)((f32)x - 110.0f),
                  (double)((f32)y - 160.0f),
                  100.0, 50.0, pWin, 0);

    title = msgSearch((char*)catalog + 0x4B0);
    messageWidth = FontGetMessageWidth(title);
    scaledWidth = (u16)(0.75f * (f32)messageWidth);
    if ((f32)scaledWidth > 80.0f) {
        scaledWidth = (u16)((f32)scaledWidth *
                            (80.0f / (f32)scaledWidth));
    }

    winFontInit();
    titlePos = *(Vec3*)(catalog + 0x290);
    titlePos.x = (f32)x - 108.0f +
                 (f32)((100 - scaledWidth) / 2);
    titlePos.y = (f32)y - 166.0f;
    titleScale = *(Vec3*)(catalog + 0x29C);
    titleCallPos = titlePos;
    titleColor = titleColorSeed;
    winFontSetWidth(&titleCallPos, &titleScale, &titleColor,
                    80.0f, title);

    format = "%d/%d";
    if ((u32)*(s32*)((s32)gp + 0x16C) != 0) {
        format = str_PCTd_PCTd_80423b74;
    }
    sprintf(countText, format, *(s32*)((s32)pWin + 0x1044),
            *(s32*)((s32)pWin + 0x1040));

    messageWidth = FontGetMessageWidth(countText);
    scaledWidth = (u16)(0.75f * (f32)messageWidth);
    countPos = *(Vec3*)(catalog + 0x2A8);
    countPos.x = (f32)x - 108.0f +
                 (f32)((100 - scaledWidth) / 2);
    countPos.y = (f32)y - 184.0f;
    countScale = *(Vec3*)(catalog + 0x2B4);
    countCallPos = countPos;
    countColor = countColorSeed;
    winFontSet(&countCallPos, &countScale, &countColor, countText);

    winFontInit();
    numberBaseX = (f32)x + 19.0f;
    baseY = (f32)y - 16.0f;
    nameBaseX = (f32)x + 56.0f;

    i = 0;
    do {
        unit = ((u8*)pWin)[
            0x1059 + ((*(s32*)((s32)pWin + 0x104C) * 16 + i) * 2)];
        mono = (void**)battleGetUnitMonosiriPtr(unit);

        if ((*(s32*)((s32)pWin + 0x104C) * 16 + i) >=
            *(s32*)((s32)pWin + 0x1040)) {
            break;
        }

        if (swGet(unit + 0x117A) != 0) {
            if (unit == 0x4D || unit == 0x87) {
                if (evtGetValue(0, -170000000) < 0xD2) {
                    name = msgSearch((char*)catalog + 0x420);
                } else {
                    name = msgSearch((char*)catalog + 0x430);
                }
            } else {
                name = msgSearch((char*)mono[0]);
            }
        } else {
            name = "---";
            if ((u32)*(s32*)((s32)gp + 0x16C) != 0) {
                name = str__80423b8c;
            }
        }

        format = "%d";
        if ((u32)*(s32*)((s32)gp + 0x16C) != 0) {
            format = str_PCTd_80423b90;
        }
        sprintf(numberText, format, enemy_monoshiri_sort_table[unit]);

        {
            s32 columnOffset = (i / 8) * 216;
            s32 rowOffset = (i % 8) * 24;
            f32 rowY = baseY - (f32)rowOffset;

            namePos = *(volatile Vec3*)(catalog + 0x2D8);
            namePos.x = nameBaseX + (f32)(i / 8) * 216.0f;
            namePos.y = baseY - (f32)(i % 8) * 24.0f;

            numberPos = *(volatile Vec3*)(catalog + 0x2C0);
            numberPos.x = numberBaseX + (f32)columnOffset;
            numberPos.y = rowY;
            numberScale = *(volatile Vec3*)(catalog + 0x2CC);
            numberCallPos = numberPos;
            numberColor = listColorSeed;
            winFontSetWidth(&numberCallPos, &numberScale, &numberColor,
                            40.0f, numberText);

            nameScale = *(volatile Vec3*)(catalog + 0x2E4);
            nameCallPos = namePos;
            nameColor = listColorSeed;
            winFontSetWidth(&nameCallPos, &nameScale, &nameColor,
                            145.0f, name);
        }

        i++;
    } while (i < 16);

    if (*(s32*)((s32)pWin + 0x104C) > 0) {
        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));

        upTexPos = *(Vec3*)(catalog + 0x2F0);
        upTexPos.x = (f32)x + 440.0f;
        upTexPos.y = (f32)y + 18.0f;
        upTexScale = *(Vec3*)(catalog + 0x2FC);
        upTexCallPos = upTexPos;
        upTexColor = textureColorSeed;
        winTexSet(0x17, &upTexCallPos, &upTexScale, &upTexColor);

        winIconInit();
        upIconPos = *(Vec3*)(catalog + 0x308);
        upIconPos.x = (f32)x + 440.0f;
        upIconPos.y = (f32)y;
        upIconScale = *(Vec3*)(catalog + 0x314);
        upIconCallPos = upIconPos;
        upIconColor = textureColorSeed;
        winIconSet(0x86, &upIconCallPos, &upIconScale, &upIconColor);
    }

    if (*(s32*)((s32)pWin + 0x104C) <
        ((*(s32*)((s32)pWin + 0x1040) + 15) / 16) - 1) {
        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));

        downTexPos = *(Vec3*)(catalog + 0x320);
        downTexPos.x = (f32)x + 440.0f;
        downTexPos.y = (f32)y - 255.0f;
        downTexScale = *(Vec3*)(catalog + 0x32C);
        downTexCallPos = downTexPos;
        downTexColor = textureColorSeed;
        winTexSet(0x17, &downTexCallPos, &downTexScale, &downTexColor);

        winIconInit();
        downIconPos = *(Vec3*)(catalog + 0x338);
        downIconPos.x = (f32)x + 440.0f;
        downIconPos.y = (f32)y - 240.0f;
        downIconScale = *(Vec3*)(catalog + 0x344);
        downIconCallPos = downIconPos;
        downIconColor = textureColorSeed;
        winIconSet(0x88, &downIconCallPos, &downIconScale, &downIconColor);
    }

    sortText = msgSearch((char*)catalog + 0x4D8);
    sortWidth =
        0.7f * (f32)FontGetMessageWidth(sortText);

    winFontInit();
    sortPos = *(Vec3*)(catalog + 0x350);
    sortPos.x = ((f32)x + 390.0f) + (30.0f - sortWidth);
    sortPos.y = (f32)y - 210.0f;
    sortScale = *(Vec3*)(catalog + 0x35C);
    sortCallPos = sortPos;
    sortColor = titleColorSeed;
    winFontSet(&sortCallPos, &sortScale, &sortColor, sortText);

    winIconInit();
    sortIconPos = *(Vec3*)(catalog + 0x368);
    sortIconPos.x = sortPos.x - 30.0f;
    sortIconPos.y = ((f32)y - 210.0f) - 10.0f;
    sortIconScale = *(Vec3*)(catalog + 0x374);
    sortIconCallPos = sortIconPos;
    sortIconColor = textureColorSeed;
    winIconSet(0x219, &sortIconCallPos, &sortIconScale, &sortIconColor);
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
    extern void* gp;
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
    extern f32 float_60_80423b58;
    extern s32 getMarioStDvdRoot(void);
    extern s32 fileAsyncf(s32 kind, s32 unused, char* fmt, s32 root, char* lang, ...);
    extern void* fileAllocf(s32 kind, char* fmt, s32 root, char* lang, ...);
    extern void TEXGetGXTexObjFromPalette(void* tpl, void* texObj, s32 id);
    extern s32 GXGetTexObjHeight(void* texObj);
    extern s32 GXGetTexObjWidth(void* texObj);
    extern s32 GXGetTexBufferSize(s32 width, s32 height, s32 format, s32 mipmap, s32 maxLod);
    extern void* smartAlloc(s32 size, s32 heap);
    extern void* battleGetUnitMonosiriPtr(s32 enemyType);
    extern s32 animGroupBaseAsync(char* name, s32 battle, s32 unused);
    extern s32 animPoseEntry(char* name, s32 battle);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 loop);
    extern void animPoseWorldPositionEvalOn(s32 poseId, s32 flag);
    extern void animPoseMain(s32 poseId);
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
        switch (state) {
        case 0: {
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

            break;
        }
        case 1:
            monoData = battleGetUnitMonosiriPtr(*(s32*)((s32)work + 4));
            if (animGroupBaseAsync(*(char**)((s32)monoData + 0xC), inBattle, 0) != 0 &&
                (*(s32*)((s32)work + 4) != 0x6B ||
                 animGroupBaseAsync(str_c_n_moa_a_802f6bac, inBattle, 0) != 0)) {
                *(s32*)((s32)work + 0x2C) += 0x14;
                if (*(s32*)((s32)work + 0x2C) > 0xFF) {
                    *(s32*)((s32)work + 0x2C) = 0xFF;
                }
                if (*(s32*)((s32)work + 0x20) == -1) {
                    if (*(s32*)((s32)work + 4) == 0x6B) {
                        *(s32*)((s32)work + 0x20) =
                            animPoseEntry(*(char**)((s32)monoData + 0xC), inBattle);
                        animPoseSetAnim(*(s32*)((s32)work + 0x20),
                                        *(char**)((s32)monoData + 0x10), 1);
                        animPoseWorldPositionEvalOn(*(s32*)((s32)work + 0x20), 0);
                        animPoseMain(*(s32*)((s32)work + 0x20));
                        *(s32*)((s32)work + 0x24) =
                            animPoseEntry(str_c_n_moa_a_802f6bac, inBattle);
                        animPoseSetAnim(*(s32*)((s32)work + 0x24), str_S_1_80423b48, 1);
                        animPoseMain(*(s32*)((s32)work + 0x24));
                    } else {
                        *(s32*)((s32)work + 0x20) =
                            animPoseEntry(*(char**)((s32)monoData + 0xC), inBattle);
                        animPoseSetAnim(*(s32*)((s32)work + 0x20),
                                        *(char**)((s32)monoData + 0x10), 1);
                        animPoseMain(*(s32*)((s32)work + 0x20));
                    }
                    dispEntry(7, 6, capture, work, float_0_80423aec);
                }
            }

            break;
        case 2:
            *(f32*)((s32)work + 0x18) = float_600_80423b4c;
            *(f32*)((s32)work + 0x1C) = targetY;

            break;
        }

        *(f32*)((s32)work + 0x10) +=
            (*(f32*)((s32)work + 0x18) - *(f32*)((s32)work + 0x10)) /
            ((float_6_80423b54 * (f32)*(s32*)((s32)gp + 4)) / float_60_80423b58);
        *(f32*)((s32)work + 0x14) +=
            (*(f32*)((s32)work + 0x1C) - *(f32*)((s32)work + 0x14)) /
            ((float_6_80423b54 * (f32)*(s32*)((s32)gp + 4)) / float_60_80423b58);
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
    Vec3 minBounds = *(Vec3*)(animData + 0x44);
    Vec3 maxBounds = *(Vec3*)(animData + 0x50);
    f32 minX = minBounds.x;
    f32 minY = minBounds.y;
    f32 maxX = maxBounds.x;
    f32 maxY = maxBounds.y;
    f32 centerX = (minBounds.x + maxBounds.x) * 0.5f;
    f32 centerY = (minBounds.y + maxBounds.y) * 0.5f;
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
    typedef struct DispVec { f32 x, y, z; } DispVec;
    typedef union DispConvert {
        f64 d;
        struct { u32 hi, lo; } w;
    } DispConvert;
    extern void winHalfBookGX(f32, f32, void*, s32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXInitTexObj(void*, void*, s16, s16, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
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
    extern u8 enemy_monoshiri_sort_table[];
    extern char* msgSearch(char*);
    extern s32 evtGetValue(void*, s32);
    extern u16 FontGetMessageWidth(char*);
    extern void winFontSet(DispVec*, DispVec*, void*, char*);
    extern DispVec vec3_802f6ae8;
    extern DispVec vec3_802f6af4;
    extern DispVec vec3_802f6b00;
    extern DispVec vec3_802f6b0c;
    extern f32 float_0p75_80423b10;
    extern f32 float_16_80423b14;
    extern f32 float_248_80423b18;
    extern f32 float_180_80423b1c;
    extern f32 float_210_80423b28;
    extern f64 double_to_int_mask_802f6b78;
    extern char str_PCTs_PCTs_80423b20[];
    u32 texObj[8],tev=dat_80423ae8,alpha;s32 i,w,h;void* file=*(void**)((s32)work+8);f32 x=*(f32*)((s32)work+0x10),y=*(f32*)((s32)work+0x14);f32 a[3][4],b[3][4],c[3][4];
    if(file==0)return;winHalfBookGX(x,y,**(void***)((s32)file+0xA0),0);TEXGetGXTexObjFromPalette(**(void***)((s32)file+0xA0),texObj,0x37);w=GXGetTexObjWidth(texObj);h=GXGetTexObjHeight(texObj);TEXGetGXTexObjFromPalette(**(void***)((s32)file+0xA0),texObj,0x37);GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0);GXLoadTexObj(texObj,0);GXInitTexObj(texObj,**(void***)((s32)work+0x28),(s16)w,(s16)h,5,0,0,0);GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0);GXLoadTexObj(texObj,1);GXSetNumTexGens(8);for(i=0;i<8;i++){f32 s=float_1_80423af0-float_0p04_80423af4*(f32)(7-i);PSMTXTrans(a,float_0p5_80423af8,float_0p5_80423af8,0.0f);PSMTXScale(b,s,s,s);PSMTXTrans(c,float_neg0p5_80423afc,float_neg0p5_80423afc,0.0f);PSMTXConcat(a,b,a);PSMTXConcat(a,c,a);GXLoadTexMtxImm(a,0x1E+i*3,1);GXSetTexCoordGen2(i,1,4,0x1E+i*3,0,0x7D);}GXSetTevColor(1,&tev);alpha=(unk_804295f8&0xFFFFFF00)|*(u8*)((s32)work+0x2C);GXSetTevColor(2,&alpha);GXSetNumTevStages(12);
    for(i=0;i<8;i++){GXSetTevOrder(i,i,1,0xFF);GXSetTevColorOp(i,0,0,0,1,0);GXSetTevAlphaOp(i,0,0,0,1,0);GXSetTevColorIn(i,15,15,15,2);GXSetTevAlphaIn(i,7,4,6,i?0:7);GXSetTevKAlphaSel(i,7);}
    GXSetTevOrder(8,7,1,0xFF);GXSetTevColorOp(8,0,0,0,1,0);GXSetTevAlphaOp(8,0,0,0,1,0);GXSetTevColorIn(8,0,8,9,15);GXSetTevAlphaIn(8,0,7,4,4);GXSetTevOrder(9,0xFF,0xFF,0xFF);GXSetTevColorOp(9,0,0,0,1,0);GXSetTevAlphaOp(9,0,0,0,1,0);GXSetTevColorIn(9,15,15,15,0);GXSetTevAlphaIn(9,7,0,6,7);GXSetTevOrder(10,7,0,0xFF);GXSetTevColorOp(10,0,0,0,1,0);GXSetTevAlphaOp(10,0,0,0,1,0);GXSetTevColorIn(10,15,8,0,15);GXSetTevAlphaIn(10,7,7,7,0);GXSetTevOrder(11,7,0,0xFF);GXSetTevColorOp(11,0,0,0,1,0);GXSetTevAlphaOp(11,0,0,0,1,0);GXSetTevColorIn(11,8,0,1,15);GXSetTevAlphaIn(11,7,7,7,4);
    GXBegin(0x80,0,4);DAT_cc008000=x+32.0f;DAT_cc008000=y-8.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=x+32.0f;DAT_cc008000=y-168.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=x+248.0f;DAT_cc008000=y-168.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=1.0f;DAT_cc008000=x+248.0f;DAT_cc008000=y-8.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=0.0f;
    {
        char text[256];
        char* base = (char*)str_msg_menu_kiroku_map_802f6708;
        char* format;
        char* message;
        void** mono;
        s32 unit = *(s32*)((s32)work + 4);
        u32 color = dat_80423ae8;
        f32 width;
        u16 messageWidth;
        DispConvert convert;
        DispVec pos;
        DispVec scale;

        mono = battleGetUnitMonosiriPtr(unit);
        if (unit == 0x4D || unit == 0x87) {
            if (evtGetValue(0, -170000000) < 0xD2) {
                message = msgSearch(base + 0x420);
            } else {
                message = msgSearch(base + 0x430);
            }
        } else {
            message = msgSearch((char*)mono[0]);
        }
        format = base + 0x418;
        if (*(s32*)((s32)gp + 0x16C) != 0) {
            format = base + 0x410;
        }
        sprintf(text, format, enemy_monoshiri_sort_table[unit], message);
        messageWidth = FontGetMessageWidth(text);
        convert.w.hi = 0x43300000;
        convert.w.lo = messageWidth;
        width = float_0p75_80423b10 *
                (f32)(convert.d - double_to_int_mask_802f6b78);
        winFontInit();
        pos = vec3_802f6ae8;
        pos.x = (float_248_80423b18 - width) * float_0p5_80423af8 +
                (float_16_80423b14 + x);
        pos.y = y - float_180_80423b1c;
        scale = vec3_802f6af4;
        winFontSet(&pos, &scale, &color, text);

        format = base + 0x440;
        if (*(s32*)((s32)gp + 0x16C) != 0) {
            format = str_PCTs_PCTs_80423b20;
        }
        sprintf(text, format, msgSearch(base + 0x448), msgSearch((char*)mono[5]));
        messageWidth = FontGetMessageWidth(text);
        convert.w.hi = 0x43300000;
        convert.w.lo = messageWidth;
        width = float_0p75_80423b10 *
                (f32)(convert.d - double_to_int_mask_802f6b78);
        pos = vec3_802f6b00;
        pos.x = float_1_80423af0 +
                (float_248_80423b18 - width) * float_0p5_80423af8 +
                (float_16_80423b14 + x);
        pos.y = y - float_210_80423b28;
        scale = vec3_802f6b0c;
        winFontSet(&pos, &scale, &color, text);
    }
}
