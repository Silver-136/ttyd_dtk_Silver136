#include "window/win_badge.h"

extern f32 float_184_804236f8;
extern f32 float_0p25_804236fc;
void* pouchGetPtr(void);

void winBadgeExit(void) {
    ;
}


u8 badge_disp(void) {
    return 0;
}


u8 winBadgeDisp(s32 param_1, void* pWin, s32 param_3) {
    return 0;
}


s32 winBadgeMain(void* pWin) {
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
