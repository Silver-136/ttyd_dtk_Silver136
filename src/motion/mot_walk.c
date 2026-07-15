#include "motion/mot_walk.h"


void mot_dash(void) {
    extern void* marioGetPtr(void);
    extern void kpa_dash(void);
    extern void peach_dash(void);
    extern void* caseCheckHitObj(void*);
    extern s32 marioChkPushAnime(void);
    extern s32 marioBgmodeChk(void);
    extern void marioChgPose(char*);
    extern void marioResetCamFollowRate(void);
    extern void motSlitContinue(void);
    extern s32 strcmp(char*, char*);
    extern void marioChgMot(s32);
    extern s32 marioSlitAbilityChk(void);
    extern s32 marioSlitButton(void);
    extern s32 marioChkItemMotion(void);
    extern void marioChkJump(void);
    extern void marioChkTransform(void);
    extern void marioWalkDashSe(void*, u32);
    extern s32 pouchEquipCheckBadge(s32);
    extern char str_M_R_1_80420870[];
    extern char str_M_O_1_8042087c[];
    extern char str_M_W_1_80420884[];
    extern char str_M_I_Y_80420890[];
    extern f32 float_1_80420878;
    extern f32 float_0p5_8042088c;
    extern f32 float_0_80420898;

    void* player;
    void* caseEntry;
    s32 activeCase;
    f32 speed;
    s32 sx;
    s32 sy;
    u32 interval;

    player = marioGetPtr();
    if (*(s8*)((s32)player + 0x3C) == 2) {
        kpa_dash();
        return;
    }
    if (*(s8*)((s32)player + 0x3C) == 1) {
        peach_dash();
        return;
    }

    if (*(void**)((s32)player + 0x1F0) == 0) {
        activeCase = 0;
    } else {
        caseEntry = caseCheckHitObj(*(void**)((s32)player + 0x1F0));
        activeCase = (caseEntry != 0) && (*(s32*)((s32)caseEntry + 4) == 10);
    }
    if (!activeCase && *(void**)((s32)player + 0x1F0) == 0) {
        *(u32*)player &= ~0x8000;
        if (marioChkPushAnime() && marioBgmodeChk() == 0) {
            marioChgPose(str_M_R_1_80420870);
        }
    }

    if (*(u32*)((s32)player + 0xC) & 1) {
        *(u32*)((s32)player + 0xC) &= ~1;
        *(u32*)player &= 0xF87FFFFF;
        marioResetCamFollowRate();
        if (*(u32*)player & 0x100000) {
            motSlitContinue();
        }
        if (*(f32*)((s32)player + 0x228) == float_1_80420878) {
            if (*(u32*)player & 0x8000) {
                if (!marioChkPushAnime()) {
                    marioChgPose(str_M_O_1_8042087c);
                }
            } else if (marioBgmodeChk() == 0) {
                marioChgPose(str_M_R_1_80420870);
            }
        } else {
            if (*(u32*)player & 0x8000) {
                if (!marioChkPushAnime()) {
                    marioChgPose(str_M_O_1_8042087c);
                }
            } else {
                marioChgPose(str_M_W_1_80420884);
            }
        }
        if (*(s16*)((s32)player + 0x4E) == 0) {
            void* player2 = marioGetPtr();
            speed = *(f32*)((s32)player2 + 0x188);
            if (*(u32*)player2 & 0x100000) {
                sx = *(s8*)((s32)player2 + 0x252);
                sy = *(s8*)((s32)player2 + 0x253);
                speed = *(f32*)((s32)player2 + 0x184);
                if ((sx * sx + sy * sy) <= 0xBD1) {
                    speed *= float_0p5_8042088c;
                }
            } else if (marioBgmodeChk() == 1) {
                speed *= float_0p5_8042088c;
            }
            speed *= *(f32*)((s32)player2 + 0x228);
            *(f32*)((s32)player + 0x180) = speed;
        }
    }

    if (!(*(u32*)player & 0x2000) &&
        (strcmp(*(char**)((s32)player + 0x18), str_M_I_Y_80420890) == 0 ||
         *(s8*)((s32)player + 0x40) > 10)) {
        marioChgMot(0);
        return;
    }
    if (*(u32*)player & 0x2000) {
        marioChgMot(1);
        return;
    }
    if (marioSlitAbilityChk() && marioSlitButton() == 1) {
        marioChgMot(0x15);
        return;
    }
    if (marioChkItemMotion() == 0) {
        marioChkJump();
        marioChkTransform();
        if (*(void**)((s32)player + 0x1E8) != 0) {
            if (*(f32*)((s32)player + 0x228) == float_1_80420878) {
                interval = 0x14;
            } else {
                interval = 0x28;
            }
            marioWalkDashSe(*(void**)((s32)player + 0x1E8), interval);
        }
        *(u32*)((s32)player + 0x48) += 1;
        if (!(*(u32*)player & 0x20)) {
            sx = *(s8*)((s32)player + 0x252);
            sy = *(s8*)((s32)player + 0x253);
            if ((sx * sx + sy * sy) <= 0xBD1 || pouchEquipCheckBadge(0x143) != 0) {
                marioChgMot(1);
                return;
            }
            *(f32*)((s32)player + 0x1A4) = *(f32*)((s32)player + 0x198);
            if (*(u32*)player & 0x8000) {
                if (!marioChkPushAnime()) {
                    marioChgPose(str_M_O_1_8042087c);
                }
            }
            if (*(f32*)((s32)player + 0x194) == float_0_80420898) {
                marioChgMot(0);
            }
        }
    }
}

void mot_walk(void) {
    extern void* marioGetPtr(void);
    extern void kpa_walk(void);
    extern void peach_walk(void);
    extern void* caseCheckHitObj(void*);
    extern s32 marioChkPushAnime(void);
    extern s32 marioBgmodeChk(void);
    extern void marioChgPose(char*);
    extern void marioResetCamFollowRate(void);
    extern void motSlitContinue(void);
    extern s32 strcmp(char*, char*);
    extern void marioChgMot(s32);
    extern s32 marioSlitAbilityChk(void);
    extern s32 marioSlitButton(void);
    extern s32 marioChkItemMotion(void);
    extern void marioChkJump(void);
    extern void marioChkTransform(void);
    extern void marioWalkDashSe(void*, u32);
    extern s32 pouchEquipCheckBadge(s32);
    extern char str_M_W_1_80420884[];
    extern char str_M_O_1_8042087c[];
    extern char str_M_W_7_8042089c[];
    extern char str_M_I_Y_80420890[];
    extern f32 float_1_80420878;
    extern f32 float_0p5_8042088c;
    extern f32 float_0_80420898;

    void* player;
    void* caseEntry;
    s32 activeCase;
    f32 speed;
    s32 sx;
    s32 sy;

    player = marioGetPtr();
    if (*(s8*)((s32)player + 0x3C) == 2) {
        kpa_walk();
        return;
    }
    if (*(s8*)((s32)player + 0x3C) == 1) {
        peach_walk();
        return;
    }

    if (*(u32*)player & 0x8000) {
        if (*(void**)((s32)player + 0x1F0) == 0) {
            activeCase = 0;
        } else {
            caseEntry = caseCheckHitObj(*(void**)((s32)player + 0x1F0));
            activeCase = (caseEntry != 0) && (*(s32*)((s32)caseEntry + 4) == 10);
        }
        if (!activeCase) {
            *(u32*)player &= ~0x8000;
            if (marioChkPushAnime() && marioBgmodeChk() == 0) {
                marioChgPose(str_M_W_1_80420884);
            }
        }
    }

    if (*(u32*)((s32)player + 0xC) & 1) {
        *(u32*)((s32)player + 0xC) &= ~1;
        *(u32*)player &= 0xF87FFFFF;
        marioResetCamFollowRate();
        if (*(u32*)player & 0x100000) {
            motSlitContinue();
        }
        if (*(s16*)((s32)player + 0x4E) == 0) {
            void* player2 = marioGetPtr();
            speed = *(f32*)((s32)player2 + 0x184);
            if (*(u32*)player2 & 0x100000) {
                sx = *(s8*)((s32)player2 + 0x252);
                sy = *(s8*)((s32)player2 + 0x253);
                if ((sx * sx + sy * sy) <= 0xBD1) {
                    speed *= float_0p5_8042088c;
                }
            } else if (marioBgmodeChk() == 1) {
                speed *= float_0p5_8042088c;
            }
            speed *= *(f32*)((s32)player2 + 0x228);
            *(f32*)((s32)player + 0x180) = speed;
        }
        if (*(u32*)player & 0x2000) {
            marioChgPose(str_M_W_7_8042089c);
        } else if (*(u32*)player & 0x8000) {
            if (!marioChkPushAnime()) {
                marioChgPose(str_M_O_1_8042087c);
            }
        } else if (marioBgmodeChk() == 0) {
            marioChgPose(str_M_W_1_80420884);
        }
        if (!(*(u32*)((s32)player + 4) & 0x1000000)) {
            *(u32*)((s32)player + 0x44) = 0;
        }
        *(u32*)((s32)player + 0x48) = 0;
    }

    if (!(*(u32*)player & 0x2000) &&
        (strcmp(*(char**)((s32)player + 0x18), str_M_I_Y_80420890) == 0 ||
         *(s8*)((s32)player + 0x40) > 10)) {
        marioChgMot(0);
        return;
    }
    if (*(u32*)player & 0x2000) {
        *(f32*)((s32)player + 0x1A4) = *(f32*)((s32)player + 0x198);
        if (*(f32*)((s32)player + 0x194) == float_0_80420898) {
            marioChgMot(0);
        }
        return;
    }
    if (marioSlitAbilityChk() && marioSlitButton() == 1) {
        marioChgMot(0x15);
        return;
    }
    if (marioChkItemMotion() == 0) {
        marioChkJump();
        marioChkTransform();
        if (pouchEquipCheckBadge(0x143) == 0) {
            sx = *(s8*)((s32)player + 0x252);
            sy = *(s8*)((s32)player + 0x253);
            if ((sx * sx + sy * sy) > 0xBD1) {
                marioChgMot(2);
                return;
            }
        }
        *(f32*)((s32)player + 0x1A4) = *(f32*)((s32)player + 0x198);
        if (*(void**)((s32)player + 0x1E8) != 0) {
            marioWalkDashSe(*(void**)((s32)player + 0x1E8), 0x28);
        }
        *(u32*)((s32)player + 0x48) += 1;
        if (*(u32*)player & 0x8000) {
            if (!marioChkPushAnime()) {
                marioChgPose(str_M_O_1_8042087c);
            }
        }
        if (*(f32*)((s32)player + 0x194) == float_0_80420898) {
            marioChgMot(0);
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 marioWalkDashSe(void* pHit, u32 interval) {
    extern void* marioGetPtr(void);
    extern u32 hitGetAttr(void* hit);
    extern void psndSFXOn_3D(s32 sfx, void* pos);
    extern s32 strncmp(const char* a, const char* b, u32 n);
    extern void* gp;
    extern char str_mri_804208a4;
    void* mario;
    u32 attr;
    u32 frame;

    mario = marioGetPtr();
    attr = hitGetAttr(pHit);
    frame = *(u32*)((s32)mario + 0x48);

    if ((frame % interval) == 0) {
        if (attr & 0x100) {
            psndSFXOn_3D(0x143, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x200000) {
            psndSFXOn_3D(0x145, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x100000) {
            psndSFXOn_3D(0x147, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x1000) {
            if (strncmp((char*)((s32)gp + 0x12C), &str_mri_804208a4, 3) != 0) {
                psndSFXOn_3D(0x149, (void*)((s32)mario + 0x8C));
            } else {
                psndSFXOn_3D(0x143, (void*)((s32)mario + 0x8C));
            }
        } else if (attr & 0x400000) {
            psndSFXOn_3D(0x14B, (void*)((s32)mario + 0x8C));
        } else {
            psndSFXOn_3D(0x141, (void*)((s32)mario + 0x8C));
        }
    } else if ((frame % (interval >> 1)) == 0) {
        if (attr & 0x100) {
            psndSFXOn_3D(0x142, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x200000) {
            psndSFXOn_3D(0x144, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x100000) {
            psndSFXOn_3D(0x146, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x1000) {
            if (strncmp((char*)((s32)gp + 0x12C), &str_mri_804208a4, 3) != 0) {
                psndSFXOn_3D(0x148, (void*)((s32)mario + 0x8C));
            } else {
                psndSFXOn_3D(0x142, (void*)((s32)mario + 0x8C));
            }
        } else if (attr & 0x400000) {
            psndSFXOn_3D(0x14A, (void*)((s32)mario + 0x8C));
        } else {
            psndSFXOn_3D(0x140, (void*)((s32)mario + 0x8C));
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

f32 marioGetDashSpd(void) {
    extern void* marioGetPtr(void);
    extern s32 marioBgmodeChk(void);
    extern f32 float_0p5_8042088c;
    void* mario = marioGetPtr();
    f32 speed = *(f32*)((s32)mario + 0x188);

    if (*(u32*)mario & 0x100000) {
        s32 x = *(s8*)((s32)mario + 0x252);
        s32 y = *(s8*)((s32)mario + 0x253);
        speed = *(f32*)((s32)mario + 0x184);
        if (x * x + y * y <= 0xBD1) {
            speed *= float_0p5_8042088c;
        }
    } else if (marioBgmodeChk() == 1) {
        speed *= float_0p5_8042088c;
    }
    speed *= *(f32*)((s32)mario + 0x228);
    return speed;
}

float marioGetWalkSpd(void) {
    extern void* marioGetPtr(void);
    extern s32 marioBgmodeChk(void);
    extern f32 float_0p5_8042088c;
    void* mario = marioGetPtr();
    f32 speed = *(f32*)((s32)mario + 0x184);

    if (*(u32*)mario & 0x100000) {
        register s32 x = *(s8*)((s32)mario + 0x252);
        register s32 y = *(s8*)((s32)mario + 0x253);
        if (x * x + y * y <= 0xBD1) {
            speed *= float_0p5_8042088c;
        }
    } else if (marioBgmodeChk() == 1) {
        speed *= float_0p5_8042088c;
    }
    speed *= *(f32*)((s32)mario + 0x228);
    return speed;
}

