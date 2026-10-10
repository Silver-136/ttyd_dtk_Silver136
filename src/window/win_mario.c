#include "mario/mario_pouch.h"
#include "window/win_mario.h"

char tmp2[0x100];
extern const char str_PCTd_80423948[3];

void animPoseRelease(s32 poseId);
s32 sprintf(char* str, const char* fmt, ...);
typedef struct Vec3 { f32 x, y, z; } Vec3;
typedef struct WinMarioLinkEntry {
    s16 x;
    s16 y;
    char* pose;
    char* unk_08;
    char* paperPose;
    u16 requiredItem;
    u16 pad_12;
    char* msg;
    u8 up;
    u8 down;
    u8 left;
    u8 right;
} WinMarioLinkEntry;
extern WinMarioLinkEntry linkDt[];
extern char* hammer_help[4];
extern char* boots_help[4];
void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);
void winTexInit(void* data);
void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
void winIconInit(void);
void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off
u8 winMarioInit(void* pWin) {
    extern void* marioGetPtr(void);
    extern s32 marioGetColor(void);
    extern s32 animPoseEntry(char* name, s32 mode);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 force);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern char str_msg_menu_mario_lv_802f5e88[];
    extern char str_S_1_80423890[4];
    extern char str_S_2_80423a1c[4];
    extern char str_S_3_80423a20[4];
    extern char str_S_4_80423a24[4];
    extern char str_M_S_1_80423888[6];
    void* pouch;
    char* base;
    char* name;
    s32 color;
    s32 index;

    base = str_msg_menu_mario_lv_802f5e88;
    pouch = pouchGetPtr();
    if ((*(u16*)pWin & 0x2000) != 0) {
        name = str_S_1_80423890;
        color = marioGetColor();
        switch (color) {
            case 0: name = str_S_1_80423890; break;
            case 1: name = str_S_2_80423a1c; break;
            case 2: name = str_S_3_80423a20; break;
            case 3: name = str_S_4_80423a24; break;
        }
        *(s32*)((s32)pWin + 0x188) = animPoseEntry(base + 0x848, 0);
        animPoseSetAnim(*(s32*)((s32)pWin + 0x188), name, 1);
    } else {
        name = base + 0x850;
        color = marioGetColor();
        switch (color) {
            case 0: break;
            case 1: name = base + 0x858; break;
            case 2: name = base + 0x864; break;
            case 3: name = base + 0x870; break;
        }
        *(s32*)((s32)pWin + 0x188) = animPoseEntry(name, 0);
        animPoseSetAnim(*(s32*)((s32)pWin + 0x188), str_M_S_1_80423888, 1);
    }

    animPoseSetMaterialFlagOn(*(s32*)((s32)pWin + 0x188), 0x1800);
    if ((*(u32*)marioGetPtr() & 0x40000000) != 0) {
        animPoseSetMaterialFlagOn(*(s32*)((s32)pWin + 0x188), 0x0A000000);
    }

    *(s32*)((s32)pWin + 0x18C) = 0;
    *(s32*)((s32)pWin + 0x190) = 0;
    *(s32*)((s32)pWin + 0x194) = 0;
    *(s32*)((s32)pWin + 0x198) = 0;
    for (index = 0; index < 8; index++) {
        if ((*(u16*)((s32)pouch + 0x8C) & (1 << index)) != 0) {
            (*(s32*)((s32)pWin + 0x198))++;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma optimize_for_size reset
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma opt_common_subs off
s32 winMarioMain(void* pWin) {
    typedef struct WinMarioLinkFull {
        s16 x;
        s16 y;
        char* pose;
        char* unk_08;
        char* paperPose;
        u16 requiredItem;
        u16 pad_12;
        char* msg;
        u8 up;
        u8 down;
        u8 left;
        u8 right;
    } WinMarioLinkFull;

    extern void psndSFXOn(s32);
    extern void* superActionTable[];
    extern void animPoseSetAnim(s32, char*, s32);
    extern f64 animPoseGetLoopTimes(s32);
    extern s32 marioGetColor(void);
    extern s32 strcmp(const char*, const char*);
    extern char* hammer_pose[];
    extern char str_S_1_80423890[4];
    extern char str_S_2_80423a1c[4];
    extern char str_S_3_80423a20[4];
    extern char str_S_4_80423a24[4];
    extern char str_R_1_8042389c[4];
    extern char str_R_15_80423a28[5];
    extern char str_R_17_80423a30[5];
    extern char str_R_18_80423a38[5];
    extern char str_M_S_1_80423888[6];
    extern const f32 float_0p89_80423a40;

    WinMarioLinkFull* links;
    WinMarioLinkFull* row;
    s32 open;
    s32 state;
    s32 available;
    s32 color;
    char* closePose;
    char* displayPose;
    u32 buttons;
    u32 dirs;

    open = *(s32*)((s32)pWin + 0x190);
    switch (open) {
        case 1: goto star_list;
        case 0: break;
        default: return 0;
    }

    buttons = *(u32*)((s32)pWin + 0x4);
    if ((buttons & 0x100) != 0) {
        if (*(s32*)((s32)pWin + 0x160) == 0xC) {
            *(s32*)((s32)pWin + 0x190) = 1;
            *(s32*)((s32)pWin + 0x194) = 0;
            psndSFXOn(0x20012);
        }
        goto update_display;
    }

    if ((buttons & 0x200) != 0) {
        psndSFXOn(0x20013);

        if ((*(u16*)pWin & 0x2000) == 0) {
            closePose = str_M_S_1_80423888;
        } else {
            if (strcmp(str_S_1_80423890, str_S_1_80423890) == 0) {
                color = marioGetColor();
                switch (color) {
                    case 0: closePose = str_S_1_80423890; break;
                    case 1: closePose = str_S_2_80423a1c; break;
                    case 2: closePose = str_S_3_80423a20; break;
                    case 3: closePose = str_S_4_80423a24; break;
                }
            } else {
                color = marioGetColor();
                switch (color) {
                    case 0: closePose = str_R_1_8042389c; break;
                    case 1: closePose = str_R_15_80423a28; break;
                    case 2: closePose = str_R_17_80423a30; break;
                    case 3: closePose = str_R_18_80423a38; break;
                }
            }
        }

        animPoseSetAnim(*(s32*)((s32)pWin + 0x188), closePose, 0);
        return -1;
    }

    if ((buttons & 0x1000) != 0) {
        return -2;
    }

    dirs = *(u32*)((s32)pWin + 0x10);

    if ((dirs & 0x1000) != 0) {
        links = (WinMarioLinkFull*)linkDt;
        *(s32*)((s32)pWin + 0x160) = links[*(s32*)((s32)pWin + 0x160)].up;
        row = &links[*(s32*)((s32)pWin + 0x160)];
        while (1) {
            if (*(s32*)((s32)pWin + 0x160) == 0xB || *(s32*)((s32)pWin + 0x160) == 0xC) {
                if (*(s32*)((s32)pWin + 0x198) != 0) break;
            } else {
                if (row->requiredItem == 0) break;
                if (pouchCheckItem(row->requiredItem) != 0) break;
            }
            *(s32*)((s32)pWin + 0x160) = row->up;
            row = &links[*(s32*)((s32)pWin + 0x160)];
        }

        psndSFXOn(0x20005);
        goto update_display;
    }

    if ((dirs & 0x2000) != 0) {
        links = (WinMarioLinkFull*)linkDt;
        *(s32*)((s32)pWin + 0x160) = links[*(s32*)((s32)pWin + 0x160)].down;
        row = &links[*(s32*)((s32)pWin + 0x160)];
        while (1) {
            if (*(s32*)((s32)pWin + 0x160) == 0xB || *(s32*)((s32)pWin + 0x160) == 0xC) {
                if (*(s32*)((s32)pWin + 0x198) != 0) break;
            } else {
                if (row->requiredItem == 0) break;
                if (pouchCheckItem(row->requiredItem) != 0) break;
            }
            *(s32*)((s32)pWin + 0x160) = row->down;
            row = &links[*(s32*)((s32)pWin + 0x160)];
        }

        psndSFXOn(0x20005);
        goto update_display;
    }

    if ((dirs & 0x4000) != 0) {
        links = (WinMarioLinkFull*)linkDt;
        *(s32*)((s32)pWin + 0x160) = links[*(s32*)((s32)pWin + 0x160)].left;
        row = &links[*(s32*)((s32)pWin + 0x160)];
        while (1) {
            if (*(s32*)((s32)pWin + 0x160) == 0xB || *(s32*)((s32)pWin + 0x160) == 0xC) {
                if (*(s32*)((s32)pWin + 0x198) != 0) break;
            } else {
                if (row->requiredItem == 0) break;
                if (pouchCheckItem(row->requiredItem) != 0) break;
            }
            *(s32*)((s32)pWin + 0x160) = row->left;
            row = &links[*(s32*)((s32)pWin + 0x160)];
        }

        psndSFXOn(0x20005);
        goto update_display;
    }

    if ((dirs & 0x8000) != 0) {
        links = (WinMarioLinkFull*)linkDt;
        *(s32*)((s32)pWin + 0x160) = links[*(s32*)((s32)pWin + 0x160)].right;
        row = &links[*(s32*)((s32)pWin + 0x160)];
        while (1) {
            if (*(s32*)((s32)pWin + 0x160) == 0xB || *(s32*)((s32)pWin + 0x160) == 0xC) {
                if (*(s32*)((s32)pWin + 0x198) != 0) break;
            } else {
                if (row->requiredItem == 0) break;
                if (pouchCheckItem(row->requiredItem) != 0) break;
            }
            *(s32*)((s32)pWin + 0x160) = row->right;
            row = &links[*(s32*)((s32)pWin + 0x160)];
        }

        psndSFXOn(0x20005);
    }

update_display:
    links = (WinMarioLinkFull*)linkDt;
    state = *(s32*)((s32)pWin + 0x160);
    row = &links[state];

    *(f32*)((s32)pWin + 0x158) = (f32)row->x;
    *(f32*)((s32)pWin + 0x15C) = (f32)row->y;

    if (state == 2) {
        if ((*(u16*)pWin & 0x2000) == 0) {
            displayPose = hammer_pose[pouchGetHammerLv()];
        } else {
            if (strcmp(row->paperPose, str_S_1_80423890) == 0) {
                color = marioGetColor();
                switch (color) {
                    case 0: displayPose = str_S_1_80423890; break;
                    case 1: displayPose = str_S_2_80423a1c; break;
                    case 2: displayPose = str_S_3_80423a20; break;
                    case 3: displayPose = str_S_4_80423a24; break;
                }
            } else {
                color = marioGetColor();
                switch (color) {
                    case 0: displayPose = str_R_1_8042389c; break;
                    case 1: displayPose = str_R_15_80423a28; break;
                    case 2: displayPose = str_R_17_80423a30; break;
                    case 3: displayPose = str_R_18_80423a38; break;
                }
            }
        }

        animPoseSetAnim(*(s32*)((s32)pWin + 0x188), displayPose, 0);

        if (*(s32*)((s32)pWin + 0x18C) == 0) {
            if (animPoseGetLoopTimes(*(s32*)((s32)pWin + 0x188)) >=
                float_0p89_80423a40) {
                *(s32*)((s32)pWin + 0x18C) = 0x1E;
            }
        } else {
            *(s32*)((s32)pWin + 0x18C) =
                *(s32*)((s32)pWin + 0x18C) - 1;
        }

        if ((*(u16*)pWin & 0x2000) != 0) {
            *(s32*)((s32)pWin + 0x18C) = 0;
        }
    } else {
        *(s32*)((s32)pWin + 0x18C) = 0;

        if ((*(u16*)pWin & 0x2000) == 0) {
            displayPose = row->pose;
        } else {
            if (strcmp(row->paperPose, str_S_1_80423890) == 0) {
                color = marioGetColor();
                switch (color) {
                    case 0: displayPose = str_S_1_80423890; break;
                    case 1: displayPose = str_S_2_80423a1c; break;
                    case 2: displayPose = str_S_3_80423a20; break;
                    case 3: displayPose = str_S_4_80423a24; break;
                }
            } else {
                color = marioGetColor();
                switch (color) {
                    case 0: displayPose = str_R_1_8042389c; break;
                    case 1: displayPose = str_R_15_80423a28; break;
                    case 2: displayPose = str_R_17_80423a30; break;
                    case 3: displayPose = str_R_18_80423a38; break;
                }
            }
        }

        animPoseSetAnim(*(s32*)((s32)pWin + 0x188), displayPose, 0);
    }

    state = *(s32*)((s32)pWin + 0x160);
    if (state == 3) {
        winMsgEntry(pWin, 0, boots_help[pouchGetJumpLv()], 0);
    } else if (state == 2) {
        winMsgEntry(pWin, 0, hammer_help[pouchGetHammerLv()], 0);
    } else {
        winMsgEntry(pWin, 0, row->msg, 0);
    }
    return 0;

star_list:
    dirs = *(u32*)((s32)pWin + 0x10);
    if ((dirs & 0x1000) != 0) {
        *(s32*)((s32)pWin + 0x194) =
            *(s32*)((s32)pWin + 0x194) - 1;
        if (*(s32*)((s32)pWin + 0x194) < 0) {
            *(s32*)((s32)pWin + 0x194) =
                *(s32*)((s32)pWin + 0x198) - 1;
        }
        psndSFXOn(0x20005);
    } else if ((dirs & 0x2000) != 0) {
        *(s32*)((s32)pWin + 0x194) =
            *(s32*)((s32)pWin + 0x194) + 1;
        if (*(s32*)((s32)pWin + 0x194) >=
            *(s32*)((s32)pWin + 0x198)) {
            *(s32*)((s32)pWin + 0x194) = 0;
        }
        psndSFXOn(0x20005);
    } else {
        buttons = *(u32*)((s32)pWin + 0x4);
        if ((buttons & 0x200) != 0) {
            *(s32*)((s32)pWin + 0x190) = 0;
            psndSFXOn(0x20013);
        } else if ((buttons & 0x1000) != 0) {
            return -2;
        }
    }

    *(f32*)((s32)pWin + 0x158) = -280.0f;
    *(f32*)((s32)pWin + 0x15C) =
        118.0f - 26.0f * (f32)*(s32*)((s32)pWin + 0x194);
    winMsgEntry(
        pWin,
        0,
        *(char**)((s32)superActionTable[*(s32*)((s32)pWin + 0x194)] + 0xC),
        0);
    return 0;
}

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
#pragma opt_common_subs reset
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma opt_common_subs off
void fukidashi(f32 x, f32 y, void* menu, s32 type) {
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern u8 itemDataTable[];
    extern char str_msg_menu_mario_lv_802f5e88[];
    extern u32 dat_804238b8;
    extern u32 dat_804238bc;
    extern u32 dat_804238c0;
    extern u32 dat_804238c4;
    extern u32 dat_804238c8;
    extern u32 dat_804238cc;

    char* base = str_msg_menu_mario_lv_802f5e88;
    s32 level;
    u16 icon;
    Vec3 h_scale2, h_src2, h_pos2;
    Vec3 h_scale1, h_src1, h_pos1;
    Vec3 h_scale0, h_src0, h_pos0;
    u32 h_color2, h_color1, h_color0;
    Vec3 j_scale2, j_src2, j_pos2;
    Vec3 j_scale1, j_src1, j_pos1;
    Vec3 j_scale0, j_src0, j_pos0;
    u32 j_color2, j_color1, j_color0;
    Vec3 p_scale2, p_src2, p_pos2;
    Vec3 p_scale1, p_src1, p_pos1;
    Vec3 p_scale0, p_src0, p_pos0;
    u32 p_color2, p_color1, p_color0;
    Vec3 a_scale2, a_src2, a_pos2;
    Vec3 a_scale1, a_src1, a_pos1;
    Vec3 a_scale0, a_src0, a_pos0;
    u32 a_color2, a_color1, a_color0;
    Vec3 b_scale2, b_src2, b_pos2;
    Vec3 b_scale1, b_src1, b_pos1;
    Vec3 b_scale0, b_src0, b_pos0;
    u32 b_color2, b_color1, b_color0;
    Vec3 t_scale2, t_src2, t_pos2;
    Vec3 t_scale1, t_src1, t_pos1;
    Vec3 t_scale0, t_src0, t_pos0;
    u32 t_color2, t_color1, t_color0;

    switch (type) {
        case 2:
            level = pouchGetHammerLv();
            if (level <= 0) {
                break;
            }
            level = pouchGetHammerLv();
            icon = *(u16*)(itemDataTable + level * 0x28 + 0x160);
            winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
            GXSetTevColorIn(0, 15, 15, 15, 2);
            GXSetTevAlphaIn(0, 7, 1, 4, 7);

            h_src0 = *(Vec3*)(base + 0x1CC);
            h_src0.x = (f32)x - 230.0f + 5.0f;
            y += 72.0f;
            h_src0.y = y - 5.0f;
            h_pos0 = h_src0;
            h_scale0 = *(Vec3*)(base + 0x1D8);
            h_color0 = dat_804238c4;
            winTexSet(0xB1, &h_pos0, &h_scale0, &h_color0);

            h_src1 = *(Vec3*)(base + 0x1E4);
            h_src1.x = (f32)x - 230.0f;
            h_src1.y = y;
            h_pos1 = h_src1;
            h_scale1 = *(Vec3*)(base + 0x1F0);
            h_color1 = dat_804238c8;
            winTexSet(0xB1, &h_pos1, &h_scale1, &h_color1);

            winIconInit();

            h_src2 = *(Vec3*)(base + 0x1FC);
            h_src2.x = (f32)x - 230.0f - 6.0f;
            h_src2.y = y;
            h_pos2 = h_src2;
            h_scale2 = *(Vec3*)(base + 0x208);
            h_color2 = dat_804238cc;
            winIconSet(icon, &h_pos2, &h_scale2, &h_color2);
            break;

        case 3:
            level = pouchGetJumpLv();
            if (level <= 0) {
                break;
            }
            level = pouchGetJumpLv();
            icon = *(u16*)(itemDataTable + level * 0x28 + 0xE8);
            winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
            GXSetTevColorIn(0, 15, 15, 15, 2);
            GXSetTevAlphaIn(0, 7, 1, 4, 7);

            j_src0 = *(Vec3*)(base + 0x1CC);
            j_src0.x = (f32)x - 210.0f + 5.0f;
            y += 20.0f;
            j_src0.y = y - 5.0f;
            j_pos0 = j_src0;
            j_scale0 = *(Vec3*)(base + 0x1D8);
            j_color0 = dat_804238c4;
            winTexSet(0xB1, &j_pos0, &j_scale0, &j_color0);

            j_src1 = *(Vec3*)(base + 0x1E4);
            j_src1.x = (f32)x - 210.0f;
            j_src1.y = y;
            j_pos1 = j_src1;
            j_scale1 = *(Vec3*)(base + 0x1F0);
            j_color1 = dat_804238c8;
            winTexSet(0xB1, &j_pos1, &j_scale1, &j_color1);

            winIconInit();

            j_src2 = *(Vec3*)(base + 0x1FC);
            j_src2.x = (f32)x - 210.0f - 6.0f;
            j_src2.y = y;
            j_pos2 = j_src2;
            j_scale2 = *(Vec3*)(base + 0x208);
            j_color2 = dat_804238cc;
            winIconSet(icon, &j_pos2, &j_scale2, &j_color2);
            break;

        case 7:
            if (pouchCheckItem(2) == 0) {
                break;
            }
            icon = *(u16*)(itemDataTable + 0x70);
            winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
            GXSetTevColorIn(0, 15, 15, 15, 2);
            GXSetTevAlphaIn(0, 7, 1, 4, 7);

            p_src0 = *(Vec3*)(base + 0x184);
            p_src0.x = (f32)x - 80.0f + 5.0f;
            y += 90.0f;
            p_src0.y = y - 5.0f;
            p_pos0 = p_src0;
            p_scale0 = *(Vec3*)(base + 0x190);
            p_color0 = dat_804238b8;
            winTexSet(0xB1, &p_pos0, &p_scale0, &p_color0);

            p_src1 = *(Vec3*)(base + 0x19C);
            p_src1.x = (f32)x - 80.0f;
            p_src1.y = y;
            p_pos1 = p_src1;
            p_scale1 = *(Vec3*)(base + 0x1A8);
            p_color1 = dat_804238bc;
            winTexSet(0xB1, &p_pos1, &p_scale1, &p_color1);

            winIconInit();

            p_src2 = *(Vec3*)(base + 0x1B4);
            p_src2.x = (f32)x - 80.0f + 6.0f;
            p_src2.y = y;
            p_pos2 = p_src2;
            p_scale2 = *(Vec3*)(base + 0x1C0);
            p_color2 = dat_804238c0;
            winIconSet(icon, &p_pos2, &p_scale2, &p_color2);
            break;

        case 8:
            if (pouchCheckItem(4) == 0) {
                break;
            }
            icon = *(u16*)(itemDataTable + 0xC0);
            winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
            GXSetTevColorIn(0, 15, 15, 15, 2);
            GXSetTevAlphaIn(0, 7, 1, 4, 7);

            a_src0 = *(Vec3*)(base + 0x184);
            a_src0.x = (f32)x - 80.0f + 5.0f;
            y += 20.0f;
            a_src0.y = y - 5.0f;
            a_pos0 = a_src0;
            a_scale0 = *(Vec3*)(base + 0x190);
            a_color0 = dat_804238b8;
            winTexSet(0xB1, &a_pos0, &a_scale0, &a_color0);

            a_src1 = *(Vec3*)(base + 0x19C);
            a_src1.x = (f32)x - 80.0f;
            a_src1.y = y;
            a_pos1 = a_src1;
            a_scale1 = *(Vec3*)(base + 0x1A8);
            a_color1 = dat_804238bc;
            winTexSet(0xB1, &a_pos1, &a_scale1, &a_color1);

            winIconInit();

            a_src2 = *(Vec3*)(base + 0x1B4);
            a_src2.x = (f32)x - 80.0f + 6.0f;
            a_src2.y = y;
            a_pos2 = a_src2;
            a_scale2 = *(Vec3*)(base + 0x1C0);
            a_color2 = dat_804238c0;
            winIconSet(icon, &a_pos2, &a_scale2, &a_color2);
            break;

        case 9:
            if (pouchCheckItem(5) == 0) {
                break;
            }
            icon = *(u16*)(itemDataTable + 0xE8);
            winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
            GXSetTevColorIn(0, 15, 15, 15, 2);
            GXSetTevAlphaIn(0, 7, 1, 4, 7);

            b_src0 = *(Vec3*)(base + 0x184);
            b_src0.x = (f32)x - 30.0f + 5.0f;
            y += 130.0f;
            b_src0.y = y - 5.0f;
            b_pos0 = b_src0;
            b_scale0 = *(Vec3*)(base + 0x190);
            b_color0 = dat_804238b8;
            winTexSet(0xB1, &b_pos0, &b_scale0, &b_color0);

            b_src1 = *(Vec3*)(base + 0x19C);
            b_src1.x = (f32)x - 30.0f;
            b_src1.y = y;
            b_pos1 = b_src1;
            b_scale1 = *(Vec3*)(base + 0x1A8);
            b_color1 = dat_804238bc;
            winTexSet(0xB1, &b_pos1, &b_scale1, &b_color1);

            winIconInit();

            b_src2 = *(Vec3*)(base + 0x1B4);
            b_src2.x = (f32)x - 30.0f + 6.0f;
            b_src2.y = y;
            b_pos2 = b_src2;
            b_scale2 = *(Vec3*)(base + 0x1C0);
            b_color2 = dat_804238c0;
            winIconSet(icon, &b_pos2, &b_scale2, &b_color2);
            break;

        case 10:
            if (pouchCheckItem(3) == 0) {
                break;
            }
            icon = *(u16*)(itemDataTable + 0x98);
            winTexInit(**(void***)((u8*)*(void**)((u8*)menu + 0x28) + 0xA0));
            GXSetTevColorIn(0, 15, 15, 15, 2);
            GXSetTevAlphaIn(0, 7, 1, 4, 7);

            t_src0 = *(Vec3*)(base + 0x184);
            t_src0.x = (f32)x - 30.0f + 5.0f;
            y += 60.0f;
            t_src0.y = y - 5.0f;
            t_pos0 = t_src0;
            t_scale0 = *(Vec3*)(base + 0x190);
            t_color0 = dat_804238b8;
            winTexSet(0xB1, &t_pos0, &t_scale0, &t_color0);

            t_src1 = *(Vec3*)(base + 0x19C);
            t_src1.x = (f32)x - 30.0f;
            t_src1.y = y;
            t_pos1 = t_src1;
            t_scale1 = *(Vec3*)(base + 0x1A8);
            t_color1 = dat_804238bc;
            winTexSet(0xB1, &t_pos1, &t_scale1, &t_color1);

            winIconInit();

            t_src2 = *(Vec3*)(base + 0x1B4);
            t_src2.x = (f32)x - 30.0f + 6.0f;
            t_src2.y = y;
            t_pos2 = t_src2;
            t_scale2 = *(Vec3*)(base + 0x1C0);
            t_color2 = dat_804238c0;
            winIconSet(icon, &t_pos2, &t_scale2, &t_color2);
            break;
    }
}
#pragma opt_common_subs reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


/*
 * Data-pass A1r3: target .sdata order is
 * hammer_help -> boots_help -> hammer_pose -> winMarioDisp::pri.
 * Keep these definitions after their executable consumers but before winMarioDisp
 * so MWCC emits the three source-owned globals ahead of the function-local static.
 */
extern char str_msg_hammer_1_802f5fb8[13];
extern char str_msg_hammer_2_802f5fc8[13];
extern char str_msg_hammer_3_802f5fd8[13];
extern char str_msg_boots_1_802f5fe8[12];
extern char str_msg_boots_2_802f5ff4[12];
extern char str_msg_boots_3_802f6000[12];
extern char str_M_H_1_80423894[6];
extern char str_M_H_4_804238a8[6];
extern char str_M_H_7_804238b0[6];

__declspec(section ".sdata") char* hammer_help[4] = {
    0, str_msg_hammer_1_802f5fb8, str_msg_hammer_2_802f5fc8, str_msg_hammer_3_802f5fd8
};
__declspec(section ".sdata") char* boots_help[4] = {
    0, str_msg_boots_1_802f5fe8, str_msg_boots_2_802f5ff4, str_msg_boots_3_802f6000
};
__declspec(section ".sdata") char* hammer_pose[4] = {
    0, str_M_H_1_80423894, str_M_H_4_804238a8, str_M_H_7_804238b0
};


/*
 * Data-pass A6r: exact target .sdata2 prefix (+0x000..+0x04F),
 * source-ordered immediately before winMarioDisp.
 *
 * All executable consumers of these identities occur earlier in the TU.
 * Their existing declaration-only views are preserved.  Placing the definitions
 * here tests whether this source-owned prefix emits ahead of winMarioDisp's
 * anonymous status-string/compiler pool without A4's early-definition codegen.
 */
__declspec(section ".sdata2") char str_M_S_1_80423888[6] __attribute__ ((aligned(4))) = "M_S_1";
__declspec(section ".sdata2") char str_S_1_80423890[4] __attribute__ ((aligned(4))) = "S_1";
__declspec(section ".sdata2") char str_M_H_1_80423894[6] __attribute__ ((aligned(4))) = "M_H_1";
__declspec(section ".sdata2") char str_R_1_8042389c[4] __attribute__ ((aligned(4))) = "R_1";
__declspec(section ".sdata2") char str_M_R_1_804238a0[6] __attribute__ ((aligned(4))) = "M_R_1";
__declspec(section ".sdata2") char str_M_H_4_804238a8[6] __attribute__ ((aligned(4))) = "M_H_4";
__declspec(section ".sdata2") char str_M_H_7_804238b0[6] __attribute__ ((aligned(4))) = "M_H_7";

__declspec(section ".sdata2") u32 dat_804238b8 = 0x00000080;
__declspec(section ".sdata2") u32 dat_804238bc = 0xE1E1E1FE;
__declspec(section ".sdata2") u32 dat_804238c0 = 0xFFFFFFFF;
__declspec(section ".sdata2") u32 dat_804238c4 = 0x00000080;
__declspec(section ".sdata2") u32 dat_804238c8 = 0xE1E1E1FE;
__declspec(section ".sdata2") u32 dat_804238cc = 0xFFFFFFFF;
__declspec(section ".sdata2") u32 dat_804238d0 = 0xFFFFFFFF;
__declspec(section ".sdata2") u32 dat_804238d4 = 0x000000FF;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
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

    typedef struct { u8 r, g, b, a; } LocalColor;
    u8* w = (u8*)pWin;
    u8* pouch = (u8*)pouchGetPtr();
    f32 x = *(f32*)(w + 0xC4 + index * 0x14);
    f32 y = *(f32*)(w + 0xC8 + index * 0x14);
    s32 menuState = *(s32*)(w + 0x160);
    s32 starCount = *(s32*)(w + 0x198);
    static s32 pri[6] = { 3, 2, 8, 10, 7, 9 };

    Vec3 pName, pNameRaw, sName;
    Vec3 pLevelIcon, pLevelIconRaw, sLevelIcon;
    Vec3 pLevelLabel, pLevelLabelRaw, sLevelLabel;
    Vec3 pLevelValue, pLevelValueRaw, sLevelValue;
    Vec3 pRankIcon, pRankIconRaw, sRankIcon;
    Vec3 pRankText, pRankTextRaw, sRankText;
    Vec3 pStatusBg, pStatusBgRaw, sStatusBg;
    Vec3 pStatusText, pStatusTextRaw, sStatusText;
    Vec3 pStatusRuby, pStatusRubyRaw, sStatusRuby;
    Vec3 pHpIcon, pHpIconRaw, sHpIcon;
    Vec3 pFpIcon, pFpIconRaw, sFpIcon;
    Vec3 pBpIcon, pBpIconRaw, sBpIcon;
    Vec3 pHp, pHpRaw, sHp;
    Vec3 pFp, pFpRaw, sFp;
    Vec3 pBp, pBpRaw, sBp;
    Vec3 pMaxHp, pMaxHpRaw, sMaxHp;
    Vec3 pMaxFp, pMaxFpRaw, sMaxFp;
    Vec3 pStarBox, pStarBoxRaw, sStarBox;
    Vec3 pStarIcon, pStarIconRaw, sStarIcon;
    Vec3 pSpLabel, pSpLabelRaw, sSpLabel;
    Vec3 pSpecialText, pSpecialTextRaw, sSpecialText;

    Vec3 pInfoIcon0, pInfoIcon0Raw, sInfoIcon0;
    Vec3 pInfoIcon1, pInfoIcon1Raw, sInfoIcon1;
    Vec3 pInfoIcon2, pInfoIcon2Raw, sInfoIcon2;
    Vec3 pInfoIcon3, pInfoIcon3Raw, sInfoIcon3;
    Vec3 pInfoIcon4, pInfoIcon4Raw, sInfoIcon4;
    Vec3 pInfoLabel0, pInfoLabel0Raw, sInfoLabel0;
    Vec3 pInfoLabel1, pInfoLabel1Raw, sInfoLabel1;
    Vec3 pInfoLabel2, pInfoLabel2Raw, sInfoLabel2;
    Vec3 pInfoLabel3, pInfoLabel3Raw, sInfoLabel3;
    Vec3 pInfoLabel4, pInfoLabel4Raw, sInfoLabel4;
    Vec3 pInfoValue0, pInfoValue0Raw, sInfoValue0;
    Vec3 pInfoValue1, pInfoValue1Raw, sInfoValue1;
    Vec3 pInfoValue2, pInfoValue2Raw, sInfoValue2;
    Vec3 pInfoValue3, pInfoValue3Raw, sInfoValue3;
    Vec3 pInfoTimeH, pInfoTimeHRaw, sInfoTimeH;
    Vec3 pInfoTimeM, pInfoTimeMRaw, sInfoTimeM;
    Vec3 pDivider, pDividerRaw, sDivider;
    Vec3 pDividerEnd, pDividerEndRaw, sDividerEnd;

    Vec3 pWazaTitle, pWazaTitleRaw, sWazaTitle;
    Vec3 pWazaAp, pWazaApRaw, sWazaAp;
    Vec3 pWazaName, pWazaNameRaw, sWazaName;
    Vec3 pWazaCost, pWazaCostRaw, sWazaCost;
    Vec3 pWazaCostAp, pWazaCostApRaw, sWazaCostAp;

    f32 marioMtx[3][4];

    LocalColor white0 = *(LocalColor*)&dat_804238d0;
    LocalColor white1 = *(LocalColor*)&dat_804238d0;
    LocalColor white2 = *(LocalColor*)&dat_804238d0;
    LocalColor white3 = *(LocalColor*)&dat_804238d0;
    LocalColor white4 = *(LocalColor*)&dat_804238d0;
    LocalColor white5 = *(LocalColor*)&dat_804238d0;
    LocalColor white6 = *(LocalColor*)&dat_804238d0;
    LocalColor white7 = *(LocalColor*)&dat_804238d0;
    LocalColor white8 = *(LocalColor*)&dat_804238d0;
    LocalColor white9 = *(LocalColor*)&dat_804238d0;
    LocalColor black = *(LocalColor*)&dat_804238d4;
    LocalColor shadowColor = *(LocalColor*)&dat_804238b8;
    LocalColor drawColor0;
    LocalColor drawColor1;
    LocalColor drawColor2;
    LocalColor drawColor3;
    LocalColor drawColor4;
    LocalColor drawColor5;
    LocalColor drawColor6;
    LocalColor drawColor7;
    LocalColor drawColor8;
    LocalColor drawColor9;
    LocalColor drawColor10;
    LocalColor drawColor11;
    LocalColor drawColor12;
    LocalColor drawColor13;
    LocalColor drawColor14;
    LocalColor drawColor15;
    LocalColor drawColor16;
    LocalColor drawColor17;
    LocalColor drawColor18;
    LocalColor drawColor19;
    LocalColor drawColor20;
    LocalColor drawColor21;
    LocalColor drawColor22;
    LocalColor drawColor23;
    LocalColor drawColor24;
    LocalColor drawColor25;
    LocalColor drawColor26;
    LocalColor drawColor27;
    LocalColor drawColor28;
    LocalColor drawColor29;
    LocalColor drawColor30;
    LocalColor drawColor31;
    LocalColor drawColor32;
    LocalColor drawColor33;
    LocalColor drawColor34;
    LocalColor drawColor35;
    LocalColor drawColor36;
    LocalColor drawColor37;
    LocalColor drawColor38;
    LocalColor drawColor39;
    LocalColor drawColor40;
    LocalColor drawColor41;
    LocalColor drawColor42;
    LocalColor drawColor43;
    LocalColor drawColor44;
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
    SET_POS(pNameRaw, nameX, baseY);
    pName = pNameRaw;
    UNIT_SCALE(sName);
    text = msgSearch("name_mario");
    drawColor0 = white0;
    winFontSetEdge(&pName, &sName, &drawColor0, text);

    text = msgSearch("name_mario");
    width = (f32)FontGetMessageWidth(text);
    nameX += width * 0.8f + 20.0f;

    winIconInit();
    SET_POS(pLevelIconRaw, nameX, y + 142.0f);
    pLevelIcon = pLevelIconRaw;
    UNIT_SCALE(sLevelIcon);
    drawColor1 = white1;
    winIconSet(0x1A6, &pLevelIcon, &sLevelIcon, &drawColor1);

    nameX += 20.0f;
    winFontInit();
    SET_POS(pLevelLabelRaw, nameX, baseY);
    pLevelLabel = pLevelLabelRaw;
    UNIT_SCALE(sLevelLabel);
    text = msgSearch("msg_menu_mario_level");
    drawColor2 = white2;
    winFontSetEdgeWidth(&pLevelLabel, &sLevelLabel, &drawColor2, 45.0f, text);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x8A));
    sprintf(levelText, "%s", tmp2);
    width = (f32)FontGetMessageWidth(levelText);
    if (width > 20.0f) {
        width = 20.0f;
    }

    winFontInit();
    SET_POS(pLevelValueRaw, ((x - 265.0f) + 190.0f) - width - 10.0f, baseY);
    pLevelValue = pLevelValueRaw;
    UNIT_SCALE(sLevelValue);
    drawColor3 = white3;
    winFontSetEdgeWidth(&pLevelValue, &sLevelValue, &drawColor3, 20.0f, levelText);

    /* Rank icon + rank name. */
    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
    SET_POS(pRankIconRaw, x - 255.0f, y + 112.0f);
    pRankIcon = pRankIconRaw;
    UNIT_SCALE(sRankIcon);
    drawColor4 = white4;
    winTexSet(0xB0, &pRankIcon, &sRankIcon, &drawColor4);

    winFontInit();
    SET_POS(pRankTextRaw, x - 240.0f, y + 124.0f);
    pRankText = pRankTextRaw;
    UNIT_SCALE(sRankText);
    text = msgSearch(BattleGetRankNameLabel(*(s16*)(pouch + 0x8A)));
    drawColor5 = black;
    winFontSetWidth(&pRankText, &sRankText, &drawColor5, 140.0f, text);

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

        SET_POS(pStatusBgRaw, x - 222.0f, rowY - 4.0f);
        pStatusBg = pStatusBgRaw;
        UNIT_SCALE(sStatusBg);
        winTexSet(0xAF, &pStatusBg, &sStatusBg, &bgColors[i]);

        winFontInit();

        SET_POS(pStatusTextRaw, (x - 220.0f) - (f32)statusWidth * 0.5f, rowY + 12.0f);
        pStatusText = pStatusTextRaw;
        UNIT_SCALE(sStatusText);
        drawColor6 = black;
        winFontSet(&pStatusText, &sStatusText, &drawColor6, statusName);

        SET_POS(pStatusRubyRaw, (x - 220.0f) - (f32)rubyWidth * 0.25f, rowY + 21.0f);
        pStatusRuby = pStatusRubyRaw;
        HALF_SCALE(sStatusRuby);
        text = msgSearch(rubyNames[i]);
        winFontSet(&pStatusRuby, &sStatusRuby, &rubyColors[i], text);

        rowY -= 34.0f;
    }

    winIconInit();
    SET_POS(pHpIconRaw, x - 160.0f, y - 34.0f);
    pHpIcon = pHpIconRaw;
    UNIT_SCALE(sHpIcon);
    drawColor7 = white8;
    winIconSet(0x1A7, &pHpIcon, &sHpIcon, &drawColor7);

    SET_POS(pFpIconRaw, x - 160.0f, y - 68.0f);
    pFpIcon = pFpIconRaw;
    UNIT_SCALE(sFpIcon);
    drawColor8 = white8;
    winIconSet(0x1A8, &pFpIcon, &sFpIcon, &drawColor8);

    SET_POS(pBpIconRaw, x - 160.0f, y - 102.0f);
    pBpIcon = pBpIconRaw;
    UNIT_SCALE(sBpIcon);
    drawColor9 = white8;
    winIconSet(0x1A9, &pBpIcon, &sBpIcon, &drawColor9);

    /* Current HP / FP / BP values. */
    winFontInit();

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x70));
    SET_POS(pHpRaw, x - 140.0f, y - 22.0f);
    pHp = pHpRaw;
    UNIT_SCALE(sHp);
    drawColor10 = black;
    winFontSetR(&pHp, &sHp, &drawColor10, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x74));
    SET_POS(pFpRaw, x - 140.0f, y - 56.0f);
    pFp = pFpRaw;
    UNIT_SCALE(sFp);
    drawColor11 = black;
    winFontSetR(&pFp, &sFp, &drawColor11, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x94));
    SET_POS(pBpRaw, x - 140.0f, y - 90.0f);
    pBp = pBpRaw;
    UNIT_SCALE(sBp);
    drawColor12 = black;
    winFontSetR(&pBp, &sBp, &drawColor12, "%s", tmp2);

    /* Target draws separator texture and maximum HP/FP values separately. */
    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
    SET_POS(pStatusBgRaw, x - 80.0f, y - 34.0f);
    pStatusBg = pStatusBgRaw;
    UNIT_SCALE(sStatusBg);
    drawColor13 = white6;
    winTexSet(0x10, &pStatusBg, &sStatusBg, &drawColor13);
    pStatusBg.y -= 34.0f;
    drawColor14 = white6;
    winTexSet(0x10, &pStatusBg, &sStatusBg, &drawColor14);

    winFontInit();
    sprintf(tmp2, "%d", *(s16*)(pouch + 0x72));
    SET_POS(pMaxHpRaw, x - 70.0f, y - 22.0f);
    pMaxHp = pMaxHpRaw;
    UNIT_SCALE(sMaxHp);
    drawColor15 = black;
    winFontSet(&pMaxHp, &sMaxHp, &drawColor15, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x76));
    SET_POS(pMaxFpRaw, x - 70.0f, y - 56.0f);
    pMaxFp = pMaxFpRaw;
    UNIT_SCALE(sMaxFp);
    drawColor16 = black;
    winFontSet(&pMaxFp, &sMaxFp, &drawColor16, "%s", tmp2);

    /* Star-power meter.  The target only emits this box when Mario owns at
       least one star power and fills icons according to current SP / 100. */
    if (starCount > 0) {
        winKirinukiGX(x + 10.0f, y + 130.0f, 260.0f, 80.0f, pWin, 0);

        winIconInit();
        SET_POS(pStarBoxRaw, x + 50.0f, y + 106.0f);
        pStarBox = pStarBoxRaw;
        UNIT_SCALE(sStarBox);
        drawColor17 = white0;
        winIconSet(0x212, &pStarBox, &sStarBox, &drawColor17);

        SET_POS(pSpLabelRaw, x + 50.0f, y + 70.0f);
        pSpLabel = pSpLabelRaw;
        UNIT_SCALE(sSpLabel);
        drawColor18 = white0;
        winIconSet(0x19F, &pSpLabel, &sSpLabel, &drawColor18);

        winFontInit();
        SET_POS(pSpecialTextRaw, x + 80.0f, y + 80.0f);
        pSpecialText = pSpecialTextRaw;
        UNIT_SCALE(sSpecialText);
        text = msgSearch("msg_menu_mario_speci");
        drawColor19 = black;
        winFontSetWidth(&pSpecialText, &sSpecialText, &drawColor19, 175.0f, text);

        winIconInit();
        filledStars = *(s16*)(pouch + 0x7A) / 100;
        for (i = 0; i < starCount; i++) {
            SET_POS(pStarIconRaw, x + 90.0f + (f32)(i * 22), y + 106.0f);
            pStarIcon = pStarIconRaw;
            UNIT_SCALE(sStarIcon);
            if (i < filledStars) {
                drawColor20 = white1;
                winIconSet(0x1BF + i, &pStarIcon, &sStarIcon, &drawColor20);
            } else {
                drawColor21 = white1;
                winIconSet(0x1C7, &pStarIcon, &sStarIcon, &drawColor21);
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

    SET_POS(pInfoIcon0Raw, infoX, y + 10.0f);
    pInfoIcon0 = pInfoIcon0Raw;
    UNIT_SCALE(sInfoIcon0);
    drawColor22 = white0;
    winIconSet(0x194, &pInfoIcon0, &sInfoIcon0, &drawColor22);

    SET_POS(pInfoIcon1Raw, infoX, y - 18.0f);
    pInfoIcon1 = pInfoIcon1Raw;
    UNIT_SCALE(sInfoIcon1);
    drawColor23 = white0;
    winIconSet(0x193, &pInfoIcon1, &sInfoIcon1, &drawColor23);

    SET_POS(pInfoIcon2Raw, infoX, y - 46.0f);
    pInfoIcon2 = pInfoIcon2Raw;
    UNIT_SCALE(sInfoIcon2);
    drawColor24 = white0;
    winIconSet(0x195, &pInfoIcon2, &sInfoIcon2, &drawColor24);

    SET_POS(pInfoIcon3Raw, infoX, y - 74.0f);
    pInfoIcon3 = pInfoIcon3Raw;
    UNIT_SCALE(sInfoIcon3);
    drawColor25 = white0;
    winIconSet(0x10A, &pInfoIcon3, &sInfoIcon3, &drawColor25);

    SET_POS(pInfoIcon4Raw, infoX, y - 102.0f);
    pInfoIcon4 = pInfoIcon4Raw;
    UNIT_SCALE(sInfoIcon4);
    drawColor26 = white0;
    winIconSet(0x1AA, &pInfoIcon4, &sInfoIcon4, &drawColor26);

    winFontInit();
    labelX = x + 45.0f;

    SET_POS(pInfoLabel0Raw, labelX, y + 22.0f);
    pInfoLabel0 = pInfoLabel0Raw;
    UNIT_SCALE(sInfoLabel0);
    text = msgSearch("msg_menu_mario_num_sp");
    drawColor27 = black;
    winFontSetWidth(&pInfoLabel0, &sInfoLabel0, &drawColor27, 140.0f, text);

    SET_POS(pInfoLabel1Raw, labelX, y - 6.0f);
    pInfoLabel1 = pInfoLabel1Raw;
    UNIT_SCALE(sInfoLabel1);
    text = msgSearch("msg_menu_mario_num_coin");
    drawColor28 = black;
    winFontSetWidth(&pInfoLabel1, &sInfoLabel1, &drawColor28, 140.0f, text);

    SET_POS(pInfoLabel2Raw, labelX, y - 34.0f);
    pInfoLabel2 = pInfoLabel2Raw;
    UNIT_SCALE(sInfoLabel2);
    text = msgSearch("msg_menu_mario_num_hoshi");
    drawColor29 = black;
    winFontSetWidth(&pInfoLabel2, &sInfoLabel2, &drawColor29, 140.0f, text);

    SET_POS(pInfoLabel3Raw, labelX, y - 62.0f);
    pInfoLabel3 = pInfoLabel3Raw;
    UNIT_SCALE(sInfoLabel3);
    text = msgSearch("msg_menu_mario_num_super_coin");
    drawColor30 = black;
    winFontSetWidth(&pInfoLabel3, &sInfoLabel3, &drawColor30, 140.0f, text);

    SET_POS(pInfoLabel4Raw, labelX, y - 90.0f);
    pInfoLabel4 = pInfoLabel4Raw;
    UNIT_SCALE(sInfoLabel4);
    text = msgSearch("msg_menu_mario_num_playtime");
    drawColor31 = black;
    winFontSetWidth(&pInfoLabel4, &sInfoLabel4, &drawColor31, 125.0f, text);

    infoX = x + 215.0f;

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x96));
    SET_POS(pInfoValue0Raw, infoX, y + 22.0f);
    pInfoValue0 = pInfoValue0Raw;
    UNIT_SCALE(sInfoValue0);
    drawColor32 = black;
    winFontSetR(&pInfoValue0, &sInfoValue0, &drawColor32, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x78));
    SET_POS(pInfoValue1Raw, infoX, y - 6.0f);
    pInfoValue1 = pInfoValue1Raw;
    UNIT_SCALE(sInfoValue1);
    drawColor33 = black;
    winFontSetR(&pInfoValue1, &sInfoValue1, &drawColor33, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x9A));
    SET_POS(pInfoValue2Raw, infoX, y - 34.0f);
    pInfoValue2 = pInfoValue2Raw;
    UNIT_SCALE(sInfoValue2);
    drawColor34 = black;
    winFontSetR(&pInfoValue2, &sInfoValue2, &drawColor34, "%s", tmp2);

    sprintf(tmp2, "%d", *(s16*)(pouch + 0x9C));
    SET_POS(pInfoValue3Raw, infoX, y - 62.0f);
    pInfoValue3 = pInfoValue3Raw;
    UNIT_SCALE(sInfoValue3);
    drawColor35 = black;
    winFontSetR(&pInfoValue3, &sInfoValue3, &drawColor35, "%s", tmp2);

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
    SET_POS(pInfoTimeHRaw, x + 168.0f, y - 90.0f);
    pInfoTimeH = pInfoTimeHRaw;
    UNIT_SCALE(sInfoTimeH);
    drawColor36 = black;
    winFontSetR(&pInfoTimeH, &sInfoTimeH, &drawColor36, "%s", unk_803e5f78);

    sprintf(minuteFmt, "%%0%dd", 2);
    sprintf(unk_803e5f78, minuteFmt, minutes);
    SET_POS(pInfoTimeMRaw, infoX, y - 90.0f);
    pInfoTimeM = pInfoTimeMRaw;
    UNIT_SCALE(sInfoTimeM);
    drawColor37 = black;
    winFontSetR(&pInfoTimeM, &sInfoTimeM, &drawColor37, "%s", unk_803e5f78);

    /* Four horizontal separator textures, then the lower end-cap. */
    winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
    UNIT_SCALE(sDivider);
    for (i = 0; i < 4; i++) {
        SET_POS(pDividerRaw, x + 200.0f, y + 10.0f - (f32)(i * 28));
        pDivider = pDividerRaw;
        drawColor38 = white1;
        winTexSet(0x11, &pDivider, &sDivider, &drawColor38);
    }

    SET_POS(pDividerEndRaw, x + 220.0f, y - 102.0f);
    pDividerEnd = pDividerEndRaw;
    UNIT_SCALE(sDividerEnd);
    drawColor39 = white0;
    winTexSet(0x12, &pDividerEnd, &sDividerEnd, &drawColor39);

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
        SET_POS(pWazaTitleRaw, (x - 270.0f) + (f32)titleOffset, 155.0f);
        pWazaTitle = pWazaTitleRaw;
        POINT8_SCALE(sWazaTitle);
        drawColor40 = white0;
        winFontSetEdge(&pWazaTitle, &sWazaTitle, &drawColor40, text);

        SET_POS(pWazaApRaw, x - 50.0f, 155.0f);
        pWazaAp = pWazaApRaw;
        POINT8_SCALE(sWazaAp);
        text = msgSearch("btl_cost_disp_AP");
        drawColor41 = white0;
        winFontSetEdge(&pWazaAp, &sWazaAp, &drawColor41, text);

        for (j = 0; j < starCount; j++) {
            void* action = superActionTable[j];
            char* actionName = *(char**)action;

            SET_POS(pWazaNameRaw, x - 250.0f, 130.0f - (f32)(j * 26));
            pWazaName = pWazaNameRaw;
            UNIT_SCALE(sWazaName);
            text = msgSearch(actionName);
            drawColor42 = black;
            winFontSetWidth(&pWazaName, &sWazaName, &drawColor42, 180.0f, text);

            cost = *(u8*)((u8*)action + 0x12);
            sprintf(tmp2, "%d", cost);
            sprintf(costText, "%s", tmp2);
            costWidth = FontGetMessageWidth(costText);

            SET_POS(pWazaCostRaw, x - 42.0f - (f32)(costWidth / 2), 130.0f - (f32)(j * 26));
            pWazaCost = pWazaCostRaw;
            UNIT_SCALE(sWazaCost);
            drawColor43 = black;
            winFontSet(&pWazaCost, &sWazaCost, &drawColor43, costText);

            SET_POS(pWazaCostApRaw, x - 29.0f, 120.0f - (f32)(j * 26));
            pWazaCostAp = pWazaCostApRaw;
            HALF_SCALE(sWazaCostAp);
            text = msgSearch("btl_cost_disp_AP");
            drawColor44 = black;
            winFontSet(&pWazaCostAp, &sWazaCostAp, &drawColor44, text);
        }
    }

#undef SET_POS
#undef UNIT_SCALE
#undef HALF_SCALE
#undef POINT8_SCALE
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

char unk_803e5f78[0x100];

extern char str_msg_menu_mario_lv_802f5e88[];
extern char str_msg_menu_mario_star__802f5e9c[];
extern char str_msg_menu_mario_hp_802f5eb4[];
extern char str_msg_menu_mario_fp_802f5ec8[];
extern char str_msg_menu_mario_bp_802f5edc[];
extern char str_msg_slit_802f5ef0[];
extern char str_msg_plane_802f5efc[];
extern char str_msg_ship_802f5f08[];
extern char str_msg_roll_802f5f14[];
extern char str_msg_menu_star_power_802f5f20[];
extern char str_msg_menu_mario_waza_802f5f34[];
extern char str_msg_menu_mario_star__802f5f48[];
extern char str_msg_menu_mario_coin_802f5f60[];
extern char str_msg_menu_mario_kaker_802f5f74[];
extern char str_msg_menu_mario_scoin_802f5f8c[];
extern char str_msg_menu_mario_time_802f5fa4[];
__declspec(section ".data") WinMarioLinkEntry linkDt[18] = {
    { -268,  142, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_lv_802f5e88,        6,  1, 11,  9 },
    { -268,  118, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_star__802f5e9c,    0,  2, 11,  7 },
    { -272,   68, str_M_H_1_80423894, 0, str_R_1_8042389c, 9, 0, 0,                                      1,  3, 11,  7 },
    { -242,   24, str_M_R_1_804238a0, 0, str_R_1_8042389c, 6, 0, 0,                                      2,  4, 11,  8 },
    { -270,  -34, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_hp_802f5eb4,        3,  5, 13, 13 },
    { -270,  -68, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_fp_802f5ec8,        4,  6, 13, 13 },
    { -270, -102, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_bp_802f5edc,        5,  0, 13, 13 },
    { -100,   90, str_M_S_1_80423888, str_M_S_1_80423888, str_S_1_80423890, 2, 0, str_msg_slit_802f5ef0, 9, 10,  2,  9 },
    { -100,   20, str_M_S_1_80423888, str_M_S_1_80423888, str_S_1_80423890, 4, 0, str_msg_plane_802f5efc, 10,  4,  3, 10 },
    {  -50,  130, str_M_S_1_80423888, str_M_S_1_80423888, str_S_1_80423890, 5, 0, str_msg_ship_802f5f08,  0,  7,  0, 11 },
    {  -50,   60, str_M_S_1_80423888, str_M_S_1_80423888, str_S_1_80423890, 3, 0, str_msg_roll_802f5f14,  7,  8,  8, 12 },
    {   20,  106, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_star_power_802f5f20,      17, 12,  9,  0 },
    {   20,   70, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_waza_802f5f34,      11, 13, 10,  0 },
    {    3,   10, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_star__802f5f48,     12, 14,  4,  4 },
    {    3,  -18, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_coin_802f5f60,      13, 15,  4,  4 },
    {    3,  -46, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_kaker_802f5f74,     14, 16,  4,  4 },
    {    3,  -74, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_scoin_802f5f8c,     15, 17,  4,  4 },
    {    3, -102, str_M_S_1_80423888, 0, str_S_1_80423890, 0, 0, str_msg_menu_mario_time_802f5fa4,      16, 11,  4,  4 },
};

/*
 * Data-pass A3: exact target .rodata prefix (+0x000..+0x183), late-defined.
 *
 * Keep all executable consumers declaration-only. This tests whether MWCC emits
 * the explicit source-owned string catalog ahead of compiler-generated .rodata
 * while preserving the retained pre-A2 function source shape.
 */
__declspec(section ".rodata") char str_msg_menu_mario_lv_802f5e88[18] __attribute__ ((aligned(4))) = "msg_menu_mario_lv";
__declspec(section ".rodata") char str_msg_menu_mario_star__802f5e9c[22] __attribute__ ((aligned(4))) = "msg_menu_mario_star_r";
__declspec(section ".rodata") char str_msg_menu_mario_hp_802f5eb4[18] __attribute__ ((aligned(4))) = "msg_menu_mario_hp";
__declspec(section ".rodata") char str_msg_menu_mario_fp_802f5ec8[18] __attribute__ ((aligned(4))) = "msg_menu_mario_fp";
__declspec(section ".rodata") char str_msg_menu_mario_bp_802f5edc[18] __attribute__ ((aligned(4))) = "msg_menu_mario_bp";
__declspec(section ".rodata") char str_msg_slit_802f5ef0[9] __attribute__ ((aligned(4))) = "msg_slit";
__declspec(section ".rodata") char str_msg_plane_802f5efc[10] __attribute__ ((aligned(4))) = "msg_plane";
__declspec(section ".rodata") char str_msg_ship_802f5f08[9] __attribute__ ((aligned(4))) = "msg_ship";
__declspec(section ".rodata") char str_msg_roll_802f5f14[9] __attribute__ ((aligned(4))) = "msg_roll";
__declspec(section ".rodata") char str_msg_menu_star_power_802f5f20[20] __attribute__ ((aligned(4))) = "msg_menu_star_power";
__declspec(section ".rodata") char str_msg_menu_mario_waza_802f5f34[20] __attribute__ ((aligned(4))) = "msg_menu_mario_waza";
__declspec(section ".rodata") char str_msg_menu_mario_star__802f5f48[22] __attribute__ ((aligned(4))) = "msg_menu_mario_star_p";
__declspec(section ".rodata") char str_msg_menu_mario_coin_802f5f60[20] __attribute__ ((aligned(4))) = "msg_menu_mario_coin";
__declspec(section ".rodata") char str_msg_menu_mario_kaker_802f5f74[22] __attribute__ ((aligned(4))) = "msg_menu_mario_kakera";
__declspec(section ".rodata") char str_msg_menu_mario_scoin_802f5f8c[21] __attribute__ ((aligned(4))) = "msg_menu_mario_scoin";
__declspec(section ".rodata") char str_msg_menu_mario_time_802f5fa4[20] __attribute__ ((aligned(4))) = "msg_menu_mario_time";
__declspec(section ".rodata") char str_msg_hammer_1_802f5fb8[13] __attribute__ ((aligned(4))) = "msg_hammer_1";
__declspec(section ".rodata") char str_msg_hammer_2_802f5fc8[13] __attribute__ ((aligned(4))) = "msg_hammer_2";
__declspec(section ".rodata") char str_msg_hammer_3_802f5fd8[13] __attribute__ ((aligned(4))) = "msg_hammer_3";
__declspec(section ".rodata") char str_msg_boots_1_802f5fe8[12] __attribute__ ((aligned(4))) = "msg_boots_1";
__declspec(section ".rodata") char str_msg_boots_2_802f5ff4[12] __attribute__ ((aligned(4))) = "msg_boots_2";
__declspec(section ".rodata") char str_msg_boots_3_802f6000[12] __attribute__ ((aligned(4))) = "msg_boots_3";

