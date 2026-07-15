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
    extern void winIconInit(void);
    extern void winIconGrayInit(void);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern void winFontInit(void);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* text);
    extern char* msgSearch(char* key);
    extern u8 itemDataTable[];
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    s32 count = pouchGetHaveBadgeCnt();
    s32 first = *(s32*)((s32)pWin + 0x3EC);
    s32 i;
    s32 badge;
    s32 equipped;

    GXSetScissor(0, 0x77, 0x260, 0xB8);
    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;
    pos.z = 0.0f;
    for (i = first; i < count && i < first + 8; i++) {
        badge = pouchHaveBadge(i);
        equipped = pouchEquipCheckBadgeIndex(i);
        pos.x = (f32)x - 210.0f + ((i - first) & 1) * 220.0f;
        pos.y = (f32)y + 75.0f - ((i - first) >> 1) * 42.0f;
        if (equipped == 0) winIconGrayInit();
        else winIconInit();
        winIconSet(badge, &pos, &scale, &white);
        winFontInit();
        pos.x += 36.0f;
        winFontSet(&pos, &scale, &white,
                   msgSearch(*(char**)((s32)itemDataTable + badge * 0x28 + 8)));
    }
    GXSetScissor(0, 0, 0x260, 0x1E0);
}

void winBadgeDisp(s32 cameraId, void* pWin, s32 index) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void* pouchGetPtr(void);
    extern void winBgGX(f32 x, f32 y, void* win, s32 type);
    extern void winKirinukiGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void badge_disp(f32 x, f32 y, void* win);
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void winFontInit(void);
    extern void winFontSetR(Vec3* pos, Vec3* scale, void* color, char* format, ...);
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    void* pouch = pouchGetPtr();
    f32 x = *(f32*)((s32)pWin + 0x30 + index * 0x18);
    f32 y = *(f32*)((s32)pWin + 0x34 + index * 0x18);
    s32 tab;
    s32 bp;

    winBgGX(x, y, pWin, 3);
    winKirinukiGX(x - 150.0f, y + 120.0f, 415.0f, 210.0f, pWin, 0);
    winTexInit(**(void***)((s32)*(void**)((s32)pWin + 0x28) + 0xA0));
    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;
    pos.z = 0.0f;
    for (tab = 0; tab < 2; tab++) {
        pos.x = x - 125.0f + tab * 130.0f;
        pos.y = y + 130.0f;
        winTexSet(0x1A + tab, &pos, &scale, &white);
    }
    badge_disp(x, y, pWin);
    bp = *(s32*)((s32)pouch + 0x130);
    winFontInit();
    pos.x = x + 140.0f;
    pos.y = y - 112.0f;
    winFontSetR(&pos, &scale, &white, "%d", bp);
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
