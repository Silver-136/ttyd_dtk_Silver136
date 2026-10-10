#include "driver/npcdrv.h"
#include "driver/dispdrv.h"
#include "driver/seqdrv.h"

#include "battle/battle_information.h"
#include "mario/mariost.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr.h"
#include "effect/eff_majinai.h"
#include "motion/mot_jabara.h"
#include "party/party_nokonoko.h"
#include "party/party_vivian.h"

// This is only here to make things easier to reference
typedef struct NPCWork2 {
    NPCWork field;  //0x0
    NPCWork battle; //0x14
} NPCWork2;

static NPCWork2 work;
void* release_wp;
s32 npcMainCount;
s32 dbg_battleskip;
char MarioStayPose[0x20] = "KPA_S_1";
char MarioTalkPose[0x20] = "KPA_T_1";

extern s32 strcmp(const char* a, const char* b);
extern void* animPoseGetAnimPosePtr(s32 poseId);
extern s32 animPoseEntry(void* name, s32 mode);
extern void animPoseSetAnim(s32 poseId, void* name, s32 force);
extern void animPoseSetLocalTimeRate(s32 poseId, f32 rate);
extern void* marioGetPtr(void);
extern void* camGetPtr(s32 cameraId);
extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
extern void animPoseMain(s32 poseId);
extern void animPoseDrawMtx(s32 poseId, f32 (*mtx)[4], s32 mode, f32 x, f32 y);
extern char* animPoseGetCurrentAnim(s32 poseId);
void mtx_setup(void* npc, f32 (*mtx)[4], s32 historyIndex);
extern f64 atan2(f64 y, f64 x);
extern f64 sqrt(f64 value);
extern f64 __frsqrte(f64 value);
extern void sincosf(f32 angle, f32* outSin, f32* outCos);
extern char* strcpy(char* dest, const char* src);
extern void animPoseRelease(s32 poseId);
extern s32 irand(s32 max);
extern f32 PSVECDistance(void* a, void* b);
extern void offscreenAddBoundingBox(s32 id, u16 left, u16 top, u16 right, u16 bottom);
extern void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom);
extern void animPoseSetMaterialFlagOff(s32 poseId, u32 flag);
extern void PSMTXTransApply(f32 (*src)[4], f32 (*dst)[4], f32 x, f32 y, f32 z);
extern void GXClearBoundingBox(void);
extern void sysWaitDrawSync(void);
extern void evtStartID(s32 id);
extern void evtStopID(s32 id);
extern void npcGroupDead(void* npc, s32 scoreType);
extern void marioKeyOff(void);
extern void itemFlagOn(void* item, u16 flags);
extern s32 itemEntry(s32 name, s32 itemId, s32 mode, s32 collectExpr, s32 script, f32 x, f32 y, f32 z);
extern void marioKeyOn(void);
extern s32 marioChkTalkable(void);
extern f32 cloudGetBreathPower(f32 width, void* position);
extern void kpaAddScorePos(s32 score, void* position);
extern void psndSFXOn_3D(char* id, void* pos);
extern void* evtGetPtrID(s32 id);
extern void partyChgPoseId(void* party, s32 poseId);
extern void marioChgPose(char* poseName);
extern s32 marioGetPartyId(void);
extern s32 marioGetExtraPartyId(void);
extern void* partyGetPtr(s32 partyId);

/* Data-pass A1: exact external identities used by restored npcdrv initialized data. */
__declspec(section ".sdata2") char str_Z_1_8041fbb8[4] __attribute__ ((aligned(4))) = "Z_1";
__declspec(section ".sdata2") char str_Z_1R_8041fbbc[5] __attribute__ ((aligned(4))) = "Z_1R";
__declspec(section ".sdata2") char str_S_1_8041fbc4[4] __attribute__ ((aligned(4))) = "S_1";
__declspec(section ".sdata2") char str_S_1R_8041fbc8[5] __attribute__ ((aligned(4))) = "S_1R";
__declspec(section ".sdata2") char str_W_1_8041fbd0[4] __attribute__ ((aligned(4))) = "W_1";
__declspec(section ".sdata2") char str_W_1R_8041fbd4[5] __attribute__ ((aligned(4))) = "W_1R";
__declspec(section ".sdata2") char str_R_1_8041fbdc[4] __attribute__ ((aligned(4))) = "R_1";
__declspec(section ".sdata2") char str_R_1R_8041fbe0[5] __attribute__ ((aligned(4))) = "R_1R";
__declspec(section ".sdata2") char str_T_1_8041fbe8[4] __attribute__ ((aligned(4))) = "T_1";
__declspec(section ".sdata2") char str_T_1R_8041fbec[5] __attribute__ ((aligned(4))) = "T_1R";
__declspec(section ".sdata2") char str_J_1A_8041fbf4[5] __attribute__ ((aligned(4))) = "J_1A";
__declspec(section ".sdata2") char str_J_1RR_8041fbfc[6] __attribute__ ((aligned(4))) = "J_1RR";
__declspec(section ".sdata2") char str_J_1B_8041fc04[5] __attribute__ ((aligned(4))) = "J_1B";
__declspec(section ".sdata2") char str_J_1BR_8041fc0c[6] __attribute__ ((aligned(4))) = "J_1BR";
__declspec(section ".sdata2") char str_Z_2_8041fc14[4] __attribute__ ((aligned(4))) = "Z_2";
__declspec(section ".sdata2") char str_Z_2R_8041fc18[5] __attribute__ ((aligned(4))) = "Z_2R";
__declspec(section ".sdata2") char str_S_2_8041fc20[4] __attribute__ ((aligned(4))) = "S_2";
__declspec(section ".sdata2") char str_S_2R_8041fc24[5] __attribute__ ((aligned(4))) = "S_2R";
__declspec(section ".sdata2") char str_T_2_8041fc2c[4] __attribute__ ((aligned(4))) = "T_2";
__declspec(section ".sdata2") char str_T_2R_8041fc30[5] __attribute__ ((aligned(4))) = "T_2R";
__declspec(section ".sdata2") char str_L_1_8041fc38[4] __attribute__ ((aligned(4))) = "L_1";
__declspec(section ".sdata2") char str_L_1R_8041fc3c[5] __attribute__ ((aligned(4))) = "L_1R";
__declspec(section ".sdata2") char str_L_2_8041fc44[4] __attribute__ ((aligned(4))) = "L_2";
__declspec(section ".sdata2") char str_L_2R_8041fc48[5] __attribute__ ((aligned(4))) = "L_2R";
__declspec(section ".sdata2") char str_L_3_8041fc50[4] __attribute__ ((aligned(4))) = "L_3";
__declspec(section ".sdata2") char str_L_3R_8041fc54[5] __attribute__ ((aligned(4))) = "L_3R";
__declspec(section ".sdata2") char str_I_1_8041fc5c[4] __attribute__ ((aligned(4))) = "I_1";
__declspec(section ".sdata2") char str_I_1R_8041fc60[5] __attribute__ ((aligned(4))) = "I_1R";
__declspec(section ".sdata2") s32 dat_8041fc68 = -256;
__declspec(section ".sdata2") s32 dat_8041fc6c = -1;
__declspec(section ".sdata2") s32 dat_8041fc70 = -8355585;      /* 0xFF8080FF */
__declspec(section ".sdata2") s32 dat_8041fc74 = -2139029505;  /* 0x8080FFFF */
__declspec(section ".sdata2") u32 dat_8041fc78 = 0xFFFFFFFF;

extern char str_SFX_EVT_BTL_INCANT_C_802c12e4[];
extern char str_SFX_EVT_BTL_INCANT_C_802c1300[];
extern char str_SFX_EVT_BTL_INCANT_C_802c131c[];
extern char str_SFX_EVT_BTL_INCANT_C_802c1338[];
extern char str_SFX_EVT_BTL_INCANT_C_802c1354[];
extern char str_SFX_EVT_FLD_INCANT_C_802c1258[];
extern char str_SFX_EVT_FLD_INCANT_C_802c1274[];
extern char str_SFX_EVT_FLD_INCANT_C_802c1290[];
extern char str_SFX_EVT_FLD_INCANT_C_802c12ac[];
extern char str_SFX_EVT_FLD_INCANT_C_802c12c8[];
extern char str_msg_mj_coin_up_802c1370[];
extern char str_msg_mj_last_802c1380[];
extern s32 btlevtcmd_ftomsec(EventEntry* event, BOOL isFirstCall);
extern s32 evt_mario_get_pos(EventEntry* event, BOOL isFirstCall);
extern s32 evt_msg_print(EventEntry* event, BOOL isFirstCall);
extern s32 evt_snd_sfxon_3d(EventEntry* event, BOOL isFirstCall);

NPCWork* npcGetWorkPtr(void) {
    return gp->inBattle ? &work.battle : &work.field;
}


u8 npcReleaseFiledNpc(void) {
    extern void* release_wp;
    extern void* animPoseGetAnimBaseDataPtr(s32);
    s32 wp = (s32)&work;
    char* release = release_wp;
    void* npc;
    void* pose;
    s32 i;

    if ((*(u32*)(wp + 8) & 2) != 0) {
        npc = *(void**)(wp + 0xC);
        i = 0;
        while (i < *(s32*)(wp + 4)) {
            if ((*(u32*)npc & 1) && (*(u32*)npc & 2)) {
                pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
                if (*(s32*)((s32)pose + 0x90) == -1 && *(s32*)((s32)npc + 0x104) >= 0) {
                    pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
                    *(s32*)(release + 0x80) = *(s32*)pose;
                    *(f32*)(release + 0x84) = *(f32*)((s32)pose + 0x70);
                    *(f32*)(release + 0x88) = *(f32*)((s32)pose + 0x74);
                    *(f32*)(release + 0x8C) = *(f32*)((s32)pose + 0x78);
                    *(f32*)(release + 0x90) = *(f32*)((s32)pose + 0x7C);
                    strcpy(release, (char*)animPoseGetAnimBaseDataPtr(*(s32*)((s32)npc + 0x104)) + 4);
                    strcpy(release + 0x40, animPoseGetCurrentAnim(*(s32*)((s32)npc + 0x104)));
                    animPoseRelease(*(s32*)((s32)npc + 0x104));
                    release += 0x94;
                    *(s32*)((s32)npc + 0x104) = -1;
                }
            }
            i++;
            npc = (void*)((s32)npc + 0x340);
        }
    }
}

u8 npcRecoveryFiledNpc(void) {
    extern void* release_wp;
    extern void animPoseWorldPositionEvalOn(s32);
    extern void animPoseWorldMatrixEvalOn(s32);
    s32 wp = (s32)&work;
    char* release = release_wp;
    void* npc;
    void* pose;
    s32 i;

    if ((*(u32*)(wp + 8) & 2) != 0) {
        npc = *(void**)(wp + 0xC);
        i = 0;
        while (i < *(s32*)(wp + 4)) {
            if ((*(u32*)npc & 1) && (*(u32*)npc & 2) && *(s32*)((s32)npc + 0x104) == -1) {
                *(s32*)((s32)npc + 0x104) = animPoseEntry(release, 0);
                animPoseSetAnim(*(s32*)((s32)npc + 0x104), release + 0x40, 1);
                pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
                *(s32*)pose = *(s32*)(release + 0x80);
                *(f32*)((s32)pose + 0x70) = *(f32*)(release + 0x84);
                *(f32*)((s32)pose + 0x74) = *(f32*)(release + 0x88);
                *(f32*)((s32)pose + 0x78) = *(f32*)(release + 0x8C);
                *(f32*)((s32)pose + 0x7C) = *(f32*)(release + 0x90);
                if ((*(u32*)pose & 0x40) != 0) {
                    *(u32*)pose &= ~0x40;
                    animPoseWorldPositionEvalOn(*(s32*)((s32)npc + 0x104));
                }
                if ((*(u32*)pose & 0x80) != 0) {
                    *(u32*)pose &= ~0x80;
                    animPoseWorldMatrixEvalOn(*(s32*)((s32)npc + 0x104));
                }
                animPoseMain(*(s32*)((s32)npc + 0x104));
                release += 0x94;
            }
            i++;
            npc = (void*)((s32)npc + 0x340);
        }
        *(u32*)(wp + 8) &= ~2;
    }
}

void mtx_setup(void* npc, f32 mtx[3][4], s32 historyIndex) {
    extern void* camGetCurPtr(void);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 reviseAngle(f32 angle);
    extern void PSMTXIdentity(f32 mtx[3][4]);
    extern void PSMTXScaleApply(f32 src[3][4], f32 dst[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 c[3][4]);
    extern f32 float_0_8041fc7c;
    extern f32 float_1_8041fc94;
    extern f32 float_90_8041fcb0;
    extern f32 float_270_8041fcc4;
    extern f32 float_neg1_8041fcd8;
    extern f32 float_deg2rad_8041fd7c;
    f32 xRot[3][4];
    f32 yRot[3][4];
    f32 zRot[3][4];
    f32 camAngle;
    f32 scaleSign;
    f32 angle;
    f32 one;
    void* cam;
    u32 flags;

    camAngle = float_0_8041fc7c;
    scaleSign = float_1_8041fc94;
    flags = *(u32*)npc;
    if (!(flags & 0x2000000) && !(flags & 0x8000000)) {
        cam = camGetCurPtr();
        camAngle = angleABf(
            *(f32*)((s32)cam + 0xC),
            *(f32*)((s32)cam + 0x14),
            *(f32*)((s32)cam + 0x18),
            *(f32*)((s32)cam + 0x20));
        angle = angleABf(
            *(f32*)((s32)cam + 0xC),
            *(f32*)((s32)cam + 0x14),
            *(f32*)((s32)npc + 0x8C),
            *(f32*)((s32)npc + 0x94));
        camAngle = reviseAngle(camAngle - angle);
    }

    angle = reviseAngle(*(f32*)((s32)npc + 0xF0));
    angle = reviseAngle(angle);
    if (angle >= float_90_8041fcb0 && angle <= float_270_8041fcc4) {
        scaleSign *= float_neg1_8041fcd8;
    }

    PSMTXIdentity(mtx);
    one = float_1_8041fc94;
    if (one != *(f32*)((s32)npc + 0xE0) || one != *(f32*)((s32)npc + 0xE4) ||
        one != *(f32*)((s32)npc + 0xE8) || one != scaleSign) {
        PSMTXScaleApply(
            mtx,
            mtx,
            *(f32*)((s32)npc + 0xE0),
            *(f32*)((s32)npc + 0xE4),
            *(f32*)((s32)npc + 0xE8) * scaleSign);
    }

    if (*(f32*)((s32)npc + 0xF8) != float_0_8041fc7c ||
        *(f32*)((s32)npc + 0xFC) != float_0_8041fc7c ||
        *(f32*)((s32)npc + 0x100) != float_0_8041fc7c) {
        PSMTXTransApply(
            mtx,
            mtx,
            -*(f32*)((s32)npc + 0xF8),
            -*(f32*)((s32)npc + 0xFC),
            -*(f32*)((s32)npc + 0x100));
    }

    if (*(f32*)((s32)npc + 0xF4) != float_0_8041fc7c) {
        PSMTXRotRad(zRot, 0x7A, float_deg2rad_8041fd7c * *(f32*)((s32)npc + 0xF4));
        PSMTXConcat(zRot, mtx, mtx);
    }
    if (*(f32*)((s32)npc + 0xEC) != float_0_8041fc7c) {
        PSMTXRotRad(xRot, 0x78, float_deg2rad_8041fd7c * *(f32*)((s32)npc + 0xEC));
        PSMTXConcat(xRot, mtx, mtx);
    }
    if (*(f32*)((s32)npc + 0xF0) != float_0_8041fc7c || camAngle != float_0_8041fc7c) {
        PSMTXRotRad(yRot, 0x79, float_deg2rad_8041fd7c * (*(f32*)((s32)npc + 0xF0) + camAngle));
        PSMTXConcat(yRot, mtx, mtx);
    }

    if (*(f32*)((s32)npc + 0xF8) != float_0_8041fc7c ||
        *(f32*)((s32)npc + 0xFC) != float_0_8041fc7c ||
        *(f32*)((s32)npc + 0x100) != float_0_8041fc7c) {
        PSMTXTransApply(
            mtx,
            mtx,
            *(f32*)((s32)npc + 0xF8),
            *(f32*)((s32)npc + 0xFC),
            *(f32*)((s32)npc + 0x100));
    }

    if (historyIndex == -1) {
        PSMTXTransApply(
            mtx,
            mtx,
            *(f32*)((s32)npc + 0x8C),
            *(f32*)((s32)npc + 0x90),
            *(f32*)((s32)npc + 0x94));
    } else {
        void* history = (void*)((s32)npc + historyIndex * 0xC);
        PSMTXTransApply(
            mtx,
            mtx,
            *(f32*)((s32)history + 0xA4),
            *(f32*)((s32)history + 0xA8),
            *(f32*)((s32)history + 0xAC));
    }
}

void npcDisp_xlu(s32 cameraId, void* npc) {
    extern f32 reviseAngle(f32);
    extern const f32 float_0_8041fc7c;
    extern const f32 float_360_8041fcc0;
    extern const f32 float_rad2deg_8041fcbc;
    extern const f32 float_90_8041fcb0;
    extern const f32 float_270_8041fcc4;
    extern const f32 float_neg90_8041fcb8;
    extern const f32 float_neg270_8041fcc8;
    extern const f32 float_2_8041fd34;
    extern char* luigi_pose[];
    f32 mtx[3][4];
    s32 color;
    s32 color2;
    f32 rot;
    f32 angle;
    void* pose;
    u32 i;
    char* anim;
    char** poses;
    void* cam;
    s32 r;
    s32 g;
    s32 b;

    rot = reviseAngle(-*(f32*)((s32)npc + 0x144));
    mtx_setup(npc, mtx, -1);
    color = *(s32*)((s32)npc + 0x114);
    if ((*(u32*)((s32)npc + 0x1D4) & 0x400000) != 0) {
        r = *(u8*)((s32)&color + 0);
        g = *(u8*)((s32)&color + 1);
        b = *(u8*)((s32)&color + 2);
        *(u8*)((s32)&color + 0) = (s32)(r + ((u32)r >> 31)) >> 1;
        *(u8*)((s32)&color + 1) = (s32)(g + ((u32)g >> 31)) >> 1;
        *(u8*)((s32)&color + 2) = (s32)(b + ((u32)b >> 31)) >> 1;
        color2 = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)npc + 0x104), &color2);
        animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), 0x40);
    }
    if ((*(u32*)npc & 0x800000) != 0) {
        animPoseGetCurrentAnim(*(s32*)((s32)npc + 0x104));
        pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
        anim = animPoseGetCurrentAnim(*(s32*)((s32)npc + 0x104));
        cam = camGetPtr(4);
        angle = atan2(-(*(f32*)((s32)cam + 0x18) - *(f32*)((s32)cam + 0xC)),
                      -(*(f32*)((s32)cam + 0x20) - *(f32*)((s32)cam + 0x14)));
        angle = reviseAngle(float_rad2deg_8041fcbc * angle);
        if (angle < float_0_8041fc7c) {
            angle += float_360_8041fcc0;
        }
        angle = reviseAngle(angle - *(f32*)((s32)pose + 0x74));
        if ((angle > float_90_8041fcb0 && angle <= float_270_8041fcc4) ||
            (angle < float_neg90_8041fcb8 && angle >= float_neg270_8041fcc8)) {
            i = 0;
            poses = luigi_pose;
            do {
                if (strcmp(poses[0], anim) == 0) {
                    break;
                }
                i++;
                poses += 2;
            } while (i < 0xE);
            if (i < 0xE) {
                animPoseSetAnim(*(s32*)((s32)npc + 0x104), luigi_pose[i * 2 + 1], 0);
            }
        } else {
            i = 0;
            poses = luigi_pose;
            do {
                if (strcmp(poses[1], anim) == 0) {
                    break;
                }
                i++;
                poses += 2;
            } while (i < 0xE);
            if (i < 0xE) {
                animPoseSetAnim(*(s32*)((s32)npc + 0x104), luigi_pose[i * 2], 0);
            }
        }
    }
    animPoseMain(*(s32*)((s32)npc + 0x104));
    animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 1, rot, float_2_8041fd34);
    animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 2, rot, float_2_8041fd34);
    animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 3, rot, float_2_8041fd34);
}

void npcDisp(s32 cameraId, void* npc) {
    extern f32 reviseAngle(f32);
    extern const f32 float_0_8041fc7c;
    extern const f32 float_360_8041fcc0;
    extern const f32 float_rad2deg_8041fcbc;
    extern const f32 float_90_8041fcb0;
    extern const f32 float_270_8041fcc4;
    extern const f32 float_neg90_8041fcb8;
    extern const f32 float_neg270_8041fcc8;
    extern const f32 float_2_8041fd34;
    extern char* luigi_pose[];
    f32 mtx[3][4];
    s32 color;
    s32 color2;
    f32 rot;
    f32 angle;
    void* pose;
    u32 i;
    char* anim;
    char** poses;
    void* cam;
    void* disp;
    s32 r;
    s32 g;
    s32 b;
    s32 mode;

    rot = reviseAngle(-*(f32*)((s32)npc + 0x144));
    mtx_setup(npc, mtx, -1);
    color = *(s32*)((s32)npc + 0x114);
    if ((*(u32*)((s32)npc + 0x1D4) & 0x400000) != 0) {
        r = *(u8*)((s32)&color + 0);
        g = *(u8*)((s32)&color + 1);
        b = *(u8*)((s32)&color + 2);
        *(u8*)((s32)&color + 0) = (s32)(r + ((u32)r >> 31)) >> 1;
        *(u8*)((s32)&color + 1) = (s32)(g + ((u32)g >> 31)) >> 1;
        *(u8*)((s32)&color + 2) = (s32)(b + ((u32)b >> 31)) >> 1;
        color2 = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)npc + 0x104), &color2);
        animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), 0x40);
    }

    disp = dispGetCurWork();
    mode = *(u8*)((s32)disp + 1);
    switch (mode) {
        case 0:
            if ((*(u32*)npc & 0x800000) != 0) {
            animPoseGetCurrentAnim(*(s32*)((s32)npc + 0x104));
            pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
            anim = animPoseGetCurrentAnim(*(s32*)((s32)npc + 0x104));
            cam = camGetPtr(4);
            angle = atan2(-(*(f32*)((s32)cam + 0x18) - *(f32*)((s32)cam + 0xC)),
                          -(*(f32*)((s32)cam + 0x20) - *(f32*)((s32)cam + 0x14)));
            angle = reviseAngle(float_rad2deg_8041fcbc * angle);
            if (angle < float_0_8041fc7c) {
                angle += float_360_8041fcc0;
            }
            angle = reviseAngle(angle - *(f32*)((s32)pose + 0x74));
            if ((angle > float_90_8041fcb0 && angle <= float_270_8041fcc4) ||
                (angle < float_neg90_8041fcb8 && angle >= float_neg270_8041fcc8)) {
                i = 0;
                poses = luigi_pose;
                do {
                    if (strcmp(poses[0], anim) == 0) {
                        break;
                    }
                    i++;
                    poses += 2;
                } while (i < 0xE);
                if (i < 0xE) {
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), luigi_pose[i * 2 + 1], 0);
                }
            } else {
                i = 0;
                poses = luigi_pose;
                do {
                    if (strcmp(poses[1], anim) == 0) {
                        break;
                    }
                    i++;
                    poses += 2;
                } while (i < 0xE);
                if (i < 0xE) {
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), luigi_pose[i * 2], 0);
                }
            }
            }
            animPoseMain(*(s32*)((s32)npc + 0x104));
            animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 1, rot, float_2_8041fd34);
            break;
        case 1:
            animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 2, rot, float_2_8041fd34);
            break;
        case 2:
            animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 3, rot, float_2_8041fd34);
            break;
    }
}

void npcDisp_offscreen_xlu(s32 cameraId, void* npc) {
    extern f32 reviseAngle(f32);
    extern const f32 float_0_8041fc7c;
    extern const f32 float_360_8041fcc0;
    extern const f32 float_rad2deg_8041fcbc;
    extern const f32 float_90_8041fcb0;
    extern const f32 float_270_8041fcc4;
    extern const f32 float_neg90_8041fcb8;
    extern const f32 float_neg270_8041fcc8;
    extern const f32 float_2_8041fd34;
    extern char* luigi_pose[];
    f32 mtx[3][4];
    s32 color2;
    s32 color;
    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    f32 rot;
    f32 angle;
    void* pose;
    u32 i;
    char* anim;
    char** poses;
    void* cam;
    s32 r;
    s32 g;
    s32 b;

    sysWaitDrawSync();
    GXClearBoundingBox();
    rot = reviseAngle(-*(f32*)((s32)npc + 0x144));
    mtx_setup(npc, mtx, -1);
    color = *(s32*)((s32)npc + 0x114);
    if ((*(u32*)((s32)npc + 0x1D4) & 0x400000) != 0) {
        r = *(u8*)((s32)&color + 0);
        g = *(u8*)((s32)&color + 1);
        b = *(u8*)((s32)&color + 2);
        *(u8*)((s32)&color + 0) = (s32)(r + ((u32)r >> 31)) >> 1;
        *(u8*)((s32)&color + 1) = (s32)(g + ((u32)g >> 31)) >> 1;
        *(u8*)((s32)&color + 2) = (s32)(b + ((u32)b >> 31)) >> 1;
        color2 = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)npc + 0x104), &color2);
        animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), 0x40);
    }

    if ((*(u32*)npc & 0x800000) != 0) {
        animPoseGetCurrentAnim(*(s32*)((s32)npc + 0x104));
        pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
        anim = animPoseGetCurrentAnim(*(s32*)((s32)npc + 0x104));
        cam = camGetPtr(4);
        angle = atan2(-(*(f32*)((s32)cam + 0x18) - *(f32*)((s32)cam + 0xC)),
                      -(*(f32*)((s32)cam + 0x20) - *(f32*)((s32)cam + 0x14)));
        angle = reviseAngle(float_rad2deg_8041fcbc * angle);
        if (angle < float_0_8041fc7c) {
            angle += float_360_8041fcc0;
        }
        angle = reviseAngle(angle - *(f32*)((s32)pose + 0x74));
        if ((angle > float_90_8041fcb0 && angle <= float_270_8041fcc4) ||
            (angle < float_neg90_8041fcb8 && angle >= float_neg270_8041fcc8)) {
            i = 0;
            poses = luigi_pose;
            do {
                if (strcmp(poses[0], anim) == 0) {
                    break;
                }
                i++;
                poses += 2;
            } while (i < 0xE);
            if (i < 0xE) {
                animPoseSetAnim(*(s32*)((s32)npc + 0x104), luigi_pose[i * 2 + 1], 0);
            }
        } else {
            i = 0;
            poses = luigi_pose;
            do {
                if (strcmp(poses[1], anim) == 0) {
                    break;
                }
                i++;
                poses += 2;
            } while (i < 0xE);
            if (i < 0xE) {
                animPoseSetAnim(*(s32*)((s32)npc + 0x104), luigi_pose[i * 2], 0);
            }
        }
    }

    animPoseMain(*(s32*)((s32)npc + 0x104));
    animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 1, rot, float_2_8041fd34);
    animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 2, rot, float_2_8041fd34);
    animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 3, rot, float_2_8041fd34);
    sysWaitDrawSync();
    GXReadBoundingBox(&x0, &y0, &x1, &y1);
    offscreenAddBoundingBox(*(s32*)((s32)npc + 0x110), x0, y0, x1, y1);
}

void npcDisp_offscreen(s32 cameraId, void* npc) {
    extern f32 reviseAngle(f32);
    extern const f32 float_0_8041fc7c;
    extern const f32 float_360_8041fcc0;
    extern const f32 float_rad2deg_8041fcbc;
    extern const f32 float_90_8041fcb0;
    extern const f32 float_270_8041fcc4;
    extern const f32 float_neg90_8041fcb8;
    extern const f32 float_neg270_8041fcc8;
    extern const f32 float_2_8041fd34;
    extern char* luigi_pose[];
    f32 mtx[3][4];
    s32 color;
    s32 color2;
    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    f32 rot;
    f32 angle;
    void* pose;
    u32 i;
    char* anim;
    char** poses;
    void* cam;
    void* disp;
    s32 r;
    s32 g;
    s32 b;

    sysWaitDrawSync();
    GXClearBoundingBox();
    rot = reviseAngle(-*(f32*)((s32)npc + 0x144));
    mtx_setup(npc, mtx, -1);
    color = *(s32*)((s32)npc + 0x114);
    if ((*(u32*)((s32)npc + 0x1D4) & 0x400000) != 0) {
        r = *(u8*)((s32)&color + 0);
        g = *(u8*)((s32)&color + 1);
        b = *(u8*)((s32)&color + 2);
        *(u8*)((s32)&color + 0) = (s32)(r + ((u32)r >> 31)) >> 1;
        *(u8*)((s32)&color + 1) = (s32)(g + ((u32)g >> 31)) >> 1;
        *(u8*)((s32)&color + 2) = (s32)(b + ((u32)b >> 31)) >> 1;
        color2 = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)npc + 0x104), &color2);
        animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), 0x40);
    }

    disp = dispGetCurWork();
    switch (*(u8*)((s32)disp + 1)) {
    case 0:
        if ((*(u32*)npc & 0x800000) != 0) {
            animPoseGetCurrentAnim(*(s32*)((s32)npc + 0x104));
            pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
            anim = animPoseGetCurrentAnim(*(s32*)((s32)npc + 0x104));
            cam = camGetPtr(4);
            angle = atan2(-(*(f32*)((s32)cam + 0x18) - *(f32*)((s32)cam + 0xC)),
                          -(*(f32*)((s32)cam + 0x20) - *(f32*)((s32)cam + 0x14)));
            angle = reviseAngle(float_rad2deg_8041fcbc * angle);
            if (angle < float_0_8041fc7c) {
                angle += float_360_8041fcc0;
            }
            angle = reviseAngle(angle - *(f32*)((s32)pose + 0x74));
            if ((angle > float_90_8041fcb0 && angle <= float_270_8041fcc4) ||
                (angle < float_neg90_8041fcb8 && angle >= float_neg270_8041fcc8)) {
                i = 0;
                poses = luigi_pose;
                do {
                    if (strcmp(poses[0], anim) == 0) {
                        break;
                    }
                    i++;
                    poses += 2;
                } while (i < 0xE);
                if (i < 0xE) {
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), luigi_pose[i * 2 + 1], 0);
                }
            } else {
                i = 0;
                poses = luigi_pose;
                do {
                    if (strcmp(poses[1], anim) == 0) {
                        break;
                    }
                    i++;
                    poses += 2;
                } while (i < 0xE);
                if (i < 0xE) {
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), luigi_pose[i * 2], 0);
                }
            }
        }
        animPoseMain(*(s32*)((s32)npc + 0x104));
        animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 1, rot, float_2_8041fd34);
        break;
    case 1:
        animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 2, rot, float_2_8041fd34);
        break;
    case 2:
        animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 3, rot, float_2_8041fd34);
        break;
    }
    sysWaitDrawSync();
    GXReadBoundingBox(&x0, &y0, &x1, &y1);
    offscreenAddBoundingBox(*(s32*)((s32)npc + 0x110), x0, y0, x1, y1);
}

void npcDisp_blur(s32 cameraId, void* npc) {
    extern f32 reviseAngle(f32);
    extern u32 animPoseGetMaterialFlag(s32);
    extern const f32 float_6p2832_8041fd10;
    extern const f32 float_360_8041fcc0;
    extern const f32 float_3p1416_8041fd1c;
    extern const f32 float_1p5708_8041fd20;
    extern const f32 float_4p7124_8041fd24;
    extern const f32 float_0p00761_8041fd14;
    extern const f32 float_0p16605_8041fd18;
    extern const f32 float_0p03705_8041fd28;
    extern const f32 float_0p4967_8041fd2c;
    extern const f32 float_2_8041fd34;
    extern const f32 float_1_8041fc94;
    extern const f32 float_0_8041fc7c;
    extern s32 dat_8041fc68;
    f32 mtx[3][4];
    f32 rot;
    f32 t;
    f32 x;
    f32 z;
    f32 sq;
    f32 c0;
    f32 c1;
    f32 one;
    s32 poseId;
    u32 flags;
    s32 i;
    s32 alpha;
    s32 color;
    s32 color2;
    void* cam;

    rot = reviseAngle(-*(f32*)((s32)npc + 0x144));
    poseId = *(s32*)((s32)npc + 0x104);
    flags = animPoseGetMaterialFlag(poseId);
    cam = camGetPtr(cameraId);
    t = (float_6p2832_8041fd10 * *(f32*)((s32)cam + 0x114)) / float_360_8041fcc0;
    c0 = float_0p00761_8041fd14;
    c1 = float_0p16605_8041fd18;
    one = float_1_8041fc94;
    if (t <= float_3p1416_8041fd1c) {
        if (t <= float_1p5708_8041fd20) {
            sq = t * t;
            x = ((c0 * sq) - c1) * sq + one;
            x *= t;
        } else {
            t = float_1p5708_8041fd20 - (t - float_1p5708_8041fd20);
            sq = t * t;
            x = ((c0 * sq) - c1) * sq + one;
            x *= t;
        }
    } else if (t < float_4p7124_8041fd24) {
        t -= float_3p1416_8041fd1c;
        sq = t * t;
        x = ((c0 * sq) - c1) * sq + one;
        x *= t;
        x = -x;
    } else {
        t = float_1p5708_8041fd20 - (t - float_4p7124_8041fd24);
        sq = t * t;
        x = ((c0 * sq) - c1) * sq + one;
        x *= t;
        x = -x;
    }
    x *= float_2_8041fd34;

    cam = camGetPtr(cameraId);
    t = (float_6p2832_8041fd10 * *(f32*)((s32)cam + 0x114)) / float_360_8041fcc0;
    c0 = float_0p03705_8041fd28;
    c1 = float_0p4967_8041fd2c;
    one = float_1_8041fc94;
    if (t <= float_3p1416_8041fd1c) {
        if (t <= float_1p5708_8041fd20) {
            sq = t * t;
            z = ((c0 * sq) - c1) * sq + one;
        } else {
            t = float_1p5708_8041fd20 - (t - float_1p5708_8041fd20);
            sq = t * t;
            z = ((c0 * sq) - c1) * sq + one;
            z = -z;
        }
    } else if (t < float_4p7124_8041fd24) {
        t -= float_3p1416_8041fd1c;
        sq = t * t;
        z = ((c0 * sq) - c1) * sq + one;
        z = -z;
    } else {
        t = float_1p5708_8041fd20 - (t - float_4p7124_8041fd24);
        sq = t * t;
        z = ((c0 * sq) - c1) * sq + one;
    }
    z = -z * float_2_8041fd34;

    animPoseMain(*(s32*)((s32)npc + 0x104));
    i = 4;
    alpha = 0x64;
    do {
        mtx_setup(npc, mtx, i);
        PSMTXTransApply(mtx, mtx, x, float_0_8041fc7c, z);
        animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), 0x40);
        color = dat_8041fc68;
        *(u8*)((s32)&color + 3) = 0x80 - alpha;
        color2 = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)npc + 0x104), &color2);
        animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 1, rot, float_2_8041fd34);
        animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 2, rot, float_2_8041fd34);
        animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 3, rot, float_2_8041fd34);
        i--;
        alpha -= 0x19;
    } while (i > 0);
    animPoseSetMaterialFlagOff(*(s32*)((s32)npc + 0x104), 0x40);
    animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), flags);
}
void npcInit(void) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* release_wp;
    u8* field = (u8*)&work;
    void* fbat;

    *(u32*)(field + 4) = 0x24;
    *(void**)((u8*)&work + 0xC) = __memAlloc(0, *(u32*)(field + 4) * 0x340);
    memset(*(void**)((u8*)&work + 0xC), 0, *(u32*)(field + 4) * 0x340);
    release_wp = __memAlloc(0, *(u32*)(field + 4) * 0x94);

    *(u32*)((u8*)&work + 0x18) = 0x10;
    *(void**)((u8*)&work + 0x20) =
        __memAlloc(0, *(u32*)((u8*)&work + 0x18) * 0x340);
    memset(*(void**)((u8*)&work + 0x20), 0,
           *(u32*)((u8*)&work + 0x18) * 0x340);

    fbat = __memAlloc(0, 0x580);
    *(void**)((u8*)gp + 0x168) = fbat;
    memset(*(void**)((u8*)gp + 0x168), 0, 0x580);
    *(s16*)fbat = 0;
    npcMainCount = 0;
}

void npcReset(s32 battle) {
    extern s32 npcMainCount;
    void* wp = &work.field;

    if (battle != 0) {
        wp = (void*)((s32)wp + 0x14);
    }
    memset(*(void**)((s32)wp + 0xC), 0, *(s32*)((s32)wp + 4) * 0x340);
    *(s32*)wp = 0;
    if (battle == 0) {
        *(s16*)*(void**)((s32)gp + 0x168) = 0;
    }
    npcMainCount = 0;
}

s32 npcGetReactionOfLivingBody(s32 battle) {
    void* ptr = &work.field;
    if (battle) {
        ptr = (void*)((s32)ptr + 0x14);
    }
    return *(s32*)ptr;
}

s32 npcEntry(char* name, char* modelName) {
    extern void animPosePeraOn(s32);
    extern f32 animPoseGetHeight(s32);
    extern f32 animPoseGetRadius(s32);
    extern s32 animPoseGetVivianType(s32);
    extern void animPoseSetMaterialLightFlagOn(s32, u32);
    extern s32 vec3_802c11f8[];
    extern s32 vec3_802c1204[];
    extern char str_hoshi_8041fd6c;
    extern char str_M_I_2_8041fd74;
    extern char str_c_luigi_802c1468[];
    extern const f32 float_1_8041fc94;
    extern const f32 float_0_8041fc7c;
    extern s32 dat_8041fc6c;
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    s32 wp;
    s32 max;
    s32 npc;
    s32 index;
    s32 i;
    s32 data;

    data = (s32)&work;
    wp = (s32)vec3_802c11f8;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        data += 0x14;
    }

    max = *(s32*)(data + 4);
    index = 0;
    npc = *(s32*)(data + 0xC);
    while (index < max) {
        if ((*(u32*)npc & 1) != 0 && strcmp((char*)(npc + 8), name) == 0) {
            break;
        }
        index++;
        npc += 0x340;
    }

    npc = *(s32*)(data + 0xC);
    for (i = 0; i < max; i++, npc += 0x340) {
        if ((*(u32*)npc & 1) == 0) {
            break;
        }
    }

    memset((void*)npc, 0, 0x340);
    *(u32*)npc = 3;
    strcpy((char*)(npc + 8), name);
    *(s32*)(npc + 0x104) = animPoseEntry(modelName, (*(s32*)((s32)gp + 0x14) != 0));
    if (*(s32*)(npc + 0x104) == -2) {
        *(s32*)(npc + 0x104) = animPoseEntry(&str_hoshi_8041fd6c, (*(s32*)((s32)gp + 0x14) != 0));
    }
    animPosePeraOn(*(s32*)(npc + 0x104));
    *(VecLocal*)(npc + 0x8C) = *(VecLocal*)wp;
    *(VecLocal*)(npc + 0x98) = *(VecLocal*)(npc + 0x8C);
    *(f32*)(npc + 0x14C) = animPoseGetHeight(*(s32*)(npc + 0x104));
    *(f32*)(npc + 0x150) = animPoseGetRadius(*(s32*)(npc + 0x104));
    *(VecLocal*)(npc + 0xE0) = *(VecLocal*)(wp + 0xC);
    *(f32*)(npc + 0x154) = float_1_8041fc94;
    *(s32*)(npc + 0x108) = 0;
    *(char**)(npc + 0x10C) = &str_M_I_2_8041fd74;
    *(s32*)(npc + 0x19C) = *(s32*)((s32)gp + 0x3C);
    *(s32*)(npc + 0x198) = *(s32*)((s32)gp + 0x38);
    *(s32*)(npc + 0x18C) = 0;
    *(s32*)(npc + 0x188) = 0;
    *(f32*)(npc + 0x1CC) = float_1_8041fc94;
    *(f32*)(npc + 0x1D0) = float_0_8041fc7c;
    *(f32*)(npc + 0x1C8) = float_1_8041fc94;
    *(s32*)(npc + 0x110) = -1;
    *(s32*)(npc + 0x114) = dat_8041fc6c;
    *(s32*)(npc + 0x1F4) = 4;
    *(s32*)(npc + 0x2F8) = 0;
    *(u8*)(npc + 0x317) = 1;
    if (animPoseGetVivianType(*(s32*)(npc + 0x104)) != 0) {
        *(u32*)npc |= 0x4000000;
    }
    if (strcmp(modelName, (char*)(wp + 0x270)) == 0) {
        *(u32*)npc |= 0x800000;
    }
    animPoseSetMaterialLightFlagOn(*(s32*)(npc + 0x104), 2);
    *(s32*)data = *(s32*)data + 1;
    return i;
}

void* npcGetTribe(const char* name) {
    extern char npcTribe[];
    void* tribe = npcTribe;

    while (*(char**)tribe != 0) {
        if (strcmp(*(char**)tribe, name) == 0) {
            return tribe;
        }
        tribe = (void*)((s32)tribe + 0x54);
    }
    return 0;
}

void npcDelete(void* npc) {
    u8* entry = npc;
    s32 i;
    s32 wp;

    for (i = 0; i < 4; i++) {
        u8* slave = *(u8**)(entry + 0x330 + i * 4);
        if (slave != 0) {
            *(void**)(slave + 0x32C) = 0;
        }
        *(void**)(entry + 0x330 + i * 4) = 0;
    }

    if (*(u8**)(entry + 0x324) != 0) {
        if (*(u8**)(entry + 0x328) != 0) {
            *(void**)(*(u8**)(entry + 0x324) + 0x328) = *(u8**)(entry + 0x328);
            *(void**)(*(u8**)(entry + 0x328) + 0x324) = *(u8**)(entry + 0x324);
        } else {
            *(void**)(*(u8**)(entry + 0x324) + 0x328) = 0;
        }
    } else if (*(u8**)(entry + 0x328) != 0) {
        *(void**)(*(u8**)(entry + 0x328) + 0x324) = 0;
    }

    *(u32*)entry &= ~2;
    if (*(s32*)(entry + 0x104) >= 0) {
        animPoseRelease(*(s32*)(entry + 0x104));
    }
    *(s32*)(entry + 0x104) = -1;
    *(u32*)entry &= ~1;

    wp = (s32)&work;
    if (gp->inBattle != 0) wp += 0x14;
    *(s32*)wp -= 1;
}

void _npcDeleteGroup(void* npc) {
    extern void npcDelete(void* npc);
    s32 current;
    s32 child;
    s32 slave;
    s32 slave2;
    s32 workPtr;
    s32 i;

    child = *(s32*)((s32)npc + 0x328);
    if ((void*)child != 0) {
        if (*(void**)(child + 0x328) != 0) {
            _npcDeleteGroup(*(void**)(child + 0x328));
        }
        i = 0;
        current = child;
        do {
            slave = *(s32*)(current + 0x330);
            if ((void*)slave != 0) {
                npcDelete((void*)slave);
                *(s32*)(current + 0x330) = 0;
            }
            i++;
            current += 4;
        } while (i < 4);
        npcDelete((void*)child);
    }

    workPtr = (s32)&work + 0x14;
    current = (s32)npc;
    i = 0;
    do {
        slave = *(s32*)(current + 0x330);
        if ((void*)slave != 0) {
            if (*(void**)(slave + 0x330) != 0) {
                *(s32*)(*(s32*)(slave + 0x330) + 0x32C) = 0;
            }
            *(s32*)(slave + 0x330) = 0;
            if (*(void**)(slave + 0x334) != 0) {
                *(s32*)(*(s32*)(slave + 0x334) + 0x32C) = 0;
            }
            *(s32*)(slave + 0x334) = 0;
            slave2 = slave + 8;
            if (*(void**)(slave2 + 0x330) != 0) {
                *(s32*)(*(s32*)(slave2 + 0x330) + 0x32C) = 0;
            }
            *(s32*)(slave2 + 0x330) = 0;
            if (*(void**)(slave2 + 0x334) != 0) {
                *(s32*)(*(s32*)(slave2 + 0x334) + 0x32C) = 0;
            }
            *(s32*)(slave2 + 0x334) = 0;

            if (*(void**)(slave + 0x324) != 0) {
                if (*(void**)(slave + 0x328) != 0) {
                    *(s32*)(*(s32*)(slave + 0x324) + 0x328) = *(s32*)(slave + 0x328);
                    *(s32*)(*(s32*)(slave + 0x328) + 0x324) = *(s32*)(slave + 0x324);
                } else {
                    *(s32*)(*(s32*)(slave + 0x324) + 0x328) = 0;
                }
            } else {
                if (*(void**)(slave + 0x328) != 0) {
                    *(s32*)(*(s32*)(slave + 0x328) + 0x324) = 0;
                }
            }
            *(u32*)slave &= ~2;
            if (*(s32*)(slave + 0x104) >= 0) {
                animPoseRelease(*(s32*)(slave + 0x104));
            }
            *(s32*)(slave + 0x104) = -1;
            *(u32*)slave &= ~1;
            if (*(s32*)((s32)gp + 0x14) != 0) {
                slave2 = workPtr;
            } else {
                slave2 = (s32)&work;
            }
            *(s32*)slave2 = *(s32*)slave2 - 1;
            *(s32*)(current + 0x330) = 0;
        }
        i++;
        current += 4;
    } while (i < 4);

    if (*(void**)((s32)npc + 0x330) != 0) {
        *(s32*)(*(s32*)((s32)npc + 0x330) + 0x32C) = 0;
    }
    *(s32*)((s32)npc + 0x330) = 0;
    if (*(void**)((s32)npc + 0x334) != 0) {
        *(s32*)(*(s32*)((s32)npc + 0x334) + 0x32C) = 0;
    }
    *(s32*)((s32)npc + 0x334) = 0;
    slave2 = (s32)npc + 8;
    if (*(void**)(slave2 + 0x330) != 0) {
        *(s32*)(*(s32*)(slave2 + 0x330) + 0x32C) = 0;
    }
    *(s32*)(slave2 + 0x330) = 0;
    if (*(void**)(slave2 + 0x334) != 0) {
        *(s32*)(*(s32*)(slave2 + 0x334) + 0x32C) = 0;
    }
    *(s32*)(slave2 + 0x334) = 0;

    if (*(void**)((s32)npc + 0x324) != 0) {
        if (*(void**)((s32)npc + 0x328) != 0) {
            *(s32*)(*(s32*)((s32)npc + 0x324) + 0x328) = *(s32*)((s32)npc + 0x328);
            *(s32*)(*(s32*)((s32)npc + 0x328) + 0x324) = *(s32*)((s32)npc + 0x324);
        } else {
            *(s32*)(*(s32*)((s32)npc + 0x324) + 0x328) = 0;
        }
    } else {
        if (*(void**)((s32)npc + 0x328) != 0) {
            *(s32*)(*(s32*)((s32)npc + 0x328) + 0x324) = 0;
        }
    }
    *(u32*)npc &= ~2;
    if (*(s32*)((s32)npc + 0x104) >= 0) {
        animPoseRelease(*(s32*)((s32)npc + 0x104));
    }
    *(s32*)((s32)npc + 0x104) = -1;
    *(u32*)npc &= ~1;
    slave2 = (s32)&work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        slave2 = workPtr;
    }
    *(s32*)slave2 = *(s32*)slave2 - 1;
}

void npcDeleteGroup(void* npc) {
    extern void _npcDeleteGroup(void* npc);
    void* parent;

    parent = *(void**)((s32)npc + 0x32C);
    if (parent != 0) {
        npc = parent;
    }
    while (*(void**)((s32)npc + 0x324) != 0) {
        npc = *(void**)((s32)npc + 0x324);
    }
    _npcDeleteGroup(npc);
}

void npcMain(void) {
    extern void* marioGetPtr(void);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern s64 __div2i(s64 dividend, s64 divisor);
    extern f32 __cvt_sll_flt(s64 value);
    extern s32 strcmp(const char* a, const char* b);
    extern void psndSFXOn_3D(char* id, void* pos);
    extern s32 npcHitCheckSide(void* npc, f32 moveX, f32 moveZ, f32* outX, f32* outZ, f32* outAngle);
    extern u8 npcMoveG(void* npc);
    extern u8 npcCheckHitMarioSide(s32 npc);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern void animPoseSetAnim(s32 poseId, void* animName, s32 force);
    extern void* evtGetPtrID(s32 id);
    extern f32 cloudGetBreathPower(f32 width, void* pos);
    extern s32 npcCheckInterrupt(void* npc);
    extern void* camGetPtr(s32 camId);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTX44MultVec(void* mtx, void* src, void* dst);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialFlagOff(s32 poseId, u32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void _fbatFirstAttackAnnouceDisp(s32 cameraId, void* info);
    extern void npcDisp(s32 cameraId, void* npc);
    extern void npcDisp_xlu(s32 cameraId, void* npc);
    extern void npcDisp_blur(s32 cameraId, void* npc);
    extern void npcDisp_offscreen(s32 cameraId, void* npc);
    extern void npcDisp_offscreen_xlu(s32 cameraId, void* npc);
    extern f64 sinfd(f64 angle);
    extern s32 shadowEntry(f64 x, f64 y, f64 z, f64 size);
    extern void shadowSetCollision(s32 id, s32 enable);
    extern s32 hitCheckFilter(f64 x, f64 y, f64 z, f64 dx, f64 dy, f64 dz, s32 flags,
                              void* outA, void* outB, void* outC, void* dist,
                              void* outNX, void* outNY, void* outNZ);
    extern void animPoseVivianMain(s32 poseId, void* pos);
    extern f64 intplGetValue(f64 start, f64 end, s32 mode, s32 cur, s32 max);
    extern void fbatEncountCheck(void);
    extern s32 kpaJumpChk(void);
    extern s32 kpaFireAttackCheck(void);
    extern void kpaAddScorePos(s32 score, void* pos);
    extern void fbatTalkMode(void);
    extern void fbatBattleMode(void);
    extern f64 angleABf(f64 x0, f64 z0, f64 x1, f64 z1);
    extern f64 compAngle(f64 a, f64 b);
    extern f64 reviseAngle(f64 angle);
    extern f64 sqrt(f64 value);
    extern f32 __float_nan[];
    extern const f64 double_0p5_802c13c8;
    extern const f64 double_3_802c13d0;
    extern const f64 double_0_802c13d8;
    extern s32 seqGetSeq(void);
    extern s32 kpaGetLevel(void);

    extern s32 npcMainCount;
    extern u32 DAT_800000f8;
    extern f32 float_0_8041fc7c;
    extern f32 float_0p5_8041fc80;
    extern f32 float_60_8041fc84;
    extern f32 float_365_8041fc88;
    extern f32 float_1_8041fc94;
    extern f32 float_10_8041fc98;
    extern f32 float_90_8041fcb0;
    extern f32 float_180_8041fccc;
    extern f32 float_270_8041fcc4;
    extern f32 float_0p75_8041fcd4;
    extern f32 float_neg1_8041fcd8;
    extern f32 float_0p1_8041fcdc;
    extern f32 float_3_8041fce0;
    extern f32 float_neg600_8041fce4;
    extern f32 float_600_8041fce8;
    extern f32 float_100_8041fcf0;
    extern f32 float_0p25_8041fd58;
    extern f32 float_0p05_8041fd54;
    extern f32 float_neg2_8041fd5c;
    extern f32 float_2_8041fd34;
    extern f32 float_5000_8041fd60;
    extern f32 float_10000_8041fd64;
    extern f32 float_1p33_8041fd68;
    extern f32 float_1E06_8041fd50;
    extern char str_kpa_03_802c1394[];
    extern char str_SFX_KUPPA_3RD_WATER__802c139c[];
    extern const u8 vec3_802c11f8[];
    extern f32 dat_802c1210;
    extern f32 DAT_802c1214;
    extern f32 DAT_802c1218;
    extern f32 dat_802c121c;

    void* npcWork;
    void* player;
    void* npc;
    void* pose;
    void* tribe;
    void* evt;
    void* cam;
    void* fbatData;
    s32 count;
    s32 i;
    u32 jumpFlags;
    u32 hi;
    u32 lo;
    u32 oldHi;
    u32 oldLo;
    s64 divv;
    f64 dval;
    f32 x;
    f32 y;
    f32 z;
    f32 moveX;
    f32 moveZ;
    f32 speed;
    f32 speedRatio;
    struct NpcMotionVec { f32 x, y, z; } cloudPos, breathTop, breathMiddle, breathBottom;
    f32 motionVec[3];
    f32 eventVec[3];
    f32 territoryVec[3];
    f32 projVec[3];
    f32 outX;
    f32 outZ;
    f32 order;
    f32 shadowPos[3];
    f32 colorWord;
    s32 hitA;
    s32 hitB;
    s32 hitC;
    f32 hitDist;
    f32 hitNX;
    f32 hitNY;
    f32 hitNZ;
    s32 shadowId;
    s32 mode;
    s32 jumpChk;
    s32 fireChk;
    f32 kpaPos[3];
    f32 offsetAngle;
    f32 waterScale;
    f32 dx;
    f32 dz;
    f32 distance;
    f32 eventAngle;
    u8 evtType;
    s32 interrupt;
    s32 zero;
    const u8* constBase;
    char* waterSfx;

    constBase = vec3_802c11f8;
    npcWork = (void*)&work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        npcWork = (void*)((s32)npcWork + 0x14);
    }

    zero = 0;
    player = marioGetPtr();
    *(s32*)((s32)player + 0x1DC) = zero;
    waterSfx = (char*)(constBase + 0x1A4);
    count = *(s32*)((s32)npcWork + 4);
    npcMainCount++;
    npc = *(void**)((s32)npcWork + 0xC);

    for (i = 0; i < count; i++) {
        if ((*(u32*)npc & 1) && (*(u32*)npc & 2)) {
            if ((*(u32*)npc & 0x8000) != 0) {
                pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
                *(u32*)pose |= 0x100;
            } else {
                pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
                *(u32*)pose &= ~0x100;
            }

            if ((*(u32*)npc & 0x100000) == 0) {
                *(u32*)((s32)npc + 0x1D4) |= 0x10;
            }
            if ((*(u32*)((s32)npc + 0x1D4) & 0x10000) != 0) {
                *(u32*)((s32)npc + 0x1D4) &= ~0x10;
            }
            if (*(u16*)((s32)npc + 0x320) != 0) {
                *(u16*)((s32)npc + 0x320) = *(u16*)((s32)npc + 0x320) - 1;
            }

            if ((*(u32*)((s32)npc + 0x1D4) & 0x10) != 0) {
                u32 tickDiv;

                *(s64*)((s32)npc + 0x190) =
                    *(s64*)((s32)gp + 0x38) -
                    *(s64*)((s32)npc + 0x198);

                tickDiv = DAT_800000f8 / 4000;
                divv = __div2i(*(s64*)((s32)npc + 0x190), (s64)tickDiv);
                if (divv > 500) {
                    *(s64*)((s32)npc + 0x190) = (s64)(tickDiv << 4);
                }

                if ((*(u32*)npc & 0x20000) != 0) {
                    *(s64*)((s32)npc + 0x178) =
                        *(s64*)((s32)npc + 0x178) +
                        *(s64*)((s32)npc + 0x190);
                } else {
                    *(s64*)((s32)npc + 0x178) = 0;
                }

                *(s64*)((s32)npc + 0x188) =
                    *(s64*)((s32)npc + 0x188) +
                    *(s64*)((s32)npc + 0x190);

                *(s64*)((s32)npc + 0x198) =
                    *(s64*)((s32)gp + 0x38);

                *(f32*)((s32)npc + 0x1A0) =
                    __cvt_sll_flt(
                        __div2i(
                            *(s64*)((s32)npc + 0x188) << 3,
                            (s64)(DAT_800000f8 / 500000))) /
                    float_1E06_8041fd50;

                *(f32*)((s32)npc + 0x180) =
                    __cvt_sll_flt(
                        __div2i(
                            *(s64*)((s32)npc + 0x178) << 3,
                            (s64)(DAT_800000f8 / 500000))) /
                    float_1E06_8041fd50;

                *(f32*)((s32)npc + 0x1A4) =
                    __cvt_sll_flt(
                        __div2i(
                            *(s64*)((s32)npc + 0x190) << 3,
                            (s64)(DAT_800000f8 / 500000))) /
                    float_1E06_8041fd50;

                *(f32*)((s32)npc + 0x1A8) =
                    __cvt_sll_flt(
                        __div2i(
                            *(s64*)((s32)npc + 0x198) << 3,
                            (s64)(DAT_800000f8 / 500000))) /
                    float_1E06_8041fd50;
            } else {
                *(s64*)((s32)npc + 0x198) =
                    *(s64*)((s32)gp + 0x38);
            }

            jumpFlags = *(u32*)((s32)npc + 0x1D4);
            if ((jumpFlags & 0x100000) == 0) {
                *(u32*)((s32)npc + 0x1D4) = jumpFlags & ~0x200000;
            } else {
                *(u32*)((s32)npc + 0x1D4) = jumpFlags | 0x200000;
            }
            *(u32*)((s32)npc + 0x1D4) &= ~0x100000;

            if (strcmp((char*)((s32)gp + 0x1C), (char*)(constBase + 0x19C)) == 0 && *(f32*)((s32)npc + 0x90) <= float_365_8041fc88) {
                *(u32*)((s32)npc + 0x1D4) |= 0x100000;
                if ((*(u32*)((s32)npc + 0x1D4) & 0x200000) == 0) {
                    psndSFXOn_3D(waterSfx, (void*)((s32)npc + 0x8C));
                }
            }

            waterScale = float_1_8041fc94;
            if ((*(u32*)((s32)npc + 0x1D4) & 0x100000) && ((*(u32*)((s32)npc + 0x1D4) & 0x10000000) == 0)) {
                *(f32*)((s32)npc + 0x1D0) = float_100_8041fcf0;
                waterScale = float_0p5_8041fc80;
            }
            if ((*(u32*)((s32)npc + 0x1D4) & 0x100000) != 0) {
                *(u16*)((s32)npc + 0x31E) |= 1;
            }

            if ((*(u32*)((s32)npc + 0x1D4) & 0x10) && *(f32*)((s32)npc + 0x1A4) > float_0_8041fc7c) {
                moveX = *(f32*)((s32)npc + 0x1B4) * waterScale;
                moveZ = *(f32*)((s32)npc + 0x1BC) * waterScale;
                if ((*(u32*)npc & 0x8000000) != 0) {
                    moveZ = float_0_8041fc7c;
                }
                if ((*(u32*)npc & 0x40000000) == 0) {
                    outX = *(f32*)((s32)npc + 0x8C);
                    outZ = *(f32*)((s32)npc + 0x94);
                    *(u32*)((s32)npc + 0x2FC) = npcHitCheckSide(npc, moveX, moveZ, &outX, &outZ, (f32*)((s32)npc + 0x1AC));
                    if (*(u32*)((s32)npc + 0x2FC) == 0) {
                        *(f32*)((s32)npc + 0x8C) = outX;
                        *(f32*)((s32)npc + 0x94) = outZ;
                    }
                } else {
                    *(f32*)((s32)npc + 0x8C) += moveX;
                    *(f32*)((s32)npc + 0x94) += moveZ;
                }
                *(f32*)((s32)npc + 0x1B4) = float_0_8041fc7c;
                *(f32*)((s32)npc + 0x1BC) = float_0_8041fc7c;
                npcMoveG(npc);
            }

            if (((*(u32*)npc & 8) == 0) && ((*(u32*)npc & 0x20) == 0) && ((*(u32*)npc & 0x4000) == 0)) {
                npcCheckHitMarioSide((s32)npc);
            }

            PSVECSubtract((void*)((s32)npc + 0x8C), (void*)((s32)npc + 0x98), motionVec);
            if (((*(u32*)npc & 0x400) != 0) && (*(f32*)((s32)npc + 0x1A4) > float_0_8041fc7c)) {
                {
                    f32 speedSq;
                    f64 d;
                    f64 inv;
                    u32 bits;
                    u32 exp;
                    s32 kind;

                    speedSq = motionVec[0] * motionVec[0] + motionVec[2] * motionVec[2];
                    d = (f64)speedSq;

                    if (speedSq > float_0_8041fc7c) {
                        inv = __frsqrte(d);
                        inv = double_0p5_802c13c8 * inv *
                              -(d * inv * inv - double_3_802c13d0);
                        inv = double_0p5_802c13c8 * inv *
                              -(d * inv * inv - double_3_802c13d0);
                        speed = (f32)(
                            d * double_0p5_802c13c8 * inv *
                            -(d * inv * inv - double_3_802c13d0));
                    } else if (d < double_0_802c13d8) {
                        speed = __float_nan[0];
                    } else {
                        bits = *(u32*)&speedSq;
                        exp = bits;
                        exp &= 0x7F800000;

                        if (exp >= 0x7F800000) {
                            if (exp == 0x7F800000) {
                                if ((bits & 0x7FFFFF) != 0) {
                                    kind = 1;
                                } else {
                                    kind = 2;
                                }
                            } else {
                                kind = 4;
                            }
                        } else if (exp == 0) {
                            if ((bits & 0x7FFFFF) != 0) {
                                kind = 5;
                            } else {
                                kind = 3;
                            }
                        } else {
                            kind = 4;
                        }

                        if (kind == 1) {
                            speed = __float_nan[0];
                        } else {
                            speed = speedSq;
                        }
                    }
                }
                speedRatio = speed / *(f32*)((s32)npc + 0x1A4);
                tribe = *(void**)((s32)npc + 0x28);
                if (speedRatio >= float_0p1_8041fcdc) {
                    if (tribe != 0 && (*(f32*)((s32)tribe + 0x3C) * waterScale) <= speedRatio) {
                        if (*(u32*)((s32)npc + 0x108) != 3) {
                            animPoseSetAnim(*(s32*)((s32)npc + 0x104), *(void**)((s32)tribe + 0x1C), 1);
                            *(u32*)((s32)npc + 0x108) = 3;
                        }
                    } else if (*(u32*)((s32)npc + 0x108) != 2) {
                        if (tribe != 0) {
                            animPoseSetAnim(*(s32*)((s32)npc + 0x104), *(void**)((s32)tribe + 0x18), 1);
                        }
                        *(u32*)((s32)npc + 0x108) = 2;
                    }
                } else if (*(u32*)((s32)npc + 0x108) != 1) {
                    if (tribe != 0) {
                        animPoseSetAnim(*(s32*)((s32)npc + 0x104), *(void**)((s32)tribe + 0x10), 1);
                    }
                    *(u32*)((s32)npc + 0x108) = 1;
                }
            }

            if (*(s32*)((s32)npc + 0x11C) != 0) {
                evt = evtGetPtrID(*(s32*)((s32)npc + 0x11C));
                if (evt != 0 && ((*(u32*)((s32)evt + 0) & 2) == 0)) {
                    if (*(void**)((s32)npc + 0x13C) != 0 && ((*(u32*)npc & 0x20000000) == 0) && ((*(u32*)npc & 0x20) == 0)) {
                        cloudPos.x = *(f32*)((s32)npc + 0x8C);
                        cloudPos.y = *(f32*)((s32)npc + 0x90);
                        cloudPos.z = *(f32*)((s32)npc + 0x94);
                        breathTop = cloudPos;
                        if (cloudGetBreathPower(*(f32*)((s32)npc + 0x14C), &breathTop) > float_0_8041fc7c) {
                            *(u32*)((s32)npc + 0x140) |= 8;
                        } else {
                            cloudPos.x = *(f32*)((s32)npc + 0x8C);
                            cloudPos.y = *(f32*)((s32)npc + 0x90) -
                                        (*(f32*)((s32)npc + 0x150) * float_0p5_8041fc80);
                            cloudPos.z = *(f32*)((s32)npc + 0x94);
                            breathMiddle = cloudPos;
                            if (cloudGetBreathPower(*(f32*)((s32)npc + 0x14C), &breathMiddle) >
                                float_0_8041fc7c) {
                                *(u32*)((s32)npc + 0x140) |= 8;
                            } else {
                                cloudPos.x = *(f32*)((s32)npc + 0x8C);
                                cloudPos.y = *(f32*)((s32)npc + 0x90) -
                                            *(f32*)((s32)npc + 0x150);
                                cloudPos.z = *(f32*)((s32)npc + 0x94);
                                breathBottom = cloudPos;
                                if (cloudGetBreathPower(*(f32*)((s32)npc + 0x14C), &breathBottom) >
                                    float_0_8041fc7c) {
                                    *(u32*)((s32)npc + 0x140) |= 8;
                                }
                            }
                        }
                    }

                    evtType = *(u8*)((s32)evt + 0x10);
                    if ((*(s32*)((s32)npc + 0x1F8) != 0) &&
                        ((evtType == 1) || (evtType == 2))) {
                        interrupt = 0;
                        if (*(s32*)((s32)npc + 0x1F8) == 1) {
                            dx = *(f32*)((s32)npc + 0x1FC) - *(f32*)((s32)npc + 0x8C);
                            dz = *(f32*)((s32)npc + 0x204) - *(f32*)((s32)npc + 0x94);
                            {
                                f32 distanceSq = dx * dx + dz * dz;
                                f64 distanceD = (f64)distanceSq;
                                f64 inv;
                                u32 bits;
                                u32 exp;
                                s32 kind;

                                if (distanceSq > float_0_8041fc7c) {
                                    inv = __frsqrte(distanceD);
                                    inv = double_0p5_802c13c8 * inv *
                                          -(distanceD * inv * inv - double_3_802c13d0);
                                    inv = double_0p5_802c13c8 * inv *
                                          -(distanceD * inv * inv - double_3_802c13d0);
                                    distance = (f32)(
                                        distanceD * double_0p5_802c13c8 * inv *
                                        -(distanceD * inv * inv - double_3_802c13d0));
                                } else if (distanceD < double_0_802c13d8) {
                                    distance = __float_nan[0];
                                } else {
                                    bits = *(u32*)&distanceSq;
                                    exp = bits & 0x7F800000;
                                    if (exp >= 0x7F800000) {
                                        if (exp == 0x7F800000) {
                                            kind = (bits & 0x7FFFFF) != 0 ? 1 : 2;
                                        } else {
                                            kind = 4;
                                        }
                                    } else if (exp == 0) {
                                        kind = (bits & 0x7FFFFF) != 0 ? 5 : 3;
                                    } else {
                                        kind = 4;
                                    }
                                    distance = kind == 1 ? __float_nan[0] : distanceSq;
                                }
                            }
                            if (distance > *(f32*)((s32)npc + 0x214)) {
                                interrupt = 1;
                            }
                        } else if (*(s32*)((s32)npc + 0x1F8) == 2) {
                            f32* range = (f32*)((s32)npc + ((evtType == 2) ? 0x214 : 0x208));
                            x = *(f32*)((s32)npc + 0x8C);
                            z = *(f32*)((s32)npc + 0x94);
                            if ((x < (*(f32*)((s32)npc + 0x1FC) - range[0])) ||
                                (x > (*(f32*)((s32)npc + 0x1FC) + range[0])) ||
                                (z < (*(f32*)((s32)npc + 0x204) - range[2])) ||
                                (z > (*(f32*)((s32)npc + 0x204) + range[2]))) {
                                interrupt = 1;
                            }
                        }
                        if (interrupt != 0) {
                            *(u32*)((s32)npc + 0x140) |= 4;
                        }
                    }

                    if (evtType == 2) {
                        player = marioGetPtr();
                        dx = *(f32*)((s32)player + 0x8C) - *(f32*)((s32)npc + 0x8C);
                        y = *(f32*)((s32)player + 0x90) - *(f32*)((s32)npc + 0x90);
                        dz = *(f32*)((s32)player + 0x94) - *(f32*)((s32)npc + 0x94);
                        PSVECSubtract((void*)((s32)npc + 0x98),
                                      (void*)((s32)npc + 0x8C), eventVec);
                        eventAngle = *(f32*)((s32)npc + 0x144);
                        if ((*(u32*)((s32)npc + 0x1D4) & 2) != 0) {
                            f32 speedSq = eventVec[0] * eventVec[0] + eventVec[2] * eventVec[2];
                            f64 speedD = (f64)speedSq;
                            f64 inv;
                            u32 bits;
                            u32 exp;
                            s32 kind;

                            if (speedSq > float_0_8041fc7c) {
                                inv = __frsqrte(speedD);
                                inv = double_0p5_802c13c8 * inv *
                                      -(speedD * inv * inv - double_3_802c13d0);
                                inv = double_0p5_802c13c8 * inv *
                                      -(speedD * inv * inv - double_3_802c13d0);
                                speed = (f32)(speedD * double_0p5_802c13c8 * inv *
                                              -(speedD * inv * inv - double_3_802c13d0));
                            } else if (speedD < double_0_802c13d8) {
                                speed = __float_nan[0];
                            } else {
                                bits = *(u32*)&speedSq;
                                exp = bits & 0x7F800000;
                                if (exp >= 0x7F800000) {
                                    if (exp == 0x7F800000) {
                                        kind = (bits & 0x7FFFFF) != 0 ? 1 : 2;
                                    } else {
                                        kind = 4;
                                    }
                                } else if (exp == 0) {
                                    kind = (bits & 0x7FFFFF) != 0 ? 5 : 3;
                                } else {
                                    kind = 4;
                                }
                                speed = kind == 1 ? __float_nan[0] : speedSq;
                            }
                            if (speed > float_0p05_8041fd54) {
                                eventAngle = *(f32*)((s32)npc + 0x1AC);
                            }
                        }
                        player = marioGetPtr();
                        {
                            f32 distanceSq = dx * dx + dz * dz;
                            f64 distanceD = (f64)distanceSq;
                            f64 inv;
                            u32 bits;
                            u32 exp;
                            s32 kind;

                            if (distanceSq > float_0_8041fc7c) {
                                inv = __frsqrte(distanceD);
                                inv = double_0p5_802c13c8 * inv *
                                      -(distanceD * inv * inv - double_3_802c13d0);
                                inv = double_0p5_802c13c8 * inv *
                                      -(distanceD * inv * inv - double_3_802c13d0);
                                distance = (f32)(
                                    distanceD * double_0p5_802c13c8 * inv *
                                    -(distanceD * inv * inv - double_3_802c13d0));
                            } else if (distanceD < double_0_802c13d8) {
                                distance = __float_nan[0];
                            } else {
                                bits = *(u32*)&distanceSq;
                                exp = bits & 0x7F800000;
                                if (exp >= 0x7F800000) {
                                    if (exp == 0x7F800000) {
                                        kind = (bits & 0x7FFFFF) != 0 ? 1 : 2;
                                    } else {
                                        kind = 4;
                                    }
                                } else if (exp == 0) {
                                    kind = (bits & 0x7FFFFF) != 0 ? 5 : 3;
                                } else {
                                    kind = 4;
                                }
                                distance = kind == 1 ? __float_nan[0] : distanceSq;
                            }
                        }
                        interrupt = 1;
                        if (((*(u32*)player & 0x82000000) == 0) &&
                            ((*(u32*)((s32)player + 8) & 1) == 0) &&
                            ((*(u32*)player & 0xA) == 0) &&
                            (*(s32*)((s32)gp + 0x164) == 0) && (seqGetSeq() == 2) &&
                            (*(u16*)((s32)player + 0x2E) != 0x19) &&
                            ((vivianGetStatus() == 0) || (vivianGetStatus() == 1)) &&
                            (distance >= float_0p25_8041fd58 * *(f32*)((s32)npc + 0x228)) &&
                            (distance <= *(f32*)((s32)npc + 0x228)) &&
                            (y <= *(f32*)((s32)npc + 0x20C)) &&
                            (-y <= *(f32*)((s32)npc + 0x218))) {
                            eventAngle = (f32)compAngle(eventAngle,
                                                       (f32)angleABf(float_0_8041fc7c,
                                                                   float_0_8041fc7c, dx, dz));
                            if ((eventAngle <= float_0p5_8041fc80 *
                                               *(f32*)((s32)npc + 0x22C)) &&
                                (eventAngle >= -(float_0p5_8041fc80 *
                                                *(f32*)((s32)npc + 0x22C)))) {
                                interrupt = 0;
                            }
                        }
                        if (interrupt != 0) {
                            *(u32*)((s32)npc + 0x140) |= 2;
                        }
                    }

                    if ((evtType == 1) || (evtType == 4)) {
                        player = marioGetPtr();
                        dx = *(f32*)((s32)player + 0x8C) - *(f32*)((s32)npc + 0x8C);
                        y = *(f32*)((s32)player + 0x90) - *(f32*)((s32)npc + 0x90);
                        dz = *(f32*)((s32)player + 0x94) - *(f32*)((s32)npc + 0x94);
                        {
                            f32 distanceSq = dx * dx + dz * dz;
                            f64 distanceD = (f64)distanceSq;
                            f64 inv;
                            u32 bits;
                            u32 exp;
                            s32 kind;

                            if (distanceSq > float_0_8041fc7c) {
                                inv = __frsqrte(distanceD);
                                inv = double_0p5_802c13c8 * inv *
                                      -(distanceD * inv * inv - double_3_802c13d0);
                                inv = double_0p5_802c13c8 * inv *
                                      -(distanceD * inv * inv - double_3_802c13d0);
                                distance = (f32)(
                                    distanceD * double_0p5_802c13c8 * inv *
                                    -(distanceD * inv * inv - double_3_802c13d0));
                            } else if (distanceD < double_0_802c13d8) {
                                distance = __float_nan[0];
                            } else {
                                bits = *(u32*)&distanceSq;
                                exp = bits & 0x7F800000;
                                if (exp >= 0x7F800000) {
                                    if (exp == 0x7F800000) {
                                        kind = (bits & 0x7FFFFF) != 0 ? 1 : 2;
                                    } else {
                                        kind = 4;
                                    }
                                } else if (exp == 0) {
                                    kind = (bits & 0x7FFFFF) != 0 ? 5 : 3;
                                } else {
                                    kind = 4;
                                }
                                distance = kind == 1 ? __float_nan[0] : distanceSq;
                            }
                        }
                        if ((*(u32*)npc & 0x8000000) != 0) {
                            f32 levelOffsets[4];
                            levelOffsets[0] = dat_802c1210;
                            levelOffsets[1] = DAT_802c1214;
                            levelOffsets[2] = DAT_802c1218;
                            levelOffsets[3] = dat_802c121c;
                            distance -= levelOffsets[kpaGetLevel()];
                            if (distance < float_0_8041fc7c) {
                                distance = float_0_8041fc7c;
                            }
                        }
                        PSVECSubtract((void*)((s32)npc + 0x98),
                                      (void*)((s32)npc + 0x8C), territoryVec);
                        eventAngle = float_270_8041fcc4;
                        if (reviseAngle(*(f32*)((s32)npc + 0x144)) < float_180_8041fccc) {
                            eventAngle = float_90_8041fcb0;
                        }
                        player = marioGetPtr();
                        interrupt = 0;
                        if (((*(u32*)player & 0x82000000) == 0) &&
                            ((*(u32*)((s32)player + 8) & 1) == 0) &&
                            ((*(u32*)player & 0xA) == 0) &&
                            (*(s32*)((s32)gp + 0x164) == 0) && (seqGetSeq() == 2) &&
                            (*(u16*)((s32)player + 0x2E) != 0x19) &&
                            ((vivianGetStatus() == 0) || (vivianGetStatus() == 1)) &&
                            (y <= *(f32*)((s32)npc + 0x20C)) &&
                            (-y <= *(f32*)((s32)npc + 0x218))) {
                            if (distance <= float_0p25_8041fd58 * *(f32*)((s32)npc + 0x220)) {
                                interrupt = 1;
                            } else if (distance <= *(f32*)((s32)npc + 0x220)) {
                                dval = compAngle(eventAngle,
                                                angleABf(float_0_8041fc7c,
                                                         float_0_8041fc7c, dx, dz));
                                if ((dval <= (f64)(float_0p5_8041fc80 *
                                                   *(f32*)((s32)npc + 0x224))) &&
                                    (dval >= (f64)(-(float_0p5_8041fc80 *
                                                    *(f32*)((s32)npc + 0x224))))) {
                                    interrupt = 1;
                                }
                            }
                        }
                        if ((interrupt != 0) && (evtType == 4)) {
                            if (*(s32*)((s32)npc + 0x1F8) == 1) {
                                dx = *(f32*)((s32)npc + 0x1FC) - *(f32*)((s32)npc + 0x8C);
                                dz = *(f32*)((s32)npc + 0x204) - *(f32*)((s32)npc + 0x94);
                                {
                                    f32 distanceSq = dx * dx + dz * dz;
                                    f64 distanceD = (f64)distanceSq;
                                    f64 inv;
                                    u32 bits;
                                    u32 exp;
                                    s32 kind;

                                    if (distanceSq > float_0_8041fc7c) {
                                        inv = __frsqrte(distanceD);
                                        inv = double_0p5_802c13c8 * inv *
                                              -(distanceD * inv * inv - double_3_802c13d0);
                                        inv = double_0p5_802c13c8 * inv *
                                              -(distanceD * inv * inv - double_3_802c13d0);
                                        distance = (f32)(
                                            distanceD * double_0p5_802c13c8 * inv *
                                            -(distanceD * inv * inv - double_3_802c13d0));
                                    } else if (distanceD < double_0_802c13d8) {
                                        distance = __float_nan[0];
                                    } else {
                                        bits = *(u32*)&distanceSq;
                                        exp = bits & 0x7F800000;
                                        if (exp >= 0x7F800000) {
                                            if (exp == 0x7F800000) {
                                                kind = (bits & 0x7FFFFF) != 0 ? 1 : 2;
                                            } else {
                                                kind = 4;
                                            }
                                        } else if (exp == 0) {
                                            kind = (bits & 0x7FFFFF) != 0 ? 5 : 3;
                                        } else {
                                            kind = 4;
                                        }
                                        distance = kind == 1 ? __float_nan[0] : distanceSq;
                                    }
                                }
                                if (distance > 0.75 *
                                               *(f32*)((s32)npc + 0x214)) {
                                    interrupt = 0;
                                }
                            } else if (*(s32*)((s32)npc + 0x1F8) == 2) {
                                dx = float_0p75_8041fcd4 * *(f32*)((s32)npc + 0x214);
                                dz = float_0p75_8041fcd4 * *(f32*)((s32)npc + 0x21C);
                                x = *(f32*)((s32)npc + 0x8C);
                                z = *(f32*)((s32)npc + 0x94);
                                if ((x < (*(f32*)((s32)npc + 0x1FC) - dx)) ||
                                    (x > (*(f32*)((s32)npc + 0x1FC) + dx)) ||
                                    (z < (*(f32*)((s32)npc + 0x204) - dz)) ||
                                    (z > (*(f32*)((s32)npc + 0x204) + dz))) {
                                    interrupt = 0;
                                }
                            }
                        }
                        if (interrupt != 0) {
                            *(u32*)((s32)npc + 0x140) |= 1;
                        }
                    }
                    npcCheckInterrupt(npc);
                }
            }

            if ((*(u32*)npc & 0x800) == 0) {
                cam = camGetPtr(4);
                PSMTXMultVec((void*)((s32)cam + 0x18C), (void*)((s32)npc + 0x8C), projVec);
                cam = camGetPtr(4);
                PSMTX44MultVec((void*)((s32)cam + 0x1BC), projVec, projVec);
                if (((projVec[2] > float_neg1_8041fcd8) && (projVec[2] < float_0_8041fc7c) &&
                     (projVec[0] >= float_neg2_8041fd5c) && (projVec[0] <= float_2_8041fd34) &&
                     (projVec[1] >= float_neg2_8041fd5c) && (projVec[1] <= float_2_8041fd34)) || ((*(u32*)npc & 0x40) != 0)) {
                    if (*(u32*)((s32)npc + 0x114) == 0xFFFFFFFF) {
                        animPoseSetMaterialFlagOff(*(s32*)((s32)npc + 0x104), 0x40);
                    } else {
                        animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), 0x40);
                        colorWord = *(f32*)((s32)npc + 0x114);
                        animPoseSetMaterialEvtColor(*(s32*)((s32)npc + 0x104), &colorWord);
                    }
                    order = (float_5000_8041fd60 * projVec[2]) + float_5000_8041fd60;
                    if (order < float_0_8041fc7c) {
                        order = float_0_8041fc7c;
                    } else if (order > float_10000_8041fd64) {
                        order = float_10000_8041fd64;
                    }
                    order = -order;
                    if (((*(u32*)npc & 0x80) == 0) && (((*(u16*)((s32)npc + 0x320) & 2) == 0) || ((*(u32*)((s32)npc + 0x1D4) & 0x10000) != 0))) {
                        if (*(u8*)((s32)npc + 0x117) == 0xFF) {
                            dispEntry(*(s32*)((s32)npc + 0x1F4), 0, npcDisp, npc, order);
                            dispEntry(*(s32*)((s32)npc + 0x1F4), 1, npcDisp, npc, order);
                            dispEntry(*(s32*)((s32)npc + 0x1F4), 2, npcDisp, npc, order);
                        } else {
                            dispEntry(*(s32*)((s32)npc + 0x1F4), 2, npcDisp_xlu, npc, order);
                        }
                        if ((*(u32*)npc & 0x4000000) == 0) {
                            if ((*(u32*)npc & 0x40000) == 0) {
                                shadowPos[0] = *(f32*)((s32)npc + 0x8C);
                                shadowPos[1] = *(f32*)((s32)npc + 0x90);
                                shadowPos[2] = *(f32*)((s32)npc + 0x94);
                                tribe = *(void**)((s32)npc + 0x28);
                                if (tribe != 0) {
                                    offsetAngle = float_270_8041fcc4;
                                    if (reviseAngle(*(f32*)((s32)npc + 0x144)) < float_180_8041fccc) {
                                        offsetAngle = float_90_8041fcb0;
                                    }
                                    dval = sinfd(offsetAngle);
                                    shadowPos[0] += (f32)((f64)(*(f32*)((s32)tribe + 0x30) * *(f32*)((s32)npc + 0xE0)) * dval);
                                    shadowPos[1] += *(f32*)((s32)tribe + 0x34) * *(f32*)((s32)npc + 0xE4);
                                    shadowPos[2] += *(f32*)((s32)tribe + 0x38) * *(f32*)((s32)npc + 0xE8);
                                }
                                shadowId = shadowEntry(shadowPos[0], shadowPos[1], shadowPos[2],
                                                       float_1p33_8041fd68 * *(f32*)((s32)npc + 0x154) *
                                                       (*(f32*)((s32)npc + 0x14C) / float_3_8041fce0) * *(f32*)((s32)npc + 0xE0));
                                if ((*(u32*)npc & 0x80000) != 0) {
                                    shadowSetCollision(shadowId, 0);
                                }
                            }
                        } else {
                            shadowPos[0] = *(f32*)((s32)npc + 0x8C);
                            shadowPos[1] = *(f32*)((s32)npc + 0x90);
                            shadowPos[2] = *(f32*)((s32)npc + 0x94);
                            hitDist = float_100_8041fcf0;
                            if (hitCheckFilter(shadowPos[0], shadowPos[1] + float_1_8041fc94, shadowPos[2],
                                               float_0_8041fc7c, float_neg1_8041fcd8, float_0_8041fc7c,
                                               0, &hitA, &hitB, &hitC, &hitDist, &hitNX, &hitNY, &hitNZ) != 0) {
                                shadowPos[1] = hitNX;
                            }
                            animPoseVivianMain(*(s32*)((s32)npc + 0x104), shadowPos);
                            shadowId = shadowEntry(*(f32*)((s32)npc + 0x8C), *(f32*)((s32)npc + 0x90), *(f32*)((s32)npc + 0x94),
                                                   *(f32*)((s32)npc + 0x154) * (*(f32*)((s32)npc + 0x14C) / float_3_8041fce0) * *(f32*)((s32)npc + 0xE0));
                            if ((*(u32*)npc & 0x80000) != 0) {
                                shadowSetCollision(shadowId, 0);
                            }
                        }
                        if ((*(u32*)npc & 0x100) != 0) {
                            dispEntry(*(s32*)((s32)npc + 0x1F4), 2, npcDisp_blur, npc, order);
                        }
                    }
                    if (*(s32*)((s32)npc + 0x110) != -1) {
                        if (*(u8*)((s32)npc + 0x117) == 0xFF) {
                            dispEntry(5, 0, npcDisp_offscreen, npc, order);
                            dispEntry(5, 1, npcDisp_offscreen, npc, order);
                            dispEntry(5, 2, npcDisp_offscreen, npc, order);
                        } else {
                            dispEntry(5, 2, npcDisp_offscreen_xlu, npc, order);
                        }
                    }
                }
            }

            *(f32*)((s32)npc + 0x98) = *(f32*)((s32)npc + 0x8C);
            *(f32*)((s32)npc + 0x9C) = *(f32*)((s32)npc + 0x90);
            *(f32*)((s32)npc + 0xA0) = *(f32*)((s32)npc + 0x94);
            if ((*(u32*)npc & 0x100) && ((npcMainCount & 1) == 0)) {
                *(f32*)((s32)npc + 0xD4) = *(f32*)((s32)npc + 0xC8);
                *(f32*)((s32)npc + 0xD8) = *(f32*)((s32)npc + 0xCC);
                *(f32*)((s32)npc + 0xDC) = *(f32*)((s32)npc + 0xD0);
                *(f32*)((s32)npc + 0xC8) = *(f32*)((s32)npc + 0xBC);
                *(f32*)((s32)npc + 0xCC) = *(f32*)((s32)npc + 0xC0);
                *(f32*)((s32)npc + 0xD0) = *(f32*)((s32)npc + 0xC4);
                *(f32*)((s32)npc + 0xBC) = *(f32*)((s32)npc + 0xB0);
                *(f32*)((s32)npc + 0xC0) = *(f32*)((s32)npc + 0xB4);
                *(f32*)((s32)npc + 0xC4) = *(f32*)((s32)npc + 0xB8);
                *(f32*)((s32)npc + 0xB0) = *(f32*)((s32)npc + 0xA4);
                *(f32*)((s32)npc + 0xB4) = *(f32*)((s32)npc + 0xA8);
                *(f32*)((s32)npc + 0xB8) = *(f32*)((s32)npc + 0xAC);
                *(f32*)((s32)npc + 0xA4) = *(f32*)((s32)npc + 0x8C);
                *(f32*)((s32)npc + 0xA8) = *(f32*)((s32)npc + 0x90);
                *(f32*)((s32)npc + 0xAC) = *(f32*)((s32)npc + 0x94);
            }
            *(u32*)((s32)npc + 0x1D4) &= ~0x10;
        }
        npc = (void*)((s32)npc + 0x340);
    }

    fbatData = *(void**)((s32)gp + 0x168);
    if (*(s32*)((s32)fbatData + 0x550) != 0) {
        mode = *(s32*)((s32)fbatData + 0x558);
        if (mode == 2) {
            *(s32*)((s32)fbatData + 0x55C) += 1;
            i = *(s32*)((s32)fbatData + 0x55C);
            if (i < 0x14) {
                *(f32*)((s32)fbatData + 0x560) = (f32)intplGetValue(float_neg600_8041fce4, float_0_8041fc7c, 5, i, 0x14);
            } else if (i < 0x50) {
                *(f32*)((s32)fbatData + 0x560) = float_0_8041fc7c;
            } else if (i > 99) {
                *(s32*)((s32)fbatData + 0x554) = 0;
            } else {
                *(f32*)((s32)fbatData + 0x560) = (f32)intplGetValue(float_0_8041fc7c, float_600_8041fce8, 1, i - 0x50, 0x14);
            }
            if (*(s32*)((s32)fbatData + 0x554) != 0) {
                *(f32*)((s32)fbatData + 0x564) = float_0_8041fc7c;
                dispEntry(2, 2, _fbatFirstAttackAnnouceDisp, (void*)((s32)fbatData + 0x550), float_0_8041fc7c);
            }
        } else if (mode > 0 && mode < 2) {
            *(s32*)((s32)fbatData + 0x55C) += 1;
            if (*(s32*)((s32)fbatData + 0x55C) < 0x3D) {
                *(f32*)((s32)fbatData + 0x560) = float_0_8041fc7c;
                *(f32*)((s32)fbatData + 0x564) = float_0_8041fc7c;
                dispEntry(2, 2, _fbatFirstAttackAnnouceDisp, (void*)((s32)fbatData + 0x550), float_0_8041fc7c);
            } else {
                *(s32*)((s32)fbatData + 0x554) = 0;
            }
        }
    }

    mode = *(s16*)fbatData;
    if (mode != 4) {
        if (mode < 4) {
            if (mode == 1) {
                fbatEncountCheck();
                fbatData = *(void**)((s32)gp + 0x168);
                player = marioGetPtr();
                jumpChk = 0;
                fireChk = 0;
                if (*(s8*)((s32)player + 0x3C) == 2) {
                    jumpChk = kpaJumpChk();
                    fireChk = kpaFireAttackCheck();
                }
                if (jumpChk != 0) {
                    if (*(s32*)((s32)fbatData + 0x504) > 0 && *(s32*)((s32)fbatData + 0x508) > 1) {
                        kpaPos[0] = *(f32*)((s32)player + 0x8C);
                        kpaPos[1] = *(f32*)((s32)player + 0x90) + float_60_8041fc84;
                        kpaPos[2] = *(f32*)((s32)player + 0x94);
                        kpaAddScorePos(*(s32*)((s32)fbatData + 0x504), kpaPos);
                    }
                    *(s32*)((s32)fbatData + 0x508) = 0;
                    *(s32*)((s32)fbatData + 0x504) = 0;
                }
                if (fireChk == 0) {
                    *(s32*)((s32)fbatData + 0x50C) = 0;
                }
            } else if (mode > 0) {
                fbatTalkMode();
            }
        } else if (mode < 6) {
            fbatBattleMode();
        }
    }
    *(u8*)((s32)fbatData + 0x54D) = zero;
}

void* npcNameToPtr(s32 name) {
    s32 wp = (s32)&work;
    s32 i;
    s32 count;
    void* npc;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if ((*(u32*)npc & 1) && strcmp((char*)((s32)npc + 8), (char*)name) == 0) {
            break;
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
    return npc;
}

void* npcNameToPtr_NoAssert(s32 name) {
    s32 wp = (s32)&work;
    s32 i;
    s32 count;
    void* npc;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if ((*(u32*)npc & 1) && strcmp((char*)((s32)npc + 8), (char*)name) == 0) {
            break;
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
    if (i >= count) {
        return 0;
    }
    return npc;
}

void npcSetMarioAutoTalkPose(const char* stay, const char* talk) {
    strcpy(MarioStayPose, stay);
    strcpy(MarioTalkPose, talk);
}

void npcSetTalkPose(char* talkPose) {
    extern char str_party_8041fd40[];
    extern char str_mario_8041fd48[];
    s32 wp;
    s32 count;
    s32 i;
    void* npc;
    void* party;

    if (*talkPose == 0 || strcmp(talkPose, "(^x^)party") == 0) {
        return;
    }
    if (strcmp(talkPose, str_party_8041fd40) == 0) {
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            partyChgPoseId(party, 7);
        }
        return;
    }
    if (strcmp(talkPose, "extparty") == 0) {
        party = partyGetPtr(marioGetExtraPartyId());
        if (party != 0) {
            partyChgPoseId(party, 7);
        }
        return;
    }
    if (strcmp(talkPose, str_mario_8041fd48) == 0) {
        marioChgPose(MarioTalkPose);
        return;
    }
    wp = (s32)&work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if ((*(u32*)npc & 1) && strcmp((char*)((s32)npc + 8), talkPose) == 0) {
            break;
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
    if ((*(u32*)npc & 0x200) != 0) {
        animPoseSetAnim(*(s32*)((s32)npc + 0x104), (char*)((s32)npc + 0x6C), 1);
    }
}

void npcSetStayPose(char* stayPose) {
    extern char str_party_8041fd40[];
    extern char str_mario_8041fd48[];
    s32 wp;
    s32 count;
    s32 i;
    void* npc;
    void* party;

    if (*stayPose == 0 || strcmp(stayPose, "(^x^)party") == 0) {
        return;
    }
    if (strcmp(stayPose, str_party_8041fd40) == 0) {
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            partyChgPoseId(party, 1);
        }
        return;
    }
    if (strcmp(stayPose, "extparty") == 0) {
        party = partyGetPtr(marioGetExtraPartyId());
        if (party != 0) {
            partyChgPoseId(party, 1);
        }
        return;
    }
    if (strcmp(stayPose, str_mario_8041fd48) == 0) {
        marioChgPose(MarioStayPose);
        return;
    }
    wp = (s32)&work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if ((*(u32*)npc & 1) && strcmp((char*)((s32)npc + 8), stayPose) == 0) {
            break;
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
    if ((*(u32*)npc & 0x200) != 0) {
        animPoseSetAnim(*(s32*)((s32)npc + 0x104), (char*)((s32)npc + 0x4C), 1);
    }
}

#pragma opt_common_subs off
u8 npcCheckHitMarioSide(s32 pNpc) {
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 compAngle(f32, f32);

    extern f32 __float_nan[];
    extern s32 marioChkWallAround(void*, s32, f32, f32, f32, f32);
    extern const u8 vec3_802c11f8[];
    extern const f32 float_0_8041fc7c;
    extern const f32 float_40_8041fd0c;
    extern const f32 float_6p2832_8041fd10;
    extern const f32 float_360_8041fcc0;
    extern const f32 float_0p00761_8041fd14;
    extern const f32 float_0p16605_8041fd18;
    extern const f32 float_3p1416_8041fd1c;
    extern const f32 float_1p5708_8041fd20;
    extern const f32 float_4p7124_8041fd24;
    extern const f32 float_0p03705_8041fd28;
    extern const f32 float_0p4967_8041fd2c;
    extern const f32 float_1_8041fc94;
    extern const f32 float_1p5_8041fcac;
    extern const f32 float_3_8041fce0;
    extern const f32 float_4_8041fd30;
    extern const f32 float_2_8041fd34;
    extern const f32 float_160_8041fd38;
    extern const f32 float_20_8041fd3c;
    extern const f32 float_5_8041fd08;
    extern const f32 float_0p5_8041fc80;

    const u8* constBase;
    void* mario;
    f32 pos[3];
    f32 dist;
    f32 radius;
    f32 angle;
    f32 diff;
    f32 theta;
    f32 t;
    f32 t2;
    f32 sine;
    f32 cosine;
    f32 amount;
    f32 pushX;
    f32 pushZ;
    f32 lenSq;
    f32 len;
    f32 threshold;
    f64 d;
    f64 inv;
    u32 bits;
    u32 exp;
    s32 kind;

    constBase = vec3_802c11f8;
    mario = marioGetPtr();

    if (*(f32*)(pNpc + 0x90) >
        *(f32*)((s32)mario + 0x90) + *(f32*)((s32)mario + 0x1BC)) {
        return 0;
    }
    if (*(f32*)((s32)mario + 0x90) >
        *(f32*)(pNpc + 0x90) + *(f32*)(pNpc + 0x150)) {
        return 0;
    }

    pos[0] = *(f32*)((s32)mario + 0x8C);
    pos[1] = *(f32*)(pNpc + 0x90);
    pos[2] = *(f32*)((s32)mario + 0x94);

    dist = PSVECDistance(pos, (void*)(pNpc + 0x8C));

    radius = (float_5_8041fd08 + (*(f32*)((s32)mario + 0x1B8) +
              *(f32*)(pNpc + 0x14C))) * float_0p5_8041fc80;

    if (dist > radius) {
        return 0;
    }

    *(f32*)(pNpc + 0x8C) = *(f32*)(pNpc + 0x98);
    *(f32*)(pNpc + 0x94) = *(f32*)(pNpc + 0xA0);

    dist = PSVECDistance(pos, (void*)(pNpc + 0x8C));
    if (dist > radius) {
        return 0;
    }

    *(void**)((s32)mario + 0x1DC) = (void*)pNpc;

    angle = angleABf(
        *(f32*)((s32)mario + 0x8C),
        *(f32*)((s32)mario + 0x94),
        *(f32*)(pNpc + 0x8C),
        *(f32*)(pNpc + 0x94));

    diff = __fabsf(compAngle(angle, *(f32*)((s32)mario + 0x1A4)));

    /*
     * The target deliberately contains two complete polynomial sin/cos
     * paths.  Do not factor these together: the duplicated control flow is
     * part of the target instruction shape.
     */
    if (diff < float_40_8041fd0c) {
        theta =
            (float_6p2832_8041fd10 * *(f32*)((s32)mario + 0x1A4)) /
            float_360_8041fcc0;

        if (theta <= float_3p1416_8041fd1c) {
            if (theta <= float_1p5708_8041fd20) {
                t = theta;
            } else {
                t = float_1p5708_8041fd20 -
                    (theta - float_1p5708_8041fd20);
            }

            t2 = t * t;
            sine =
                (float_0p00761_8041fd14 * t2 -
                 float_0p16605_8041fd18) *
                    t2 +
                float_1_8041fc94;
            sine *= t;
        } else {
            if (theta < float_4p7124_8041fd24) {
                t = theta - float_3p1416_8041fd1c;
            } else {
                t = float_1p5708_8041fd20 -
                    (theta - float_4p7124_8041fd24);
            }

            t2 = t * t;
            sine =
                (float_0p00761_8041fd14 * t2 -
                 float_0p16605_8041fd18) *
                    t2 +
                float_1_8041fc94;
            sine = -(sine * t);
        }

        amount = radius - dist;
        pushX = amount * -sine;

        if (theta <= float_3p1416_8041fd1c) {
            if (theta <= float_1p5708_8041fd20) {
                t = theta;
                t2 = t * t;
                cosine =
                    (float_0p03705_8041fd28 * t2 -
                     float_0p4967_8041fd2c) *
                        t2 +
                    float_1_8041fc94;
            } else {
                t = float_1p5708_8041fd20 -
                    (theta - float_1p5708_8041fd20);
                t2 = t * t;
                cosine = -(
                    (float_0p03705_8041fd28 * t2 -
                     float_0p4967_8041fd2c) *
                        t2 +
                    float_1_8041fc94);
            }
        } else {
            if (theta < float_4p7124_8041fd24) {
                t = theta - float_3p1416_8041fd1c;
                t2 = t * t;
                cosine = -(
                    (float_0p03705_8041fd28 * t2 -
                     float_0p4967_8041fd2c) *
                        t2 +
                    float_1_8041fc94);
            } else {
                t = float_1p5708_8041fd20 -
                    (theta - float_4p7124_8041fd24);
                t2 = t * t;
                cosine =
                    (float_0p03705_8041fd28 * t2 -
                     float_0p4967_8041fd2c) *
                        t2 +
                    float_1_8041fc94;
            }
        }

        pushZ = amount * cosine;
    } else {
        theta =
            (float_6p2832_8041fd10 * angle) /
            float_360_8041fcc0;

        if (theta <= float_3p1416_8041fd1c) {
            if (theta <= float_1p5708_8041fd20) {
                t = theta;
            } else {
                t = float_1p5708_8041fd20 -
                    (theta - float_1p5708_8041fd20);
            }

            t2 = t * t;
            sine =
                (float_0p00761_8041fd14 * t2 -
                 float_0p16605_8041fd18) *
                    t2 +
                float_1_8041fc94;
            sine *= t;
        } else {
            if (theta < float_4p7124_8041fd24) {
                t = theta - float_3p1416_8041fd1c;
            } else {
                t = float_1p5708_8041fd20 -
                    (theta - float_4p7124_8041fd24);
            }

            t2 = t * t;
            sine =
                (float_0p00761_8041fd14 * t2 -
                 float_0p16605_8041fd18) *
                    t2 +
                float_1_8041fc94;
            sine = -(sine * t);
        }

        pushX =
            *(f32*)(pNpc + 0x8C) -
            radius * sine -
            *(f32*)((s32)mario + 0x8C);

        if (theta <= float_3p1416_8041fd1c) {
            if (theta <= float_1p5708_8041fd20) {
                t = theta;
                t2 = t * t;
                cosine =
                    (float_0p03705_8041fd28 * t2 -
                     float_0p4967_8041fd2c) *
                        t2 +
                    float_1_8041fc94;
            } else {
                t = float_1p5708_8041fd20 -
                    (theta - float_1p5708_8041fd20);
                t2 = t * t;
                cosine = -(
                    (float_0p03705_8041fd28 * t2 -
                     float_0p4967_8041fd2c) *
                        t2 +
                    float_1_8041fc94);
            }
        } else {
            if (theta < float_4p7124_8041fd24) {
                t = theta - float_3p1416_8041fd1c;
                t2 = t * t;
                cosine = -(
                    (float_0p03705_8041fd28 * t2 -
                     float_0p4967_8041fd2c) *
                        t2 +
                    float_1_8041fc94);
            } else {
                t = float_1p5708_8041fd20 -
                    (theta - float_4p7124_8041fd24);
                t2 = t * t;
                cosine =
                    (float_0p03705_8041fd28 * t2 -
                     float_0p4967_8041fd2c) *
                        t2 +
                    float_1_8041fc94;
            }
        }

        pushZ =
            radius * cosine +
            *(f32*)(pNpc + 0x94) -
            *(f32*)((s32)mario + 0x94);
    }

    lenSq = pushX * pushX + pushZ * pushZ;
    d = (f64)lenSq;

    if (lenSq > float_0_8041fc7c) {
        inv = __frsqrte(d);
        inv = *(const f64*)(constBase + 0x1D0) * inv *
              (*(const f64*)(constBase + 0x1D8) - d * inv * inv);
        inv = *(const f64*)(constBase + 0x1D0) * inv *
              (*(const f64*)(constBase + 0x1D8) - d * inv * inv);
        len = (f32)(d * *(const f64*)(constBase + 0x1D0) * inv *
                    (*(const f64*)(constBase + 0x1D8) - d * inv * inv));
    } else {
        if (d < *(const f64*)(constBase + 0x1E0)) {
            len = __float_nan[0];
        } else {
            bits = *(u32*)&lenSq;
            exp = bits;
            exp &= 0x7F800000;

            switch (exp) { case 0x7F800000: if (bits & 0x7FFFFF) kind = 1; else kind = 2; break; case 0: if (bits & 0x7FFFFF) kind = 5; else kind = 3; break; default: kind = 4; break; }

            if (kind == 1) {
                len = __float_nan[0];
            } else {
                len = lenSq;
            }
        }
    }

    if (*(f32*)((s32)mario + 0x1C0) > float_4_8041fd30) {
        threshold = float_3_8041fce0;
    } else {
        threshold = float_1p5_8041fcac;
    }

    if (len > threshold) {
        amount = float_2_8041fd34 / len;
        pushX *= amount;
        pushZ *= amount;
    }

    *(f32*)((s32)mario + 0x8C) += pushX;
    *(f32*)((s32)mario + 0x94) += pushZ;

    marioChkWallAround(
        (void*)((s32)mario + 0x8C),
        0,
        float_0_8041fc7c,
        float_160_8041fd38 + angle,
        float_20_8041fd3c,
        *(f32*)((s32)mario + 0x1BC));

    return 1;
}
#pragma opt_common_subs reset

void npcMoveG(void* pNpc) {
    extern void* hitCheckFilter(double, double, double, double, double, double, s32, void*, void*, void*, void*, void*, void*, void*);
    extern f32 float_0_8041fc7c;
    extern f32 float_neg1_8041fcd8;
    extern f32 float_0p5_8041fc80;
    extern f32 float_0p8_8041fc90;
    extern f32 float_0p4_8041fcf4;
    extern f32 float_neg980_8041fcfc;
    extern f32 float_0p15_8041fd00;
    extern f32 float_neg1000_8041fd04;
    extern f64 fabs(f64 value);
    f32 oldYVel;
    f32 addY;
    f32 walking;
    f32 remain;
    f32 minAdd;
    f32 heightPart;
    f32 hitY;
    f32 hitDist;
    s32 hitTmp;
    s32 hitA;
    s32 hitB;
    s32 hitC;
    s32 hitD;
    void* hit;
    u32 jumpFlags;
    u32 dampedJump;

    oldYVel = *(f32*)((s32)pNpc + 0x1B8);
    *(f32*)((s32)pNpc + 0x1B8) = float_0_8041fc7c;
    jumpFlags = *(u32*)((s32)pNpc + 0x1D4);
    dampedJump = jumpFlags & 0x100000;
    if (dampedJump != 0 && (jumpFlags & 0x10000000) == 0) {
        oldYVel *= float_0p5_8041fc80;
    }

    addY = float_0_8041fc7c;
    if ((*(u32*)pNpc & 0x20000) != 0) {
        remain = *(f32*)((s32)pNpc + 0x180);
        walking = *(f32*)((s32)pNpc + 0x1A4);
        if (dampedJump == 0 || (jumpFlags & 0x10000000) != 0) {
            addY = *(f32*)((s32)pNpc + 0x1C4) * walking +
                   float_neg980_8041fcfc * *(f32*)((s32)pNpc + 0x1CC) * (remain * remain - (remain - walking) * (remain - walking));
        } else {
            addY = float_0p4_8041fcf4 * *(f32*)((s32)pNpc + 0x1C4) * walking +
                   float_0p15_8041fd00 * float_neg980_8041fcfc * *(f32*)((s32)pNpc + 0x1CC) * (remain * remain - (remain - walking) * (remain - walking));
        }
        addY *= *(f32*)((s32)pNpc + 0x1C8);
        if (*(f32*)((s32)pNpc + 0x1D0) > float_0_8041fc7c) {
            if (dampedJump == 0) {
                minAdd = -*(f32*)((s32)pNpc + 0x1D0) * walking;
            } else {
                minAdd = float_0p5_8041fc80 * -*(f32*)((s32)pNpc + 0x1D0) * walking;
            }
            if (addY < minAdd) {
                addY = minAdd;
            }
        }
    }

    oldYVel += addY;
    if ((*(u32*)pNpc & 0x10) == 0) {
        *(f32*)((s32)pNpc + 0x90) += oldYVel;
        if (*(f32*)((s32)pNpc + 0x90) < float_neg1000_8041fd04) {
            *(f32*)((s32)pNpc + 0x90) = float_neg1000_8041fd04;
        }
    } else if (oldYVel != float_0_8041fc7c) {
        if (oldYVel > float_0_8041fc7c) {
            *(u32*)((s32)pNpc + 0x1D4) |= 1;
            *(f32*)((s32)pNpc + 0x90) += oldYVel;
        } else {
            heightPart = float_0p8_8041fc90 * *(f32*)((s32)pNpc + 0x150);
            hitDist = heightPart + (f32)fabs((f64)oldYVel);
            hit = hitCheckFilter((double)*(f32*)((s32)pNpc + 0x8C),
                                 (double)(*(f32*)((s32)pNpc + 0x90) + heightPart),
                                 (double)*(f32*)((s32)pNpc + 0x94),
                                 float_0_8041fc7c, float_neg1_8041fcd8, float_0_8041fc7c, 0,
                                 &hitTmp, &hitY, &hitA, &hitDist, &hitB, &hitC, &hitD);
            *(void**)((s32)pNpc + 0x300) = hit;
            if (hit != 0) {
                *(u32*)((s32)pNpc + 0x1D4) &= ~1;
                *(f32*)((s32)pNpc + 0x90) = hitY;
                *(s32*)((s32)pNpc + 0x17C) = 0;
                *(s32*)((s32)pNpc + 0x178) = 0;
                return;
            }

            hit = hitCheckFilter((double)(float_0p5_8041fc80 * *(f32*)((s32)pNpc + 0x14C) + *(f32*)((s32)pNpc + 0x8C)),
                                 (double)(*(f32*)((s32)pNpc + 0x90) + heightPart),
                                 (double)*(f32*)((s32)pNpc + 0x94),
                                 float_0_8041fc7c, float_neg1_8041fcd8, float_0_8041fc7c, 0,
                                 &hitTmp, &hitY, &hitA, &hitDist, &hitB, &hitC, &hitD);
            *(void**)((s32)pNpc + 0x300) = hit;
            if (hit != 0) {
                *(u32*)((s32)pNpc + 0x1D4) &= ~1;
                *(f32*)((s32)pNpc + 0x90) = hitY;
                *(s32*)((s32)pNpc + 0x17C) = 0;
                *(s32*)((s32)pNpc + 0x178) = 0;
                return;
            }

            hit = hitCheckFilter((double)(*(f32*)((s32)pNpc + 0x8C) - float_0p5_8041fc80 * *(f32*)((s32)pNpc + 0x14C)),
                                 (double)(*(f32*)((s32)pNpc + 0x90) + heightPart),
                                 (double)*(f32*)((s32)pNpc + 0x94),
                                 float_0_8041fc7c, float_neg1_8041fcd8, float_0_8041fc7c, 0,
                                 &hitTmp, &hitY, &hitA, &hitDist, &hitB, &hitC, &hitD);
            *(void**)((s32)pNpc + 0x300) = hit;
            if (hit != 0) {
                *(u32*)((s32)pNpc + 0x1D4) &= ~1;
                *(f32*)((s32)pNpc + 0x90) = hitY;
                *(s32*)((s32)pNpc + 0x17C) = 0;
                *(s32*)((s32)pNpc + 0x178) = 0;
                return;
            }

            *(u32*)((s32)pNpc + 0x1D4) |= 1;
            *(f32*)((s32)pNpc + 0x90) += oldYVel;
        }
    }
    return;
}

void npcTuningRy(void* npc, f32 angle) {
    extern f32 reviseAngle(f32 angle);
    *(f32*)((s32)npc + 0x144) = reviseAngle(angle);
}

f32 npcTransRytoFaceDir(void* npc) {
    extern f32 reviseAngle(f32 angle);
    extern f32 float_180_8041fccc;
    extern f32 float_90_8041fcb0;
    extern f32 float_270_8041fcc4;
    f32 angle = reviseAngle(*(f32*)((s32)npc + 0x144));
    if (angle < float_180_8041fccc) {
        return float_90_8041fcb0;
    }
    return float_270_8041fcc4;
}

#pragma opt_common_subs off
s32 npcHitCheckSide(void* npc, f32 moveX, f32 moveZ, f32* outX, f32* outZ, f32* outAngle) {
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern f32 reviseAngle(f32 angle);
    extern s32 hitCheckFilter(f64 x, f64 y, f64 z, f64 dx, f64 dy, f64 dz, s32 flags,
                              void* outA, void* outB, void* outC, void* dist,
                              void* outNX, void* outNY, void* outNZ);
    extern s32 hitCheckAttr(f64 x, f64 y, f64 z, f64 dx, f64 dy, f64 dz, s32 attr,
                            void* outA, void* outB, void* outC, void* dist,
                            void* outNX, void* outNY, void* outNZ);
    extern f32 __float_nan[];
    extern const u8 vec3_802c11f8[];

    extern f32 float_0_8041fc7c;
    extern f32 float_0p75_8041fcd4;
    extern f32 float_0p2_8041fcec;
    extern f32 float_10_8041fc98;
    extern f32 float_0p5_8041fc80;
    extern f32 float_180_8041fccc;
    extern f32 float_100_8041fcf0;
    extern f32 float_0p4_8041fcf4;
    extern f32 float_45_8041fcf8;

    f32 sinDir;
    f32 cosDir;
    f32 lenSq;
    f32 moveLen;
    f32 nyAdd;
    struct {
        f32 nz;
        f32 ny;
        f32 nx;
        f32 dist;
        s32 outC;
        s32 outB;
        s32 outA;
    } hitOut;
    s32 hit;
    f32 dot;
    f32 normalLenSq;
    f32 normalLen;
    f32 slopeAngle;
    f64 d;
    f64 inv;
    const u8* constBase;

    constBase = vec3_802c11f8;

    *outX = *(f32*)((s32)npc + 0x8C);
    *outZ = *(f32*)((s32)npc + 0x94);

    if (moveX == float_0_8041fc7c && moveZ == float_0_8041fc7c) {
        return 0;
    }

    sincosf(angleABf(float_0_8041fc7c, float_0_8041fc7c, moveX, moveZ), &sinDir, &cosDir);

    lenSq = moveX * moveX + moveZ * moveZ;
    if (lenSq > float_0_8041fc7c) {
        d = (f64)lenSq;
        inv = __frsqrte(d);
        inv = *(f64*)(constBase + 0x1D0) * inv *
              (*(f64*)(constBase + 0x1D8) - d * inv * inv);
        inv = *(f64*)(constBase + 0x1D0) * inv *
              (*(f64*)(constBase + 0x1D8) - d * inv * inv);
        inv = *(f64*)(constBase + 0x1D0) * inv *
              (*(f64*)(constBase + 0x1D8) - d * inv * inv);
        moveLen = (f32)(d * inv);
    } else if ((f64)lenSq < *(f64*)(constBase + 0x1E0)) {
        moveLen = __float_nan[0];
    } else {
        u32 bits = *(u32*)&lenSq;
        u32 exponent = bits & 0x7F800000;
        s32 kind;
        switch (exponent) {
        case 0x7F800000:
            if (bits & 0x7FFFFF) kind = 1; else kind = 2;
            break;
        case 0:
            if (bits & 0x7FFFFF) kind = 5; else kind = 3;
            break;
        default: kind = 4; break;
        }
        moveLen = lenSq;
        if (kind == 1) {
            moveLen = __float_nan[0];
        }
    }

    hitOut.dist = float_0p75_8041fcd4 * *(f32*)((s32)npc + 0x14C) + moveLen;
    nyAdd = float_0p2_8041fcec * *(f32*)((s32)npc + 0x150);
    if (nyAdd > float_10_8041fc98) {
        nyAdd = float_10_8041fc98;
    }

    if (*(u32*)npc & 0x2000) {
        hit = hitCheckAttr(*(f32*)((s32)npc + 0x8C),
                           *(f32*)((s32)npc + 0x90) + nyAdd,
                           *(f32*)((s32)npc + 0x94), sinDir, float_0_8041fc7c, cosDir, 4,
                           &hitOut.outA, &hitOut.outB, &hitOut.outC, &hitOut.dist,
                           &hitOut.nx, &hitOut.ny, &hitOut.nz);
    } else {
        hit = hitCheckFilter(*(f32*)((s32)npc + 0x8C),
                             *(f32*)((s32)npc + 0x90) + nyAdd,
                             *(f32*)((s32)npc + 0x94), sinDir, float_0_8041fc7c, cosDir, 0,
                             &hitOut.outA, &hitOut.outB, &hitOut.outC, &hitOut.dist,
                             &hitOut.nx, &hitOut.ny, &hitOut.nz);
    }

    if (hit != 0) {
        dot = moveX * hitOut.nx + moveZ * hitOut.nz;
        *outX += float_0p5_8041fc80 * -(dot * hitOut.nx - moveX);
        *outZ += float_0p5_8041fc80 * -(dot * hitOut.nz - moveZ);

        if (*(u32*)npc & 0x10000) {
            f32 baseAngle;
            f32 normalAngle;
            f32 delta;
            baseAngle = reviseAngle(float_180_8041fccc + *outAngle);
            normalAngle = reviseAngle(angleABf(float_0_8041fc7c, float_0_8041fc7c,
                                               float_100_8041fcf0 * hitOut.nx,
                                               float_100_8041fcf0 * hitOut.nz));
            delta = reviseAngle(normalAngle - baseAngle);
            *outAngle = (f32)(s32)reviseAngle(normalAngle + delta);
        }

        return hit;
    }

    if (lenSq > float_0_8041fc7c) {
        d = (f64)lenSq;
        inv = __frsqrte(d);
        inv = *(f64*)(constBase + 0x1D0) * inv *
              (*(f64*)(constBase + 0x1D8) - d * inv * inv);
        inv = *(f64*)(constBase + 0x1D0) * inv *
              (*(f64*)(constBase + 0x1D8) - d * inv * inv);
        inv = *(f64*)(constBase + 0x1D0) * inv *
              (*(f64*)(constBase + 0x1D8) - d * inv * inv);
        moveLen = (f32)(d * inv);
    } else if ((f64)lenSq < *(f64*)(constBase + 0x1E0)) {
        moveLen = __float_nan[0];
    } else {
        u32 bits = *(u32*)&lenSq;
        u32 exponent = bits & 0x7F800000;
        s32 kind;
        switch (exponent) {
        case 0x7F800000:
            if (bits & 0x7FFFFF) kind = 1; else kind = 2;
            break;
        case 0:
            if (bits & 0x7FFFFF) kind = 5; else kind = 3;
            break;
        default: kind = 4; break;
        }
        moveLen = lenSq;
        if (kind == 1) {
            moveLen = __float_nan[0];
        }
    }
    hitOut.dist = float_0p75_8041fcd4 * *(f32*)((s32)npc + 0x14C) + moveLen;
    nyAdd = float_0p4_8041fcf4 * *(f32*)((s32)npc + 0x150);

    if (*(u32*)npc & 0x2000) {
        hit = hitCheckAttr(*(f32*)((s32)npc + 0x8C),
                           *(f32*)((s32)npc + 0x90) + nyAdd,
                           *(f32*)((s32)npc + 0x94), sinDir, float_0_8041fc7c, cosDir, 4,
                           &hitOut.outA, &hitOut.outB, &hitOut.outC, &hitOut.dist,
                           &hitOut.nx, &hitOut.ny, &hitOut.nz);
    } else {
        hit = hitCheckFilter(*(f32*)((s32)npc + 0x8C),
                             *(f32*)((s32)npc + 0x90) + nyAdd,
                             *(f32*)((s32)npc + 0x94), sinDir, float_0_8041fc7c, cosDir, 0,
                             &hitOut.outA, &hitOut.outB, &hitOut.outC, &hitOut.dist,
                             &hitOut.nx, &hitOut.ny, &hitOut.nz);
    }

    if (hit != 0) {
        normalLenSq = hitOut.nx * hitOut.nx + hitOut.nz * hitOut.nz;
        if (normalLenSq > float_0_8041fc7c) {
            d = (f64)normalLenSq;
            inv = __frsqrte(d);
            inv = *(f64*)(constBase + 0x1D0) * inv *
                  (*(f64*)(constBase + 0x1D8) - d * inv * inv);
            inv = *(f64*)(constBase + 0x1D0) * inv *
                  (*(f64*)(constBase + 0x1D8) - d * inv * inv);
            inv = *(f64*)(constBase + 0x1D0) * inv *
                  (*(f64*)(constBase + 0x1D8) - d * inv * inv);
            normalLen = (f32)(d * inv);
        } else if ((f64)normalLenSq < *(f64*)(constBase + 0x1E0)) {
            normalLen = __float_nan[0];
        } else {
            u32 bits = *(u32*)&normalLenSq;
            u32 exponent = bits & 0x7F800000;
            s32 kind;
            switch (exponent) {
            case 0x7F800000:
                if (bits & 0x7FFFFF) kind = 1; else kind = 2;
                break;
            case 0:
                if (bits & 0x7FFFFF) kind = 5; else kind = 3;
                break;
            default: kind = 4; break;
            }
            normalLen = normalLenSq;
            if (kind == 1) {
                normalLen = __float_nan[0];
            }
        }

        slopeAngle = angleABf(float_0_8041fc7c, float_0_8041fc7c, normalLen, -hitOut.ny);
        if (slopeAngle >= float_45_8041fcf8) {
            dot = moveX * hitOut.nx + moveZ * hitOut.nz;
            *outX += float_0p5_8041fc80 * -(dot * hitOut.nx - moveX);
            *outZ += float_0p5_8041fc80 * -(dot * hitOut.nz - moveZ);

            if (*(u32*)npc & 0x10000) {
                f32 baseAngle;
                f32 normalAngle;
                f32 delta;
                baseAngle = reviseAngle(float_180_8041fccc + *outAngle);
                normalAngle = reviseAngle(angleABf(float_0_8041fc7c, float_0_8041fc7c,
                                                   float_100_8041fcf0 * hitOut.nx,
                                                   float_100_8041fcf0 * hitOut.nz));
                delta = reviseAngle(normalAngle - baseAngle);
                *outAngle = (f32)(s32)reviseAngle(normalAngle + delta);
            }

            return hit;
        }
    }

    if (lenSq > float_0_8041fc7c) {
        d = (f64)lenSq;
        inv = __frsqrte(d);
        inv = *(f64*)(constBase + 0x1D0) * inv *
              (*(f64*)(constBase + 0x1D8) - d * inv * inv);
        inv = *(f64*)(constBase + 0x1D0) * inv *
              (*(f64*)(constBase + 0x1D8) - d * inv * inv);
        inv = *(f64*)(constBase + 0x1D0) * inv *
              (*(f64*)(constBase + 0x1D8) - d * inv * inv);
        moveLen = (f32)(d * inv);
    } else if ((f64)lenSq < *(f64*)(constBase + 0x1E0)) {
        moveLen = __float_nan[0];
    } else {
        u32 bits = *(u32*)&lenSq;
        u32 exponent = bits & 0x7F800000;
        s32 kind;
        switch (exponent) {
        case 0x7F800000:
            if (bits & 0x7FFFFF) kind = 1; else kind = 2;
            break;
        case 0:
            if (bits & 0x7FFFFF) kind = 5; else kind = 3;
            break;
        default: kind = 4; break;
        }
        moveLen = lenSq;
        if (kind == 1) {
            moveLen = __float_nan[0];
        }
    }
    hitOut.dist = float_0p75_8041fcd4 * *(f32*)((s32)npc + 0x14C) + moveLen;
    nyAdd = float_0p75_8041fcd4 * *(f32*)((s32)npc + 0x150);

    if (*(u32*)npc & 0x2000) {
        hit = hitCheckAttr(*(f32*)((s32)npc + 0x8C),
                           *(f32*)((s32)npc + 0x90) + nyAdd,
                           *(f32*)((s32)npc + 0x94), sinDir, float_0_8041fc7c, cosDir, 4,
                           &hitOut.outA, &hitOut.outB, &hitOut.outC, &hitOut.dist,
                           &hitOut.nx, &hitOut.ny, &hitOut.nz);
    } else {
        hit = hitCheckFilter(*(f32*)((s32)npc + 0x8C),
                             *(f32*)((s32)npc + 0x90) + nyAdd,
                             *(f32*)((s32)npc + 0x94), sinDir, float_0_8041fc7c, cosDir, 0,
                             &hitOut.outA, &hitOut.outB, &hitOut.outC, &hitOut.dist,
                             &hitOut.nx, &hitOut.ny, &hitOut.nz);
    }

    if (hit == 0) {
        *outX += moveX;
        *outZ += moveZ;
        return 0;
    }

    dot = moveX * hitOut.nx + moveZ * hitOut.nz;
    *outX += float_0p5_8041fc80 * -(dot * hitOut.nx - moveX);
    *outZ += float_0p5_8041fc80 * -(dot * hitOut.nz - moveZ);

    if (*(u32*)npc & 0x10000) {
        f32 baseAngle;
        f32 normalAngle;
        f32 delta;
        baseAngle = reviseAngle(float_180_8041fccc + *outAngle);
        normalAngle = reviseAngle(angleABf(float_0_8041fc7c, float_0_8041fc7c,
                                           float_100_8041fcf0 * hitOut.nx,
                                           float_100_8041fcf0 * hitOut.nz));
        delta = reviseAngle(normalAngle - baseAngle);
        *outAngle = (f32)(s32)reviseAngle(normalAngle + delta);
    }

    return hit;
}
#pragma opt_common_subs reset

void* npcNearDistCheck(f32 x, f32 y, f32 z, f32 radius) {
    extern f32 __float_nan[];
    extern const u8 vec3_802c11f8[];
    extern const f32 float_0_8041fc7c;
    u8* workPtr;
    s32 count;
    s32 i;
    u8* npc;
    void* found;
    const u8* constBase;

    constBase = vec3_802c11f8;

    workPtr = (u8*)&work;
    if (gp->inBattle != 0) {
        workPtr += 0x14;
    }

    count = *(s32*)(workPtr + 4);
    found = 0;
    npc = *(u8**)(workPtr + 0xC);

    for (i = 0; i < count; i++) {
        if ((*(u32*)npc & 1) != 0) {
                f32 dx;
                f32 dy;
                f32 dz;
                f32 dxSq;
                f32 dzSq;
                f32 xzSq;
                f32 xzDist;

                dx = x - *(f32*)(npc + 0x8C);
                dy = y - *(f32*)(npc + 0x90);
                dz = z - *(f32*)(npc + 0x94);
                dxSq = dx * dx;
                dzSq = dz * dz;
                xzSq = dxSq + dzSq;

                if (xzSq > float_0_8041fc7c) {
                    f64 value;
                    f64 inv;

                    value = (f64)xzSq;
                    inv = __frsqrte(value);
                    inv = *(f64*)(constBase + 0x1D0) * inv *
                          (*(f64*)(constBase + 0x1D8) - value * inv * inv);
                    inv = *(f64*)(constBase + 0x1D0) * inv *
                          (*(f64*)(constBase + 0x1D8) - value * inv * inv);
                    inv = *(f64*)(constBase + 0x1D0) * inv *
                          (*(f64*)(constBase + 0x1D8) - value * inv * inv);
                    xzDist = (f32)(value * inv);
                } else if ((f64)xzSq < *(f64*)(constBase + 0x1E0)) {
                    xzDist = __float_nan[0];
                } else {
                    u32 bits = *(u32*)&xzSq;
                    u32 exponent = bits & 0x7F800000;
                    s32 kind;

                    switch (exponent) {
                    case 0x7F800000:
                        if (bits & 0x7FFFFF) kind = 1; else kind = 2;
                        break;
                    case 0:
                        if (bits & 0x7FFFFF) kind = 5; else kind = 3;
                        break;
                    default: kind = 4; break;
                    }
                    if (kind == 1) {
                        xzDist = __float_nan[0];
                    } else {
                        xzDist = xzSq;
                    }
                }

            if ((dxSq + dy * dy + dzSq) >= float_0_8041fc7c && xzDist < radius) {
                found = npc;
            }
        }

        npc += 0x340;
    }

    return found;
}

void* fbatGetPointer(void) {
    return *(void**)((s32)gp + 0x168);
}

void fbatChangeMode(s16 mode) {
    *(s16*)*(void**)((s32)gp + 0x168) = mode;
}

u8 fbatSetAttackAnnounce(s32 flag) {
    extern s32 dat_8041fc70;
    extern s32 dat_8041fc74;
    s32 data;

    data = *(s32*)((s32)gp + 0x168);
    *(s32*)(data + 0x550) = 0;
    *(s32*)(data + 0x554) = flag;
    *(s32*)(data + 0x55C) = 0;
    switch (*(s32*)(data + 0x554)) {
        case 0x20000:
        case 0x40000:
        case 0x80000:
        case 0x100000:
        case 0x200000:
        case 0x400000:
        case 0x800000:
        case 0x1000000:
        case 0x2000000:
            *(char**)(data + 0x568) = "fb_sensei_shita";
            *(s32*)(data + 0x558) = 2;
            *(s32*)(data + 0x56C) = dat_8041fc70;
            break;
        case 0x10000000:
            *(char**)(data + 0x568) = "fb_sensei_sareta";
            *(s32*)(data + 0x558) = 1;
            *(s32*)(data + 0x56C) = dat_8041fc74;
            break;
        default:
            *(s32*)(data + 0x558) = 0;
            break;
    }
}

void fbatSetAttackAnnounceEnable(void) {
    *(s32*)((s32)*(void**)((s32)gp + 0x168) + 0x550) = 1;
}

void _fbatFirstAttackAnnouceDisp(s32 param_1, void* param_2) {
    extern char* msgSearch(char*);
    extern u32 FontGetMessageWidthLine(char*, u16*);
    extern void windowDispGX_Waku_col(double, double, double, double, double, u16, u32*);
    extern void FontDrawStart(void);
    extern void FontDrawEdge(void);
    extern void FontDrawColor(u32*);
    extern void FontDrawScale(f32);
    extern void FontDrawStringShake(double, double, char*);
    extern const f32 float_0p1_8041fcdc;
    extern const f32 float_1_8041fc94;
    extern const f32 float_3_8041fce0;
    extern const f32 float_10_8041fc98;
    extern u32 dat_8041fc78;
    char lines[512];
    u32 color;
    u32 fontColor;
    u16 lineCount[2];
    char* msg;
    char* out;
    char* dst;
    s32 i;
    s32 j;
    s32 height;
    s32 yOffset;
    s32 width;
    s32 halfWidth;
    f32 windowX;
    f32 windowY;
    f32 x;
    f32 y;
    s8 c;

    msg = msgSearch(*(char**)((s32)param_2 + 0x18));
    x = *(f32*)((s32)param_2 + 0x10);
    y = *(f32*)((s32)param_2 + 0x14);
    width = FontGetMessageWidthLine(msg, lineCount);
    lineCount[0]++;
    height = lineCount[0] * 0x1D + 3;
    windowX = x + (f32)(irand(10000) % 5);
    windowY = y + (f32)(irand(10000) % 5);
    if (lineCount[0] > 4) {
        lineCount[0] = 4;
    }
    out = lines;
    dst = out;
    for (i = 0; i < lineCount[0]; i++, dst += 0x80) {
        for (j = 0; j < 0x80; j++) {
            c = *msg;
            if (c == 0 || c == '\n') {
                dst[j] = 0;
                msg++;
                break;
            }
            dst[j] = c;
            msg++;
        }
    }

    halfWidth = (width >> 1) & 0x7FFF;
    color = *(u32*)((s32)param_2 + 0x1C);
    windowDispGX_Waku_col(
        (double)(float_0p1_8041fcdc * ((windowX - float_10_8041fc98) - (f32)halfWidth)),
        (double)(windowY * float_0p1_8041fcdc),
        (double)((f32)((width & 0xFFFF) + 0x14) * float_0p1_8041fcdc),
        (double)((f32)height * float_0p1_8041fcdc),
        (double)float_1_8041fc94,
        0,
        &color);
    FontDrawStart();
    FontDrawEdge();
    fontColor = dat_8041fc78;
    FontDrawColor(&fontColor);
    FontDrawScale(float_0p1_8041fcdc);
    x = float_0p1_8041fcdc * (x - (f32)halfWidth);
    out = lines;
    yOffset = 0;
    for (i = 0; i < lineCount[0]; i++, yOffset -= 0x1D) {
        FontDrawStringShake((double)x,
                            (double)(float_0p1_8041fcdc *
                                     ((y + (f32)yOffset) - float_3_8041fce0)),
                            out);
        out += 0x80;
    }
    return;
}

void fbatEncountCheck(void) {
    extern s32 marioStGetSystemLevel(void);
    extern s32 marioKeyOffChk(void);
    extern s32 marioCtrlOffChk(void);
    extern s32 N_itemPickUpFromFieldCheck(void);
    extern void marioChgMotJump2(void);
    extern f32 angleABf(f64 x0, f64 z0, f64 x1, f64 z1);
    extern f32 reviseAngle(f64 angle);
    extern void effSmallStarEntry();
    extern f32 marioGetDispDir(void);
    extern f64 __frsqrte(f64 value);
    extern f32 __float_nan[];
    extern const u8 vec3_802c11f8[];
    extern s32 hitCheckFilter(f64 x, f64 y, f64 z, f64 dx, f64 dy, f64 dz, s32 flags,
                              void* outX, void* outY, void* outZ, void* dist,
                              void* outNX, void* outNY, void* outNZ);
    extern u16 keyGetButtonTrg(s32 controller);
    extern void npcKoopaModeEncountCheck(void);

    extern s32 dbg_battleskip;
    extern f32 float_1000_8041fca8;
    extern f32 float_0p5_8041fc80;
    extern f32 float_10_8041fc98;
    extern f32 float_0_8041fc7c;
    extern f32 float_0p75_8041fcd4;
    extern f32 float_neg90_8041fcb8;
    extern f32 float_rad2deg_8041fcbc;
    extern f32 float_360_8041fcc0;
    extern f32 float_90_8041fcb0;
    extern f32 float_270_8041fcc4;
    extern f32 float_neg270_8041fcc8;
    extern f32 float_180_8041fccc;
    extern f32 float_neg180_8041fcd0;
    extern f32 float_neg1_8041fcd8;

    void* npcWork;
    void* player;
    void* fbatData;
    void* hitInfo;
    void* npc;
    void* pose;
    void* cam;
    void* npcEntry;
    void* nearestNpc;
    void* talkNpc;
    void* player2;
    s32 maxCount;
    s32 i;
    s32 talkable;
    s32 facing;
    s32 hit;
    s32 modeSet;
    u32 flags;
    f32 bestDist;
    f32 dist;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 outNX;
    f32 outNY;
    f32 outNZ;
    f32 angle;
    f32 dispDir;
    f32 camDir;
    f32 relAngle;
    f32 targetRot;
    f32 delta;
    f32 sinv;
    f32 cosv;
    f32 localPos[3];
    const u8* constBase;

    constBase = vec3_802c11f8;
    npcWork = (void*)&work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        npcWork = (void*)((s32)npcWork + 0x14);
    }

    maxCount = *(s32*)((s32)npcWork + 4);
    player = marioGetPtr();
    *(void**)((s32)npcWork + 0x10) = 0;

    if ((u32)marioStGetSystemLevel() != 0U) {
        return;
    }

    if ((*(u32*)((s32)npcWork + 8) & 1) == 0) {
        if (marioKeyOffChk() != 0) {
            return;
        }
        if (marioCtrlOffChk() != 0) {
            return;
        }
    }

    if (N_itemPickUpFromFieldCheck() != 0) {
        return;
    }

    fbatData = *(void**)((s32)gp + 0x168);
    hitInfo = (void*)((s32)fbatData + 0x10);
    npc = ((void* (*)(s32, void*))fbatHitCheck)(0x13F1, hitInfo);

    if (npc != 0 && *(s16*)fbatData == 1) {
        fbatData = *(void**)((s32)gp + 0x168);
        if (*(s16*)fbatData == 1) {
            *(s16*)fbatData = 5;
            *(u8*)((s32)fbatData + 2) = 0;
            *(void**)((s32)fbatData + 4) = npc;
            *(void**)((s32)fbatData + 8) = npc;
            modeSet = 1;
        } else {
            modeSet = 0;
        }

        if (modeSet == 0) {
            return;
        }

        flags = *(u32*)hitInfo;

        switch (flags) {
            case 0x10000:
                break;
            case 0x20000:
            case 0x40000:
            case 0x80000:
                marioChgMotJump2();
                if (dbg_battleskip != 1) {
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), *(void**)(*(s32*)((s32)npc + 0x28) + 0x20), 1);
                }
                break;
            case 0x100000:
            case 0x200000:
            case 0x400000:
                if (dbg_battleskip != 1) {
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), *(void**)(*(s32*)((s32)npc + 0x28) + 0x20), 1);
                }
                break;
            case 0x800000:
                if (dbg_battleskip != 1) {
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), *(void**)(*(s32*)((s32)npc + 0x28) + 0x20), 1);
                }
                break;
            case 0x1000000:
                if (dbg_battleskip != 1) {
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), *(void**)(*(s32*)((s32)npc + 0x28) + 0x20), 1);
                }
                break;
        }

        pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
        *(f32*)((s32)npc + 0x148) = *(f32*)((s32)npc + 0x144);

        angle = angleABf(*(f32*)((s32)npc + 0x8C), *(f32*)((s32)npc + 0x94),
                         *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
        *(f32*)((s32)npc + 0x144) = angle;

        targetRot = reviseAngle(-*(f32*)((s32)npc + 0x144)) + float_neg90_8041fcb8;

        cam = camGetPtr(4);
        camDir = (f32)atan2(-(*(f32*)((s32)cam + 0x18) - *(f32*)((s32)cam + 0xC)),
                            -(*(f32*)((s32)cam + 0x20) - *(f32*)((s32)cam + 0x14)));
        camDir = reviseAngle(float_rad2deg_8041fcbc * camDir);
        if (camDir < float_0_8041fc7c) {
            camDir += float_360_8041fcc0;
        }

        relAngle = reviseAngle(camDir - targetRot);
        if ((relAngle > float_90_8041fcb0 && relAngle <= float_270_8041fcc4) ||
            (relAngle < float_neg90_8041fcb8 && relAngle >= float_neg270_8041fcc8)) {
            cam = camGetPtr(4);
            camDir = (f32)atan2(-(*(f32*)((s32)cam + 0x18) - *(f32*)((s32)cam + 0xC)),
                                -(*(f32*)((s32)cam + 0x20) - *(f32*)((s32)cam + 0x14)));
            camDir = reviseAngle(float_rad2deg_8041fcbc * camDir);
            if (camDir < float_0_8041fc7c) {
                camDir += float_360_8041fcc0;
            }
            *(f32*)((s32)pose + 0x7C) = reviseAngle(float_180_8041fccc + camDir);
        } else {
            cam = camGetPtr(4);
            camDir = (f32)atan2(-(*(f32*)((s32)cam + 0x18) - *(f32*)((s32)cam + 0xC)),
                                -(*(f32*)((s32)cam + 0x20) - *(f32*)((s32)cam + 0x14)));
            camDir = reviseAngle(float_rad2deg_8041fcbc * camDir);
            if (camDir < float_0_8041fc7c) {
                camDir += float_360_8041fcc0;
            }
            *(f32*)((s32)pose + 0x7C) = camDir;
        }

        while (float_180_8041fccc < (*(f32*)((s32)pose + 0x7C) - *(f32*)((s32)pose + 0x74))) {
            *(f32*)((s32)pose + 0x74) += float_360_8041fcc0;
        }
        while (float_neg180_8041fcd0 > (*(f32*)((s32)pose + 0x7C) - *(f32*)((s32)pose + 0x74))) {
            *(f32*)((s32)pose + 0x74) -= float_360_8041fcc0;
        }
        delta = *(f32*)((s32)pose + 0x7C) - *(f32*)((s32)pose + 0x74);
        *(f32*)((s32)pose + 0x74) += float_0p75_8041fcd4 * delta;

        flags = *(u32*)hitInfo;
        switch (flags) {
            case 0x20000:
            case 0x40000:
            case 0x80000:
            case 0x100000:
            case 0x200000:
            case 0x400000:
            case 0x800000:
            case 0x1000000:
                effSmallStarEntry(3, 3, *(f32*)((s32)npc + 0x8C),
                                  *(f32*)((s32)npc + 0x90) + *(f32*)((s32)npc + 0x150),
                                  *(f32*)((s32)npc + 0x94), float_0_8041fc7c,
                                  float_neg1_8041fcd8, float_0_8041fc7c);
                break;
            case 0x10000000:
                effSmallStarEntry(3, 3, *(f32*)((s32)player + 0x8C),
                                  *(f32*)((s32)player + 0x90) + *(f32*)((s32)player + 0x1BC),
                                  *(f32*)((s32)player + 0x94), float_0_8041fc7c,
                                  float_neg1_8041fcd8, float_0_8041fc7c);
                break;
        }

        flags = *(u32*)hitInfo;
        switch (flags) {
            case 0x20000:
            case 0x40000:
            case 0x80000:
            case 0x100000:
            case 0x200000:
            case 0x400000:
            case 0x800000:
            case 0x1000000:
                if (npc != 0) {
                    psndSFXOn_3D("SFX_BTL_FLD_ENEMY_DAMAGED1", (void*)((s32)npc + 0x8C));
                }
                break;
            case 0x10000000:
                psndSFXOn_3D("SFX_BTL_FLD_MARIO_DAMAGED1", (void*)((s32)player + 0x8C));
                break;
        }
        return;
    }

    bestDist = float_1000_8041fca8;

    if (marioChkTalkable() != 0 && *(s8*)((s32)player + 0x39) == 0) {
        npcEntry = *(void**)((s32)npcWork + 0xC);
        nearestNpc = 0;
        i = 0;
        while (i < maxCount) {
            if ((*(u32*)npcEntry & 2) != 0 &&
                (*(u32*)npcEntry & 8) == 0 &&
                *(void**)((s32)npcEntry + 0x128) != 0) {
                talkable = 0;

                if (*(f32*)((s32)npcEntry + 0x90) <=
                        *(f32*)((s32)player + 0x90) + *(f32*)((s32)player + 0x1BC) &&
                    *(f32*)((s32)player + 0x90) <=
                        *(f32*)((s32)npcEntry + 0x90) + *(f32*)((s32)npcEntry + 0x150)) {
                    localPos[0] = *(f32*)((s32)player + 0x8C);
                    localPos[1] = *(f32*)((s32)npcEntry + 0x90);
                    localPos[2] = *(f32*)((s32)player + 0x94);
                    dist = PSVECDistance(localPos, (void*)((s32)npcEntry + 0x8C));
                    if (dist <= float_0p5_8041fc80 *
                                    (*(f32*)((s32)player + 0x1B8) +
                                     *(f32*)((s32)npcEntry + 0x14C)) +
                                    float_10_8041fc98 &&
                        dist < bestDist) {
                        nearestNpc = npcEntry;
                    }

                    facing = 0;
                    cam = camGetPtr(4);
                    camDir = *(f32*)((s32)cam + 0x114);
                    dispDir = reviseAngle(marioGetDispDir() - camDir);
                    for (hit = 0; hit < 3; hit++) {
                        if (hit == 1) {
                            cam = camGetPtr(4);
                            camDir = *(f32*)((s32)cam + 0x114);
                            angle = angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                                             *(f32*)((s32)npcEntry + 0x8C) + float_0p5_8041fc80 * *(f32*)((s32)npcEntry + 0x14C),
                                             *(f32*)((s32)npcEntry + 0x94));
                        } else if (hit < 1 && hit >= 0) {
                            cam = camGetPtr(4);
                            camDir = *(f32*)((s32)cam + 0x114);
                            angle = angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                                             *(f32*)((s32)npcEntry + 0x8C), *(f32*)((s32)npcEntry + 0x94));
                        } else {
                            cam = camGetPtr(4);
                            camDir = *(f32*)((s32)cam + 0x114);
                            angle = angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                                             *(f32*)((s32)npcEntry + 0x8C) - float_0p5_8041fc80 * *(f32*)((s32)npcEntry + 0x14C),
                                             *(f32*)((s32)npcEntry + 0x94));
                        }
                        angle = reviseAngle(angle - camDir);
                        if ((float_180_8041fccc <= angle && float_180_8041fccc <= dispDir) ||
                            (angle < float_180_8041fccc && dispDir <= float_180_8041fccc)) {
                            facing = 1;
                            break;
                        }
                    }

                    if (facing != 0) {
                        localPos[0] = *(f32*)((s32)player + 0x8C);
                        localPos[1] = *(f32*)((s32)npcEntry + 0x90);
                        localPos[2] = *(f32*)((s32)player + 0x94);
                        dist = PSVECDistance(localPos, (void*)((s32)npcEntry + 0x8C));
                        if (dist <= float_0p5_8041fc80 * (*(f32*)((s32)player + 0x1B8) + *(f32*)((s32)npcEntry + 0x14C)) + float_10_8041fc98) {
                            f32 dx = *(f32*)((s32)npcEntry + 0x8C) - *(f32*)((s32)player + 0x8C);
                            f32 dz = *(f32*)((s32)npcEntry + 0x94) - *(f32*)((s32)player + 0x94);
                            f32 distanceSq;
                            f32 hitDistance;

                            angle = angleABf(float_0_8041fc7c, float_0_8041fc7c, dx, dz);
                            sincosf(angle, &sinv, &cosv);
                            distanceSq = dz * dz + dx * dx;
                            if (distanceSq > float_0_8041fc7c) {
                                f64 value = (f64)distanceSq;
                                f64 inv = __frsqrte(value);

                                inv = *(f64*)(constBase + 0x1D0) * inv *
                                      (*(f64*)(constBase + 0x1D8) - value * inv * inv);
                                inv = *(f64*)(constBase + 0x1D0) * inv *
                                      (*(f64*)(constBase + 0x1D8) - value * inv * inv);
                                inv = *(f64*)(constBase + 0x1D0) * inv *
                                      (*(f64*)(constBase + 0x1D8) - value * inv * inv);
                                hitDistance = (f32)(value * inv);
                            } else if ((f64)distanceSq < *(f64*)(constBase + 0x1E0)) {
                                hitDistance = __float_nan[0];
                            } else {
                                u32 bits = *(u32*)&distanceSq;
                                u32 exponent = bits & 0x7F800000;
                                s32 kind;

                                switch (exponent) {
                                case 0x7F800000:
                                    if (bits & 0x7FFFFF) kind = 1; else kind = 2;
                                    break;
                                case 0:
                                    if (bits & 0x7FFFFF) kind = 5; else kind = 3;
                                    break;
                                default: kind = 4; break;
                                }
                                hitDistance = distanceSq;
                                if (kind == 1) {
                                    hitDistance = __float_nan[0];
                                }
                            }
                            hit = hitCheckFilter(*(f32*)((s32)player + 0x8C),
                                                 *(f32*)((s32)player + 0x90) + float_0p75_8041fcd4 * *(f32*)((s32)player + 0x1BC),
                                                 *(f32*)((s32)player + 0x94),
                                                 sinv, float_0_8041fc7c, cosv, 0,
                                                 &nx, &ny, &nz, &hitDistance,
                                                 &outNX, &outNY, &outNZ);
                            if (hit == 0 && dist < bestDist) {
                                *(void**)((s32)npcWork + 0x10) = npcEntry;
                                bestDist = dist;
                                talkable = 1;
                            }
                        }
                    }
                }

                if (talkable != 0) {
                    if ((*(u32*)((s32)npcEntry + 0x1D4) & 0x20000000) == 0) {
                        player2 = marioGetPtr();
                        *(u32*)((s32)npcEntry + 0x1D4) |= 0x20000000;
                        *(f32*)((s32)npcEntry + 0x148) = *(f32*)((s32)npcEntry + 0x144);
                        evtStopID(*(s32*)((s32)npcEntry + 0x11C));
                        if ((*(u32*)npcEntry & 0x1000000) == 0) {
                            *(f32*)((s32)npcEntry + 0x144) =
                                reviseAngle(angleABf(*(f32*)((s32)npcEntry + 0x8C),
                                                     *(f32*)((s32)npcEntry + 0x94),
                                                     *(f32*)((s32)player2 + 0x8C),
                                                     *(f32*)((s32)player2 + 0x94)));
                        }
                    }
                } else if ((*(u32*)((s32)npcEntry + 0x1D4) & 0x20000000) != 0) {
                    *(u32*)((s32)npcEntry + 0x1D4) &= ~0x20000000;
                    if ((*(u32*)npcEntry & 0x400000) == 0) {
                        *(u32*)((s32)npcEntry + 0x140) |= 0x10000000;
                    } else {
                        *(f32*)((s32)npcEntry + 0x144) = *(f32*)((s32)npcEntry + 0x148);
                        evtStartID(*(s32*)((s32)npcEntry + 0x11C));
                    }
                }
            }
            i++;
            npcEntry = (void*)((s32)npcEntry + 0x340);
        }

        *(void**)((s32)player + 0x2E8) = nearestNpc;

        talkNpc = *(void**)((s32)npcWork + 0x10);
        if (talkNpc != 0 && (keyGetButtonTrg(0) & 0x100) != 0) {
            fbatData = *(void**)((s32)gp + 0x168);
            if (*(s16*)fbatData != 1) {
                return;
            }
            *(s16*)fbatData = 2;
            *(u8*)((s32)fbatData + 2) = 0;
            *(void**)((s32)fbatData + 4) = talkNpc;
            *(u32*)((s32)talkNpc + 0x318) = 0;
            return;
        }
    }

    fbatData = *(void**)((s32)gp + 0x168);
    if (*(s16*)fbatData != 1) {
        npcKoopaModeEncountCheck();
        return;
    }

    hitInfo = (void*)((s32)fbatData + 0x10);
    npc = ((void* (*)(s32, void*))fbatHitCheck)(0x2361, hitInfo);
    if (npc == 0 || *(s16*)fbatData != 1) {
        npcKoopaModeEncountCheck();
        return;
    }

    flags = *(u32*)hitInfo;
    fbatData = *(void**)((s32)gp + 0x168);
    if (*(s16*)fbatData == 1) {
        *(s16*)fbatData = 3;
        *(u8*)((s32)fbatData + 2) = 0;
        *(void**)((s32)fbatData + 4) = npc;
        *(u32*)((s32)npc + 0x318) = flags;
    }

    flags = *(u32*)hitInfo;
    switch (flags) {
        case 0x20000:
        case 0x40000:
        case 0x80000:
            marioChgMotJump2();
            break;
    }
    effSmallStarEntry(3, 3, *(f32*)((s32)npc + 0x8C),
                      *(f32*)((s32)npc + 0x90) + *(f32*)((s32)npc + 0x150),
                      *(f32*)((s32)npc + 0x94), float_0_8041fc7c,
                      float_neg1_8041fcd8, float_0_8041fc7c);
}

void fbatTalkMode(void) {
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 reviseAngle(f32 angle);
    extern void partyKeyOff(void);
    extern void marioChgTalkMotion(void);
    extern void partyKeyOn(void);
    extern void marioChgStayMotion(void);
    s32 fbatData;
    s32 npc;
    s32 player;
    void* entry;
    s32 wp;

    fbatData = *(s32*)((s32)gp + 0x168);
    npc = *(s32*)(fbatData + 4);
    player = (s32)marioGetPtr();
    switch (*(u8*)(fbatData + 2)) {
        case 0:
            evtStopID(*(s32*)(*(s32*)(fbatData + 4) + 0x11C));
            if ((*(u32*)npc & 0x1000000) == 0) {
                *(f32*)(npc + 0x144) = reviseAngle(angleABf(
                    *(f32*)(npc + 0x8C), *(f32*)(npc + 0x94),
                    *(f32*)(player + 0x8C), *(f32*)(player + 0x94)));
            }
            marioKeyOff();
            partyKeyOff();
            switch (*(s16*)fbatData) {
                case 3:
                    entry = evtEntry(*(void**)(npc + 0x12C), 0, 0);
                    *(u8*)((s32)entry + 0x10) = 6;
                    break;
                default:
                    entry = evtEntry(*(void**)(npc + 0x128), 0, 0);
                    *(u8*)((s32)entry + 0x10) = 6;
                    marioChgTalkMotion();
                    break;
            }
            *(s32*)((s32)entry + 0x170) = npc;
            *(s32*)(fbatData + 0xC) = *(s32*)((s32)entry + 0x15C);
            *(u8*)(fbatData + 2) = *(u8*)(fbatData + 2) + 1;
            break;
        case 1:
            if (evtCheckID(*(s32*)(fbatData + 0xC)) == 0) {
                if ((*(u32*)(npc + 0x1D4) & 0x20000000) == 0) {
                    evtStartID(*(s32*)(*(s32*)(fbatData + 4) + 0x11C));
                    *(f32*)(npc + 0x144) = *(f32*)(npc + 0x148);
                }
                if ((*(u32*)npc & 0x400) != 0) {
                    *(s32*)(npc + 0x108) = 0;
                }
                if ((*(u32*)npc & 0x200) != 0) {
                    animPoseSetAnim(*(s32*)(npc + 0x104), (void*)(npc + 0x2C), 1);
                }
                marioKeyOn();
                partyKeyOn();
                marioChgStayMotion();
                *(s16*)fbatData = 1;
                wp = (s32)&work;
                if (*(s32*)((s32)gp + 0x14) != 0) {
                    wp += 0x14;
                }
                *(s32*)(wp + 0x10) = 0;
            }
            break;
    }
}

void fbatBattleMode(void) {
    extern void marioFBattlePrepare(void);
    extern void seqSetSeq(s32 seq, void* map, void* bero);
    extern void unk_JP_US_EU_01_800591b4(void);
    extern void BattleMajinaiDone(void);
    extern void marioFBattlePost(void);
    extern void marioChgMoveMotion(void);
    extern u16 marioSetMutekiTime(s32 msec);
    extern s32 sysMsec2Frame(s32 msec);
    extern f32 float_0_8041fc7c;
    extern f32 float_1_8041fc94;
    extern s32 dbg_battleskip;
    extern u32 _evt_majinai_coin_up_effect[31];

    void* fbatData;
    void* npc;
    void* pouch;
    void* player;
    void* entry;
    s32 state;
    s32 result;
    s32 fastBattle;
    s32 firstAttack;
    s32 hasItems;
    s32 multiplier;
    s32 i;
    void* wp;
    void* curNpc;
    void* item;
    void* battleInfo;
    s32 count;
    void* setup;

    fbatData = *(void**)((s32)gp + 0x168);
    npc = *(void**)((s32)fbatData + 0x8);
    pouch = pouchGetPtr();
    player = marioGetPtr();

    *(f32*)((s32)npc + 0xF0) = float_0_8041fc7c;
    state = *(u8*)((s32)fbatData + 2);

    switch (state) {
        case 0:
            if (dbg_battleskip == 1) {
                *(s16*)fbatData = 1;
                return;
            }
            if (*(void**)((s32)npc + 0x248) == 0) {
                *(s16*)fbatData = 1;
                return;
            }
            if ((*(u32*)((s32)gp + 0x18) & 0x10) != 0) {
                *(s16*)fbatData = 1;
                return;
            }
            if (*(void**)((s32)npc + 0x12C) == 0) {
                *(s16*)fbatData = 1;
                return;
            }

            *(u32*)((s32)*(void**)((s32)fbatData + 0x8) + 0x140) &= 0xF00;
            evtStopID(*(s32*)((s32)*(void**)((s32)fbatData + 0x8) + 0x11C));

            battleInfo = (void*)((s32)fbatData + 0x20);
            if ((*(u32*)((s32)npc + 0x234) & 0x10000000) == 0) {
                if (pouchEquipCheckBadge(0x142) != 0) {
                    setup = *(void**)((s32)npc + 0x248);
                    if ((s32)*(u8*)(*(s32*)*(s32*)((s32)setup + 4) + 0xE) < *(s16*)((s32)pouch + 0x8A)) {
                        fastBattle = 1;
                        firstAttack = 1;
                        goto case0_flags_done;
                    }
                }
                if (pouchEquipCheckBadge(0x141) != 0) {
                    if ((*(u32*)((s32)fbatData + 0x10) & 0x1FE0000) != 0) {
                        setup = *(void**)((s32)npc + 0x248);
                        if ((s32)*(u8*)(*(s32*)*(s32*)((s32)setup + 4) + 0xE) < *(s16*)((s32)pouch + 0x8A)) {
                            fastBattle = 1;
                            firstAttack = 1;
                            goto case0_flags_done;
                        }
                    }
                }
            }
            if (dbg_battleskip == 2) {
                fastBattle = 1;
                firstAttack = 1;
                goto case0_flags_done;
            }
            fastBattle = 0;
            firstAttack = 0;
case0_flags_done:

            if (fastBattle == 0) {
                if (firstAttack == 0) {
                    *(s32*)((s32)*(void**)((s32)gp + 0x168) + 0x550) = 1;
                }
                marioFBattlePrepare();
                seqSetSeq(4, 0, 0);
                if (*(u32*)((s32)fbatData + 0x10) == 0x10000000) {
                    unk_JP_US_EU_01_800591b4();
                }
                *(u8*)((s32)fbatData + 2) = 1;
                return;
            }
            if (firstAttack != 0) {
                BattleInformationSetResult(battleInfo, 2);
            } else {
                BattleInformationSetResult(battleInfo, 5);
            }
            *(u8*)((s32)fbatData + 2) = 4;
            return;

        case 1:
            if (*(s32*)((s32)gp + 0x14) != 0) {
                *(u8*)((s32)fbatData + 2) = state + 1;
            }
            return;

        case 2:
            if (*(s32*)((s32)gp + 0x14) != 0) {
                return;
            }
            *(u8*)((s32)fbatData + 0x54C) = 0;
            battleInfo = (void*)((s32)fbatData + 0x20);
            result = BattleInformationGetResult(battleInfo);
            if (*(u8*)((s32)pouch + 0x5BA) == 4 && result == 1) {
                *(u8*)((s32)fbatData + 2) = 3;
                *(u8*)((s32)fbatData + 0x54C) = 1;
                entry = evtEntry(_evt_majinai_coin_up_effect, 0, 0);
                *(s32*)((s32)fbatData + 0x48) = *(s32*)((s32)entry + 0x15C);
                BattleMajinaiDone();
                marioKeyOff();

                wp = &work;
                if (*(s32*)((s32)gp + 0x14) != 0) {
                    wp = (void*)((s32)wp + 0x14);
                }
                count = *(s32*)((s32)wp + 4);
                curNpc = *(void**)((s32)wp + 0xC);
                i = 0;
                while (i < count) {
                    if (curNpc != 0 && (*(u32*)curNpc & 2) && (*(u32*)curNpc & 0x10000000)) {
                        *(u32*)((s32)curNpc + 0x1D4) |= 0x10000;
                        evtStopID(*(s32*)((s32)curNpc + 0x11C));
                        animPoseSetLocalTimeRate(*(s32*)((s32)curNpc + 0x104), float_0_8041fc7c);
                    }
                    i++;
                    curNpc = (void*)((s32)curNpc + 0x340);
                }
                return;
            }
            marioFBattlePost();
            *(u8*)((s32)fbatData + 2) = 4;
            return;

        case 3:
            if (evtCheckID(*(s32*)((s32)fbatData + 0x48)) != 0) {
                return;
            }
            marioKeyOn();
            marioFBattlePost();
            *(s32*)((s32)fbatData + 0x48) = 0;
            *(u8*)((s32)fbatData + 2) = 4;

            wp = &work;
            if (*(s32*)((s32)gp + 0x14) != 0) {
                wp = (void*)((s32)wp + 0x14);
            }
            count = *(s32*)((s32)wp + 4);
            curNpc = *(void**)((s32)wp + 0xC);
            i = 0;
            while (i < count) {
                if (curNpc != 0 && (*(u32*)curNpc & 2) && (*(u32*)curNpc & 0x10000000) && (*(u32*)((s32)curNpc + 0x1D4) & 0x10000)) {
                    *(u32*)((s32)curNpc + 0x1D4) &= ~0x10000;
                    evtStartID(*(s32*)((s32)curNpc + 0x11C));
                    animPoseSetLocalTimeRate(*(s32*)((s32)curNpc + 0x104), float_1_8041fc94);
                }
                i++;
                curNpc = (void*)((s32)curNpc + 0x340);
            }
            return;

        case 4:
            marioChgMoveMotion();
            if (*(s32*)((s32)gp + 0x14) != 0) {
                return;
            }
            battleInfo = (void*)((s32)fbatData + 0x20);
            result = BattleInformationGetResult(battleInfo);
            hasItems = 0;
            if (*(s32*)((s32)npc + 0x28C) != 0) {
                hasItems = 1;
            } else if (*(s32*)((s32)npc + 0x290) != 0) {
                hasItems = 1;
            } else if (*(s32*)((s32)npc + 0x294) != 0) {
                hasItems = 1;
            } else if (*(s32*)((s32)npc + 0x298) != 0) {
                hasItems = 1;
            } else if (*(s32*)((s32)npc + 0x29C) != 0) {
                hasItems = 1;
            } else if (*(s32*)((s32)npc + 0x2A0) != 0) {
                hasItems = 1;
            } else if (*(s32*)((s32)npc + 0x2A4) != 0) {
                hasItems = 1;
            } else if (*(s32*)((s32)npc + 0x2A8) != 0) {
                hasItems = 1;
            }

            *(s16*)fbatData = 1;
            if (result == 1 || result == 2) {
                multiplier = 1;
                if (BattleInformationGetResult(battleInfo) == 1) {
                    *(u8*)((s32)npc + 0x315) = *(u32*)((s32)fbatData + 0x34);
                } else if (result == 1) {
                    *(u8*)((s32)npc + 0x315) = *(u8*)((s32)npc + 0x316);
                } else {
                    multiplier = 0;
                    *(u8*)((s32)npc + 0x315) = 0;
                }
                if (multiplier != 0) {
                    if (*(u8*)((s32)fbatData + 0x54C) != 0) {
                        multiplier = 3;
                    }
                    if (pouchEquipCheckBadge(0x139) != 0) {
                        multiplier += pouchEquipCheckBadge(0x139);
                    }
                    if ((*(u32*)((s32)npc + 0x234) & 0x100) != 0) {
                        multiplier += 1;
                    }
                }
                *(u8*)((s32)npc + 0x315) = *(u8*)((s32)npc + 0x315) * multiplier;

                if (hasItems == 0) {
                    npcGroupDead(npc, 0);
                    return;
                }

                if ((*(u32*)npc & 0x400) != 0) {
                    *(s32*)((s32)npc + 0x108) = 0;
                }
                *(s16*)((s32)npc + 0x320) = 0x5A;
                *(u32*)((s32)npc + 0x140) &= ~0x100;
                *(u32*)((s32)npc + 0x140) |= 0x10000000;
                if ((*(u32*)npc & 0x200) != 0) {
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), (void*)((s32)npc + 0x2C), 1);
                }
                i = 0;
                while (i < 8) {
                    s32 itemId;
                    itemId = *(s32*)((s32)npc + 0x2AC + i * 4);
                    if (itemId != 0) {
                        item = (void*)itemEntry(0, itemId, 0xB, -1, 0, *(f32*)((s32)npc + 0x8C), *(f32*)((s32)npc + 0x90), *(f32*)((s32)npc + 0x94));
                        if (item != 0) {
                            itemFlagOn(item, 0x100);
                        }
                        *(s32*)((s32)npc + 0x2AC + i * 4) = 0;
                    }
                    i++;
                }
                return;
            }
            if (result == 4) {
                if ((*(u32*)npc & 0x400) != 0) {
                    *(s32*)((s32)npc + 0x108) = 0;
                }
                *(u32*)((s32)npc + 0x140) |= 0x10000000;
                marioSetMutekiTime(3000);
                *(s16*)((s32)player + 0x2D8) = sysMsec2Frame(3000);
                if ((*(u32*)npc & 0x200) != 0) {
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), (void*)((s32)npc + 0x2C), 1);
                }
            }
            return;
    }
}

void _npcDead(u32* npc, s16 scoreType) {
    u8* fbat;

    npc[0x50] |= 0x10;
    npc[0x75] |= 4;
    *(s16*)((u8*)npc + 0x31C) = scoreType;
    if ((*npc & 4) == 0) {
        return;
    }
    fbat = *(u8**)((u8*)gp + 0x168);
    *(u32*)(fbat + 0x5C) |= 1 << *(u8*)((u8*)npc + 0x314);
}

u8 _npcGroupDead(u32* param_1, s16 param_2) {
    extern void _npcDead(u32*, s16);
    extern void* fbatGetPointer(void);
    u32* child;
    u32* grand;
    u32* great;
    u32* cur;
    s32 i;
    s32 fbat;

    child = (u32*)param_1[0xCA];
    if (child != 0) {
        grand = (u32*)child[0xCA];
        if (grand != 0) {
            great = (u32*)grand[0xCA];
            if (great != 0) {
                child = (u32*)great[0xCA];
                if (child != 0) {
                    _npcGroupDead(child, param_2);
                }
                cur = great;
                for (i = 0; i < 4; i++, cur++) {
                    child = (u32*)cur[0xCC];
                    if (child != 0) {
                        _npcDead(child, param_2);
                    }
                }
                _npcDead(great, param_2);
            }
            cur = grand;
            for (i = 0; i < 4; i++, cur++) {
                child = (u32*)cur[0xCC];
                if (child != 0) {
                    child[0x50] |= 0x10;
                    child[0x75] |= 4;
                    *(s16*)((s32)child + 0x31C) = param_2;
                    if ((*child & 4) != 0) {
                        fbat = (s32)fbatGetPointer();
                        *(u32*)(fbat + 0x5C) |= 1 << *(u8*)((s32)child + 0x314);
                    }
                }
            }
            grand[0x50] |= 0x10;
            grand[0x75] |= 4;
            *(s16*)((s32)grand + 0x31C) = param_2;
            if ((*grand & 4) != 0) {
                fbat = (s32)fbatGetPointer();
                *(u32*)(fbat + 0x5C) |= 1 << *(u8*)((s32)grand + 0x314);
            }
        }
        cur = child;
        for (i = 0; i < 2; i++, cur += 2) {
            grand = (u32*)cur[0xCC];
            if (grand != 0) {
                grand[0x50] |= 0x10;
                grand[0x75] |= 4;
                *(s16*)((s32)grand + 0x31C) = param_2;
                if ((*grand & 4) != 0) {
                    *(u32*)(*(s32*)((s32)gp + 0x168) + 0x5C) |= 1 << *(u8*)((s32)grand + 0x314);
                }
            }
            grand = (u32*)cur[0xCD];
            if (grand != 0) {
                grand[0x50] |= 0x10;
                grand[0x75] |= 4;
                *(s16*)((s32)grand + 0x31C) = param_2;
                if ((*grand & 4) != 0) {
                    *(u32*)(*(s32*)((s32)gp + 0x168) + 0x5C) |= 1 << *(u8*)((s32)grand + 0x314);
                }
            }
        }
        child[0x50] |= 0x10;
        child[0x75] |= 4;
        *(s16*)((s32)child + 0x31C) = param_2;
        if ((*child & 4) != 0) {
            *(u32*)(*(s32*)((s32)gp + 0x168) + 0x5C) |= 1 << *(u8*)((s32)child + 0x314);
        }
    }

    cur = param_1;
    for (i = 0; i < 2; i++, cur += 2) {
        child = (u32*)cur[0xCC];
        if (child != 0) {
            child[0x50] |= 0x10;
            child[0x75] |= 4;
            *(s16*)((s32)child + 0x31C) = param_2;
            if ((*child & 4) != 0) {
                *(u32*)(*(s32*)((s32)gp + 0x168) + 0x5C) |= 1 << *(u8*)((s32)child + 0x314);
            }
        }
        child = (u32*)cur[0xCD];
        if (child != 0) {
            child[0x50] |= 0x10;
            child[0x75] |= 4;
            *(s16*)((s32)child + 0x31C) = param_2;
            if ((*child & 4) != 0) {
                *(u32*)(*(s32*)((s32)gp + 0x168) + 0x5C) |= 1 << *(u8*)((s32)child + 0x314);
            }
        }
    }
    param_1[0x50] |= 0x10;
    param_1[0x75] |= 4;
    *(s16*)((s32)param_1 + 0x31C) = param_2;
    if ((*param_1 & 4) != 0) {
        *(u32*)(*(s32*)((s32)gp + 0x168) + 0x5C) |= 1 << *(u8*)((s32)param_1 + 0x314);
    }
    return 0;
}

void npcGroupDead(void* npc, s32 scoreType) {
    extern void _npcGroupDead(u32* npc, s32 scoreType);
    u8* current = npc;

    if (*(void**)(current + 0x32C) != 0) {
        current = *(u8**)(current + 0x32C);
    }
    while (*(void**)(current + 0x324) != 0) {
        current = *(u8**)(current + 0x324);
    }
    _npcGroupDead((u32*)current, scoreType);
}

void* fbatNpcTalkCheck(void) {
    s32 wp = (s32)&work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        wp += 0x14;
    }
    if (*(void**)(wp + 0x10) != 0 && marioChkTalkable() != 0) {
        return *(void**)(wp + 0x10);
    }
    return 0;
}

void fbatHitCheckAll(void) {
    fbatHitCheck(0x13F1, 0);
}


void* fbatHitCheck(u32 flags, void* hitInfo) {
    typedef struct FbatHitWork {
        u32 hitFlags;
        u32 attackMode;
        f32 bestDistance;
        u32 unknownFlags;
    } FbatHitWork;
    extern f32 float_1000_8041fca8;
    extern f32 float_0_8041fc7c;
    extern f32 float_0p5_8041fc80;
    extern f32 float_12_8041fca0;
    extern f32 float_15_8041fca4;
    extern f32 float_30_8041fcb4;
    extern f32 __float_nan[];
    extern const f64 double_0p5_802c13c8;
    extern const f64 double_3_802c13d0;
    extern const f64 double_0_802c13d8;
    extern s32 dat_8041fc70;
    extern s32 dat_8041fc74;
    s32 npcWork;
    s32 player;
    s32 npc;
    s32 count;
    s32 i;
    s32 bestNpc = 0;
    FbatHitWork hitWork;


    npcWork = (s32)&work.field;
    if (gp->inBattle != 0) {
        npcWork += 0x14;
    }
    player = (s32)marioGetPtr();
    hitWork.hitFlags = 0;
    hitWork.bestDistance = float_1000_8041fca8;
    hitWork.unknownFlags &= ~1;

    if ((flags & 0x2000) != 0) {
        hitWork.unknownFlags |= 1;
    }

    if ((flags & 0x30) != 0) {
        count = *(s32*)(npcWork + 4);
        npc = *(s32*)(npcWork + 0xC);
    }
    if ((flags & 0x30) != 0 &&
        ((flags & 0x10) == 0 || *(s16*)(player + 0x4C) < 1)) {
        s32 checkPlayer = (s32)marioGetPtr();
        s32 checkNpcWork = (s32)&work.field;
        s32 canCheck;

        if (gp->inBattle != 0) {
            checkNpcWork += 0x14;
        }
        if ((*(u32*)checkPlayer & 0x82000000) != 0) { canCheck = 0; }
        else if ((*(u32*)(checkNpcWork + 8) & 1) == 0 && (*(u32*)checkPlayer & 0xA) != 0) { canCheck = 0; }
        else if (*(s32*)((s32)gp + 0x164) != 0) { canCheck = 0; }
        else if (seqGetSeq() != 2) { canCheck = 0; }
        else if (*(u16*)(checkPlayer + 0x2E) == 0x19) { canCheck = 0; }
        else if (vivianGetStatus() != 0) { canCheck = 0; }
        else if (*(u16*)(checkPlayer + 0x2E) == 0x1F || *(u16*)(checkPlayer + 0x2E) == 0x20) { canCheck = 0; }
        else if (*(u16*)(npc + 0x320) != 0) { canCheck = 0; }
        else { canCheck = 1; }
        if (!canCheck) goto skip_jump_check;
        for (i = 0; i < count; i++, npc += 0x340) {
            u32 npcFlags;
            f32 dx;
            f32 dz;
            f64 distance;

            if (npc == 0) {
                continue;
            }
            npcFlags = *(u32*)npc;
            if ((npcFlags & 2) == 0) {
                continue;
            }
            if ((flags & 0x1000) != 0) {
                if ((npcFlags & 8) == 0) {
                    continue;
                }
            } else if ((npcFlags & 8) != 0) {
                continue;
            }
            if ((flags & 0x2000) != 0 &&
                (*(u32*)(npc + 4) & 1) == 0) {
                continue;
            }
            if ((npcFlags & 0x20) != 0 ||
                (*(u32*)(npc + 0x1D4) & 4) != 0 ||
                (npcFlags & 0x08000000) != 0) {
                continue;
            }

            if (*(f32*)(player + 0x90) >=
                    *(f32*)(npc + 0x90) + *(f32*)(npc + 0x150) ||
                *(f32*)(npc + 0x90) >=
                    *(f32*)(player + 0x90) + *(f32*)(player + 0x1BC)) {
                continue;
            }

            dx = *(f32*)(player + 0x8C) - *(f32*)(npc + 0x8C);
            dz = *(f32*)(player + 0x94) - *(f32*)(npc + 0x94);
            {
                f32 distanceSq;
                f64 distanceD;
                f64 inv;
                u32 bits;
                u32 exp;
                s32 kind;

                distanceSq = dx * dx + dz * dz;
                distanceD = (f64)distanceSq;
                if (distanceSq > float_0_8041fc7c) {
                    inv = __frsqrte(distanceD);
                    inv = double_0p5_802c13c8 * inv *
                          -(distanceD * inv * inv - double_3_802c13d0);
                    inv = double_0p5_802c13c8 * inv *
                          -(distanceD * inv * inv - double_3_802c13d0);
                    distance = (f32)(
                        distanceD * double_0p5_802c13c8 * inv *
                        -(distanceD * inv * inv - double_3_802c13d0));
                } else if (distanceD < double_0_802c13d8) {
                    distance = __float_nan[0];
                } else {
                    bits = *(u32*)&distanceSq;
                    exp = bits & 0x7F800000;
                    switch (exp) {
                    case 0x7F800000:
                        if (bits & 0x7FFFFF) kind = 1; else kind = 2;
                        break;
                    case 0:
                        if (bits & 0x7FFFFF) kind = 5; else kind = 3;
                        break;
                    default: kind = 4; break;
                    }
                    distance = kind == 1 ? __float_nan[0] : distanceSq;
                }
            }
            if (distance <= float_0p5_8041fc80 *
                               (*(f32*)(player + 0x1B8) + *(f32*)(npc + 0x14C)) &&
                distance < hitWork.bestDistance) {
                if ((flags & 0x10) != 0 &&
                    (*(u32*)(npc + 0x1D4) & 0x10000) == 0 &&
                    *(s16*)(player + 0x4C) < 1) {
                    if (*(s32*)(npc + 0x2F8) != 0) {
                        s32 isNpcEvent = 0;

                        if (evtGetPtrID(*(s32*)(npc + 0x11C)) != 0) {
                            isNpcEvent =
                                *(u8*)((s32)evtGetPtrID(*(s32*)(npc + 0x11C)) + 0x10) == 5;
                        }
                        if (isNpcEvent == 0) {
                            if (pouchEquipCheckBadge(0x140) == 0) {
                                hitWork.hitFlags = 0x10000000;
                                hitWork.bestDistance = distance;
                                if ((flags & 0x2000) == 0) {
                                    {
                                        s32 announceData;

                                        announceData = *(s32*)((s32)gp + 0x168);
                                        *(s32*)(announceData + 0x550) = 0;
                                        *(s32*)(announceData + 0x554) = 0x10000000;
                                        *(s32*)(announceData + 0x55C) = 0;
                                        switch (*(s32*)(announceData + 0x554)) {
                                            case 0x20000:
                                            case 0x40000:
                                            case 0x80000:
                                            case 0x100000:
                                            case 0x200000:
                                            case 0x400000:
                                            case 0x800000:
                                            case 0x1000000:
                                                *(char**)(announceData + 0x568) =
                                                    "fb_sensei_shita";
                                                *(s32*)(announceData + 0x558) = 2;
                                                *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                                break;
                                            case 0x10000000:
                                                *(char**)(announceData + 0x568) =
                                                    "fb_sensei_sareta";
                                                *(s32*)(announceData + 0x558) = 1;
                                                *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                                break;
                                            default:
                                                *(s32*)(announceData + 0x558) = 0;
                                                break;
                                        }
                                    }
                                }
                                hitWork.attackMode = *(u32*)(npc + 0x2F8);
                                bestNpc = npc;
                                goto jump_attack_done;
                            }

                            hitWork.hitFlags = 0x10000;
                            hitWork.bestDistance = distance;
                            bestNpc = npc;
                            if ((flags & 0x2000) == 0) {
                                {
                                    s32 announceData;

                                    announceData = *(s32*)((s32)gp + 0x168);
                                    *(s32*)(announceData + 0x550) = 0;
                                    *(s32*)(announceData + 0x554) = 0x10000;
                                    *(s32*)(announceData + 0x55C) = 0;
                                    switch (*(s32*)(announceData + 0x554)) {
                                        case 0x20000:
                                        case 0x40000:
                                        case 0x80000:
                                        case 0x100000:
                                        case 0x200000:
                                        case 0x400000:
                                        case 0x800000:
                                        case 0x1000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_shita";
                                            *(s32*)(announceData + 0x558) = 2;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                            break;
                                        case 0x10000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_sareta";
                                            *(s32*)(announceData + 0x558) = 1;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                            break;
                                        default:
                                            *(s32*)(announceData + 0x558) = 0;
                                            break;
                                    }
                                }
                            }
                            goto jump_attack_done;
                        }
                    }

                    if ((npcFlags & 0x20000000) != 0) {
                        continue;
                    }
                    hitWork.hitFlags = 0x10000;
                    hitWork.bestDistance = distance;
                    bestNpc = npc;
                    if ((flags & 0x2000) == 0) {
                        {
                            s32 announceData;

                            announceData = *(s32*)((s32)gp + 0x168);
                            *(s32*)(announceData + 0x550) = 0;
                            *(s32*)(announceData + 0x554) = 0x10000;
                            *(s32*)(announceData + 0x55C) = 0;
                            switch (*(s32*)(announceData + 0x554)) {
                                case 0x20000:
                                case 0x40000:
                                case 0x80000:
                                case 0x100000:
                                case 0x200000:
                                case 0x400000:
                                case 0x800000:
                                case 0x1000000:
                                    *(char**)(announceData + 0x568) =
                                        "fb_sensei_shita";
                                    *(s32*)(announceData + 0x558) = 2;
                                    *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                    break;
                                case 0x10000000:
                                    *(char**)(announceData + 0x568) =
                                        "fb_sensei_sareta";
                                    *(s32*)(announceData + 0x558) = 1;
                                    *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                    break;
                                default:
                                    *(s32*)(announceData + 0x558) = 0;
                                    break;
                            }
                        }
                    }
                }

jump_attack_done:
                if ((flags & 0x20) != 0) {
                    if ((*(u32*)player & 0x20000) == 0 ||
                        (*(u32*)player & 0x10000) != 0 ||
                        (npcFlags & 0x20000000) != 0) {
                        if (marioGetJabaraState() == 6 &&
                            (npcFlags & 0x20000000) == 0) {
                            hitWork.hitFlags = 0x20000;
                            hitWork.bestDistance = distance;
                            bestNpc = npc;
                            if ((flags & 0x2000) == 0) {
                                {
                                    s32 announceData;

                                    announceData = *(s32*)((s32)gp + 0x168);
                                    *(s32*)(announceData + 0x550) = 0;
                                    *(s32*)(announceData + 0x554) = 0x20000;
                                    *(s32*)(announceData + 0x55C) = 0;
                                    switch (*(s32*)(announceData + 0x554)) {
                                        case 0x20000:
                                        case 0x40000:
                                        case 0x80000:
                                        case 0x100000:
                                        case 0x200000:
                                        case 0x400000:
                                        case 0x800000:
                                        case 0x1000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_shita";
                                            *(s32*)(announceData + 0x558) = 2;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                            break;
                                        case 0x10000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_sareta";
                                            *(s32*)(announceData + 0x558) = 1;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                            break;
                                        default:
                                            *(s32*)(announceData + 0x558) = 0;
                                            break;
                                    }
                                }
                            }
                        }
                    } else {
                        hitWork.bestDistance = distance;
                        if (marioGetJabaraState() == 6) {
                            hitWork.hitFlags = 0x20000;
                            bestNpc = npc;
                            if ((flags & 0x2000) == 0) {
                                {
                                    s32 announceData;

                                    announceData = *(s32*)((s32)gp + 0x168);
                                    *(s32*)(announceData + 0x550) = 0;
                                    *(s32*)(announceData + 0x554) = 0x20000;
                                    *(s32*)(announceData + 0x55C) = 0;
                                    switch (*(s32*)(announceData + 0x554)) {
                                        case 0x20000:
                                        case 0x40000:
                                        case 0x80000:
                                        case 0x100000:
                                        case 0x200000:
                                        case 0x400000:
                                        case 0x800000:
                                        case 0x1000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_shita";
                                            *(s32*)(announceData + 0x558) = 2;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                            break;
                                        case 0x10000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_sareta";
                                            *(s32*)(announceData + 0x558) = 1;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                            break;
                                        default:
                                            *(s32*)(announceData + 0x558) = 0;
                                            break;
                                    }
                                }
                            }
                        } else if (*(u16*)(player + 0x2E) == 0x10) {
                            hitWork.hitFlags = 0x40000;
                            bestNpc = npc;
                            if ((flags & 0x2000) == 0) {
                                {
                                    s32 announceData;

                                    announceData = *(s32*)((s32)gp + 0x168);
                                    *(s32*)(announceData + 0x550) = 0;
                                    *(s32*)(announceData + 0x554) = 0x40000;
                                    *(s32*)(announceData + 0x55C) = 0;
                                    switch (*(s32*)(announceData + 0x554)) {
                                        case 0x20000:
                                        case 0x40000:
                                        case 0x80000:
                                        case 0x100000:
                                        case 0x200000:
                                        case 0x400000:
                                        case 0x800000:
                                        case 0x1000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_shita";
                                            *(s32*)(announceData + 0x558) = 2;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                            break;
                                        case 0x10000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_sareta";
                                            *(s32*)(announceData + 0x558) = 1;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                            break;
                                        default:
                                            *(s32*)(announceData + 0x558) = 0;
                                            break;
                                    }
                                }
                            }
                        } else {
                            hitWork.hitFlags = 0x20000;
                            bestNpc = npc;
                            if ((flags & 0x2000) == 0) {
                                {
                                    s32 announceData;

                                    announceData = *(s32*)((s32)gp + 0x168);
                                    *(s32*)(announceData + 0x550) = 0;
                                    *(s32*)(announceData + 0x554) = 0x20000;
                                    *(s32*)(announceData + 0x55C) = 0;
                                    switch (*(s32*)(announceData + 0x554)) {
                                        case 0x20000:
                                        case 0x40000:
                                        case 0x80000:
                                        case 0x100000:
                                        case 0x200000:
                                        case 0x400000:
                                        case 0x800000:
                                        case 0x1000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_shita";
                                            *(s32*)(announceData + 0x558) = 2;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                            break;
                                        case 0x10000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_sareta";
                                            *(s32*)(announceData + 0x558) = 1;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                            break;
                                        default:
                                            *(s32*)(announceData + 0x558) = 0;
                                            break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
skip_jump_check:

    if ((flags & 0x40) != 0 && bestNpc == 0) {
        if ((flags & 0x10) == 0 || *(s16*)(player + 0x4C) < 1) {
            s32 checkPlayer = (s32)marioGetPtr();
            s32 checkNpcWork = (s32)&work.field;
            s32 canCheck;

            if (gp->inBattle != 0) {
                checkNpcWork += 0x14;
            }
            if ((*(u32*)checkPlayer & 0x82000000) != 0) {
                canCheck = 0;
            } else if (((*(u32*)(checkNpcWork + 8) & 1) == 0) &&
                       ((*(u32*)checkPlayer & 0xA) != 0)) {
                canCheck = 0;
            } else if (*(s32*)((s32)gp + 0x164) != 0) {
                canCheck = 0;
            } else if (seqGetSeq() != 2) {
                canCheck = 0;
            } else if (*(u16*)(checkPlayer + 0x2E) == 0x19) {
                canCheck = 0;
            } else if (vivianGetStatus() != 0) {
                canCheck = 0;
            } else if (*(u16*)(checkPlayer + 0x2E) == 0x1F ||
                       *(u16*)(checkPlayer + 0x2E) == 0x20) {
                canCheck = 0;
            } else if (*(u16*)(npc + 0x320) != 0) {
                canCheck = 0;
            } else {
                canCheck = 1;
            }

            if (canCheck != 0) {
                if ((flags & 1) != 0) {
                    if (*(u16*)(player + 0x2E) == 0x12) {
                        extern f64 reviseAngle(f64 angle);
                        extern s32 marioAnimeId(void);
                        extern const f32 float_1p5_8041fcac;
                        extern const f32 float_90_8041fcb0;
                        extern const f32 float_0_8041fc7c;
                        extern const f32 float_10_8041fc98;
                        extern const f32 float_11_8041fc9c;
                        extern const f32 float_12_8041fca0;
                        f32 radius = float_1p5_8041fcac * *(f32*)(player + 0x1B8);
                        f64 angle = reviseAngle(*(f32*)(player + 0x1AC) -
                                                float_90_8041fcb0 +
                                                *(f32*)((s32)camGetPtr(4) + 0x114));
                        f32 sinBase;
                        f32 cosBase;
                        f32 sinFrame;
                        f32 cosFrame;
                        s32 frameIndex;
                        f32 frame;
                        f32 hitX;
                        f32 hitY;
                        f32 hitZ;
                        s32 hammerPlayer;

                        hammerPlayer = (s32)marioGetPtr();
                        animPoseGetAnimPosePtr(*(s32*)(hammerPlayer + 0x22C + marioAnimeId() * 4));
                        frameIndex = *(s32*)(hammerPlayer + 0x48);
                        if (frameIndex > 20) {
                            frameIndex = 10;
                        }
                        frame = (f32)frameIndex;
                        hammerPlayer = (s32)marioGetPtr();
                        {
                            typedef struct HammerStepTable {
                                s32 value[11];
                            } HammerStepTable;
                            extern const HammerStepTable dat_802c122c;
                            HammerStepTable steps = dat_802c122c;

                            if (frame < float_0_8041fc7c) {
                                frame = float_0_8041fc7c;
                            }
                            if (frame >= float_11_8041fc9c) {
                                frame = float_10_8041fc98;
                            }
                            sincosf((f32)angle, &sinBase, &cosBase);
                            sincosf((f32)steps.value[(s32)frame], &sinFrame, &cosFrame);
                            hitX = sinFrame * sinBase * radius + *(f32*)(hammerPlayer + 0x8C);
                            hitY = radius * cosFrame + *(f32*)(hammerPlayer + 0x90);
                            hitZ = sinFrame * cosBase * radius + *(f32*)(hammerPlayer + 0x94);

                            npc = *(s32*)(npcWork + 0xC);
                            for (i = 0; i < count; i++, npc += 0x340) {
                                f32 distance;
                                u32 npcFlags;

                                if (npc == 0) {
                                    continue;
                                }
                                npcFlags = *(u32*)npc;
                                if ((npcFlags & 2) == 0) {
                                    continue;
                                }
                                if ((flags & 0x1000) != 0) {
                                    if ((npcFlags & 8) == 0) {
                                        continue;
                                    }
                                } else if ((npcFlags & 8) != 0) {
                                    continue;
                                }
                                if ((flags & 0x2000) != 0 && (*(u32*)(npc + 4) & 2) == 0) {
                                    continue;
                                }
                                if ((npcFlags & 0x20000000) != 0 ||
                                    (npcFlags & 0x20) != 0 ||
                                    (*(u32*)(npc + 0x1D4) & 4) != 0 ||
                                    (npcFlags & 0x08000000) != 0) {
                                    continue;
                                }
                                if (_npcHitCheckSphere(hitX, hitY, hitZ, float_12_8041fca0,
                                                       npc, &distance) != 0 &&
                                    distance < hitWork.bestDistance) {
                                    s32 announceData;

                                    hitWork.bestDistance = distance;
                                    bestNpc = npc;
                                    hitWork.hitFlags = 0x100000;
                                    if ((flags & 0x2000) != 0) {
                                        break;
                                    }
                                    announceData = *(s32*)((s32)gp + 0x168);
                                    *(s32*)(announceData + 0x550) = 0;
                                    *(s32*)(announceData + 0x554) = 0x100000;
                                    *(s32*)(announceData + 0x55C) = 0;
                                    switch (*(s32*)(announceData + 0x554)) {
                                        case 0x20000:
                                        case 0x40000:
                                        case 0x80000:
                                        case 0x100000:
                                        case 0x200000:
                                        case 0x400000:
                                        case 0x800000:
                                        case 0x1000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_shita";
                                            *(s32*)(announceData + 0x558) = 2;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                            break;
                                        case 0x10000000:
                                            *(char**)(announceData + 0x568) =
                                                "fb_sensei_sareta";
                                            *(s32*)(announceData + 0x558) = 1;
                                            *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                            break;
                                        default:
                                            *(s32*)(announceData + 0x558) = 0;
                                            break;
                                    }
                                    break;
                                }
                            }

                        }
                    }
                } else {
                    extern f64 reviseAngle(f64 angle);
                    extern const f32 float_1p5_8041fcac;
                    extern const f32 float_90_8041fcb0;
                    f32 radius = float_1p5_8041fcac * *(f32*)(player + 0x1B8);
                    f64 angle = reviseAngle(*(f32*)(player + 0x1AC) -
                                            float_90_8041fcb0 +
                                            *(f32*)((s32)camGetPtr(4) + 0x114));

                    npc = *(s32*)(npcWork + 0xC);
                    for (i = 0; i < count; i++, npc += 0x340) {
                        f32 distance;
                        u32 npcFlags;

                        if (npc == 0) {
                            continue;
                        }
                        npcFlags = *(u32*)npc;
                        if ((npcFlags & 2) == 0) {
                            continue;
                        }
                        if ((flags & 0x1000) != 0) {
                            if ((npcFlags & 8) == 0) {
                                continue;
                            }
                        } else if ((npcFlags & 8) != 0) {
                            continue;
                        }
                        if ((flags & 0x2000) != 0 &&
                            (*(u32*)(npc + 4) & 2) == 0) {
                            continue;
                        }
                        if ((npcFlags & 0x20000000) != 0 ||
                            (npcFlags & 0x20) != 0 ||
                            (*(u32*)(npc + 0x1D4) & 4) != 0 ||
                            (npcFlags & 0x08000000) != 0) {
                            continue;
                        }
                        if (_npcHitCheckHammerAllMotion(radius, angle, npc, &distance) != 0 &&
                            distance < hitWork.bestDistance) {
                            hitWork.bestDistance = distance;
                            bestNpc = npc;
                            hitWork.hitFlags = 0x100000;
                            if ((flags & 0x2000) != 0) {
                                continue;
                            }

                            {
                                s32 announceData;

                                announceData = *(s32*)((s32)gp + 0x168);
                                *(s32*)(announceData + 0x550) = 0;
                                *(s32*)(announceData + 0x554) = 0x100000;
                                *(s32*)(announceData + 0x55C) = 0;
                                switch (*(s32*)(announceData + 0x554)) {
                                    case 0x20000:
                                    case 0x40000:
                                    case 0x80000:
                                    case 0x100000:
                                    case 0x200000:
                                    case 0x400000:
                                    case 0x800000:
                                    case 0x1000000:
                                        *(char**)(announceData + 0x568) =
                                            "fb_sensei_shita";
                                        *(s32*)(announceData + 0x558) = 2;
                                        *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                        break;
                                    case 0x10000000:
                                        *(char**)(announceData + 0x568) =
                                            "fb_sensei_sareta";
                                        *(s32*)(announceData + 0x558) = 1;
                                        *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                        break;
                                    default:
                                        *(s32*)(announceData + 0x558) = 0;
                                        break;
                                }
                            }
                            break;
                        }
                    }

                }
            }
        }
    }

    if ((flags & 0x80) != 0 && bestNpc == 0 &&
        ((flags & 0x10) == 0 || *(s16*)(player + 0x4C) < 1) &&
        (*(u32*)player & 0x82000000) == 0 &&
        ((*(u32*)(npcWork + 8) & 1) != 0 || (*(u32*)player & 0xA) == 0) &&
        *(s32*)((s32)gp + 0x164) == 0 &&
        seqGetSeq() == 2 &&
        *(u16*)(player + 0x2E) != 0x19 &&
        vivianGetStatus() == 0 &&
        *(u16*)(player + 0x2E) != 0x1F &&
        *(u16*)(player + 0x2E) != 0x20 &&
        *(u16*)(*(s32*)(npcWork + 0xC) + 0x320) == 0) {
        if ((flags & 1) != 0) {
            if (*(u16*)(player + 0x2E) == 0x13) {

                extern f32 marioGetAngleKaitenHammer(void);
                extern const f32 float_1p5_8041fcac;
                f32 hammerAngle = marioGetAngleKaitenHammer();

                if (hammerAngle >= float_0_8041fc7c) {
                    f32 radius = float_1p5_8041fcac * *(f32*)(player + 0x1B8);
                    f32 sinAngle;
                    f32 cosAngle;
                    f32 hitX;
                    f32 hitY;
                    f32 hitZ;
                    u8* hammerPlayer = marioGetPtr();

                    sincosf(hammerAngle, &sinAngle, &cosAngle);
                    hitX = sinAngle * radius + *(f32*)(hammerPlayer + 0x8C);
                    hitZ = cosAngle * radius + *(f32*)(hammerPlayer + 0x94);
                    npc = *(s32*)(npcWork + 0xC);
                    hitY = *(f32*)(hammerPlayer + 0x90) + float_15_8041fca4;
                    for (i = 0; i < count; i++, npc += 0x340) {
                        f32 distance;
                        u32 npcFlags;

                        if (npc == 0) {
                            continue;
                        }
                        npcFlags = *(u32*)npc;
                        if ((npcFlags & 2) == 0) {
                            continue;
                        }
                        if ((flags & 0x1000) != 0) {
                            if ((npcFlags & 8) == 0) {
                                continue;
                            }
                        } else if ((npcFlags & 8) != 0) {
                            continue;
                        }
                        if ((flags & 0x2000) != 0 &&
                            (*(u32*)(npc + 4) & 2) == 0) {
                            continue;
                        }
                        if ((npcFlags & 0x20000000) != 0 ||
                            (npcFlags & 0x20) != 0 ||
                            (*(u32*)(npc + 0x1D4) & 4) != 0 ||
                            (npcFlags & 0x08000000) != 0) {
                            continue;
                        }
                        if (_npcHitCheckSphere(
                                hitX, hitY, hitZ, float_12_8041fca0, npc, &distance) != 0 &&
                            distance < hitWork.bestDistance) {
                            hitWork.bestDistance = distance;
                            bestNpc = npc;
                            hitWork.hitFlags =
                                pouchGetHammerLv() < 3 ? 0x200000 : 0x400000;
                            if ((flags & 0x2000) != 0) {
                                continue;
                            }

                            {
                                s32 announceData;

                                announceData = *(s32*)((s32)gp + 0x168);
                                *(s32*)(announceData + 0x550) = 0;
                                *(s32*)(announceData + 0x554) = hitWork.hitFlags;
                                *(s32*)(announceData + 0x55C) = 0;
                                switch (*(s32*)(announceData + 0x554)) {
                                    case 0x20000:
                                    case 0x40000:
                                    case 0x80000:
                                    case 0x100000:
                                    case 0x200000:
                                    case 0x400000:
                                    case 0x800000:
                                    case 0x1000000:
                                        *(char**)(announceData + 0x568) =
                                            "fb_sensei_shita";
                                        *(s32*)(announceData + 0x558) = 2;
                                        *(s32*)(announceData + 0x56C) =
                                            dat_8041fc70;
                                        break;
                                    case 0x10000000:
                                        *(char**)(announceData + 0x568) =
                                            "fb_sensei_sareta";
                                        *(s32*)(announceData + 0x558) = 1;
                                        *(s32*)(announceData + 0x56C) =
                                            dat_8041fc74;
                                        break;
                                    default:
                                        *(s32*)(announceData + 0x558) = 0;
                                        break;
                                }
                            }
                            break;
                        }
                    }
                }
                }
        } else {

            extern f64 reviseAngle(f64 angle);
            extern const f32 float_1p5_8041fcac;
            extern const f32 float_90_8041fcb0;
            f32 radius = float_1p5_8041fcac * *(f32*)(player + 0x1B8);
            f64 angle = reviseAngle(*(f32*)(player + 0x1AC) -
                                    float_90_8041fcb0 +
                                    *(f32*)((s32)camGetPtr(4) + 0x114));

            npc = *(s32*)(npcWork + 0xC);
            for (i = 0; i < count; i++, npc += 0x340) {
                f32 distance;
                u32 npcFlags;

                if (npc == 0) {
                    continue;
                }
                npcFlags = *(u32*)npc;
                if ((npcFlags & 2) == 0) {
                    continue;
                }
                if ((flags & 0x1000) != 0) {
                    if ((npcFlags & 8) == 0) {
                        continue;
                    }
                } else if ((npcFlags & 8) != 0) {
                    continue;
                }
                if ((flags & 0x2000) != 0 &&
                    (*(u32*)(npc + 4) & 2) == 0) {
                    continue;
                }
                if ((npcFlags & 0x20000000) != 0 ||
                    (npcFlags & 0x20) != 0 ||
                    (*(u32*)(npc + 0x1D4) & 4) != 0 ||
                    (npcFlags & 0x08000000) != 0) {
                    continue;
                }
                if (_npcHitCheckHammerAllMotion(radius, angle, npc, &distance) != 0 &&
                    distance < hitWork.bestDistance) {
                    hitWork.bestDistance = distance;
                    bestNpc = npc;
                    hitWork.hitFlags = pouchGetHammerLv() < 3 ? 0x200000 : 0x400000;
                    if ((flags & 0x2000) != 0) {
                        continue;
                    }

                    {
                        s32 announceData;

                        announceData = *(s32*)((s32)gp + 0x168);
                        *(s32*)(announceData + 0x550) = 0;
                        *(s32*)(announceData + 0x554) = hitWork.hitFlags;
                        *(s32*)(announceData + 0x55C) = 0;
                        switch (*(s32*)(announceData + 0x554)) {
                            case 0x20000:
                            case 0x40000:
                            case 0x80000:
                            case 0x100000:
                            case 0x200000:
                            case 0x400000:
                            case 0x800000:
                            case 0x1000000:
                                *(char**)(announceData + 0x568) =
                                    "fb_sensei_shita";
                                *(s32*)(announceData + 0x558) = 2;
                                *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                break;
                            case 0x10000000:
                                *(char**)(announceData + 0x568) =
                                    "fb_sensei_sareta";
                                *(s32*)(announceData + 0x558) = 1;
                                *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                break;
                            default:
                                *(s32*)(announceData + 0x558) = 0;
                                break;
                        }
                    }
                    break;
                }
            }
            }
    }

    if ((flags & 0x100) != 0) {
        s32 party = (s32)partyGetPtr(marioGetPartyId());

        if (bestNpc == 0 &&
            ((flags & 0x10) == 0 || *(s16*)(player + 0x4C) < 1)) {
            s32 checkPlayer = (s32)marioGetPtr();
            s32 checkNpcWork = (s32)&work.field;
            s32 canCheck;

            if (gp->inBattle != 0) {
                checkNpcWork += 0x14;
            }
            if ((*(u32*)checkPlayer & 0x82000000) == 0) {
                if (((*(u32*)(checkNpcWork + 8) & 1) == 0) &&
                    ((*(u32*)checkPlayer & 0xA) != 0)) {
                    canCheck = 0;
                } else if (*(s32*)((s32)gp + 0x164) == 0) {
                    if (seqGetSeq() == 2) {
                        if (*(u16*)(checkPlayer + 0x2E) == 0x19) {
                            canCheck = 0;
                        } else if (vivianGetStatus() == 0) {
                            if (*(u16*)(checkPlayer + 0x2E) == 0x1F ||
                                *(u16*)(checkPlayer + 0x2E) == 0x20) {
                                canCheck = 0;
                            } else if (*(u16*)(*(s32*)(checkNpcWork + 0xC) + 0x320) == 0) {
                                canCheck = 1;
                            } else {
                                canCheck = 0;
                            }
                        } else {
                            canCheck = 0;
                        }
                    } else {
                        canCheck = 0;
                    }
                } else {
                    canCheck = 0;
                }
            } else {
                canCheck = 0;
            }

            if (canCheck != 0) {
                if (party != 0 && (flags & 1) != 0 &&
                            *(u8*)(party + 0x33) == 2 && (*(u32*)party & 0x100) != 0) {
                            s32 koopsStatus = nokonokoGetStatus((void*)party);

                            if (koopsStatus >= 2 && koopsStatus <= 5) {
                                f32 partyX = *(f32*)(party + 0x58);
                                f32 partyY = *(f32*)(party + 0x5C);
                                f32 partyZ = *(f32*)(party + 0x60);
                                f32 partyRadius = float_0p5_8041fc80 * *(f32*)(party + 0xF4);
                                f32 partyHalfHeight = float_0p5_8041fc80 * *(f32*)(party + 0xF0);

                            npc = *(s32*)(npcWork + 0xC);
                            for (i = 0; i < count; i++, npc += 0x340) {
                                f32 distance;
                                u32 npcFlags;

                                if (npc == 0) {
                                    continue;
                                }
                                npcFlags = *(u32*)npc;
                                if ((npcFlags & 2) == 0) {
                                    continue;
                                }
                                if ((flags & 0x1000) != 0) {
                                    if ((npcFlags & 8) == 0) {
                                        continue;
                                    }
                                } else if ((npcFlags & 8) != 0) {
                                    continue;
                                }
                                if ((flags & 0x2000) != 0 &&
                                    (*(u32*)(npc + 4) & 4) == 0) {
                                    continue;
                                }
                                if ((npcFlags & 0x20000000) != 0 ||
                                    (npcFlags & 0x20) != 0 ||
                                    (*(u32*)(npc + 0x1D4) & 4) != 0 ||
                                    (npcFlags & 0x08000000) != 0) {
                                    continue;
                                }
                                if (_npcHitCheckSphere(partyX, partyY + partyHalfHeight,
                                                       partyZ, partyRadius, npc, &distance) != 0 &&
                                    distance < hitWork.bestDistance) {
                                    hitWork.bestDistance = distance;
                                    bestNpc = npc;
                                    hitWork.hitFlags = 0x800000;
                                    if ((flags & 0x2000) != 0) {
                                        continue;
                                    }

                                    {
                                        s32 announceData;

                                        announceData = *(s32*)((s32)gp + 0x168);
                                        *(s32*)(announceData + 0x550) = 0;
                                        *(s32*)(announceData + 0x554) = 0x800000;
                                        *(s32*)(announceData + 0x55C) = 0;
                                        switch (*(s32*)(announceData + 0x554)) {
                                            case 0x20000:
                                            case 0x40000:
                                            case 0x80000:
                                            case 0x100000:
                                            case 0x200000:
                                            case 0x400000:
                                            case 0x800000:
                                            case 0x1000000:
                                                *(char**)(announceData + 0x568) =
                                                    "fb_sensei_shita";
                                                *(s32*)(announceData + 0x558) = 2;
                                                *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                                break;
                                            case 0x10000000:
                                                *(char**)(announceData + 0x568) =
                                                    "fb_sensei_sareta";
                                                *(s32*)(announceData + 0x558) = 1;
                                                *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                                break;
                                            default:
                                                *(s32*)(announceData + 0x558) = 0;
                                                break;
                                        }
                                    }
                                    break;
                                }
                            }
                            }
                        }
            }
        }
    }

    if ((flags & 0x200) != 0 && bestNpc == 0 &&
        ((flags & 0x10) == 0 || *(s16*)(player + 0x4C) < 1) &&
        (*(u32*)player & 0x82000000) == 0 &&
        ((*(u32*)(npcWork + 8) & 1) != 0 || (*(u32*)player & 0xA) == 0) &&
        *(s32*)((s32)gp + 0x164) == 0 &&
        seqGetSeq() == 2 &&
        *(u16*)(player + 0x2E) != 0x19 &&
        vivianGetStatus() == 0 &&
        *(u16*)(player + 0x2E) != 0x1F &&
        *(u16*)(player + 0x2E) != 0x20 &&
        *(u16*)(*(s32*)(npcWork + 0xC) + 0x320) == 0) {
        s32 party = (s32)partyGetPtr(marioGetPartyId());

        if (party != 0 && (flags & 1) != 0 &&
            *(u8*)(party + 0x33) == 3 &&
            *(u8*)(*(s32*)((s32)gp + 0x168) + 0x54D) != 0) {
            f32 partyX = *(f32*)(party + 0x58);
            f32 partyY = *(f32*)(party + 0x5C);
            f32 partyZ = *(f32*)(party + 0x60);
            f32 partyHalfHeight = float_0p5_8041fc80 * *(f32*)(party + 0xF0);

            npc = *(s32*)(npcWork + 0xC);
            for (i = 0; i < count; i++, npc += 0x340) {
                f32 distance;
                u32 npcFlags;

                if (npc == 0) {
                    continue;
                }
                npcFlags = *(u32*)npc;
                if ((npcFlags & 2) == 0) {
                    continue;
                }
                if ((flags & 0x1000) != 0) {
                    if ((npcFlags & 8) == 0) {
                        continue;
                    }
                } else if ((npcFlags & 8) != 0) {
                    continue;
                }
                if ((flags & 0x2000) != 0 &&
                    (*(u32*)(npc + 4) & 8) == 0) {
                    continue;
                }
                if ((npcFlags & 0x20000000) != 0 ||
                    (npcFlags & 0x20) != 0 ||
                    (*(u32*)(npc + 0x1D4) & 4) != 0 ||
                    (npcFlags & 0x08000000) != 0) {
                    continue;
                }
                if (_npcHitCheckSphere(partyX, partyY + partyHalfHeight,
                                       partyZ, float_30_8041fcb4, npc, &distance) != 0 &&
                    distance < hitWork.bestDistance) {
                    hitWork.bestDistance = distance;
                    bestNpc = npc;
                    hitWork.hitFlags = 0x1000000;
                    if ((flags & 0x2000) != 0) {
                        continue;
                    }

                    {
                        s32 announceData;

                        announceData = *(s32*)((s32)gp + 0x168);
                        *(s32*)(announceData + 0x550) = 0;
                        *(s32*)(announceData + 0x554) = 0x1000000;
                        *(s32*)(announceData + 0x55C) = 0;
                        switch (*(s32*)(announceData + 0x554)) {
                            case 0x20000:
                            case 0x40000:
                            case 0x80000:
                            case 0x100000:
                            case 0x200000:
                            case 0x400000:
                            case 0x800000:
                            case 0x1000000:
                                *(char**)(announceData + 0x568) =
                                    "fb_sensei_shita";
                                *(s32*)(announceData + 0x558) = 2;
                                *(s32*)(announceData + 0x56C) = dat_8041fc70;
                                break;
                            case 0x10000000:
                                *(char**)(announceData + 0x568) =
                                    "fb_sensei_sareta";
                                *(s32*)(announceData + 0x558) = 1;
                                *(s32*)(announceData + 0x56C) = dat_8041fc74;
                                break;
                            default:
                                *(s32*)(announceData + 0x558) = 0;
                                break;
                        }
                    }
                    break;
                }
            }
        }
    }

    if (hitInfo != 0) {
        *(FbatHitWork*)hitInfo = hitWork;
    }
    return (void*)bestNpc;
}

s32 _npcHitCheckSphere(f64 x, f64 y, f64 z, f64 radius, s32 npc, f32* outDist) {
    extern f32 __float_nan[];
    extern f32 float_0_8041fc7c;
    extern f32 float_0p5_8041fc80;
    extern const f64 double_0p5_802c13c8;
    extern const f64 double_3_802c13d0;
    extern const f64 double_0_802c13d8;
    f32 baseY;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 topY;
    f32 distSq;
    f32 distance;

    baseY = *(f32*)(npc + 0x90);
    dx = (f32)x - *(f32*)(npc + 0x8C);
    dz = (f32)z - *(f32*)(npc + 0x94);
    dy = (f32)y - baseY;

    if (y != (f64)baseY) {
        if (y >= (f64)baseY) {
            topY = baseY + *(f32*)(npc + 0x150);
            if (y <= (f64)topY) {
                dy = float_0_8041fc7c;
            } else {
                dy = (f32)y - topY;
            }
        }
    }

    distSq = dz * dz + dx * dx + dy * dy;
    if (distSq > float_0_8041fc7c) {
        f64 value;
        f64 inv;
        value = (f64)distSq;
        inv = __frsqrte(value);
        inv = double_0p5_802c13c8 * inv *
              (double_3_802c13d0 - value * inv * inv);
        inv = double_0p5_802c13c8 * inv *
              (double_3_802c13d0 - value * inv * inv);
        inv = double_0p5_802c13c8 * inv *
              (double_3_802c13d0 - value * inv * inv);
        distance = (f32)(value * inv);
    } else if ((f64)distSq < double_0_802c13d8) {
        distance = __float_nan[0];
    } else {
        u32 bits = *(u32*)&distSq;
        u32 exponent = bits & 0x7F800000;
        s32 kind;

        switch (exponent) {
            case 0x7F800000:
                if (bits & 0x7FFFFF) { kind = 1; } else { kind = 2; }
                break;
            case 0:
                if (bits & 0x7FFFFF) { kind = 5; } else { kind = 3; }
                break;
            default:
                kind = 4;
                break;
        }
        distance = distSq;
        if (kind == 1) {
            distance = __float_nan[0];
        }
    }

    *outDist = distance;
    if (*outDist < float_0p5_8041fc80 * *(f32*)(npc + 0x14C) + (f32)radius) {
        return npc;
    }
    return 0;
}

/*
 * Target-backed npcdrv .rodata leading prefix.
 *
 * A12/A13 proved this exact 0x34-byte placement puts the naturally emitted,
 * A11-corrected hammer table at target +0x34. These objects remain non-const
 * so executable consumers keep their target-like load behavior.
 */
__declspec(section ".rodata") f32 vec3_802c11f8[3] __attribute__ ((aligned(4))) = {
    0.0f, 0.0f, 0.0f
};
__declspec(section ".rodata") f32 vec3_802c1204[3] __attribute__ ((aligned(4))) = {
    1.0f, 1.0f, 1.0f
};
__declspec(section ".rodata") f32 dat_802c1210[3] __attribute__ ((aligned(4))) = {
    0.0f, 0.0f, 30.0f
};
__declspec(section ".rodata") f32 dat_802c121c[4] __attribute__ ((aligned(4))) = {
    100.0f, 0.0f, 0.0f, 0.0f
};

s32 _npcHitCheckHammerAllMotion(f32 radius, f64 angle, s32 npc, f32* outDist) {
    extern f32 __float_nan[];
    extern void sincosf(f64 angle, f32* outSin, f32* outCos);
    extern const f64 double_0_802c13d8;
    extern const f64 double_0p5_802c13c8;
    extern const f64 double_3_802c13d0;
    extern const f32 float_0_8041fc7c;
    extern const f32 float_0p5_8041fc80;
    extern const f32 float_12_8041fca0;
    f32 cosB;
    f32 sinB;
    f32 cosA;
    f32 sinA;
    f32 distance;
    s32 result = npc;
    extern const f32 float_1_8041fc94;
    extern const f32 float_10_8041fc98;
    extern const f32 float_11_8041fc9c;
    f32 i;

    for (i = float_0_8041fc7c; i <= float_11_8041fc9c; i += float_1_8041fc94) {
        distance = float_0_8041fc7c;
        {
        u8* player = marioGetPtr();
        s32 steps[11] = {0, 0, 0, 0, 45, 60, 90, 100, 101, 101, 101};
        f32 sampleY;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 baseY;
        f32 topY;
        f32 index = i;

        if (index < float_0_8041fc7c) {
            index = float_0_8041fc7c;
        }
        if (index >= float_11_8041fc9c) {
            index = float_10_8041fc98;
        }

        sincosf(angle, &sinA, &cosA);
        sincosf((f64)steps[(s32)index], &sinB, &cosB);
        sampleY = (f32)(*(f32*)(player + 0x90) + radius * cosB);
        dx = (f32)(sinB * (sinA * radius) + *(f32*)(player + 0x8C)) - *(f32*)(npc + 0x8C);
        dz = (f32)(sinB * (cosA * radius) + *(f32*)(player + 0x94)) - *(f32*)(npc + 0x94);
        baseY = *(f32*)(npc + 0x90);
        dy = sampleY - baseY;
        if (sampleY != baseY) {
            if (sampleY >= baseY && sampleY <= baseY + *(f32*)(npc + 0x150)) {
                dy = float_0_8041fc7c;
            } else {
                topY = baseY + *(f32*)(npc + 0x150);
                if (sampleY > topY) {
                    dy = sampleY - topY;
                }
            }
        }
        {
            f32 distSq = dz * dz + dx * dx + dy * dy;

            if (distSq > float_0_8041fc7c) {
                f64 value;
                f64 inv;
                value = (f64)distSq;
                inv = __frsqrte(value);
                inv = double_0p5_802c13c8 * inv *
                      (double_3_802c13d0 - value * inv * inv);
                inv = double_0p5_802c13c8 * inv *
                      (double_3_802c13d0 - value * inv * inv);
                inv = double_0p5_802c13c8 * inv *
                      (double_3_802c13d0 - value * inv * inv);
                distance = (f32)(value * inv);
            } else if ((f64)distSq < double_0_802c13d8) {
                distance = __float_nan[0];
            } else {
                u32 bits = *(u32*)&distSq;
                u32 exponent = bits & 0x7F800000;
                s32 kind;

                switch (exponent) {
                    case 0x7F800000:
                        if (bits & 0x7FFFFF) { kind = 1; } else { kind = 2; }
                        break;
                    case 0:
                        if (bits & 0x7FFFFF) { kind = 5; } else { kind = 3; }
                        break;
                    default:
                        kind = 4;
                        break;
                }
                distance = distSq;
                if (kind == 1) {
                    distance = __float_nan[0];
                }
            }
        }
        result = npc;
        if (distance >= float_0p5_8041fc80 * *(f32*)(npc + 0x14C) + float_12_8041fca0) {
            result = 0;
        }
        if (result != 0) {
            break;
        }
        }
    }
    *outDist = distance;
    return result;
}

s32 npcCheckInterrupt(void* pNpc) {
    extern const f32 float_0_8041fc7c;
    void* entry;
    u32 interruptFlags;

    evtGetPtrID(*(s32*)((s32)pNpc + 0x11C));
    interruptFlags = *(u32*)((s32)pNpc + 0x140);
    if ((interruptFlags & ~0xF00) != 0) {
        entry = 0;
        if ((interruptFlags & 0x10) != 0 && *(void**)((s32)pNpc + 0x12C) != 0) {
            entry = evtEntry(*(void**)((s32)pNpc + 0x12C), 0, 0x20);
            *(u8*)((s32)entry + 0x10) = 8;
            *(void**)((s32)entry + 0x170) = pNpc;
        } else if ((interruptFlags & 0x10000000) != 0 && *(void**)((s32)pNpc + 0x124) != 0) {
            entry = evtEntry(*(void**)((s32)pNpc + 0x124), 0, 0x20);
            *(u8*)((s32)entry + 0x10) = 1;
            *(void**)((s32)entry + 0x170) = pNpc;
        } else if ((interruptFlags & 8) != 0 && *(void**)((s32)pNpc + 0x13C) != 0) {
            entry = evtEntry(*(void**)((s32)pNpc + 0x13C), 0, 0x20);
            *(u8*)((s32)entry + 0x10) = 5;
            *(void**)((s32)entry + 0x170) = pNpc;
        } else if ((interruptFlags & 1) != 0 && *(void**)((s32)pNpc + 0x130) != 0 &&
                   (*(u32*)((s32)pNpc + 0x1D4) & 0x1000) == 0 &&
                   (*(u32*)pNpc & 0x200000) == 0 && (interruptFlags & 0x100) == 0) {
            entry = evtEntry(*(void**)((s32)pNpc + 0x130), 0, 0x20);
            *(u8*)((s32)entry + 0x10) = 2;
            *(void**)((s32)entry + 0x170) = pNpc;
        } else if ((interruptFlags & 2) != 0 && *(void**)((s32)pNpc + 0x134) != 0 &&
                   (*(u32*)((s32)pNpc + 0x1D4) & 0x1000) == 0 &&
                   (*(u32*)pNpc & 0x200000) == 0) {
            entry = evtEntry(*(void**)((s32)pNpc + 0x134), 0, 0x20);
            *(u8*)((s32)entry + 0x10) = 3;
            *(void**)((s32)entry + 0x170) = pNpc;
        } else if ((interruptFlags & 4) != 0 && *(void**)((s32)pNpc + 0x138) != 0 &&
                   (*(u32*)((s32)pNpc + 0x1D4) & 0x1000) == 0 &&
                   (*(u32*)pNpc & 0x200000) == 0) {
            entry = evtEntry(*(void**)((s32)pNpc + 0x138), 0, 0x20);
            *(u8*)((s32)entry + 0x10) = 4;
            *(void**)((s32)entry + 0x170) = pNpc;
        }

        if (entry != 0) {
            evtDeleteID(*(s32*)((s32)pNpc + 0x11C));
            *(s32*)((s32)pNpc + 0x11C) = *(s32*)((s32)entry + 0x15C);
            *(u32*)((s32)pNpc + 0x140) &= 0xF00;
            *(u32*)((s32)pNpc + 0x1D4) &= ~0x1000;
            *(s32*)((s32)pNpc + 0x18C) = 0;
            *(s32*)((s32)pNpc + 0x188) = 0;
            *(f32*)((s32)pNpc + 0x1B0) = float_0_8041fc7c;
            return 1;
        }
    }

    *(u32*)((s32)pNpc + 0x140) &= 0xF00;
    *(u32*)((s32)pNpc + 0x1D4) &= ~0x1000;
    return 0;
}

void npcSetupBattleInfo(void* npc, void* battleInfo) {
    extern s32 swGet(s32 flag);
    s32 battle;
    u32 setup;
    s32 loadout;
    s32 iter;
    s32 total;
    s32 group;
    s32 enemy;
    s32 itemTable;
    s32 i;
    s32 offset;

    battle = (s32)npc + 0x230;
    setup = (u32)battleInfo;
    memset((void*)battle, 0, 0xC8);
    memset((void*)(battle + 0x7C), 0, 0x20);

    if (setup == 0) {
        return;
    }
    if (*(void**)setup == 0) {
        return;
    }

    *(s32*)(battle + 0xAC) = *(s32*)(setup + 0x1C);
    if (*(s32*)(battle + 0xAC) == 0 || *(s32*)(battle + 0xAC) < 0 || *(s32*)(battle + 0xAC) >= 3) {
        i = 0;
        iter = setup;
        do {
            total = irand((s32)*(s8*)(iter + 0x21) - (s32)*(s8*)(iter + 0x20) + 1);
            offset = i + 0xB0;
            i++;
            *(u8*)(battle + offset) = *(u8*)(iter + 0x20) + total;
            iter += 2;
        } while (i < 0x10);
    }

    *(s32*)(battle + 4) = *(s32*)(setup + 0x18);
    *(s32*)(battle + 8) = *(s32*)(setup + 8);
    *(s32*)(battle + 0xC) = 0;
    *(s32*)(battle + 0x10) = 0;
    *(s32*)(battle + 0x14) = 0;
    *(s32*)(battle + 0xC0) = *(s32*)(setup + 0x40);

    i = *(s32*)(setup + 0xC);
    if (i >= -130000000 && i <= -120000000) {
        if (swGet(i + 130000000) != 0) {
            loadout = *(s32*)(setup + 0x10);
        } else {
            loadout = *(s32*)(setup + 0x14);
        }
    } else {
        loadout = *(s32*)(setup + 0x14);
    }

    *(s32*)(battle + 0x18) = 0;
    if ((void*)loadout != 0) {
        total = 0;
        iter = loadout;
        while (*(s32*)iter > 0) {
            total += *(s32*)iter;
            iter += 0xC;
        }
        if (total > 0) {
            total = irand(total);
            for (;;) {
                total -= *(s32*)loadout;
                if (total < 0) {
                    break;
                }
                loadout += 0xC;
            }
            *(s32*)(battle + 0x18) = *(s32*)(loadout + 4);
            *(s32*)battle = *(s32*)(loadout + 8);
        }
    }

    group = *(s32*)(battle + 0x18);
    if ((void*)group != 0) {
        i = *(s32*)group;
        enemy = *(s32*)(group + 4);
        memset((void*)(battle + 0x1C), 0, 0x20);
        offset = 0;
        iter = battle;
        while (offset < i) {
            itemTable = *(s32*)(enemy + 0x2C);
            if ((void*)itemTable != 0) {
                total = 0;
                loadout = itemTable;
                while (*(s16*)(loadout + 4) > 0 || *(s32*)loadout != 0) {
                    total += *(s16*)(loadout + 4);
                    loadout += 8;
                }
                if (total > 0) {
                    total = irand(total);
                    for (;;) {
                        total -= *(s16*)(itemTable + 4);
                        if (total >= 0) {
                            itemTable += 8;
                        } else {
                            *(s32*)(iter + 0x1C) = *(s32*)itemTable;
                            *(s32*)(iter + 0x3C) = *(s32*)(enemy + 0x2C);
                            break;
                        }
                    }
                }
            }
            enemy += 0x30;
            iter += 4;
            offset++;
        }
    }

    *(u8*)((s32)npc + 0x316) = 0;
    group = *(s32*)(battle + 0x18);
    if ((void*)group != 0) {
        *(u8*)((s32)npc + 0x316) = *(u8*)(*(s32*)(*(s32*)(group + 4)) + 0x12);
    }
    *(u32*)npc |= 0x10000000;
}

void npcSetBattleInfo(void* npc, s32 info) {
    extern void* areaDataPtr(char* area);
    extern void npcSetupBattleInfo(void* npc, void* setup);
    u8* base = npc;
    u8* setup;
    char area[4];
    s32 index;

    if (info == -1) {
        setup = 0;
    } else {
        area[0] = *((char*)gp + 0x12C);
        area[1] = *((char*)gp + 0x12D);
        area[2] = *((char*)gp + 0x12E);
        area[3] = 0;
        setup = *(u8**)((u8*)areaDataPtr(area) + 8);
        index = info;
        while (index > 0) {
            setup += 0x44;
            index--;
        }
    }
    npcSetupBattleInfo(base, setup);

    index = 0;
    do {
        u8* child = *(u8**)(base + 0x330 + index * 4);
        if (child != 0) {
            u32* src = (u32*)(base + 0x22C);
            u32* dst = (u32*)(child + 0x22C);
            s32 count = 0x19;
            do {
                u32 first = src[1];
                u32 second = src[2];
                src += 2;
                dst[1] = first;
                dst += 2;
                dst[0] = second;
                count--;
            } while (count != 0);
        }
        index++;
    } while (index < 4);
}

void npcStopForEvent(void) {
    extern f32 float_0_8041fc7c;
    s32 wp = (s32)&work;
    s32 i;
    s32 count;
    void* npc;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if (npc != 0 && (*(u32*)npc & 2) && (*(u32*)npc & 0x10000000)) {
            *(u32*)((s32)npc + 0x1D4) |= 0x10000;
            evtStopID(*(s32*)((s32)npc + 0x11C));
            animPoseSetLocalTimeRate((s32)*(void**)((s32)npc + 0x104), float_0_8041fc7c);
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
}

void npcStopForOneEvent(void* npc) {
    extern f32 float_0_8041fc7c;
    u32 flags;

    if (npc == 0) {
        return;
    }
    flags = *(u32*)npc;
    if (!(flags & 2)) {
        return;
    }
    if (!(flags & 0x10000000)) {
        return;
    }
    *(u32*)((s32)npc + 0x1D4) |= 0x10000;
    evtStopID(*(s32*)((s32)npc + 0x11C));
    animPoseSetLocalTimeRate((s32)*(void**)((s32)npc + 0x104), float_0_8041fc7c);
}

void npcStartForEvent(void) {
    extern f32 float_1_8041fc94;
    s32 wp = (s32)&work;
    s32 i;
    s32 count;
    void* npc;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if (npc != 0 && (*(u32*)npc & 2) && (*(u32*)npc & 0x10000000) &&
            (*(u32*)((s32)npc + 0x1D4) & 0x10000)) {
            *(u32*)((s32)npc + 0x1D4) &= ~0x10000;
            evtStartID(*(s32*)((s32)npc + 0x11C));
            animPoseSetLocalTimeRate((s32)*(void**)((s32)npc + 0x104), float_1_8041fc94);
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
}

void npcStartForOneEvent(void* npc) {
    extern f32 float_1_8041fc94;
    u32 flags;

    if (npc == 0) {
        return;
    }
    flags = *(u32*)npc;
    if (!(flags & 2)) {
        return;
    }
    if (!(flags & 0x10000000)) {
        return;
    }
    flags = *(u32*)((s32)npc + 0x1D4);
    if (!(flags & 0x10000)) {
        return;
    }
    *(u32*)((s32)npc + 0x1D4) = flags & ~0x10000;
    evtStartID(*(s32*)((s32)npc + 0x11C));
    animPoseSetLocalTimeRate((s32)*(void**)((s32)npc + 0x104), float_1_8041fc94);
}

void npcSetColor(char* name, void* color) {
    s32 wp = (s32)&work;
    void* npc;
    s32 count;
    s32 i;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if ((*(u32*)npc & 1) && strcmp((char*)((s32)npc + 8), name) == 0) {
            break;
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
    *(s32*)((s32)npc + 0x114) = *(s32*)color;
}

void npcBlurOn(char* name) {
    typedef struct BlurPosition {
        s32 x;
        s32 y;
        s32 z;
    } BlurPosition;
    s32 wp = (s32)&work;
    void* npc;
    s32 count;
    s32 i;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if ((*(u32*)npc & 1) && strcmp((char*)((s32)npc + 8), name) == 0) {
            break;
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
    *(u32*)npc |= 0x100;
    *(BlurPosition*)((s32)npc + 0xA4) = *(BlurPosition*)((s32)npc + 0x8C);
    *(BlurPosition*)((s32)npc + 0xB0) = *(BlurPosition*)((s32)npc + 0x8C);
    *(BlurPosition*)((s32)npc + 0xBC) = *(BlurPosition*)((s32)npc + 0x8C);
    *(BlurPosition*)((s32)npc + 0xC8) = *(BlurPosition*)((s32)npc + 0x8C);
    *(BlurPosition*)((s32)npc + 0xD4) = *(BlurPosition*)((s32)npc + 0x8C);
}

void npcBlurOff(char* name) {
    s32 wp = (s32)&work;
    void* npc;
    s32 count;
    s32 i;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if ((*(u32*)npc & 1) && strcmp((char*)((s32)npc + 8), name) == 0) {
            break;
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
    *(u32*)npc &= ~0x100;
}

void* npcGetBtlSetupWork(void* npc, s32 index) {
    s32 offset = index << 2;
    npc = (void*)((s32)npc + offset);
    return *(void**)((s32)npc + 0x2CC);
}

void npcSetBtlSetupWork(void* npc, s32 index, void* value) {
    s32 offset = index << 2;
    npc = (void*)((s32)npc + offset);
    *(void**)((s32)npc + 0x2CC) = value;
}

void npcSetSlave(void* master, void* slave, s32 index) {
    s32 offset;
    *(void**)((s32)slave + 0x32C) = master;
    offset = index << 2;
    master = (void*)((s32)master + offset);
    *(void**)((s32)master + 0x330) = slave;
}

void npcSetLink(void* a, void* b) {
    *(void**)((s32)a + 0x328) = b;
    *(void**)((s32)b + 0x324) = a;
}

void* dbGetDefData(void* entry, const char* name) {
    void* ret = 0;

    for (;;) {
        char* current = *(char**)entry;
        if (current == 0) {
            break;
        }
        if (strcmp(current, name) == 0) {
            ret = *(void**)((s32)entry + 4);
            break;
        }
        entry = (void*)((s32)entry + 8);
    }
    return ret;
}

void npcExecAllInitEvt(void) {
    s32 wp = (s32)&work;
    s32 i;
    s32 count;
    void* npc;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if (npc != 0 && (*(u32*)npc & 1)) {
            if (*(void**)((s32)npc + 0x120) != 0) {
                if (*(s32*)((s32)npc + 0x118) != 0) {
                    evtDeleteID(*(s32*)((s32)npc + 0x118));
                }
                {
                    s32 evt = (s32)evtEntry(*(void**)((s32)npc + 0x120), 0, 0);
                    *(u8*)(evt + 0x10) = 0;
                    *(void**)(evt + 0x170) = npc;
                    *(s32*)((s32)npc + 0x118) = *(s32*)(evt + 0x15C);
                }
            } else {
                *(s32*)((s32)npc + 0x118) = 0;
            }
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
}

s32 npcWaitAllInitEvtEnd(void) {
    s32 wp = (s32)&work;
    s32 ret;
    s32 i;
    s32 count;
    void* npc;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    ret = 1;
    npc = *(void**)(wp + 0xC);
    i = 0;
    while (i < count) {
        if (npc != 0 && (*(u32*)npc & 1) && !(*(u32*)npc & 2)) {
            u32 flags = *(u32*)npc;
            s32 id = *(s32*)((s32)npc + 0x118);
            if (id != 0) {
                ret = 0;
                if (evtCheckID(id) == 0) {
                    *(s32*)((s32)npc + 0x118) = 0;
                    *(u32*)npc |= 2;
                }
            } else {
                *(u32*)npc = flags | 2;
            }
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
    return ret;
}

void npcExecAllReglEvt(void) {
    s32 wp = (s32)&work;
    s32 i;
    s32 count;
    void* npc;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    i = 0;
    npc = *(void**)(wp + 0xC);
    while (i < count) {
        if (npc != 0 && (*(u32*)npc & 1) && *(void**)((s32)npc + 0x124) != 0) {
            if (*(s32*)((s32)npc + 0x11C) != 0) {
                evtDeleteID(*(s32*)((s32)npc + 0x11C));
            }
            {
                s32 evt = (s32)evtEntry(*(void**)((s32)npc + 0x124), 0, 0x20);
                *(u8*)(evt + 0x10) = 1;
                *(void**)(evt + 0x170) = npc;
                *(s32*)((s32)npc + 0x11C) = *(s32*)(evt + 0x15C);
            }
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
}

s32 _check(void* event, BOOL isFirstCall) {
    extern void evtSetValue(void* event, s32 dst, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 dst = args[0];

    if (*(u8*)((s32)pouchGetPtr() + 0x5B8) != 0) {
        evtSetValue(event, dst, 0);
    } else {
        evtSetValue(event, dst, 1);
    }
    return 2;
}

s32 _majinai_effect(void* pEvt, s32 firstCall) {
    extern void effSetName(void* effect, char* name);
    extern void* effNameToPtr(char* name);
    extern f32 float_16_8041fc8c;
    extern f32 float_0p8_8041fc90;
    extern char str_mjef_coinup_802c13b8[];
    u8* evt = pEvt;
    u8* player = marioGetPtr();
    void* effect;
    f32 scale;

    if (firstCall != 0) {
        *(void**)(evt + 0x78) = effMajinaiEntry(
            0, *(f32*)(player + 0x8C),
            float_16_8041fc8c + *(f32*)(player + 0x90) + *(f32*)(player + 0x1BC),
            *(f32*)(player + 0x94));
        scale = float_0p8_8041fc90;
        effect = *(void**)(evt + 0x78);
        *(f32*)(*(u8**)((u8*)effect + 0xC) + 0x10) = scale;
        effSetName(*(void**)(evt + 0x78), str_mjef_coinup_802c13b8);
    }
    if (*(s32*)(evt + 0x78) != 0) {
        if (effNameToPtr(str_mjef_coinup_802c13b8) != 0) {
            return 0;
        }
    }
    return 2;
}

void npcGetBackItemEntry(void* npc) {
    u8* battle = (u8*)npc + 0x230;
    s32 zero = 0;
    s32 i = 0;

    do {
        s32 id = *(s32*)(battle + 0x7C);
        if (id != 0) {
            void* item = (void*)itemEntry(0, id, 0xB, -1, 0,
                                   *(f32*)((u8*)npc + 0x8C),
                                   *(f32*)((u8*)npc + 0x90),
                                   *(f32*)((u8*)npc + 0x94));
            if (item != 0) {
                itemFlagOn(item, 0x100);
            }
            *(s32*)(battle + 0x7C) = zero;
        }
        i++;
        battle += 4;
    } while (i < 8);
}

void npcClearDeadInfo(void) {
    memset((void*)((s32)*(void**)((s32)gp + 0x168) + 0x4C), 0, 0x500);
}

void npcKoopaModeEncountCheck(void) {
    extern s32 kpaEnemyHitChk(f64 x, f64 y, f64 z, f64 height, f64 width);
    extern void kpaPowDown(void);
    extern char* hitGetName(void* hit);
    extern char str_a_magu_802c138c[];
    s32 wp = (s32)&work;
    s32 count;
    s32 i;
    u8* npc;

    if (gp->inBattle != 0) {
        wp += 0x14;
    }
    count = *(s32*)(wp + 4);
    if (*(s16*)*(u8**)((u8*)gp + 0x168) != 1) {
        return;
    }

    npc = *(u8**)(wp + 0xC);
    for (i = 0; i < count; i++, npc += 0x340) {
        s32 hit;
        u32 flags;

        if (npc == 0) {
            continue;
        }
        flags = *(u32*)npc;
        if ((flags & 1) == 0 || (flags & 0x08000000) == 0 ||
            (*(u32*)(npc + 0x1D4) & 4) != 0) {
            continue;
        }

        hit = kpaEnemyHitChk(*(f32*)(npc + 0x8C), *(f32*)(npc + 0x90),
                            *(f32*)(npc + 0x94), *(f32*)(npc + 0x150),
                            *(f32*)(npc + 0x14C));
        switch (hit) {
            case 1:
                if ((*(u32*)(npc + 0x318) & 1) == 0) npcGroupDead(npc, 1);
                break;
            case 2:
                if ((*(u32*)(npc + 0x318) & 2) == 0) npcGroupDead(npc, 2);
                break;
            case 3:
                if ((*(u32*)(npc + 0x318) & 4) == 0) npcGroupDead(npc, 4);
                break;
            case 4:
                kpaPowDown();
                break;
            default:
                if ((*(u32*)(npc + 0x318) & 0x10) == 0 &&
                    *(void**)(npc + 0x300) != 0 &&
                    strcmp(hitGetName(*(void**)(npc + 0x300)), str_a_magu_802c138c) == 0) {
                    npcGroupDead(npc, 0x10);
                }
                break;
        }
    }
}

s32 npcKoopaModeMobjBoundDeadCheck(void* hit) {
    u8* npcWork = (u8*)&work;
    u8* npc;
    s32 count;
    s32 i;
    s32 result = 0;

    if (*(s32*)((u8*)gp + 0x14) != 0) npcWork += 0x14;
    if (hit == 0) return 0;
    count = *(s32*)(npcWork + 4);
    npc = *(u8**)(npcWork + 0xC);
    for (i = 0; i < count; i++, npc += 0x340) {
        if (npc != 0 && (*(u32*)npc & 1) != 0 &&
            (*(u32*)npc & 0x08000000) != 0 &&
            (*(u32*)(npc + 0x1D4) & 4) == 0 &&
            (*(u16*)(npc + 0x31E) & 8) == 0 &&
            *(void**)(npc + 0x300) == hit) {
            npcGroupDead(npc, 8);
            result++;
        }
    }
    return result;
}

void fbatSandersBombTriggerOn(void) {
    *(u8*)((s32)*(void**)((s32)gp + 0x168) + 0x54D) = 1;
}

s32 npcCalcScore(void* npc) {
    extern s32 kpaMutekiCheck(void);
    typedef struct ScorePosition {
        f32 x;
        f32 y;
        f32 z;
    } ScorePosition;
    ScorePosition position;
    u8* fbat = *(u8**)((u8*)gp + 0x168);
    s32 score = 300;

    position = *(ScorePosition*)((u8*)npc + 0x8C);
    position.y += *(f32*)(*(u8**)((u8*)npc + 0x28) + 0x2C);
    if (*(u16*)((u8*)npc + 0x31C) == 0x10) {
        score = 0;
    } else {
        if (*(u16*)((u8*)npc + 0x31C) == 8) score = 400;
        if (kpaMutekiCheck() != 0) score += 100;
        if (*(u16*)((u8*)npc + 0x31C) == 2) {
            *(s32*)(fbat + 0x574) += 1;
            score *= *(s32*)(fbat + 0x574);
        }
        if (*(u16*)((u8*)npc + 0x31C) == 1) {
            *(s32*)(fbat + 0x578) += 1;
            *(s32*)(fbat + 0x57C) += score;
        }
        if (score > 9999) score = 10000;
    }
    if (score > 0) kpaAddScorePos(score, (f32*)&position);
    return score;
}

s32 npcCheckBlow(void* npc) {
    extern f32 float_0_8041fc7c;
    extern f32 float_0p5_8041fc80;
    typedef struct BlowPosition {
        f32 x;
        f32 y;
        f32 z;
    } BlowPosition;
    BlowPosition position;
    BlowPosition top;
    BlowPosition middle;
    BlowPosition bottom;

    if ((*(u32*)npc & 0x20000000) != 0) return 0;
    if ((*(u32*)npc & 0x20) != 0) return 0;
    position = *(BlowPosition*)((u8*)npc + 0x8C);
    top = position;
    if (cloudGetBreathPower(*(f32*)((u8*)npc + 0x14C), &top) > float_0_8041fc7c) return 1;

    position = *(BlowPosition*)((u8*)npc + 0x8C);
    position.y -= *(f32*)((u8*)npc + 0x150) * float_0p5_8041fc80;
    middle = position;
    if (cloudGetBreathPower(*(f32*)((u8*)npc + 0x14C), &middle) > float_0_8041fc7c) return 1;

    position = *(BlowPosition*)((u8*)npc + 0x8C);
    position.y -= *(f32*)((u8*)npc + 0x150);
    bottom = position;
    return cloudGetBreathPower(*(f32*)((u8*)npc + 0x14C), &bottom) > float_0_8041fc7c;
}

s32 N_fbatPreventMarioEventChk(void) {
    s32 mode = *(s16*)*(void**)((s32)gp + 0x168);
    if (mode == 0) {
        goto ok;
    }
    if (mode == 1) {
        goto ok;
    }
    if (mode == 8) {
        goto ok;
    }
    return 1;
ok:
    return 0;
}

/* Data-pass A1: target-backed .data prefix required by the complete npcdrv Event family. */
char* luigi_pose[28] = {
    str_Z_1_8041fbb8, str_Z_1R_8041fbbc, str_S_1_8041fbc4, str_S_1R_8041fbc8,
    str_W_1_8041fbd0, str_W_1R_8041fbd4, str_R_1_8041fbdc, str_R_1R_8041fbe0,
    str_T_1_8041fbe8, str_T_1R_8041fbec, str_J_1A_8041fbf4, str_J_1RR_8041fbfc,
    str_J_1B_8041fc04, str_J_1BR_8041fc0c, str_Z_2_8041fc14, str_Z_2R_8041fc18,
    str_S_2_8041fc20, str_S_2R_8041fc24, str_T_2_8041fc2c, str_T_2R_8041fc30,
    str_L_1_8041fc38, str_L_1R_8041fc3c, str_L_2_8041fc44, str_L_2R_8041fc48,
    str_L_3_8041fc50, str_L_3R_8041fc54, str_I_1_8041fc5c, str_I_1R_8041fc60,
};


u32 evt_majinai_effect_se_fld[67] = {
    0x0003005B, (u32)btlevtcmd_ftomsec, 0x0000001D, 0xFE363C8A,
    0x0001000A, 0xFE363C8A, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_EVT_FLD_INCANT_C_802c1258, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x000000C8,
    0xFE363C8A, 0x0001000A, 0xFE363C8A, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_EVT_FLD_INCANT_C_802c1274, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_ftomsec,
    0x0000005A, 0xFE363C8A, 0x0001000A, 0xFE363C8A,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_EVT_FLD_INCANT_C_802c1290, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_ftomsec, 0x00000050, 0xFE363C8A, 0x0001000A,
    0xFE363C8A, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_EVT_FLD_INCANT_C_802c12ac,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x0003005B, (u32)btlevtcmd_ftomsec, 0x00000005, 0xFE363C8A,
    0x0001000A, 0xFE363C8A, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_EVT_FLD_INCANT_C_802c12c8, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000002, 0x00000001,
};

u32 evt_majinai_effect_se_btl[67] = {
    0x0003005B, (u32)btlevtcmd_ftomsec, 0x0000001D, 0xFE363C8A,
    0x0001000A, 0xFE363C8A, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_EVT_BTL_INCANT_C_802c12e4, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x000000C8,
    0xFE363C8A, 0x0001000A, 0xFE363C8A, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_EVT_BTL_INCANT_C_802c1300, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_ftomsec,
    0x0000005A, 0xFE363C8A, 0x0001000A, 0xFE363C8A,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_EVT_BTL_INCANT_C_802c131c, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_ftomsec, 0x00000050, 0xFE363C8A, 0x0001000A,
    0xFE363C8A, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_EVT_BTL_INCANT_C_802c1338,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x0003005B, (u32)btlevtcmd_ftomsec, 0x00000005, 0xFE363C8A,
    0x0001000A, 0xFE363C8A, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_EVT_BTL_INCANT_C_802c1354, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000002, 0x00000001,
};

u32 _evt_majinai_coin_up_effect[31] = {
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0001005C, (u32)evt_majinai_effect_se_fld,
    0x0001005B, (u32)_majinai_effect, 0x0005005B, (u32)evt_msg_print,
    0x00000000, (u32)(char*)str_msg_mj_coin_up_802c1370, 0x00000000, 0x00000000,
    0x0002005B, (u32)_check, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0005005B, (u32)evt_msg_print,
    0x00000000, (u32)(char*)str_msg_mj_last_802c1380, 0x00000000, 0x00000000,
    0x00000021, 0x00000002, 0x00000001,
};

/*
 * Target-backed npcdrv read-only string catalog.
 *
 * A9r2 previously authenticated these exact native identities.  A16 adds
 * only the strings now that the leading 0x60-byte target prefix is banked;
 * no executable expression or compiler-generated numeric pool is authored.
 */
__declspec(section ".rodata") char str_SFX_EVT_FLD_INCANT_C_802c1258[25] __attribute__ ((aligned(4))) = "SFX_EVT_FLD_INCANT_CUBE1";
__declspec(section ".rodata") char str_SFX_EVT_FLD_INCANT_C_802c1274[25] __attribute__ ((aligned(4))) = "SFX_EVT_FLD_INCANT_CUBE2";
__declspec(section ".rodata") char str_SFX_EVT_FLD_INCANT_C_802c1290[25] __attribute__ ((aligned(4))) = "SFX_EVT_FLD_INCANT_CUBE3";
__declspec(section ".rodata") char str_SFX_EVT_FLD_INCANT_C_802c12ac[25] __attribute__ ((aligned(4))) = "SFX_EVT_FLD_INCANT_CUBE4";
__declspec(section ".rodata") char str_SFX_EVT_FLD_INCANT_C_802c12c8[25] __attribute__ ((aligned(4))) = "SFX_EVT_FLD_INCANT_CUBE5";
__declspec(section ".rodata") char str_SFX_EVT_BTL_INCANT_C_802c12e4[25] __attribute__ ((aligned(4))) = "SFX_EVT_BTL_INCANT_CUBE1";
__declspec(section ".rodata") char str_SFX_EVT_BTL_INCANT_C_802c1300[25] __attribute__ ((aligned(4))) = "SFX_EVT_BTL_INCANT_CUBE2";
__declspec(section ".rodata") char str_SFX_EVT_BTL_INCANT_C_802c131c[25] __attribute__ ((aligned(4))) = "SFX_EVT_BTL_INCANT_CUBE3";
__declspec(section ".rodata") char str_SFX_EVT_BTL_INCANT_C_802c1338[25] __attribute__ ((aligned(4))) = "SFX_EVT_BTL_INCANT_CUBE4";
__declspec(section ".rodata") char str_SFX_EVT_BTL_INCANT_C_802c1354[25] __attribute__ ((aligned(4))) = "SFX_EVT_BTL_INCANT_CUBE5";
__declspec(section ".rodata") char str_msg_mj_coin_up_802c1370[15] __attribute__ ((aligned(4))) = "msg_mj_coin_up";
__declspec(section ".rodata") char str_msg_mj_last_802c1380[12] __attribute__ ((aligned(4))) = "msg_mj_last";
__declspec(section ".rodata") char str_a_magu_802c138c[7] __attribute__ ((aligned(4))) = "a_magu";
__declspec(section ".rodata") char str_kpa_03_802c1394[7] __attribute__ ((aligned(4))) = "kpa_03";
__declspec(section ".rodata") char str_SFX_KUPPA_3RD_WATER__802c139c[26] __attribute__ ((aligned(4))) = "SFX_KUPPA_3RD_WATER_FALL1";
__declspec(section ".rodata") char str_mjef_coinup_802c13b8[12] __attribute__ ((aligned(4))) = "mjef_coinup";
/*
 * Target-backed npcdrv named double family.
 *
 * Natural 8-byte alignment after the retained string catalog places these at
 * target +0x1D0, +0x1D8, and +0x1E0. The compiler-generated conversion
 * double remains natural at target +0x1E8.
 */
__declspec(section ".rodata") const f64 double_0p5_802c13c8
    __attribute__ ((aligned(8))) = 0.5;
__declspec(section ".rodata") const f64 double_3_802c13d0
    __attribute__ ((aligned(8))) = 3.0;
__declspec(section ".rodata") const f64 double_0_802c13d8
    __attribute__ ((aligned(8))) = 0.0;

/*
 * Data-pass A39: complete target-backed npcdrv .sdata2 catalog.
 *
 * Definitions intentionally appear after all executable consumers so functions
 * retain declaration-only views.  The six remaining private literal producers
 * above are expressed through these target identities, eliminating the
 * anonymous pool that previously occupied +0xC4..+0xF3.
 */
__declspec(section ".sdata2") const f32 float_0_8041fc7c = 0.0f;
__declspec(section ".sdata2") const f32 float_0p5_8041fc80 = 0.5f;
__declspec(section ".sdata2") const f32 float_60_8041fc84 = 60.0f;
__declspec(section ".sdata2") const f32 float_365_8041fc88 = 365.0f;
__declspec(section ".sdata2") const f32 float_16_8041fc8c = 16.0f;
__declspec(section ".sdata2") const f32 float_0p8_8041fc90 = 0.800000011920929f;
__declspec(section ".sdata2") const f32 float_1_8041fc94 = 1.0f;
__declspec(section ".sdata2") const f32 float_10_8041fc98 = 10.0f;
__declspec(section ".sdata2") const f32 float_11_8041fc9c = 11.0f;
__declspec(section ".sdata2") const f32 float_12_8041fca0 = 12.0f;
__declspec(section ".sdata2") const f32 float_15_8041fca4 = 15.0f;
__declspec(section ".sdata2") const f32 float_1000_8041fca8 = 1000.0f;
__declspec(section ".sdata2") const f32 float_1p5_8041fcac = 1.5f;
__declspec(section ".sdata2") const f32 float_90_8041fcb0 = 90.0f;
__declspec(section ".sdata2") const f32 float_30_8041fcb4 = 30.0f;
__declspec(section ".sdata2") const f32 float_neg90_8041fcb8 = -90.0f;
__declspec(section ".sdata2") const f32 float_rad2deg_8041fcbc = 57.29579162597656f;
__declspec(section ".sdata2") const f32 float_360_8041fcc0 = 360.0f;
__declspec(section ".sdata2") const f32 float_270_8041fcc4 = 270.0f;
__declspec(section ".sdata2") const f32 float_neg270_8041fcc8 = -270.0f;
__declspec(section ".sdata2") const f32 float_180_8041fccc = 180.0f;
__declspec(section ".sdata2") const f32 float_neg180_8041fcd0 = -180.0f;
__declspec(section ".sdata2") const f32 float_0p75_8041fcd4 = 0.75f;
__declspec(section ".sdata2") const f32 float_neg1_8041fcd8 = -1.0f;
__declspec(section ".sdata2") const f32 float_0p1_8041fcdc = 0.10000000149011612f;
__declspec(section ".sdata2") const f32 float_3_8041fce0 = 3.0f;
__declspec(section ".sdata2") const f32 float_neg600_8041fce4 = -600.0f;
__declspec(section ".sdata2") const f32 float_600_8041fce8 = 600.0f;
__declspec(section ".sdata2") const f32 float_0p2_8041fcec = 0.20000000298023224f;
__declspec(section ".sdata2") const f32 float_100_8041fcf0 = 100.0f;
__declspec(section ".sdata2") const f32 float_0p4_8041fcf4 = 0.4000000059604645f;
__declspec(section ".sdata2") const f32 float_45_8041fcf8 = 45.0f;
__declspec(section ".sdata2") const f32 float_neg980_8041fcfc = -980.0f;
__declspec(section ".sdata2") const f32 float_0p15_8041fd00 = 0.15000000596046448f;
__declspec(section ".sdata2") const f32 float_neg1000_8041fd04 = -1000.0f;
__declspec(section ".sdata2") const f32 float_5_8041fd08 = 5.0f;
__declspec(section ".sdata2") const f32 float_40_8041fd0c = 40.0f;
__declspec(section ".sdata2") const f32 float_6p2832_8041fd10 = 6.2831854820251465f;
__declspec(section ".sdata2") const f32 float_0p00761_8041fd14 = 0.00761000020429492f;
__declspec(section ".sdata2") const f32 float_0p16605_8041fd18 = 0.1660500019788742f;
__declspec(section ".sdata2") const f32 float_3p1416_8041fd1c = 3.1415927410125732f;
__declspec(section ".sdata2") const f32 float_1p5708_8041fd20 = 1.5707963705062866f;
__declspec(section ".sdata2") const f32 float_4p7124_8041fd24 = 4.71238899230957f;
__declspec(section ".sdata2") const f32 float_0p03705_8041fd28 = 0.037050001323223114f;
__declspec(section ".sdata2") const f32 float_0p4967_8041fd2c = 0.4966999888420105f;
__declspec(section ".sdata2") const f32 float_4_8041fd30 = 4.0f;
__declspec(section ".sdata2") const f32 float_2_8041fd34 = 2.0f;
__declspec(section ".sdata2") const f32 float_160_8041fd38 = 160.0f;
__declspec(section ".sdata2") const f32 float_20_8041fd3c = 20.0f;
__declspec(section ".sdata2") char str_party_8041fd40[6] __attribute__ ((aligned(4))) = "party";
__declspec(section ".sdata2") char str_mario_8041fd48[6] __attribute__ ((aligned(4))) = "mario";
__declspec(section ".sdata2") const f32 float_1E06_8041fd50 = 1000000.0f;
__declspec(section ".sdata2") const f32 float_0p05_8041fd54 = 0.05000000074505806f;
__declspec(section ".sdata2") const f32 float_0p25_8041fd58 = 0.25f;
__declspec(section ".sdata2") const f32 float_neg2_8041fd5c = -2.0f;
__declspec(section ".sdata2") const f32 float_5000_8041fd60 = 5000.0f;
__declspec(section ".sdata2") const f32 float_10000_8041fd64 = 10000.0f;
__declspec(section ".sdata2") const f32 float_1p33_8041fd68 = 1.3300000429153442f;
__declspec(section ".sdata2") char str_hoshi_8041fd6c[6] __attribute__ ((aligned(4))) = "hoshi";
__declspec(section ".sdata2") char str_M_I_2_8041fd74[6] __attribute__ ((aligned(4))) = "M_I_2";
__declspec(section ".sdata2") const f32 float_deg2rad_8041fd7c = 0.01745329238474369f;
