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

void winMarioMain2(void) {
    ;
}

char* winZenkakuStr(s32 value) {
    sprintf(tmp2, str_PCTd_80423948, value);
    return tmp2;
}

void winMarioExit(void* wp) {
    animPoseRelease(*(s32*)((s32)wp + 0x188));
}


u8 winMarioDisp(void) {
    return 0;
}


u8 fukidashi(double x, double y, void* menu, int type) {
    return 0;
}


s32 winMarioMain(void* pWin) {
    return 0;
}


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

char* unk_801703e8(s32 value, s32 width) {
    extern const char str_PCTPCT0PCTdd_802f6688[];
    extern char unk_803e5f78[];
    char buffer[0x100];

    sprintf(buffer, str_PCTPCT0PCTdd_802f6688, width);
    sprintf(unk_803e5f78, buffer, value);
    return unk_803e5f78;
}
