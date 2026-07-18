#include "peach.h"

const char str_c_peach_802fd210[] = "c_peach";
const char str_c_zako_n_802fd218[] = "c_zako_n";
f32 float_0_804265b0 = 0.0f;
char str_P_T_1_804265b4[] = "P_T_1";
char str_P_R_1_804265bc[] = "P_R_1";
char str_P_W_1_804265c4[] = "P_W_1";
char str_P_S_1_804265cc[] = "P_S_1";
char str_P_U_1_804265d4[] = "P_U_1";
char str_P_N_1_804265dc[] = "P_N_1";
f32 float_47_804265e4 = 47.0f;
f32 float_20_804265e8 = 20.0f;
char str_S_1_804265ec[] = "S_1";
char str_T_1_804265f0[] = "T_1";
f32 float_30_804265f4 = 30.0f;

void* marioGetPtr(void);
void marioSetCharMode(s32 mode);
void peachChgPose(void* pose);
void animPoseRelease(s32 poseId);
s32 animPoseEntry(const char* name, s32 mode);
void npcSetMarioAutoTalkPose(const char* stay, const char* talk);

void peachPreInit(void) {
    ;
}

void peachSetStsAftershower(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x10;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsAngry(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x100;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsCarry(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x400;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsHappy(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x40;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsInvisible(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x20;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsOrdinary(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x200;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsUneasy(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x80;
    peachChgPose(str_P_S_1_804265cc);
}


void peachChgPose(void* posePtr) {
    extern void* marioGetPtr(void);
    extern s32 strcmp(const char*, const char*);
    extern char str_P_Z_1_804265f8[];
    extern char str_Z_1_80426600[];
    extern char str_W_1_80426604[];
    extern char str_R_1_80426608[];
    extern char str_P_Z_2_8042660c[], str_P_S_2_80426614[], str_P_W_2_8042661c[];
    extern char str_P_R_2_80426624[], str_P_T_2_8042662c[];
    extern char str_P_Z_3_80426634[], str_P_S_3_8042663c[], str_P_W_3_80426644[];
    extern char str_P_R_3_8042664c[], str_P_T_3_80426654[];
    extern char str_P_Z_4_8042665c[], str_P_S_4_80426664[], str_P_W_4_8042666c[];
    extern char str_P_R_4_80426674[], str_P_T_4_8042667c[];
    extern char str_P_Z_5_80426684[], str_P_S_5_8042668c[], str_P_W_5_80426694[];
    extern char str_P_R_5_8042669c[], str_P_T_5_804266a4[];
    extern char str_P_Z_6_804266ac[], str_P_S_6_804266b4[], str_P_W_6_804266bc[];
    extern char str_P_R_6_804266c4[], str_P_T_6_804266cc[];
    extern char str_P_Z_7_804266d4[], str_P_S_7_804266dc[], str_P_W_7_804266e4[];
    extern char str_P_R_7_804266ec[], str_P_T_13_802fd224[];
    extern char str_P_Z_8_804266f4[], str_P_S_8_804266fc[], str_P_W_8_80426704[];
    extern char str_P_R_8_8042670c[], str_P_T_14_802fd22c[];
    u8* mario = marioGetPtr();
    char* pose = posePtr;
    u32 status;

    if ((*(u32*)(mario + 4) & 2) != 0) {
        return;
    }
    status = *(u32*)(mario + 0x10);

#define MAP_POSE(z, s, w, r, t)                                      \
    if (strcmp(pose, str_P_Z_1_804265f8) == 0) pose = (z);            \
    else if (strcmp(pose, str_P_S_1_804265cc) == 0) pose = (s);       \
    else if (strcmp(pose, str_P_W_1_804265c4) == 0) pose = (w);       \
    else if (strcmp(pose, str_P_R_1_804265bc) == 0) pose = (r);       \
    else if (strcmp(pose, str_P_T_1_804265b4) == 0) pose = (t)

    if (status & 0x2000) {
        MAP_POSE(str_Z_1_80426600, str_S_1_804265ec, str_W_1_80426604,
                 str_R_1_80426608, str_T_1_804265f0);
    } else if (status & 0x10) {
        MAP_POSE(str_P_Z_2_8042660c, str_P_S_2_80426614, str_P_W_2_8042661c,
                 str_P_R_2_80426624, str_P_T_2_8042662c);
    } else if (status & 0x20) {
        MAP_POSE(str_P_Z_3_80426634, str_P_S_3_8042663c, str_P_W_3_80426644,
                 str_P_R_3_8042664c, str_P_T_3_80426654);
    } else if (status & 0x80) {
        MAP_POSE(str_P_Z_4_8042665c, str_P_S_4_80426664, str_P_W_4_8042666c,
                 str_P_R_4_80426674, str_P_T_4_8042667c);
    } else if (status & 0x40) {
        MAP_POSE(str_P_Z_5_80426684, str_P_S_5_8042668c, str_P_W_5_80426694,
                 str_P_R_5_8042669c, str_P_T_5_804266a4);
    } else if (status & 0x100) {
        MAP_POSE(str_P_Z_6_804266ac, str_P_S_6_804266b4, str_P_W_6_804266bc,
                 str_P_R_6_804266c4, str_P_T_6_804266cc);
    } else if (status & 0x200) {
        MAP_POSE(str_P_Z_7_804266d4, str_P_S_7_804266dc, str_P_W_7_804266e4,
                 str_P_R_7_804266ec, str_P_T_13_802fd224);
    } else if (status & 0x400) {
        MAP_POSE(str_P_Z_8_804266f4, str_P_S_8_804266fc, str_P_W_8_80426704,
                 str_P_R_8_8042670c, str_P_T_14_802fd22c);
    }
#undef MAP_POSE

    if (*(char**)(mario + 0x18) == NULL || strcmp(*(char**)(mario + 0x18), pose) != 0) {
        *(char**)(mario + 0x18) = pose;
        *(u16*)(mario + 0x28) = 0;
        *(u32*)(mario + 4) &= 0xCFFFFFFF;
        *(u32*)(mario + 0xC) |= 1;
    }
}

u8 peach_dash(void) {
    extern f32 marioGetDashSpd(void);
    extern void marioChgMot(s32 motion);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    void* mario = marioGetPtr();

    if (*(u32*)((s32)mario + 0xC) & 1) {
        *(u32*)((s32)mario + 0xC) &= ~1;
        *(u32*)((s32)mario + 0) &= ~0x7F0;
        if (*(s16*)((s32)mario + 0x4E) == 0) {
            *(f32*)((s32)mario + 0x180) = marioGetDashSpd();
        }
        peachChgPose(str_P_R_1_804265bc);
        *(s32*)((s32)mario + 0x44) = 0;
        *(s32*)((s32)mario + 0x48) = 0;
    }

    if (*(s16*)((s32)mario + 0x4E) == 0) {
        *(f32*)((s32)mario + 0x180) = marioGetDashSpd();
    }

    if (*(u32*)((s32)mario + 0) & 0x20) {
        s32 id0;
        s32 id1;
        s32 timer;
        mario = marioGetPtr();
        if ((*(u32*)((s32)mario + 0x10) & 0x2000) == 0) {
            if ((*(u32*)((s32)mario + 0x10) & 0x20) != 0 && float_0_804265b0 == *(f32*)((s32)mario + 0xCC)) {
                id0 = 0x80C;
                id1 = 0x80B;
            } else {
                id0 = 0x7C4;
                id1 = 0x7C3;
            }
        } else {
            id0 = 0x7E9;
            id1 = 0x7E8;
        }
        timer = *(s32*)((s32)mario + 0x48);
        if ((timer % 20) == 0) {
            psndSFXOn_3D(id0, (void*)((s32)mario + 0x8C));
        } else if ((timer % 10) == 0) {
            psndSFXOn_3D(id1, (void*)((s32)mario + 0x8C));
        }
        *(s32*)((s32)mario + 0x48) = timer + 1;
        return;
    }

    if ((s8)*(u8*)((s32)mario + 0x252) * (s8)*(u8*)((s32)mario + 0x252) +
        (s8)*(u8*)((s32)mario + 0x253) * (s8)*(u8*)((s32)mario + 0x253) <= 0xBD1) {
        marioChgMot(1);
        return;
    }

    *(f32*)((s32)mario + 0x1A4) = *(f32*)((s32)mario + 0x198);
    mario = marioGetPtr();
    {
        s32 id0;
        s32 id1;
        s32 timer;
        if ((*(u32*)((s32)mario + 0x10) & 0x2000) == 0) {
            if ((*(u32*)((s32)mario + 0x10) & 0x20) != 0 && float_0_804265b0 == *(f32*)((s32)mario + 0xCC)) {
                id0 = 0x80C;
                id1 = 0x80B;
            } else {
                id0 = 0x7C4;
                id1 = 0x7C3;
            }
        } else {
            id0 = 0x7E9;
            id1 = 0x7E8;
        }
        timer = *(s32*)((s32)mario + 0x48);
        if ((timer % 20) == 0) {
            psndSFXOn_3D(id0, (void*)((s32)mario + 0x8C));
        } else if ((timer % 10) == 0) {
            psndSFXOn_3D(id1, (void*)((s32)mario + 0x8C));
        }
        *(s32*)((s32)mario + 0x48) = timer + 1;
    }

    if (*(f32*)((s32)mario + 0x194) == float_0_804265b0) {
        marioChgMot(0);
    }
}

u8 peach_stay(void) {
    extern void marioChgMot(s32 motion);
    extern void marioChgPose(void* pose);
    extern s32 sysMsec2Frame(s32 msec);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void marioAdjustMoveDir(void);
    void* mario = marioGetPtr();

    if (*(u32*)((s32)mario + 0xC) & 1) {
        *(u32*)((s32)mario + 0xC) &= ~1;
        *(u32*)((s32)mario + 0) &= ~0x7F0;
        *(s16*)((s32)mario + 0x50) = 0;
        *(s32*)((s32)mario + 0x48) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        peachChgPose(str_P_S_1_804265cc);
        if ((*(u32*)((s32)mario + 0) & 0x20) == 0) {
            *(f32*)((s32)mario + 0x180) = float_0_804265b0;
        }
    }

    if (*(f32*)((s32)mario + 0x194) > float_0_804265b0) {
        marioChgMot(1);
    }

    if ((*(u32*)((s32)mario + 0) & 8) == 0 &&
        (*(u32*)((s32)mario + 4) & 0x40000000) == 0 &&
        (*(u32*)((s32)mario + 0x10) & 0x27F0) == 0) {
        switch (*(s32*)((s32)mario + 0x44)) {
            case 0:
                *(s32*)((s32)mario + 0x48) += 1;
                if (*(s32*)((s32)mario + 0x48) >= sysMsec2Frame(50000)) {
                    marioChgPose(str_P_U_1_804265d4);
                    *(s32*)((s32)mario + 0x48) = 0x50;
                    *(s32*)((s32)mario + 0x44) = 1;
                    *(u32*)((s32)mario + 0x27C) = psndSFXOn_3D(0x101, (void*)((s32)mario + 0x8C));
                    marioAdjustMoveDir();
                }
                break;
            case 1:
                *(s32*)((s32)mario + 0x48) -= 1;
                if (*(s32*)((s32)mario + 0x48) < 0) {
                    marioChgPose(str_P_S_1_804265cc);
                    *(s32*)((s32)mario + 0x48) = 100;
                    *(s32*)((s32)mario + 0x44) = 2;
                }
                break;
            case 2:
                *(s32*)((s32)mario + 0x48) -= 1;
                if (*(s32*)((s32)mario + 0x48) < 0) {
                    marioChgPose(str_P_N_1_804265dc);
                    *(s32*)((s32)mario + 0x48) = 0;
                    *(s32*)((s32)mario + 0x44) = 0x14;
                    *(s32*)((s32)mario + 0x2CC) = 0;
                }
                break;
            case 0x14:
                *(s32*)((s32)mario + 0x2CC) += 1;
                if (*(s32*)((s32)mario + 0x2CC) >= sysMsec2Frame(10000)) {
                    *(s32*)((s32)mario + 0x2CC) = sysMsec2Frame(10000);
                }
                *(s32*)((s32)mario + 0x48) += 1;
                if (*(s32*)((s32)mario + 0x48) >= 100) {
                    *(s32*)((s32)mario + 0x48) = 0;
                }
                if (*(u32*)((s32)mario + 0) & 8) {
                    marioChgPose(str_P_S_1_804265cc);
                    *(s32*)((s32)mario + 0x44) = 0;
                }
                break;
        }
    }
}

s32 peachMain(void) {
    extern void marioBottomless(void);
    extern void marioPreDisp(void);
    extern void marioBalloonMain(void);
    extern void mario_kemuri(void);
    extern void partyMain(void);
    extern void marioCamMain(void);
    extern void marioMove(void);
    extern void marioChkOverhead(void);
    extern void marioChkGnd(void);
    extern void marioChkGnd2(void);
    extern void marioChgMot(s32 motion);
    extern void marioSetFallPara(void);
    extern void marioJump(void);
    extern void marioFall(void);
    extern void marioBoots(void);
    void* mario = marioGetPtr();

    if (*(u16*)((s32)mario + 0x2E) == 8) {
        *(u32*)((s32)mario + 0x110) = *(u32*)((s32)mario + 0x8C);
        *(u32*)((s32)mario + 0x114) = *(u32*)((s32)mario + 0x90);
        *(u32*)((s32)mario + 0x118) = *(u32*)((s32)mario + 0x94);
        *(f32*)((s32)mario + 0x174) = float_0_804265b0;
        *(f32*)((s32)mario + 0x178) = float_0_804265b0;
        *(f32*)((s32)mario + 0x17C) = float_0_804265b0;
        marioBottomless();
        marioPreDisp();
        marioBalloonMain();
        mario_kemuri();
        partyMain();
        marioCamMain();
        *(s32*)((s32)mario + 0x34) += 1;
        return 1;
    }

    if (*(u32*)((s32)mario + 0) & 0x40000) {
        marioMove();
        marioChkOverhead();
        if (*(s16*)((s32)mario + 0x50) > 100) {
            marioChgMot(10);
            marioSetFallPara();
        }
    } else if (*(u32*)((s32)mario + 0) & 0x10000) {
        marioJump();
        marioMove();
        marioChkOverhead();
        if (*(s16*)((s32)mario + 0x50) > 5) {
            marioChkGnd2();
        }
    } else if (*(u32*)((s32)mario + 0) & 0x20000) {
        marioMove();
        marioFall();
        marioChkOverhead();
        if (*(u16*)((s32)mario + 0x2E) != 0x10 && *(u16*)((s32)mario + 0x2E) != 0x11) {
            if (*(s16*)((s32)mario + 0x50) > 5) {
                marioChkGnd2();
            }
        }
    } else {
        marioMove();
        marioChkOverhead();
        marioChkGnd();
    }

    if (*(u32*)((s32)mario + 0) & 0x20) {
        *(s16*)((s32)mario + 0x4E) -= 1;
        if (*(s16*)((s32)mario + 0x4E) <= 0) {
            *(s16*)((s32)mario + 0x4E) = 0;
            *(u32*)((s32)mario + 0) &= ~0x20;
        }
    }

    marioBoots();
    marioBottomless();
    marioPreDisp();
    marioBalloonMain();
    mario_kemuri();
    partyMain();
    marioCamMain();
    *(s32*)((s32)mario + 0x34) += 1;
    *(u32*)((s32)mario + 0x110) = *(u32*)((s32)mario + 0x8C);
    *(u32*)((s32)mario + 0x114) = *(u32*)((s32)mario + 0x90);
    *(u32*)((s32)mario + 0x118) = *(u32*)((s32)mario + 0x94);
    *(f32*)((s32)mario + 0x174) = float_0_804265b0;
    *(f32*)((s32)mario + 0x178) = float_0_804265b0;
    *(f32*)((s32)mario + 0x17C) = float_0_804265b0;
    return 1;
}

u8 peach_walk(void) {
    extern f32 marioGetWalkSpd(void);
    extern void marioChgMot(s32 motion);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    void* mario = marioGetPtr();

    if (*(u32*)((s32)mario + 0xC) & 1) {
        *(u32*)((s32)mario + 0xC) &= ~1;
        *(u32*)((s32)mario + 0) &= ~0x7F0;
        if (*(s16*)((s32)mario + 0x4E) == 0) {
            *(f32*)((s32)mario + 0x180) = marioGetWalkSpd();
        }
        peachChgPose(str_P_W_1_804265c4);
        *(s32*)((s32)mario + 0x44) = 0;
        *(s32*)((s32)mario + 0x48) = 0;
    }

    if (*(s16*)((s32)mario + 0x4E) == 0) {
        *(f32*)((s32)mario + 0x180) = marioGetWalkSpd();
    }

    if ((s8)*(u8*)((s32)mario + 0x252) * (s8)*(u8*)((s32)mario + 0x252) +
        (s8)*(u8*)((s32)mario + 0x253) * (s8)*(u8*)((s32)mario + 0x253) > 0xBD1) {
        marioChgMot(2);
        return;
    }

    *(f32*)((s32)mario + 0x1A4) = *(f32*)((s32)mario + 0x198);
    mario = marioGetPtr();
    {
        s32 id0;
        s32 id1;
        s32 timer;
        if ((*(u32*)((s32)mario + 0x10) & 0x2000) == 0) {
            if ((*(u32*)((s32)mario + 0x10) & 0x20) != 0 && float_0_804265b0 == *(f32*)((s32)mario + 0xCC)) {
                id0 = 0x80C;
                id1 = 0x80B;
            } else {
                id0 = 0x7C4;
                id1 = 0x7C3;
            }
        } else {
            id0 = 0x7E9;
            id1 = 0x7E8;
        }
        timer = *(s32*)((s32)mario + 0x48);
        if ((timer % 40) == 0) {
            psndSFXOn_3D(id0, (void*)((s32)mario + 0x8C));
        } else if ((timer % 20) == 0) {
            psndSFXOn_3D(id1, (void*)((s32)mario + 0x8C));
        }
        *(s32*)((s32)mario + 0x48) = timer + 1;
    }

    if (*(f32*)((s32)mario + 0x194) == float_0_804265b0) {
        marioChgMot(0);
    }
}

void peachSetStsNormal(void) {
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0x10);

    if (flags & 0x2000) {
        s32 mode;

        mario = marioGetPtr();
        mode = *(u8*)((s32)mario + 0x3C);
        if (mode != 1) {
            marioSetCharMode(1);
        }

        if (*(u32*)((s32)mario + 0x10) & 0x2000) {
            f32 height;
            f32 width;

            *(u32*)((s32)mario + 0x10) &= ~0x2000;
            if (*(s32*)((s32)mario + 0x22C) >= 0) {
                animPoseRelease(*(s32*)((s32)mario + 0x22C));
            }
            *(s32*)((s32)mario + 0x22C) = animPoseEntry(str_c_peach_802fd210, 2);
            npcSetMarioAutoTalkPose(str_P_S_1_804265cc, str_P_T_1_804265b4);
            height = float_47_804265e4;
            width = float_20_804265e8;
            *(f32*)((s32)mario + 0x1BC) = height;
            *(f32*)((s32)mario + 0x1B8) = width;
        }
    } else {
        *(u32*)((s32)mario + 0x10) &= ~0x7F0;
        peachChgPose(str_P_S_1_804265cc);
    }
}

void peachTransformOff(void) {
    void* mario = marioGetPtr();
    s32 mode = *(u8*)((s32)mario + 0x3C);

    if (mode != 1) {
        marioSetCharMode(1);
    }

    if (*(u32*)((s32)mario + 0x10) & 0x2000) {
        f32 height;
        f32 width;

        *(u32*)((s32)mario + 0x10) &= ~0x2000;
        if (*(s32*)((s32)mario + 0x22C) >= 0) {
            animPoseRelease(*(s32*)((s32)mario + 0x22C));
        }
        *(s32*)((s32)mario + 0x22C) = animPoseEntry(str_c_peach_802fd210, 2);
        npcSetMarioAutoTalkPose(str_P_S_1_804265cc, str_P_T_1_804265b4);
        height = float_47_804265e4;
        width = float_20_804265e8;
        *(f32*)((s32)mario + 0x1BC) = height;
        *(f32*)((s32)mario + 0x1B8) = width;
    }
}

void peachTransformOn(void) {
    void* mario = marioGetPtr();
    s32 mode = *(u8*)((s32)mario + 0x3C);

    if (mode != 1) {
        marioSetCharMode(1);
    }

    if (!(*(u32*)((s32)mario + 0x10) & 0x2000)) {
        f32 scale;

        *(u32*)((s32)mario + 0x10) |= 0x2000;
        if (*(s32*)((s32)mario + 0x22C) >= 0) {
            animPoseRelease(*(s32*)((s32)mario + 0x22C));
        }
        *(s32*)((s32)mario + 0x22C) = animPoseEntry(str_c_zako_n_802fd218, 2);
        npcSetMarioAutoTalkPose(str_S_1_804265ec, str_T_1_804265f0);
        scale = float_30_804265f4;
        *(f32*)((s32)mario + 0x1BC) = scale;
        *(f32*)((s32)mario + 0x1B8) = scale;
    }
}

u8 peach_talk(void) {
    extern void marioAdjustMoveDir(void);
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);

    if (flags & 1) {
        f32 zero;

        *(u32*)((s32)mario + 0xC) = flags & ~1;
        zero = float_0_804265b0;
        *(s32*)((s32)mario + 0x48) = 0;
        *(s16*)((s32)mario + 0x50) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        *(f32*)((s32)mario + 0x180) = zero;
        marioAdjustMoveDir();
        peachChgPose(str_P_T_1_804265b4);
    }
}

