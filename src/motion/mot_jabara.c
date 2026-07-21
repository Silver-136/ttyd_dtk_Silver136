#include "motion/mot_jabara.h"

s32 marioGetJabaraState(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    s32 pose;
    register s32 state = 7;

    if (*(u16*)((s32)mario + 0x2E) != 0x14) {
        state = 0;
    } else {
        pose = *(s32*)((s32)mario + 0x44);
        if (pose == 0x14) {
            state = 1;
        } else if (pose >= 0x46 && pose <= 0x47) {
            state = 2;
        } else if (pose == 0x50) {
            state = 3;
        } else if (pose >= 0x28 && pose <= 0x2B) {
            state = 4;
        } else if (pose >= 0x32 && pose <= 0x34) {
            state = 5;
        } else if (pose >= 0x1E && pose <= 0x1F) {
            state = 6;
        }
    }
    return state;
}

s32 marioPriCheckJabara(void) {
    extern void* marioGetPtr(void);
    extern s32 pouchCheckItem(s32 itemId);

    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;
    s32 item;

    if ((flags & 0x80000000) || (flags & 0x02100000)) {
        *(u8*)((s32)mario + 0x40) = 0;
        return 0;
    }
    item = pouchCheckItem(8);
    return ((u32)(-item) | (u32)item) >> 31;
}

int marioChkJabara(void) {
    extern void* marioGetPtr(void);
    extern s32 pouchCheckItem(s32 itemId);
    extern s32 strcmp(const char* a, const char* b);
    extern void marioChgPose(const char* pose);
    extern void marioVoiceGlareOff(void);
    extern void marioVoiceGlareOn(void);
    extern s32 marioGetRub(s32 buttonFlags, s32* out1, s32* out2, s32* out3);
    extern char str_M_I_Y_80420cbc[];
    extern char str_M_S_1_80420c80[];
    extern f32 float_0_80420bd4;
    void* mario = marioGetPtr();
    s32 v1;
    s32 v2;
    s32 v3;
    s32 ok;

    ok = marioPriCheckJabara();
    if (ok == 0) {
        return 0;
    }
    if (!(*(u16*)((s32)mario + 0x24A) & 0x100)) {
        if (!(*(u16*)((s32)mario + 0x24A) & 0x200)) {
            if (strcmp(*(const char**)((s32)mario + 0x18), str_M_I_Y_80420cbc) == 0 && *(u16*)((s32)mario + 0x2E) == 0) {
                marioChgPose(str_M_S_1_80420c80);
                marioVoiceGlareOff();
            }
            *(u8*)((s32)mario + 0x40) = 0;
        }
        return 0;
    }
    if ((*(u32*)((s32)mario + 0xC) & 0x400000) && *(u16*)((s32)mario + 0x2E) == 0 &&
        !(*(u32*)((s32)mario + 0x4) & 0x40000000) && *(f32*)((s32)mario + 0x194) != float_0_80420bd4) {
        marioChgPose(str_M_I_Y_80420cbc);
        marioVoiceGlareOn();
    }
    if (marioGetRub(0x100, &v1, &v2, &v3) == 0) {
        return 0;
    }
    return v2 > 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void mot_jabara(void) {
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void __memFree(s32 heap, void* ptr);
    extern void mot_slit_post(void);
    extern void marioChgPose(char* pose);
    extern void marioPaperOn(char* pose);
    extern void marioPaperOff(void);
    extern void marioPaperLightOff(void);
    extern void marioChgPaper(char* pose);
    extern void marioChkGnd(void);
    extern void L_allPartyRideOn(void);
    extern void allPartyRideOff(void);
    extern u32 psndSFXOn_3D(s32 id, void* position);
    extern s32 sysMsec2Frame(s32 msec);
    extern s32 marioGetRub(s32 type, void* dir, void* count, void* scratch);
    extern f64 marioChkOverheadJabara(s32* side, f32* height);
    extern f64 marioChkOverhead(void);
    extern s32 marioSearchHead(f64 height, void* position, f32* outHeight);
    extern void* marioSearchHead_jabara(f64 height, void* position, f32* outHeight, void* scratch);
    extern void camFollowYOn(void);
    extern void camFollowYOff(void);
    extern void marioMakeJumpPara(void);
    extern void marioSetPeakYpos(void);
    extern void marioSetFallPara(void);
    extern f64 marioGetFallSpd(void);
    extern void* marioChkLandon(f32 speed, f32* y);
    extern void* marioChkLandon2(f64 speed, f64 y, f32* outY);
    extern void marioAdjustMoveDir(void);
    extern void marioChgMotSub(s32 motion, s32 param);
    extern void marioChgMot(s32 motion);
    extern void marioChgSmallJumpMotion(void);
    extern void marioClearJumpPara(void);
    extern void swingMain(void);
    extern u32 hitGetAttr(void* hit);
    extern char* hitGetName(void* hit);
    extern void hitObjGetPos(char* name, void* outPos);
    extern s32 strcmp(const char* a, const char* b);
    extern void* evtEntryType(void* script, s32 priority, s32 flags, s32 typeMask);
    extern s32 marioAnimeId(void);
    extern void animPoseSetLocalTime(f64 time, s32 poseId);
    extern f64 sin(f64 angle);
    extern f64 cos(f64 angle);
    extern void sincosf(f32 angle, f32* outSin, f32* outCos);
    extern s32 hitCheckFilter(f64 x, f64 y, f64 z, f64 dx, f64 dy, f64 dz, s32 flags,
                              void* out0, void* out1, void* out2, void* radius,
                              void* out3, void* out4, void* out5);
    extern void marioUpdateCamCollision(void* pos);
    extern void unk_JP_US_EU_31_8013587c(void);
    extern void quake_evt;
    extern void* gp;
    extern f32 vec3_802c3fd0[];
    extern f32 float_0_80420bd4;
    extern f32 float_0p1_80420bd8;
    extern f32 float_180_80420bdc;
    extern f32 float_1p4_80420be0;
    extern f32 float_0p8_80420be4;
    extern f32 float_5_80420be8;
    extern f32 float_neg0p1_80420bec;
    extern f32 float_0p18_80420bf0;
    extern f32 float_24_80420bf4;
    extern f32 float_2p4_80420bf8;
    extern f32 float_neg0p4_80420bfc;
    extern f32 float_0p01_80420c00;
    extern f32 float_20_80420c04;
    extern f32 float_37_80420c08;
    extern f32 float_0p2_80420c0c;
    extern f32 float_0p005_80420c10;
    extern f32 float_1p5_80420c14;
    extern f32 float_0p4_80420c18;
    extern f32 float_1p7_80420c1c;
    extern f32 float_0p3_80420c20;
    extern f32 float_1p6_80420c24;
    extern f32 float_480_80420c28;
    extern f32 float_50_80420c2c;
    extern f32 float_25_80420c30;
    extern f32 float_200_80420c34;
    extern f32 float_10p45_80420c38;
    extern f32 float_0p278_80420c3c;
    extern f32 float_neg0p031_80420c40;
    extern f32 float_8_80420c44;
    extern f32 float_neg0p84_80420c48;
    extern f32 float_0p08_80420c4c;
    extern f32 float_neg0p015_80420c50;
    extern f32 float_4p3_80420c54;
    extern f32 float_0p5_80420c58;
    extern f32 float_100_80420c5c;
    extern f32 float_3p5_80420c60;
    extern f32 float_9_80420c64;
    extern f32 float_neg6_80420c68;
    extern f32 float_neg450_80420c6c;
    extern f32 float_neg190_80420c70;
    extern f32 float_neg8_80420c74;
    extern f32 float_4_80420c78;
    extern f32 float_2_80420c7c;
    extern char str_M_S_1_80420c80;
    extern f32 float_0p6_80420c88;
    extern f32 float_neg0p6_80420c8c;
    extern f32 float_3p1416_80420c90;
    extern f32 float_neg1_80420c94;
    extern char str_M_S_4_80420c98;
    extern f32 float_10_80420ca0;
    extern f32 float_1_80420ca4;
    extern char str_M_W_6_80420ca8;
    extern f32 float_90_80420cb0;
    extern f32 float_270_80420cb4;
    extern f32 float_neg0p5_80420cb8;

    typedef struct VecBits { u32 x; u32 y; u32 z; } VecBits;
    typedef struct VecF { f32 x; f32 y; f32 z; } VecF;

    f32* data = vec3_802c3fd0;
    void* player = marioGetPtr();
    void* work;
    void* hit;
    f32 headY;
    f32 landY;
    f32 fall;
    f32 temp;
    f32 localPos[3];
    f32 localPos2[3];
    f32 localSin;
    f32 localCos;
    f32 localRadius;
    f32 localScratch0;
    f32 localScratch1;
    f32 localScratch2;
    u32 stack0[2];
    s32 side;
    s32 rub;
    s32 timedOut;
    s32 animId;
    s32 frame;
    u8 rubDir[4];
    u8 rubCount[4];
    u8 rubScratch[4];
    char* name;

#define STR(o) ((char*)((char*)data + (o)))
#define FLAGS (*(u32*)((s32)player + 0x0))
#define DISP_FLAGS (*(u32*)((s32)player + 0x4))
#define TRIG_FLAGS (*(u32*)((s32)player + 0xC))
#define BUTTON_HELD (*(u16*)((s32)player + 0x24A))
#define BUTTON_PRESSED (*(u16*)((s32)player + 0x24C))
#define BUTTON_RELEASED (*(u16*)((s32)player + 0x250))
#define STICK_DIR1 (*(s8*)((s32)player + 0x244))
#define STICK_DIR2 (*(s8*)((s32)player + 0x245))
#define SUBMOTION (*(u32*)((s32)player + 0x44))
#define TIMER (*(s32*)((s32)player + 0x48))
#define MOTION_COUNT (*(s8*)((s32)player + 0x41))
#define CHARGE (*(s8*)((s32)player + 0x42))
#define AIR_TIMER (*(s16*)((s32)player + 0x50))
#define AIR_TIMER2 (*(s16*)((s32)player + 0x52))
#define CUR_MOTION (*(u16*)((s32)player + 0x2E))
#define POS ((f32*)((s32)player + 0x8C))
#define MODEL ((f32*)((s32)player + 0x98))
#define PIVOT ((VecBits*)((s32)player + 0xB0))
#define ROTV ((VecBits*)((s32)player + 0xBC))
#define COLL ((f32*)((s32)player + 0x1B8))
#define JUMP_SPEED (*(f32*)((s32)player + 0x7C))
#define JUMP_ACCEL (*(f32*)((s32)player + 0x80))
#define JUMP_JERK (*(f32*)((s32)player + 0x84))
#define JUMP_SNAP (*(f32*)((s32)player + 0x88))
#define BASE_SPEED (*(f32*)((s32)player + 0x180))
#define STICK_SENS (*(f32*)((s32)player + 0x194))
#define STICK_ANGLE (*(f32*)((s32)player + 0x198))
#define VIEW_DIR (*(f32*)((s32)player + 0x19C))
#define DISP_CUR (*(f32*)((s32)player + 0x1AC))
#define DISP_TGT (*(f32*)((s32)player + 0x1B0))
#define HEAD_HIT (*(void**)((s32)player + 0x1F8))
#define STAND_HIT (*(void**)((s32)player + 0x1E8))
#define JUMP_FROM (*(void**)((s32)player + 0x1EC))
#define POSE_ID(i) (*(s32*)((s32)player + 0x22C + ((i) * 4)))
#define MOT_WORK (*(void**)((s32)player + 0x294))
#define WM2 (*(f32*)((s32)player + 0x2BC))
#define WM3 (*(s32*)((s32)player + 0x2C0))
#define WM5 (*(f32*)((s32)player + 0x2C8))
#define WM6 (*(f32*)((s32)player + 0x2CC))
#define WMTIMER (*(s16*)((s32)player + 0x2D0))
#define SWING_STATE (*(s8*)((s32)player + 0x2D2))
#define ANIM_COUNTER (*(s8*)((s32)player + 0x2D3))
#define WU32(o) (*(u32*)((s32)work + (o)))
#define W32(o) (*(s32*)((s32)work + (o)))
#define WF(o) (*(f32*)((s32)work + (o)))

#define APPLY_CAM(z, zoom) do { \
        void* mp__ = marioGetPtr(); \
        *(f32*)((s32)mp__ + 0x148) = (zoom); \
        *(f32*)((s32)mp__ + 0x158) = float_0p005_80420c10; \
        *(f32*)((s32)mp__ + 0x68) = (z); \
        *(VecBits*)((s32)mp__ + 0x6C) = *(VecBits*)data; \
    } while (0)

#define RESTORE_NORMAL() do { \
        allPartyRideOff(); \
        player = marioGetPtr(); \
        DISP_CUR = WM2; \
        DISP_TGT = WM2; \
        *PIVOT = *(VecBits*)(data + 3); \
        *(VecBits*)((s32)player + 0x98) = *(VecBits*)(data + 6); \
        *ROTV = *(VecBits*)(data + 9); \
        COLL[0] = float_20_80420c04; \
        COLL[1] = float_37_80420c08; \
        marioAdjustMoveDir(); \
        DISP_FLAGS &= ~0x4U; \
        DISP_FLAGS &= ~0x100U; \
        if (MOT_WORK != 0) { \
            __memFree(0, MOT_WORK); \
            MOT_WORK = 0; \
        } \
    } while (0)

    if ((TRIG_FLAGS & 1) != 0) {
        TRIG_FLAGS &= ~1U;
        *(u32*)((s32)player + 0x2B8) = 0;
        *(u32*)((s32)player + 0x2BC) = 0;
        *(u32*)((s32)player + 0x2C0) = 0;
        *(u32*)((s32)player + 0x2C4) = 0;
        *(u32*)((s32)player + 0x2C8) = 0;
        *(u32*)((s32)player + 0x2CC) = 0;
        *(u32*)((s32)player + 0x2D0) = 0;
        if ((DISP_FLAGS & 0x01000000) != 0) {
            mot_slit_post();
        }
        FLAGS &= ~0xF0000U;
        SUBMOTION = 0;
        work = __memAlloc(0, 0x20);
        marioGetPtr();
        MOT_WORK = work;
        marioGetPtr();
        memset(MOT_WORK, 0, 0x20);
    }

    HEAD_HIT = 0;
    work = MOT_WORK;

    switch (SUBMOTION) {
        case 0:
            DISP_FLAGS |= 0x100;
            MOTION_COUNT = 1;
            CHARGE = 0;
            AIR_TIMER = 0;
            AIR_TIMER2 = 0;
            BASE_SPEED = float_0_80420bd4;
            SUBMOTION = 10;
            marioChgPose(STR(0x30));
            marioPaperOn(STR(0x38));
            marioChgPaper(STR(0x44));
            *(VecBits*)((s32)player + 0x11C) = *(VecBits*)((s32)player + 0x8C);
            marioChkGnd();
            if (CUR_MOTION != 0x14) {
                SUBMOTION = 100;
            }
            break;

        case 10:
            if (CUR_MOTION != 0x14) {
                SUBMOTION = 100;
                break;
            }
            L_allPartyRideOn();
            psndSFXOn_3D(0x182, POS);
            APPLY_CAM(float_1p5_80420c14, float_0p2_80420c0c);
            TIMER = 8;
            SUBMOTION = 11;
            /* fallthrough */
        case 11:
            TIMER--;
            if (TIMER < 1) {
                SUBMOTION = 12;
                if ((WU32(0) & 0x10000) == 0) {
                    WU32(0) |= 0x10000;
                }
            }
            /* fallthrough */
        case 12:
            headY = (f32)marioChkOverheadJabara(&side, &landY);
            rub = marioGetRub(0x100, rubDir, rubCount, rubScratch);
            if (rub == 1) {
                MOTION_COUNT++;
                if (MOTION_COUNT > 0x13) {
                    MOTION_COUNT = 0x14;
                }
            }
            if (MOTION_COUNT == 3) {
                if ((WU32(0) & 0x40000) == 0) {
                    WU32(0) |= 0x40000;
                    psndSFXOn_3D(0x184, POS);
                }
                marioChgPaper(STR(0x4C));
                CHARGE = 3;
                APPLY_CAM(float_1p7_80420c1c, float_0p4_80420c18);
            } else if (MOTION_COUNT == 2) {
                if ((WU32(0) & 0x20000) == 0) {
                    WU32(0) |= 0x20000;
                    psndSFXOn_3D(0x183, POS);
                }
                marioChgPaper(STR(0x54));
                CHARGE = 2;
                APPLY_CAM(float_1p6_80420c24, float_0p3_80420c20);
            }
            marioChkGnd();
            if (CUR_MOTION != 0x14) {
                SUBMOTION = 100;
                return;
            }
            if (((f32)(s32)*(u32*)((s32)player + 0x16C) > float_480_80420c28) ||
                ((f32)(s32)(-*(s32*)((s32)player + 0x16C)) < float_50_80420c2c)) {
                camFollowYOn();
            }
            if (POS[1] == *(f32*)((s32)player + 0x114)) {
                camFollowYOff();
            }
            localPos[0] = POS[0];
            localPos[1] = POS[1];
            localPos[2] = POS[2];
            landY = float_25_80420c30;
            timedOut = 0;
            if (marioSearchHead((f64)float_200_80420c34, localPos, &landY) != 0) {
                if ((BUTTON_HELD & 0x100) == 0) {
                    work = MOT_WORK;
                    W32(0x1C)++;
                    if (sysMsec2Frame(5000) < W32(0x1C)) {
                        timedOut = 1;
                    }
                }
                if (!timedOut) {
                    break;
                }
            }
            if (timedOut || ((BUTTON_HELD & 0x100) == 0) || ((BUTTON_RELEASED & 0x100) != 0)) {
                APPLY_CAM(float_0_80420bd4, float_0_80420bd4);
                *(f32*)((s32)marioGetPtr() + 0x158) = float_0p01_80420c00;
                camFollowYOff();
                marioChgPose(STR(0x5C));
                marioChgPaper(STR(0x64));
                psndSFXOn_3D(0x20185, POS);
                psndSFXOn_3D(0x200B5, POS);
                if (CHARGE == 3) {
                    JUMP_SPEED = float_10p45_80420c38;
                    JUMP_ACCEL = float_0p278_80420c3c;
                    JUMP_JERK = float_neg0p031_80420c40;
                    JUMP_SNAP = float_0_80420bd4;
                    unk_JP_US_EU_31_8013587c();
                } else if (CHARGE == 2) {
                    JUMP_SPEED = float_8_80420c44;
                    JUMP_ACCEL = float_neg0p84_80420c48;
                    JUMP_JERK = float_0p08_80420c4c;
                    JUMP_SNAP = float_neg0p015_80420c50;
                } else {
                    JUMP_SPEED = float_4p3_80420c54;
                    JUMP_ACCEL = float_neg0p84_80420c48;
                    JUMP_JERK = float_0p08_80420c4c;
                    JUMP_SNAP = float_neg0p015_80420c50;
                }
                JUMP_FROM = STAND_HIT;
                STAND_HIT = 0;
                WM2 = DISP_TGT;
                SUBMOTION = 20;
            }
            break;

        case 20:
            marioMakeJumpPara();
            if (CHARGE == 3) {
                headY = (f32)marioChkOverheadJabara(&side, &landY);
            } else {
                headY = (f32)marioChkOverhead();
            }
            if (HEAD_HIT == 0) {
                if (JUMP_SPEED <= float_0_80420bd4) {
                    SUBMOTION = 30;
                    marioSetPeakYpos();
                }
                POS[1] += JUMP_SPEED;
            } else {
                if (headY < *(f32*)((s32)player + 0x120)) {
                    marioPaperOff();
                    hit = STAND_HIT;
                    temp = *(f32*)((s32)player + 0x120);
                    player = marioGetPtr();
                    AIR_TIMER = 0;
                    AIR_TIMER2 = 0;
                    STAND_HIT = hit;
                    JUMP_FROM = 0;
                    POS[1] = temp;
                    FLAGS &= ~0xF0000U;
                    camFollowYOn();
                    RESTORE_NORMAL();
                    marioChgMotSub(0, 0);
                    return;
                }
                POS[1] = headY;
                if (CHARGE < 3) {
                    SUBMOTION = 30;
                } else if ((hitGetAttr(HEAD_HIT) & 0x40000) == 0) {
                    SUBMOTION = 40;
                } else {
                    WM2 = DISP_TGT;
                    DISP_CUR = DISP_TGT;
                    SUBMOTION = 0x46;
                    TIMER = 10;
                    WM3 = (s32)(headY + COLL[1]);
                    name = hitGetName(HEAD_HIT);
                    hitObjGetPos(name, localPos2);
                    POS[2] = (f32)(s32)((float_100_80420c5c * localPos2[2] + float_0p5_80420c58) / float_100_80420c5c);
                    POS[1] = ((f32)(-WM3) - COLL[1]) - float_3p5_80420c60;
                    *(f32*)((s32)player + 0xB4) = float_9_80420c64 + COLL[1];
                    marioPaperOff();
                    camFollowYOn();
                    DISP_FLAGS |= 4;
                    marioChgPose(STR(0x5C));
                    psndSFXOn_3D(0x186, POS);
                    WMTIMER = 4;
                    WM5 = float_24_80420bf4;
                    work = MOT_WORK;
                    WF(4) = float_2p4_80420bf8;
                    WF(8) = float_neg0p4_80420bfc;
                    WF(0x10) = float_0p18_80420bf0;
                    SWING_STATE = (side < 0) ? 11 : 1;
                }
            }
            break;

        case 30:
            marioChgPose(STR(0x6C));
            marioChgPaper(STR(0x74));
            marioSetFallPara();
            SUBMOTION = 31;
            /* fallthrough */
        case 31:
            fall = (f32)marioGetFallSpd();
            if (fall < float_neg6_80420c68) {
                fall = float_neg6_80420c68;
            }
            hit = marioChkLandon(fall, &landY);
            if (hit != 0) {
                marioPaperOff();
                player = marioGetPtr();
                AIR_TIMER = 0;
                AIR_TIMER2 = 0;
                STAND_HIT = hit;
                JUMP_FROM = 0;
                POS[1] = landY;
                FLAGS &= ~0xF0000U;
                camFollowYOn();
                RESTORE_NORMAL();
                marioChgMotSub(0, 0);
                return;
            }
            POS[1] += fall;
            break;
    }

    switch (SUBMOTION) {
        case 0x28:
            if ((strcmp((const char*)((s32)gp + 0x12C), STR(0x7C)) == 0) &&
                POS[0] >= float_neg450_80420c6c && POS[0] <= float_neg190_80420c70) {
                FLAGS |= 0x800;
            }
            psndSFXOn_3D(0x188, POS);
            evtEntryType(&quake_evt, 0, 0, 0);
            marioClearJumpPara();
            marioChgPose(STR(0x5C));
            marioChgPaper(STR(0x84));
            ANIM_COUNTER = 1;
            *(s16*)((s32)player + 0x28) = ANIM_COUNTER;
            MODEL[1] = float_neg8_80420c74;
            TIMER = 0x32;
            SUBMOTION = 0x29;
            break;
        case 0x29:
            animId = marioAnimeId();
            animPoseSetLocalTime((f64)(-ANIM_COUNTER), POSE_ID(animId));
            ANIM_COUNTER++;
            if (ANIM_COUNTER > 7) {
                ANIM_COUNTER = 8;
                COLL[1] = float_4_80420c78;
            }
            TIMER--;
            if (TIMER < 1) {
                SUBMOTION = 0x2A;
            }
            break;
        case 0x2A:
            MODEL[1] += float_2_80420c7c;
            if (MODEL[1] >= float_0_80420bd4) {
                MODEL[1] = float_0_80420bd4;
            }
            animId = marioAnimeId();
            animPoseSetLocalTime((f64)(-ANIM_COUNTER), POSE_ID(animId));
            ANIM_COUNTER++;
            if (ANIM_COUNTER > 0x0F) {
                ANIM_COUNTER = 0x10;
                marioChgPaper(STR(0x8C));
                marioPaperLightOff();
                SUBMOTION = 0x2B;
                TIMER = 0x18;
            }
            break;
        case 0x2B:
            TIMER--;
            if (TIMER < 1) {
                marioChgPose(&str_M_S_1_80420c80);
                marioChgPaper(STR(0x94));
                marioPaperLightOff();
                SUBMOTION = 0x32;
                ANIM_COUNTER = 0;
                work = MOT_WORK;
                WF(0x0C) = float_0_80420bd4;
            }
            break;
        case 0x32:
            animId = marioAnimeId();
            animPoseSetLocalTime((f64)(-ANIM_COUNTER), POSE_ID(animId));
            if (ANIM_COUNTER < 0x23 || ANIM_COUNTER > 0x62) {
                temp = (DISP_TGT == float_180_80420bdc) ? float_neg0p6_80420c8c : float_0p6_80420c88;
            } else {
                temp = (DISP_TGT == float_180_80420bdc) ? float_0p6_80420c88 : float_neg0p6_80420c8c;
            }
            work = MOT_WORK;
            if (WF(0x18) != temp) {
                W32(0x14)++;
                if (W32(0x14) == 1) {
                    psndSFXOn_3D(0x189, POS);
                } else if (W32(0x14) == 2) {
                    psndSFXOn_3D(0x18A, POS);
                } else if (W32(0x14) == 3) {
                    psndSFXOn_3D(0x18B, POS);
                }
            }
            WF(0x18) = temp;
            MODEL[0] += temp * (f32)cos((f64)((float_3p1416_80420c90 * VIEW_DIR) / float_180_80420bdc));
            MODEL[2] += temp * (f32)sin((f64)((float_3p1416_80420c90 * VIEW_DIR) / float_180_80420bdc));
            fall = float_neg1_80420c94 * (f32)sin((f64)1.5708f);
            hit = marioChkLandon2((f64)fall, (f64)MODEL[1], &landY);
            if (hit == 0) {
                POS[1] += fall;
                if ((f32)(-*(s32*)((s32)player + 0x16C)) > float_480_80420c28) {
                    camFollowYOn();
                }
                ANIM_COUNTER++;
                if (ANIM_COUNTER > 0x78) {
                    ANIM_COUNTER = 0;
                }
                if ((BUTTON_PRESSED & 0x200) != 0) {
                    FLAGS |= 0x40000;
                    FLAGS &= ~0x800U;
                    marioPaperOff();
                    RESTORE_NORMAL();
                    marioChgMotSub(0, 0);
                    return;
                }
            } else {
                STAND_HIT = hit;
                JUMP_FROM = 0;
                POS[1] = landY;
                SUBMOTION = 0x34;
                ANIM_COUNTER = 0;
                TIMER = 0x14;
            }
            break;
        case 0x34:
            animId = marioAnimeId();
            animPoseSetLocalTime((f64)(-ANIM_COUNTER), POSE_ID(animId));
            TIMER--;
            if (TIMER < 1) {
                SUBMOTION = 0x3C;
            }
            break;
    }

    if (SUBMOTION == 0x3C) {
        allPartyRideOff();
        SUBMOTION = 0x3D;
        JUMP_FROM = STAND_HIT;
        STAND_HIT = 0;
        BASE_SPEED = float_0_80420bd4;
        FLAGS &= ~0x800U;
        RESTORE_NORMAL();
        marioPaperOff();
        camFollowYOff();
        marioChgSmallJumpMotion();
        BASE_SPEED = float_0_80420bd4;
        return;
    }
    if (SUBMOTION > 0x3B && SUBMOTION < 0x46) {
        return;
    }

    if (SUBMOTION == 0x46) {
        TIMER--;
        if (TIMER < 1) {
            marioChgPose(&str_M_S_4_80420c98);
            SUBMOTION = 0x47;
        }
    }
    if (SUBMOTION == 0x46 || SUBMOTION == 0x47) {
        if (((SWING_STATE == 5) || (SWING_STATE == 15)) && (STICK_DIR1 != 0 || STICK_DIR2 != 0)) {
            goto start_crawl;
        }
        if (SWING_STATE == 0 || SWING_STATE == 10) {
start_crawl:
            SUBMOTION = 0x50;
            ROTV->x = float_0_80420bd4;
            PIVOT->x = float_0_80420bd4;
            SWING_STATE = 0;
            WM5 = float_0_80420bd4;
            POS[1] = ((f32)(-WM3) - COLL[1]) - float_3p5_80420c60;
            *(f32*)((s32)player + 0xB4) = float_9_80420c64 + COLL[1];
        }
    } else if (SUBMOTION == 0x50) {
        if ((BUTTON_PRESSED & 0x100) != 0) {
            FLAGS |= 0x40000;
            BASE_SPEED = float_0_80420bd4;
            ROTV->x = float_0_80420bd4;
            SUBMOTION = 0x5A;
        } else if ((BUTTON_PRESSED & 0x200) != 0) {
            FLAGS |= 0x40000;
            RESTORE_NORMAL();
            marioChgMotSub(0, 0);
            return;
        } else {
            animId = marioAnimeId();
            animPoseSetLocalTime((f64)(-ANIM_COUNTER), POSE_ID(animId));
            localPos[0] = POS[0];
            localPos[1] = POS[1];
            localPos[2] = POS[2];
            landY = float_10_80420ca0 + COLL[1];
            localPos[0] += float_1_80420ca4 * (f32)sin((f64)((float_3p1416_80420c90 * (STICK_ANGLE + VIEW_DIR)) / float_180_80420bdc));
            localPos[2] -= float_1_80420ca4 * (f32)cos((f64)((float_3p1416_80420c90 * (STICK_ANGLE + VIEW_DIR)) / float_180_80420bdc));
            HEAD_HIT = marioSearchHead_jabara((f64)float_0_80420bd4, localPos, &landY, &localScratch0);
            if (HEAD_HIT == 0) {
                landY = float_5_80420be8 + COLL[1];
                HEAD_HIT = (void*)marioSearchHead((f64)float_0_80420bd4, localPos, &landY);
            }
            if (STICK_SENS != float_0_80420bd4) {
                marioChgPose(&str_M_W_6_80420ca8);
                if (SWING_STATE == 0) {
                    SWING_STATE = 1;
                    WMTIMER = 10;
                    WM5 = float_0_80420bd4;
                    work = MOT_WORK;
                    WF(4) = float_0p1_80420bd8;
                    WF(8) = float_neg0p1_80420bec;
                    WF(0x10) = float_0p18_80420bf0;
                }
                if (HEAD_HIT != 0 && (hitGetAttr(HEAD_HIT) & 0x40000) != 0) {
                    localRadius = float_0p5_80420c58 * COLL[0];
                    sincosf(STICK_ANGLE, &localSin, &localCos);
                    if (hitCheckFilter((f64)POS[0], (f64)(float_2_80420c7c + POS[1] + COLL[1]), (f64)POS[2],
                                       (f64)localSin, (f64)float_0_80420bd4, (f64)localCos, 0,
                                       &localScratch0, &localScratch1, &localScratch2, &localRadius,
                                       &localPos2[0], &localPos2[1], &localPos2[2]) == 0) {
                        temp = float_1_80420ca4 * (f32)sin((f64)((float_3p1416_80420c90 * STICK_ANGLE) / float_180_80420bdc));
                        POS[0] += temp;
                        if (temp <= float_0_80420bd4) {
                            if (DISP_CUR < float_90_80420cb0 || DISP_CUR > float_270_80420cb4) {
                                ANIM_COUNTER++;
                                if (ANIM_COUNTER > 0x20) ANIM_COUNTER = 0;
                            } else {
                                ANIM_COUNTER--;
                                if (ANIM_COUNTER < 0) ANIM_COUNTER = 0x20;
                            }
                        } else {
                            if (DISP_CUR < float_90_80420cb0 || DISP_CUR > float_270_80420cb4) {
                                ANIM_COUNTER--;
                                if (ANIM_COUNTER < 0) ANIM_COUNTER = 0x20;
                            } else {
                                ANIM_COUNTER++;
                                if (ANIM_COUNTER > 0x20) ANIM_COUNTER = 0;
                            }
                        }
                        if (ANIM_COUNTER == 0 || ANIM_COUNTER == 0x10) {
                            psndSFXOn_3D(0x187, POS);
                        }
                    }
                }
                localPos2[0] = POS[0];
                localPos2[1] = POS[1];
                localPos2[2] = POS[2];
                marioUpdateCamCollision(localPos2);
            }
        }
    }

    if (SUBMOTION == 0x5A) {
        marioChgPose(STR(0xA8));
        SUBMOTION = 0x5B;
        DISP_FLAGS |= 4;
        marioAdjustMoveDir();
        JUMP_SPEED = float_4_80420c78;
        JUMP_ACCEL = float_neg0p5_80420cb8;
        JUMP_JERK = float_0_80420bd4;
        JUMP_SNAP = float_0_80420bd4;
    }
    if (SUBMOTION == 0x5B) {
        POS[1] += JUMP_SPEED;
        marioMakeJumpPara();
        if (JUMP_SPEED <= float_0_80420bd4) {
            marioChgMot(3);
            marioSetFallPara();
        }
    }

    if (SUBMOTION > 0x45) {
        swingMain();
    }

#undef WF
#undef W32
#undef WU32
#undef ANIM_COUNTER
#undef SWING_STATE
#undef WMTIMER
#undef WM6
#undef WM5
#undef WM3
#undef WM2
#undef MOT_WORK
#undef POSE_ID
#undef JUMP_FROM
#undef STAND_HIT
#undef HEAD_HIT
#undef DISP_TGT
#undef DISP_CUR
#undef VIEW_DIR
#undef STICK_ANGLE
#undef STICK_SENS
#undef BASE_SPEED
#undef JUMP_SNAP
#undef JUMP_JERK
#undef JUMP_ACCEL
#undef JUMP_SPEED
#undef COLL
#undef ROTV
#undef PIVOT
#undef MODEL
#undef POS
#undef CUR_MOTION
#undef AIR_TIMER2
#undef AIR_TIMER
#undef CHARGE
#undef MOTION_COUNT
#undef TIMER
#undef SUBMOTION
#undef STICK_DIR2
#undef STICK_DIR1
#undef BUTTON_RELEASED
#undef BUTTON_PRESSED
#undef BUTTON_HELD
#undef TRIG_FLAGS
#undef DISP_FLAGS
#undef FLAGS
#undef RESTORE_NORMAL
#undef APPLY_CAM
#undef STR
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw reset


s32 jabaraNoHitChk(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);
    s32 pose;

    if (motion != 0x14) {
        return 0;
    }

    pose = *(s32*)((s32)mario + 0x44);
    if (pose >= 0x28 && pose < 0x32) {
        return 1;
    }

    return 0;
}

void mot_jabara_post(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void allPartyRideOff(void);
    extern void marioAdjustMoveDir(void);
    extern void __memFree(s32 heap, void* ptr);
    extern void marioClearJumpPara(void);
    extern f32 vec3_802c3fd0[];
    extern f32 float_0_80420bd4;
    extern f32 float_0p01_80420c00;
    extern f32 float_20_80420c04;
    extern f32 float_37_80420c08;
    typedef struct Vec { f32 x, y, z; } Vec;
    f32* data = vec3_802c3fd0;
    void* mario = marioGetPtr();
    void* mario2;
    void* mario3;

    *(u32*)((s32)mario + 0x0) &= ~0x800;
    mario2 = marioGetPtr();
    *(f32*)((s32)mario2 + 0x148) = float_0_80420bd4;
    *(f32*)((s32)mario2 + 0x158) = float_0p01_80420c00;
    if (*(u32*)((s32)mario + 0x4) & 0x40000000) {
        marioPaperOff();
    }
    allPartyRideOff();
    mario3 = marioGetPtr();
    *(f32*)((s32)mario3 + 0x1AC) = *(f32*)((s32)mario3 + 0x2BC);
    *(f32*)((s32)mario3 + 0x1B0) = *(f32*)((s32)mario3 + 0x2BC);
    *(Vec*)((s32)mario3 + 0xB0) = *(Vec*)(data + 3);
    *(Vec*)((s32)mario3 + 0x98) = *(Vec*)(data + 6);
    *(Vec*)((s32)mario3 + 0xBC) = *(Vec*)(data + 9);
    *(f32*)((s32)mario3 + 0x1B8) = float_20_80420c04;
    *(f32*)((s32)mario3 + 0x1BC) = float_37_80420c08;
    marioAdjustMoveDir();
    *(u32*)((s32)mario3 + 0x4) &= ~0x4;
    *(u32*)((s32)mario3 + 0x4) &= ~0x100;
    if (*(void**)((s32)marioGetPtr() + 0x294) != 0) {
        void* ptr = *(void**)((s32)marioGetPtr() + 0x294);
        __memFree(0, ptr);
        *(void**)((s32)marioGetPtr() + 0x294) = 0;
    }
    if (*(u16*)((s32)mario + 0x2E) < 3 || (*(u16*)((s32)mario + 0x2E) >= 9 && *(u16*)((s32)mario + 0x2E) < 0xC)) {
        marioClearJumpPara();
    }
}
u8 swingMain(void) {
    extern void* marioGetPtr(void);
    extern f64 revise360(f64 angle);
    extern f64 __fabs(f64 value);
    extern f32 float_0_80420bd4;
    extern f32 float_0p1_80420bd8;
    extern f32 float_0p8_80420be4;
    extern f32 float_1p4_80420be0;
    extern f32 float_5_80420be8;
    extern f32 float_40_80420bd0;
    extern f32 float_180_80420bdc;

    void* player = marioGetPtr();
    void* mot = *(void**)((s32)player + 0x294);
    f32 previous;
    s16 timer;
    s8 state;

#define ROT (*(f32*)((s32)player + 0xBC))
#define SPEED (*(f32*)((s32)player + 0x2C8))
#define ACCEL (*(f32*)((s32)player + 0x2CC))
#define TIMER (*(s16*)((s32)player + 0x2D0))
#define STATE (*(s8*)((s32)player + 0x2D2))

    if (mot == 0) {
        return 0;
    }

    if (*(s32*)((s32)player + 0x44) < 0x5A &&
        (*(u16*)((s32)player + 0x24C) & 0x300) != 0 &&
        ((*(u32*)mot & 1) == 0)) {
        TIMER = 0;
        *(u32*)mot |= 1;
    }

    if (STATE == 1) {
        timer = TIMER;
        if (timer > 0) {
            TIMER = timer - 1;
            SPEED += *(f32*)((s32)mot + 4);
            if (SPEED >= float_40_80420bd0) {
                SPEED = float_40_80420bd0;
            }
        } else if (timer == 0) {
            SPEED += *(f32*)((s32)mot + 8);
            if (SPEED <= float_0_80420bd4) {
                SPEED = float_0_80420bd4;
                ACCEL = float_0p1_80420bd8;
                STATE = ROT < float_180_80420bdc ? 4 : 2;
            }
        }

        if ((*(u32*)mot & 1) == 0) {
            ROT = (f32)revise360(ROT + SPEED);
        } else {
            previous = ROT;
            ROT = (f32)revise360(previous + SPEED);
            if (ROT < previous) {
                SPEED = float_1p4_80420be0;
                ACCEL = float_0p1_80420bd8;
                STATE = ROT < float_180_80420bdc ? 4 : 2;
            }
        }
    }

    state = STATE;
    if (state == 4) {
        SPEED -= ACCEL;
        ROT = (f32)revise360(ROT + SPEED);
        if (ROT > float_180_80420bdc) {
            STATE = 5;
            ACCEL += *(f32*)((s32)mot + 0x10);
        }
    } else if (state == 2) {
        SPEED += ACCEL;
        ROT = (f32)revise360(ROT + SPEED);
        if (ROT < float_180_80420bdc) {
            STATE = 3;
            ACCEL += *(f32*)((s32)mot + 0x10);
        }
    } else if (state == 3) {
        SPEED -= ACCEL;
        if (SPEED <= float_0_80420bd4) {
            SPEED = float_0_80420bd4;
            STATE = 4;
            ACCEL -= *(f32*)((s32)mot + 0x10);
            if (__fabs(ROT) < float_0p8_80420be4) {
                STATE = 0;
            }
        }
        ROT = (f32)revise360(ROT + SPEED);
    } else if (state == 5) {
        SPEED += ACCEL;
        if (SPEED >= float_0_80420bd4) {
            SPEED = float_0_80420bd4;
            STATE = 2;
            ACCEL -= *(f32*)((s32)mot + 0x10);
            if (__fabs(ROT) < float_0p8_80420be4) {
                STATE = 0;
            }
        }
        ROT = (f32)revise360(ROT + SPEED);
    }

    if (STATE == 11) {
        timer = TIMER;
        if (timer > 0) {
            TIMER = timer - 1;
            SPEED += *(f32*)((s32)mot + 4);
            if (SPEED >= float_40_80420bd0) {
                SPEED = float_40_80420bd0;
            }
        } else if (timer == 0) {
            SPEED += *(f32*)((s32)mot + 8);
            if (SPEED <= float_0_80420bd4) {
                SPEED = float_1p4_80420be0;
                ACCEL = float_0p1_80420bd8;
                STATE = ROT < float_180_80420bdc ? 14 : 12;
            }
        }

        if ((*(u32*)mot & 1) == 0) {
            ROT = (f32)revise360(ROT - SPEED);
        } else {
            previous = ROT;
            ROT = (f32)revise360(previous - SPEED);
            if (previous < ROT) {
                SPEED = float_5_80420be8;
                ACCEL = float_0p1_80420bd8;
                STATE = ROT < float_180_80420bdc ? 14 : 12;
            }
        }
    }

    state = STATE;
    if (state == 14) {
        SPEED -= ACCEL;
        ROT = (f32)revise360(ROT - SPEED);
        if (ROT < float_180_80420bdc) {
            STATE = 15;
            ACCEL += *(f32*)((s32)mot + 0x10);
        }
    } else if (state == 12) {
        SPEED += ACCEL;
        ROT = (f32)revise360(ROT - SPEED);
        if (ROT > float_180_80420bdc) {
            STATE = 13;
            ACCEL += *(f32*)((s32)mot + 0x10);
        }
    } else if (state == 13) {
        SPEED -= ACCEL;
        if (SPEED <= float_0_80420bd4) {
            SPEED = float_0_80420bd4;
            STATE = 14;
            ACCEL -= *(f32*)((s32)mot + 0x10);
            if (__fabs(ROT) < float_0p8_80420be4) {
                STATE = 0;
            }
        }
        ROT = (f32)revise360(ROT - SPEED);
    } else if (state == 15) {
        SPEED += ACCEL;
        if (SPEED >= float_0_80420bd4) {
            SPEED = float_0_80420bd4;
            STATE = 12;
            ACCEL -= *(f32*)((s32)mot + 0x10);
            if (__fabs(ROT) < float_0p8_80420be4) {
                STATE = 10;
            }
        }
        ROT = (f32)revise360(ROT - SPEED);
    }

#undef STATE
#undef TIMER
#undef ACCEL
#undef SPEED
#undef ROT
    return 0;
}

void mot_grasp(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        *(u32*)mario &= ~0xF0000;
    }
}

