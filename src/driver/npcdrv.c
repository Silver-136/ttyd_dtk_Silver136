#include "driver/npcdrv.h"

#include "mario/mariost.h"

// This is only here to make things easier to reference
typedef struct NPCWork2 {
    NPCWork field;  //0x0
    NPCWork battle; //0x14
} NPCWork2;

NPCWork2 work;
char MarioStayPose[0x20] = "KPA_S_1";
char MarioTalkPose[0x20] = "KPA_T_1";

NPCWork* npcGetWorkPtr(void) {
    return gp->inBattle ? &work.battle : &work.field;
}

void npcInit(void) {
    NPCWork* wp = &work.field;
}

void* fbatGetPointer(void) {
    return *(void**)((s32)gp + 0x168);
}

void npcSetLink(void* a, void* b) {
    *(void**)((s32)a + 0x328) = b;
    *(void**)((s32)b + 0x324) = a;
}

void fbatSandersBombTriggerOn(void) {
    *(u8*)((s32)*(void**)((s32)gp + 0x168) + 0x54D) = 1;
}

void fbatSetAttackAnnounceEnable(void) {
    *(s32*)((s32)*(void**)((s32)gp + 0x168) + 0x550) = 1;
}

void fbatChangeMode(s16 mode) {
    *(s16*)*(void**)((s32)gp + 0x168) = mode;
}

void fbatHitCheckAll(void) {
    extern void fbatHitCheck(s32, s32);
    fbatHitCheck(0x13F1, 0);
}

void npcClearDeadInfo(void) {
    extern void* memset(void*, int, unsigned long);
    memset((void*)((s32)*(void**)((s32)gp + 0x168) + 0x4C), 0, 0x500);
}

s32 npcGetReactionOfLivingBody(s32 battle) {
    void* ptr = &work.field;
    if (battle) {
        ptr = (void*)((s32)ptr + 0x14);
    }
    return *(s32*)ptr;
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

void npcStartForOneEvent(void* npc) {
    extern void evtStartID(s32 id);
    extern void animPoseSetLocalTimeRate(void* pose, f32 rate);
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
    animPoseSetLocalTimeRate(*(void**)((s32)npc + 0x104), float_1_8041fc94);
}

void npcStopForOneEvent(void* npc) {
    extern void evtStopID(s32 id);
    extern void animPoseSetLocalTimeRate(void* pose, f32 rate);
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
    animPoseSetLocalTimeRate(*(void**)((s32)npc + 0x104), float_0_8041fc7c);
}

void npcTuningRy(void* npc, f32 angle) {
    extern f32 reviseAngle(f32 angle);
    *(f32*)((s32)npc + 0x144) = reviseAngle(angle);
}

void npcSetMarioAutoTalkPose(const char* stay, const char* talk) {
    extern char* strcpy(char* dest, const char* src);
    strcpy(MarioStayPose, stay);
    strcpy(MarioTalkPose, talk);
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

s32 _check(void* event, BOOL isFirstCall) {
    extern void* pouchGetPtr(void);
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

void* npcGetTribe(const char* name) {
    extern char npcTribe[];
    extern s32 strcmp(const char* s1, const char* s2);
    void* tribe = npcTribe;

    while (*(char**)tribe != 0) {
        if (strcmp(*(char**)tribe, name) == 0) {
            return tribe;
        }
        tribe = (void*)((s32)tribe + 0x54);
    }
    return 0;
}

void* dbGetDefData(void* entry, const char* name) {
    extern s32 strcmp(const char* s1, const char* s2);
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

void npcReset(s32 battle) {
    extern void* memset(void* dest, int value, unsigned long size);
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

void* fbatNpcTalkCheck(void) {
    extern s32 marioChkTalkable(void);
    void* wp = &work.field;
    void* ret;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        wp = (void*)((s32)wp + 0x14);
    }
    ret = *(void**)((s32)wp + 0x10);
    if (ret != 0 && marioChkTalkable() != 0) {
        return ret;
    }
    return 0;
}


void fbatHitCheck(s32, s32) {
    ;
}


u8 npcMain(void) {
    return 0;
}


u8 fbatEncountCheck(void) {
    return 0;
}


u8 npcHitCheckSide(void) {
    return 0;
}


u8 fbatBattleMode(void) {
    return 0;
}


u8 npcCheckHitMarioSide(s32 pNpc) {
    return 0;
}


u8 _npcHitCheckHammerAllMotion(void) {
    return 0;
}


u8 _npcGroupDead(u32* param_1, s16 param_2) {
    return 0;
}


u8 npcDisp_offscreen(void) {
    return 0;
}


void npcDisp_blur(s32 cameraId, void* npc) {
    extern f32 reviseAngle(f32);
    extern u32 animPoseGetMaterialFlag(s32);
    extern void* camGetPtr(s32);
    extern void animPoseMain(s32);
    extern void mtx_setup(void*, f32 (*)[4], s32);
    extern void PSMTXTransApply(f32 (*)[4], f32 (*)[4], f32, f32, f32);
    extern void animPoseSetMaterialFlagOn(s32, u32);
    extern void animPoseSetMaterialEvtColor(s32, void*);
    extern void animPoseDrawMtx(s32, f32 (*)[4], s32, f32, f32);
    extern void animPoseSetMaterialFlagOff(s32, u32);
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

void npcDisp_offscreen_xlu(s32 cameraId, void* npc) {
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern f32 reviseAngle(f32);
    extern void mtx_setup(void*, f32 (*)[4], s32);
    extern void animPoseSetMaterialEvtColor(s32, void*);
    extern void animPoseSetMaterialFlagOn(s32, u32);
    extern char* animPoseGetCurrentAnim(s32);
    extern void* animPoseGetAnimPosePtr(s32);
    extern void* camGetPtr(s32);
    extern f64 atan2(f64, f64);
    extern s32 strcmp(const char*, const char*);
    extern void animPoseSetAnim(s32, char*, s32);
    extern void animPoseMain(s32);
    extern void animPoseDrawMtx(s32, f32 (*)[4], s32, f32, f32);
    extern void GXReadBoundingBox(u16*, u16*, u16*, u16*);
    extern void offscreenAddBoundingBox(s32, u16, u16, u16, u16);
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

void npcDisp(s32 cameraId, void* npc) {
    extern f32 reviseAngle(f32);
    extern void mtx_setup(void*, f32 (*)[4], s32);
    extern void animPoseSetMaterialEvtColor(s32, void*);
    extern void animPoseSetMaterialFlagOn(s32, u32);
    extern void* dispGetCurWork(void);
    extern char* animPoseGetCurrentAnim(s32);
    extern void* animPoseGetAnimPosePtr(s32);
    extern void* camGetPtr(s32);
    extern f64 atan2(f64, f64);
    extern s32 strcmp(const char*, const char*);
    extern void animPoseSetAnim(s32, char*, s32);
    extern void animPoseMain(s32);
    extern void animPoseDrawMtx(s32, f32 (*)[4], s32, f32, f32);
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

void mtx_setup(void* npc, f32 mtx[3][4], s32 historyIndex) {
    extern void* camGetCurPtr(void);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 reviseAngle(f32 angle);
    extern void PSMTXIdentity(f32 mtx[3][4]);
    extern void PSMTXScaleApply(f32 src[3][4], f32 dst[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXTransApply(f32 src[3][4], f32 dst[3][4], f32 x, f32 y, f32 z);
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

void _npcDeleteGroup(void* npc) {
    extern void npcDelete(void* npc);
    extern void animPoseRelease(s32 poseId);
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

void npcDisp_xlu(s32 cameraId, void* npc) {
    extern f32 reviseAngle(f32);
    extern void mtx_setup(void*, f32 (*)[4], s32);
    extern void animPoseSetMaterialEvtColor(s32, void*);
    extern void animPoseSetMaterialFlagOn(s32, u32);
    extern char* animPoseGetCurrentAnim(s32);
    extern void* animPoseGetAnimPosePtr(s32);
    extern void* camGetPtr(s32);
    extern f64 atan2(f64, f64);
    extern s32 strcmp(const char*, const char*);
    extern void animPoseSetAnim(s32, char*, s32);
    extern void animPoseMain(s32);
    extern void animPoseDrawMtx(s32, f32 (*)[4], s32, f32, f32);
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

void fbatTalkMode(void) {
    extern void* marioGetPtr(void);
    extern void evtStopID(s32 id);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 reviseAngle(f32 angle);
    extern void marioKeyOff(void);
    extern void partyKeyOff(void);
    extern void* evtEntry(void* code, s32 order, u32 flags);
    extern void marioChgTalkMotion(void);
    extern s32 evtCheckID(s32 id);
    extern void evtStartID(s32 id);
    extern void animPoseSetAnim(s32 poseId, void* name, s32 force);
    extern void marioKeyOn(void);
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
            if (*(s16*)fbatData == 3) {
                entry = evtEntry(*(void**)(npc + 0x12C), 0, 0);
                *(u8*)((s32)entry + 0x10) = 6;
            } else {
                entry = evtEntry(*(void**)(npc + 0x128), 0, 0);
                *(u8*)((s32)entry + 0x10) = 6;
                marioChgTalkMotion();
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

u8 npcKoopaModeEncountCheck(void) {
    return 0;
}


int _npcHitCheckSphere(double param_1, double param_2, double param_3, double param_4, int param_5, float* param_6) {
    return 0;
}


u32* npcNearDistCheck(double param_1, s64 param_2, double param_3, double param_4) {
    return 0;
}


void npcSetStayPose(char* stayPose) {
    ;
}


void npcSetTalkPose(char* talkPose) {
    ;
}


u8 npcRecoveryFiledNpc(void) {
    return 0;
}


void npcBlurOn(char* name) {
    extern s32 strcmp(const char*, const char*);
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
    *(s32*)((s32)npc + 0xA4) = *(s32*)((s32)npc + 0x8C);
    *(s32*)((s32)npc + 0xA8) = *(s32*)((s32)npc + 0x90);
    *(s32*)((s32)npc + 0xAC) = *(s32*)((s32)npc + 0x94);
    *(s32*)((s32)npc + 0xB0) = *(s32*)((s32)npc + 0x8C);
    *(s32*)((s32)npc + 0xB4) = *(s32*)((s32)npc + 0x90);
    *(s32*)((s32)npc + 0xB8) = *(s32*)((s32)npc + 0x94);
    *(s32*)((s32)npc + 0xBC) = *(s32*)((s32)npc + 0x8C);
    *(s32*)((s32)npc + 0xC0) = *(s32*)((s32)npc + 0x90);
    *(s32*)((s32)npc + 0xC4) = *(s32*)((s32)npc + 0x94);
    *(s32*)((s32)npc + 0xC8) = *(s32*)((s32)npc + 0x8C);
    *(s32*)((s32)npc + 0xCC) = *(s32*)((s32)npc + 0x90);
    *(s32*)((s32)npc + 0xD0) = *(s32*)((s32)npc + 0x94);
    *(s32*)((s32)npc + 0xD4) = *(s32*)((s32)npc + 0x8C);
    *(s32*)((s32)npc + 0xD8) = *(s32*)((s32)npc + 0x90);
    *(s32*)((s32)npc + 0xDC) = *(s32*)((s32)npc + 0x94);
}


u8 npcReleaseFiledNpc(void) {
    return 0;
}


void npcSetBattleInfo(void* npc, s32 info) {
    ;
}


u8 fbatSetAttackAnnounce(s32 param_1) {
    return 0;
}


s32 npcWaitAllInitEvtEnd(void) {
    extern s32 evtCheckID(s32 id);
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

void npcExecAllInitEvt(void) {
    extern s32 evtEntry(void* script, s32 type, s32 flags);
    extern void evtDeleteID(s32 id);
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
                    s32 evt = evtEntry(*(void**)((s32)npc + 0x120), 0, 0);
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

void npcExecAllReglEvt(void) {
    extern s32 evtEntry(void* script, s32 type, s32 flags);
    extern void evtDeleteID(s32 id);
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
                s32 evt = evtEntry(*(void**)((s32)npc + 0x124), 0, 0x20);
                *(u8*)(evt + 0x10) = 1;
                *(void**)(evt + 0x170) = npc;
                *(s32*)((s32)npc + 0x11C) = *(s32*)(evt + 0x15C);
            }
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
}

s32 _majinai_effect(void* pEvt, int param_2) {
    return 0;
}


void npcStartForEvent(void) {
    extern void evtStartID(s32 id);
    extern void animPoseSetLocalTimeRate(void* pose, f32 rate);
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
            animPoseSetLocalTimeRate(*(void**)((s32)npc + 0x104), float_1_8041fc94);
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
}

void* npcNameToPtr_NoAssert(s32 name) {
    extern s32 strcmp(const char*, const char*);
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

void npcStopForEvent(void) {
    extern void evtStopID(s32 id);
    extern void animPoseSetLocalTimeRate(void* pose, f32 rate);
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
            animPoseSetLocalTimeRate(*(void**)((s32)npc + 0x104), float_0_8041fc7c);
        }
        i++;
        npc = (void*)((s32)npc + 0x340);
    }
}

void npcBlurOff(char* name) {
    extern s32 strcmp(const char*, const char*);
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

void npcGetBackItemEntry(void* npc) {
    ;
}


void* npcNameToPtr(s32 name) {
    extern s32 strcmp(const char*, const char*);
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

u8 _npcDead(u32* param_1, s16 param_2) {
    return 0;
}


u8 _fbatFirstAttackAnnouceDisp(s32 param_1, void* param_2) {
    return 0;
}


u8 npcMoveG(void* pNpc) {
    return 0;
}


void npcSetupBattleInfo(void* npc, void* battleInfo) {
    extern void* memset(void* dst, int value, unsigned long size);
    extern s32 irand(s32 max);
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

void npcDelete(void* npc) {
    ;
}


void npcSetColor(char* name, void* color) {
    extern s32 strcmp(const char*, const char*);
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

s32 npcEntry(char* name, char* modelName) {
    extern s32 strcmp(const char*, const char*);
    extern void* memset(void*, int, unsigned long);
    extern char* strcpy(char*, const char*);
    extern s32 animPoseEntry(char*, u32);
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
    i = 0;
    if (max > 0) {
        for (;;) {
            if ((*(u32*)npc & 1) == 0) {
                break;
            }
            i++;
            npc += 0x340;
            if (i >= max) {
                break;
            }
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
    *(s32*)(npc + 0x8C) = *(s32*)wp;
    *(s32*)(npc + 0x90) = *(s32*)(wp + 4);
    *(s32*)(npc + 0x94) = *(s32*)(wp + 8);
    *(s32*)(npc + 0x98) = *(s32*)(npc + 0x8C);
    *(s32*)(npc + 0x9C) = *(s32*)(npc + 0x90);
    *(s32*)(npc + 0xA0) = *(s32*)(npc + 0x94);
    *(f32*)(npc + 0x14C) = animPoseGetHeight(*(s32*)(npc + 0x104));
    *(f32*)(npc + 0x150) = animPoseGetRadius(*(s32*)(npc + 0x104));
    *(s32*)(npc + 0xE0) = *(s32*)(wp + 0xC);
    *(s32*)(npc + 0xE4) = *(s32*)(wp + 0x10);
    *(s32*)(npc + 0xE8) = *(s32*)(wp + 0x14);
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

s32 npcCalcScore(void* npc) {
    return 0;
}


s32 npcKoopaModeMobjBoundDeadCheck(void* hit) {
    return 0;
}


void npcGroupDead(void* npc, s32 wKpaScoreType) {
    ;
}


s32 npcCheckInterrupt(void* pNpc) {
    extern void* evtGetPtrID(s32 id);
    extern void* evtEntry(void* code, s32 order, u32 flags);
    extern void evtDeleteID(s32 id);
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

s32 npcCheckBlow(void* npc) {
    return 0;
}
