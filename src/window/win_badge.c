#include "window/win_badge.h"

extern f32 float_184_804236f8;
extern f32 float_0p25_804236fc;
void* pouchGetPtr(void);
s32 pouchGetHaveBadgeCnt(void);
void winFontInit(void);
char* winZenkakuStr(s32 value);
char* msgSearch(char* key);
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off
void winMakeEquipList(void* pWin) {
    void* pouch;
    void* out;
    s32 index;
    s32 one;
    s32 i;
    s32 count;

    count = 0;
    pouch = pouchGetPtr();
    out = pWin;
    index = 0;
    one = 1;

    for (index = 0; index < 200; index++) {
        if (*(s16*)((s32)pouch + 0x38A + index * 2) != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = *(s16*)((s32)pouch + 0x38A + index * 2);
            out = (void*)((s32)out + 0xC);
        }
    }
    *(s32*)((s32)pWin + 0xD64) = count;
}
#pragma optimize_for_size reset

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off
void winBadgeInit(void* pWin) {
    void* pouch;
    void* out;
    s32 count = 0;
    s32 index;
    s32 one = 1;
    s32 i;

    *(s32*)((s32)pWin + 0x3DC) = 0;
    *(s32*)((s32)pWin + 0x3E0) = 0;
    *(s32*)((s32)pWin + 0x3E4) = 0;
    *(s32*)((s32)pWin + 0x3EC) = 0;
    *(s32*)((s32)pWin + 0x3E8) = 0;
    *(s32*)((s32)pWin + 0x3F0) = 0;

    pouch = pouchGetPtr();
    out = pWin;
    index = 0;
    for (index = 0; index < 200; index++) {
        if (*(s16*)((s32)pouch + 0x38A + index * 2) != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = *(s16*)((s32)pouch + 0x38A + index * 2);
            out = (void*)((s32)out + 0xC);
        }
    }
    *(s32*)((s32)pWin + 0xD64) = count;
}
#pragma optimize_for_size reset

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma optimize_for_size off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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
    for (index = 0; index < 200; index++) {
        if (*(s16*)((s32)pouch + 0x38A + index * 2) != 0) {
            *(s32*)((s32)out + 0x404) = one;
            count++;
            *(s32*)((s32)out + 0x408) = index;
            *(s32*)((s32)out + 0x40C) = *(s16*)((s32)pouch + 0x38A + index * 2);
            out = (void*)((s32)out + 0xC);
        }
    }
    *(s32*)((s32)pWin + 0xD64) = count;
    return 0;
}
#pragma optimize_for_size reset
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

void winBadgeExit(void) {
    ;
}

u8 mario_change(void* pWin) {
    extern s32 pouchEquipCheckBadge(s32 badgeId);
    extern void animPoseRelease(s32 poseId);
    extern s32 animPoseEntry(char* name, s32 mode);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 force);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern char vec3_802f5250[];
    extern char str_S_2_8042370c[4];
    extern char str_W_15_80423710[5];
    extern char str_S_3_80423718[4];
    extern char str_W_17_8042371c[5];
    extern char str_S_4_80423724[4];
    extern char str_W_18_80423728[5];
    extern char str_S_1_80423730[4];
    extern char str_W_1_80423734[4];
    extern char str_M_S_1_80423738[6];
    extern char str_M_W_1_80423740[6];
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
        if (badgeL != 0 && badgeW == 0) base += 0x2A4;
        else if (badgeL == 0 && badgeW != 0) base += 0x2B0;
        else if (badgeL != 0 && badgeW != 0) base += 0x2BC;
        else base += 0x2C8;
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

s32 winBadgeMain(void* pWin) {
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
    extern void winMsgEntry(void*, s32, char*, s32);
    extern u8 itemDataTable[];
    extern char str_msg_menu_badge_all_802f54c0[];
    extern char str_msg_menu_badge_equip_802f54d4[];
    void* pouch = pouchGetPtr();
    s32 state = *(s32*)((s32)pWin + 0x3DC);
    s32 sub;
    u32 pressed;
    s32 count;
    s32* cursor;
    s32* page;

    switch (state) {
    case 0: {
        pressed = *(u32*)((s32)pWin + 4);
        if (pressed & 0x200) { psndSFXOn(0x20013); return -1; }
        if (pressed & 0x1000) return -2;
        if ((*(u32*)((s32)pWin + 0x10) & 0x3000) != 0) {
            sub = *(s32*)((s32)pWin + 0x3E0);
            sub = 1 - sub;
            *(s32*)((s32)pWin + 0x3E0) = sub;
            psndSFXOn(0x20005);
            if (sub != 0) {
                s32 countEquipped = 0;
                s32 badgeIndex = 0;
                s32 group;
                u8* pouchEntry = (u8*)pouchGetPtr();
                u8* out = (u8*)pWin;
                s32 one = 1;
                s32 badge;
                for (group = 0; group < 0x28; group++) {
                    badge = *(s16*)(pouchEntry + 0x38A);
                    if (badge != 0) {
                        *(s32*)(out + 0x404) = one;
                        countEquipped++;
                        *(s32*)(out + 0x408) = badgeIndex;
                        *(s32*)(out + 0x40C) = *(s16*)(pouchEntry + 0x38A);
                        out += 0xC;
                    }
                    badge = *(s16*)(pouchEntry + 0x38C);
                    badgeIndex++;
                    if (badge != 0) {
                        *(s32*)(out + 0x404) = one;
                        countEquipped++;
                        *(s32*)(out + 0x408) = badgeIndex;
                        *(s32*)(out + 0x40C) = *(s16*)(pouchEntry + 0x38C);
                        out += 0xC;
                    }
                    badge = *(s16*)(pouchEntry + 0x38E);
                    badgeIndex++;
                    if (badge != 0) {
                        *(s32*)(out + 0x404) = one;
                        countEquipped++;
                        *(s32*)(out + 0x408) = badgeIndex;
                        *(s32*)(out + 0x40C) = *(s16*)(pouchEntry + 0x38E);
                        out += 0xC;
                    }
                    badge = *(s16*)(pouchEntry + 0x390);
                    badgeIndex++;
                    if (badge != 0) {
                        *(s32*)(out + 0x404) = one;
                        countEquipped++;
                        *(s32*)(out + 0x408) = badgeIndex;
                        *(s32*)(out + 0x40C) = *(s16*)(pouchEntry + 0x390);
                        out += 0xC;
                    }
                    badge = *(s16*)(pouchEntry + 0x392);
                    badgeIndex++;
                    if (badge != 0) {
                        *(s32*)(out + 0x404) = one;
                        countEquipped++;
                        *(s32*)(out + 0x408) = badgeIndex;
                        *(s32*)(out + 0x40C) = *(s16*)(pouchEntry + 0x392);
                        out += 0xC;
                    }
                    pouchEntry += 0xA;
                    badgeIndex++;
                }
                *(s32*)((s32)pWin + 0xD64) = countEquipped;
            }
        } else if ((pressed & 0x100) != 0) {
            if (*(s32*)((s32)pWin + 0x3E0) == 0 && pouchGetHaveBadgeCnt() == 0) return 0;
            if (*(s32*)((s32)pWin + 0x3E0) == 1 && *(s32*)((s32)pWin + 0xD64) == 0) return 0;
            psndSFXOn(0x20012);
            *(s32*)((s32)pWin + 0x3DC) = 10;
            return 0;
        } else if ((pressed & 0x10) != 0) {
            winSortEntry(-310.0f, 150.0f, pWin, *(s32*)((s32)pWin + 0x3E0) == 0 ? 2 : 3);
            *(s32*)((s32)pWin + 0x124) = *(s32*)((s32)pWin + 0x3DC);
            psndSFXOn(0x20012);
            *(s32*)((s32)pWin + 0x3DC) = 1000;
        }
        sub = *(s32*)((s32)pWin + 0x3E0);
        *(f32*)((s32)pWin + 0x158) = -270.0f;
        *(f32*)((s32)pWin + 0x15C) = (f32)(130 - sub * 45);
        winMsgEntry(pWin, 0, sub == 0 ? str_msg_menu_badge_all_802f54c0 : str_msg_menu_badge_equip_802f54d4, 0);
        break;
    }
    case 10: {
        count = *(s32*)((s32)pWin + 0x3E0) == 0 ? pouchGetHaveBadgeCnt() : *(s32*)((s32)pWin + 0xD64);
        pressed = *(u32*)((s32)pWin + 4);
        sub = *(s32*)((s32)pWin + 0x3E0);
        cursor = (s32*)((s32)pWin + 0x3E4 + sub * 4);
        page = (s32*)((s32)pWin + 0x3EC + sub * 4);
        if (pressed & 0x100) {
            s32 index = *cursor;
            if (sub == 0) {
                if (pouchEquipCheckBadgeIndex(index) == 0) {
                    s32 badge = pouchHaveBadge(index);
                    if (*(s16*)((s32)pouch + 0x92) < *(s8*)(itemDataTable + badge * 0x28 + 0x1C)) {
                        psndSFXOn(0x20014);
                    } else {
                        psndSFXOn(0x20038);
                        pouchEquipBadgeIndex(index);
                    }
                } else {
                    psndSFXOn(0x20039); pouchUnEquipBadgeIndex(index);
                }
                pouchReviseMarioParam();
                pouchRevisePartyParam();
                ((u8 (*)(void*, s32))mario_change)(pWin, pouchHaveBadge(index));
            } else {
                u8* entry = (u8*)pWin + 0x404 + index * 0xC;
                s32 badgeIndex = *(s32*)(entry + 4);
                if (*(s32*)entry == 0) {
                    s32 badge = pouchHaveBadge(badgeIndex);
                    if (*(s16*)((s32)pouch + 0x92) >= *(s8*)(itemDataTable + badge * 0x28 + 0x1C)) {
                        psndSFXOn(0x20038);
                        pouchEquipBadgeIndex(badgeIndex);
                        *(s32*)entry = 1;
                    }
                } else {
                    psndSFXOn(0x20039);
                    pouchUnEquipBadgeIndex(badgeIndex);
                    *(s32*)entry = 0;
                }
                pouchReviseMarioParam();
                pouchRevisePartyParam();
                ((u8 (*)(void*, s32))mario_change)(pWin, pouchHaveBadge(badgeIndex));
            }
        } else if (pressed & 0x200) {
            psndSFXOn(0x20013); *(s32*)((s32)pWin + 0x3DC) = 0;
        } else if (pressed & 0x1000) return -2;
        else if (*(u32*)((s32)pWin + 8) & 0x40) { if (--*page < 0) *page = 0; *cursor = *page << 3; psndSFXOn(0x20035); }
        else if (*(u32*)((s32)pWin + 8) & 0x20) { s32 pages=(count+7)/8; if (++*page >= pages) *page=pages-1; *cursor=*page<<3; psndSFXOn(0x20035); }
        else if (*(u32*)((s32)pWin + 0x10) & 0x1000) { --*cursor; psndSFXOn(0x20035); }
        else if (*(u32*)((s32)pWin + 0x10) & 0x2000) { ++*cursor; psndSFXOn(0x20035); }
        else if (pressed & 0x10) {
            winSortEntry(-310.0f, 150.0f, pWin, sub == 0 ? 2 : 3);
            *(s32*)((s32)pWin + 0x124) = *(s32*)((s32)pWin + 0x3DC);
            psndSFXOn(0x20012);
            *(s32*)((s32)pWin + 0x3DC) = 1000;
        }
        if (*cursor < 0) *cursor = count - 1;
        if (*cursor >= count) *cursor = 0;
        *page = *cursor / 8;
        *(f32*)((s32)pWin + 0x158) = -110.0f;
        *(f32*)((s32)pWin + 0x15C) = (f32)(110 - (*cursor % 8) * 23);
        if (*(s32*)((s32)pWin + 0x3E0) == 0) {
            s32 badge = pouchHaveBadge(*cursor);
            winMsgEntry(pWin, badge, *(char**)(itemDataTable + badge * 0x28 + 8), 0);
        } else {
            s32 badge = *(s32*)((s32)pWin + 0x40C + *cursor * 0xC);
            winMsgEntry(pWin, badge, *(char**)(itemDataTable + badge * 0x28 + 8), 0);
        }
        break;
    }
    case 1000: {
        if (winSortWait(pWin) == 0) *(s32*)((s32)pWin + 0x3DC) = *(s32*)((s32)pWin + 0x124);
        if (*(u32*)((s32)pWin + 4) & 0x1000) return -2;
    }
    }
    return 0;
}
void winBadgeMain2(void* pWin) {
    *(f32*)((u8*)pWin + 0x3FC) = float_184_804236f8 * (f32)*(s32*)((u8*)pWin + 0x3EC);
    *(f32*)((u8*)pWin + 0x3F4) +=
        (*(f32*)((u8*)pWin + 0x3FC) - *(f32*)((u8*)pWin + 0x3F4)) * float_0p25_804236fc;
    *(f32*)((u8*)pWin + 0x400) = float_184_804236f8 * (f32)*(s32*)((u8*)pWin + 0x3F0);
    *(f32*)((u8*)pWin + 0x3F8) +=
        (*(f32*)((u8*)pWin + 0x400) - *(f32*)((u8*)pWin + 0x3F8)) * float_0p25_804236fc;
}

void winBadgeDisp(s32 cameraId, void* pWin, s32 index) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void winBgGX(f32 x, f32 y, void* win, s32 type);
    extern void winKirinukiGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void badge_disp(f32 x, f32 y, void* win);
    extern void winTexInit(void* data);
    extern void winTexInit_x2(void* data);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void winTexSet_x2(s32 id, s32 frame, Vec3* pos, Vec3* scale, void* color);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* format, ...);
    extern void winFontSetR(Vec3* pos, Vec3* scale, void* color, char* format, ...);
    extern char str_msg_menu_badge_nokor_802f54a8[];
    extern f32 float_4_804236c0;
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    void* pouch = pouchGetPtr();
    f32 x = *(f32*)((u8*)pWin + 0xC4 + index * 0x14);
    f32 y = *(f32*)((u8*)pWin + 0xC8 + index * 0x14);
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
            pos.x -= float_4_804236c0;
            pos.y += float_4_804236c0;
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

void badge_disp(double x, double y, void* pWin) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct DrawRecord {
        Vec3 scale;
        Vec3 position;
        Vec3 argument;
        u32 color;
    } DrawRecord;
    extern void* pouchGetPtr(void);
    extern s32 pouchGetHaveBadgeCnt(void);
    extern s32 pouchHaveBadge(s32);
    extern s32 pouchEquipCheckBadgeIndex(s32);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern void winTexInit(void*);
    extern void winTexSet(s32, Vec3*, Vec3*, void*);
    extern void winIconInit(void);
    extern void winIconGrayInit(void);
    extern void winIconSet(s32, Vec3*, Vec3*, void*);
    extern void winFontInit(void);
    extern void winFontSet(Vec3*, Vec3*, void*, char*, ...);
    extern void winFontSetWidth(Vec3*, Vec3*, void*, f32, char*, ...);
    extern u32 FontGetMessageWidth(char*);
    extern char* msgSearch(char*);
    extern char* winZenkakuStr(s32);
    extern s32 sprintf(char*, char*, ...);
    extern u8 itemDataTable[];
    extern char str_msg_menu_sort_narabi_802f5464[];
    extern char str_msg_menu_badge_hituy_802f547c[];
    extern f32 float_160_80423658;
    extern f32 float_0p7_80423690;
    extern f32 vec3_802f5350;
    extern f32 vec3_802f5368;
    extern f32 vec3_802f5380;
    extern f32 vec3_802f5398;
    extern f32 vec3_802f53b0;
    extern f32 vec3_802f53c8;
    extern f32 vec3_802f53e0;
    extern f32 vec3_802f53f8;
    extern f32 vec3_802f5410;
    extern f32 vec3_802f5428;
    extern f32 vec3_802f5440;
    extern f32 vec3_802f5458;
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    u32 gray = 0x7F7F7FFF;
    s32 mode = *(s32*)((u8*)pWin + 0x3E0);
    s32 count;
    s32 index;
    char cost[64];
    DrawRecord draws[12];

    if (mode == 0) {
        count = pouchGetHaveBadgeCnt();
    } else {
        count = *(s32*)((u8*)pWin + 0xD64);
    }
    GXSetScissor(0, 0x77, 0x260, 0xB8);
    scale.x = scale.y = scale.z = 1.0f;
    pos.z = 0.0f;
    for (index = 0; index < count; index++) {
        s32 badge;
        s32 equipped;
        s32 grayFlag;
        s32 bpCost;
        s32 i;
        f32 scroll;
        f32 rowY;
        u32* color;

        if (mode == 0) {
            badge = pouchHaveBadge(index);
            equipped = pouchEquipCheckBadgeIndex(index);
        } else {
            badge = *(s32*)((u8*)pWin + 0x40C + index * 0xC);
            equipped = *(s32*)((u8*)pWin + 0x404 + index * 0xC);
        }
        scroll = *(f32*)((u8*)pWin + 0x3F4 + mode * 4);
        rowY = (f32)y + 110.0f - (f32)(index * 23) + scroll;
        if (rowY > 240.0f) {
            continue;
        }
        if (rowY < -240.0f) {
            break;
        }

        bpCost = *(s8*)(itemDataTable + badge * 0x28 + 0x1C);
        if (equipped == 0) {
            if (bpCost > *(s16*)((u8*)pouchGetPtr() + 0x92)) {
                grayFlag = 1;
                color = &gray;
            } else {
                grayFlag = 0;
                color = &white;
            }
        } else {
            winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
            scale.x = vec3_802f5350;
            scale.y = scale.z = 1.0f;
            pos.x = (f32)x + 82.0f;
            pos.y = rowY;
            draws[0].scale = scale;
            draws[0].position = pos;
            draws[0].position.x = pos.x;
            draws[0].position.y = pos.y;
            draws[0].argument = draws[0].position;
            draws[0].color = white;
            winTexSet(0x1E, &draws[0].argument, &draws[0].scale, &draws[0].color);
            grayFlag = 0;
            color = &white;
        }
        if (grayFlag == 0) {
            winIconInit();
        } else {
            winIconGrayInit();
        }
        scale.x = scale.y = scale.z = vec3_802f5368;
        pos.x = (f32)x - 75.0f;
        pos.y = rowY;
        draws[1].scale = scale;
        draws[1].position = pos;
        draws[1].position.x = pos.x;
        draws[1].position.y = pos.y;
        draws[1].argument = draws[1].position;
        draws[1].color = white;
        winIconSet(*(s16*)(itemDataTable + badge * 0x28 + 0x20), &draws[1].argument, &draws[1].scale, &draws[1].color);

        winFontInit();
        scale.x = scale.y = scale.z = vec3_802f5380;
        pos.x = (f32)x - 55.0f;
        pos.y = rowY + 12.0f;
        draws[2].scale = scale;
        draws[2].position = pos;
        draws[2].position.x = pos.x;
        draws[2].position.y = pos.y;
        draws[2].argument = draws[2].position;
        draws[2].color = *color;
        winFontSetWidth(&draws[2].argument, &draws[2].scale, &draws[2].color, 170.0f,
                        msgSearch(*(char**)(itemDataTable + badge * 0x28 + 4)));
        sprintf(cost, "%s", winZenkakuStr(bpCost));
        pos.x = (f32)x + 142.0f - (f32)(FontGetMessageWidth(cost) >> 1);
        pos.y = rowY + 12.0f;
        draws[3].scale = scale;
        draws[3].position = pos;
        draws[3].position.x = pos.x;
        draws[3].position.y = pos.y;
        draws[3].argument = draws[3].position;
        draws[3].color = *color;
        winFontSet(&draws[3].argument, &draws[3].scale, &draws[3].color, cost);

        winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
        scale.x = scale.y = scale.z = 1.0f;
        for (i = 0; i < bpCost; i++) {
            pos.x = (f32)x + float_160_80423658 + (f32)(i * 13);
            pos.y = rowY;
            draws[4].scale = scale;
            draws[4].position = pos;
            draws[4].position.x = pos.x;
            draws[4].position.y = pos.y;
            draws[4].argument = draws[4].position;
            draws[4].color = white;
            winTexSet(equipped != 0 ? 0x1C : 0x1D, &draws[4].argument, &draws[4].scale, &draws[4].color);
        }
    }
    GXSetScissor(0, 0, 0x260, 0x1E0);

    if (*(s32*)((u8*)pWin + 0x3EC + mode * 4) > 0) {
        winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
        scale.x = scale.y = scale.z = vec3_802f53c8;
        pos.x = (f32)x + 250.0f;
        pos.y = (f32)y + 151.0f;
        draws[5].scale = scale;
        draws[5].position = pos;
        draws[5].position.x = pos.x;
        draws[5].position.y = pos.y;
        draws[5].argument = draws[5].position;
        draws[5].color = white;
        winTexSet(0x17, &draws[5].argument, &draws[5].scale, &draws[5].color);
        winIconInit();
        scale.x = scale.y = scale.z = vec3_802f53e0;
        pos.y = (f32)y + 133.0f;
        draws[6].scale = scale;
        draws[6].position = pos;
        draws[6].position.x = pos.x;
        draws[6].position.y = pos.y;
        draws[6].argument = draws[6].position;
        draws[6].color = white;
        winIconSet(0x86, &draws[6].argument, &draws[6].scale, &draws[6].color);
    }
    if (*(s32*)((u8*)pWin + 0x3EC + mode * 4) * 8 + 8 < count) {
        winTexInit(**(void***)((u8*)*(void**)((u8*)pWin + 0x28) + 0xA0));
        scale.x = scale.y = scale.z = vec3_802f53f8;
        pos.x = (f32)x + 250.0f;
        pos.y = (f32)y - 85.0f;
        draws[7].scale = scale;
        draws[7].position = pos;
        draws[7].position.x = pos.x;
        draws[7].position.y = pos.y;
        draws[7].argument = draws[7].position;
        draws[7].color = white;
        winTexSet(0x17, &draws[7].argument, &draws[7].scale, &draws[7].color);
        winIconInit();
        scale.x = scale.y = scale.z = vec3_802f5410;
        pos.y = (f32)y - 70.0f;
        draws[8].scale = scale;
        draws[8].position = pos;
        draws[8].position.x = pos.x;
        draws[8].position.y = pos.y;
        draws[8].argument = draws[8].position;
        draws[8].color = white;
        winIconSet(0x88, &draws[8].argument, &draws[8].scale, &draws[8].color);
    }

    {
        char* text = msgSearch(str_msg_menu_sort_narabi_802f5464);
        f32 textWidth = (f32)(FontGetMessageWidth(text) & 0xFFFF) * float_0p7_80423690;
        winFontInit();
        scale.x = scale.y = scale.z = vec3_802f5428;
        pos.x = (f32)x + 220.0f - textWidth;
        pos.y = (f32)y - 82.0f;
        draws[9].scale = scale;
        draws[9].position = pos;
        draws[9].position.x = pos.x;
        draws[9].position.y = pos.y;
        draws[9].argument = draws[9].position;
        draws[9].color = white;
        winFontSet(&draws[9].argument, &draws[9].scale, &draws[9].color, text);
        winIconInit();
        scale.x = scale.y = scale.z = vec3_802f5440;
        pos.x -= 30.0f;
        pos.y -= 8.0f;
        draws[10].scale = scale;
        draws[10].position = pos;
        draws[10].position.x = pos.x;
        draws[10].position.y = pos.y;
        draws[10].argument = draws[10].position;
        draws[10].color = white;
        winIconSet(0x219, &draws[10].argument, &draws[10].scale, &draws[10].color);
    }
    winFontInit();
    scale.x = scale.y = scale.z = vec3_802f5458;
    pos.x = (f32)x + 120.0f;
    pos.y = (f32)y + 135.0f;
    draws[11].scale = scale;
    draws[11].position = pos;
    draws[11].position.x = pos.x;
    draws[11].position.y = pos.y;
    draws[11].argument = draws[11].position;
    draws[11].color = white;
    winFontSet(&draws[11].argument, &draws[11].scale, &draws[11].color, msgSearch(str_msg_menu_badge_hituy_802f547c));
}
