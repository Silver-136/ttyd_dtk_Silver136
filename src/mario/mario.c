#include "mario/mario.h"

void* marioGetPtr(void) {
    extern void* mp;
    return mp;
}

s32 marioCtrlOffChk(void) {
    extern void* mp;
    return *(s8*)((s32)mp + 0x38);
}

s32 marioGetColor(void) {
    extern void* mp;
    return *(s8*)((s32)mp + 0x3D);
}

s32 marioKeyOffChk(void) {
    extern void* mp;
    return *(s8*)((s32)mp + 0x39);
}

s32 marioItemGetOk(void) {
    extern void* mp;
    return ((*(u32*)mp >> 14) & 1) ^ 1;
}

s32 marioBgmodeChk(void) {
    extern void* mp;
    return (*(u32*)mp >> 25) & 1;
}

s32 marioChkCtrl(void) {
    extern void* mp;
    return *(s8*)((s32)mp + 0x38) == 0;
}

void marioItemGetDisable(void) {
    extern void* mp;
    void* mario = mp;

    *(u32*)mario |= 0x4000;
    *(s16*)((s32)mario + 0x7A) = 20;
}

f32 marioGetScale(void) {
    extern void* mp;
    extern f32 float_1p2_80420000;
    extern f32 float_2_80420004;

    if ((*(u32*)mp & 0x02000000) != 0) {
        return float_1p2_80420000;
    }
    return float_2_80420004;
}

void marioSoundInit(void) {
    extern void* mp;
    void* mario = mp;

    *(s32*)((s32)mario + 0x27C) = -1;
    *(s32*)((s32)mario + 0x280) = -1;
    *(s32*)((s32)mario + 0x284) = -1;
    *(s32*)((s32)mario + 0x288) = -1;
    *(s32*)((s32)mario + 0x28C) = -1;
}

s32 marioCtrlOff(void) {
    extern void* mp;
    void* mario = mp;

    *(u32*)mario |= 2;
    (*(u8*)((s32)mario + 0x38))++;
    return *(s8*)((s32)mario + 0x38);
}

s32 marioKeyOff(void) {
    extern void* mp;
    void* mario = mp;

    *(u32*)mario |= 8;
    (*(u8*)((s32)mario + 0x39))++;
    return *(s8*)((s32)mario + 0x39);
}

void marioPaperLightOff(void) {
    extern void* mp;
    extern void animPoseSetMaterialLightFlagOff(s32 poseId, u32 flag);

    animPoseSetMaterialLightFlagOff(*(s32*)((s32)mp + 0x22C), 1);
}

s32 marioCtrlOff2(void) {
    extern void* mp;
    void* mario = mp;

    *(u32*)mario |= 4;
    *(u32*)mario |= 2;
    (*(u8*)((s32)mario + 0x38))++;
    return *(s8*)((s32)mario + 0x38);
}

s32 marioCtrlOn(void) {
    extern void* mp;
    void* mario = mp;

    (*(u8*)((s32)mario + 0x38))--;
    if (*(s8*)((s32)mario + 0x38) == 0) {
        *(u32*)mario &= ~2;
    }
    return *(s8*)((s32)mario + 0x38);
}

s32 marioKeyOn(void) {
    extern void* mp;
    void* mario = mp;

    (*(u8*)((s32)mario + 0x39))--;
    if (*(s8*)((s32)mario + 0x39) == 0) {
        *(u32*)mario &= ~8;
    }
    return *(s8*)((s32)mario + 0x39);
}

s32 unk_8005ca2c(void) {
    extern void* mp;
    void* mario = mp;

    if (*(u16*)((s32)mario + 0x2E) == 0x16 || (*(u32*)mario & 0x00100000) != 0) {
        return 0;
    }
    return 1;
}

s32 marioChkInScreen(s32 x, s32 y) {
    s32 ret = 0;

    if (x >= 0 && x <= 600 && y >= 0 && y <= 535) {
        ret = 1;
    }
    return ((u32)-ret | (u32)ret) >> 31;
}

s32 marioCaseEventValidChk(void) {
    extern void* mp;
    extern s32 vivianGetStatus(void);

    if (*(u16*)((s32)mp + 0x2E) == 0x18) {
        return 0;
    }
    return vivianGetStatus() == 0;
}

s32 marioSetMutekiTime(s32 msec) {
    extern void* mp;
    extern s32 sysMsec2Frame(s32 msec);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    void* mario = mp;
    void* party;

    *(s16*)((s32)mario + 0x4C) = sysMsec2Frame(msec);
    party = partyGetPtr(marioGetPartyId());
    if (party != NULL) {
        *(s16*)((s32)party + 0x2A) = *(s16*)((s32)mario + 0x4C);
    }
    return *(s16*)((s32)mario + 0x4C);
}

void marioItemGetChk(void) {
    extern void* mp;
    void* mario = mp;
    s32 value = *(s16*)((s32)mario + 0x7A);

    if (value > 0) {
        value--;
        *(s16*)((s32)mario + 0x7A) = value;
        if ((s16)value <= 0) {
            *(s16*)((s32)mario + 0x7A) = 0;
            *(u32*)mp &= ~0x4000;
        }
    }
}

s32 marioCtrlOn2(void) {
    extern void* mp;
    void* mario = mp;

    *(u32*)mario &= ~4;
    (*(u8*)((s32)mario + 0x38))--;
    if (*(s8*)((s32)mario + 0x38) == 0) {
        *(u32*)mario &= ~2;
    }
    return *(s8*)((s32)mario + 0x38);
}

s32 N_marioChkUseParty(void) {
    extern void* gp;
    extern void* mp;

    if (((u16)*(u32*)((s32)gp + 0x1338) & 0x100F) != 0) {
        return 1;
    }
    return *(s8*)((s32)mp + 0x38);
}

s32 marioAnimeId(void) {
    extern void* mp;
    u32 flags = *(u32*)((s32)mp + 4);
    s32 ret;

    if ((flags & 0x10000000) != 0) {
        ret = 2;
    } else if ((flags & 0x80000000) != 0) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void marioPaperOff(void) {
    extern void* mp;
    extern void animPoseSetPaperAnimGroup(s32 poseId, s32 a, s32 b);
    extern void animPaperPoseRelease(s32 poseId);
    void* mario = mp;

    if ((*(u32*)((s32)mario + 4) & 0x40000000) != 0 && *(void**)((s32)mario + 0x1C) != NULL) {
        animPoseSetPaperAnimGroup(*(s32*)((s32)mario + 0x22C), 0, 0);
        *(s32*)((s32)mario + 0x1C) = 0;
        *(u32*)((s32)mario + 4) &= ~0x40000000;
        mario = mp;
        if (*(s32*)((s32)mario + 0x240) >= 0) {
            animPaperPoseRelease(*(s32*)((s32)mario + 0x240));
            *(s32*)((s32)mario + 0x240) = -1;
        }
    }
}

void marioOfsRotReset(void) {
    typedef struct WordVec {
        u32 x;
        u32 y;
        u32 z;
    } WordVec;
    extern void* mp;
    extern u8 str_a_mario_802c18a0[];
    void* base = str_a_mario_802c18a0;
    void* mario = mp;

    *(WordVec*)((s32)mario + 0xBC) = *(WordVec*)((s32)base + 0x88);
    *(WordVec*)((s32)mario + 0xB0) = *(WordVec*)((s32)base + 0x94);
    *(WordVec*)((s32)mario + 0x98) = *(WordVec*)((s32)base + 0xA0);
    *(WordVec*)((s32)mario + 0xA4) = *(WordVec*)((s32)base + 0xAC);
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioMove(void) {
    typedef struct RawVec {
        u32 x;
        u32 y;
        u32 z;
    } RawVec;

    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern void* mp;
    extern void* gp;
    extern char str_a_mario_802c18a0[];
    extern char str_M_Z_1_8041ff90[];
    extern char str_M_S_1_8041ff98[];
    extern char str_M_W_1_8041ffa0[];
    extern char str_M_R_1_8041ffa8[];
    extern void* dotMarioPose[][6];
    extern u8 marioMoveMain(void);
    extern void* camGetPtr(s32 camId);
    extern void GXSetProjection(void* projMtx, s32 projType);
    extern void GXGetProjectionv(f32* proj);
    extern void GXGetViewportv(f32* viewport);
    extern void GXProject(f32 x, f32 y, f32 z, void* model, f32* proj, f32* viewport, f32* outX, f32* outY, f32* outZ);
    extern s32 marioCamZoomOff(void);
    extern void camFollowYOff(void);
    extern void camFollowYOn(void);
    extern f32 PSVECDistance(void* a, void* b);
    extern s32 strcmp(const char* a, const char* b);
    extern f32 float_0_80420020;
    extern f32 float_1_80420008;
    extern f32 float_400_804200b4;

    char* base = str_a_mario_802c18a0;
    void* player = mp;
    void* current;
    void* cam;
    char* dotPose;
    RawVec oldPos;
    RawVec bgPos;
    RawVec curPos;
    VecLocal bgScreen;
    VecLocal screen;
    f32 bgViewport[6];
    f32 bgProj[7];
    f32 viewport[6];
    f32 proj[7];
    s32 stick2;

#define MARIO_DOT_POSE(_pose) \
    do { \
        dotPose = NULL; \
        if (strcmp(str_M_Z_1_8041ff90, (_pose)) == 0) { \
            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0]; \
        } else if (strcmp(str_M_S_1_8041ff98, (_pose)) == 0) { \
            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1]; \
        } else if (strcmp(str_M_W_1_8041ffa0, (_pose)) == 0) { \
            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2]; \
        } else if (strcmp(str_M_R_1_8041ffa8, (_pose)) == 0) { \
            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3]; \
        } else if (strcmp(base + 0x284, (_pose)) == 0) { \
            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5]; \
        } else if (strcmp(base + 0x294, (_pose)) == 0) { \
            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5]; \
        } \
    } while (0)

#define MARIO_CHG_POSE(_pose) \
    do { \
        current = mp; \
        if ((*(u32*)((s32)current + 0x4) & 0x2) == 0) { \
            if ((*(char**)((s32)current + 0x18) != NULL) && (strcmp(*(char**)((s32)current + 0x18), (_pose)) == 0)) { \
                return; \
            } \
            if ((*(u32*)current & 0x80000000) == 0) { \
                *(char**)((s32)current + 0x18) = (_pose); \
                *(s16*)((s32)current + 0x28) = 0; \
                *(u32*)((s32)current + 0x4) &= ~0x30000000; \
                *(u32*)((s32)current + 0xC) |= 0x1000; \
            } else { \
                MARIO_DOT_POSE(_pose); \
                if (dotPose != NULL) { \
                    *(char**)((s32)current + 0x18) = dotPose; \
                    *(s16*)((s32)current + 0x28) = 0; \
                    *(u32*)((s32)current + 0x4) &= ~0x30000000; \
                    *(u32*)((s32)current + 0xC) |= 0x1000; \
                } \
            } \
        } \
    } while (0)

    oldPos.x = *(u32*)((s32)player + 0x8C);
    oldPos.y = *(u32*)((s32)player + 0x90);
    oldPos.z = *(u32*)((s32)player + 0x94);

    marioMoveMain();

    current = mp;
    curPos.x = *(u32*)((s32)current + 0x8C);
    curPos.y = *(u32*)((s32)current + 0x90);
    curPos.z = *(u32*)((s32)current + 0x94);

    cam = camGetPtr(4);
    GXSetProjection((void*)((s32)cam + 0x15C), *(s32*)((s32)cam + 0x19C));
    GXGetProjectionv(proj);
    GXGetViewportv(viewport);
    GXProject(*(f32*)&curPos.x, *(f32*)&curPos.y, *(f32*)&curPos.z,
              (void*)((s32)cam + 0x11C), proj, viewport, &screen.x, &screen.y, &screen.z);

    *(s32*)((s32)current + 0x168) = (s32)screen.x;
    *(s32*)((s32)current + 0x16C) = (s32)screen.y;
    *(s32*)((s32)current + 0x170) = (s32)screen.z;

    if (((*(u32*)((s32)player + 0x4) & 0x00100000) != 0) &&
        ((*(f32*)((s32)player + 0x194) != float_0_80420020) ||
         ((*(u32*)((s32)player + 0x4) & 0x01000000) == 0))) {
        marioCamZoomOff();
    }

    if ((*(u32*)player & 0x02000000) != 0) {
        if (strcmp((char*)((s32)gp + 0x12C), base + 0x358) != 0) {
            if ((*(f32*)((s32)player + 0x194) != float_0_80420020) &&
                ((*(u32*)((s32)player + 0x4) & 0x00020000) == 0)) {
                camFollowYOff();
            }
        } else {
            bgPos.x = *(u32*)((s32)player + 0x8C);
            bgPos.y = *(u32*)((s32)player + 0x90);
            bgPos.z = *(u32*)((s32)player + 0x94);

            cam = camGetPtr(4);
            GXSetProjection((void*)((s32)cam + 0x15C), *(s32*)((s32)cam + 0x19C));
            GXGetProjectionv(bgProj);
            GXGetViewportv(bgViewport);
            GXProject(*(f32*)&bgPos.x, *(f32*)&bgPos.y, *(f32*)&bgPos.z,
                      (void*)((s32)cam + 0x11C), bgProj, bgViewport, &bgScreen.z, &bgScreen.y, &bgScreen.x);

            if (bgScreen.y >= float_400_804200b4) {
                camFollowYOn();
            }
        }

        if ((*(u16*)((s32)player + 0x2E) == 1) || (*(u16*)((s32)player + 0x2E) == 2)) {
            if (float_0_80420020 == *(f32*)&oldPos.x - *(f32*)((s32)player + 0x8C)) {
                stick2 = (s32)*(s8*)((s32)player + 0x252) * (s32)*(s8*)((s32)player + 0x252);
                if (stick2 < 0x19) {
                    MARIO_CHG_POSE(str_M_Z_1_8041ff90);
                } else if (stick2 < 0xBD1) {
                    MARIO_CHG_POSE(str_M_W_1_8041ffa0);
                } else {
                    MARIO_CHG_POSE(str_M_R_1_8041ffa8);
                }
            } else if (PSVECDistance(&oldPos, (void*)((s32)player + 0x8C)) > float_1_80420008) {
                MARIO_CHG_POSE(str_M_R_1_8041ffa8);
            } else {
                MARIO_CHG_POSE(str_M_W_1_8041ffa0);
            }
        }
    }

#undef MARIO_CHG_POSE
#undef MARIO_DOT_POSE
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioMain(void) {
    typedef struct RawVec {
        u32 x;
        u32 y;
        u32 z;
    } RawVec;

    extern void* mp;
    extern void* gp;
    extern u8 str_a_mario_802c18a0[];
    extern s32 g_blurPosId;
    extern f32 g_blurPosX[];
    extern f32 float_0_80420020;
    extern f32 float_0p5_80420068;
    extern f32 float_1_80420008;
    extern f32 float_30_80420048;

    extern s32 pouchEquipCheckBadge(s32 badgeId);
    extern s32 pouchGetHP(void);
    extern void seqSetSeq(s32 seq, char* map, char* bero);
    extern s32 strcmp(const char* s1, const char* s2);

    extern void marioSetCharMode(s32 mode);
    extern void marioChgMot(s32 motion);
    extern void marioSetDirEventMain(void);
    extern void marioPreCamera(void);
    extern void marioHitCheckWidth(void);
    extern void itemCoinDrop(RawVec* pos);
    extern void marioMotion(void);
    extern void* marioSearchGround(f64 x, f64 z, f32* outX, f32* outY, f32* outZ);
    extern u8 marioPreDisp(void);
    extern void partyMain(void);
    extern void marioCamMain(void);
    extern void marioCtrlOff2Main(void);
    extern void marioUpdateKeyData(void);
    extern void marioGetStick(f32* stickAngle, f32* stickPower);
    extern void marioUseParty(void);
    extern void kpaMain(void);
    extern void peachMain(void);
    extern void marioSearchUnder(void);
    extern void marioBottomless(void);
    extern void marioChkGnd(void);
    extern void marioChkToge(void);
    extern void marioBalloonMain(void);
    extern void marioMove(void);
    extern void mario_kemuri(void);
    extern f32 marioChkOverhead(void);
    extern void marioSetFallPara(void);
    extern void marioJump(void);
    extern void marioChkGnd2(void);
    extern void marioFall(void);
    extern void marioBoots(void);
    extern void marioForceMoveMain(void);

    u8* base = str_a_mario_802c18a0;
    void* player = mp;
    void* p;
    void* dst;
    RawVec coinPos;
    RawVec coinWork;
    f32 hitZ;
    f32 hitY;
    f32 hitX;
    s32 temp;
    s32 color;
    s32 idx;
    s32 next;
    u16 motion;
    f32 headY;

#define MARIO_FINALIZE(_p) \
    do { \
        (_p) = mp; \
        *(s16*)((s32)(_p) + 0x54) = *(s16*)((s32)(_p) + 0x50); \
        *(u32*)((s32)(_p) + 0x20C) = *(u32*)((s32)(_p) + 0x1E8); \
        if (pouchGetHP() <= 0) { \
            seqSetSeq(5, 0, 0); \
        } \
        { \
            u32 _y = *(u32*)((s32)(_p) + 0x90); \
            u32 _x = *(u32*)((s32)(_p) + 0x8C); \
            f32 _zero = float_0_80420020; \
            *(u32*)((s32)(_p) + 0x110) = _x; \
            *(u32*)((s32)(_p) + 0x114) = _y; \
            *(u32*)((s32)(_p) + 0x118) = *(u32*)((s32)(_p) + 0x94); \
            *(f32*)((s32)(_p) + 0x174) = _zero; \
            *(f32*)((s32)(_p) + 0x178) = _zero; \
            *(f32*)((s32)(_p) + 0x17C) = _zero; \
        } \
        *(u32*)((s32)(_p) + 0x34) = *(u32*)((s32)(_p) + 0x34) + 1; \
    } while (0)

#define MARIO_BLUR(_mapBase) \
    do { \
        if (((*(u32*)((s32)(_mapBase) + 0x34) & 1) != 0) && ((*(u32*)((s32)mp + 0x4) & 0x08000000) != 0)) { \
            idx = g_blurPosId; \
            dst = &g_blurPosX[idx * 3]; \
            next = idx + 1; \
            *(u32*)((s32)dst + 0x0) = *(u32*)((s32)mp + 0x8C); \
            *(u32*)((s32)dst + 0x4) = *(u32*)((s32)mp + 0x90); \
            *(u32*)((s32)dst + 0x8) = *(u32*)((s32)mp + 0x94); \
            g_blurPosId = next; \
            if (next >= 8) { \
                g_blurPosId = next - 8; \
            } \
        } \
    } while (0)

#define FINISH_NO_BLUR() \
    do { \
        MARIO_FINALIZE(p); \
        return; \
    } while (0)

#define FINISH_WITH_BLUR(_mapBase) \
    do { \
        MARIO_BLUR(_mapBase); \
        MARIO_FINALIZE(p); \
        return; \
    } while (0)

    if ((*(u32*)player & 1) == 0) {
        return;
    }

    if (((u16)*(u32*)((s32)gp + 0x1338) & 0x100F) != 0) {
        temp = 1;
    } else {
        temp = *(s8*)((s32)player + 0x38);
    }

    if (temp != 0) {
        if (*(s8*)((s32)player + 0x3C) == 0) {
            temp = pouchEquipCheckBadge(0x147);
            color = pouchEquipCheckBadge(0x148);
            if (temp != 0 && color == 0) {
                temp = 1;
            } else if (temp == 0 && color != 0) {
                temp = 2;
            } else if (temp != 0 && color != 0) {
                temp = 3;
            } else {
                temp = 0;
            }

            p = mp;
            if (*(s8*)((s32)p + 0x3C) == 0 && *(s8*)((s32)p + 0x3D) != temp) {
                *(s8*)((s32)p + 0x3D) = temp;
                marioSetCharMode(0);
            }
        }

        p = mp;
        if (pouchEquipCheckBadge(0x143) != 0) {
            motion = *(u16*)((s32)p + 0x2E);
            if (motion == 0) {
                *(u32*)((s32)p + 0xC) |= 0x1000;
            } else if (motion == 1) {
                *(u32*)((s32)p + 0xC) |= 0x1000;
            } else if (motion == 2) {
                marioChgMot(1);
                *(u32*)((s32)p + 0xC) |= 0x1000;
            }
        }
    }

    marioSetDirEventMain();
    *(f32*)((s32)player + 0x228) = float_1_80420008;

    if (strcmp((char*)((s32)gp + 0x12C), (char*)base + 0x360) == 0 ||
        strcmp((char*)((s32)gp + 0x12C), (char*)base + 0x368) == 0 ||
        strcmp((char*)((s32)gp + 0x12C), (char*)base + 0x370) == 0 ||
        strcmp((char*)((s32)gp + 0x12C), (char*)base + 0x378) == 0 ||
        strcmp((char*)((s32)gp + 0x12C), (char*)base + 0x380) == 0 ||
        strcmp((char*)((s32)gp + 0x12C), (char*)base + 0x388) == 0) {
        *(f32*)((s32)player + 0x228) = float_0p5_80420068;
    }

    marioPreCamera();
    marioHitCheckWidth();

    if (*(s16*)((s32)player + 0x4C) > 0) {
        *(s16*)((s32)player + 0x4C) = *(s16*)((s32)player + 0x4C) - 1;
    }

    if (*(s16*)((s32)player + 0x2D8) > 0) {
        *(s16*)((s32)player + 0x2D8) = *(s16*)((s32)player + 0x2D8) - 1;
        temp = *(s16*)((s32)player + 0x2D8);
        if ((temp % 30) == 0 && *(s16*)((s32)player + 0x2D8) > 0) {
            coinWork = *(RawVec*)((s32)base + 0xF4);
            *(f32*)&coinWork.y = *(f32*)((s32)player + 0x90) + float_30_80420048;
            coinWork.x = *(u32*)((s32)player + 0x8C);
            coinWork.z = *(u32*)((s32)player + 0x94);
            coinPos.x = coinWork.x;
            coinPos.y = coinWork.y;
            coinPos.z = coinWork.z;
            itemCoinDrop(&coinPos);
        }
    }

    if (*(s16*)((s32)player + 0x5E) > 0) {
        *(s16*)((s32)player + 0x5E) = *(s16*)((s32)player + 0x5E) - 1;
        if (*(s16*)((s32)player + 0x5E) <= 0) {
            *(u32*)((s32)player + 0x4) &= ~0x00100000;
        }
    }

    motion = *(u16*)((s32)player + 0x2E);
    if (motion == 0x1D) {
        marioMotion();
        *(void**)((s32)mp + 0x1E8) = marioSearchGround((f64)float_0_80420020, (f64)float_0_80420020, &hitX, &hitY, &hitZ);
        marioPreDisp();
        partyMain();
        marioCamMain();
        FINISH_WITH_BLUR(mp);
    }

    if ((*(u32*)player & 2) == 0) {
        marioUpdateKeyData();
        marioGetStick((f32*)((s32)player + 0x198), (f32*)((s32)player + 0x194));
        marioUseParty();
        marioMotion();

        if (*(s8*)((s32)player + 0x3C) == 2) {
            kpaMain();
            FINISH_NO_BLUR();
        }

        if (*(s8*)((s32)player + 0x3C) == 1) {
            peachMain();
            return;
        }

        motion = *(u16*)((s32)player + 0x2E);
        if (motion == 0x18 || (u16)(motion - 0x1F) <= 1 || motion == 0x21) {
            marioSearchUnder();
            marioPreDisp();
            marioBottomless();
            partyMain();
            marioCamMain();
            *(u32*)((s32)player + 0x1F8) = 0;
            *(u32*)((s32)player + 0x1E4) = 0;
            *(u32*)((s32)player + 0x1E0) = 0;
            *(u32*)((s32)player + 0x1F0) = 0;
            *(u32*)((s32)player + 0x1F4) = 0;
            FINISH_NO_BLUR();
        }

        if (motion == 0x1C) {
            marioChkGnd();
            marioChkToge();
            marioPreDisp();
            marioBalloonMain();
            marioBottomless();
            partyMain();
            marioCamMain();
            FINISH_NO_BLUR();
        }

        if (motion == 0x14) {
            marioMove();
            marioChkToge();
            marioPreDisp();
            marioBalloonMain();
            marioBottomless();
            partyMain();
            marioCamMain();
            FINISH_NO_BLUR();
        }

        if (motion == 0x1A) {
            marioChkToge();
            marioBottomless();
            marioPreDisp();
            marioBalloonMain();
            mario_kemuri();
            partyMain();
            marioCamMain();
            FINISH_NO_BLUR();
        }

        if (motion == 8) {
            marioBottomless();
            marioPreDisp();
            marioBalloonMain();
            mario_kemuri();
            partyMain();
            marioCamMain();
            FINISH_WITH_BLUR(player);
        }

        if ((*(u32*)player & 0x00040000) != 0) {
            marioMove();
            marioChkOverhead();
            if (*(s16*)((s32)player + 0x50) > 100) {
                marioChgMot(10);
                marioSetFallPara();
            }
        } else if ((*(u32*)player & 0x00010000) != 0) {
            marioJump();
            marioChkOverhead();
            marioMove();
            temp = 0;
            if (*(u16*)((s32)mp + 0x2E) == 0x16) {
                temp = 1;
            }
            if (temp == 0 && *(s16*)((s32)player + 0x50) > 5) {
                marioChkGnd2();
            }
        } else if ((*(u32*)player & 0x00020000) != 0) {
            if (motion != 0x19) {
                marioMove();
                if (*(u16*)((s32)player + 0x2E) == 0x10) {
                    headY = marioChkOverhead();
                    if (*(void**)((s32)player + 0x1F8) != 0) {
                        *(f32*)((s32)player + 0x90) = headY;
                    }
                } else {
                    marioChkOverhead();
                }
                marioFall();
                motion = *(u16*)((s32)player + 0x2E);
                if (motion == 0x10 || motion == 0x11) {
                    marioChkGnd2();
                } else {
                    temp = 0;
                    if (*(u16*)((s32)mp + 0x2E) == 0x16) {
                        temp = 1;
                    }
                    if (temp == 0 && *(s16*)((s32)player + 0x50) > 5) {
                        marioChkGnd2();
                    }
                }
            }
        } else {
            marioMove();
            marioChkOverhead();
            marioChkGnd();
        }

        marioBoots();
    } else if ((*(u32*)player & 4) != 0) {
        marioCtrlOff2Main();
    }

    marioForceMoveMain();
    marioBottomless();
    marioPreDisp();
    marioBalloonMain();
    mario_kemuri();
    partyMain();
    marioCamMain();
    FINISH_WITH_BLUR(player);

#undef FINISH_WITH_BLUR
#undef FINISH_NO_BLUR
#undef MARIO_BLUR
#undef MARIO_FINALIZE
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioSetCharMode(s32 mode) {
    extern void* mp;
    extern char str_a_mario_802c18a0[];
    extern void* marioAnimeGroupData[];
    extern char str_P_S_1_8041ff18[];
    extern char str_P_T_1_8041ff30[];
    extern char str_S_1_8041fe8c[];
    extern char str_T_1_804200d0[];
    extern char str_M_S_1_8041ff98[];
    extern char str_M_I_2_8041ffe8[];
    extern void peachPreInit(void);
    extern void kpaPreInit(void);
    extern s32 kpaGetStageViewType(void);
    extern void kpaSetLevel(s32 level);
    extern void animPoseRelease(s32 poseId);
    extern s32 animPoseEntry(void* name, s32 mode);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern u32 animPoseGetMaterialLightFlag(s32 poseId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern void animPoseSetMaterialLightFlagOn(s32 poseId, u32 flags);
    extern void npcSetMarioAutoTalkPose(const char* stay, const char* talk);
    extern s32 pouchEquipCheckBadge(s32 badgeId);
    extern void marioSetFamicomMode(s32 mode);
    extern void marioForcePlaneAnime(void);
    extern void marioForceShipAnime(void);
    extern void marioForceRollAnime(void);
    extern void marioForceSlitAnime(void);
    extern void N_marioForceVivianAnime(void);
    extern f32 float_1_80420008;
    extern f32 float_2_80420004;
    extern f32 float_10_80420078;
    extern f32 float_20_80420088;
    extern f32 float_25p9_804200c4;
    extern f32 float_30_80420048;
    extern f32 float_37_804200c0;
    extern f32 float_47_804200bc;
    extern f32 float_2p25_804200b8;
    s32 player = (s32)mp;
    s32 i;
    u32 materialFlags;
    u32 lightFlags;
    s8 color;

    if (mode == 1) {
        *(s8*)(player + 0x3C) = 1;
        peachPreInit();
        if (*(s32*)(player + 0x22C) >= 0) {
            animPoseRelease(*(s32*)(player + 0x22C));
            *(s32*)(player + 0x22C) = -1;
        }
        if (*(s32*)(player + 0x230) >= 0) {
            animPoseRelease(*(s32*)(player + 0x230));
            *(s32*)(player + 0x230) = -1;
        }
        if (*(s32*)(player + 0x234) >= 0) {
            animPoseRelease(*(s32*)(player + 0x234));
            *(s32*)(player + 0x234) = -1;
        }
        if ((*(u32*)(player + 0x10) & 0x2000) == 0) {
            *(s32*)(player + 0x22C) = animPoseEntry(str_a_mario_802c18a0 + 0x398, 2);
            *(s32*)(player + 0x230) = animPoseEntry(str_a_mario_802c18a0 + 0x3A0, 2);
            npcSetMarioAutoTalkPose(str_P_S_1_8041ff18, str_P_T_1_8041ff30);
        } else {
            *(s32*)(player + 0x22C) = animPoseEntry(str_a_mario_802c18a0 + 0x3AC, 2);
            *(s32*)(player + 0x230) = animPoseEntry(str_a_mario_802c18a0 + 0x3A0, 2);
            npcSetMarioAutoTalkPose(str_S_1_8041fe8c, str_T_1_804200d0);
            *(f32*)(player + 0xF8) = float_30_80420048;
            *(f32*)(player + 0xFC) = float_30_80420048;
        }
        *(f32*)(player + 0xF8) = float_20_80420088;
        *(f32*)(player + 0xFC) = float_47_804200bc;
        *(f32*)(player + 0x104) = float_1_80420008;
        *(f32*)(player + 0x108) = float_2_80420004;
        *(u32*)(player + 0xC) |= 0x1000;
        return;
    }

    if (mode == 2) {
        char* name;
        kpaPreInit();
        if (*(s32*)(player + 0x22C) >= 0) {
            animPoseRelease(*(s32*)(player + 0x22C));
            *(s32*)(player + 0x22C) = -1;
        }
        if (*(s32*)(player + 0x230) >= 0) {
            animPoseRelease(*(s32*)(player + 0x230));
            *(s32*)(player + 0x230) = -1;
        }
        if (*(s32*)(player + 0x234) >= 0) {
            animPoseRelease(*(s32*)(player + 0x234));
            *(s32*)(player + 0x234) = -1;
        }
        name = str_a_mario_802c18a0 + 0x3C0;
        if (kpaGetStageViewType() == 1) {
            name = str_a_mario_802c18a0 + 0x3B8;
        }
        *(s32*)(player + 0x22C) = animPoseEntry(name, 2);
        *(s8*)(player + 0x3C) = 2;
        npcSetMarioAutoTalkPose(str_a_mario_802c18a0 + 0x3CC,
                                str_a_mario_802c18a0 + 0x3D4);
        kpaSetLevel(1);
        *(u32*)(player + 0xC) |= 0x1000;
        return;
    }

    if ((*(u32*)player & 0x80000000) == 0) {
        if ((*(u32*)(player + 4) & 4) == 0) {
            i = ((*(u32*)(player + 4) & 1) != 0) ? 1 : 0;
        } else {
            i = 2;
        }
        materialFlags = animPoseGetMaterialFlag(*(s32*)(player + 0x22C + i * 4));
        lightFlags = animPoseGetMaterialLightFlag(*(s32*)(player + 0x22C + i * 4));
        *(s8*)(player + 0x3C) = 0;

        if (pouchEquipCheckBadge(0x147) != 0 && pouchEquipCheckBadge(0x148) == 0) {
            color = 1;
        } else if (pouchEquipCheckBadge(0x147) == 0 &&
                   pouchEquipCheckBadge(0x148) != 0) {
            color = 2;
        } else if (pouchEquipCheckBadge(0x147) != 0 &&
                   pouchEquipCheckBadge(0x148) != 0) {
            color = 3;
        } else {
            color = 0;
        }
        *(s8*)(player + 0x3D) = color;

        for (i = 0; i < 3; i++) {
            s32* poseId = (s32*)(player + 0x22C + i * 4);
            if (*poseId >= 0) {
                animPoseRelease(*poseId);
                *poseId = -1;
            }
        }
        for (i = 0; i < 3; i++) {
            s32 poseId = animPoseEntry(marioAnimeGroupData[color * 3 + i], 2);
            *(s32*)(player + 0x22C + i * 4) = poseId;
            animPoseSetMaterialFlagOn(poseId, materialFlags);
            animPoseSetMaterialLightFlagOn(poseId, lightFlags);
        }
        npcSetMarioAutoTalkPose(str_M_S_1_8041ff98, str_M_I_2_8041ffe8);
        *(u32*)(player + 0xC) |= 0x1000;
    } else {
        marioSetFamicomMode(1);
    }

    if ((*(u32*)player & 0x2000) == 0) {
        switch (*(u16*)(player + 0x2E)) {
            case 6:
                marioForcePlaneAnime();
                return;
            case 7:
                marioForceShipAnime();
                return;
            case 8:
                marioForceRollAnime();
                return;
            case 9:
                marioForceSlitAnime();
                return;
            case 10:
                N_marioForceVivianAnime();
                return;
        }
    }

    if (*(s8*)(player + 0x3C) == 1) {
        *(f32*)(player + 0xF8) = float_20_80420088;
        *(f32*)(player + 0xFC) = float_47_804200bc;
        *(f32*)(player + 0x104) = float_1_80420008;
        *(f32*)(player + 0x108) = float_2_80420004;
    } else if (*(s8*)(player + 0x3C) == 2) {
        *(f32*)(player + 0x104) = float_1_80420008;
        *(f32*)(player + 0x108) = float_2p25_804200b8;
    } else {
        if ((*(u32*)player & 0x2000) == 0) {
            *(f32*)(player + 0xF8) = float_20_80420088;
            *(f32*)(player + 0xFC) = float_37_804200c0;
        } else {
            *(f32*)(player + 0xF8) = float_10_80420078;
            *(f32*)(player + 0xFC) = float_25p9_804200c4;
        }
        *(f32*)(player + 0x104) = float_1_80420008;
        *(f32*)(player + 0x108) = float_2p25_804200b8;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioSetFamicomMode(int param_1) {
    extern void* mp;
    extern char str_a_mario_802c18a0[];
    extern void* marioAnimeGroupData[2];
    extern char* dotMarioPose[][6];
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_I_2_8041ffe8[6];
    extern void marioChgMot(s32 motion);
    extern void animPoseRelease(s32 poseId);
    extern s32 animPoseEntry(void* name, s32 mode);
    extern void npcSetMarioAutoTalkPose(const char* stay, const char* talk);
    extern s32 strcmp(const char* a, const char* b);
    char* base;
    char* dotPose;
    void* m;

#define MARIO_SET_STAY_POSE() \
    do { \
        m = mp; \
        if ((*(u32*)((s32)m + 0x4) & 0x2) == 0) { \
            if ((*(char**)((s32)m + 0x18) != 0) && (strcmp(*(char**)((s32)m + 0x18), str_M_S_1_8041ff98) == 0)) { \
                return; \
            } \
            if ((*(u32*)m & 0x80000000) == 0) { \
                *(char**)((s32)m + 0x18) = (char*)str_M_S_1_8041ff98; \
                *(s16*)((s32)m + 0x28) = 0; \
                *(u32*)((s32)m + 0x4) &= ~0x30000000; \
                *(u32*)((s32)m + 0xC) |= 0x1000; \
            } else { \
                dotPose = 0; \
                if (strcmp(str_M_Z_1_8041ff90, str_M_S_1_8041ff98) == 0) { \
                    dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0]; \
                } else if (strcmp(str_M_S_1_8041ff98, str_M_S_1_8041ff98) == 0) { \
                    dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1]; \
                } else if (strcmp(str_M_W_1_8041ffa0, str_M_S_1_8041ff98) == 0) { \
                    dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2]; \
                } else if (strcmp(str_M_R_1_8041ffa8, str_M_S_1_8041ff98) == 0) { \
                    dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3]; \
                } else if (strcmp(base + 0x284, str_M_S_1_8041ff98) == 0) { \
                    dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5]; \
                } else if (strcmp(base + 0x294, str_M_S_1_8041ff98) == 0) { \
                    dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5]; \
                } \
                if (dotPose != 0) { \
                    *(char**)((s32)m + 0x18) = dotPose; \
                    *(s16*)((s32)m + 0x28) = 0; \
                    *(u32*)((s32)m + 0x4) &= ~0x30000000; \
                    *(u32*)((s32)m + 0xC) |= 0x1000; \
                } \
            } \
        } \
    } while (0)

#define RELEASE_MARIO_POSES() \
    do { \
        if (*(s32*)((s32)m + 0x22C) >= 0) { \
            animPoseRelease(*(s32*)((s32)m + 0x22C)); \
        } \
        if (*(s32*)((s32)m + 0x230) >= 0) { \
            animPoseRelease(*(s32*)((s32)m + 0x230)); \
        } \
    } while (0)

#define SET_COMMON_POSE_STATE() \
    do { \
        *(s8*)((s32)m + 0x3C) = 0; \
        npcSetMarioAutoTalkPose(str_M_S_1_8041ff98, str_M_I_2_8041ffe8); \
        *(char**)((s32)m + 0x18) = 0; \
    } while (0)

    base = str_a_mario_802c18a0;
    m = mp;

    if (param_1 == 1) {
        if ((*(u32*)m & 0x80000000) == 0) {
            *(u32*)m |= 0x80000000;
            marioChgMot(0);
            RELEASE_MARIO_POSES();
            *(s32*)((s32)m + 0x22C) = animPoseEntry(base + 0x390, 2);
            *(s32*)((s32)m + 0x230) = animPoseEntry(marioAnimeGroupData[*(s8*)((s32)mp + 0x3D) * 3 + 1], 2);
            SET_COMMON_POSE_STATE();
            MARIO_SET_STAY_POSE();
        } else {
            *(char**)((s32)m + 0x18) = 0;
            MARIO_SET_STAY_POSE();
        }
    } else {
        if ((*(u32*)m & 0x80000000) != 0) {
            *(u32*)m &= ~0x80000000;
            RELEASE_MARIO_POSES();
            *(s32*)((s32)m + 0x22C) = animPoseEntry(marioAnimeGroupData[*(s8*)((s32)mp + 0x3D) * 3], 2);
            *(s32*)((s32)m + 0x230) = animPoseEntry(marioAnimeGroupData[*(s8*)((s32)mp + 0x3D) * 3 + 1], 2);
            SET_COMMON_POSE_STATE();
            MARIO_SET_STAY_POSE();
        } else {
            *(char**)((s32)m + 0x18) = 0;
            MARIO_SET_STAY_POSE();
        }
    }
#undef SET_COMMON_POSE_STATE
#undef RELEASE_MARIO_POSES
#undef MARIO_SET_STAY_POSE
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioRearAnime(void) {
    extern void* mp;
    extern f32 revise360(f64 angle);
    extern char* toRearPose(char* name);
    extern char* toFrontPose(char* name);
    extern s32 strcmp(const char* s1, const char* s2);
    extern f32 float_290_80420060;
    extern f32 float_70_80420064;
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_J_1B_802c1b24[7];
    extern const char str_M_J_1C_802c1b34[7];
    extern void* dotMarioPose[][6];
    void* m;
    char* pose;
    char* dotPose;
    f32 angle;

    m = mp;
    if (((*(u32*)((s32)m + 0x4) & 0x70000004) == 0) && ((*(u32*)m & 0x00002000) == 0)) {
        angle = revise360((f64)(*(f32*)((s32)m + 0x1A0) - *(f32*)((s32)m + 0x19C)));
        if ((angle >= float_290_80420060) || (angle <= float_70_80420064)) {
            pose = toRearPose(*(char**)((s32)m + 0x18));
            if (pose != NULL) {
                *(u32*)((s32)m + 0x4) |= 0x80000000;
                m = mp;
                if ((*(u32*)((s32)m + 0x4) & 0x2) == 0) {
                    if ((*(char**)((s32)m + 0x18) != NULL) && (strcmp(*(char**)((s32)m + 0x18), pose) == 0)) {
                        return;
                    }
                    if ((*(u32*)m & 0x80000000) == 0) {
                        *(char**)((s32)m + 0x18) = pose;
                        *(s16*)((s32)m + 0x28) = 0;
                        *(u32*)((s32)m + 0x4) &= ~0x30000000;
                        *(u32*)((s32)m + 0xC) |= 0x1000;
                    } else {
                        dotPose = NULL;
                        if (strcmp(str_M_Z_1_8041ff90, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
                        } else if (strcmp(str_M_S_1_8041ff98, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
                        } else if (strcmp(str_M_W_1_8041ffa0, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
                        } else if (strcmp(str_M_R_1_8041ffa8, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
                        } else if (strcmp(str_M_J_1B_802c1b24, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
                        } else if (strcmp(str_M_J_1C_802c1b34, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
                        }
                        if (dotPose != NULL) {
                            *(char**)((s32)m + 0x18) = dotPose;
                            *(s16*)((s32)m + 0x28) = 0;
                            *(u32*)((s32)m + 0x4) &= ~0x30000000;
                            *(u32*)((s32)m + 0xC) |= 0x1000;
                        }
                    }
                }
            } else {
                pose = toFrontPose(*(char**)((s32)m + 0x18));
                if (pose == NULL) {
                    *(u32*)((s32)m + 0x4) &= ~0x80000000;
                }
            }
        } else {
            *(u32*)((s32)m + 0x4) &= ~0x80000000;
            pose = toFrontPose(*(char**)((s32)m + 0x18));
            if (pose != NULL) {
                m = mp;
                if ((*(u32*)((s32)m + 0x4) & 0x2) == 0) {
                    if ((*(char**)((s32)m + 0x18) != NULL) && (strcmp(*(char**)((s32)m + 0x18), pose) == 0)) {
                        return;
                    }
                    if ((*(u32*)m & 0x80000000) == 0) {
                        *(char**)((s32)m + 0x18) = pose;
                        *(s16*)((s32)m + 0x28) = 0;
                        *(u32*)((s32)m + 0x4) &= ~0x30000000;
                        *(u32*)((s32)m + 0xC) |= 0x1000;
                    } else {
                        dotPose = NULL;
                        if (strcmp(str_M_Z_1_8041ff90, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
                        } else if (strcmp(str_M_S_1_8041ff98, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
                        } else if (strcmp(str_M_W_1_8041ffa0, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
                        } else if (strcmp(str_M_R_1_8041ffa8, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
                        } else if (strcmp(str_M_J_1B_802c1b24, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
                        } else if (strcmp(str_M_J_1C_802c1b34, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
                        }
                        if (dotPose != NULL) {
                            *(char**)((s32)m + 0x18) = dotPose;
                            *(s16*)((s32)m + 0x28) = 0;
                            *(u32*)((s32)m + 0x4) &= ~0x30000000;
                            *(u32*)((s32)m + 0xC) |= 0x1000;
                        }
                    }
                }
            }
        }
    } else {
        if ((*(u32*)((s32)m + 0x4) & 0x80000000) != 0) {
            *(u32*)((s32)m + 0x4) &= ~0x80000000;
            pose = toFrontPose(*(char**)((s32)m + 0x18));
            if (pose != NULL) {
                m = mp;
                if ((*(u32*)((s32)m + 0x4) & 0x2) == 0) {
                    if ((*(char**)((s32)m + 0x18) != NULL) && (strcmp(*(char**)((s32)m + 0x18), pose) == 0)) {
                        return;
                    }
                    if ((*(u32*)m & 0x80000000) == 0) {
                        *(char**)((s32)m + 0x18) = pose;
                        *(s16*)((s32)m + 0x28) = 0;
                        *(u32*)((s32)m + 0x4) &= ~0x30000000;
                        *(u32*)((s32)m + 0xC) |= 0x1000;
                    } else {
                        dotPose = NULL;
                        if (strcmp(str_M_Z_1_8041ff90, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
                        } else if (strcmp(str_M_S_1_8041ff98, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
                        } else if (strcmp(str_M_W_1_8041ffa0, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
                        } else if (strcmp(str_M_R_1_8041ffa8, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
                        } else if (strcmp(str_M_J_1B_802c1b24, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
                        } else if (strcmp(str_M_J_1C_802c1b34, pose) == 0) {
                            dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
                        }
                        if (dotPose != NULL) {
                            *(char**)((s32)m + 0x18) = dotPose;
                            *(s16*)((s32)m + 0x28) = 0;
                            *(u32*)((s32)m + 0x4) &= ~0x30000000;
                            *(u32*)((s32)m + 0xC) |= 0x1000;
                        }
                    }
                }
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 marioDispBlurSub(s32 param_1, int param_2) {
    typedef f32 MtxLocal[3][4];
    extern void* mp;
    extern void PSMTXScale(MtxLocal, f32, f32, f32);
    extern void PSMTXTrans(MtxLocal, double, double, double);
    extern void PSMTXRotRad(MtxLocal, double, char);
    extern void PSMTXConcat(MtxLocal, MtxLocal, MtxLocal);
    extern f32 revise360(f64);
    extern void animPoseSetMaterialFlagOn(s32, u32);
    extern void animPoseSetMaterialEvtColor(s32, void*);
    extern void animPoseDrawMtx(s32, MtxLocal, s32, double, double);
    extern f32 float_0_80420020;
    extern f32 float_1_80420008;
    extern f32 float_1p2_80420000;
    extern f32 float_2_80420004;
    extern f32 float_neg1_8042000c;
    extern f32 float_90_80420010;
    extern f32 float_270_80420014;
    extern f32 float_deg2rad_80420018;
    extern f32 float_180_8042001c;
    extern f32 g_blurPosX[];
    void* m = mp;
    MtxLocal a, b, c;
    f32 scale = float_2_80420004;
    f32 zScale;
    f32 angle;
    s32 poseIndex;
    u32 color;

    if ((*(u32*)m & 0x02000000) != 0) {
        scale = float_1p2_80420000;
    }
    PSMTXScale(c, *(f32*)((s32)m + 0xC8) * scale, *(f32*)((s32)m + 0xCC) * scale, *(f32*)((s32)m + 0xD0) * scale);
    if (((*(u32*)((s32)m + 4) & 0x200) == 0) && *(u16*)((s32)m + 0x2E) == 0x14) {
        angle = revise360((f64)*(f32*)((s32)m + 0xBC));
        zScale = (angle >= float_90_80420010 && angle <= float_270_80420014) ? float_neg1_8042000c : float_1_80420008;
        PSMTXScale(a, float_1_80420008, float_1_80420008, zScale);
        PSMTXConcat(a, c, c);
    }
    if ((*(u32*)((s32)m + 4) & 0xC00) != 0) {
        zScale = ((*(u32*)((s32)m + 4) & 0x800) != 0) ? float_1_80420008 : float_neg1_8042000c;
        PSMTXScale(a, float_1_80420008, float_1_80420008, zScale);
        PSMTXConcat(a, c, c);
    } else if ((*(u32*)((s32)m + 4) & 0x200) == 0) {
        angle = revise360((f64)*(f32*)((s32)m + 0x1AC));
        zScale = (angle >= float_90_80420010 && angle <= float_270_80420014) ? float_neg1_8042000c : float_1_80420008;
        PSMTXScale(a, float_1_80420008, float_1_80420008, zScale);
        PSMTXConcat(a, c, c);
    }
    PSMTXTrans(b, -(double)*(f32*)((s32)m + 0xB0), -(double)*(f32*)((s32)m + 0xB4), -(double)*(f32*)((s32)m + 0xB8));
    PSMTXConcat(b, c, c);
    PSMTXRotRad(b, (double)(float_deg2rad_80420018 * *(f32*)((s32)m + 0xC4)), 'z');
    PSMTXConcat(b, c, c);
    PSMTXRotRad(b, (double)(float_deg2rad_80420018 * *(f32*)((s32)m + 0xBC)), 'x');
    PSMTXConcat(b, c, c);
    angle = revise360((f64)(*(f32*)((s32)m + 0x1AC) - *(f32*)((s32)m + 0x19C)));
    PSMTXRotRad(b, (double)(float_deg2rad_80420018 * angle), 'y');
    PSMTXConcat(b, c, c);
    PSMTXTrans(b, (double)*(f32*)((s32)m + 0xB0), (double)*(f32*)((s32)m + 0xB4), (double)*(f32*)((s32)m + 0xB8));
    PSMTXConcat(b, c, c);
    PSMTXTrans(b, (double)(*(f32*)((s32)m + 0xA4) + *(f32*)((s32)m + 0x98) + g_blurPosX[param_2 * 3]),
              (double)(*(f32*)((s32)m + 0xA8) + *(f32*)((s32)m + 0x9C) + g_blurPosX[param_2 * 3 + 1]),
              (double)(*(f32*)((s32)m + 0xAC) + *(f32*)((s32)m + 0xA0) + g_blurPosX[param_2 * 3 + 2]));
    PSMTXConcat(b, c, c);
    poseIndex = (*(u32*)((s32)m + 4) & 0x10000000) != 0 ? 2 : ((*(u32*)((s32)m + 4) & 0x80000000) != 0 ? 1 : 0);
    animPoseSetMaterialFlagOn(*(s32*)((s32)m + 0x22C + poseIndex * 4), 0x40);
    color = 0xFFFFFF00 | (u8)(0x80 - (param_2 << 4));
    animPoseSetMaterialEvtColor(*(s32*)((s32)m + 0x22C + poseIndex * 4), &color);
    animPoseDrawMtx(*(s32*)((s32)m + 0x22C + poseIndex * 4), c, 1, (double)float_0_80420020, (double)float_1_80420008);
    animPoseDrawMtx(*(s32*)((s32)m + 0x22C + poseIndex * 4), c, 2, (double)float_0_80420020, (double)float_1_80420008);
    animPoseDrawMtx(*(s32*)((s32)m + 0x22C + poseIndex * 4), c, 3, (double)float_0_80420020, (double)float_1_80420008);
    return 0;
}

u8 marioMoveMain(void) {
    extern void* mp;
    extern s32 getRollEvtFlag(void);
    extern s32 L_marioChkRub(void);
    extern s32 pouchEquipCheckBadge(s32);
    extern void sincosf(f32, f32*, f32*);
    extern u32 hitGetAttr(void*);
    extern double cos(double);
    extern double angleABf(double, double, double, double);
    extern f32 toMovedir(f64);
    extern f32 marioGetMoveRate(f64);
    extern void marioCheckWall(double, double);
    extern void marioCheckWall_roll(double, double);
    extern void N_dou10_yoko_yari3(void*);
    extern void set_damage_root_ypos(double);
    extern void marioChgMot(s32);
    extern void marioChkToge(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void movePos(double, double, void*, void*);
    extern void marioSetRollDispDir(void);
    extern void marioShipSetDispDir(void);
    extern f32 float_0_80420020;
    extern f32 float_0p015_804200a4;
    extern f32 float_0p01_804200a8;
    extern f32 float_0p25_804200ac;
    extern f32 float_0p4_804200b0;
    extern f32 float_0p75_80420024;
    extern f32 float_3p1416_80420030;
    extern f32 float_180_8042001c;
    void* m = mp;
    f32 sx;
    f32 cz;
    f32 vx = float_0_80420020;
    f32 vz = float_0_80420020;
    f32 speed;
    f32 dir;
    f32 mag;
    u32 flags;
    void* party;
    u16 motion;

    if (*(u16*)((s32)m + 0x2E) != 0x17) {
        if ((*(u32*)m & 0x00100000) != 0) {
            *(void**)((s32)m + 0x1F4) = 0;
        }
        *(void**)((s32)m + 0x200) = 0;
        *(void**)((s32)m + 0x204) = 0;
        *(void**)((s32)m + 0x208) = 0;
        *(void**)((s32)m + 0x1FC) = 0;
    }
    if (((*(u32*)m & 2) == 0) && *(u16*)((s32)m + 0x2E) != 0x11 && getRollEvtFlag() == 0) {
        if ((*(u32*)m & 0x400000) == 0) {
            if (*(u16*)((s32)m + 0x2E) != 0x17 && L_marioChkRub() == 0) {
                if (((*(u32*)m & 0x00800000) == 0) && *(s16*)((s32)m + 0x50) != 0) {
                    speed = *(f32*)((s32)m + 0x194) * *(f32*)((s32)m + 0x1B8);
                    if ((*(u32*)m & 0x00080000) == 0) {
                        if (pouchEquipCheckBadge(0x47) == 0) {
                            vx = speed * ((*(u16*)((s32)m + 0x2E) == 0x15) ? float_0p01_804200a8 : float_0p015_804200a4);
                            if ((*(u32*)m & 0x00070000) == 0) {
                                vx *= float_0p25_804200ac;
                            }
                            if ((*(u32*)m & 0x02000000) != 0) {
                                vx *= float_0p4_804200b0;
                            }
                        } else {
                            vx = speed * float_0p01_804200a8;
                        }
                    } else if (*(u16*)((s32)m + 0x2E) == 0x10) {
                        vx = speed * float_0p015_804200a4;
                    }
                    sincosf(*(f32*)((s32)m + 0x198), &sx, &cz);
                    vz = vx * cz;
                    vx *= sx;
                }
                speed = *(f32*)((s32)m + 0x180);
                if (*(void**)((s32)m + 0x1F8) == 0) {
                    *(u32*)m &= ~0x20000000;
                } else {
                    if ((hitGetAttr(*(void**)((s32)m + 0x1F8)) & 0x40000) != 0) {
                        *(void**)((s32)m + 0x20C) = *(void**)((s32)m + 0x1F8);
                    }
                    if ((hitGetAttr(*(void**)((s32)m + 0x1F8)) & 0x20000000) != 0) {
                        speed *= float_0p75_80420024;
                        *(u32*)m |= 0x20000000;
                    } else {
                        *(u32*)m &= ~0x20000000;
                    }
                }
                sincosf(*(f32*)((s32)m + 0x1A4), &sx, &cz);
                vx += speed * sx;
                vz += speed * cz;
                if ((*(u32*)m & 0x02001000) == 0) {
                    if (*(f32*)((s32)m + 0x224) != float_0_80420020) {
                        vx *= (f32)cos((double)((float_3p1416_80420030 * *(f32*)((s32)m + 0x218)) / float_180_8042001c));
                    }
                    if (*(f32*)((s32)m + 0x21C) != float_0_80420020) {
                        vz *= (f32)cos((double)((float_3p1416_80420030 * *(f32*)((s32)m + 0x218)) / float_180_8042001c));
                    }
                }
                vx += *(f32*)((s32)m + 0x174);
                vz += *(f32*)((s32)m + 0x17C);
                *(f32*)((s32)m + 0x90) += *(f32*)((s32)m + 0x178);
                mag = vx * vx + vz * vz;
                if (mag != float_0_80420020) {
                    dir = (f32)angleABf(0.0, 0.0, (double)vx, (double)vz);
                } else {
                    dir = toMovedir((f64)*(f32*)((s32)m + 0x1AC));
                }
                speed = mag;
                if (((*(u32*)m & 0x00080000) == 0) && ((*(u32*)m & 0x00060000) != 0) &&
                    (*(f32*)((s32)m + 0x188) * *(f32*)((s32)m + 0x1B8) < speed)) {
                    speed = *(f32*)((s32)m + 0x188) * *(f32*)((s32)m + 0x1B8);
                }
                if ((*(u32*)m & 0x00100000) == 0) {
                    speed *= marioGetMoveRate((f64)*(f32*)((s32)m + 0x1A0));
                }
                if ((*(u32*)m & 0x00400000) != 0) {
                    speed = float_0_80420020;
                }
                *(f32*)((s32)m + 0x18C) = speed;
                if ((*(u32*)m & 0x08000000) != 0) {
                    dir = *(f32*)((s32)m + 0x1A8);
                    *(f32*)((s32)m + 0x1A4) = *(f32*)((s32)m + 0x1A8);
                } else if ((*(u32*)m & 0x00000800) != 0) {
                    dir = *(f32*)((s32)m + 0x1A4);
                }
                if ((*(u32*)m & 0x00000800) == 0) {
                    if ((*(u32*)m & 0x00100000) == 0 || ((*(u32*)m & 0x00020000) == 0)) {
                        marioCheckWall((double)speed, (double)dir);
                    } else {
                        marioCheckWall_roll((double)speed, (double)dir);
                        N_dou10_yoko_yari3((void*)((s32)m + 0x214));
                        if (*(void**)((s32)m + 0x200) != 0 && (hitGetAttr(*(void**)((s32)m + 0x200)) & 0x100000) != 0) {
                            set_damage_root_ypos((double)*(f32*)((s32)m + 0x90));
                            marioChgMot(0x20);
                            return 0;
                        }
                    }
                    marioChkToge();
                } else {
                    movePos((double)*(f32*)((s32)m + 0x180), (double)dir, (void*)((s32)m + 0x8C), (void*)((s32)m + 0x94));
                }
            }
        } else {
            motion = *(u16*)((s32)m + 0x2E);
            if (motion == 0x13) {
                party = partyGetPtr(marioGetPartyId());
                *(f32*)((s32)m + 0x1A0) = *(f32*)((s32)m + 0x1A4);
                movePos((double)*(f32*)((s32)m + 0x180), (double)*(f32*)((s32)m + 0x1A4), (void*)((s32)party + 0x8C), (void*)((s32)party + 0x94));
                *(f32*)((s32)m + 0x8C) = *(f32*)((s32)party + 0x8C);
                *(f32*)((s32)m + 0x94) = *(f32*)((s32)party + 0x94);
            } else {
                if (motion == 0x15) {
                    marioSetRollDispDir();
                } else if (motion == 0x17) {
                    marioShipSetDispDir();
                }
                if (motion != 0x15 && motion != 0x17 && motion != 0x18) {
                    *(f32*)((s32)m + 0x1A0) = *(f32*)((s32)m + 0x1A4);
                }
                movePos((double)*(f32*)((s32)m + 0x180), (double)*(f32*)((s32)m + 0x1A4), (void*)((s32)m + 0x8C), (void*)((s32)m + 0x94));
            }
        }
    }
    return 0;
}

void marioMakeDispDir(void) {
    extern void* mp;
    extern f32 revise360(f64);
    extern f32 float_0_80420020;
    extern f32 float_0p5_80420068;
    extern f32 float_10_80420078;
    extern f32 float_170_8042007c;
    extern f32 float_180_8042001c;
    extern f32 float_190_80420070;
    extern f32 float_250_8042006c;
    extern f32 float_290_80420060;
    extern f32 float_350_80420074;
    extern f32 float_0p125_80420084;
    extern f32 float_0p3_80420080;
    extern f32 float_30_80420048;
    extern f32 float_20_80420088;
    extern f32 float_200_80420090;
    extern f32 float_210_8042008c;
    void* mario = mp;
    f32 diff;
    f32 target;
    f32 current;
    f32 step;

    if ((*(u32*)((s32)mario + 4) & 0x01000000) != 0) {
        return;
    }

    if ((*(u32*)((s32)mario + 4) & 0x100) == 0) {
        diff = revise360((f64)(*(f32*)((s32)mario + 0x1A0) -
                               *(f32*)((s32)mario + 0x19C)));
        target = (f32)(s32)(float_0p5_80420068 + *(f32*)((s32)mario + 0x1B0));
        if ((target == float_290_80420060 || target == float_250_8042006c) &&
            diff >= float_180_8042001c) {
            target = float_0_80420020;
        }
        if (*(f32*)((s32)mario + 0x180) != float_0_80420020) {
            if ((diff >= float_190_80420070 && diff <= float_350_80420074)) {
                target = float_0_80420020;
            } else if (diff >= float_10_80420078 && diff <= float_170_8042007c) {
                target = float_180_8042001c;
            }
        }
        *(f32*)((s32)mario + 0x1B0) = target;
        if ((*(u32*)((s32)mario + 4) & 0x200) != 0) {
            *(f32*)((s32)mario + 0x1AC) = target;
        }
        if (*(f32*)((s32)mario + 0x1AC) == target) {
            return;
        }
    }

    target = *(f32*)((s32)mario + 0x1B0);
    current = *(f32*)((s32)mario + 0x1AC);
    if (*(u16*)((s32)mario + 0x2E) == 0x12) {
        if (target == float_0_80420020) {
            *(f32*)((s32)mario + 0x1AC) = float_0_80420020;
            *(u32*)((s32)mario + 0xC) &= ~0x100;
        } else if (target == float_180_8042001c) {
            *(f32*)((s32)mario + 0x1AC) = float_180_8042001c;
            *(u32*)((s32)mario + 0xC) &= ~0x100;
        }
        return;
    }

    if (target == float_0_80420020) {
        if ((*(u32*)((s32)mario + 0xC) & 0x400) != 0) {
            *(u32*)((s32)mario + 0xC) &= ~0x400;
            *(u32*)((s32)mario + 0xC) &= ~0x100;
        }
        *(u32*)((s32)mario + 0xC) |= 0x200;
        if ((*(u32*)((s32)mario + 0xC) & 0x100) == 0) {
            step = float_0p125_80420084 * (-float_30_80420048 - current);
        } else {
            step = float_0p3_80420080 * (float_10_80420078 - current);
        }
        current += step;
        if ((*(u32*)((s32)mario + 0xC) & 0x100) == 0) {
            if (current < -float_20_80420088) {
                *(u32*)((s32)mario + 0xC) |= 0x100;
            }
        } else if (current >= float_0_80420020) {
            current = float_0_80420020;
            *(u32*)((s32)mario + 0xC) &= ~0x100;
        }
    } else if (target == float_180_8042001c) {
        if ((*(u32*)((s32)mario + 0xC) & 0x200) != 0) {
            *(u32*)((s32)mario + 0xC) &= ~0x200;
            *(u32*)((s32)mario + 0xC) &= ~0x100;
        }
        *(u32*)((s32)mario + 0xC) |= 0x400;
        if ((*(u32*)((s32)mario + 0xC) & 0x100) == 0) {
            step = float_0p125_80420084 * (float_210_8042008c - current);
        } else {
            step = float_0p3_80420080 * (float_170_8042007c - current);
        }
        current += step;
        if ((*(u32*)((s32)mario + 0xC) & 0x100) == 0) {
            if (current > float_200_80420090) {
                *(u32*)((s32)mario + 0xC) |= 0x100;
            }
        } else if (current <= float_180_8042001c) {
            current = float_180_8042001c;
            *(u32*)((s32)mario + 0xC) &= ~0x100;
        }
    }
    *(f32*)((s32)mario + 0x1AC) = current;
}

u8 marioDisp(void) {
    typedef f32 MtxLocal[3][4];
    extern void* mp;
    extern void PSMTXScale(MtxLocal, f32, f32, f32);
    extern void PSMTXTrans(MtxLocal, double, double, double);
    extern void PSMTXRotRad(MtxLocal, double, char);
    extern void PSMTXConcat(MtxLocal, MtxLocal, MtxLocal);
    extern f32 revise360(f64);
    extern void animPoseDrawMtx(s32, MtxLocal, s32, double, double);
    extern void animPoseSetLocalTimeRate(double, s32);
    extern f32 float_0_80420020;
    extern f32 float_1_80420008;
    extern f32 float_1p2_80420000;
    extern f32 float_2_80420004;
    extern f32 float_neg1_8042000c;
    extern f32 float_90_80420010;
    extern f32 float_270_80420014;
    extern f32 float_deg2rad_80420018;
    extern f32 float_180_8042001c;
    extern f32 float_0p75_80420024;
    void* m = mp;
    MtxLocal a, b, c;
    f32 scale = float_2_80420004;
    f32 zScale;
    f32 angle;
    s32 poseIndex;

    if ((*(u32*)m & 0x02000000) != 0) {
        scale = float_1p2_80420000;
    }
    PSMTXScale(c, *(f32*)((s32)m + 0xC8) * scale, *(f32*)((s32)m + 0xCC) * scale, *(f32*)((s32)m + 0xD0) * scale);
    if (((*(u32*)((s32)m + 4) & 0x200) == 0) && *(u16*)((s32)m + 0x2E) == 0x14) {
        angle = revise360((f64)*(f32*)((s32)m + 0xBC));
        zScale = float_1_80420008;
        if (angle >= float_90_80420010 && angle <= float_270_80420014) {
            zScale = float_neg1_8042000c;
        }
        PSMTXScale(a, float_1_80420008, float_1_80420008, zScale);
        PSMTXConcat(a, c, c);
    }
    if ((*(u32*)((s32)m + 4) & 0xC00) != 0) {
        zScale = float_neg1_8042000c;
        if ((*(u32*)((s32)m + 4) & 0x800) != 0) {
            zScale = float_1_80420008;
        }
        PSMTXScale(a, float_1_80420008, float_1_80420008, zScale);
        PSMTXConcat(a, c, c);
    } else if ((*(u32*)((s32)m + 4) & 0x200) == 0) {
        angle = revise360((f64)*(f32*)((s32)m + 0x1AC));
        zScale = float_1_80420008;
        if (angle >= float_90_80420010 && angle <= float_270_80420014) {
            zScale = float_neg1_8042000c;
        }
        PSMTXScale(a, float_1_80420008, float_1_80420008, zScale);
        PSMTXConcat(a, c, c);
    }
    PSMTXTrans(b, -(double)*(f32*)((s32)m + 0xB0), -(double)*(f32*)((s32)m + 0xB4), -(double)*(f32*)((s32)m + 0xB8));
    PSMTXConcat(b, c, c);
    if (*(u16*)((s32)m + 0x2E) == 0x16) {
        PSMTXRotRad(b, (double)(float_deg2rad_80420018 * *(f32*)((s32)m + 0xBC)), 'x');
        PSMTXConcat(b, c, c);
        PSMTXRotRad(b, (double)(float_deg2rad_80420018 * *(f32*)((s32)m + 0xC4)), 'z');
        PSMTXConcat(b, c, c);
    } else {
        PSMTXRotRad(b, (double)(float_deg2rad_80420018 * *(f32*)((s32)m + 0xC4)), 'z');
        PSMTXConcat(b, c, c);
        PSMTXRotRad(b, (double)(float_deg2rad_80420018 * *(f32*)((s32)m + 0xBC)), 'x');
        PSMTXConcat(b, c, c);
    }
    angle = revise360((f64)(*(f32*)((s32)m + 0x1AC) - *(f32*)((s32)m + 0x19C)));
    PSMTXRotRad(b, (double)(float_deg2rad_80420018 * angle), 'y');
    PSMTXConcat(b, c, c);
    if ((*(u32*)((s32)m + 4) & 0xC00) != 0) {
        angle = revise360((f64)*(f32*)((s32)m + 0x1AC));
        if (angle < float_90_80420010 || angle > float_270_80420014) {
            angle = ((*(u32*)((s32)m + 4) & 0x800) != 0) ? float_0_80420020 : float_180_8042001c;
        } else {
            angle = ((*(u32*)((s32)m + 4) & 0x800) != 0) ? float_180_8042001c : float_0_80420020;
        }
        PSMTXRotRad(b, (double)(float_deg2rad_80420018 * angle), 'y');
        PSMTXConcat(b, c, c);
    }
    PSMTXTrans(b, (double)*(f32*)((s32)m + 0xB0), (double)*(f32*)((s32)m + 0xB4), (double)*(f32*)((s32)m + 0xB8));
    PSMTXConcat(b, c, c);
    if ((*(u32*)m & 0x00100000) != 0) {
        PSMTXScale(a, float_1_80420008, *(f32*)((s32)m + 0x130), float_1_80420008);
        PSMTXConcat(a, c, c);
    }
    PSMTXTrans(b, (double)(*(f32*)((s32)m + 0xA4) + *(f32*)((s32)m + 0x8C) + *(f32*)((s32)m + 0x98)),
              (double)(*(f32*)((s32)m + 0xA8) + *(f32*)((s32)m + 0x90) + *(f32*)((s32)m + 0x9C)),
              (double)(*(f32*)((s32)m + 0xAC) + *(f32*)((s32)m + 0x94) + *(f32*)((s32)m + 0xA0)));
    PSMTXConcat(b, c, c);
    poseIndex = (*(u32*)((s32)m + 4) & 0x10000000) != 0 ? 2 : ((*(u32*)((s32)m + 4) & 0x80000000) != 0 ? 1 : 0);
    animPoseDrawMtx(*(s32*)((s32)m + 0x22C + poseIndex * 4), c, 2, (double)float_0_80420020, (double)float_1_80420008);
    animPoseDrawMtx(*(s32*)((s32)m + 0x22C + poseIndex * 4), c, 1, (double)float_0_80420020, (double)float_1_80420008);
    scale = float_1_80420008;
    if ((*(u32*)m & 0x20000000) != 0) {
        scale = float_0p75_80420024;
    }
    animPoseSetLocalTimeRate((double)scale, *(s32*)((s32)m + 0x22C + poseIndex * 4));
    return 0;
}

char* toFrontPose(char* name) {
    extern void* mp;
    extern s32 strcmp(const char*, const char*);
    extern char str_P_Z_1R_802c19a0[], str_P_Z_1_8041ff10[];
    extern char str_P_S_1R_802c19a8[], str_P_S_1_8041ff18[];
    extern char str_P_W_1R_802c19b0[], str_P_W_1_8041ff20[];
    extern char str_P_R_1R_802c19b8[], str_P_R_1_8041ff28[];
    extern char str_P_T_1R_802c19c0[], str_P_T_1_8041ff30[];
    extern char str_P_Z_2R_802c19c8[], str_P_Z_2_8041ff38[];
    extern char str_P_S_2R_802c19d0[], str_P_S_2_8041ff40[];
    extern char str_P_W_2R_802c19d8[], str_P_W_2_8041ff48[];
    extern char str_P_R_2R_802c19e0[], str_P_R_2_8041ff50[];
    extern char str_P_T_2R_802c19e8[], str_P_T_2_8041ff58[];
    extern char str_P_Z_3R_802c19f0[], str_P_Z_3_8041ff60[];
    extern char str_P_S_3R_802c19f8[], str_P_S_3_8041ff68[];
    extern char str_P_W_3R_802c1a00[], str_P_W_3_8041ff70[];
    extern char str_P_R_3R_802c1a08[], str_P_R_3_8041ff78[];
    extern char str_P_T_3R_802c1a10[], str_P_T_3_8041ff80[];
    extern char str_P_O_Z_1R_802c1a18[], str_P_O_Z_1_802c1a24[];
    extern char str_P_O_S_1R_802c1a2c[], str_P_O_S_1_802c1a38[];
    extern char str_P_O_F_1R_802c1a40[], str_P_O_F_1_802c1a4c[];
    extern char str_P_O_R_1R_802c1a54[], str_P_O_R_1_802c1a60[];
    extern char str_P_O_N_1R_802c1a68[], str_P_O_N_1_802c1a74[];
    extern char str_P_O_S_2R_802c1a7c[], str_P_O_S_2_802c1a88[];
    extern char str_P_O_F_2R_802c1a90[], str_P_O_F_2_802c1a9c[];
    extern char str_P_O_R_2R_802c1aa4[], str_P_O_R_2_802c1ab0[];
    extern char str_P_O_N_2R_802c1ab8[], str_P_O_N_2_802c1ac4[];
    extern char str_P_O_D_1R_802c1acc[], str_P_O_D_1_802c1ad8[];
    extern char str_P_O_D_2R_802c1ae0[], str_P_O_D_2_802c1aec[];
    extern char str_P_A_1R_802c1af4[], str_P_A_1_8041ff88[];
    extern char str_M_Z_1R_802c1afc[], str_M_Z_1_8041ff90[];
    extern char str_M_S_1R_802c1b04[], str_M_S_1_8041ff98[];
    extern char str_M_W_1R_802c1b0c[], str_M_W_1_8041ffa0[];
    extern char str_M_R_1R_802c1b14[], str_M_R_1_8041ffa8[];
    extern char str_M_J_1BR_802c1b1c[], str_M_J_1B_802c1b24[];
    extern char str_M_J_1CR_802c1b2c[], str_M_J_1C_802c1b34[];
    extern char str_M_H_1R_802c1b3c[], str_M_H_1_8041ffb0[];
    extern char str_M_H_2R_802c1b44[], str_M_H_2_8041ffb8[];
    extern char str_M_H_4R_802c1b4c[], str_M_H_4_8041ffc0[];
    extern char str_M_H_5R_802c1b54[], str_M_H_5_8041ffc8[];
    extern char str_M_H_7R_802c1b5c[], str_M_H_7_8041ffd0[];
    extern char str_M_H_8R_802c1b64[], str_M_H_8_8041ffd8[];
    extern char str_M_O_2R_802c1b6c[], str_M_O_1_8041ffe0[];
    extern char str_M_I_2R_802c1b74[], str_M_I_2_8041ffe8[];
    extern char str_M_U_2R_802c1b7c[], str_M_U_2_8041fff0[];
    extern char str_M_C_1R_802c1b84[], str_M_C_1_8041fff8[];

#define POSE_MAP(_rear, _front) if (strcmp(name, (_rear)) == 0) return (_front)
    if (*(s8*)((s32)mp + 0x3C) == 1 &&
        (*(u32*)((s32)mp + 0x10) & 0x10) == 0) {
        POSE_MAP(str_P_Z_1R_802c19a0, str_P_Z_1_8041ff10);
        POSE_MAP(str_P_S_1R_802c19a8, str_P_S_1_8041ff18);
        POSE_MAP(str_P_W_1R_802c19b0, str_P_W_1_8041ff20);
        POSE_MAP(str_P_R_1R_802c19b8, str_P_R_1_8041ff28);
        POSE_MAP(str_P_T_1R_802c19c0, str_P_T_1_8041ff30);
        POSE_MAP(str_P_Z_2R_802c19c8, str_P_Z_2_8041ff38);
        POSE_MAP(str_P_S_2R_802c19d0, str_P_S_2_8041ff40);
        POSE_MAP(str_P_W_2R_802c19d8, str_P_W_2_8041ff48);
        POSE_MAP(str_P_R_2R_802c19e0, str_P_R_2_8041ff50);
        POSE_MAP(str_P_T_2R_802c19e8, str_P_T_2_8041ff58);
        POSE_MAP(str_P_Z_3R_802c19f0, str_P_Z_3_8041ff60);
        POSE_MAP(str_P_S_3R_802c19f8, str_P_S_3_8041ff68);
        POSE_MAP(str_P_W_3R_802c1a00, str_P_W_3_8041ff70);
        POSE_MAP(str_P_R_3R_802c1a08, str_P_R_3_8041ff78);
        POSE_MAP(str_P_T_3R_802c1a10, str_P_T_3_8041ff80);
        POSE_MAP(str_P_O_Z_1R_802c1a18, str_P_O_Z_1_802c1a24);
        POSE_MAP(str_P_O_S_1R_802c1a2c, str_P_O_S_1_802c1a38);
        POSE_MAP(str_P_O_F_1R_802c1a40, str_P_O_F_1_802c1a4c);
        POSE_MAP(str_P_O_R_1R_802c1a54, str_P_O_R_1_802c1a60);
        POSE_MAP(str_P_O_N_1R_802c1a68, str_P_O_N_1_802c1a74);
        POSE_MAP(str_P_O_S_2R_802c1a7c, str_P_O_S_2_802c1a88);
        POSE_MAP(str_P_O_F_2R_802c1a90, str_P_O_F_2_802c1a9c);
        POSE_MAP(str_P_O_R_2R_802c1aa4, str_P_O_R_2_802c1ab0);
        POSE_MAP(str_P_O_N_2R_802c1ab8, str_P_O_N_2_802c1ac4);
        POSE_MAP(str_P_O_D_1R_802c1acc, str_P_O_D_1_802c1ad8);
        POSE_MAP(str_P_O_D_2R_802c1ae0, str_P_O_D_2_802c1aec);
        POSE_MAP(str_P_A_1R_802c1af4, str_P_A_1_8041ff88);
    } else {
        POSE_MAP(str_M_Z_1R_802c1afc, str_M_Z_1_8041ff90);
        POSE_MAP(str_M_S_1R_802c1b04, str_M_S_1_8041ff98);
        POSE_MAP(str_M_W_1R_802c1b0c, str_M_W_1_8041ffa0);
        POSE_MAP(str_M_R_1R_802c1b14, str_M_R_1_8041ffa8);
        POSE_MAP(str_M_J_1BR_802c1b1c, str_M_J_1B_802c1b24);
        POSE_MAP(str_M_J_1CR_802c1b2c, str_M_J_1C_802c1b34);
        POSE_MAP(str_M_H_1R_802c1b3c, str_M_H_1_8041ffb0);
        POSE_MAP(str_M_H_2R_802c1b44, str_M_H_2_8041ffb8);
        POSE_MAP(str_M_H_4R_802c1b4c, str_M_H_4_8041ffc0);
        POSE_MAP(str_M_H_5R_802c1b54, str_M_H_5_8041ffc8);
        POSE_MAP(str_M_H_7R_802c1b5c, str_M_H_7_8041ffd0);
        POSE_MAP(str_M_H_8R_802c1b64, str_M_H_8_8041ffd8);
        POSE_MAP(str_M_O_2R_802c1b6c, str_M_O_1_8041ffe0);
        POSE_MAP(str_M_I_2R_802c1b74, str_M_I_2_8041ffe8);
        POSE_MAP(str_M_U_2R_802c1b7c, str_M_U_2_8041fff0);
        POSE_MAP(str_M_C_1R_802c1b84, str_M_C_1_8041fff8);
    }
#undef POSE_MAP
    return 0;
}

char* toRearPose(char* name) {
    extern void* mp;
    extern s32 strcmp(const char*, const char*);
    extern char str_P_Z_1R_802c19a0[], str_P_Z_1_8041ff10[];
    extern char str_P_S_1R_802c19a8[], str_P_S_1_8041ff18[];
    extern char str_P_W_1R_802c19b0[], str_P_W_1_8041ff20[];
    extern char str_P_R_1R_802c19b8[], str_P_R_1_8041ff28[];
    extern char str_P_T_1R_802c19c0[], str_P_T_1_8041ff30[];
    extern char str_P_Z_2R_802c19c8[], str_P_Z_2_8041ff38[];
    extern char str_P_S_2R_802c19d0[], str_P_S_2_8041ff40[];
    extern char str_P_W_2R_802c19d8[], str_P_W_2_8041ff48[];
    extern char str_P_R_2R_802c19e0[], str_P_R_2_8041ff50[];
    extern char str_P_T_2R_802c19e8[], str_P_T_2_8041ff58[];
    extern char str_P_Z_3R_802c19f0[], str_P_Z_3_8041ff60[];
    extern char str_P_S_3R_802c19f8[], str_P_S_3_8041ff68[];
    extern char str_P_W_3R_802c1a00[], str_P_W_3_8041ff70[];
    extern char str_P_R_3R_802c1a08[], str_P_R_3_8041ff78[];
    extern char str_P_T_3R_802c1a10[], str_P_T_3_8041ff80[];
    extern char str_P_O_Z_1R_802c1a18[], str_P_O_Z_1_802c1a24[];
    extern char str_P_O_S_1R_802c1a2c[], str_P_O_S_1_802c1a38[];
    extern char str_P_O_F_1R_802c1a40[], str_P_O_F_1_802c1a4c[];
    extern char str_P_O_R_1R_802c1a54[], str_P_O_R_1_802c1a60[];
    extern char str_P_O_N_1R_802c1a68[], str_P_O_N_1_802c1a74[];
    extern char str_P_O_S_2R_802c1a7c[], str_P_O_S_2_802c1a88[];
    extern char str_P_O_F_2R_802c1a90[], str_P_O_F_2_802c1a9c[];
    extern char str_P_O_R_2R_802c1aa4[], str_P_O_R_2_802c1ab0[];
    extern char str_P_O_N_2R_802c1ab8[], str_P_O_N_2_802c1ac4[];
    extern char str_P_O_D_1R_802c1acc[], str_P_O_D_1_802c1ad8[];
    extern char str_P_O_D_2R_802c1ae0[], str_P_O_D_2_802c1aec[];
    extern char str_P_A_1R_802c1af4[], str_P_A_1_8041ff88[];
    extern char str_M_Z_1R_802c1afc[], str_M_Z_1_8041ff90[];
    extern char str_M_S_1R_802c1b04[], str_M_S_1_8041ff98[];
    extern char str_M_W_1R_802c1b0c[], str_M_W_1_8041ffa0[];
    extern char str_M_R_1R_802c1b14[], str_M_R_1_8041ffa8[];
    extern char str_M_J_1BR_802c1b1c[], str_M_J_1B_802c1b24[];
    extern char str_M_J_1CR_802c1b2c[], str_M_J_1C_802c1b34[];
    extern char str_M_H_1R_802c1b3c[], str_M_H_1_8041ffb0[];
    extern char str_M_H_2R_802c1b44[], str_M_H_2_8041ffb8[];
    extern char str_M_H_4R_802c1b4c[], str_M_H_4_8041ffc0[];
    extern char str_M_H_5R_802c1b54[], str_M_H_5_8041ffc8[];
    extern char str_M_H_7R_802c1b5c[], str_M_H_7_8041ffd0[];
    extern char str_M_H_8R_802c1b64[], str_M_H_8_8041ffd8[];
    extern char str_M_O_2R_802c1b6c[], str_M_O_1_8041ffe0[];
    extern char str_M_I_2R_802c1b74[], str_M_I_2_8041ffe8[];
    extern char str_M_U_2R_802c1b7c[], str_M_U_2_8041fff0[];
    extern char str_M_C_1R_802c1b84[], str_M_C_1_8041fff8[];

#define POSE_MAP(_front, _rear) if (strcmp(name, (_front)) == 0) return (_rear)
    if (*(s8*)((s32)mp + 0x3C) == 1 &&
        (*(u32*)((s32)mp + 0x10) & 0x10) == 0) {
        POSE_MAP(str_P_Z_1_8041ff10, str_P_Z_1R_802c19a0);
        POSE_MAP(str_P_S_1_8041ff18, str_P_S_1R_802c19a8);
        POSE_MAP(str_P_W_1_8041ff20, str_P_W_1R_802c19b0);
        POSE_MAP(str_P_R_1_8041ff28, str_P_R_1R_802c19b8);
        POSE_MAP(str_P_T_1_8041ff30, str_P_T_1R_802c19c0);
        POSE_MAP(str_P_Z_2_8041ff38, str_P_Z_2R_802c19c8);
        POSE_MAP(str_P_S_2_8041ff40, str_P_S_2R_802c19d0);
        POSE_MAP(str_P_W_2_8041ff48, str_P_W_2R_802c19d8);
        POSE_MAP(str_P_R_2_8041ff50, str_P_R_2R_802c19e0);
        POSE_MAP(str_P_T_2_8041ff58, str_P_T_2R_802c19e8);
        POSE_MAP(str_P_Z_3_8041ff60, str_P_Z_3R_802c19f0);
        POSE_MAP(str_P_S_3_8041ff68, str_P_S_3R_802c19f8);
        POSE_MAP(str_P_W_3_8041ff70, str_P_W_3R_802c1a00);
        POSE_MAP(str_P_R_3_8041ff78, str_P_R_3R_802c1a08);
        POSE_MAP(str_P_T_3_8041ff80, str_P_T_3R_802c1a10);
        POSE_MAP(str_P_O_Z_1_802c1a24, str_P_O_Z_1R_802c1a18);
        POSE_MAP(str_P_O_S_1_802c1a38, str_P_O_S_1R_802c1a2c);
        POSE_MAP(str_P_O_F_1_802c1a4c, str_P_O_F_1R_802c1a40);
        POSE_MAP(str_P_O_R_1_802c1a60, str_P_O_R_1R_802c1a54);
        POSE_MAP(str_P_O_N_1_802c1a74, str_P_O_N_1R_802c1a68);
        POSE_MAP(str_P_O_S_2_802c1a88, str_P_O_S_2R_802c1a7c);
        POSE_MAP(str_P_O_F_2_802c1a9c, str_P_O_F_2R_802c1a90);
        POSE_MAP(str_P_O_R_2_802c1ab0, str_P_O_R_2R_802c1aa4);
        POSE_MAP(str_P_O_N_2_802c1ac4, str_P_O_N_2R_802c1ab8);
        POSE_MAP(str_P_O_D_1_802c1ad8, str_P_O_D_1R_802c1acc);
        POSE_MAP(str_P_O_D_2_802c1aec, str_P_O_D_2R_802c1ae0);
        POSE_MAP(str_P_A_1_8041ff88, str_P_A_1R_802c1af4);
    } else {
        POSE_MAP(str_M_Z_1_8041ff90, str_M_Z_1R_802c1afc);
        POSE_MAP(str_M_S_1_8041ff98, str_M_S_1R_802c1b04);
        POSE_MAP(str_M_W_1_8041ffa0, str_M_W_1R_802c1b0c);
        POSE_MAP(str_M_R_1_8041ffa8, str_M_R_1R_802c1b14);
        POSE_MAP(str_M_J_1B_802c1b24, str_M_J_1BR_802c1b1c);
        POSE_MAP(str_M_J_1C_802c1b34, str_M_J_1CR_802c1b2c);
        POSE_MAP(str_M_H_1_8041ffb0, str_M_H_1R_802c1b3c);
        POSE_MAP(str_M_H_2_8041ffb8, str_M_H_2R_802c1b44);
        POSE_MAP(str_M_H_4_8041ffc0, str_M_H_4R_802c1b4c);
        POSE_MAP(str_M_H_5_8041ffc8, str_M_H_5R_802c1b54);
        POSE_MAP(str_M_H_7_8041ffd0, str_M_H_7R_802c1b5c);
        POSE_MAP(str_M_H_8_8041ffd8, str_M_H_8R_802c1b64);
        POSE_MAP(str_M_O_1_8041ffe0, str_M_O_2R_802c1b6c);
        POSE_MAP(str_M_I_2_8041ffe8, str_M_I_2R_802c1b74);
        POSE_MAP(str_M_U_2_8041fff0, str_M_U_2R_802c1b7c);
        POSE_MAP(str_M_C_1_8041fff8, str_M_C_1R_802c1b84);
    }
#undef POSE_MAP
    return 0;
}

u8 marioInit(void) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void* __memAlloc(s32, u32);
    extern void* memset(void*, s32, u32);
    extern void* mp;
    extern void pouchInit(void);
    extern void marioResetGravity(void);
    extern void marioInitCamId(void);
    extern s32 animGroupBaseAsync(void*, s32, s32);
    extern s32 strcmp(const char*, const char*);
    extern void marioChgMot(s32);
    extern void marioBaloonInit(void);
    extern void marioGetRubInit(void);
    extern void marioResetCamShift(void);
    extern void marioResetCamShiftRate(void);
    extern void marioResetCamFollowRate(void);
    extern s32 pouchCheckItem(s32);
    extern void pouchGetItem(s32);
    extern void partyInit(void);
    extern void* marioAnimeGroupData[];
    extern void* dotMarioPose[][6];
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_J_1B_802c1b24[7];
    extern const char str_M_J_1C_802c1b34[7];
    extern s32 g_blurPosId;
    extern f32 g_blurPosX[];
    extern f32 float_1_80420008;
    extern f32 float_2_80420004;
    extern f32 float_2p25_804200b8;
    extern f32 float_20_80420088;
    extern f32 float_37_804200c0;
    extern f32 vec3_802c1958;
    extern f32 DAT_802c195c;
    extern f32 DAT_802c1960;
    extern f32 vec3_802c1964;
    extern f32 DAT_802c1968;
    extern f32 DAT_802c196c;
    void* p;
    char* dotPose;
    s32 i;

    mp = __memAlloc(0, 0x2F8);
    memset(mp, 0, 0x2F8);
    p = mp;
    *(s32*)((s32)p + 0x27C) = -1;
    *(s32*)((s32)p + 0x280) = -1;
    *(s32*)((s32)p + 0x284) = -1;
    *(s32*)((s32)p + 0x288) = -1;
    *(s32*)((s32)p + 0x28C) = -1;
    pouchInit();
    marioResetGravity();
    marioInitCamId();
    *(s8*)((s32)p + 0x3D) = 0;
    animGroupBaseAsync(marioAnimeGroupData[*(s8*)((s32)p + 0x3D) * 3], 2, 0);
    animGroupBaseAsync(marioAnimeGroupData[*(s8*)((s32)p + 0x3D) * 3 + 1], 2, 0);
    animGroupBaseAsync(marioAnimeGroupData[*(s8*)((s32)p + 0x3D) * 3 + 2], 2, 0);
    *(s32*)((s32)p + 0x22C) = -1;
    *(s32*)((s32)p + 0x230) = -1;
    *(s32*)((s32)p + 0x234) = -1;
    *(s32*)((s32)p + 0x240) = -1;
    *(f32*)((s32)p + 0x184) = float_1_80420008;
    *(f32*)((s32)p + 0x188) = (*(s8*)((s32)p + 0x3C) == 0) ? float_2p25_804200b8 : float_2_80420004;
    *(s8*)((s32)p + 0x245) = -1;
    *(s8*)((s32)p + 0x246) = -1;
    *(f32*)((s32)p + 0x1B8) = float_20_80420088;
    *(f32*)((s32)p + 0x1BC) = float_37_804200c0;
    *(f32*)((s32)p + 0xC8) = vec3_802c1958;
    *(f32*)((s32)p + 0xCC) = DAT_802c195c;
    *(f32*)((s32)p + 0xD0) = DAT_802c1960;
    *(f32*)((s32)p + 0x110) = *(f32*)((s32)p + 0x8C);
    *(f32*)((s32)p + 0x114) = *(f32*)((s32)p + 0x90);
    *(f32*)((s32)p + 0x118) = *(f32*)((s32)p + 0x94);
    *(f32*)((s32)p + 0x1C0) = float_20_80420088;
    if ((*(u32*)((s32)p + 4) & 2) == 0) {
        if (*(char**)((s32)p + 0x18) != 0 && strcmp(*(char**)((s32)p + 0x18), str_M_S_1_8041ff98) == 0) {
        } else if ((*(u32*)p & 0x80000000) == 0) {
            *(const char**)((s32)p + 0x18) = str_M_S_1_8041ff98;
            *(u16*)((s32)p + 0x28) = 0;
            *(u32*)((s32)p + 4) &= ~0x30000000;
            *(u32*)((s32)p + 0xC) |= 0x1000;
        } else {
            dotPose = 0;
            if (strcmp(str_M_Z_1_8041ff90, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)p + 0x3D)][0];
            } else if (strcmp(str_M_S_1_8041ff98, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)p + 0x3D)][1];
            } else if (strcmp(str_M_W_1_8041ffa0, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)p + 0x3D)][2];
            } else if (strcmp(str_M_R_1_8041ffa8, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)p + 0x3D)][3];
            } else if (strcmp(str_M_J_1B_802c1b24, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)p + 0x3D)][5];
            } else if (strcmp(str_M_J_1C_802c1b34, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)p + 0x3D)][5];
            }
            if (dotPose != 0) {
                *(char**)((s32)p + 0x18) = dotPose;
                *(u16*)((s32)p + 0x28) = 0;
                *(u32*)((s32)p + 4) &= ~0x30000000;
                *(u32*)((s32)p + 0xC) |= 0x1000;
            }
        }
    }
    *(u32*)((s32)p + 0x20) = 0;
    *(f32*)((s32)p + 0xD4) = vec3_802c1964;
    *(f32*)((s32)p + 0xD8) = DAT_802c1968;
    *(f32*)((s32)p + 0xDC) = DAT_802c196c;
    marioChgMot(1);
    marioBaloonInit();
    marioGetRubInit();
    g_blurPosId = 0;
    for (i = 0; i < 24; i += 3) {
        g_blurPosX[i] = *(f32*)((s32)p + 0x8C);
        g_blurPosX[i + 1] = *(f32*)((s32)p + 0x90);
        g_blurPosX[i + 2] = *(f32*)((s32)p + 0x94);
    }
    marioResetCamShift();
    marioResetCamShiftRate();
    marioResetCamFollowRate();
    if (pouchCheckItem(0x15) == 0) {
        pouchGetItem(0x15);
    }
    partyInit();
    return 0;
}

u8 marioPreDisp(void) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void* mp;
    extern void marioRearAnime(void);
    extern void marioMakeDispDir(void);
    extern void animPoseSetAnim(s32, char*, s32);
    extern void animPoseSetPaperAnim(s32, char*);
    extern void animPoseSetLocalTime(double, s32);
    extern void animPoseMain(s32);
    extern void* camGetPtr(s32);
    extern void PSMTX44MultVec(void*, VecLocal*, VecLocal*);
    extern s32 kpaNoHitEnemyCheck(void);
    extern s32 marioGetCamId(void);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern s32 shadowEntryMode(double, double, double, double);
    extern double sin(double);
    extern double cos(double);
    extern void marioDisp(void);
    extern void marioDispBlur(void);
    extern f32 float_0_80420020;
    extern f32 float_1_80420008;
    extern f32 float_neg1_8042000c;
    extern f32 float_180_8042001c;
    extern f32 float_3p1416_80420030;
    extern f32 float_5000_8042003c;
    extern f32 float_10000_80420040;
    extern f32 float_13p333_80420044;
    extern f32 float_30_80420048;
    extern f32 float_14_8042004c;
    extern f32 float_6_80420050;
    extern f32 float_0p6_80420054;
    extern f32 float_neg2_80420058;
    extern f32 float_0p1_8042005c;
    void* m = mp;
    VecLocal pos;
    void* cam;
    s32 poseIndex;
    s32 blink;
    f32 order;
    f32 shadowX;
    f32 shadowZ;
    f32 shadowSize;
    f32 groundDelta;

    if (*(s8*)((s32)m + 0x3C) == 0 || *(s8*)((s32)m + 0x3C) == 1) {
        marioRearAnime();
    }
    if (*(s8*)((s32)m + 0x38) == 0 && ((*(u32*)((s32)m + 4) & 0x01000000) == 0)) {
        marioMakeDispDir();
    }
    if ((*(u32*)((s32)m + 0xC) & 0x3000) != 0) {
        *(u32*)((s32)m + 0xC) &= ~0x3000;
        poseIndex = (*(u32*)((s32)m + 4) & 0x10000000) != 0 ? 2 : ((*(u32*)((s32)m + 4) & 0x80000000) != 0 ? 1 : 0);
        animPoseSetAnim(*(s32*)((s32)m + 0x22C + poseIndex * 4), *(char**)((s32)m + 0x18), 1);
        if (*(char**)((s32)m + 0x1C) != 0) {
            animPoseSetPaperAnim(*(s32*)((s32)m + 0x22C), *(char**)((s32)m + 0x1C));
        }
        poseIndex = (*(u32*)((s32)m + 4) & 0x10000000) != 0 ? 2 : ((*(u32*)((s32)m + 4) & 0x80000000) != 0 ? 1 : 0);
        animPoseSetLocalTime((double)(f32)*(u16*)((s32)m + 0x28), *(s32*)((s32)m + 0x22C + poseIndex * 4));
    }
    poseIndex = (*(u32*)((s32)m + 4) & 0x10000000) != 0 ? 2 : ((*(u32*)((s32)m + 4) & 0x80000000) != 0 ? 1 : 0);
    animPoseMain(*(s32*)((s32)m + 0x22C + poseIndex * 4));
    if ((*(u32*)((s32)m + 4) & 0x20) == 0) {
        pos.x = *(f32*)((s32)m + 0x8C);
        pos.y = *(f32*)((s32)m + 0x90);
        pos.z = *(f32*)((s32)m + 0x94);
        cam = camGetPtr(4);
        PSMTX44MultVec((void*)((s32)cam + 0xA0), &pos, &pos);
        order = float_5000_8042003c * pos.z + float_5000_8042003c;
        if (order < float_0_80420020) {
            order = float_0_80420020;
        } else if (order > float_10000_80420040) {
            order = float_10000_80420040;
        }
        order *= float_neg1_8042000c;
        if (((*(u32*)((s32)m + 4) & 0x08000000) == 0) || ((*(u32*)m & 0x80000000) != 0)) {
            blink = (*(s8*)((s32)m + 0x3C) == 2) ? kpaNoHitEnemyCheck() : *(u16*)((s32)m + 0x4C);
            if (blink < 1 || ((u32)blink & 2) == 0) {
                dispEntry(marioGetCamId(), 1, marioDisp, m, order);
            }
        } else {
            dispEntry(marioGetCamId(), 2, marioDispBlur, m, order);
        }
        if (((*(u32*)((s32)m + 4) & 0x4000000) == 0) && ((*(u32*)m & 0x02000000) == 0)) {
            if (*(u16*)((s32)m + 0x2E) == 0x16) {
                shadowEntryMode((double)*(f32*)((s32)m + 0x8C), (double)*(f32*)((s32)m + 0xF8),
                                (double)*(f32*)((s32)m + 0x94), (double)float_13p333_80420044);
            } else {
                shadowSize = float_13p333_80420044;
                shadowX = float_0_80420020;
                shadowZ = float_0_80420020;
                if (*(s8*)((s32)m + 0x3C) == 2) {
                    shadowSize = float_30_80420048;
                    shadowX = float_14_8042004c * (f32)sin((double)((float_3p1416_80420030 * *(f32*)((s32)m + 0x19C)) / float_180_8042001c));
                    shadowZ = float_14_8042004c * -(f32)cos((double)((float_3p1416_80420030 * *(f32*)((s32)m + 0x19C)) / float_180_8042001c));
                } else if (*(s8*)((s32)m + 0x3C) == 1) {
                    shadowX = float_6_80420050 * (f32)sin((double)((float_3p1416_80420030 * *(f32*)((s32)m + 0x19C)) / float_180_8042001c));
                    shadowZ = float_6_80420050 * -(f32)cos((double)((float_3p1416_80420030 * *(f32*)((s32)m + 0x19C)) / float_180_8042001c));
                }
                if (*(void**)((s32)m + 0x1F8) == 0) {
                    groundDelta = *(f32*)((s32)m + 0x90) - *(f32*)((s32)m + 0xF8);
                    if (groundDelta >= float_neg2_80420058) {
                        shadowSize -= groundDelta / float_13p333_80420044;
                        if (shadowSize < float_0p1_8042005c) {
                            shadowSize = float_0_80420020;
                        }
                        if (shadowSize != float_0_80420020) {
                            shadowEntryMode((double)(*(f32*)((s32)m + 0x8C) + shadowX), (double)*(f32*)((s32)m + 0x90),
                                            (double)(*(f32*)((s32)m + 0x94) + shadowZ), (double)shadowSize);
                        }
                    }
                } else {
                    if ((*(u32*)m & 0x02000000) != 0) {
                        shadowSize *= float_0p6_80420054;
                    }
                    shadowEntryMode((double)(*(f32*)((s32)m + 0x8C) + shadowX), (double)*(f32*)((s32)m + 0x90),
                                    (double)(*(f32*)((s32)m + 0x94) + shadowZ), (double)shadowSize);
                }
            }
        }
    }
    return 0;
}

s32 marioCheckMenuDisable(void) {
    extern void* mp;
    extern void* gp;
    extern s32 strncmp(const char*, const char*, u32);
    extern char str_yuu_804200c8[];
    extern s32 marioChkKey(void);
    extern s32 kpaGetStageViewType(void);
    extern s32 marioGetPlaneStatus(void);
    extern s32 marioShipChanging(void);
    extern s32 mario_bomhei_keychk(void);
    extern s32 vivianGetStatus(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern s32 seqGetSeq(void);
    extern s32 yoshiGetStatus(void);

    void* mario = mp;
    void* party;
    s32 status;
    u16 motion;

    if (*(s8*)((s32)mario + 0x38) != 0) {
        return 1;
    }
    if (strncmp((char*)((s32)gp + 0x12C), str_yuu_804200c8, 4) == 0) {
        return marioChkKey() != 0 ? 0 : 1;
    }
    if (*(s8*)((s32)mario + 0x3C) == 1) {
        return 1;
    }
    if (*(s8*)((s32)mario + 0x3C) == 2 && kpaGetStageViewType() != 0) {
        return 1;
    }

    motion = *(u16*)((s32)mario + 0x2E);
    if (motion == 0x12 || motion == 0x13) {
        return 1;
    }
    if (motion == 0x14) {
        return 1;
    }
    if (marioGetPlaneStatus() == 1) {
        return 1;
    }
    status = marioShipChanging();
    if (status == 1 || status == 2) {
        return 1;
    }
    if (motion == 0x1F || motion == 0x20) {
        return 1;
    }
    if (*(s8*)((s32)mario + 0x3C) != 2 && marioGetPlaneStatus() == 0 &&
        *(void**)((s32)mario + 0x1F8) == 0) {
        return 1;
    }
    if (mario_bomhei_keychk() == 0) {
        return 1;
    }

    status = vivianGetStatus();
    if (status != 0) {
        party = partyGetPtr(marioGetPartyId());
        if ((*(u32*)party & 0x200) != 0) {
            return 1;
        }
        if (vivianGetStatus() == 2) {
            return 0;
        }
    }

    partyGetPtr(marioGetPartyId());
    if (seqGetSeq() == 2 && *(s8*)((s32)mario + 0x39) == 0 && yoshiGetStatus() == 1) {
        return 0;
    }
    return marioChkKey() != 0 ? 0 : 1;
}

void marioReInit(void) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void* mp;
    extern s32 marioGetCamId(void);
    extern void animPoseSetPaperAnimGroup(s32, char*, s32);
    extern void animPoseRelease(s32);
    extern void* memset(void*, s32, u32);
    extern void animPoseSetMaterialFlagOff(s32, u32);
    extern void animPoseSetMaterialLightFlagOn(s32, u32);
    extern void partyReInit(void);
    extern void marioBaloonInit(void);
    extern void marioGetRubInit(void);
    extern void marioSetCharMode(u32);
    extern void marioResetCamShift(void);
    extern void marioResetCamShiftRate(void);
    extern void marioResetCamFollowRate(void);
    extern s32 pouchCheckItem(s32);
    extern void pouchGetItem(s32);
    extern s32 g_blurPosId;
    extern f32 g_blurPosX[];
    extern f32 float_1_80420008;
    extern f32 float_2_80420004;
    extern f32 float_2p25_804200b8;
    extern f32 float_10_80420078;
    extern f32 float_20_80420088;
    extern f32 float_25p9_804200c4;
    extern f32 float_37_804200c0;
    extern f32 float_47_804200bc;
    extern f32 vec3_802c1970;
    extern f32 DAT_802c1974;
    extern f32 DAT_802c1978;
    extern f32 vec3_802c197c;
    extern f32 DAT_802c1980;
    extern f32 DAT_802c1984;
    extern f32 vec3_802c1988;
    extern f32 DAT_802c198c;
    extern f32 DAT_802c1990;
    void* p = mp;
    u32 oldFlags = *(u32*)p;
    s32 camId = marioGetCamId();
    s32 paperPoseId = *(s32*)((s32)p + 0x240);
    s8 characterId = *(s8*)((s32)p + 0x3C);
    s8 party0 = *(s8*)((s32)p + 0x245);
    s8 party1 = *(s8*)((s32)p + 0x246);
    u32 peachSts = *(u32*)((s32)p + 0x10);
    void* kpaPtr = *(void**)((s32)p + 0x238);
    u32 trigFlags = *(u32*)((s32)p + 0xC);
    u16 motion = *(u16*)((s32)p + 0x2E);
    f32 dispCur = *(f32*)((s32)p + 0x1AC);
    f32 dispTarget = *(f32*)((s32)p + 0x1B0);
    f32 viewDir = *(f32*)((s32)p + 0x1A4);
    f32 worldDir = *(f32*)((s32)p + 0x1A0);
    s32 prev0 = *(s32*)((s32)p + 0x248);
    s32 prev1 = *(s32*)((s32)p + 0x24C);
    s32 pose0 = *(s32*)((s32)p + 0x22C);
    s32 pose1 = *(s32*)((s32)p + 0x230);
    s32 pose2 = *(s32*)((s32)p + 0x234);
    s32 i;

    if (paperPoseId > -1) {
        animPoseSetPaperAnimGroup(pose0, 0, 0);
        animPoseRelease(paperPoseId);
        paperPoseId = -1;
        *(s32*)((s32)p + 0x240) = -1;
    }
    memset(p, 0, 0x2F8);
    *(s32*)((s32)p + 0x27C) = -1;
    *(s32*)((s32)p + 0x280) = -1;
    *(s32*)((s32)p + 0x284) = -1;
    *(s32*)((s32)p + 0x288) = -1;
    *(s32*)((s32)p + 0x28C) = -1;
    if ((oldFlags & 0x80000000) != 0) {
        *(u32*)p |= 0x80000000;
    }
    *(u32*)((s32)p + 0x10) = peachSts;
    *(void**)((s32)p + 0x238) = kpaPtr;
    *(s32*)((s32)p + 0x134) = camId;
    *(s8*)((s32)p + 0x3C) = characterId;
    *(u32*)((s32)p + 0xC) = trigFlags;
    *(u16*)((s32)p + 0x2E) = motion;
    *(f32*)((s32)p + 0x1AC) = dispCur;
    *(f32*)((s32)p + 0x1B0) = dispTarget;
    *(f32*)((s32)p + 0x1A4) = viewDir;
    *(f32*)((s32)p + 0x1A0) = worldDir;
    *(s32*)((s32)p + 0x248) = prev0;
    *(s32*)((s32)p + 0x24C) = prev1;
    *(s8*)((s32)p + 0x245) = party0;
    *(s8*)((s32)p + 0x246) = party1;
    *(s32*)((s32)p + 0x22C) = pose0;
    *(s32*)((s32)p + 0x230) = pose1;
    *(s32*)((s32)p + 0x234) = pose2;
    *(s32*)((s32)p + 0x240) = paperPoseId;

    *(f32*)((s32)p + 0x184) = float_1_80420008;
    *(f32*)((s32)p + 0x188) = characterId == 0 ? float_2p25_804200b8 : float_2_80420004;
    if (characterId == 1) {
        *(f32*)((s32)p + 0x1B8) = float_20_80420088;
        *(f32*)((s32)p + 0x1BC) = float_47_804200bc;
        *(f32*)((s32)p + 0x184) = float_1_80420008;
        *(f32*)((s32)p + 0x188) = float_2_80420004;
    } else if (characterId == 2) {
        *(f32*)((s32)p + 0x184) = float_1_80420008;
        *(f32*)((s32)p + 0x188) = float_2p25_804200b8;
    } else if ((*(u32*)p & 0x02000000) == 0) {
        *(f32*)((s32)p + 0x1B8) = float_20_80420088;
        *(f32*)((s32)p + 0x1BC) = float_37_804200c0;
    } else {
        *(f32*)((s32)p + 0x1B8) = float_10_80420078;
        *(f32*)((s32)p + 0x1BC) = float_25p9_804200c4;
    }
    *(u32*)p &= ~1;
    if (pose0 > -1) {
        animPoseSetMaterialFlagOff(pose0, 0xFFFFFFFF);
        animPoseSetMaterialLightFlagOn(pose0, 2);
    }
    if (pose1 > -1) {
        animPoseSetMaterialFlagOff(pose1, 0xFFFFFFFF);
        animPoseSetMaterialLightFlagOn(pose1, 2);
    }
    if (pose2 > -1) {
        animPoseSetMaterialFlagOff(pose2, 0xFFFFFFFF);
        animPoseSetMaterialLightFlagOn(pose2, 2);
    }
    *(f32*)((s32)p + 0xC8) = vec3_802c1970;
    *(f32*)((s32)p + 0xCC) = DAT_802c1974;
    *(f32*)((s32)p + 0xD0) = DAT_802c1978;
    *(f32*)((s32)p + 0xD4) = vec3_802c197c;
    *(f32*)((s32)p + 0xD8) = DAT_802c1980;
    *(f32*)((s32)p + 0xDC) = DAT_802c1984;
    partyReInit();
    marioBaloonInit();
    marioGetRubInit();
    g_blurPosId = 0;
    for (i = 0; i < 24; i += 3) {
        g_blurPosX[i] = *(f32*)((s32)p + 0x8C);
        g_blurPosX[i + 1] = *(f32*)((s32)p + 0x90);
        g_blurPosX[i + 2] = *(f32*)((s32)p + 0x94);
    }
    if (*(s8*)((s32)p + 0x3C) == 0) {
        marioSetCharMode(0);
    }
    if (*(s8*)((s32)p + 0x3C) == 2) {
        marioSetCharMode(2);
    }
    marioResetCamShift();
    marioResetCamShiftRate();
    marioResetCamFollowRate();
    if (pouchCheckItem(0x15) == 0) {
        pouchGetItem(0x15);
    }
    *(f32*)((s32)p + 0xE0) = vec3_802c1988;
    *(f32*)((s32)p + 0xE4) = DAT_802c198c;
    *(f32*)((s32)p + 0xE8) = DAT_802c1990;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioEntry(void) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_J_1B_802c1b24[7];
    extern const char str_M_J_1C_802c1b34[7];
    extern void* dotMarioPose[][6];
    extern void marioSlitForceCancel(void);
    extern void marioChgMot(s32 motion);
    extern void marioReInit_ship(void);
    extern void marioReInit_roll(void);
    extern void marioReInit_slit(void);
    extern void marioRideYoshi(void);
    void* player;
    void* m;
    char* dotPose;
    u16 motion;

    player = mp;
    *(u32*)player |= 1;
    m = mp;
    if ((*(u32*)((s32)m + 4) & 2) == 0) {
        if (*(char**)((s32)m + 0x18) != NULL && strcmp(*(char**)((s32)m + 0x18), str_M_S_1_8041ff98) == 0) {
        } else if ((*(u32*)m & 0x80000000) == 0) {
            *(const char**)((s32)m + 0x18) = str_M_S_1_8041ff98;
            *(s16*)((s32)m + 0x28) = 0;
            *(u32*)((s32)m + 4) &= ~0x30000000;
            *(u32*)((s32)m + 0xC) |= 0x1000;
        } else {
            dotPose = NULL;
            if (strcmp(str_M_Z_1_8041ff90, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
            } else if (strcmp(str_M_S_1_8041ff98, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
            } else if (strcmp(str_M_W_1_8041ffa0, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
            } else if (strcmp(str_M_R_1_8041ffa8, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
            } else if (strcmp(str_M_J_1B_802c1b24, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            } else if (strcmp(str_M_J_1C_802c1b34, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            }
            if (dotPose != NULL) {
                *(char**)((s32)m + 0x18) = dotPose;
                *(s16*)((s32)m + 0x28) = 0;
                *(u32*)((s32)m + 4) &= ~0x30000000;
                *(u32*)((s32)m + 0xC) |= 0x1000;
            }
        }
    }

    if ((*(u32*)player & 0x02000000) == 0) {
        motion = *(u16*)((s32)player + 0x2E);
        if (motion == 0x19) {
            marioReInit_ship();
        } else if (motion == 0x16) {
            marioReInit_roll();
        } else if (motion == 0x14) {
            marioReInit_slit();
        } else if (motion == 0x1A) {
            marioRideYoshi();
        } else {
            marioChgMot(1);
        }
    } else {
        if (*(u16*)((s32)player + 0x2E) == 0x14) {
            *(u32*)player |= 0x40000000;
            marioSlitForceCancel();
        }
        marioChgMot(1);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioReset(void) {
    typedef struct WordVec {
        u32 x;
        u32 y;
        u32 z;
    } WordVec;
    extern void* mp;
    extern void animPoseSetPaperAnimGroup(s32 poseId, s32 a, s32 b);
    extern void animPaperPoseRelease(s32 poseId);
    extern s32 strcmp(const char* a, const char* b);
    extern void marioChgMot(s32 motion);
    extern u8 str_a_mario_802c18a0[];
    extern const f32 float_20_80420088;
    extern const f32 float_47_804200bc;
    extern const f32 float_1_80420008;
    extern const f32 float_2_80420004;
    extern const f32 float_2p25_804200b8;
    extern const f32 float_37_804200c0;
    extern const f32 float_10_80420078;
    extern const f32 float_25p9_804200c4;
    extern const f32 float_0_80420020;
    void* player;
    void* p;
    void* base;
    s32 spec;

    player = mp;
    spec = *(s8*)((s32)mp + 0x3C);
    if (spec == 1) {
        *(f32*)((s32)mp + 0x1B8) = float_20_80420088;
        *(f32*)((s32)player + 0x1BC) = float_47_804200bc;
        *(f32*)((s32)player + 0x184) = float_1_80420008;
        *(f32*)((s32)player + 0x188) = float_2_80420004;
    } else if (spec == 2) {
        *(f32*)((s32)mp + 0x184) = float_1_80420008;
        *(f32*)((s32)player + 0x188) = float_2p25_804200b8;
    } else {
        if ((*(u32*)mp & 0x02000000) == 0) {
            *(f32*)((s32)mp + 0x1B8) = float_20_80420088;
            *(f32*)((s32)player + 0x1BC) = float_37_804200c0;
        } else {
            *(f32*)((s32)mp + 0x1B8) = float_10_80420078;
            *(f32*)((s32)player + 0x1BC) = float_25p9_804200c4;
        }
        *(f32*)((s32)player + 0x184) = float_1_80420008;
        *(f32*)((s32)player + 0x188) = float_2p25_804200b8;
    }

    *(u32*)player &= ~0x60000000;
    p = mp;
    if ((*(u32*)((s32)mp + 4) & 0x40000000) != 0 && *(void**)((s32)mp + 0x1C) != NULL) {
        animPoseSetPaperAnimGroup(*(s32*)((s32)mp + 0x22C), 0, 0);
        *(s32*)((s32)p + 0x1C) = 0;
        *(u32*)((s32)p + 4) &= ~0x40000000;
        p = mp;
        if (*(s32*)((s32)mp + 0x240) >= 0) {
            animPaperPoseRelease(*(s32*)((s32)mp + 0x240));
            *(s32*)((s32)p + 0x240) = -1;
        }
    }

    p = mp;
    if ((*(u32*)((s32)mp + 4) & 0x40000000) != 0) {
        if ((*(u32*)((s32)mp + 4) & 2) == 0 && (*(u32*)mp & 0x80000000) == 0 &&
            (*(char**)((s32)mp + 0x1C) == NULL || strcmp(*(char**)((s32)mp + 0x1C), NULL) != 0)) {
            *(s32*)((s32)p + 0x1C) = 0;
            *(s16*)((s32)p + 0x28) = 0;
            *(u32*)((s32)p + 0xC) |= 0x4000;
        }
    }

    *(u32*)((s32)player + 4) &= ~0x08000000;
    *(u32*)((s32)player + 4) &= ~0x00000040;
    p = mp;
    base = str_a_mario_802c18a0;
    *(WordVec*)((s32)p + 0xBC) = *(WordVec*)((s32)base + 0x88);
    *(WordVec*)((s32)p + 0xB0) = *(WordVec*)((s32)base + 0x94);
    *(WordVec*)((s32)p + 0x98) = *(WordVec*)((s32)base + 0xA0);
    *(WordVec*)((s32)p + 0xA4) = *(WordVec*)((s32)base + 0xAC);
    *(f32*)((s32)player + 0x180) = float_0_80420020;
    marioChgMot(1);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgPose(char* pose) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_J_1B_802c1b24[7];
    extern const char str_M_J_1C_802c1b34[7];
    extern void* dotMarioPose[][6];
    void* m;
    char* dotPose;

    m = mp;
    if ((*(u32*)((s32)m + 4) & 2) == 0) {
        if (*(char**)((s32)m + 0x18) != NULL && strcmp(*(char**)((s32)m + 0x18), pose) == 0) {
            return;
        }
        if ((*(u32*)m & 0x80000000) == 0) {
            *(char**)((s32)m + 0x18) = pose;
            *(s16*)((s32)m + 0x28) = 0;
            *(u32*)((s32)m + 4) &= ~0x30000000;
            *(u32*)((s32)m + 0xC) |= 0x1000;
        } else {
            dotPose = NULL;
            if (strcmp(str_M_Z_1_8041ff90, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
            } else if (strcmp(str_M_S_1_8041ff98, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
            } else if (strcmp(str_M_W_1_8041ffa0, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
            } else if (strcmp(str_M_R_1_8041ffa8, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
            } else if (strcmp(str_M_J_1B_802c1b24, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            } else if (strcmp(str_M_J_1C_802c1b34, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            }
            if (dotPose != NULL) {
                *(char**)((s32)m + 0x18) = dotPose;
                *(s16*)((s32)m + 0x28) = 0;
                *(u32*)((s32)m + 4) &= ~0x30000000;
                *(u32*)((s32)m + 0xC) |= 0x1000;
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioCtrlOff2Main(void) {
    extern void* mp;
    extern s32 vivianGetStatus(void);
    extern void marioMotion(void);
    extern void marioMove(void);
    extern f32 marioChkOverhead(void);
    extern void marioChgMot(s32 motion);
    extern void marioSetFallPara(void);
    extern void marioJump(void);
    extern void marioChkGnd2(void);
    extern void marioFall(void);
    extern s32 marioChkWallAround(void* pos, s32 arg, f32 a, f32 b, f32 c, f32 d);
    extern void marioChkGnd(void);
    extern f32 float_0_80420020;
    void* mario;
    u16 motion;
    u32 flags;

    mario = mp;
    motion = *(u16*)((s32)mario + 0x2E);
    if (motion == 0x18) {
        return;
    }
    if (motion == 0x19) {
        return;
    }
    if ((u16)(motion - 0x1F) <= 2) {
        return;
    }
    if (motion == 0x14) {
        return;
    }
    if (motion == 0x1A) {
        return;
    }
    if (motion == 8) {
        return;
    }
    if (motion == 6) {
        return;
    }
    if (vivianGetStatus() != 0) {
        return;
    }

    marioMotion();
    flags = *(u32*)mario;
    if (flags & 0x40000) {
        marioMove();
        marioChkOverhead();
        if (*(s16*)((s32)mario + 0x50) > 100) {
            marioChgMot(0xA);
            marioSetFallPara();
        }
    } else if (flags & 0x10000) {
        marioJump();
        marioChkOverhead();
        marioMove();
        if (*(u16*)((s32)mp + 0x2E) != 0x16 && *(s16*)((s32)mario + 0x50) > 5) {
            marioChkGnd2();
        }
    } else if (flags & 0x20000) {
        if (*(u16*)((s32)mario + 0x2E) != 0x19) {
            marioMove();
            if (*(u16*)((s32)mario + 0x2E) == 0x10) {
                f32 y = marioChkOverhead();
                if (*(void**)((s32)mario + 0x1F8) != NULL) {
                    *(f32*)((s32)mario + 0x90) = y;
                }
            } else {
                marioChkOverhead();
            }
            marioFall();
            motion = *(u16*)((s32)mario + 0x2E);
            if (motion == 0x10 || motion == 0x11) {
                marioChkGnd2();
            } else if (*(u16*)((s32)mp + 0x2E) != 0x16 && *(s16*)((s32)mario + 0x50) > 5) {
                marioChkGnd2();
            }
            marioChkWallAround((void*)((s32)mario + 0x8C), 0, float_0_80420020, float_0_80420020, *(f32*)((s32)mario + 0x1C0), *(f32*)((s32)mario + 0x1BC));
        }
    } else {
        marioChkGnd();
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioResetHitObj(char* name) {
    extern void* mp;
    extern char* hitGetName(void* hit);
    extern s32 strcmp(const char* a, const char* b);
    void* m;
    void* hit;

    m = mp;
    hit = *(void**)((s32)m + 0x1E0);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1E0) = NULL;
    }
    hit = *(void**)((s32)m + 0x1E4);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1E4) = NULL;
    }
    hit = *(void**)((s32)m + 0x1E8);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1E8) = NULL;
    }
    hit = *(void**)((s32)m + 0x1EC);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1EC) = NULL;
    }
    hit = *(void**)((s32)m + 0x1F0);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1F0) = NULL;
    }
    hit = *(void**)((s32)m + 0x1F4);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1F4) = NULL;
    }
    hit = *(void**)((s32)m + 0x1F8);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1F8) = NULL;
    }
    hit = *(void**)((s32)m + 0x1FC);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1FC) = NULL;
    }
    hit = *(void**)((s32)m + 0x200);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x200) = NULL;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* toDotMarioPose(char* pose) {
    extern s32 strcmp(const char* a, const char* b);
    extern void* mp;
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_J_1B_802c1b24[7];
    extern const char str_M_J_1C_802c1b34[7];
    extern void* dotMarioPose[][6];
    void* result;

    result = NULL;
    if (strcmp(str_M_Z_1_8041ff90, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
    } else if (strcmp(str_M_S_1_8041ff98, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
    } else if (strcmp(str_M_W_1_8041ffa0, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
    } else if (strcmp(str_M_R_1_8041ffa8, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
    } else if (strcmp(str_M_J_1B_802c1b24, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
    } else if (strcmp(str_M_J_1C_802c1b34, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
    }
    return result;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioDispBlur(s32 param_1, void* mario) {
    extern void* mp;
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern void marioDispBlurSub(void* data, s32 index);
    extern void animPoseSetMaterialFlagOff(s32 poseId, u32 flag);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern f32 float_6p2832_80420028;
    extern f32 float_360_8042002c;
    extern f32 float_3p1416_80420030;
    extern f32 float_1p5708_80420034;
    extern f32 float_4p7124_80420038;
    u8 data[60];
    u32 flags;
    s32 index;
    u32 materialFlags;
    s32 i;
    f32 angle;

    flags = *(u32*)((s32)mp + 4);
    if (flags & 0x10000000) {
        index = 2;
    } else if (flags & 0x80000000) {
        index = 1;
    } else {
        index = 0;
    }
    materialFlags = animPoseGetMaterialFlag(*(s32*)((s32)mario + 0x22C + index * 4));

    angle = (float_6p2832_80420028 * *(f32*)((s32)mario + 0x19C)) / float_360_8042002c;
    if (angle <= float_3p1416_80420030) {
        if (angle >= float_1p5708_80420034) {
        }
    } else {
        (void)(angle == float_4p7124_80420038);
    }
    if (angle <= float_3p1416_80420030) {
        if (angle >= float_1p5708_80420034) {
        }
    } else {
        (void)(angle == float_4p7124_80420038);
    }

    for (i = 7; i > 0; i--) {
        marioDispBlurSub(data, i);
    }

    flags = *(u32*)((s32)mp + 4);
    if (flags & 0x10000000) {
        index = 2;
    } else if (flags & 0x80000000) {
        index = 1;
    } else {
        index = 0;
    }
    animPoseSetMaterialFlagOff(*(s32*)((s32)mario + 0x22C + index * 4), 0x40);

    flags = *(u32*)((s32)mp + 4);
    if (flags & 0x10000000) {
        index = 2;
    } else if (flags & 0x80000000) {
        index = 1;
    } else {
        index = 0;
    }
    animPoseSetMaterialFlagOn(*(s32*)((s32)mario + 0x22C + index * 4), materialFlags);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 marioChkKey(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern s32 seqGetSeq(void);
    extern void* mp;
    extern s32 marioShipChanging(void);
    extern s32 marioRollKeyDisable(void);
    extern s32 marioSlitKeyDisable(void);
    extern s32 christineGetStatus(void);
    extern s32 mario_bomhei_keychk2(void);
    extern s32 yoshiGetStatus(void);
    extern s32 nokonoko_holdItem(void);
    void* party;

    party = partyGetPtr(marioGetPartyId());
    if (seqGetSeq() != 2) {
        return 0;
    }
    if (*(s8*)((s32)mp + 0x39) != 0) {
        return 0;
    }
    if (marioShipChanging() == 1 || marioShipChanging() == 2) {
        return 0;
    }
    if (marioRollKeyDisable() == 0) {
        return 0;
    }
    if (marioSlitKeyDisable() == 0) {
        return 0;
    }
    if (party == 0) {
        return 1;
    }
    if (christineGetStatus() == 1) {
        return 0;
    }
    if (mario_bomhei_keychk2() == 0) {
        return 0;
    }
    if (yoshiGetStatus() != 0) {
        return 0;
    }
    if ((*(u32*)party & 0x100) != 0) {
        s32 mode = *(s8*)((s32)party + 0x31);
        if (mode == 7 || mode == 5 || mode == 6) {
            return 0;
        }
    }
    if (nokonoko_holdItem() != 0) {
        return 0;
    }
    return 1;
}

s32 marioPaperOn(char* name) {
    extern void* mp;
    extern s32 strcmp(const char*, const char*);
    extern void animPaperPoseRelease(s32);
    extern s32 animPaperPoseEntry(char*, s32);
    extern void animPoseSetPaperAnimGroup(s32, char*, s32);
    extern char str_a_mario_802c18a0[];
    void* mario;
    char* base;
    s32 special;

    base = str_a_mario_802c18a0;
    mario = mp;
    if (*(s32*)((s32)mario + 0x240) >= 0) {
        animPaperPoseRelease(*(s32*)((s32)mario + 0x240));
        *(s32*)((s32)mario + 0x240) = -1;
    }
    *(u32*)((s32)mario + 4) |= 0x40000000;
    *(s32*)((s32)mario + 0x240) = animPaperPoseEntry(name, 2);

    special = 0;
    if (strcmp(name, base + 0x300) == 0 || strcmp(name, base + 0x308) == 0 ||
        strcmp(name, base + 0x310) == 0 || strcmp(name, base + 0x318) == 0 ||
        strcmp(name, base + 0x324) == 0 || strcmp(name, base + 0x330) == 0) {
        special = 1;
    }
    animPoseSetPaperAnimGroup(*(s32*)((s32)mario + 0x22C), name, special == 0);
    return 1;
}

void marioBgmodeOff(void) {
    extern void* mp;
    extern void camFollowYOn(void);
    extern const f32 float_20_80420088;
    extern const f32 float_47_804200bc;
    extern const f32 float_1_80420008;
    extern const f32 float_2_80420004;
    extern const f32 float_2p25_804200b8;
    extern const f32 float_37_804200c0;
    extern const f32 float_10_80420078;
    extern const f32 float_25p9_804200c4;
    void* m = mp;
    s32 spec;
    *(u32*)m &= ~0x02000000;
    m = mp;
    spec = *(s8*)((s32)m + 0x3C);
    if (spec == 1) {
        *(f32*)((s32)m + 0x1B8) = float_20_80420088;
        *(f32*)((s32)m + 0x1BC) = float_47_804200bc;
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2_80420004;
    } else if (spec == 2) {
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
    } else {
        if ((*(u32*)m & 0x02000000) == 0) {
            *(f32*)((s32)m + 0x1B8) = float_20_80420088;
            *(f32*)((s32)m + 0x1BC) = float_37_804200c0;
        } else {
            *(f32*)((s32)m + 0x1B8) = float_10_80420078;
            *(f32*)((s32)m + 0x1BC) = float_25p9_804200c4;
        }
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
    }
    camFollowYOn();
}

void marioBgmodeOn(void) {
    extern void* mp;
    extern void camFollowYOff(void);
    extern const f32 float_20_80420088;
    extern const f32 float_47_804200bc;
    extern const f32 float_1_80420008;
    extern const f32 float_2_80420004;
    extern const f32 float_2p25_804200b8;
    extern const f32 float_37_804200c0;
    extern const f32 float_10_80420078;
    extern const f32 float_25p9_804200c4;
    void* m = mp;
    s32 spec;
    *(u32*)m |= 0x02000000;
    m = mp;
    spec = *(s8*)((s32)m + 0x3C);
    if (spec == 1) {
        *(f32*)((s32)m + 0x1B8) = float_20_80420088;
        *(f32*)((s32)m + 0x1BC) = float_47_804200bc;
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2_80420004;
    } else if (spec == 2) {
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
    } else {
        if ((*(u32*)m & 0x02000000) == 0) {
            *(f32*)((s32)m + 0x1B8) = float_20_80420088;
            *(f32*)((s32)m + 0x1BC) = float_37_804200c0;
        } else {
            *(f32*)((s32)m + 0x1B8) = float_10_80420078;
            *(f32*)((s32)m + 0x1BC) = float_25p9_804200c4;
        }
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
    }
    camFollowYOff();
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioSetPaperAnimeLocalTime(s32 time) {
    extern void* mp;
    extern void animPoseSetLocalTime(s32 poseId, f32 time);
    void* m = mp;
    s32 index;
    *(s16*)((s32)m + 0x28) = time;
    animPoseSetLocalTime(*(s32*)((s32)m + 0x240), (f32)time);
    m = mp;
    if ((*(u32*)((s32)m + 4) & 0x10000000) != 0) {
        index = 2;
    } else if ((*(u32*)((s32)m + 4) & 0x80000000) != 0) {
        index = 1;
    } else {
        index = 0;
    }
    animPoseSetLocalTime(*(s32*)((s32)m + 0x22C + index * 4), (f32)time);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioFBattlePost(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* mp;
    extern Vec R_last_position;
    extern void marioYoshiForceCancel(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void partyGetAppearPos(void*, Vec*);
    extern void partyCtrlOn(void);
    extern void marioChgMot(s32);
    void* mario;
    void* party;
    Vec pos;

    mario = mp;
    *(u32*)mario &= ~0x1000;
    if (*(u16*)((s32)mario + 0x2E) == 0x14) {
        *(Vec*)((s32)mario + 0x8C) = R_last_position;
    }
    if (*(u16*)((s32)mario + 0x2E) == 0x1A) {
        marioYoshiForceCancel();
    }
    party = partyGetPtr(marioGetPartyId());
    if (party != 0) {
        partyGetAppearPos(party, &pos);
        *(Vec*)((s32)party + 0x58) = pos;
    }
    partyCtrlOn();
    if (*(u16*)((s32)mario + 0x2E) == 0x12 || *(u16*)((s32)mario + 0x2E) == 0x14) {
        marioChgMot(0);
    }
}

s32 marioChkSts(u32 flags) {
    extern void* mp;
    void* m = mp;
    s32 ret = 0;

    if ((flags & 1) && (*(u32*)m & 0x10000)) {
        ret = 1;
    }
    if ((flags & 2) && (*(u32*)m & 0x20000)) {
        ret = 1;
    }
    if ((flags & 4) && (*(u32*)m & 0x100000)) {
        ret = 1;
    }
    if ((flags & 0x20) && *(u16*)((s32)m + 0x2E) == 0x17) {
        ret = 1;
    }
    if ((flags & 0x10) && *(u16*)((s32)m + 0x2E) == 0x16) {
        ret = 1;
    }
    if ((flags & 8) && *(u16*)((s32)m + 0x2E) == 0x14) {
        ret = 1;
    }
    return ret;
}

void marioPoseInit(void) {
    extern void* mp;
    extern void* marioAnimeGroupData[];
    extern s32 animPoseEntry(void* name, s32 flags);
    void* m = mp;
    s32 color = *(s8*)((s32)m + 0x3D);
    *(s32*)((s32)m + 0x22C) = animPoseEntry(marioAnimeGroupData[color * 3], 2);
    m = mp;
    color = *(s8*)((s32)m + 0x3D);
    *(s32*)((s32)mp + 0x230) = animPoseEntry(marioAnimeGroupData[color * 3 + 1], 2);
    m = mp;
    color = *(s8*)((s32)m + 0x3D);
    *(s32*)((s32)mp + 0x234) = animPoseEntry(marioAnimeGroupData[color * 3 + 2], 2);
}

void marioSetSpec(void) {
    extern void* mp;
    extern const f32 float_20_80420088;
    extern const f32 float_47_804200bc;
    extern const f32 float_1_80420008;
    extern const f32 float_2_80420004;
    extern const f32 float_2p25_804200b8;
    extern const f32 float_37_804200c0;
    extern const f32 float_10_80420078;
    extern const f32 float_25p9_804200c4;
    void* m = mp;
    s32 spec = *(s8*)((s32)m + 0x3C);

    if (spec == 1) {
        *(f32*)((s32)m + 0x1B8) = float_20_80420088;
        *(f32*)((s32)m + 0x1BC) = float_47_804200bc;
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2_80420004;
        return;
    }
    if (spec == 2) {
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
        return;
    }
    if ((*(u32*)m & 0x02000000) == 0) {
        *(f32*)((s32)m + 0x1B8) = float_20_80420088;
        *(f32*)((s32)m + 0x1BC) = float_37_804200c0;
    } else {
        *(f32*)((s32)m + 0x1B8) = float_10_80420078;
        *(f32*)((s32)m + 0x1BC) = float_25p9_804200c4;
    }
    *(f32*)((s32)m + 0x184) = float_1_80420008;
    *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgPoseTime(char* name, s32 time) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    void* m = mp;
    if (!(*(u32*)((s32)m + 4) & 2) && !(*(u32*)m & 0x80000000)) {
        char* old = *(char**)((s32)m + 0x18);
        if (old == 0 || strcmp(old, name) != 0) {
            *(char**)((s32)m + 0x18) = name;
            *(s16*)((s32)m + 0x28) = time;
            *(u32*)((s32)m + 4) &= ~0x30000000;
            *(u32*)((s32)m + 0xC) |= 0x1000;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgEvtPose(char* name) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    void* m = mp;
    if (!(*(u32*)((s32)m + 4) & 2) && !(*(u32*)m & 0x80000000)) {
        char* old = *(char**)((s32)m + 0x18);
        if (old == 0 || strcmp(old, name) != 0) {
            *(char**)((s32)m + 0x18) = name;
            *(u32*)((s32)m + 4) &= ~0x20000000;
            *(u32*)((s32)m + 4) |= 0x10000000;
            *(u32*)((s32)m + 0xC) |= 0x1000;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgPaper(char* name) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    void* m = mp;
    if ((*(u32*)((s32)m + 4) & 0x40000000) && !(*(u32*)((s32)m + 4) & 2) && !(*(u32*)m & 0x80000000)) {
        char* old = *(char**)((s32)m + 0x1C);
        if (old == 0 || strcmp(old, name) != 0) {
            *(char**)((s32)m + 0x1C) = name;
            *(s16*)((s32)m + 0x28) = 0;
            *(u32*)((s32)m + 0xC) |= 0x4000;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioGetScreenPos(f32* pos, f32* x, f32* y, f32* z) {
    extern void* camGetPtr(s32 id);
    extern void GXSetProjection(void* proj, s32 type);
    extern void GXGetProjectionv(f32* proj);
    extern void GXGetViewportv(f32* viewport);
    extern void GXProject(f32 x, f32 y, f32 z, void* model, f32* proj, f32* viewport, f32* outX, f32* outY, f32* outZ);
    f32 proj[7];
    f32 viewport[6];
    void* cam;

    cam = camGetPtr(4);
    GXSetProjection((void*)((s32)cam + 0x15C), *(s32*)((s32)cam + 0x19C));
    GXGetProjectionv(proj);
    GXGetViewportv(viewport);
    GXProject(pos[0], pos[1], pos[2], (void*)((s32)cam + 0x11C), proj, viewport, x, y, z);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioChkPushAnime(void) {
    extern void* mp;
    extern s32 strcmp(const char* s1, const char* s2);
    extern char str_M_O_1_8041ffe0;
    extern char str_M_O_2_8042009c;
    extern char str_M_O_2R_802c1b6c[];
    void* mario;
    s32 result;

    result = 0;
    mario = mp;
    if (strcmp(*(char**)((s32)mario + 0x18), &str_M_O_1_8041ffe0) == 0 ||
        strcmp(*(char**)((s32)mario + 0x18), &str_M_O_2_8042009c) == 0 ||
        strcmp(*(char**)((s32)mario + 0x18), str_M_O_2R_802c1b6c) == 0) {
        result = 1;
    }
    return result;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 marioFBattlePrepare(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* mp;
    extern Vec R_last_position;
    extern void marioAdjustMoveDir(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    extern void partyUsePost(void* party);
    extern void partyCtrlOff(void);
    void* mario = mp;

    R_last_position = *(Vec*)((s32)mario + 0x8C);
    marioAdjustMoveDir();
    if (*(u16*)((s32)mario + 0x2E) != 0x14) {
        *(u32*)mario |= 0x1000;
    }
    partyUsePost(partyGetPtr(marioGetPartyId()));
    partyCtrlOff();
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_800591b4(void) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    extern const char str_M_D_1_80420094[6];
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_J_1B_802c1b24[7];
    extern const char str_M_J_1C_802c1b34[7];
    extern void* dotMarioPose[][6];
    void* m;
    char* dotPose;
    u16 motion;

    m = mp;
    motion = *(u16*)((s32)m + 0x2E);
    if (motion != 0x19 && motion != 0x16 && motion != 0x18 && motion != 0x14 && motion != 0x1A &&
        (*(u32*)((s32)m + 4) & 0x01000000) == 0 && (*(u32*)((s32)m + 4) & 2) == 0 &&
        (*(char**)((s32)m + 0x18) == NULL || strcmp(*(char**)((s32)m + 0x18), str_M_D_1_80420094) != 0)) {
        if ((*(u32*)m & 0x80000000) == 0) {
            *(const char**)((s32)m + 0x18) = str_M_D_1_80420094;
            *(s16*)((s32)m + 0x28) = 0;
            *(u32*)((s32)m + 4) &= ~0x30000000;
            *(u32*)((s32)m + 0xC) |= 0x1000;
        } else {
            dotPose = NULL;
            if (strcmp(str_M_Z_1_8041ff90, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
            } else if (strcmp(str_M_S_1_8041ff98, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
            } else if (strcmp(str_M_W_1_8041ffa0, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
            } else if (strcmp(str_M_R_1_8041ffa8, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
            } else if (strcmp(str_M_J_1B_802c1b24, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            } else if (strcmp(str_M_J_1C_802c1b34, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            }
            if (dotPose != NULL) {
                *(char**)((s32)m + 0x18) = dotPose;
                *(s16*)((s32)m + 0x28) = 0;
                *(u32*)((s32)m + 4) &= ~0x30000000;
                *(u32*)((s32)m + 0xC) |= 0x1000;
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
