#include "window/win_badge.h"

extern f32 float_184_804236f8;
extern f32 float_0p25_804236fc;
void* pouchGetPtr(void);

void winBadgeExit(void) {
    ;
}


void badge_disp(double x, double y, void* pWin) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern s32 pouchGetHaveBadgeCnt(void);
    extern s32 pouchHaveBadge(s32 index);
    extern s32 pouchEquipCheckBadgeIndex(s32 index);
    extern void GXSetScissor(s32 x, s32 y, s32 w, s32 h);
    extern void winTexInit(void* texture);
    extern void winTexSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern void winIconInit(void);
    extern void winIconGrayInit(void);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern void winFontInit(void);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* text, ...);
    extern void winFontSetWidth(Vec3* pos, Vec3* scale, void* color, f32 width,
                                char* text, ...);
    extern u32 FontGetMessageWidth(char* text);
    extern char* msgSearch(char* key);
    extern char* winZenkakuStr(s32 value);
    extern s32 sprintf(char* dst, char* format, ...);
    extern u8 itemDataTable[];
    extern char str_msg_menu_sort_narabi_802f5464[];
    extern char str_msg_menu_badge_hituy_802f547c[];
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    u32 gray = 0x7F7F7FFF;
    s32 mode = *(s32*)((u8*)pWin + 0x3E0);
    s32 count;
    s32 index;
    s32 badge;
    s32 equipped;
    f32 scroll;
    f32 row_y;
    char cost[64];

    if (mode == 0) {
        count = pouchGetHaveBadgeCnt();
    } else {
        count = *(s32*)((u8*)pWin + 0xD64);
    }

    GXSetScissor(0, 0x77, 0x260, 0xB8);
    scale.x = scale.y = scale.z = 1.0f;
    pos.z = 0.0f;
    for (index = 0; index < count; index++) {
        if (mode == 0) {
            badge = pouchHaveBadge(index);
            equipped = pouchEquipCheckBadgeIndex(index);
        } else {
            badge = *(s32*)((u8*)pWin + 0x40C + index * 0xC);
            equipped = *(s32*)((u8*)pWin + 0x404 + index * 0xC);
        }
        scroll = *(f32*)((u8*)pWin + 0x3F4 + mode * 4);
        row_y = (f32)y + 110.0f - (f32)(index * 23) + scroll;
        if (row_y > 240.0f) {
            continue;
        }
        if (row_y < -240.0f) {
            break;
        }

        if (equipped == 0) {
            winIconInit();
        } else {
            winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
            pos.x = (f32)x + 82.0f;
            pos.y = row_y;
            winTexSet(0x1E, &pos, &scale, &white);
            winIconInit();
        }

        pos.x = (f32)x - 75.0f;
        pos.y = row_y;
        winIconSet(*(s32*)(itemDataTable + badge * 0x28 + 0x28), &pos, &scale, &white);

        winFontInit();
        pos.x = (f32)x - 55.0f;
        pos.y = row_y + 12.0f;
        winFontSetWidth(&pos, &scale, equipped ? &white : &gray, 170.0f,
                        msgSearch(*(char**)(itemDataTable + badge * 0x28 + 8)));

        sprintf(cost, "%s", winZenkakuStr(*(s8*)(itemDataTable + badge * 0x28 + 0x24)));
        pos.x = (f32)x + 142.0f - (f32)(FontGetMessageWidth(cost) >> 1);
        pos.y = row_y + 12.0f;
        winFontSet(&pos, &scale, equipped ? &white : &gray, cost);
    }
    GXSetScissor(0, 0, 0x260, 0x1E0);

    if (*(s32*)((u8*)pWin + 0x3EC + mode * 4) > 0) {
        winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
        pos.x = (f32)x + 250.0f;
        pos.y = (f32)y + 151.0f;
        winTexSet(0x17, &pos, &scale, &white);
        winIconInit();
        pos.y = (f32)y + 133.0f;
        winIconSet(0x86, &pos, &scale, &white);
    }
    if (*(s32*)((u8*)pWin + 0x3EC + mode * 4) * 8 + 8 < count) {
        winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
        pos.x = (f32)x + 250.0f;
        pos.y = (f32)y - 85.0f;
        winTexSet(0x17, &pos, &scale, &white);
        winIconInit();
        pos.y = (f32)y - 70.0f;
        winIconSet(0x88, &pos, &scale, &white);
    }

    winFontInit();
    pos.x = (f32)x + 220.0f;
    pos.y = (f32)y - 82.0f;
    winFontSet(&pos, &scale, &white, msgSearch(str_msg_menu_sort_narabi_802f5464));
    winIconInit();
    pos.x -= 30.0f;
    pos.y -= 8.0f;
    winIconSet(0x219, &pos, &scale, &white);
    winFontInit();
    pos.x = (f32)x + 120.0f;
    pos.y = (f32)y + 135.0f;
    winFontSet(&pos, &scale, &white, msgSearch(str_msg_menu_badge_hituy_802f547c));
}

void winBadgeDisp(s32 cameraId, void* pWin, s32 index) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void* pouchGetPtr(void);
    extern void winBgGX(f32 x, f32 y, void* win, s32 type);
    extern void winKirinukiGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void badge_disp(f32 x, f32 y, void* win);
    extern void winTexInit(void* data);
    extern void winTexInit_x2(void* data);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void winTexSet_x2(s32 id, s32 frame, Vec3* pos, Vec3* scale, void* color);
    extern void winFontInit(void);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* format, ...);
    extern void winFontSetR(Vec3* pos, Vec3* scale, void* color, char* format, ...);
    extern char* winZenkakuStr(s32 value);
    extern char* msgSearch(char* key);
    extern char str_msg_menu_badge_nokor_802f54a8[];
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    void* pouch = pouchGetPtr();
    f32 x = *(f32*)((u8*)pWin + 0x30 + index * 0x18);
    f32 y = *(f32*)((u8*)pWin + 0x34 + index * 0x18);
    s32 active = *(s32*)((u8*)pWin + 0x3E0);
    s32 total = *(s16*)((u8*)pouch + 0x94);
    s32 available = *(s16*)((u8*)pouch + 0x92);
    s32 tab;
    s32 i;

    winBgGX(x, y, pWin, 3);
    winKirinukiGX(x - 100.0f, y + 140.0f, 365.0f, 210.0f, pWin, 0);

    scale.x = scale.y = scale.z = 1.0f;
    pos.z = 0.0f;
    for (tab = 0; tab < 2; tab++) {
        pos.x = x - 190.0f;
        pos.y = y + 130.0f - (f32)(tab * 45);
        if (tab == active) {
            winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
            winTexSet(0xB4, &pos, &scale, &white);
            winTexInit_x2(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
            winTexSet_x2(tab + 0x1A, 0xB4, &pos, &scale, &white);
        } else {
            winTexInit_x2(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
            winTexSet_x2(tab + 0x1A, 0xB4, &pos, &scale, &white);
        }
    }

    winKirinukiGX(x - 260.0f, y + 60.0f, 150.0f,
                  (f32)((10 - ((total + 9) / 10)) * 13), pWin, 1);

    winFontInit();
    pos.x = x - 220.0f;
    pos.y = y + 56.0f;
    winFontSet(&pos, &scale, &white, "BP");

    pos.x = x - 193.0f;
    winFontSetR(&pos, &scale, &white, "%s", winZenkakuStr(total));

    pos.x = x - 180.0f;
    pos.y = y - 106.0f + (f32)((10 - ((total + 9) / 10)) * 13);
    winFontSet(&pos, &scale, &white, msgSearch(str_msg_menu_badge_nokor_802f54a8));

    pos.x = x - 165.0f;
    pos.y = y - 100.0f + (f32)((10 - ((total + 9) / 10)) * 13);
    winFontSetR(&pos, &scale, &white, "%s", winZenkakuStr(available));

    winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
    for (i = 0; i < available; i++) {
        pos.x = x - 242.0f + (f32)((i % 10) * 13);
        pos.y = y + 30.0f - (f32)((i / 10) * 13);
        winTexSet(0x1C, &pos, &scale, &white);
    }
    for (; i < total; i++) {
        pos.x = x - 242.0f + (f32)((i % 10) * 13);
        pos.y = y + 30.0f - (f32)((i / 10) * 13);
        winTexSet(0x1D, &pos, &scale, &white);
    }

    badge_disp(x, y, pWin);
    (void)cameraId;
}

s32 winBadgeMain(void* pWin) {
    extern void* pouchGetPtr(void);
    extern s32 pouchGetHaveBadgeCnt(void);
    extern s32 pouchEquipCheckBadgeIndex(s32);
    extern s32 pouchHaveBadge(s32);
    extern void pouchEquipBadgeIndex(s32);
    extern void pouchUnEquipBadgeIndex(s32);
    extern void pouchReviseMarioParam(void);
    extern void pouchRevisePartyParam(void);
    extern void psndSFXOn(s32);
    extern u8 mario_change(void*);
    extern void winSortEntry(f32,f32,void*,s32);
    extern s32 winSortWait(void*);
    s32 state = *(s32*)((s32)pWin + 0x3DC);
    s32 sub = *(s32*)((s32)pWin + 0x3E0);
    u32 pressed = *(u32*)((s32)pWin + 4);
    u32 repeat = *(u32*)((s32)pWin + 8);
    u32 dirs = *(u32*)((s32)pWin + 0x10);
    s32 count = sub ? *(s32*)((s32)pWin + 0xD64) : pouchGetHaveBadgeCnt();
    s32* cursor = (s32*)((s32)pWin + 0x3E4 + sub * 4);
    s32* page = (s32*)((s32)pWin + 0x3EC + sub * 4);

    if (state == 0) {
        if (pressed & 0x200) { psndSFXOn(0x20013); return -1; }
        if (pressed & 0x1000) return -2;
        if ((dirs & 0x3000) != 0) {
            sub = 1 - sub;
            *(s32*)((s32)pWin + 0x3E0) = sub;
            psndSFXOn(0x20005);
        } else if ((pressed & 0x100) != 0) {
            if ((sub == 0 && count == 0) || (sub == 1 && *(s32*)((s32)pWin + 0xD64) == 0)) return 0;
            psndSFXOn(0x20012);
            *(s32*)((s32)pWin + 0x3DC) = 10;
            return 0;
        } else if ((pressed & 0x10) != 0) {
            winSortEntry(-310.0f, 150.0f, pWin, sub == 0 ? 2 : 3);
            *(s32*)((s32)pWin + 0x124) = state;
            psndSFXOn(0x20012);
            *(s32*)((s32)pWin + 0x3DC) = 1000;
        }
        *(f32*)((s32)pWin + 0x158) = -270.0f;
        *(f32*)((s32)pWin + 0x15C) = (f32)(130 - sub * 45);
    } else if (state == 10) {
        if (pressed & 0x100) {
            s32 index = *cursor;
            if (sub == 0) {
                if (pouchEquipCheckBadgeIndex(index) == 0) {
                    psndSFXOn(0x20038); pouchEquipBadgeIndex(index);
                } else {
                    psndSFXOn(0x20039); pouchUnEquipBadgeIndex(index);
                }
            }
            pouchReviseMarioParam(); pouchRevisePartyParam(); pouchHaveBadge(index); mario_change(pWin);
        } else if (pressed & 0x200) {
            psndSFXOn(0x20013); *(s32*)((s32)pWin + 0x3DC) = 0;
        } else if (pressed & 0x1000) return -2;
        else if (repeat & 0x40) { if (--*page < 0) *page = 0; *cursor = *page << 3; psndSFXOn(0x20035); }
        else if (repeat & 0x20) { s32 pages=(count+7)/8; if (++*page >= pages) *page=pages-1; *cursor=*page<<3; psndSFXOn(0x20035); }
        else if (dirs & 0x1000) { --*cursor; psndSFXOn(0x20035); }
        else if (dirs & 0x2000) { ++*cursor; psndSFXOn(0x20035); }
        if (count > 0) { if (*cursor < 0) *cursor = count - 1; if (*cursor >= count) *cursor = 0; *page = *cursor / 8; }
        *(f32*)((s32)pWin + 0x158) = -110.0f;
        *(f32*)((s32)pWin + 0x15C) = (f32)(110 - (*cursor & 7) * 23);
    } else if (state == 1000) {
        if (winSortWait(pWin) == 0) *(s32*)((s32)pWin + 0x3DC) = *(s32*)((s32)pWin + 0x124);
        if (pressed & 0x1000) return -2;
    }
    return 0;
}

u8 mario_change(void* pWin) {
    extern s32 pouchEquipCheckBadge(s32 badgeId);
    extern void animPoseRelease(s32 poseId);
    extern s32 animPoseEntry(char* name, s32 mode);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 force);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern char vec3_802f5250[];
    extern char str_S_2_8042370c[];
    extern char str_W_15_80423710[];
    extern char str_S_3_80423718[];
    extern char str_W_17_8042371c[];
    extern char str_S_4_80423724[];
    extern char str_W_18_80423728[];
    extern char str_S_1_80423730[];
    extern char str_W_1_80423734[];
    extern char str_M_S_1_80423738[];
    extern char str_M_W_1_80423740[];
    s32 badgeL;
    s32 badgeW;
    char* base;
    char* animName;
    char* mapAnimName;

    base = vec3_802f5250;
    badgeL = pouchEquipCheckBadge(0x147);
    badgeW = pouchEquipCheckBadge(0x148);
    if ((*(u16*)pWin & 0x2000) != 0) {
        if (badgeL != 0 && badgeW == 0) {
            animName = str_S_2_8042370c;
            mapAnimName = str_W_15_80423710;
        } else if (badgeL == 0 && badgeW != 0) {
            animName = str_S_3_80423718;
            mapAnimName = str_W_17_8042371c;
        } else if (badgeL != 0 && badgeW != 0) {
            animName = str_S_4_80423724;
            mapAnimName = str_W_18_80423728;
        } else {
            animName = str_S_1_80423730;
            mapAnimName = str_W_1_80423734;
        }
        if (*(s32*)((s32)pWin + 0x188) != -1) {
            animPoseRelease(*(s32*)((s32)pWin + 0x188));
            *(s32*)((s32)pWin + 0x188) = animPoseEntry(base + 0x29C, 0);
            animPoseSetAnim(*(s32*)((s32)pWin + 0x188), animName, 1);
            animPoseSetMaterialFlagOn(*(s32*)((s32)pWin + 0x188), 0x1800);
        }
        if (*(s32*)((s32)pWin + 0xE1C) != -1) {
            animPoseRelease(*(s32*)((s32)pWin + 0xE1C));
            *(s32*)((s32)pWin + 0xE1C) = animPoseEntry(base + 0x29C, 0);
            animPoseSetAnim(*(s32*)((s32)pWin + 0xE1C), mapAnimName, 1);
            animPoseSetMaterialFlagOn(*(s32*)((s32)pWin + 0xE1C), 0x1800);
        }
    } else {
        if (badgeL != 0 && badgeW == 0) {
            base += 0x2A4;
        } else if (badgeL == 0 && badgeW != 0) {
            base += 0x2B0;
        } else if (badgeL != 0 && badgeW != 0) {
            base += 0x2BC;
        } else {
            base += 0x2C8;
        }
        if (*(s32*)((s32)pWin + 0x188) != -1) {
            animPoseRelease(*(s32*)((s32)pWin + 0x188));
            *(s32*)((s32)pWin + 0x188) = animPoseEntry(base, 0);
            animPoseSetAnim(*(s32*)((s32)pWin + 0x188), str_M_S_1_80423738, 1);
            animPoseSetMaterialFlagOn(*(s32*)((s32)pWin + 0x188), 0x1800);
        }
        if (*(s32*)((s32)pWin + 0xE1C) != -1) {
            animPoseRelease(*(s32*)((s32)pWin + 0xE1C));
            *(s32*)((s32)pWin + 0xE1C) = animPoseEntry(base, 0);
            animPoseSetAnim(*(s32*)((s32)pWin + 0xE1C), str_M_W_1_80423740, 1);
            animPoseSetMaterialFlagOn(*(s32*)((s32)pWin + 0xE1C), 0x1800);
        }
    }
    return 0;
}

u8 winBadgeInit2(void* pWin) {
    extern f32 float_neg270_80423704;
    extern char str_msg_menu_badge_all_802f54c0[];
    extern char str_msg_menu_badge_equip_802f54d4[];
    extern void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);
    void* pouch;
    void* out;
    s32 count;
    s32 index;
    s32 one;
    s32 i;
    s16 badge;
    f32 value;
    char* msg;

    msg = str_msg_menu_badge_equip_802f54d4;
    *(f32*)((s32)pWin + 0x158) = float_neg270_80423704;
    *(f32*)((s32)pWin + 0x15C) = (f32)(0x7D - *(s32*)((s32)pWin + 0x3E0) * 0x28);
    value = float_184_804236f8 * (f32)*(s32*)((s32)pWin + 0x3EC);
    *(f32*)((s32)pWin + 0x3FC) = value;
    *(f32*)((s32)pWin + 0x3F4) = value;
    value = float_184_804236f8 * (f32)*(s32*)((s32)pWin + 0x3F0);
    *(f32*)((s32)pWin + 0x400) = value;
    *(f32*)((s32)pWin + 0x3F8) = value;
    if (*(s32*)((s32)pWin + 0x3E0) == 0) {
        msg = str_msg_menu_badge_all_802f54c0;
    }
    winMsgEntry(pWin, 0, msg, 0);

    count = 0;
    pouch = pouchGetPtr();
    out = pWin;
    index = 0;
    one = 1;
    for (i = 0; i < 0x28; i++) {
        badge = *(s16*)((s32)pouch + 0x38A);
        if (badge != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = badge;
            out = (void*)((s32)out + 0xC);
        }
        badge = *(s16*)((s32)pouch + 0x38C);
        index++;
        if (badge != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = badge;
            out = (void*)((s32)out + 0xC);
        }
        badge = *(s16*)((s32)pouch + 0x38E);
        index++;
        if (badge != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = badge;
            out = (void*)((s32)out + 0xC);
        }
        badge = *(s16*)((s32)pouch + 0x390);
        index++;
        if (badge != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = badge;
            out = (void*)((s32)out + 0xC);
        }
        badge = *(s16*)((s32)pouch + 0x392);
        index++;
        if (badge != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = badge;
            out = (void*)((s32)out + 0xC);
        }
        pouch = (void*)((s32)pouch + 0xA);
        index++;
    }
    *(s32*)((s32)pWin + 0xD64) = count;
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winBadgeInit(void* pWin) {
    void* pouch;
    void* out;
    s32 count = 0;
    s32 index;
    s32 one = 1;
    s32 i;
    s16 badge;

    *(s32*)((s32)pWin + 0x3DC) = 0;
    *(s32*)((s32)pWin + 0x3E0) = 0;
    *(s32*)((s32)pWin + 0x3E4) = 0;
    *(s32*)((s32)pWin + 0x3EC) = 0;
    *(s32*)((s32)pWin + 0x3E8) = 0;
    *(s32*)((s32)pWin + 0x3F0) = 0;

    pouch = pouchGetPtr();
    out = pWin;
    index = 0;
    for (i = 0; i < 0x28; i++) {
        badge = *(s16*)((s32)pouch + 0x38A);
        if (badge != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = badge;
            out = (void*)((s32)out + 0xC);
        }
        badge = *(s16*)((s32)pouch + 0x38C);
        index++;
        if (badge != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = badge;
            out = (void*)((s32)out + 0xC);
        }
        badge = *(s16*)((s32)pouch + 0x38E);
        index++;
        if (badge != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = badge;
            out = (void*)((s32)out + 0xC);
        }
        badge = *(s16*)((s32)pouch + 0x390);
        index++;
        if (badge != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = badge;
            out = (void*)((s32)out + 0xC);
        }
        badge = *(s16*)((s32)pouch + 0x392);
        index++;
        if (badge != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = badge;
            out = (void*)((s32)out + 0xC);
        }
        pouch = (void*)((s32)pouch + 0xA);
        index++;
    }
    *(s32*)((s32)pWin + 0xD64) = count;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winMakeEquipList(void* pWin) {
    void* pouch = pouchGetPtr();
    void* out = pWin;
    s32 index = 0;
    s32 one = 1;
    s32 i;
    s32 count = 0;

    for (i = 0; i < 0x28; i++) {
        if (*(s16*)((s32)pouch + 0x38A) != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = *(s16*)((s32)pouch + 0x38A);
            out = (void*)((s32)out + 0xC);
        }
        if (*(s16*)((s32)pouch + 0x38C) != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            index++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = *(s16*)((s32)pouch + 0x38C);
            out = (void*)((s32)out + 0xC);
        } else {
            index++;
        }
        if (*(s16*)((s32)pouch + 0x38E) != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            index++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = *(s16*)((s32)pouch + 0x38E);
            out = (void*)((s32)out + 0xC);
        } else {
            index++;
        }
        if (*(s16*)((s32)pouch + 0x390) != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            index++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = *(s16*)((s32)pouch + 0x390);
            out = (void*)((s32)out + 0xC);
        } else {
            index++;
        }
        if (*(s16*)((s32)pouch + 0x392) != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            index++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = *(s16*)((s32)pouch + 0x392);
            out = (void*)((s32)out + 0xC);
        } else {
            index++;
        }
        pouch = (void*)((s32)pouch + 0xA);
        index++;
    }
    *(s32*)((s32)pWin + 0xD64) = count;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void winBadgeMain2(void* pWin) {
    f32 scale = float_184_804236f8;
    s32 xIndex = *(s32*)((s32)pWin + 0x3EC);
    f32 rate = float_0p25_804236fc;
    f32 target;
    f32 current;
    f32 diff;

    target = scale * (f32)xIndex;
    *(f32*)((s32)pWin + 0x3FC) = target;
    target = *(f32*)((s32)pWin + 0x3FC);
    current = *(f32*)((s32)pWin + 0x3F4);
    diff = target - current;
    *(f32*)((s32)pWin + 0x3F4) = diff * rate + current;

    target = scale * (f32)*(s32*)((s32)pWin + 0x3F0);
    *(f32*)((s32)pWin + 0x400) = target;
    target = *(f32*)((s32)pWin + 0x400);
    current = *(f32*)((s32)pWin + 0x3F8);
    diff = target - current;
    *(f32*)((s32)pWin + 0x3F8) = diff * rate + current;
}
