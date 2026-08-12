#include "window/win_mario.h"

char tmp2[0x100];
extern const char str_PCTd_80423948[3];

void animPoseRelease(s32 poseId);
s32 sprintf(char* str, const char* fmt, ...);
typedef struct Vec3 { f32 x, y, z; } Vec3;
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
void* pouchGetPtr(void);
void winTexInit(void* data);
void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
void winIconInit(void);
void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 winMarioInit(void* pWin) {
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
    extern s32 pouchCheckItem(s32 item);
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
    extern void winBgGX(f32 x, f32 y, void* win, s32 type);
    extern void winNameGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void winKirinukiGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void winFontInit(void);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* text, ...);
    extern void winFontSetEdge(Vec3* pos, Vec3* scale, void* color, char* text, ...);
    extern void winFontSetEdgeWidth(Vec3* pos, Vec3* scale, void* color, f32 width,
                                    char* text, ...);
    extern void winFontSetWidth(Vec3* pos, Vec3* scale, void* color, f32 width,
                                char* text, ...);
    extern void winFontSetR(Vec3* pos, Vec3* scale, void* color, char* format, ...);
    extern u16 FontGetMessageWidth(char* text);
    extern char* msgSearch(char* key);
    extern char* BattleGetRankNameLabel(s32 level);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern void* animPoseGetAnimDataPtr(s32 poseId);
    extern void animPoseSetLocalTime(f64 time, s32 poseId);
    extern void animPoseMain(s32 poseId);
    extern void winKageGX(f32 x, f32 y, f32 z, f32 scale, void* win, void* color);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void winZClear(void);
    extern void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, f32 rot, f32 scale);
    extern void winWazaGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void* gp;
    extern void* superActionTable[];
    extern char unk_803e5f78[];

    u8* w = (u8*)pWin;
    u8* pouch = (u8*)pouchGetPtr();
    f32 x = *(f32*)(w + 0xC4 + index * 0x14);
    f32 y = *(f32*)(w + 0xC8 + index * 0x14);
    s32 menuState = *(s32*)(w + 0x160);
    s32 starCount = *(s32*)(w + 0x198);
    static s32 pri[6] = { 3, 2, 8, 10, 7, 9 };

    Vec3 pName, sName;
    Vec3 pLevelIcon, sLevelIcon;
    Vec3 pLevelLabel, sLevelLabel;
    Vec3 pLevelValue, sLevelValue;
    Vec3 pRankIcon, sRankIcon;
    Vec3 pRankText, sRankText;
    Vec3 pStatusBg, sStatusBg;
    Vec3 pStatusText, sStatusText;
    Vec3 pStatusRuby, sStatusRuby;
    Vec3 pHpIcon, sHpIcon;
    Vec3 pFpIcon, sFpIcon;
    Vec3 pBpIcon, sBpIcon;
    Vec3 pHp, sHp;
    Vec3 pFp, sFp;
    Vec3 pBp, sBp;
    Vec3 pMaxHp, sMaxHp;
    Vec3 pMaxFp, sMaxFp;
    Vec3 pStarBox, sStarBox;
    Vec3 pStarIcon, sStarIcon;
    Vec3 pSpLabel, sSpLabel;
    Vec3 pSpecialText, sSpecialText;

    Vec3 pInfoIcon0, sInfoIcon0;
    Vec3 pInfoIcon1, sInfoIcon1;
    Vec3 pInfoIcon2, sInfoIcon2;
    Vec3 pInfoIcon3, sInfoIcon3;
    Vec3 pInfoIcon4, sInfoIcon4;
    Vec3 pInfoLabel0, sInfoLabel0;
    Vec3 pInfoLabel1, sInfoLabel1;
    Vec3 pInfoLabel2, sInfoLabel2;
    Vec3 pInfoLabel3, sInfoLabel3;
    Vec3 pInfoLabel4, sInfoLabel4;
    Vec3 pInfoValue0, sInfoValue0;
    Vec3 pInfoValue1, sInfoValue1;
    Vec3 pInfoValue2, sInfoValue2;
    Vec3 pInfoValue3, sInfoValue3;
    Vec3 pInfoTimeH, sInfoTimeH;
    Vec3 pInfoTimeM, sInfoTimeM;
    Vec3 pDivider, sDivider;
    Vec3 pDividerEnd, sDividerEnd;

    Vec3 pWazaTitle, sWazaTitle;
    Vec3 pWazaAp, sWazaAp;
    Vec3 pWazaName, sWazaName;
    Vec3 pWazaCost, sWazaCost;
    Vec3 pWazaCostAp, sWazaCostAp;

    f32 marioMtx[3][4];

    u32 white0 = 0xFFFFFFFF;
    u32 white1 = 0xFFFFFFFF;
    u32 white2 = 0xFFFFFFFF;
    u32 white3 = 0xFFFFFFFF;
    u32 white4 = 0xFFFFFFFF;
    u32 white5 = 0xFFFFFFFF;
    u32 white6 = 0xFFFFFFFF;
    u32 white7 = 0xFFFFFFFF;
    u32 white8 = 0xFFFFFFFF;
    u32 white9 = 0xFFFFFFFF;
    u32 black = 0x000000FF;
    u32 shadowColor = 0x00000080;
    char levelText[260];
    char valueText[32];
    char* text;
    f32 nameX;
    f32 baseY;
    f32 width;
    f32 rowY;
    s32 i;
    s32 filledStars;
    s32 poseId;
    s32 animTimer;
    s32 starListOpen;
    s32 cost;
    s32 j;
    s32 animLength;
    u32 retrace;
    u32 timeBase;
    u32 hours;
    u32 minutes;
    u64 elapsed;
    void* animData;
    void* animTrack;
    char hourFmt[0x100];
    char minuteFmt[0x100];
    char costText[32];
    f32 infoX;
    f32 infoY;
    f32 labelX;
    f32 modelX;
    u16 statusWidth;
    u16 rubyWidth;
    u16 titleWidth;
    u16 costWidth;
    s32 language;
    s32 titleOffset;

#define SET_POS(v, vx, vy) do { \
    Vec3 init = { 0.0f, 0.0f, 0.0f }; \
    (v) = init; \
    (v).x = (vx); \
    (v).y = (vy); \
} while (0)
#define UNIT_SCALE(v) do { \
    Vec3 init = { 1.0f, 1.0f, 1.0f }; \
    (v) = init; \
} while (0)
#define HALF_SCALE(v) do { \
    Vec3 init = { 0.5f, 0.5f, 0.5f }; \
    (v) = init; \
} while (0)
#define POINT8_SCALE(v) do { \
    Vec3 init = { 0.8f, 0.8f, 0.8f }; \
    (v) = init; \
} while (0)

    (void)cameraId;

    winBgGX(x, y, pWin, 0);
    winNameGX(x - 265.0f, y + 154.0f, 190.0f, 24.0f, pWin, 2);

    /* Name / level header.  Target advances the level icon from the rendered
       Mario-name width rather than from a fixed +20 position. */
    winFontInit();
    nameX = x - 255.0f;
    baseY = y + 152.0f;
    SET_POS(pName, nameX, baseY);
    UNIT_SCALE(sName);
    text = msgSearch("name_mario");
    winFontSetEdge(&pName, &sName, &white0, text);

    text = msgSearch("name_mario");
    width = (f32)FontGetMessageWidth(text);
    nameX += width * 0.8f + 20.0f;

    winIconInit();
    SET_POS(pLevelIcon, nameX, y + 142.0f);
    UNIT_SCALE(sLevelIcon);
    winIconSet(0x1A6, &pLevelIcon, &sLevelIcon, &white1);

    nameX += 20.0f;
    winFontInit();
    SET_POS(pLevelLabel, nameX, baseY);
    UNIT_SCALE(sLevelLabel);
    text = msgSearch("msg_menu_mario_level");
    winFontSetEdgeWidth(&pLevelLabel, &sLevelLabel, &white2, 45.0f, text);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x8A));
    sprintf(levelText, "%s", tmp2);
    width = (f32)FontGetMessageWidth(levelText);
    if (width > 20.0f) {
        width = 20.0f;
    }

    winFontInit();
    SET_POS(pLevelValue, ((x - 265.0f) + 190.0f) - width - 10.0f, baseY);
    UNIT_SCALE(sLevelValue);
    winFontSetEdgeWidth(&pLevelValue, &sLevelValue, &white3, 20.0f, levelText);

    /* Rank icon + rank name. */
    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
    SET_POS(pRankIcon, x - 255.0f, y + 112.0f);
    UNIT_SCALE(sRankIcon);
    winTexSet(0xB0, &pRankIcon, &sRankIcon, &white4);

    winFontInit();
    SET_POS(pRankText, x - 240.0f, y + 124.0f);
    UNIT_SCALE(sRankText);
    text = msgSearch(BattleGetRankNameLabel(*(s16*)(pouch + 0x8A)));
    winFontSetWidth(&pRankText, &sRankText, &black, 140.0f, text);

    for (i = 0; i < 6; i++) {
        if (menuState != pri[i]) {
            fukidashi(x, y, pWin, pri[i]);
        }
    }
    fukidashi(x, y, pWin, menuState);

    /*
     * Status panel.  The target rebuilds its localized HP/FP/BP table and
     * per-row colors inside the three-row loop, measures both label widths,
     * and draws a plain language label plus a colored "ruby" label.
     */
    winKirinukiGX(x - 270.0f, y - 10.0f, 264.0f, 110.0f, pWin, 0);

    rowY = y - 38.0f;
    language = *(s32*)((u8*)gp + 0x16C);

    for (i = 0; i < 3; i++) {
        char* statusNames[7][3] = {
            { "\x82\x67\x82\x6F", "\x82\x65\x82\x6F", "\x82\x61\x82\x6F" },
            { "HP", "FP", "BP" },
            { "KP", "BP", "OP" },
            { "PC", "PF", "PB" },
            { "PC", "PF", "PM" },
            { "PV", "PF", "PT" },
            { "HP", "FP", "BP" }
        };
        char* rubyNames[3] = {
            "msg_menu_mario_ruby_hp",
            "msg_menu_mario_ruby_fp",
            "msg_menu_mario_ruby_bp"
        };
        u32 rubyColors[3] = {
            0xA73C3CFF, 0x37691EFF, 0x0F217AFF
        };
        u32 bgColors[3] = {
            0xCB8CA2FE, 0xCBBE89FE, 0x94B9C7FE
        };
        char* statusName = statusNames[language][i];

        statusWidth = FontGetMessageWidth(statusName);
        text = msgSearch(rubyNames[i]);
        rubyWidth = FontGetMessageWidth(text);

        winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
        GXSetTevColorIn(0, 15, 15, 15, 2);
        GXSetTevAlphaIn(0, 7, 1, 4, 7);

        SET_POS(pStatusBg, x - 222.0f, rowY - 4.0f);
        UNIT_SCALE(sStatusBg);
        winTexSet(0xAF, &pStatusBg, &sStatusBg, &bgColors[i]);

        winFontInit();

        SET_POS(pStatusText, (x - 220.0f) - (f32)statusWidth * 0.5f, rowY + 12.0f);
        UNIT_SCALE(sStatusText);
        winFontSet(&pStatusText, &sStatusText, &black, statusName);

        SET_POS(pStatusRuby, (x - 220.0f) - (f32)rubyWidth * 0.25f, rowY + 21.0f);
        HALF_SCALE(sStatusRuby);
        text = msgSearch(rubyNames[i]);
        winFontSet(&pStatusRuby, &sStatusRuby, &rubyColors[i], text);

        rowY -= 34.0f;
    }

    winIconInit();
    SET_POS(pHpIcon, x - 160.0f, y - 34.0f);
    UNIT_SCALE(sHpIcon);
    winIconSet(0x1A7, &pHpIcon, &sHpIcon, &white8);

    SET_POS(pFpIcon, x - 160.0f, y - 68.0f);
    UNIT_SCALE(sFpIcon);
    winIconSet(0x1A8, &pFpIcon, &sFpIcon, &white8);

    SET_POS(pBpIcon, x - 160.0f, y - 102.0f);
    UNIT_SCALE(sBpIcon);
    winIconSet(0x1A9, &pBpIcon, &sBpIcon, &white8);

    /* Current HP / FP / BP values. */
    winFontInit();

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x70));
    SET_POS(pHp, x - 140.0f, y - 22.0f);
    UNIT_SCALE(sHp);
    winFontSetR(&pHp, &sHp, &black, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x74));
    SET_POS(pFp, x - 140.0f, y - 56.0f);
    UNIT_SCALE(sFp);
    winFontSetR(&pFp, &sFp, &black, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x94));
    SET_POS(pBp, x - 140.0f, y - 90.0f);
    UNIT_SCALE(sBp);
    winFontSetR(&pBp, &sBp, &black, "%s", tmp2);

    /* Target draws separator texture and maximum HP/FP values separately. */
    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
    SET_POS(pStatusBg, x - 80.0f, y - 34.0f);
    UNIT_SCALE(sStatusBg);
    winTexSet(0x10, &pStatusBg, &sStatusBg, &white6);
    pStatusBg.y -= 34.0f;
    winTexSet(0x10, &pStatusBg, &sStatusBg, &white6);

    winFontInit();
    sprintf(tmp2, "%d", *(s16*)(pouch + 0x72));
    SET_POS(pMaxHp, x - 70.0f, y - 22.0f);
    UNIT_SCALE(sMaxHp);
    winFontSet(&pMaxHp, &sMaxHp, &black, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x76));
    SET_POS(pMaxFp, x - 70.0f, y - 56.0f);
    UNIT_SCALE(sMaxFp);
    winFontSet(&pMaxFp, &sMaxFp, &black, "%s", tmp2);

    /* Star-power meter.  The target only emits this box when Mario owns at
       least one star power and fills icons according to current SP / 100. */
    if (starCount > 0) {
        winKirinukiGX(x + 10.0f, y + 130.0f, 260.0f, 80.0f, pWin, 0);

        winIconInit();
        SET_POS(pStarBox, x + 50.0f, y + 106.0f);
        UNIT_SCALE(sStarBox);
        winIconSet(0x212, &pStarBox, &sStarBox, &white0);

        SET_POS(pSpLabel, x + 50.0f, y + 70.0f);
        UNIT_SCALE(sSpLabel);
        winIconSet(0x19F, &pSpLabel, &sSpLabel, &white0);

        winFontInit();
        SET_POS(pSpecialText, x + 80.0f, y + 80.0f);
        UNIT_SCALE(sSpecialText);
        text = msgSearch("msg_menu_mario_speci");
        winFontSetWidth(&pSpecialText, &sSpecialText, &black, 175.0f, text);

        winIconInit();
        filledStars = *(s16*)(pouch + 0x7A) / 100;
        for (i = 0; i < starCount; i++) {
            SET_POS(pStarIcon, x + 90.0f + (f32)(i * 22), y + 106.0f);
            UNIT_SCALE(sStarIcon);
            if (i < filledStars) {
                winIconSet(0x1BF + i, &pStarIcon, &sStarIcon, &white1);
            } else {
                winIconSet(0x1C7, &pStarIcon, &sStarIcon, &white1);
            }
        }
    }

    /*
     * Lower-right collectibles / play-time panel.
     * Target: x+10, y+30, 260x150.
     */
    winKirinukiGX(x + 10.0f, y + 30.0f, 260.0f, 150.0f, pWin, 0);

    winIconInit();
    infoX = x + 30.0f;

    SET_POS(pInfoIcon0, infoX, y + 10.0f);
    UNIT_SCALE(sInfoIcon0);
    winIconSet(0x194, &pInfoIcon0, &sInfoIcon0, &white0);

    SET_POS(pInfoIcon1, infoX, y - 18.0f);
    UNIT_SCALE(sInfoIcon1);
    winIconSet(0x193, &pInfoIcon1, &sInfoIcon1, &white0);

    SET_POS(pInfoIcon2, infoX, y - 46.0f);
    UNIT_SCALE(sInfoIcon2);
    winIconSet(0x195, &pInfoIcon2, &sInfoIcon2, &white0);

    SET_POS(pInfoIcon3, infoX, y - 74.0f);
    UNIT_SCALE(sInfoIcon3);
    winIconSet(0x10A, &pInfoIcon3, &sInfoIcon3, &white0);

    SET_POS(pInfoIcon4, infoX, y - 102.0f);
    UNIT_SCALE(sInfoIcon4);
    winIconSet(0x1AA, &pInfoIcon4, &sInfoIcon4, &white0);

    winFontInit();
    labelX = x + 45.0f;

    SET_POS(pInfoLabel0, labelX, y + 22.0f);
    UNIT_SCALE(sInfoLabel0);
    text = msgSearch("msg_menu_mario_num_sp");
    winFontSetWidth(&pInfoLabel0, &sInfoLabel0, &black, 140.0f, text);

    SET_POS(pInfoLabel1, labelX, y - 6.0f);
    UNIT_SCALE(sInfoLabel1);
    text = msgSearch("msg_menu_mario_num_coin");
    winFontSetWidth(&pInfoLabel1, &sInfoLabel1, &black, 140.0f, text);

    SET_POS(pInfoLabel2, labelX, y - 34.0f);
    UNIT_SCALE(sInfoLabel2);
    text = msgSearch("msg_menu_mario_num_hoshi");
    winFontSetWidth(&pInfoLabel2, &sInfoLabel2, &black, 140.0f, text);

    SET_POS(pInfoLabel3, labelX, y - 62.0f);
    UNIT_SCALE(sInfoLabel3);
    text = msgSearch("msg_menu_mario_num_super_coin");
    winFontSetWidth(&pInfoLabel3, &sInfoLabel3, &black, 140.0f, text);

    SET_POS(pInfoLabel4, labelX, y - 90.0f);
    UNIT_SCALE(sInfoLabel4);
    text = msgSearch("msg_menu_mario_num_playtime");
    winFontSetWidth(&pInfoLabel4, &sInfoLabel4, &black, 125.0f, text);

    infoX = x + 215.0f;

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x96));
    SET_POS(pInfoValue0, infoX, y + 22.0f);
    UNIT_SCALE(sInfoValue0);
    winFontSetR(&pInfoValue0, &sInfoValue0, &black, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x78));
    SET_POS(pInfoValue1, infoX, y - 6.0f);
    UNIT_SCALE(sInfoValue1);
    winFontSetR(&pInfoValue1, &sInfoValue1, &black, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x9A));
    SET_POS(pInfoValue2, infoX, y - 34.0f);
    UNIT_SCALE(sInfoValue2);
    winFontSetR(&pInfoValue2, &sInfoValue2, &black, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x9C));
    SET_POS(pInfoValue3, infoX, y - 62.0f);
    UNIT_SCALE(sInfoValue3);
    winFontSetR(&pInfoValue3, &sInfoValue3, &black, "%s", tmp2);

    /*
     * Play time.  The target first converts gp+0x20 tick time to seconds
     * using the timebase at 0x800000F8 / 4, then caps at 99:59.
     * Signed first division + unsigned hour/minute divisions reproduce the
     * target helper family (__div2i / __div2u / __mod2u).
     */
    timeBase = (*(u32*)0x800000F8) >> 2;
    elapsed = (u64)(*(s64*)((u8*)gp + 0x20) / (s64)timeBase);

    if (elapsed > 359999ULL) {
        hours = 99;
        minutes = 59;
    } else {
        hours = (u32)(elapsed / 3600ULL);
        minutes = (u32)((elapsed % 3600ULL) / 60ULL);
    }

    sprintf(hourFmt, "%%0%dd", 2);
    sprintf(unk_803e5f78, hourFmt, hours);
    SET_POS(pInfoTimeH, x + 168.0f, y - 90.0f);
    UNIT_SCALE(sInfoTimeH);
    winFontSetR(&pInfoTimeH, &sInfoTimeH, &black, "%s", unk_803e5f78);

    sprintf(minuteFmt, "%%0%dd", 2);
    sprintf(unk_803e5f78, minuteFmt, minutes);
    SET_POS(pInfoTimeM, infoX, y - 90.0f);
    UNIT_SCALE(sInfoTimeM);
    winFontSetR(&pInfoTimeM, &sInfoTimeM, &black, "%s", unk_803e5f78);

    /* Four horizontal separator textures, then the lower end-cap. */
    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
    UNIT_SCALE(sDivider);
    for (i = 0; i < 4; i++) {
        SET_POS(pDivider, x + 200.0f, y + 10.0f - (f32)(i * 28));
        winTexSet(0x11, &pDivider, &sDivider, &white1);
    }

    SET_POS(pDividerEnd, x + 220.0f, y - 102.0f);
    UNIT_SCALE(sDividerEnd);
    winTexSet(0x12, &pDividerEnd, &sDividerEnd, &white0);

    /*
     * Mario model preview.  The target advances the pose by current retrace
     * modulo animation length when marioAnimTimer == 0.
     */
    poseId = *(s32*)(w + 0x188);
    animTimer = *(s32*)(w + 0x18C);
    animPoseGetAnimPosePtr(poseId);
    animData = animPoseGetAnimDataPtr(poseId);

    if (animTimer == 0) {
        animTrack = *(void**)((u8*)animData + 0x24);
        animLength = (s32)*(f32*)((u8*)animTrack + 8);
        retrace = *(u32*)((u8*)gp + 0x1C);
        animPoseSetLocalTime(
            (f64)(f32)(retrace % (u32)animLength), poseId);
    }

    animPoseMain(poseId);

    modelX = x - 150.0f;
    winKageGX(modelX, y, 0.0f, 1.0f, pWin, &shadowColor);
    PSMTXTrans(marioMtx, modelX, y, 0.0f);
    winZClear();
    animPoseDrawMtx(poseId, marioMtx, 2, 0.0f, 5.0f);

    /*
     * Expanded star-power list opened by winMarioMain.
     */
    starListOpen = *(s32*)(w + 0x190);
    if (starListOpen != 0) {
        text = msgSearch("msg_menu_mario_sac_w");
        winWazaGX(x - 270.0f, 160.0f, 270.0f, 254.0f, pWin, 0);

        winFontInit();
        titleWidth = FontGetMessageWidth(text);
        titleOffset = (240 - (s32)titleWidth) / 2;
        SET_POS(pWazaTitle, (x - 270.0f) + (f32)titleOffset, 155.0f);
        POINT8_SCALE(sWazaTitle);
        winFontSetEdge(&pWazaTitle, &sWazaTitle, &white0, text);

        SET_POS(pWazaAp, x - 50.0f, 155.0f);
        POINT8_SCALE(sWazaAp);
        text = msgSearch("btl_cost_disp_AP");
        winFontSetEdge(&pWazaAp, &sWazaAp, &white0, text);

        for (j = 0; j < starCount; j++) {
            void* action = superActionTable[j];
            char* actionName = *(char**)action;

            SET_POS(pWazaName, x - 250.0f, 130.0f - (f32)(j * 26));
            UNIT_SCALE(sWazaName);
            text = msgSearch(actionName);
            winFontSetWidth(&pWazaName, &sWazaName, &black, 180.0f, text);

            cost = *(u8*)((u8*)action + 0x12);
            sprintf(tmp2, "%d", cost);
            sprintf(costText, "%s", tmp2);
            costWidth = FontGetMessageWidth(costText);

            SET_POS(pWazaCost, x - 42.0f - (f32)(costWidth / 2), 130.0f - (f32)(j * 26));
            UNIT_SCALE(sWazaCost);
            winFontSet(&pWazaCost, &sWazaCost, &black, costText);

            SET_POS(pWazaCostAp, x - 29.0f, 120.0f - (f32)(j * 26));
            HALF_SCALE(sWazaCostAp);
            text = msgSearch("btl_cost_disp_AP");
            winFontSet(&pWazaCostAp, &sWazaCostAp, &black, text);
        }
    }

#undef SET_POS
#undef UNIT_SCALE
#undef HALF_SCALE
#undef POINT8_SCALE
}

char unk_803e5f78[0x100];
