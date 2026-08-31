#include "window/win_party.h"


const char str_msg_menu_party_hp_802f4f30[] = "msg_menu_party_hp";
const char str_msg_menu_party_rank_802f4f44[] = "msg_menu_party_rank";

const char str_W_1_804234b8[] = "W_1";
const char str_R_2_804234bc[] = "R_2";
const char str_R_3_804234c0[] = "R_3";
const char str_R_4_804234c4[] = "R_4";
const char str_R_5_804234c8[] = "R_5";
const char str_R_12_804234cc[] = "R_12";
const char str_R_13_804234d4[] = "R_13";
const char str_R_14_804234dc[] = "R_14";

const u32 dat_804234e4[1] = { 0xFFFFFFFF };
const u32 dat_804234e8[1] = { 0x000000FF };
const u32 dat_804234ec[1] = { 0xA73C3CFF };

const u32 dat_804234f0[1] = { 0x00000080 };
const u32 dat_804234f4[1] = { 0x808080FF };
const f32 float_10_804234f8[1] = { 10.0f };
const f32 float_20_804234fc[1] = { 20.0f };
const f32 float_116_80423500[1] = { 116.0f };
const f32 float_240_80423504[1] = { 240.0f };
const f32 float_76_80423508[1] = { 76.0f };
const f32 float_45_8042350c[1] = { 45.0f };
const f32 float_2_80423510[1] = { 2.0f };
const f32 float_110_80423514[1] = { 110.0f };
const f32 float_1_80423518[1] = { 1.0f };
const f32 float_0p9_8042351c[1] = { 0.9f };
const f32 float_0p5_80423520[1] = { 0.5f };
const f32 float_4_80423524[1] = { 4.0f };
const f32 float_12_80423528[1] = { 12.0f };
const f32 float_21_8042352c[1] = { 21.0f };
const f32 float_60_80423530[1] = { 60.0f };
const f32 float_32_80423534[1] = { 32.0f };
const f32 float_8_80423538[1] = { 8.0f };
const f32 float_170_8042353c[1] = { 170.0f };
const char str_PCTs_80423540[] = "%s";
const f32 float_180_80423544[1] = { 180.0f };
const f32 float_0p5236_80423548[1] = { 0.5235988f };
const f32 float_deg2rad_8042354c[1] = { 0.017453292f };
const f32 float_360_80423550[1] = { 360.0f };
const f32 float_140_80423554[1] = { 140.0f };
const f32 float_0p8_80423558[1] = { 0.8f };
const f32 float_0p6_8042355c[1] = { 0.6f };
const f32 float_neg1_80423560[1] = { -1.0f };
const f32 float_3p1416_80423564[1] = { 3.1415927f };
const char str_R_6_80423568[] = "R_6";
const char str_R_7_8042356c[] = "R_7";
const char str_R_8_80423570[] = "R_8";
const char str_R_9_80423574[] = "R_9";
const char str_R_10_80423578[] = "R_10";
const char str_R_11_80423580[] = "R_11";
const f32 float_neg0p8_80423588[1] = { -0.8f };
const char str_Z_5_8042358c[] = "Z_5";
const char str_Z_6_80423590[] = "Z_6";
const char str_Z_7_80423594[] = "Z_7";
const char str_Z_8_80423598[] = "Z_8";
const char str_Z_9_8042359c[] = "Z_9";
const char str_Z_10_804235a0[] = "Z_10";
const char str_Z_11_804235a8[] = "Z_11";
const f32 float_0_804235b0[1] = { 0.0f };
const f32 float_5_804235b4[1] = { 5.0f };
const f32 float_230_804235b8[1] = { 230.0f };
const f32 float_50_804235bc[1] = { 50.0f };
const f32 float_200_804235c0[1] = { 200.0f };
const f32 float_neg230_804235c4[1] = { -230.0f };
const f32 float_90_804235c8[1] = { 90.0f };
const f32 float_30_804235cc[1] = { 30.0f };
const f32 float_100_804235d0[1] = { 100.0f };
const f32 float_150_804235d4[1] = { 150.0f };
const f32 float_16_804235d8[1] = { 16.0f };
const f32 float_0p7_804235dc[1] = { 0.7f };
const f32 float_250_804235e0[1] = { 250.0f };
const f32 float_260_804235e4[1] = { 260.0f };
const f32 float_144_804235e8[1] = { 144.0f };
const f32 float_238_804235ec[1] = { 238.0f };
const f32 float_17_804235f0[1] = { 17.0f };
const f32 float_175_804235f4[1] = { 175.0f };
const f32 float_0p25_804235f8[1] = { 0.25f };
const f32 float_neg200_804235fc[1] = { -200.0f };
const f32 float_neg10_80423600[1] = { -10.0f };
const f32 float_neg26_80423604[1] = { -26.0f };


typedef struct WinPartyDtEntry {
    char pad_00[0xC];
    char* msg;
    char pad_10[0x14];
} WinPartyDtEntry;

extern WinPartyDtEntry winPartyDt[];
extern void* pouchGetPtr(void);
extern s32 pouchGetPartyAttackLv(s32 partyId);
void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);

void winPartyInit(void* pWin) {
    extern s32 pouchGetPartyColor(s32 id);
    extern s32 animPoseEntry(char* name, s32 arg);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 loop);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern s32 marioBgmodeChk(void);
    extern s32 marioGetParty(void);
    extern void* marioGetPtr(void);
    extern char str_EFF_m_yoshi_802f4e90[];
    extern char str_EFF_m_yoshi2_802f51f0[];
    extern char str_EFF_m_yoshi3_802f5200[];
    extern char str_EFF_m_yoshi4_802f5210[];
    extern char str_EFF_m_yoshi5_802f5220[];
    extern char str_EFF_m_yoshi6_802f5230[];
    extern char str_EFF_m_yoshi7_802f5240[];

    void* pouch;
    void* dt;
    void* scan;
    void* dtScan;
    char* eff;
    char* anim;
    s32 i;
    s32 j;
    s32 partyId;
    s32 poseId;
    s32 color;
    s32 active;
    s32 groups;
    s32 currentParty;
    f32 angle;

    pouch = pouchGetPtr();
    dt = winPartyDt;
    *(s32*)((s32)pWin + 0x1E0) = 0;

    i = 0;
    dtScan = dt;
    do {
        partyId = *(s32*)dtScan;
        scan = pouch;
        j = 0;
        do {
            if (j == partyId &&
                (*(u16*)scan & 1) != 0 &&
                (*(u16*)scan & 2) == 0) {
                if ((*(u16*)pWin & 0x2000) != 0) {
                    eff = *(char**)((s32)dtScan + 0x18);
                    color = pouchGetPartyColor(4);
                    switch (color) {
                        case 0:
                            anim = (char*)str_R_5_804234c8;
                            break;
                        case 1:
                            anim = (char*)str_R_6_80423568;
                            break;
                        case 2:
                            anim = (char*)str_R_7_8042356c;
                            break;
                        case 3:
                            anim = (char*)str_R_8_80423570;
                            break;
                        case 4:
                            anim = (char*)str_R_9_80423574;
                            break;
                        case 5:
                            anim = (char*)str_R_10_80423578;
                            break;
                        default:
                            anim = (char*)str_R_11_80423580;
                            break;
                    }
                } else if (j == 4) {
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
                    anim = (char*)str_W_1_804234b8;
                } else {
                    eff = *(char**)((s32)dtScan + 0x10);
                    anim = *(char**)((s32)dtScan + 0x14);
                }
                poseId = animPoseEntry(eff, 0);
                *(s32*)((s32)pWin + 0x1A0 + *(s32*)((s32)pWin + 0x1E0) * 4) = poseId;
                animPoseSetAnim(*(s32*)((s32)pWin + 0x1A0 + *(s32*)((s32)pWin + 0x1E0) * 4), anim, 1);
                animPoseSetMaterialFlagOn(*(s32*)((s32)pWin + 0x1A0 + *(s32*)((s32)pWin + 0x1E0) * 4), 0x1800);
                *(s32*)((s32)pWin + 0x1BC + *(s32*)((s32)pWin + 0x1E0) * 4) = j;
                (*(s32*)((s32)pWin + 0x1E0))++;
            }
            j++;
            scan = (void*)((s32)scan + 0xE);
        } while (j < 8);
        i++;
        dtScan = (void*)((s32)dtScan + 0x24);
    } while (i < 7);

    *(s32*)((s32)pWin + 0x1DC) = 0;
    scan = pWin;
    for (i = 0; i < *(s32*)((s32)pWin + 0x1E0); i++) {
        if (marioBgmodeChk() == 0) {
            currentParty = marioGetParty();
        } else {
            currentParty = *(s8*)((s32)marioGetPtr() + 0x247);
        }
        if (currentParty == *(s32*)((s32)scan + 0x1BC)) {
            *(s32*)((s32)pWin + 0x1DC) = i;
            break;
        }
        scan = (void*)((s32)scan + 4);
    }

    active = 0;
    groups = 2;
    dtScan = dt;
    do {
        partyId = *(s32*)((s32)pWin + 0x1BC + *(s32*)((s32)pWin + 0x1DC) * 4);
        if (partyId == *(s32*)dtScan) {
            *(s32*)((s32)pWin + 0x1D8) = active;
            break;
        }
        if (partyId == *(s32*)((s32)dtScan + 0x24)) {
            *(s32*)((s32)pWin + 0x1D8) = active + 1;
            break;
        }
        if (partyId == *(s32*)((s32)dtScan + 0x48)) {
            *(s32*)((s32)pWin + 0x1D8) = active + 2;
            break;
        }
        if (partyId == *(s32*)((s32)dtScan + 0x6C)) {
            *(s32*)((s32)pWin + 0x1D8) = active + 3;
            break;
        }
        dtScan = (void*)((s32)dtScan + 0x90);
        active += 4;
        groups--;
    } while (groups != 0);
    *(s32*)((s32)pWin + 0x1FC) = *(s32*)((s32)pWin + 0x1DC);
    angle = (float_360_80423550[0] / (f32)*(s32*)((s32)pWin + 0x1E0)) *
            (f32)*(s32*)((s32)pWin + 0x1DC);
    *(f32*)((s32)pWin + 0x1E8) = angle;
    *(f32*)((s32)pWin + 0x1E4) = angle;
    *(s32*)((s32)pWin + 0x200) = 0;
    *(s32*)((s32)pWin + 0x204) =
        pouchGetPartyAttackLv(*(s32*)((s32)dt + *(s32*)((s32)pWin + 0x1D8) * 0x24)) + 2;

    color = pouchGetPartyColor(4);
    switch (color) {
        case 0:
            *(s32*)((s32)winPartyDt + 0x70) = 0x160;
            break;
        case 1:
            *(s32*)((s32)winPartyDt + 0x70) = 0x161;
            break;
        case 2:
            *(s32*)((s32)winPartyDt + 0x70) = 0x162;
            break;
        case 3:
            *(s32*)((s32)winPartyDt + 0x70) = 0x163;
            break;
        case 4:
            *(s32*)((s32)winPartyDt + 0x70) = 0x164;
            break;
        case 5:
            *(s32*)((s32)winPartyDt + 0x70) = 0x165;
            break;
        default:
            *(s32*)((s32)winPartyDt + 0x70) = 0x166;
            break;
    }
}

void winPartyInit2(void* pWin) {
    f32 angle;
    f32 scale;
    s32 denom = *(s32*)((s32)pWin + 0x1E0);
    s32 numer = *(s32*)((s32)pWin + 0x1DC);
    f32 neg = float_neg200_804235fc[0];
    f32 twenty = float_20_804234fc[0];

    scale = float_360_80423550[0] / (f32)denom;
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

s32 winPartyMain(void* pWin) {
    extern void psndSFXOn(s32 soundId);
    extern void winMsgEntry(void* pWin, s32 arg1, char* msg, s32 arg3);

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
                        (float_360_80423550[0] / (f32)count) * (f32)cursor;
                    while (*(f32*)((s32)pWin + 0x1E4) < *(f32*)((s32)pWin + 0x1E8)) {
                        *(f32*)((s32)pWin + 0x1E4) += float_360_80423550[0];
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
                        (float_360_80423550[0] / (f32)count) * (f32)cursor;
                    while (*(f32*)((s32)pWin + 0x1E8) < *(f32*)((s32)pWin + 0x1E4)) {
                        *(f32*)((s32)pWin + 0x1E4) -= float_360_80423550[0];
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

            *(f32*)((s32)pWin + 0x158) = float_neg200_804235fc[0];
            *(f32*)((s32)pWin + 0x15C) = float_20_804234fc[0];
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
            *(f32*)((s32)pWin + 0x158) = float_neg10_80423600[0];
            *(f32*)((s32)pWin + 0x15C) = float_neg26_80423604[0] * (f32)cursor + float_5_804235b4[0];
            msg = *(char**)(*(s32*)((s32)winPartyDt + active * 0x24 + 0x10) + cursor * 4 + 4);
            winMsgEntry(pWin, 0, msg, 0);
            break;
    }
    return 0;
}

void winPartyMain2(void* work) {
    f32 target = *(f32*)((s32)work + 0x1E8);
    f32 current = *(f32*)((s32)work + 0x1E4);
    f32 rate = float_0p25_804235f8[0];

    *(f32*)((s32)work + 0x1E4) = (target - current) * rate + current;
}
void winPartyDisp(s32 cameraId, void* pWin, s32 index) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef f32 Mtx[3][4];
    typedef union DoubleBits {
        f64 value;
        struct { u32 hi, lo; } words;
    } DoubleBits;

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
    extern u32 FontGetMessageWidth(char* text);
    extern void winZClear(void);
    extern void PSMTXRotRad(Mtx m, s32 axis, f32 radians);
    extern void PSMTXMultVec(Mtx m, Vec3* src, Vec3* dst);
    extern void winKageGX(
        f32 x, f32 y, f32 z, f32 scale, void* win, void* color);
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void* animPoseGetAnimDataPtr(s32 poseId);
    extern void animPoseSetMaterialFlagOff(s32 poseId, u32 flag);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 loop);
    extern void animPoseSetLocalTime(s32 poseId, f32 time);
    extern void animPoseMain(s32 poseId);
    extern void animPoseVivianMain(s32 poseId, Vec3* pos);
    extern void animPoseDrawMtx(
        s32 poseId, Mtx mtx, s32 mode, f32 param4, f32 param5);
    extern char* pouchGetYoshiName(void);
    extern void winFontSetEdge(
        Vec3* pos, Vec3* scale, void* color, char* text, ...);
    extern void winFontSetWidth(
        Vec3* pos, Vec3* scale, void* color, char* text, f32 width, ...);
    extern s32 sprintf(char* dst, const char* fmt, ...);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern void* gp;
    extern u8 itemDataTable[];

    extern u8 str_name_party0_802f4dd8[];

    Vec3 pos;
    Vec3 scale;
    Vec3 textPos;
    Vec3 textScale;
    u32 white = dat_804234e4[0];
    u32 gray = dat_804234e8[0];
    u8* w = (u8*)pWin;
    u8* roBase = str_name_party0_802f4dd8;
    f32 x = *(f32*)(w + 0xC4 + index * 0x14);
    f32 y = *(f32*)(w + 0xC8 + index * 0x14);
    s32 row;
    DoubleBits widthBits;

    winBgGX(x, y, pWin, 1);

    if (*(s32*)(w + 0x1E0) != 0) {
        u8* pouch = (u8*)pouchGetPtr();
        s32 active = *(s32*)(w + 0x1D8);
        s32 activePartyId = *(s32*)((u8*)winPartyDt + active * 0x24);
        u8* activePouch = pouch + activePartyId * 0xE;
        f32 panelBaseX;
        f32 panelBaseY;
        u32 rowColors[2];
        u32 texColors[2];
        char* primaryKeys[2];
        char* secondaryKeys[2];
        u32* rowColorPtr;
        u32* texColorPtr;
        char** primaryKeyPtr;
        char** secondaryKeyPtr;
        s32 rowOffset = 0;

        Vec3 panelTexScale;
        Vec3 panelTexPosTmp;
        Vec3 panelTexPosCall;
        Vec3 primaryScaleTmp;
        Vec3 primaryScaleCall;
        Vec3 primaryPosTmp;
        Vec3 primaryPosCall;
        Vec3 secondaryScale;
        Vec3 secondaryPosTmp;
        Vec3 secondaryPosCall;

        winKirinukiGX(
            x - float_10_804234f8[0],
            y + float_116_80423500[0] + float_20_804234fc[0],
            float_240_80423504[0], float_76_80423508[0], pWin, 0);

        panelBaseX =
            (x - float_10_804234f8[0]) + float_45_8042350c[0];
        panelBaseY = y + float_110_80423514[0];

        texColors[0] = *(u32*)(roBase + 0x150);
        texColors[1] = *(u32*)(roBase + 0x154);
        rowColors[0] = dat_804234ec[0];
        rowColors[1] = gray;
        primaryKeys[0] = *(char**)(roBase + 0x180);
        primaryKeys[1] = *(char**)(roBase + 0x184);
        secondaryKeys[0] = *(char**)(roBase + 0x1A0);
        secondaryKeys[1] = *(char**)(roBase + 0x1A4);

        texColorPtr = texColors;
        rowColorPtr = rowColors;
        primaryKeyPtr = primaryKeys;
        secondaryKeyPtr = secondaryKeys;

        for (row = 0; row < 2; row++) {
            f32 rowScale =
                (row == 0)
                    ? float_1_80423518[0]
                    : float_0p9_8042351c[0];
            u32 primaryWidth =
                FontGetMessageWidth(msgSearch(*primaryKeyPtr)) & 0xFFFF;
            u32 secondaryWidth =
                FontGetMessageWidth(msgSearch(*secondaryKeyPtr)) & 0xFFFF;
            f32 primaryWidthF;
            f32 secondaryWidthF;
            f32 secondaryHalf;
            f32 rowY = panelBaseY - (f32)rowOffset;

            widthBits.words.hi = 0x43300000;
            widthBits.words.lo = primaryWidth;
            primaryWidthF =
                (f32)(widthBits.value - *(const f64*)(roBase + 0x408));

            widthBits.words.hi = 0x43300000;
            widthBits.words.lo = secondaryWidth;
            secondaryWidthF =
                (f32)(widthBits.value - *(const f64*)(roBase + 0x408));
            secondaryHalf =
                float_0p5_80423520[0] * secondaryWidthF;

            winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));
            GXSetTevColorIn(0, 15, 15, 15, 2);
            GXSetTevAlphaIn(0, 7, 1, 4, 7);

            panelTexPosTmp = *(const Vec3*)(roBase + 0x1A8);
            panelTexScale = *(const Vec3*)(roBase + 0x1B4);
            panelTexPosTmp.x =
                panelBaseX - float_2_80423510[0];
            panelTexPosTmp.y =
                rowY - float_4_80423524[0];
            panelTexPosCall = panelTexPosTmp;
            winTexSet(
                0xAF, &panelTexPosCall, &panelTexScale, texColorPtr);

            winFontInit();

            primaryScaleTmp = *(const Vec3*)(roBase + 0x1CC);
            primaryPosTmp = *(const Vec3*)(roBase + 0x1C0);
            primaryScaleTmp.x = rowScale;
            primaryScaleTmp.y = rowScale;
            primaryScaleTmp.z = rowScale;
            primaryPosTmp.x =
                panelBaseX -
                primaryWidthF * rowScale *
                    float_0p5_80423520[0];
            primaryPosTmp.y =
                rowY + float_12_80423528[0];

            primaryScaleCall = primaryScaleTmp;
            primaryPosCall = primaryPosTmp;
            winFontSet(
                &primaryPosCall, &primaryScaleCall, rowColorPtr,
                msgSearch(*primaryKeyPtr));

            if (row < 1) {
                secondaryPosTmp =
                    *(const Vec3*)(roBase + 0x1D8);
                secondaryScale =
                    *(const Vec3*)(roBase + 0x1E4);
                secondaryPosTmp.x =
                    panelBaseX -
                    secondaryHalf * float_0p5_80423520[0];
                secondaryPosTmp.y =
                    rowY + float_21_8042352c[0];

                secondaryPosCall = secondaryPosTmp;
                winFontSet(
                    &secondaryPosCall, &secondaryScale, rowColorPtr,
                    msgSearch(*secondaryKeyPtr));
            }

            rowOffset += 32;
            texColorPtr++;
            rowColorPtr++;
            primaryKeyPtr++;
            secondaryKeyPtr++;
        }

        winIconInit();

        {
            Vec3 partnerIconPosTmp =
                *(const Vec3*)(roBase + 0x1F0);
            Vec3 partnerIconPosCall;
            Vec3 partnerIconScale =
                *(const Vec3*)(roBase + 0x1FC);
            u32 partnerIconColor;

            partnerIconPosTmp.x =
                panelBaseX + float_60_80423530[0];
            partnerIconPosTmp.y =
                panelBaseY + float_4_80423524[0];

            partnerIconPosCall = partnerIconPosTmp;
            partnerIconColor = white;
            winIconSet(
                0x1A7, &partnerIconPosCall, &partnerIconScale,
                &partnerIconColor);
        }

        {
            s32 iconIndex = 0;
            s32 iconXOffset = 0;

            while (iconIndex < *(s32*)(w + 0x204) - 2) {
                pos = *(const Vec3*)(roBase + 0x208);
                scale = *(const Vec3*)(roBase + 0x214);
                pos.x =
                    panelBaseX + float_60_80423530[0] +
                    (f32)iconXOffset;
                pos.y =
                    panelBaseY - float_32_80423534[0] +
                    float_4_80423524[0];
                winIconSet(
                    *(u16*)(itemDataTable + 0xDB8),
                    &pos, &scale, &white);

                iconXOffset += 34;
                iconIndex++;
            }
        }

        winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));

        pos = *(const Vec3*)(roBase + 0x220);
        scale = *(const Vec3*)(roBase + 0x22C);
        pos.x =
            (x - float_10_804234f8[0]) +
            float_170_8042353c[0] + float_8_80423538[0];
        pos.y = panelBaseY + float_4_80423524[0];
        winTexSet(0x10, &pos, &scale, &white);

        winFontInit();

        textPos = *(const Vec3*)(roBase + 0x238);
        textScale = *(const Vec3*)(roBase + 0x244);
        textPos.x = x + float_110_80423514[0];
        textPos.y = panelBaseY + float_16_804235d8[0];
        winFontSetR(
            &textPos, &textScale, &gray,
            (char*)str_PCTs_80423540,
            winZenkakuStr(*(s16*)(activePouch + 6)));

        textPos = *(const Vec3*)(roBase + 0x250);
        textScale = *(const Vec3*)(roBase + 0x25C);
        textPos.x =
            (x - float_10_804234f8[0]) +
            float_180_80423544[0] + float_8_80423538[0];
        textPos.y = panelBaseY + float_16_804235d8[0];
        winFontSet(
            &textPos, &textScale, &gray,
            (char*)str_PCTs_80423540,
            winZenkakuStr(*(s16*)(activePouch + 2)));

        {
            Mtx xRot;
            Mtx yRot;
            u32 shadowColor;

            winZClear();

            for (row = 0; row < *(s32*)(w + 0x1E0); row++) {
                Vec3 circlePos = *(const Vec3*)(roBase + 0x268);
                f32 angle =
                    float_deg2rad_8042354c[0] *
                    ((float_360_80423550[0] / (f32)*(s32*)(w + 0x1E0)) * (f32)row -
                     *(f32*)(w + 0x1E4));

                PSMTXRotRad(xRot, 'x', float_0p5236_80423548[0]);
                PSMTXRotRad(yRot, 'y', angle);
                PSMTXMultVec(yRot, &circlePos, &circlePos);
                PSMTXMultVec(xRot, &circlePos, &circlePos);

                shadowColor = dat_804234f0[0];
                winKageGX(
                    x - float_140_80423554[0] + circlePos.x,
                    float_20_804234fc[0] + y + circlePos.y,
                    circlePos.z,
                    (row == *(s32*)(w + 0x1DC))
                        ? float_0p8_80423558[0]
                        : float_0p6_8042355c[0],
                    pWin, &shadowColor);
            }
        }

        {
            Mtx xRot;
            Mtx yRot;
            Mtx drawMtx;
            Mtx scaleMtx;
            Mtx faceMtx;
            u8* partyIter = w;

            for (row = 0; row < *(s32*)(w + 0x1E0); row++) {
                Vec3 circlePos = *(const Vec3*)(roBase + 0x274);
                s32 poseId = *(s32*)(partyIter + 0x1A0);

                if (poseId != -1) {
                    void* animData = animPoseGetAnimDataPtr(poseId);
                    f32 angle;

                    PSMTXRotRad(
                        xRot, 'x', float_0p5236_80423548[0]);

                    angle =
                        float_deg2rad_8042354c[0] *
                        ((float_360_80423550[0] /
                          (f32)*(s32*)(w + 0x1E0)) *
                             (f32)row -
                         *(f32*)(w + 0x1E4));

                    PSMTXRotRad(yRot, 'y', angle);
                    PSMTXMultVec(yRot, &circlePos, &circlePos);
                    PSMTXMultVec(xRot, &circlePos, &circlePos);

                    if (row == *(s32*)(w + 0x1DC)) {
                        s32 dtIndex;
                        u32 memberId;
                        char* anim;
                        s32 color;
                        u32 duration;
                        u32 retrace;

                        PSMTXTrans(
                            drawMtx,
                            x - float_140_80423554[0] + circlePos.x,
                            float_20_804234fc[0] + y + circlePos.y,
                            circlePos.z);
                        PSMTXScale(
                            scaleMtx,
                            float_1_80423518[0],
                            float_1_80423518[0],
                            float_neg1_80423560[0]);
                        PSMTXRotRad(
                            faceMtx, 'y', float_3p1416_80423564[0]);
                        PSMTXConcat(drawMtx, scaleMtx, drawMtx);
                        PSMTXConcat(drawMtx, faceMtx, drawMtx);

                        animPoseSetMaterialFlagOff(poseId, 0x40);

                        memberId = *(u32*)(partyIter + 0x1BC);
                        dtIndex = 0;
                        if ((memberId !=
                             *(u32*)((u8*)winPartyDt + 0x00)) &&
                            (dtIndex = 1,
                             memberId !=
                             *(u32*)((u8*)winPartyDt + 0x24)) &&
                            (dtIndex = 2,
                             memberId !=
                                 *(u32*)((u8*)winPartyDt + 0x48) &&
                             (dtIndex = 3,
                              memberId !=
                                  *(u32*)((u8*)winPartyDt + 0x6C) &&
                              (dtIndex = 4,
                               memberId !=
                                   *(u32*)((u8*)winPartyDt + 0x90)) &&
                              (dtIndex = 5,
                               memberId !=
                                   *(u32*)((u8*)winPartyDt + 0xB4)))) &&
                            (dtIndex = 6,
                             memberId !=
                             *(u32*)((u8*)winPartyDt + 0xD8))) {
                            dtIndex = 7;
                        }

                        if ((*(u16*)w & 0x2000) == 0) {
                            anim =
                                *(char**)((u8*)winPartyDt +
                                          dtIndex * 0x24 + 0x14);
                        } else {
                            u8* dtEntry =
                                (u8*)winPartyDt + dtIndex * 0x24;

                            anim = *(char**)(dtEntry + 0x1C);

                            if (*(s32*)dtEntry == 4) {
                                color = pouchGetPartyColor(4);
                                if (color == 0) {
                                    anim = (char*)str_R_5_804234c8;
                                } else if (color == 1) {
                                    anim = (char*)str_R_6_80423568;
                                } else if (color == 2) {
                                    anim = (char*)str_R_7_8042356c;
                                } else if (color == 3) {
                                    anim = (char*)str_R_8_80423570;
                                } else if (color == 4) {
                                    anim = (char*)str_R_9_80423574;
                                } else if (color == 5) {
                                    anim = (char*)str_R_10_80423578;
                                } else {
                                    anim = (char*)str_R_11_80423580;
                                }
                            }
                        }

                        animPoseSetAnim(poseId, anim, 0);

                        duration =
                            (u32)(s32)*(f32*)(
                                (u8*)*(void**)((u8*)animData + 0x24) + 8);
                        retrace = *(u32*)((u8*)gp + 0x1C);

                        widthBits.words.hi = 0x43300000;
                        widthBits.words.lo = retrace % duration;
                        animPoseSetLocalTime(
                            poseId,
                            (f32)(
                                widthBits.value -
                                *(const f64*)(roBase + 0x408)));
                    } else {
                        s32 dtIndex;
                        u32 memberId;
                        char* anim;
                        s32 color;
                        u32 materialColor;

                        PSMTXTrans(
                            drawMtx,
                            x - float_140_80423554[0] + circlePos.x,
                            float_20_804234fc[0] + y + circlePos.y,
                            circlePos.z);
                        PSMTXScale(
                            scaleMtx,
                            float_0p8_80423558[0],
                            float_0p8_80423558[0],
                            float_neg0p8_80423588[0]);
                        PSMTXRotRad(
                            faceMtx, 'y', float_3p1416_80423564[0]);
                        PSMTXConcat(drawMtx, scaleMtx, drawMtx);
                        PSMTXConcat(drawMtx, faceMtx, drawMtx);

                        animPoseSetMaterialFlagOn(poseId, 0x40);
                        materialColor = dat_804234f4[0];
                        animPoseSetMaterialEvtColor(
                            poseId, &materialColor);

                        memberId = *(u32*)(partyIter + 0x1BC);
                        dtIndex = 0;
                        if ((memberId !=
                             *(u32*)((u8*)winPartyDt + 0x00)) &&
                            (dtIndex = 1,
                             memberId !=
                             *(u32*)((u8*)winPartyDt + 0x24)) &&
                            (dtIndex = 2,
                             memberId !=
                             *(u32*)((u8*)winPartyDt + 0x48)) &&
                            (dtIndex = 3,
                             memberId !=
                                 *(u32*)((u8*)winPartyDt + 0x6C) &&
                             (dtIndex = 4,
                              memberId !=
                                  *(u32*)((u8*)winPartyDt + 0x90)) &&
                             (dtIndex = 5,
                              memberId !=
                                  *(u32*)((u8*)winPartyDt + 0xB4) &&
                              (dtIndex = 6,
                               memberId !=
                                   *(u32*)((u8*)winPartyDt + 0xD8))))) {
                            dtIndex = 7;
                        }

                        if ((*(u16*)w & 0x2000) == 0) {
                            anim =
                                *(char**)((u8*)winPartyDt +
                                          dtIndex * 0x24 + 0x14);
                        } else {
                            u8* dtEntry =
                                (u8*)winPartyDt + dtIndex * 0x24;

                            anim = *(char**)(dtEntry + 0x1C);

                            if (*(s32*)dtEntry == 4) {
                                color = pouchGetPartyColor(4);
                                if (color == 0) {
                                    anim = (char*)str_Z_5_8042358c;
                                } else if (color == 1) {
                                    anim = (char*)str_Z_6_80423590;
                                } else if (color == 2) {
                                    anim = (char*)str_Z_7_80423594;
                                } else if (color == 3) {
                                    anim = (char*)str_Z_8_80423598;
                                } else if (color == 4) {
                                    anim = (char*)str_Z_9_8042359c;
                                } else if (color == 5) {
                                    anim = (char*)str_Z_10_804235a0;
                                } else {
                                    anim = (char*)str_Z_11_804235a8;
                                }
                            }
                        }

                        animPoseSetAnim(poseId, anim, 0);
                        animPoseSetLocalTime(
                            poseId, float_0_804235b0[0]);
                    }

                    animPoseMain(poseId);

                    if (*(s32*)(partyIter + 0x1BC) == 6) {
                        Vec3 vivianPos =
                            *(const Vec3*)(roBase + 0x280);

                        vivianPos.x =
                            x - float_140_80423554[0] +
                            circlePos.x;
                        vivianPos.y =
                            float_20_804234fc[0] + y +
                            circlePos.y;
                        vivianPos.z = circlePos.z;

                        animPoseVivianMain(
                            poseId, &vivianPos);
                    }

                    animPoseDrawMtx(
                        poseId, drawMtx, 2,
                        float_0_804235b0[0],
                        float_5_804235b4[0]);
                }

                partyIter += 4;
            }
        }

        if (*(s32*)(w + 0x1E0) != 0) {
            winNameGX(
                x - float_230_804235b8[0],
                float_20_804234fc[0] + (y - float_50_804235bc[0]) -
                    float_10_804234f8[0],
                float_200_804235c0[0], float_32_80423534[0], pWin, 0);

            winFontInit();
            {
                u8* dtEntry = (u8*)winPartyDt;
                f32 selectedNameY =
                    float_20_804234fc[0] +
                    (y - float_50_804235bc[0] - float_4_80423524[0]) -
                    float_10_804234f8[0];

                for (row = 0; row < 7; row++, dtEntry += 0x24) {
                    s32 selectedIndex = *(s32*)(w + 0x1DC);
                    s32 partyId = *(s32*)dtEntry;
                    s32 selectedMember =
                        *(s32*)(w + 0x1BC + selectedIndex * 4);

                    if (selectedMember == partyId) {
                        char* nameText;
                        u32 nameWidth;
                        f32 nameWidthF;

                        if (partyId != 4) {
                            nameText = msgSearch(*(char**)(dtEntry + 8));
                        } else {
                            nameText = pouchGetYoshiName();
                        }

                        nameWidth = FontGetMessageWidth(nameText) & 0xFFFF;
                        widthBits.words.hi = 0x43300000;
                        widthBits.words.lo = nameWidth;
                        nameWidthF =
                            (f32)(widthBits.value -
                                  *(const f64*)(roBase + 0x408));

                        {
                            Vec3 selectedNamePosTmp =
                                *(const Vec3*)(roBase + 0x28C);
                            Vec3 selectedNamePosCall;
                            Vec3 selectedNameScale =
                                *(const Vec3*)(roBase + 0x298);
                            u32 selectedNameColor;

                            selectedNamePosTmp.x =
                                (x - float_230_804235b8[0]) +
                                (float_200_804235c0[0] - nameWidthF) *
                                    float_0p5_80423520[0];
                            selectedNamePosTmp.y = selectedNameY;

                            selectedNamePosCall = selectedNamePosTmp;
                            selectedNameColor = white;
                            winFontSetEdge(
                                &selectedNamePosCall, &selectedNameScale,
                                &selectedNameColor, nameText);
                        }
                    }
                }
            }

        }

        if (*(s32*)(w + 0x1E0) > 1) {
            char* changeKey = (char*)(roBase + 0x3AC);
            u32 changeWidth;
            f32 changeWidthF;
            f32 changeTextX;
            Vec3 changeTextPosTmp;
            Vec3 changeTextPosCall;
            Vec3 changeTextScale;
            Vec3 changeIconPosTmp;
            Vec3 changeIconPosCall;
            Vec3 changeIconScale;
            Vec3 leftArrowPosTmp;
            Vec3 leftArrowPosCall;
            Vec3 leftArrowScale;
            Vec3 rightArrowPosTmp;
            Vec3 rightArrowPosCall;
            Vec3 rightArrowScale;
            u32 changeTextColor;
            u32 changeIconColor;
            u32 leftArrowColor;
            u32 rightArrowColor;

            winFontInit();
            changeWidth =
                FontGetMessageWidth(msgSearch(changeKey)) & 0xFFFF;
            widthBits.words.hi = 0x43300000;
            widthBits.words.lo = changeWidth;
            changeWidthF =
                (f32)(widthBits.value -
                      *(const f64*)(roBase + 0x408));

            changeTextPosTmp = *(const Vec3*)(roBase + 0x2A4);
            changeTextScale = *(const Vec3*)(roBase + 0x2B0);
            changeTextX =
                x +
                (float_neg230_804235c4[0] -
                 float_0p8_80423558[0] * changeWidthF) *
                    float_0p5_80423520[0];
            changeTextPosTmp.x = changeTextX;
            changeTextPosTmp.y = y - float_90_804235c8[0];
            changeTextPosCall = changeTextPosTmp;
            changeTextColor = gray;
            winFontSet(
                &changeTextPosCall, &changeTextScale, &changeTextColor,
                msgSearch(changeKey));

            winIconInit();
            changeIconPosTmp = *(const Vec3*)(roBase + 0x2BC);
            changeIconScale = *(const Vec3*)(roBase + 0x2C8);
            changeIconPosTmp.x =
                changeTextX - float_30_804235cc[0];
            changeIconPosTmp.y =
                y - float_100_804235d0[0];
            changeIconPosCall = changeIconPosTmp;
            changeIconColor = white;
            winIconSet(
                0x6C, &changeIconPosCall, &changeIconScale,
                &changeIconColor);

            winTexInit(**(void***)((u8*)*(void**)(w + 0x28) + 0xA0));

            leftArrowPosTmp = *(const Vec3*)(roBase + 0x2D4);
            leftArrowScale = *(const Vec3*)(roBase + 0x2E0);
            leftArrowPosTmp.x =
                x - float_150_804235d4[0] -
                float_50_804235bc[0];
            leftArrowPosTmp.y = y - float_16_804235d8[0];
            leftArrowPosCall = leftArrowPosTmp;
            leftArrowColor = white;
            winTexSet(
                0xC0, &leftArrowPosCall, &leftArrowScale,
                &leftArrowColor);

            rightArrowPosTmp = *(const Vec3*)(roBase + 0x2EC);
            rightArrowScale = *(const Vec3*)(roBase + 0x2F8);
            rightArrowPosTmp.x =
                x - float_150_804235d4[0] +
                float_50_804235bc[0];
            rightArrowPosTmp.y = y - float_16_804235d8[0];
            rightArrowPosCall = rightArrowPosTmp;
            rightArrowColor = white;
            winTexSet(
                0xC0, &rightArrowPosCall, &rightArrowScale,
                &rightArrowColor);
        }

        {
            char* headingKey = (char*)(roBase + 0x3C4);
            u32 headingWidth;
            f32 headingWidthF;
            f32 headingX;
            Vec3 headingTextPosTmp;
            Vec3 headingTextPosCall;
            Vec3 headingTextScale;
            Vec3 headingIconPosTmp;
            Vec3 headingIconPosCall;
            Vec3 headingIconScale;
            u32 headingTextColor;
            u32 headingIconColor;

            winFontInit();
            headingWidth =
                FontGetMessageWidth(msgSearch(headingKey)) & 0xFFFF;
            widthBits.words.hi = 0x43300000;
            widthBits.words.lo = headingWidth;
            headingWidthF =
                (f32)(widthBits.value -
                      *(const f64*)(roBase + 0x408));

            headingTextPosTmp = *(const Vec3*)(roBase + 0x304);
            headingTextScale = *(const Vec3*)(roBase + 0x310);
            headingX =
                x + float_250_804235e0[0] -
                float_0p7_804235dc[0] * headingWidthF;
            headingTextPosTmp.x = headingX;
            headingTextPosTmp.y =
                y - float_100_804235d0[0];
            headingTextPosCall = headingTextPosTmp;
            headingTextColor = gray;
            winFontSet(
                &headingTextPosCall, &headingTextScale,
                &headingTextColor, msgSearch(headingKey));

            winIconInit();
            headingIconPosTmp = *(const Vec3*)(roBase + 0x31C);
            headingIconScale = *(const Vec3*)(roBase + 0x328);
            headingIconPosTmp.x =
                headingX - float_20_804234fc[0];
            headingIconPosTmp.y =
                y - float_110_80423514[0];
            headingIconPosCall = headingIconPosTmp;
            headingIconColor = white;
            winIconSet(
                0x1FC, &headingIconPosCall, &headingIconScale,
                &headingIconColor);
        }

        winWazaGX(
            x - float_5_804235b4[0],
            y + float_50_804235bc[0],
            float_260_804235e4[0],
            float_144_804235e8[0], pWin, 0);

        {
            char* wazaKey = (char*)(roBase + 0x3E0);
            char* fpKey = (char*)(roBase + 0x3F4);
            u32 wazaWidth;
            f32 wazaWidthF;
            f32 wazaTitleX;
            f32 fpTitleX;
            f32 moveNameX;
            f32 costCenterX;
            f32 fpColumnX;
            s32 moveYOffset = 0;
            s32 weaponOffset = 0;
            char costBuf[0x100];

            Vec3 wazaTitlePosTmp;
            Vec3 wazaTitlePosCall;
            Vec3 wazaTitleScale;
            u32 wazaTitleColor;

            Vec3 fpTitlePosTmp;
            Vec3 fpTitlePosCall;
            Vec3 fpTitleScale;
            u32 fpTitleColor;

            Vec3 movePosTmp;
            Vec3 movePosCall;
            Vec3 moveScale;
            u32 moveColor;

            Vec3 costPosTmp;
            Vec3 costPosCall;
            Vec3 costScale;
            u32 costColor;

            Vec3 fpRowPosTmp;
            Vec3 fpRowPosCall;
            Vec3 fpRowScale;
            u32 fpRowColor;

            winFontInit();
            wazaWidth =
                FontGetMessageWidth(msgSearch(wazaKey)) & 0xFFFF;
            widthBits.words.hi = 0x43300000;
            widthBits.words.lo = wazaWidth;
            wazaWidthF =
                (f32)(widthBits.value -
                      *(const f64*)(roBase + 0x408));

            wazaTitlePosTmp =
                *(const Vec3*)(roBase + 0x334);
            wazaTitleScale =
                *(const Vec3*)(roBase + 0x340);
            wazaTitleX =
                x +
                (float_200_804235c0[0] -
                 float_0p8_80423558[0] * wazaWidthF) *
                    float_0p5_80423520[0];
            wazaTitlePosTmp.x = wazaTitleX;
            wazaTitlePosTmp.y =
                y + float_45_8042350c[0];
            wazaTitlePosCall = wazaTitlePosTmp;
            wazaTitleColor = white;
            winFontSetEdge(
                &wazaTitlePosCall, &wazaTitleScale,
                &wazaTitleColor, msgSearch(wazaKey));

            fpTitlePosTmp =
                *(const Vec3*)(roBase + 0x34C);
            fpTitleScale =
                *(const Vec3*)(roBase + 0x358);
            fpTitleX =
                x + float_230_804235b8[0] -
                float_20_804234fc[0];
            fpTitlePosTmp.x = fpTitleX;
            fpTitlePosTmp.y =
                y + float_45_8042350c[0];
            fpTitlePosCall = fpTitlePosTmp;
            fpTitleColor = white;
            winFontSetEdge(
                &fpTitlePosCall, &fpTitleScale,
                &fpTitleColor, msgSearch(fpKey));

            moveNameX = x + float_10_804234f8[0];
            costCenterX =
                x + float_238_804235ec[0] -
                float_20_804234fc[0];
            fpColumnX =
                x + float_230_804235b8[0] +
                float_17_804235f0[0] -
                float_20_804234fc[0] +
                float_2_80423510[0] +
                float_2_80423510[0];

            for (row = 0; row < *(s32*)(w + 0x204); row++) {
                f32 rowY =
                    y + float_50_804235bc[0] -
                    (f32)moveYOffset -
                    float_30_804235cc[0];
                u8* activeDt;
                u8** weaponTable;
                u8* weapon;
                u32 halfWidth;
                f32 halfWidthF;

                movePosTmp =
                    *(const Vec3*)(roBase + 0x364);
                moveScale =
                    *(const Vec3*)(roBase + 0x370);
                movePosTmp.x = moveNameX;
                movePosTmp.y = rowY;
                movePosCall = movePosTmp;
                moveColor = gray;

                activeDt =
                    (u8*)winPartyDt + *(s32*)(w + 0x1D8) * 0x24;
                weaponTable = *(u8***)(activeDt + 0x20);
                weapon = weaponTable[weaponOffset >> 2];
                winFontSetWidth(
                    &movePosCall, &moveScale, &moveColor,
                    msgSearch(*(char**)weapon),
                    float_175_804235f4[0]);

                activeDt =
                    (u8*)winPartyDt + *(s32*)(w + 0x1D8) * 0x24;
                weaponTable = *(u8***)(activeDt + 0x20);
                weapon = weaponTable[weaponOffset >> 2];
                sprintf(
                    costBuf, str_PCTs_80423540,
                    winZenkakuStr(*(u8*)(weapon + 0x11)));

                costPosTmp =
                    *(const Vec3*)(roBase + 0x37C);
                costScale =
                    *(const Vec3*)(roBase + 0x388);

                halfWidth =
                    (FontGetMessageWidth(costBuf) >> 1) & 0x7FFF;
                widthBits.words.hi = 0x43300000;
                widthBits.words.lo = halfWidth ^ 0x80000000;
                halfWidthF =
                    (f32)(widthBits.value -
                          *(const f64*)(roBase + 0x410));

                costPosTmp.x =
                    costCenterX - halfWidthF -
                    float_2_80423510[0];
                costPosTmp.y = rowY;
                costPosCall = costPosTmp;
                costColor = gray;
                winFontSet(
                    &costPosCall, &costScale, &costColor,
                    costBuf);

                fpRowPosTmp =
                    *(const Vec3*)(roBase + 0x394);
                fpRowScale =
                    *(const Vec3*)(roBase + 0x3A0);
                fpRowPosTmp.x = fpColumnX;
                fpRowPosTmp.y =
                    rowY - float_10_804234f8[0];
                fpRowPosCall = fpRowPosTmp;
                fpRowColor = gray;
                winFontSet(
                    &fpRowPosCall, &fpRowScale, &fpRowColor,
                    msgSearch((char*)(roBase + 0x3F4)));

                moveYOffset += 26;
                weaponOffset += 4;
            }
        }
    }

    (void)cameraId;
}

