#include "window/win_mario.h"

char tmp2[0x100];
extern const char str_PCTd_80423948[3];

void animPoseRelease(s32 poseId);
s32 sprintf(char* str, const char* fmt, ...);
typedef struct WinMarioLinkEntry {
    s16 x;
    s16 y;
    char pad_04[0x10];
    char* msg;
    char pad_18[4];
} WinMarioLinkEntry;
extern WinMarioLinkEntry linkDt[];
extern char* hammer_help[4];
extern char* boots_help[4];
s32 pouchGetHammerLv(void);
s32 pouchGetJumpLv(void);
void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 winMarioInit(void* pWin) {
    extern void* pouchGetPtr(void);
    extern void* marioGetPtr(void);
    extern s32 marioGetColor(void);
    extern s32 animPoseEntry(char* name, s32 mode);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 force);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern char str_msg_menu_mario_lv_802f5e88[];
    extern char str_S_1_80423890[];
    extern char str_S_2_80423a1c[];
    extern char str_S_3_80423a20[];
    extern char str_S_4_80423a24[];
    extern char str_M_S_1_80423888[];
    void* pouch;
    char* base;
    char* name;
    s32 color;
    s32 count;
    u16 powers;

    base = str_msg_menu_mario_lv_802f5e88;
    pouch = pouchGetPtr();
    if ((*(u16*)pWin & 0x2000) != 0) {
        name = str_S_1_80423890;
        color = marioGetColor();
        if (color == 2) {
            name = str_S_3_80423a20;
        } else if (color < 2) {
            if (color == 0) {
                name = str_S_1_80423890;
            } else if (color >= 0) {
                name = str_S_2_80423a1c;
            }
        } else if (color < 4) {
            name = str_S_4_80423a24;
        }
        *(s32*)((s32)pWin + 0x188) = animPoseEntry(base + 0x848, 0);
        animPoseSetAnim(*(s32*)((s32)pWin + 0x188), name, 1);
    } else {
        name = base + 0x850;
        color = marioGetColor();
        if (color == 2) {
            name = base + 0x864;
        } else if (color < 2) {
            if (color >= 0 && color != 0) {
                name = base + 0x858;
            }
        } else if (color < 4) {
            name = base + 0x870;
        }
        *(s32*)((s32)pWin + 0x188) = animPoseEntry(name, 0);
        animPoseSetAnim(*(s32*)((s32)pWin + 0x188), str_M_S_1_80423888, 1);
    }

    animPoseSetMaterialFlagOn(*(s32*)((s32)pWin + 0x188), 0x1800);
    if ((*(u32*)marioGetPtr() & 0x40000000) != 0) {
        animPoseSetMaterialFlagOn(*(s32*)((s32)pWin + 0x188), 0x0A000000);
    }

    count = 0;
    *(s32*)((s32)pWin + 0x18C) = 0;
    *(s32*)((s32)pWin + 0x190) = 0;
    *(s32*)((s32)pWin + 0x194) = 0;
    *(s32*)((s32)pWin + 0x198) = 0;
    powers = *(u16*)((s32)pouch + 0x8C);
    if ((powers & (1 << 0)) != 0) {
        count = *(s32*)((s32)pWin + 0x198);
        *(s32*)((s32)pWin + 0x198) = count + 1;
    }
    if ((powers & (1 << 1)) != 0) {
        count = *(s32*)((s32)pWin + 0x198);
        *(s32*)((s32)pWin + 0x198) = count + 1;
    }
    if ((powers & (1 << 2)) != 0) {
        count = *(s32*)((s32)pWin + 0x198);
        *(s32*)((s32)pWin + 0x198) = count + 1;
    }
    if ((powers & (1 << 3)) != 0) {
        count = *(s32*)((s32)pWin + 0x198);
        *(s32*)((s32)pWin + 0x198) = count + 1;
    }
    if ((powers & (1 << 4)) != 0) {
        count = *(s32*)((s32)pWin + 0x198);
        *(s32*)((s32)pWin + 0x198) = count + 1;
    }
    if ((powers & (1 << 5)) != 0) {
        count = *(s32*)((s32)pWin + 0x198);
        *(s32*)((s32)pWin + 0x198) = count + 1;
    }
    if ((powers & (1 << 6)) != 0) {
        count = *(s32*)((s32)pWin + 0x198);
        *(s32*)((s32)pWin + 0x198) = count + 1;
    }
    if ((powers & (1 << 7)) != 0) {
        count = *(s32*)((s32)pWin + 0x198);
        *(s32*)((s32)pWin + 0x198) = count + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void winMarioInit2(void* pWin) {
    s32 index;
    f32 value;

    *(s32*)((s32)pWin + 0x160) = 0;
    index = *(s32*)((s32)pWin + 0x160);
    value = (f32)linkDt[index].x;
    *(f32*)((s32)pWin + 0x158) = value;
    *(f32*)((s32)pWin + 0x158) = value;
    index = *(s32*)((s32)pWin + 0x160);
    value = (f32)linkDt[index].y;
    *(f32*)((s32)pWin + 0x15C) = value;
    *(f32*)((s32)pWin + 0x15C) = value;

    switch (*(s32*)((s32)pWin + 0x160)) {
        case 2:
            winMsgEntry(pWin, 0, hammer_help[pouchGetHammerLv()], 0);
            break;
        case 3:
            winMsgEntry(pWin, 0, boots_help[pouchGetJumpLv()], 0);
            break;
        default:
            winMsgEntry(pWin, 0, linkDt[*(s32*)((s32)pWin + 0x160)].msg, 0);
            break;
    }
    *(s32*)((s32)pWin + 0x18C) = 0;
    *(s32*)((s32)pWin + 0x190) = 0;
    *(s32*)((s32)pWin + 0x194) = 0;
}

void winMarioExit(void* wp) {
    animPoseRelease(*(s32*)((s32)wp + 0x188));
}

s32 winMarioMain(void* pWin) {
    extern void psndSFXOn(s32);
    extern s32 pouchCheckItem(s32);
    extern void* superActionTable[];
    extern void winMsgEntry(void*, s32, char*, s32);
    extern u8 DAT_80377708[];
    s32 open = *(s32*)((s32)pWin + 0x190);
    s32 state;
    s32 cursor;
    s32 count;
    u32 pressed;
    u32 dirs;

    if (open == 1) {
        goto star_list;
    }
    if (open > 0 || open < 0) {
        return 0;
    }

    pressed = *(u32*)((s32)pWin + 4);
    dirs = *(u32*)((s32)pWin + 0x10);
    if ((pressed & 0x100) != 0) {
        if (*(s32*)((s32)pWin + 0x160) == 12) {
            *(s32*)((s32)pWin + 0x190) = 1;
            *(s32*)((s32)pWin + 0x194) = 0;
            psndSFXOn(0x20012);
        }
    } else {
        if ((pressed & 0x200) != 0) {
            psndSFXOn(0x20013);
            return -1;
        }
        if ((pressed & 0x1000) != 0) {
            return -2;
        }
        state = *(s32*)((s32)pWin + 0x160);
        if ((dirs & 0x1000) != 0) {
            do {
                state = DAT_80377708[state * 0x1C];
            } while (state != 11 && state != 12 &&
                     *(u16*)(DAT_80377708 - 8 + state * 0x1C) != 0 &&
                     pouchCheckItem(*(u16*)(DAT_80377708 - 8 + state * 0x1C)) == 0);
        }
        if ((dirs & 0x2000) != 0) {
            do {
                state = DAT_80377708[state * 0x1C + 1];
            } while (state != 11 && state != 12 &&
                     *(u16*)(DAT_80377708 - 8 + state * 0x1C) != 0 &&
                     pouchCheckItem(*(u16*)(DAT_80377708 - 8 + state * 0x1C)) == 0);
        }
        if ((dirs & 0x4000) != 0) {
            do {
                state = DAT_80377708[state * 0x1C + 2];
            } while (state != 11 && state != 12 &&
                     *(u16*)(DAT_80377708 - 8 + state * 0x1C) != 0 &&
                     pouchCheckItem(*(u16*)(DAT_80377708 - 8 + state * 0x1C)) == 0);
        }
        if ((dirs & 0x8000) != 0) {
            do {
                state = DAT_80377708[state * 0x1C + 3];
            } while (state != 11 && state != 12 &&
                     *(u16*)(DAT_80377708 - 8 + state * 0x1C) != 0 &&
                     pouchCheckItem(*(u16*)(DAT_80377708 - 8 + state * 0x1C)) == 0);
        }
        *(s32*)((s32)pWin + 0x160) = state;
        if ((dirs & 0xF000) != 0) {
            psndSFXOn(0x20005);
        }
    }

    state = *(s32*)((s32)pWin + 0x160);
    *(f32*)((s32)pWin + 0x158) = (f32)linkDt[state].x;
    *(f32*)((s32)pWin + 0x15C) = (f32)linkDt[state].y;
    if (state == 2) {
        winMsgEntry(pWin, 0, hammer_help[pouchGetHammerLv()], 0);
    } else if (state == 3) {
        winMsgEntry(pWin, 0, boots_help[pouchGetJumpLv()], 0);
    } else {
        winMsgEntry(pWin, 0, linkDt[state].msg, 0);
    }
    return 0;

star_list:
    cursor = *(s32*)((s32)pWin + 0x194);
    count = *(s32*)((s32)pWin + 0x198);
    dirs = *(u32*)((s32)pWin + 0x10);
    pressed = *(u32*)((s32)pWin + 4);
    if ((dirs & 0x1000) != 0) {
        cursor--;
        if (cursor < 0) {
            cursor = count - 1;
        }
        psndSFXOn(0x20005);
    } else if ((dirs & 0x2000) != 0) {
        cursor++;
        if (cursor >= count) {
            cursor = 0;
        }
        psndSFXOn(0x20005);
    } else if ((pressed & 0x200) != 0) {
        *(s32*)((s32)pWin + 0x190) = 0;
        psndSFXOn(0x20013);
    } else if ((pressed & 0x1000) != 0) {
        return -2;
    }
    *(s32*)((s32)pWin + 0x194) = cursor;
    *(f32*)((s32)pWin + 0x158) = -280.0f;
    *(f32*)((s32)pWin + 0x15C) = 118.0f - 26.0f * (f32)cursor;
    winMsgEntry(pWin, 0, *(char**)((s32)superActionTable[cursor] + 0xC), 0);
    return 0;
}

void winMarioMain2(void) {
    ;
}

char* winZenkakuStr(s32 value) {
    sprintf(tmp2, str_PCTd_80423948, value);
    return tmp2;
}

char* unk_801703e8(s32 value, s32 width) {
    extern const char str_PCTPCT0PCTdd_802f6688[];
    extern char unk_803e5f78[];
    char buffer[0x100];

    sprintf(buffer, str_PCTPCT0PCTdd_802f6688, width);
    sprintf(unk_803e5f78, buffer, value);
    return unk_803e5f78;
}

void fukidashi(double x, double y, void* menu, s32 type) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern s32 pouchGetHammerLv(void);
    extern s32 pouchGetJumpLv(void);
    extern s32 pouchCheckItem(s32 item);
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void winIconInit(void);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern u8 itemDataTable[];
    Vec3 p0, p1, p2;
    Vec3 s0, s1, s2;
    u32 c0 = 0xFFFFFFFF;
    u32 c1 = 0xFFFFFFFF;
    u32 c2 = 0xFFFFFFFF;
    s32 level;
    s32 icon;
    f32 bx;
    f32 by;

    s0.x = s0.y = s0.z = 1.0f;
    s1.x = s1.y = s1.z = 1.0f;
    s2.x = s2.y = s2.z = 1.0f;
    p0.z = p1.z = p2.z = 0.0f;

    switch (type) {
        case 2:
            level = pouchGetHammerLv();
            if (level > 0) {
                level = pouchGetHammerLv();
                icon = *(s16*)(itemDataTable + (level + 8) * 0x28 + 0x28);
                winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
                GXSetTevColorIn(0, 0, 0, 0, 2);
                GXSetTevAlphaIn(0, 0, 4, 5, 7);
                bx = (f32)x - 230.0f;
                by = (f32)y + 72.0f;
                p0.x = bx + 5.0f;
                p0.y = by - 5.0f;
                winTexSet(0xB1, &p0, &s0, &c0);
                p1.x = bx;
                p1.y = by;
                winTexSet(0xB1, &p1, &s1, &c1);
                winIconInit();
                p2.x = bx - 6.0f;
                p2.y = by;
                winIconSet(icon, &p2, &s2, &c2);
            }
            break;

        case 3:
            level = pouchGetJumpLv();
            if (level > 0) {
                level = pouchGetJumpLv();
                icon = *(s16*)(itemDataTable + (level + 5) * 0x28 + 0x28);
                winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
                GXSetTevColorIn(0, 0, 0, 0, 2);
                GXSetTevAlphaIn(0, 0, 4, 5, 7);
                bx = (f32)x - 210.0f;
                by = (f32)y + 20.0f;
                p0.x = bx + 5.0f;
                p0.y = by - 5.0f;
                winTexSet(0xB1, &p0, &s0, &c0);
                p1.x = bx;
                p1.y = by;
                winTexSet(0xB1, &p1, &s1, &c1);
                winIconInit();
                p2.x = bx - 6.0f;
                p2.y = by;
                winIconSet(icon, &p2, &s2, &c2);
            }
            break;

        case 7:
            level = pouchCheckItem(2);
            icon = *(s16*)(itemDataTable + 2 * 0x28 + 0x28);
            if (level != 0) {
                winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
                GXSetTevColorIn(0, 0, 0, 0, 2);
                GXSetTevAlphaIn(0, 0, 4, 5, 7);
                bx = (f32)x - 80.0f;
                by = (f32)y + 90.0f;
                p0.x = bx + 5.0f;
                p0.y = by - 5.0f;
                winTexSet(0xB1, &p0, &s0, &c0);
                p1.x = bx;
                p1.y = by;
                winTexSet(0xB1, &p1, &s1, &c1);
                winIconInit();
                p2.x = bx + 6.0f;
                p2.y = by;
                winIconSet(icon, &p2, &s2, &c2);
            }
            break;

        case 8:
            level = pouchCheckItem(4);
            icon = *(s16*)(itemDataTable + 4 * 0x28 + 0x28);
            if (level != 0) {
                winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
                GXSetTevColorIn(0, 0, 0, 0, 2);
                GXSetTevAlphaIn(0, 0, 4, 5, 7);
                bx = (f32)x - 80.0f;
                by = (f32)y + 20.0f;
                p0.x = bx + 5.0f;
                p0.y = by - 5.0f;
                winTexSet(0xB1, &p0, &s0, &c0);
                p1.x = bx;
                p1.y = by;
                winTexSet(0xB1, &p1, &s1, &c1);
                winIconInit();
                p2.x = bx + 6.0f;
                p2.y = by;
                winIconSet(icon, &p2, &s2, &c2);
            }
            break;

        case 9:
            level = pouchCheckItem(5);
            icon = *(s16*)(itemDataTable + 5 * 0x28 + 0x28);
            if (level != 0) {
                winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
                GXSetTevColorIn(0, 0, 0, 0, 2);
                GXSetTevAlphaIn(0, 0, 4, 5, 7);
                bx = (f32)x - 30.0f;
                by = (f32)y + 130.0f;
                p0.x = bx + 5.0f;
                p0.y = by - 5.0f;
                winTexSet(0xB1, &p0, &s0, &c0);
                p1.x = bx;
                p1.y = by;
                winTexSet(0xB1, &p1, &s1, &c1);
                winIconInit();
                p2.x = bx + 6.0f;
                p2.y = by;
                winIconSet(icon, &p2, &s2, &c2);
            }
            break;

        case 10:
            level = pouchCheckItem(3);
            icon = *(s16*)(itemDataTable + 3 * 0x28 + 0x28);
            if (level != 0) {
                winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
                GXSetTevColorIn(0, 0, 0, 0, 2);
                GXSetTevAlphaIn(0, 0, 4, 5, 7);
                bx = (f32)x - 30.0f;
                by = (f32)y + 60.0f;
                p0.x = bx + 5.0f;
                p0.y = by - 5.0f;
                winTexSet(0xB1, &p0, &s0, &c0);
                p1.x = bx;
                p1.y = by;
                winTexSet(0xB1, &p1, &s1, &c1);
                winIconInit();
                p2.x = bx + 6.0f;
                p2.y = by;
                winIconSet(icon, &p2, &s2, &c2);
            }
            break;
    }
}


void winMarioDisp(s32 cameraId, void* pWin, s32 index) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void* pouchGetPtr(void);
    extern void winBgGX(f32 x, f32 y, void* win, s32 type);
    extern void winNameGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void winKirinukiGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void fukidashi(f32 x, f32 y, void* win, s32 type);
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void winIconInit(void);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern void winFontInit(void);
    extern void winFontSetEdge(Vec3* pos, Vec3* scale, void* color, char* text, ...);
    extern void winFontSetEdgeWidth(Vec3* pos, Vec3* scale, void* color, f32 width,
                                    char* text, ...);
    extern void winFontSetR(Vec3* pos, Vec3* scale, void* color, char* format, ...);
    extern char* msgSearch(char* key);
    extern char* BattleGetRankNameLabel(s32 level);
    extern char* winZenkakuStr(s32 value);
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    u8* w = (u8*)pWin;
    u8* pouch = (u8*)pouchGetPtr();
    f32 x = *(f32*)(w + 0xC4 + index * 0x14);
    f32 y = *(f32*)(w + 0xC8 + index * 0x14);
    s32 menuState = *(s32*)(w + 0x160);
    s32 modes[6] = { 2, 3, 7, 8, 9, 10 };
    s32 i;

    scale.x = scale.y = scale.z = 1.0f;
    pos.z = 0.0f;
    winBgGX(x, y, pWin, 0);
    winNameGX(x - 265.0f, y + 154.0f, 190.0f, 24.0f, pWin, 2);

    winFontInit();
    pos.x = x - 255.0f;
    pos.y = y + 152.0f;
    winFontSetEdge(&pos, &scale, &white, msgSearch("name_mario"));
    pos.x += 20.0f;
    winIconInit();
    pos.y = y + 142.0f;
    winIconSet(0x1A6, &pos, &scale, &white);

    pos.x += 20.0f;
    pos.y = y + 152.0f;
    winFontInit();
    winFontSetEdgeWidth(&pos, &scale, &white, 45.0f,
                        msgSearch("msg_menu_mario_level"));
    pos.x = x - 85.0f;
    winFontSetEdgeWidth(&pos, &scale, &white, 20.0f,
                        winZenkakuStr(*(s16*)(pouch + 0x8A)));

    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
    pos.x = x - 255.0f;
    pos.y = y + 112.0f;
    winTexSet(0xB0, &pos, &scale, &white);
    winFontInit();
    pos.x = x - 240.0f;
    pos.y = y + 124.0f;
    winFontSetEdgeWidth(&pos, &scale, &white, 140.0f,
                        msgSearch(BattleGetRankNameLabel(*(s16*)(pouch + 0x8A))));

    for (i = 0; i < 6; i++) {
        if (menuState != modes[i]) {
            fukidashi(x, y, pWin, modes[i]);
        }
    }
    fukidashi(x, y, pWin, menuState);

    winKirinukiGX(x - 270.0f, y - 10.0f, 264.0f, 110.0f, pWin, 0);
    for (i = 0; i < 3; i++) {
        winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
        pos.x = x - 222.0f;
        pos.y = y - 42.0f - (f32)(i * 34);
        winTexSet(0xAF, &pos, &scale, &white);
        winFontInit();
        pos.x += 12.0f;
        pos.y += 12.0f;
        winFontSetEdge(&pos, &scale, &white, msgSearch("msg_menu_mario_status"));
    }

    winIconInit();
    pos.x = x - 160.0f;
    pos.y = y - 72.0f;
    winIconSet(0x1A7, &pos, &scale, &white);
    pos.y -= 34.0f;
    winIconSet(0x1A8, &pos, &scale, &white);

    winFontInit();
    pos.x = x + 215.0f;
    pos.y = y + 44.0f;
    winFontSetR(&pos, &scale, &white, "%s", winZenkakuStr(*(s16*)(pouch + 0x96)));
    pos.y -= 28.0f;
    winFontSetR(&pos, &scale, &white, "%s", winZenkakuStr(*(s16*)(pouch + 0x78)));
    pos.y -= 28.0f;
    winFontSetR(&pos, &scale, &white, "%s", winZenkakuStr(*(s16*)(pouch + 0x9A)));
    pos.y -= 28.0f;
    winFontSetR(&pos, &scale, &white, "%s", winZenkakuStr(*(s16*)(pouch + 0x9C)));
    (void)cameraId;
}
