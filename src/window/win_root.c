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

s32 winSortWait(void* win) {
    s32 value = *(s32*)((s32)win + 0x17C);
    return (u32)(-value | value) >> 31;
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


void winKirinukiGX(double x, double y, double w, double h, void* pWin, s32 style) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void* camGetPtr(s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    u32 fog = 0;
    s32 row;
    s32 column;
    s32 base;

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
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

    winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
    pos.z = 0.0f;
    scale.x = (f32)w / 300.0f;
    scale.y = (f32)h / 180.0f;
    scale.z = 1.0f;
    base = style == 0 ? 8 : 0x16;
    for (row = 0; row < 3; row++) {
        for (column = 0; column < 3; column++) {
            pos.x = (f32)x + (f32)column * (f32)w * 0.5f;
            pos.y = (f32)y - (f32)row * (f32)h * 0.5f;
            winTexSet(base + row * 3 + column, &pos, &scale, &white);
        }
    }
}

void winMailGX(double x, double y, s32 page) {
    extern void GXSetBlendMode(s32, s32, s32, s32); extern void GXSetZCompLoc(s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*); extern u32 dat_80423238;
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32); extern void GXSetZMode(s32, s32, s32);
    extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32); extern void GXSetTevOp(s32, s32); extern void GXSetCullMode(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void* gp;
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, void* pos, void* scale, void* color);
    extern void* camGetPtr(s32); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32);
    f32 pos[3];
    f32 scale[3];
    u32 white = 0xFFFFFFFF;
    u32 fog = dat_80423238;
    s32 row;

    GXSetBlendMode(0, 1, 0, 0); GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&fog); GXSetZCompLoc(0); GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0); GXSetNumChans(0); GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1); GXSetTevOp(0, 3); GXSetCullMode(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2); GXClearVtxDesc(); GXSetVtxDesc(9, 1); GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0); GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(4)+0x48,0); GXSetCurrentMtx(0);
    winTexInit(**(void***)((s32)gp + 0xA0));
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
    extern void GXSetBlendMode(s32, s32, s32, s32); extern void GXSetZCompLoc(s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*); extern u32 dat_8042323c;
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32); extern void GXSetZMode(s32, s32, s32);
    extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32); extern void GXSetTevOp(s32, s32); extern void GXSetCullMode(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, void* pos, void* scale, void* color);
    extern void* camGetPtr(s32); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32);
    f32 pos[3];
    f32 scale[3];
    u32 white = 0xFFFFFFFF;
    u32 fog = dat_8042323c;
    s32 i;
    s32 j;

    GXSetBlendMode(0, 1, 0, 0); GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&fog); GXSetZCompLoc(0); GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0); GXSetNumChans(0); GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1); GXSetTevOp(0, 3); GXSetCullMode(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2); GXClearVtxDesc(); GXSetVtxDesc(9, 1); GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0); GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(4)+0x48,0); GXSetCurrentMtx(0);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    pos[2] = 0.0f;
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    scale[2] = 1.0f;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 5; i++) {
            pos[0] = (f32)x + i * 48.0f;
            pos[1] = (f32)y - j * 48.0f;
            winTexSet(0x60 + type * 4 + ((i + j) & 3), pos, scale, &white);
        }
    }
    scale[0] = 2.0f;
    scale[1] = 2.0f;
    pos[0] = (f32)x - 16.0f;
    pos[1] = (f32)y + 16.0f;
    winTexSet(0x70 + type, pos, scale, &white);
}

void winBookGX(double x, double y, void* pWin, s32 page) {
    extern void GXSetBlendMode(s32, s32, s32, s32); extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32); extern void GXSetZMode(s32, s32, s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*); extern void* camGetPtr(s32);
    extern void GXLoadPosMtxImm(void*, s32); extern void GXSetCurrentMtx(s32);
    extern u32 dat_80423220;
    extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32); extern void GXSetTevOp(s32, s32); extern void GXSetCullMode(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void winFontInit(void);
    extern void winFontSetLabel(Vec3* pos, Vec3* scale, void* color, char* label);
    extern char* msgSearch(char* key);
    extern char str_msg_menu_sort_narabi_802f4da8[];
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    u32 fog = dat_80423220;
    u32 gray = 0xA0A0A0FF;
    s32 row;
    s32 column;

    GXSetBlendMode(0, 1, 0, 0); GXSetZCompLoc(0); GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0); GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fog);
    GXSetNumChans(0); GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1); GXSetTevOp(0, 3); GXSetCullMode(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2); GXClearVtxDesc(); GXSetVtxDesc(9, 1); GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0); GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(1) + 0x30, 0); GXSetCurrentMtx(0);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;
    pos.z = 0.0f;
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

void winWazaGX(double x, double y, double w, double h, void* pWin) {
    extern void GXSetBlendMode(s32, s32, s32, s32); extern void GXSetZCompLoc(s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*); extern u32 dat_80423260;
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32); extern void GXSetZMode(s32, s32, s32);
    extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32); extern void GXSetTevOp(s32, s32); extern void GXSetCullMode(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, void* pos, void* scale, void* color);
    f32 pos[3];
    f32 scale[3];
    u32 white = 0xFFFFFFFF;
    u32 fog = dat_80423260;
    s32 row;
    s32 selected = *(s32*)((s32)pWin + 0x174);

    GXSetBlendMode(0, 1, 0, 0); GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&fog); GXSetZCompLoc(0); GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0); GXSetNumChans(0); GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1); GXSetTevOp(0, 3); GXSetCullMode(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2); GXClearVtxDesc(); GXSetVtxDesc(9, 1); GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0); GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    pos[2] = 0.0f;
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    scale[2] = 1.0f;
    for (row = 0; row < 6; row++) {
        pos[0] = (f32)x + 22.0f;
        pos[1] = (f32)y - 34.0f * row;
        winTexSet(0x98 + row, pos, scale, &white);
        pos[0] += (f32)w - 44.0f;
        winTexSet(0x9E + row, pos, scale, &white);
    }
    pos[0] = (f32)x + 8.0f;
    pos[1] = (f32)y - selected * 34.0f;
    scale[0] = (f32)w / 100.0f;
    scale[1] = (f32)h / 100.0f;
    winTexSet(0xA4, pos, scale, &white);
}

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
    extern void psndSFXOn(s32 id);
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
            psndSFXOn(0x20032);
            psndSFXOn(0x20031);
            cursor--;
            if (cursor < 0) cursor = count - 1;
            *(s32*)(w + 0x40) = cursor;
            *(s32*)(body + 0x00) = *(s32*)(w + 0x48 + cursor * 0x18);
            *(s32*)(body + 0x0C) = 0x1E;
        } else if ((*(u32*)(w + 0x10) & 0x8000) != 0 && *(s32*)(body + 0x0C) == 100) {
            psndSFXOn(0x20032);
            psndSFXOn(0x20031);
            cursor++;
            if (cursor >= count) cursor = 0;
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

void winSortGX(void* pWin) {
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern u32 dat_804232d4;
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, void* pos, void* scale, void* color);
    extern void* camGetPtr(s32); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32);
    f32 pos[3];
    f32 scale[3];
    u32 white = 0xFFFFFFFF;
    u32 fog = dat_804232d4;
    s32 type = *(s32*)((s32)pWin + 0x184);
    s32 cursor = *(s32*)((s32)pWin + 0x174);
    s32 i;

    GXSetBlendMode(0, 1, 0, 0);
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fog);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevOp(0, 3);
    GXSetCullMode(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2); GXClearVtxDesc();
    GXSetVtxDesc(9, 1); GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0); GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(4)+0x48,0); GXSetCurrentMtx(0);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    scale[2] = 1.0f;
    pos[2] = 0.0f;
    for (i = 0; i < 4; i++) {
        pos[0] = *(f32*)((s32)pWin + 0x164) + i * 42.0f;
        pos[1] = *(f32*)((s32)pWin + 0x168);
        winTexSet(0xA3 + i, pos, scale, &white);
    }
    pos[0] = *(f32*)((s32)pWin + 0x164) + cursor * 42.0f;
    pos[1] = *(f32*)((s32)pWin + 0x168) + 4.0f;
    scale[0] = 1.15f;
    scale[1] = 1.15f;
    winTexSet(type == 7 ? 0xA7 : 0xA5, pos, scale, &white);
}

void winMailDisp(void* pWin) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void winIconInit(void);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern void winFontInit(void);
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void* mailGetPtr(s32 id);
    extern s32 pouchCheckMail(s32 id);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* text);
    extern char* msgSearch(char* key);
    extern void windowDispGX_Waku_col(f32 x, f32 y, f32 w, f32 h, f32 r, s32 type, void* color);
    extern void winMailGX(double x, double y, s32 state);
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    u32 frame = 0xC0C0C0FF;
    s32 page;
    s32 count;
    s32 i;
    s32 entry;
    char* text;
    u8* mail;
    s32 mailId;

    if (*(f32*)((s32)pWin + 0x244) >= 320.0f ||
        *(f32*)((s32)pWin + 0x24C) <= -240.0f) {
        return;
    }
    winMailGX(*(f32*)((s32)pWin + 0x244),
              *(f32*)((s32)pWin + 0x24C),
              *(s32*)((s32)pWin + 0x20C));
    if (*(s32*)((s32)pWin + 0x20C) == 0xCA) {
        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    }
    page = *(s32*)((s32)pWin + 0x258);
    count = *(s32*)((s32)pWin + 0x254);
    windowDispGX_Waku_col(-250.0f, 180.0f, 500.0f, 320.0f, 20.0f, 0, &frame);
    winIconInit();
    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;
    for (i = 0; i < count && i < 8; i++) {
        entry = i;
        mailId = *(u8*)((s32)pWin + 0x26F + entry * 2);
        mail = (u8*)mailGetPtr(mailId);
        winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
        pos.x = *(f32*)((s32)pWin + 0x244) + 212.0f;
        pos.y = *(f32*)((s32)pWin + 0x260) +
                (*(f32*)((s32)pWin + 0x24C) - 56.0f - 40.0f * i);
        pos.z = 0.0f;
        winTexSet(0x77, &pos, &scale, &white);
        pos.x = *(f32*)((s32)pWin + 0x244) + 72.0f;
        pos.y = *(f32*)((s32)pWin + 0x260) +
                (*(f32*)((s32)pWin + 0x24C) - 56.0f - 40.0f * i - 3.0f);
        pos.z = 0.0f;
        winTexSet(pouchCheckMail(mailId) == 2 ? 0x5E : 0x5D, &pos, &scale, &white);
        pos.x = *(f32*)((s32)pWin + 0x244) + 90.0f;
        winTexSet((*(u8*)((s32)pWin + 0x26E + entry * 2) + 1) / 10 + 0x65,
                  &pos, &scale, &white);
        pos.x = *(f32*)((s32)pWin + 0x244) + 106.0f;
        winTexSet((*(u8*)((s32)pWin + 0x26E + entry * 2) + 1) % 10 + 0x65,
                  &pos, &scale, &white);
    }
    winFontInit();
    scale.x = 0.8f;
    scale.y = 0.8f;
    for (i = 0; i < count && i < 8; i++) {
        entry = i;
        mailId = *(u8*)((s32)pWin + 0x26F + entry * 2);
        mail = (u8*)mailGetPtr(mailId);
        text = msgSearch(*(char**)(mail + 8));
        pos.x = *(f32*)((s32)pWin + 0x244) + 120.0f;
        pos.y = *(f32*)((s32)pWin + 0x260) +
                (*(f32*)((s32)pWin + 0x24C) - 56.0f - 40.0f * i + 8.0f);
        pos.z = 0.0f;
        winFontSet(&pos, &scale, &white, text);
    }
}

void winNameGX(double x, double y, double width, double unusedHeight, void* pWin, s32 type) {
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void* camGetPtr(s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void TEXGetGXTexObjFromPalette(void*, void*, s32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern s32 GXGetTexObjHeight(void*);
    extern s32 GXGetTexObjWidth(void*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXBegin(s32, s32, s32);
    extern volatile f32 DAT_cc008000;
    extern u32 dat_8042321c;
    u32 color = dat_8042321c;
    u32 tex0[8], tex1[8];
    f32 texMtx0[3][4], texMtx1[3][4];
    void* fileData = **(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0);
    void* camera;
    s32 leftTex;
    s32 rightTex;
    f32 edge;
    f32 height;
    f32 split;
    f32 right;

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
        leftTex = 0x13;
        rightTex = 0x14;
        edge = 16.0f;
        height = 32.0f;
    } else if (type == 1) {
        leftTex = 0x15;
        rightTex = 0x16;
        edge = 16.0f;
        height = 32.0f;
    } else {
        leftTex = 0x78;
        rightTex = 0x79;
        edge = 8.0f;
        height = 24.0f;
    }

    TEXGetGXTexObjFromPalette(fileData, tex0, leftTex);
    GXInitTexObjLOD(tex0, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(tex0, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx0, ((f32)width - edge) / (f32)GXGetTexObjWidth(tex0),
                height / (f32)GXGetTexObjHeight(tex0), 1.0f);
    GXLoadTexMtxImm(texMtx0, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    split = (f32)x + (f32)width - edge;
    DAT_cc008000 = (f32)x; DAT_cc008000 = (f32)y; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f;
    DAT_cc008000 = (f32)x; DAT_cc008000 = (f32)y - height; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f; DAT_cc008000 = 1.0f;
    DAT_cc008000 = split; DAT_cc008000 = (f32)y - height; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f; DAT_cc008000 = 1.0f;
    DAT_cc008000 = split; DAT_cc008000 = (f32)y; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f; DAT_cc008000 = 0.0f;

    TEXGetGXTexObjFromPalette(fileData, tex1, rightTex);
    GXInitTexObjLOD(tex1, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(tex1, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx1, edge / (f32)GXGetTexObjWidth(tex1),
                height / (f32)GXGetTexObjHeight(tex1), 1.0f);
    GXLoadTexMtxImm(texMtx1, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    right = split + edge;
    DAT_cc008000 = split; DAT_cc008000 = (f32)y; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f;
    DAT_cc008000 = split; DAT_cc008000 = (f32)y - height; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 0.0f; DAT_cc008000 = 1.0f;
    DAT_cc008000 = right; DAT_cc008000 = (f32)y - height; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f; DAT_cc008000 = 1.0f;
    DAT_cc008000 = right; DAT_cc008000 = (f32)y; DAT_cc008000 = 0.0f;
    DAT_cc008000 = 1.0f; DAT_cc008000 = 0.0f;
}

void winMsgDisp(double x, double y, void* pWin) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void windowDispGX_Waku_col(f32, f32, f32, f32, f32, s32, void*);
    extern void winTexInit(void*);
    extern void winTexSet(s32, Vec3*, Vec3*, void*);
    extern void winIconInit(void);
    extern void winIconSet(s32, Vec3*, Vec3*, void*);
    extern char* msgSearch(char*);
    extern void winNameGX(f32, f32, f32, f32, void*, s32);
    extern u16 FontGetMessageWidth(char*);
    extern void winFontInit(void);
    extern void winFontSet(Vec3*, Vec3*, void*, char*);
    extern void winFontSetEdgeWidth(Vec3*, Vec3*, void*, char*, f32);
    extern void winFontSetLabel(Vec3*, Vec3*, void*, char*);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern u8 itemDataTable[];
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

void winBgGX(f32 x,f32 y,void* pWin,s32 type){
    extern void GXSetTevColor(s32,void*);extern void GXSetBlendMode(s32,s32,s32,s32);extern void GXSetZCompLoc(s32);extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);extern void GXSetZMode(s32,s32,s32);extern void GXSetFog(s32,f32,f32,f32,f32,void*);extern void GXSetNumChans(s32);extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetTevSwapMode(s32,s32,s32);
    extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXConcat(void*,void*,void*);extern void PSMTXScale(void*,f32,f32,f32);extern void* camGetPtr(s32);extern void GXLoadPosMtxImm(void*,s32);extern void GXLoadTexMtxImm(void*,s32,s32);extern void GXSetCurrentMtx(s32);extern void GXSetCullMode(s32);extern void GXClearVtxDesc(void);extern void GXSetVtxDesc(s32,s32);extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void TEXGetGXTexObjFromPalette(void*,void*,s32);extern void GXInitTexObjLOD(void*,s32,s32,f32,f32,f32,s32,s32,s32);extern void GXLoadTexObj(void*,s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern s32 GXGetTexObjHeight(void*);extern s32 GXGetTexObjWidth(void*);extern void GXBegin(s32,s32,s32);extern volatile f32 DAT_cc008000;
    extern u32 dat_804231b8,dat_804231bc,dat_804231c0,dat_804231c4,dat_804231c8,dat_804231cc,dat_804231d0,dat_804231d4,dat_804231d8,dat_804231dc,dat_804231e0,dat_804231e4,dat_804231e8,dat_804231ec,dat_804231f0,dat_804231f4;
    u32 colors[3],texObj[8];f32 mtx[3][4];void* cam;void* file;void* tpl;f32 w,h;
    if(type==0){colors[0]=dat_804231b8;colors[1]=dat_804231bc;colors[2]=dat_804231c0;}else if(type==1){colors[0]=dat_804231c4;colors[1]=dat_804231c8;colors[2]=dat_804231cc;}else if(type==2){colors[0]=dat_804231d0;colors[1]=dat_804231d4;colors[2]=dat_804231d8;}else if(type==3){colors[0]=dat_804231dc;colors[1]=dat_804231e0;colors[2]=dat_804231e4;}else{colors[0]=dat_804231e8;colors[1]=dat_804231ec;colors[2]=dat_804231f0;}
    GXSetTevColor(1,&colors[0]);GXSetTevColor(2,&colors[1]);GXSetTevColor(3,&colors[2]);GXSetBlendMode(0,1,0,0);GXSetZCompLoc(0);GXSetAlphaCompare(6,0x80,1,0,0);GXSetZMode(0,7,0);GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&dat_804231f4);GXSetNumChans(0);GXSetChanCtrl(4,0,0,0,0,0,2);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,15,15,15,2);GXSetTevAlphaIn(0,7,7,7,4);GXSetTevSwapMode(0,0,0);
    PSMTXTrans(mtx,x,y+18.0f,0.0f);cam=camGetPtr(1);PSMTXConcat((void*)((s32)cam+0x30),mtx,mtx);GXLoadPosMtxImm(mtx,0);GXSetCurrentMtx(0);GXSetCullMode(0);GXClearVtxDesc();GXSetVtxDesc(9,1);GXSetVtxDesc(13,1);GXSetVtxAttrFmt(0,9,1,4,0);GXSetVtxAttrFmt(0,13,1,4,0);
    file=*(void**)((s32)pWin+0x28);tpl=**(void***)((s32)file+0xA0);TEXGetGXTexObjFromPalette(tpl,texObj,7);GXInitTexObjLOD(texObj,0,0,0.0f,0.0f,0.0f,0,0,0);GXLoadTexObj(texObj,0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);h=(f32)GXGetTexObjHeight(texObj);w=(f32)GXGetTexObjWidth(texObj);PSMTXScale(mtx,570.0f/w,320.0f/h,1.0f);GXLoadTexMtxImm(mtx,0x1E,1);
    GXBegin(0x80,0,4);DAT_cc008000=-285.0f;DAT_cc008000=160.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=-285.0f;DAT_cc008000=-160.0f;DAT_cc008000=0.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=0.0f;DAT_cc008000=-160.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=1.0f;DAT_cc008000=0.0f;DAT_cc008000=160.0f;DAT_cc008000=0.0f;DAT_cc008000=1.0f;DAT_cc008000=0.0f;
}

void winKageGX(f32 x, f32 y, f32 z, f32 scale, void* pWin, u32* colorPtr) {
    extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
    extern void GXSetBlendMode(s32 type, s32 src, s32 dst, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevSwapMode(s32 stage, s32 rasSel, s32 texSel);
    extern void GXSetNumTexGens(s32 nGens);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void TEXGetGXTexObjFromPalette(void* tpl, void* texObj, s32 id);
    extern void GXInitTexObjLOD(void* texObj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias,
                                s32 biasClamp, s32 doEdgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetTevColor(s32 reg, void* color);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern s32 GXGetTexObjHeight(void* texObj);
    extern s32 GXGetTexObjWidth(void* texObj);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 angle);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void* camGetPtr(s32 id);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXBegin(s32 prim, s32 vtxFmt, s32 nVerts);
    extern volatile f32 DAT_cc008000;
    extern u32 dat_804232e4;
    extern f32 float_0_80423338;
    extern f32 float_1_80423340;
    extern f32 float_0p5_8042333c;
    extern f32 float_2_80423360;
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winBgMain(void* pWin) {
    extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 time, s32 duration);
    extern void* gp;
    extern f32 float_0_80423338;
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
    extern void winTexInit(void* fileData);
    extern void winTexSet(s32 texId, void* pos, void* size, void* color);
    extern char str_msg_menu_mario_802f47d8[];
    extern u32 dat_804231a8;
    extern u32 dat_804231ac;
    extern u32 dat_804231b0;
    extern u32 dat_804231b4;
    extern f32 float_2_80423360;

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

u8 winSortMain(void* pWin) {
    extern void psndSFXOn(s32 soundId);
    extern char* msgSearch(char* msg);
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

u8 winMsgMain(void* pWin) {
    extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 time, s32 duration);
    extern u32 keyGetDirTrg(s32 controller);
    extern void psndSFXOn(s32 soundId);
    extern void* gp;
    extern f32 float_neg800_8042347c;
    extern f32 float_neg170_80423480;
    extern f32 float_0_80423338;
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

u8 sort_4_3_func(void* pWin) {
    extern u8 itemDataTable[];
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

u8 sort_5_3_func(void* pWin) {
    extern u8 itemDataTable[];
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

u8 winZClear(void) {
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetAlphaUpdate(s32 update);
    extern void GXSetColorUpdate(s32 update);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetBlendMode(s32 type, s32 srcFact, s32 dstFact, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern f32 float_0_80423338;
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

s32 compare_func4(void* param_1, void* param_2) {
    extern void* mailGetPtr(s32 id);
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
    extern void* mailGetPtr(s32 id);
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

void winHalfBookGX(double x, double y, void* file, s32 side) {
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetCullMode(s32); extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, void* pos, void* scale, void* color);
    f32 pos[3];
    f32 scale[3];
    u32 white = 0xFFFFFFFF;
    u32 shade = 0xB0B0B0FF;
    u32 fog = 0;
    s32 row;
    s32 column;

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
    winTexInit(*(void**)file);
    pos[2] = 0.0f;
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    scale[2] = 1.0f;
    for (row = 0; row < 4; row++) {
        for (column = 0; column < 4; column++) {
            pos[0] = (f32)x + column * 56.0f;
            pos[1] = (f32)y - row * 56.0f;
            winTexSet(0x80 + row * 4 + column, pos, scale,
                      side == 0 ? &white : &shade);
        }
    }
    pos[0] = (f32)x + (side == 0 ? 0.0f : 168.0f);
    pos[1] = (f32)y + 12.0f;
    scale[0] = 1.5f;
    scale[1] = 1.5f;
    winTexSet(0x90 + side, pos, scale, &white);
}

void unk_80152e80(s32 cameraId, void* win) {
    extern void* gp;
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
    extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetBlendMode(s32 type, s32 srcFact, s32 dstFact, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void* camGetPtr(s32 cameraId);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXInitTexObj(void* texObj, void* image, u16 width, u16 height, s32 fmt, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern void* gp;
    extern u32 dat_80423334;
    extern f32 float_0_80423338;
    extern f32 float_0p5_8042333c;
    extern f32 float_1_80423340;
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

