#include "window/win_party.h"

typedef struct WinPartyDtEntry {
    char pad_00[0xC];
    char* msg;
    char pad_10[0x14];
} WinPartyDtEntry;

extern f32 float_360_80423550;
extern f32 float_neg200_804235fc;
extern f32 float_20_804234fc;
extern WinPartyDtEntry winPartyDt[];
void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);

void winPartyMain2(void* work) {
    extern f32 float_0p25_804235f8;
    f32 target = *(f32*)((s32)work + 0x1E8);
    f32 current = *(f32*)((s32)work + 0x1E4);
    f32 rate = float_0p25_804235f8;

    *(f32*)((s32)work + 0x1E4) = (target - current) * rate + current;
}


void winPartyDisp(s32 cameraId, void* pWin, s32 index) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void* pouchGetPtr(void);
    extern void winBgGX(f32 x, f32 y, void* win, s32 type);
    extern void winKirinukiGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void winNameGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void winWazaGX(f32 x, f32 y, f32 w, f32 h, void* win, s32 type);
    extern void winTexInit(void* data);
    extern void winTexSet(s32 id, Vec3* pos, Vec3* scale, void* color);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void winIconInit(void);
    extern void winIconSet(s32 icon, Vec3* pos, Vec3* scale, void* color);
    extern void winFontInit(void);
    extern void winFontSet(Vec3* pos, Vec3* scale, void* color, char* text, ...);
    extern void winFontSetR(Vec3* pos, Vec3* scale, void* color, char* text, ...);
    extern char* msgSearch(char* key);
    extern char* winZenkakuStr(s32 value);
    Vec3 pos;
    Vec3 scale;
    u32 white = 0xFFFFFFFF;
    u32 gray = 0x7F7F7FFF;
    u8* w = (u8*)pWin;
    u8* pouch = (u8*)pouchGetPtr();
    f32 x = *(f32*)(w + 0xC4 + index * 0x14);
    f32 y = *(f32*)(w + 0xC8 + index * 0x14);
    s32 partyCount = *(s32*)(w + 0x1E0);
    s32 active = *(s32*)(w + 0x1D8);
    s32 member;
    s32 row;

    scale.x = scale.y = scale.z = 1.0f;
    pos.z = 0.0f;
    winBgGX(x, y, pWin, 1);

    if (partyCount != 0) {
        winKirinukiGX(x - 10.0f, y + 136.0f, 240.0f, 76.0f, pWin, 0);

        for (row = 0; row < 2; row++) {
            winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
            GXSetTevColorIn(0, 0, 0, 0, 2);
            GXSetTevAlphaIn(0, 0, 4, 5, 7);
            pos.x = x + 33.0f;
            pos.y = y + 110.0f - (f32)(row * 32);
            winTexSet(0xAF, &pos, &scale, row == 0 ? &white : &gray);
            winFontInit();
            pos.x = x + 45.0f;
            pos.y += 12.0f;
            winFontSet(&pos, &scale, row == 0 ? &white : &gray,
                       msgSearch(row == 0 ? "msg_menu_party_rank" : "msg_menu_party_name"));
        }

        winIconInit();
        pos.x = x + 93.0f;
        pos.y = y + 114.0f;
        winIconSet(0x1A7, &pos, &scale, &white);

        for (row = 0; row < partyCount - 2; row++) {
            pos.x = x + 93.0f + (f32)(row * 34);
            pos.y = y + 78.0f;
            winIconSet(0x57, &pos, &scale, &white);
        }

        winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
        pos.x = x + 178.0f;
        pos.y = y + 114.0f;
        winTexSet(0x10, &pos, &scale, &white);
        winFontInit();
        pos.x = x + 110.0f;
        pos.y = y + 126.0f;
        winFontSetR(&pos, &scale, &white, "%s", winZenkakuStr(*(s16*)(pouch + active * 0xE + 6)));

        winNameGX(x - 230.0f, y - 40.0f, 200.0f, 32.0f, pWin, 0);
    }

    winIconInit();
    for (row = 0; row < 7; row++) {
        member = *(s32*)(w + 0x1E4 + row * 4);
        if (member < 0) {
            continue;
        }
        pos.x = x - 120.0f + (f32)((row & 1) * 180);
        pos.y = y + 70.0f - (f32)((row >> 1) * 50);
        winIconSet(0x120 + member, &pos, &scale, &white);
        winFontInit();
        pos.x += 42.0f;
        winFontSet(&pos, &scale, &white, msgSearch("msg_menu_party_name"));
    }
    winWazaGX(x - 5.0f, y + 50.0f, 260.0f, 144.0f, pWin, 0);
    (void)cameraId;
}

s32 winPartyMain(void* pWin) {
    extern void psndSFXOn(s32 soundId);
    extern s32 pouchGetPartyAttackLv(s32 partyId);
    extern void winMsgEntry(void* pWin, s32 arg1, char* msg, s32 arg3);
    extern f32 float_360_80423550;
    extern f32 float_neg200_804235fc;
    extern f32 float_20_804234fc;
    extern f32 float_neg10_80423600;
    extern f32 float_neg26_80423604;
    extern f32 float_5_804235b4;
    extern WinPartyDtEntry winPartyDt[];

    s32 prev;
    s32 count;
    s32 cursor;
    s32 active;
    s32 i;
    s32 partyId;
    u32 buttons;
    u32 dirs;
    char* msg;
    void* entry;

    if (*(s32*)((s32)pWin + 0x1E0) == 0) {
        return -1;
    }

    switch (*(s32*)((s32)pWin + 0x200)) {
        case 0:
            buttons = *(u32*)((s32)pWin + 4);
            if ((buttons & 0x100) != 0) {
                *(s32*)((s32)pWin + 0x19C) = 1;
                return -2;
            }
            if ((buttons & 0x400) != 0) {
                *(s32*)((s32)pWin + 0x200) = 1;
                *(s32*)((s32)pWin + 0x208) = 0;
                psndSFXOn(0x20012);
            } else {
                if ((buttons & 0x200) != 0) {
                    psndSFXOn(0x20013);
                    *(s32*)((s32)pWin + 0x19C) = 0;
                    return -1;
                }
                if ((buttons & 0x1000) != 0) {
                    *(s32*)((s32)pWin + 0x19C) = 0;
                    return -2;
                }
                dirs = *(u32*)((s32)pWin + 0x10);
                if ((dirs & 0x4000) != 0) {
                    prev = *(s32*)((s32)pWin + 0x1DC);
                    *(s32*)((s32)pWin + 0x1DC) = prev - 1;
                    while (*(s32*)((s32)pWin + 0x1DC) < 0) {
                        *(s32*)((s32)pWin + 0x1DC) += *(s32*)((s32)pWin + 0x1E0);
                    }
                    count = *(s32*)((s32)pWin + 0x1E0);
                    cursor = *(s32*)((s32)pWin + 0x1DC);
                    *(f32*)((s32)pWin + 0x1E8) =
                        (float_360_80423550 / (f32)count) * (f32)cursor;
                    while (*(f32*)((s32)pWin + 0x1E4) < *(f32*)((s32)pWin + 0x1E8)) {
                        *(f32*)((s32)pWin + 0x1E4) += float_360_80423550;
                    }
                    if (prev != *(s32*)((s32)pWin + 0x1DC)) {
                        psndSFXOn(0x209D6);
                    }
                } else if ((dirs & 0x8000) != 0) {
                    prev = *(s32*)((s32)pWin + 0x1DC);
                    *(s32*)((s32)pWin + 0x1DC) = prev + 1;
                    count = *(s32*)((s32)pWin + 0x1E0);
                    while (*(s32*)((s32)pWin + 0x1DC) > count - 1) {
                        *(s32*)((s32)pWin + 0x1DC) -= count;
                    }
                    cursor = *(s32*)((s32)pWin + 0x1DC);
                    *(f32*)((s32)pWin + 0x1E8) =
                        (float_360_80423550 / (f32)count) * (f32)cursor;
                    while (*(f32*)((s32)pWin + 0x1E8) < *(f32*)((s32)pWin + 0x1E4)) {
                        *(f32*)((s32)pWin + 0x1E4) -= float_360_80423550;
                    }
                    if (prev != *(s32*)((s32)pWin + 0x1DC)) {
                        psndSFXOn(0x209D6);
                    }
                }
            }

            partyId = *(s32*)((s32)pWin + 0x1BC + (*(s32*)((s32)pWin + 0x1DC) * 4));
            entry = winPartyDt;
            active = 0;
            for (i = 0; i < 8; i++, active++, entry = (void*)((s32)entry + 0x24)) {
                if (partyId == *(s32*)entry) {
                    *(s32*)((s32)pWin + 0x1D8) = active;
                    break;
                }
            }

            *(f32*)((s32)pWin + 0x158) = float_neg200_804235fc;
            *(f32*)((s32)pWin + 0x15C) = float_20_804234fc;
            active = *(s32*)((s32)pWin + 0x1D8);
            partyId = *(s32*)((s32)winPartyDt + active * 0x24);
            *(s32*)((s32)pWin + 0x204) = pouchGetPartyAttackLv(partyId) + 2;
            msg = *(char**)((s32)winPartyDt + active * 0x24 + 0xC);
            winMsgEntry(pWin, 0, msg, 0);
            break;

        case 1:
            dirs = *(u32*)((s32)pWin + 0x10);
            if ((dirs & 0x1000) != 0) {
                (*(s32*)((s32)pWin + 0x208))--;
                if (*(s32*)((s32)pWin + 0x208) < 0) {
                    *(s32*)((s32)pWin + 0x208) = *(s32*)((s32)pWin + 0x204) - 1;
                }
                psndSFXOn(0x20005);
            } else if ((dirs & 0x2000) != 0) {
                (*(s32*)((s32)pWin + 0x208))++;
                if (*(s32*)((s32)pWin + 0x208) >= *(s32*)((s32)pWin + 0x204)) {
                    *(s32*)((s32)pWin + 0x208) = 0;
                }
                psndSFXOn(0x20005);
            } else {
                buttons = *(u32*)((s32)pWin + 4);
                if ((buttons & 0x200) != 0) {
                    *(s32*)((s32)pWin + 0x200) = 0;
                    psndSFXOn(0x20013);
                } else if ((buttons & 0x1000) != 0) {
                    *(s32*)((s32)pWin + 0x19C) = 0;
                    return -2;
                }
            }
            active = *(s32*)((s32)pWin + 0x1D8);
            cursor = *(s32*)((s32)pWin + 0x208);
            *(f32*)((s32)pWin + 0x158) = float_neg10_80423600;
            *(f32*)((s32)pWin + 0x15C) = float_neg26_80423604 * (f32)cursor + float_5_804235b4;
            msg = *(char**)(*(s32*)((s32)winPartyDt + active * 0x24 + 0x10) + cursor * 4 + 4);
            winMsgEntry(pWin, 0, msg, 0);
            break;
    }
    return 0;
}

void winPartyInit(void* pWin) {
    extern void* pouchGetPtr(void);
    extern s32 pouchGetPartyColor(s32 id);
    extern s32 animPoseEntry(char* name, s32 arg);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 loop);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern s32 marioBgmodeChk(void);
    extern s32 marioGetParty(void);
    extern void* marioGetPtr(void);
    extern s32 pouchGetPartyAttackLv(s32 partyId);
    extern WinPartyDtEntry winPartyDt[];
    extern f32 float_360_80423550;
    extern char str_EFF_m_yoshi_802f4e90[];
    extern char str_EFF_m_yoshi2_802f51f0[];
    extern char str_EFF_m_yoshi3_802f5200[];
    extern char str_EFF_m_yoshi4_802f5210[];
    extern char str_EFF_m_yoshi5_802f5220[];
    extern char str_EFF_m_yoshi6_802f5230[];
    extern char str_EFF_m_yoshi7_802f5240[];
    extern char str_W_1_804234b8[];
    extern char str_R_5_804234c8[];
    extern char str_R_6_80423568[];
    extern char str_R_7_8042356c[];
    extern char str_R_8_80423570[];
    extern char str_R_9_80423574[];
    extern char str_R_10_80423578[];
    extern char str_R_11_80423580[];

    void* pouch;
    void* dt;
    void* scan;
    char* eff;
    char* anim;
    s32 i;
    s32 j;
    s32 partyId;
    s32 poseId;
    s32 color;
    s32 active;
    s32 currentParty;
    f32 angle;

    pouch = pouchGetPtr();
    dt = winPartyDt;
    *(s32*)((s32)pWin + 0x1E0) = 0;

    for (i = 0; i < 7; i++) {
        partyId = *(s32*)((s32)dt + i * 0x24);
        scan = pouch;
        for (j = 0; j < 8; j++) {
            if (j == partyId &&
                (*(u16*)((s32)scan + j * 0xE) & 1) != 0 &&
                (*(u16*)((s32)scan + j * 0xE) & 2) == 0) {
                if ((*(u16*)pWin & 0x2000) == 0) {
                    if (j == 4) {
                        color = pouchGetPartyColor(4);
                        switch (color) {
                            case 0:
                                eff = str_EFF_m_yoshi_802f4e90;
                                break;
                            case 1:
                                eff = str_EFF_m_yoshi2_802f51f0;
                                break;
                            case 2:
                                eff = str_EFF_m_yoshi3_802f5200;
                                break;
                            case 3:
                                eff = str_EFF_m_yoshi4_802f5210;
                                break;
                            case 4:
                                eff = str_EFF_m_yoshi5_802f5220;
                                break;
                            case 5:
                                eff = str_EFF_m_yoshi6_802f5230;
                                break;
                            default:
                                eff = str_EFF_m_yoshi7_802f5240;
                                break;
                        }
                        anim = str_W_1_804234b8;
                    } else {
                        eff = *(char**)((s32)dt + i * 0x24 + 0x10);
                        anim = *(char**)((s32)dt + i * 0x24 + 0x14);
                    }
                } else {
                    eff = *(char**)((s32)dt + i * 0x24 + 0x18);
                    color = pouchGetPartyColor(4);
                    switch (color) {
                        case 0:
                            anim = str_R_5_804234c8;
                            break;
                        case 1:
                            anim = str_R_6_80423568;
                            break;
                        case 2:
                            anim = str_R_7_8042356c;
                            break;
                        case 3:
                            anim = str_R_8_80423570;
                            break;
                        case 4:
                            anim = str_R_9_80423574;
                            break;
                        case 5:
                            anim = str_R_10_80423578;
                            break;
                        default:
                            anim = str_R_11_80423580;
                            break;
                    }
                }
                poseId = animPoseEntry(eff, 0);
                *(s32*)((s32)pWin + 0x1A0 + *(s32*)((s32)pWin + 0x1E0) * 4) = poseId;
                animPoseSetAnim(poseId, anim, 1);
                animPoseSetMaterialFlagOn(poseId, 0x1800);
                *(s32*)((s32)pWin + 0x1BC + *(s32*)((s32)pWin + 0x1E0) * 4) = j;
                (*(s32*)((s32)pWin + 0x1E0))++;
            }
        }
    }

    *(s32*)((s32)pWin + 0x1DC) = 0;
    for (i = 0; i < *(s32*)((s32)pWin + 0x1E0); i++) {
        if (marioBgmodeChk() == 0) {
            currentParty = marioGetParty();
        } else {
            currentParty = *(s8*)((s32)marioGetPtr() + 0x247);
        }
        if (currentParty == *(s32*)((s32)pWin + 0x1BC + i * 4)) {
            *(s32*)((s32)pWin + 0x1DC) = i;
            break;
        }
    }

    active = 0;
    for (i = 0; i < 8; i++) {
        if (*(s32*)((s32)pWin + 0x1BC + *(s32*)((s32)pWin + 0x1DC) * 4) ==
            *(s32*)((s32)dt + i * 0x24)) {
            active = i;
            break;
        }
    }
    *(s32*)((s32)pWin + 0x1D8) = active;
    *(s32*)((s32)pWin + 0x1FC) = *(s32*)((s32)pWin + 0x1DC);
    angle = (float_360_80423550 / (f32)*(s32*)((s32)pWin + 0x1E0)) *
            (f32)*(s32*)((s32)pWin + 0x1DC);
    *(f32*)((s32)pWin + 0x1E8) = angle;
    *(f32*)((s32)pWin + 0x1E4) = angle;
    *(s32*)((s32)pWin + 0x200) = 0;
    *(s32*)((s32)pWin + 0x204) =
        pouchGetPartyAttackLv(*(s32*)((s32)dt + active * 0x24)) + 2;

    color = pouchGetPartyColor(4);
    if (color < 0) {
        color = 6;
    }
    if (color > 6) {
        color = 6;
    }
    *(s32*)((s32)winPartyDt + 0x70) = 0x160 + color;
}

void winPartyInit2(void* pWin) {
    f32 angle;
    f32 scale;
    s32 denom = *(s32*)((s32)pWin + 0x1E0);
    s32 numer = *(s32*)((s32)pWin + 0x1DC);
    f32 neg = float_neg200_804235fc;
    f32 twenty = float_20_804234fc;

    scale = float_360_80423550 / (f32)denom;
    angle = scale * (f32)numer;
    *(f32*)((s32)pWin + 0x1E8) = angle;
    *(f32*)((s32)pWin + 0x1E4) = angle;
    *(s32*)((s32)pWin + 0x200) = 0;
    *(f32*)((s32)pWin + 0x158) = neg;
    *(f32*)((s32)pWin + 0x15C) = twenty;
    winMsgEntry(pWin, 0, winPartyDt[*(s32*)((s32)pWin + 0x1D8)].msg, 0);
}


u8 winPartyExit(void* pWin) {
    extern void animPoseRelease(s32 poseId);
    s32 i;
    void* scan;

    i = 0;
    scan = pWin;
    while (i < *(s32*)((s32)pWin + 0x1E0)) {
        s32 poseId = *(s32*)((s32)scan + 0x1A0);
        if (poseId != -1) {
            animPoseRelease(poseId);
        }
        scan = (void*)((s32)scan + 4);
        i++;
    }
}

