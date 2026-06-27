#include "mario/mario_motion.h"

extern f32 float_0_804209a0;
const char str_M_I_U_80420a14[] = "M_I_U";
extern void (*marioMotTbl[][3])(void);

void* marioGetPtr(void);
void marioReInit_ship(void);
void marioAdjustMoveDir(void);
void marioChgPose(const void* pose);

s32 L_marioChkRub(void) {
    return 0;
}

void marioChgShipMotion(void) {
    marioReInit_ship();
}

void mot_dummy(void) {
    marioGetPtr();
}

void mot_partyUse(void) {
    marioGetPtr();
}

void marioChgRollMotion(void) {
    void* mario = marioGetPtr();
    s32 mode = 0x32;
    s32 pose = 0x16;
    *(s32*)((s32)mario + 0x44) = mode;
    *(s16*)((s32)mario + 0x2E) = pose;
}

void marioClearJumpPara(void) {
    void* mario = marioGetPtr();
    f32 zero = float_0_804209a0;
    *(f32*)((s32)mario + 0x7C) = zero;
    *(f32*)((s32)mario + 0x80) = zero;
    *(f32*)((s32)mario + 0x84) = zero;
    *(f32*)((s32)mario + 0x88) = zero;
}

void mot_getItem(void) {
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        f32 zero;
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        zero = float_0_804209a0;
        *(s32*)((s32)mario + 0x48) = 0;
        *(s16*)((s32)mario + 0x50) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        *(f32*)((s32)mario + 0x180) = zero;
        marioAdjustMoveDir();
        marioChgPose(str_M_I_U_80420a14);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioMotion(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x200) = 0;
    do {
        marioMotTbl[*(u16*)((s32)mario + 0x2E)][0]();
    } while (*(u32*)((s32)mario + 0xC) & 1);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 marioLandOn(void) {
    return 0;
}


u8 mot_bottomless(void) {
    return 0;
}


u8 marioFall(void) {
    return 0;
}


u8 marioJump(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgMoveMotion(void) {
    extern void mario_plane_cancel(void);
    extern void allPartyForceRideOn(void);

    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);

    if (motion == 0x18) {
        mario_plane_cancel();
    } else if (motion == 0x16) {
        allPartyForceRideOn();
    } else if ((u16)(motion - 0x1F) > 1 && motion != 0x1A) {
        if (*(f32*)((s32)mario + 0x180) == float_0_804209a0) {
            void* wp = marioGetPtr();
            u16 m = *(u16*)((s32)wp + 0x2E);
            if (m != 0x19 && m != 0x1A && m != 0x15 && m != 0x16) {
                wp = marioGetPtr();
                if ((*(u32*)wp & 0x10000000) == 0) {
                    if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                        if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                            *(u32*)((s32)wp + 4) &= ~0x08000000;
                        }
                        if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                            *(u32*)((s32)wp + 4) &= ~4;
                        }
                    }
                    *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                    *(u16*)((s32)wp + 0x2E) = 0;
                    if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                        marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                    }
                    *(u32*)((s32)wp + 0xC) |= 1;
                }
            }
        }
        if (*(f32*)((s32)mario + 0x180) > *(f32*)((s32)mario + 0x184)) {
            void* wp = marioGetPtr();
            if ((*(u32*)wp & 0x10000000) == 0) {
                if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                    if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                        *(u32*)((s32)wp + 4) &= ~0x08000000;
                    }
                    if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                        *(u32*)((s32)wp + 4) &= ~4;
                    }
                }
                *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                *(u16*)((s32)wp + 0x2E) = 2;
                if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                    marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                }
                *(u32*)((s32)wp + 0xC) |= 1;
            }
        } else {
            void* wp = marioGetPtr();
            if ((*(u32*)wp & 0x10000000) == 0) {
                if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                    if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                        *(u32*)((s32)wp + 4) &= ~0x08000000;
                    }
                    if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                        *(u32*)((s32)wp + 4) &= ~4;
                    }
                }
                *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                *(u16*)((s32)wp + 0x2E) = 1;
                if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                    marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                }
                *(u32*)((s32)wp + 0xC) |= 1;
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mot_kaze(void) {
    extern f32 float_5_80420a00;
    extern f32 float_60_80420a04;
    extern f32 float_180_80420a08;
    extern f32 float_270_80420a0c;
    extern f32 float_6_80420a10;
    extern f32 float_90_804209f4;
    extern char str_M_P_2_804209f8[];
    extern f32 revise360(double);

    void* mario = marioGetPtr();

    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        f32 zero;
        *(u32*)((s32)mario + 0xC) &= ~1;
        zero = float_0_804209a0;
        *(u32*)mario &= ~0xF0000;
        *(u32*)((s32)mario + 4) |= 0x100;
        *(s32*)((s32)mario + 0x48) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        *(f32*)((s32)mario + 0x180) = zero;
        *(f32*)((s32)mario + 0x2B8) = zero;
        marioChgPose(str_M_P_2_804209f8);
    }

    if (*(s32*)((s32)mario + 0x44) == 100) {
        *(f32*)((s32)mario + 0x2B8) -= float_6_80420a10;
        if (*(f32*)((s32)mario + 0x2B8) < float_0_804209a0) {
            void* wp;
            *(f32*)((s32)mario + 0x2B8) = float_0_804209a0;
            *(u32*)((s32)mario + 4) &= ~0x100;
            wp = marioGetPtr();
            if ((*(u32*)wp & 0x10000000) == 0) {
                if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                    if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                        *(u32*)((s32)wp + 4) &= ~0x08000000;
                    }
                    if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                        *(u32*)((s32)wp + 4) &= ~4;
                    }
                }
                *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                *(u16*)((s32)wp + 0x2E) = 0;
                if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                    marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                }
                *(u32*)((s32)wp + 0xC) |= 1;
            }
        }
        *(f32*)((s32)mario + 0x1AC) = revise360((double)(*(f32*)((s32)mario + 0x1AC) + *(f32*)((s32)mario + 0x2B8)));
        *(f32*)((s32)mario + 0x1B0) = (*(f32*)((s32)mario + 0x1AC) > float_90_804209f4 && *(f32*)((s32)mario + 0x1AC) < float_270_80420a0c) ? float_180_80420a08 : float_0_804209a0;
    } else if (*(s32*)((s32)mario + 0x44) == 0) {
        *(f32*)((s32)mario + 0x2B8) += float_5_80420a00;
        if (*(f32*)((s32)mario + 0x2B8) > float_60_80420a04) {
            *(f32*)((s32)mario + 0x2B8) = float_60_80420a04;
        }
        *(f32*)((s32)mario + 0x1AC) = revise360((double)(*(f32*)((s32)mario + 0x1AC) + *(f32*)((s32)mario + 0x2B8)));
        if ((*(u16*)((s32)mario + 0x24A) & 0x10) == 0) {
            *(f32*)((s32)mario + 0x1B0) = (*(f32*)((s32)mario + 0x1AC) > float_90_804209f4 && *(f32*)((s32)mario + 0x1AC) < float_270_80420a0c) ? float_180_80420a08 : float_0_804209a0;
            *(s32*)((s32)mario + 0x44) = 100;
            *(s32*)((s32)mario + 0x48) = 10;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioChkJump(void) {
    extern u32 hitGetAttr(void*);
    extern void* mobjHitObjPtrToPtr(s32);
    extern s32 mobjGetHint(void*);
    extern void* caseCheckHitObj(void*);
    extern void* fbatNpcTalkCheck(void);

    void* mario = marioGetPtr();
    void* hit;
    void* c;

    if ((*(u16*)((s32)mario + 0x24C) & 0x100) == 0) {
        return 0;
    }
    if (*(u16*)((s32)mario + 0x2E) == 0x1A) {
        return 0;
    }

    hit = *(void**)((s32)mario + 0x1E0);
    if (hit != 0) {
        if ((hitGetAttr(hit) & 0x80000000) != 0) {
            if (mobjGetHint(mobjHitObjPtrToPtr((s32)hit)) == 1) {
                return 0;
            }
        }
        c = caseCheckHitObj(hit);
        if (c != 0 && (*(s32*)((s32)c + 4) == 9 || *(s32*)((s32)c + 4) == 4 || *(s32*)((s32)c + 4) == 0xC)) {
            return 0;
        }
    }

    hit = *(void**)((s32)mario + 0x1E4);
    if (hit != 0) {
        if ((hitGetAttr(hit) & 0x80000000) != 0) {
            if (mobjGetHint(mobjHitObjPtrToPtr((s32)hit)) == 1) {
                return 0;
            }
        }
        c = caseCheckHitObj(hit);
        if (c != 0 && (*(s32*)((s32)c + 4) == 9 || *(s32*)((s32)c + 4) == 4 || *(s32*)((s32)c + 4) == 0xC)) {
            return 0;
        }
    }

    hit = *(void**)((s32)mario + 0x1E8);
    if (hit != 0) {
        if ((hitGetAttr(hit) & 0x80000000) != 0) {
            if (mobjGetHint(mobjHitObjPtrToPtr((s32)hit)) == 1) {
                return 0;
            }
        }
        c = caseCheckHitObj(hit);
        if (c != 0 && *(s32*)((s32)c + 4) == 5) {
            return 0;
        }
    }

    if (fbatNpcTalkCheck() != 0) {
        return 0;
    }

    mario = marioGetPtr();
    if ((*(u32*)mario & 0x10000000) == 0) {
        if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
            if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                *(u32*)((s32)mario + 4) &= ~0x08000000;
            }
            if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                *(u32*)((s32)mario + 4) &= ~4;
            }
        }
        *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
        *(u16*)((s32)mario + 0x2E) = 3;
        if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
            marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
        }
        *(u32*)((s32)mario + 0xC) |= 1;
    }
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 mot_talk(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioChkTransform(void) {
    extern s32 pouchCheckItem(s32);
    extern s32 marioChkShipPlace(s32);
    extern s32 marioChkPlanePlace(s32);

    void* mario = marioGetPtr();

    if ((*(u16*)((s32)mario + 0x24C) & 0x800) == 0) {
        return 0;
    }
    if ((*(u32*)((s32)mario + 0xC) & 4) == 0 && *(s8*)((s32)mario + 0x3B) != 0) {
        return 0;
    }
    if (*(u16*)((s32)mario + 0x2E) == 0x1A) {
        return 0;
    }
    if (pouchCheckItem(5) > 0 && marioChkShipPlace(*(s32*)((s32)mario + 0x1E8)) != 0) {
        void* wp = marioGetPtr();
        if ((*(u32*)wp & 0x10000000) == 0) {
            if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)wp + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                    *(u32*)((s32)wp + 4) &= ~4;
                }
            }
            *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
            *(u16*)((s32)wp + 0x2E) = 0x19;
            if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
            }
            *(u32*)((s32)wp + 0xC) |= 1;
        }
        return 0;
    }
    if (pouchCheckItem(4) > 0 && marioChkPlanePlace(*(s32*)((s32)mario + 0x1E8)) != 0) {
        void* wp = marioGetPtr();
        if ((*(u32*)wp & 0x10000000) == 0) {
            if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)wp + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                    *(u32*)((s32)wp + 4) &= ~4;
                }
            }
            *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
            *(u16*)((s32)wp + 0x2E) = 0x18;
            if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
            }
            *(u32*)((s32)wp + 0xC) |= 1;
        }
        return 0;
    }
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioSetJumpPara(void) {
    extern f32 marioJumpData[];
    extern f32 marioJumpSwData[];
    extern f32 float_0p5_80420a1c;
    extern f32 float_1_804209b4;
    extern f32 float_2_80420a44;
    extern f32 float_neg0p003_80420a48;
    extern f32 float_0p029_80420a4c;
    extern f32 float_neg0p04_80420a50;
    extern f32 float_neg0p01_804209d4;
    extern f32 float_2p7_80420a54;
    extern f32 float_neg0p002_804209d0;
    extern f32 float_0p016_80420a58;
    extern void kpaSetJumpPara(void);

    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);

    switch (motion) {
        case 3:
        case 6:
        case 7:
        case 0x18:
        case 0x19:
            if ((*(u32*)((s32)mario + 0xC) & 0x10000) == 0) {
                if ((*(u32*)mario & 0x1000) == 0) {
                    *(f32*)((s32)mario + 0x7C) = marioJumpData[0];
                    *(f32*)((s32)mario + 0x80) = marioJumpData[1];
                    *(f32*)((s32)mario + 0x84) = marioJumpData[2];
                    *(f32*)((s32)mario + 0x88) = marioJumpData[3];
                } else {
                    *(f32*)((s32)mario + 0x7C) = float_0p5_80420a1c * marioJumpData[0];
                    *(f32*)((s32)mario + 0x80) = float_0p5_80420a1c * marioJumpData[1];
                    *(f32*)((s32)mario + 0x84) = float_0p5_80420a1c * marioJumpData[2];
                    *(f32*)((s32)mario + 0x88) = float_0p5_80420a1c * marioJumpData[3];
                }
                if (*(f32*)((s32)mario + 0x228) != float_1_804209b4) {
                    *(f32*)((s32)mario + 0x7C) = float_2_80420a44;
                    *(f32*)((s32)mario + 0x80) = float_neg0p003_80420a48;
                    *(f32*)((s32)mario + 0x84) = float_0p029_80420a4c;
                    *(f32*)((s32)mario + 0x88) = float_neg0p003_80420a48;
                }
            } else if ((*(u32*)mario & 0x1000) != 0) {
                *(f32*)((s32)mario + 0x7C) = float_0p5_80420a1c * marioJumpData[0];
                *(f32*)((s32)mario + 0x80) = float_0p5_80420a1c * marioJumpData[1];
                *(f32*)((s32)mario + 0x84) = float_0p5_80420a1c * marioJumpData[2];
                *(f32*)((s32)mario + 0x88) = float_0p5_80420a1c * marioJumpData[3];
            }
            break;
        case 4:
            *(f32*)((s32)mario + 0x7C) = float_2_80420a44;
            *(f32*)((s32)mario + 0x80) = float_neg0p04_80420a50;
            *(f32*)((s32)mario + 0x84) = float_neg0p01_804209d4;
            *(f32*)((s32)mario + 0x88) = float_neg0p003_80420a48;
            break;
        case 5:
            *(f32*)((s32)mario + 0x7C) = marioJumpSwData[0];
            *(f32*)((s32)mario + 0x80) = marioJumpSwData[1];
            *(f32*)((s32)mario + 0x84) = marioJumpSwData[2];
            *(f32*)((s32)mario + 0x88) = marioJumpSwData[3];
            break;
        case 0x16:
            *(f32*)((s32)mario + 0x7C) = marioJumpData[0];
            *(f32*)((s32)mario + 0x80) = marioJumpData[1];
            *(f32*)((s32)mario + 0x84) = marioJumpData[2];
            *(f32*)((s32)mario + 0x88) = marioJumpData[3];
            if (*(f32*)((s32)mario + 0x228) != float_1_804209b4) {
                *(f32*)((s32)mario + 0x7C) = float_2p7_80420a54;
                *(f32*)((s32)mario + 0x80) = float_neg0p002_804209d0;
                *(f32*)((s32)mario + 0x84) = float_0p016_80420a58;
                *(f32*)((s32)mario + 0x88) = float_neg0p002_804209d0;
            }
            break;
    }
    if (*(u16*)((s32)mario + 0x2E) != 8 && *(s8*)((s32)mario + 0x3C) == 2) {
        kpaSetJumpPara();
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioChkItemMotion(void) {
    extern s32 marioGetHammerLevel(void);
    extern s32 marioChkHammer2(void);

    void* mario = marioGetPtr();

    if ((*(u32*)mario & 0x80000000) != 0) {
        return 0;
    }
    if (marioGetHammerLevel() == 0) {
        return 0;
    }
    if (marioChkHammer2() != 0) {
        void* wp = marioGetPtr();
        if ((*(u32*)wp & 0x10000000) == 0) {
            if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)wp + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                    *(u32*)((s32)wp + 4) &= ~4;
                }
            }
            *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
            *(u16*)((s32)wp + 0x2E) = 0x13;
            if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
            }
            *(u32*)((s32)wp + 0xC) |= 1;
        }
        return 1;
    } else {
        u32 pressed = *(u16*)((s32)mario + 0x24C) & 0x200;
        if (pressed == 0) {
            return 0;
        }
        if ((*(u32*)mario & 0x01100000) != 0) {
            return 0;
        }
        if (pressed == 0) {
            return 0;
        }
        mario = marioGetPtr();
        if ((*(u32*)mario & 0x10000000) == 0) {
            if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
                if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)mario + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                    *(u32*)((s32)mario + 4) &= ~4;
                }
            }
            *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
            *(u16*)((s32)mario + 0x2E) = 0x12;
            if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
            }
            *(u32*)((s32)mario + 0xC) |= 1;
        }
        return 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioChgJumpStandMotion(double param_1) {
    extern void camFollowYOn(void);
    extern s32 marioChkSts(u32);
    extern void effKemuriEntry(double, double, double, double, s32);
    extern void yoshi_jumpStand(void);

    void* mario = marioGetPtr();
    f32 value = (f32)param_1;

    camFollowYOn();
    if (marioChkSts(0x10) != 0) {
        *(f32*)((s32)mario + 0x2D0) = value;
        effKemuriEntry(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x90), *(f32*)((s32)mario + 0x94), float_0_804209a0, 0);
        mario = marioGetPtr();
        if ((*(u32*)mario & 0x10000000) == 0) {
            if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
                if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)mario + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                    *(u32*)((s32)mario + 4) &= ~4;
                }
            }
            *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
            *(u16*)((s32)mario + 0x2E) = 6;
            if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
            }
            *(u32*)((s32)mario + 0xC) |= 1;
        }
    } else if (*(u16*)((s32)mario + 0x2E) == 0x1A) {
        *(f32*)((s32)mario + 0x2D0) = value;
        yoshi_jumpStand();
    } else {
        *(f32*)((s32)mario + 0x2D0) = value;
        mario = marioGetPtr();
        if ((*(u32*)mario & 0x10000000) == 0) {
            if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
                if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)mario + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                    *(u32*)((s32)mario + 4) &= ~4;
                }
            }
            *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
            *(u16*)((s32)mario + 0x2E) = 6;
            if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
            }
            *(u32*)((s32)mario + 0xC) |= 1;
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioBoots(void) {
    extern s32 kpaHipAttackOk(void);
    extern s32 marioChkHipAttack(void);

    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;

    if ((flags & 0x80000000) == 0 && (flags & 0x00100000) == 0) {
        if (*(s8*)((s32)mario + 0x3C) == 2) {
            if (*(s16*)((s32)mario + 0x52) >= 6 && kpaHipAttackOk() != 0 && (*(u16*)((s32)mario + 0x24C) & 0x100) != 0) {
                void* wp = marioGetPtr();
                if ((*(u32*)wp & 0x10000000) == 0) {
                    if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                        if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                            *(u32*)((s32)wp + 4) &= ~0x08000000;
                        }
                        if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                            *(u32*)((s32)wp + 4) &= ~4;
                        }
                    }
                    *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                    *(u16*)((s32)wp + 0x2E) = 0x10;
                    if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                        marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                    }
                    *(u32*)((s32)wp + 0xC) |= 1;
                }
            }
        } else if ((flags & 0x00080000) != 0) {
            if (marioChkHipAttack() != 0) {
                void* wp = marioGetPtr();
                if ((*(u32*)wp & 0x10000000) == 0) {
                    if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                        if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                            *(u32*)((s32)wp + 4) &= ~0x08000000;
                        }
                        if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                            *(u32*)((s32)wp + 4) &= ~4;
                        }
                    }
                    *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                    *(u16*)((s32)wp + 0x2E) = 0x10;
                    if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                        marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                    }
                    *(u32*)((s32)wp + 0xC) |= 1;
                }
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioRollChgChk(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern s32 marioGetRub(s32, void*, s32*, void*);
    extern s32 pouchCheckItem(s32);
    extern s32 marioSlitChkWallAround(void);
    extern s32 marioCreviceWallChk(Vec*);

    void* mario = marioGetPtr();
    s32 rub;
    s32 dir;
    s32 out1;
    s32 out2;
    Vec pos;

    if ((*(u32*)mario & 0x80000000) != 0) {
        return 0;
    }
    if ((*(u16*)((s32)mario + 0x24A) & 0x20) == 0) {
        return 0;
    }
    rub = marioGetRub(0x20, &out1, &dir, &out2);
    if (rub != 1) {
        return 0;
    }
    if (dir < 3) {
        return 0;
    }
    if (pouchCheckItem(3) == 0) {
        return 0;
    }
    if (marioSlitChkWallAround() == 0) {
        pos.x = *(f32*)((s32)mario + 0x8C);
        pos.y = *(f32*)((s32)mario + 0x90);
        pos.z = *(f32*)((s32)mario + 0x94);
        if (marioCreviceWallChk(&pos) != 0) {
            return 0;
        }
    } else {
        return 0;
    }

    if ((*(u16*)((s32)mario + 0x24A) & 0x20) != 0) {
        void* wp = marioGetPtr();
        if ((*(u32*)wp & 0x10000000) == 0) {
            if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)wp + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                    *(u32*)((s32)wp + 4) &= ~4;
                }
            }
            *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
            *(u16*)((s32)wp + 0x2E) = 0x16;
            if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
            }
            *(u32*)((s32)wp + 0xC) |= 1;
        }
        return 0x16;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 mot_forceReset(void) {
    return 0;
}


u8 mot_shadow(void) {
    return 0;
}


u8 marioChgSmallJumpMotion(void) {
    return 0;
}


u8 marioChgMotSub(s16 newMotionId, int param_2) {
    return 0;
}


u8 marioChgGetItemMotion(void) {
    return 0;
}


u8 marioChgStayMotion(void) {
    return 0;
}


u8 marioChgMotJump2(void) {
    return 0;
}


void marioChgMot(s32 motion) {
    ;
}


s32 marioChkTalkable(void) {
    return 0;
}


void marioChgTalkMotion(void) {
    ;
}


f32 marioGetFallSpd(void) {
    extern void* marioGetPtr(void);
    extern f32 marioJumpData[];
    extern f32 float_0_804209a0;
    void* mario = marioGetPtr();

    *(f32*)((s32)mario + 0x84) += *(f32*)((s32)mario + 0x88);
    if (*(f32*)((s32)mario + 0x84) > float_0_804209a0) {
        *(f32*)((s32)mario + 0x84) = float_0_804209a0;
    }
    *(f32*)((s32)mario + 0x80) += *(f32*)((s32)mario + 0x84);
    *(f32*)((s32)mario + 0x7C) += *(f32*)((s32)mario + 0x80);
    if (*(f32*)((s32)mario + 0x7C) <= marioJumpData[9]) {
        *(f32*)((s32)mario + 0x7C) = marioJumpData[9];
        *(f32*)((s32)mario + 0x80) = float_0_804209a0;
        *(f32*)((s32)mario + 0x84) = float_0_804209a0;
        *(f32*)((s32)mario + 0x88) = float_0_804209a0;
    }
    return *(f32*)((s32)mario + 0x7C);
}

void marioSetFallPara(void) {
    extern void* marioGetPtr(void);
    extern f32 marioJumpData[];
    extern f32 marioJumpSwData[];
    extern f32 float_1_804209b4;
    extern f32 float_0p34_804209cc;
    extern f32 float_neg0p002_804209d0;
    extern f32 float_neg0p01_804209d4;
    extern f32 float_0_804209a0;
    void* mario = marioGetPtr();
    f32* data = &marioJumpData[4];

    if (*(u16*)((s32)mario + 0x2E) == 5) {
        data = &marioJumpSwData[4];
    }
    *(f32*)((s32)mario + 0x7C) = data[0];
    *(f32*)((s32)mario + 0x80) = data[1];
    *(f32*)((s32)mario + 0x84) = data[2];
    *(f32*)((s32)mario + 0x88) = data[3];
    if (*(f32*)((s32)mario + 0x228) != float_1_804209b4) {
        *(f32*)((s32)mario + 0x7C) = float_0p34_804209cc;
        *(f32*)((s32)mario + 0x80) = float_neg0p002_804209d0;
        *(f32*)((s32)mario + 0x84) = float_neg0p01_804209d4;
        *(f32*)((s32)mario + 0x88) = float_0_804209a0;
    }
}

u32 marioChkItemGetMotion(void) {
    return 0;
}


double marioMakeJumpPara(void) {
    return 0.0;
}


void marioChgMot2(s32 motionId) {
    ;
}
