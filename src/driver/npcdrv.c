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


void fbatHitCheck(s32 flags, s32 hitInfo) {
    extern NPCWork2 work;
    extern void* marioGetPtr(void);
    extern f64 sqrt(f64 value);
    extern s32 seqGetSeq(void);
    extern s32 vivianGetStatus(void);
    extern s32 pouchEquipCheckBadge(s32 badge);
    extern s32 marioGetJabaraState(void);
    extern s32 _npcHitCheckSphere(f64 x, f64 y, f64 z, f64 radius,
                                  s32 npc, f32* distance);
    extern f32 float_1000_8041fca8;
    extern f32 float_0p5_8041fc80;
    s32 npcWork;
    s32 player;
    s32 npc;
    s32 count;
    s32 i;
    s32 bestNpc = 0;
    u32 hitFlags = 0;
    u32 attackMode = 0;
    u32 unknownFlags = 0;
    f32 bestDistance = float_1000_8041fca8;

    npcWork = (s32)&work.field;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        npcWork += 0x14;
    }
    player = (s32)marioGetPtr();
    count = *(s32*)(npcWork + 4);
    npc = *(s32*)(npcWork + 0xC);

    if ((flags & 0x2000) != 0) {
        unknownFlags |= 1;
    }

    if ((flags & 0x30) != 0 && seqGetSeq() == 2 && vivianGetStatus() == 0) {
        for (i = 0; i < count; i++, npc += 0x340) {
            u32 npcFlags;
            f32 dx;
            f32 dz;
            f32 distance;
            f32 y;

            if (npc == 0) {
                continue;
            }
            npcFlags = *(u32*)npc;
            if ((npcFlags & 2) == 0 || (npcFlags & 0x20) != 0 ||
                (npcFlags & 0x08000000) != 0 ||
                (*(u32*)(npc + 0x1D4) & 4) != 0) {
                continue;
            }
            if ((flags & 0x1000) == 0) {
                if ((npcFlags & 8) == 0) {
                    continue;
                }
            } else if ((npcFlags & 8) != 0) {
                continue;
            }

            if (*(f32*)(player + 0x90) >=
                    *(f32*)(npc + 0x90) + *(f32*)(npc + 0x150) ||
                *(f32*)(npc + 0x90) >=
                    *(f32*)(player + 0x90) + *(f32*)(player + 0xFC)) {
                continue;
            }

            dx = *(f32*)(player + 0x8C) - *(f32*)(npc + 0x8C);
            dz = *(f32*)(player + 0x94) - *(f32*)(npc + 0x94);
            distance = (f32)sqrt((f64)(dx * dx + dz * dz));
            if (distance < float_0p5_8041fc80 *
                               (*(f32*)(player + 0xF8) + *(f32*)(npc + 0x14C)) &&
                distance < bestDistance) {
                bestDistance = distance;
                bestNpc = npc;
                if ((flags & 0x10) != 0) {
                    hitFlags = pouchEquipCheckBadge(0x1D) == 0 ?
                                   0x10000000 : 0x10000;
                }
                if ((flags & 0x20) != 0 && marioGetJabaraState() == 6) {
                    hitFlags = 0x20000;
                }
                if ((flags & 0x2000) != 0) {
                    break;
                }
            }
        }
    }

    if ((flags & 1) != 0) {
        npc = *(s32*)(npcWork + 0xC);
        for (i = 0; i < count; i++, npc += 0x340) {
            f32 distance;
            if (npc != 0 && (*(u32*)npc & 2) != 0 &&
                (*(u32*)npc & 0x20000020) == 0 &&
                _npcHitCheckSphere((f64)*(f32*)(player + 0x8C),
                                   (f64)(*(f32*)(player + 0x90) +
                                         float_0p5_8041fc80 * *(f32*)(player + 0xFC)),
                                   (f64)*(f32*)(player + 0x94), 30.0, npc,
                                   &distance) != 0 &&
                distance < bestDistance) {
                bestDistance = distance;
                bestNpc = npc;
                hitFlags = 0x1000000;
            }
        }
    }

    if (hitInfo != 0) {
        *(u32*)hitInfo = hitFlags;
        *(u32*)(hitInfo + 4) = attackMode;
        *(f32*)(hitInfo + 8) = bestDistance;
        *(u32*)(hitInfo + 0xC) = unknownFlags;
    }
    (void)bestNpc;
}

void npcMain(void) {
    extern void* marioGetPtr(void);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern s64 __div2i(s64 dividend, s64 divisor);
    extern f64 __cvt_sll_flt(s32 hi, u32 lo);
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
    extern u8 _fbatFirstAttackAnnouceDisp(s32 cameraId, void* info);
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
    extern f32 float_neg2_8041fd5c;
    extern f32 float_2_8041fd34;
    extern f32 float_5000_8041fd60;
    extern f32 float_10000_8041fd64;
    extern f32 float_1p33_8041fd68;
    extern f32 float_1E06_8041fd50;
    extern char str_kpa_03_802c1394[];
    extern char str_SFX_KUPPA_3RD_WATER__802c139c[];
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
    u32 flags;
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
    f32 tmpVec[3];
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

    npcWork = (void*)&work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        npcWork = (void*)((s32)npcWork + 0x14);
    }

    player = marioGetPtr();
    *(void**)((s32)player + 0x1DC) = 0;
    count = *(s32*)((s32)npcWork + 4);
    npcMainCount++;
    npc = *(void**)((s32)npcWork + 0xC);

    for (i = 0; i < count; i++) {
        flags = *(u32*)npc;
        if ((flags & 1) && (flags & 2)) {
            if ((flags & 0x8000) != 0) {
                pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
                *(u32*)pose |= 0x100;
            } else {
                pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
                *(u32*)pose &= ~0x100;
            }

            if ((flags & 0x100000) == 0) {
                *(u32*)((s32)npc + 0x1D4) |= 0x10;
            }
            if ((*(u32*)((s32)npc + 0x1D4) & 0x10000) != 0) {
                *(u32*)((s32)npc + 0x1D4) &= ~0x10;
            }
            if (*(u16*)((s32)npc + 0x320) != 0) {
                *(u16*)((s32)npc + 0x320) = *(u16*)((s32)npc + 0x320) - 1;
            }

            if ((*(u32*)((s32)npc + 0x1D4) & 0x10) == 0) {
                *(u32*)((s32)npc + 0x198) = *(u32*)((s32)gp + 0x38);
                *(u32*)((s32)npc + 0x19C) = *(u32*)((s32)gp + 0x3C);
            } else {
                oldHi = *(u32*)((s32)npc + 0x198);
                oldLo = *(u32*)((s32)npc + 0x19C);
                hi = *(u32*)((s32)gp + 0x38);
                lo = *(u32*)((s32)gp + 0x3C);
                *(u32*)((s32)npc + 0x194) = lo - oldLo;
                *(u32*)((s32)npc + 0x190) = hi - oldHi - (lo < oldLo);

                divv = __div2i(*(s64*)((s32)npc + 0x190), (s64)(DAT_800000f8 / 4000));
                if (divv > 500) {
                    *(u32*)((s32)npc + 0x190) = 0;
                    *(u32*)((s32)npc + 0x194) = (DAT_800000f8 / 4000) << 4;
                }

                if ((flags & 0x20000) == 0) {
                    *(u32*)((s32)npc + 0x178) = 0;
                    *(u32*)((s32)npc + 0x17C) = 0;
                } else {
                    *(u64*)((s32)npc + 0x178) = *(u64*)((s32)npc + 0x178) + *(u64*)((s32)npc + 0x190);
                }
                *(u64*)((s32)npc + 0x188) = *(u64*)((s32)npc + 0x188) + *(u64*)((s32)npc + 0x190);
                *(u32*)((s32)npc + 0x198) = *(u32*)((s32)gp + 0x38);
                *(u32*)((s32)npc + 0x19C) = *(u32*)((s32)gp + 0x3C);

                dval = __cvt_sll_flt((s32)((*(s64*)((s32)npc + 0x188) << 3) >> 32), (u32)(*(s64*)((s32)npc + 0x188) << 3));
                *(f32*)((s32)npc + 0x1A0) = (f32)(dval / (f64)float_1E06_8041fd50);
                dval = __cvt_sll_flt((s32)((*(s64*)((s32)npc + 0x178) << 3) >> 32), (u32)(*(s64*)((s32)npc + 0x178) << 3));
                *(f32*)((s32)npc + 0x180) = (f32)(dval / (f64)float_1E06_8041fd50);
                dval = __cvt_sll_flt((s32)((*(s64*)((s32)npc + 0x190) << 3) >> 32), (u32)(*(s64*)((s32)npc + 0x190) << 3));
                *(f32*)((s32)npc + 0x1A4) = (f32)(dval / (f64)float_1E06_8041fd50);
            }

            jumpFlags = *(u32*)((s32)npc + 0x1D4);
            if ((jumpFlags & 0x100000) == 0) {
                *(u32*)((s32)npc + 0x1D4) = jumpFlags & ~0x200000;
            } else {
                *(u32*)((s32)npc + 0x1D4) = jumpFlags | 0x200000;
            }
            *(u32*)((s32)npc + 0x1D4) &= ~0x100000;

            if (strcmp((char*)((s32)gp + 0x1C), str_kpa_03_802c1394) == 0 && *(f32*)((s32)npc + 0x90) <= float_365_8041fc88) {
                *(u32*)((s32)npc + 0x1D4) |= 0x100000;
                if ((*(u32*)((s32)npc + 0x1D4) & 0x200000) == 0) {
                    psndSFXOn_3D(str_SFX_KUPPA_3RD_WATER__802c139c, (void*)((s32)npc + 0x8C));
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
                if ((flags & 0x8000000) != 0) {
                    moveZ = float_0_8041fc7c;
                }
                if ((flags & 0x40000000) == 0) {
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

            flags = *(u32*)npc;
            if (((flags & 8) == 0) && ((flags & 0x20) == 0) && ((flags & 0x4000) == 0)) {
                npcCheckHitMarioSide((s32)npc);
            }

            PSVECSubtract((void*)((s32)npc + 0x8C), (void*)((s32)npc + 0x98), tmpVec);
            if (((flags & 0x400) != 0) && (*(f32*)((s32)npc + 0x1A4) > float_0_8041fc7c)) {
                speed = tmpVec[0] * tmpVec[0] + tmpVec[2] * tmpVec[2];
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
                    if (*(void**)((s32)npc + 0x13C) != 0 && ((flags & 0x20000000) == 0) && ((flags & 0x20) == 0)) {
                        tmpVec[0] = *(f32*)((s32)npc + 0x8C);
                        tmpVec[1] = *(f32*)((s32)npc + 0x90);
                        tmpVec[2] = *(f32*)((s32)npc + 0x94);
                        if (cloudGetBreathPower(*(f32*)((s32)npc + 0x14C), tmpVec) > float_0_8041fc7c) {
                            *(u32*)((s32)npc + 0x140) |= 8;
                        }
                    }
                    npcCheckInterrupt(npc);
                }
            }

            if ((flags & 0x800) == 0) {
                cam = camGetPtr(4);
                PSMTXMultVec((void*)((s32)cam + 0x18C), (void*)((s32)npc + 0x8C), projVec);
                cam = camGetPtr(4);
                PSMTX44MultVec((void*)((s32)cam + 0x1BC), projVec, projVec);
                if (((projVec[2] > float_neg1_8041fcd8) && (projVec[2] < float_0_8041fc7c) &&
                     (projVec[0] >= float_neg2_8041fd5c) && (projVec[0] <= float_2_8041fd34) &&
                     (projVec[1] >= float_neg2_8041fd5c) && (projVec[1] <= float_2_8041fd34)) || ((flags & 0x40) != 0)) {
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
                    if (((flags & 0x80) == 0) && (((*(u16*)((s32)npc + 0x320) & 2) == 0) || ((*(u32*)((s32)npc + 0x1D4) & 0x10000) != 0))) {
                        if (*(u8*)((s32)npc + 0x117) == 0xFF) {
                            dispEntry(*(s32*)((s32)npc + 0x1F4), 0, npcDisp, npc, order);
                            dispEntry(*(s32*)((s32)npc + 0x1F4), 1, npcDisp, npc, order);
                            dispEntry(*(s32*)((s32)npc + 0x1F4), 2, npcDisp, npc, order);
                        } else {
                            dispEntry(*(s32*)((s32)npc + 0x1F4), 2, npcDisp_xlu, npc, order);
                        }
                        if ((flags & 0x4000000) == 0) {
                            if ((flags & 0x40000) == 0) {
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
                                if ((flags & 0x80000) != 0) {
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
                            if ((flags & 0x80000) != 0) {
                                shadowSetCollision(shadowId, 0);
                            }
                        }
                        if ((flags & 0x100) != 0) {
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
            if ((flags & 0x100) && ((npcMainCount & 1) == 0)) {
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
    *(u8*)((s32)fbatData + 0x54D) = 0;
}

void fbatEncountCheck(void) {
    extern void* marioGetPtr(void);
    extern s32 marioStGetSystemLevel(void);
    extern s32 marioKeyOffChk(void);
    extern s32 marioCtrlOffChk(void);
    extern s32 N_itemPickUpFromFieldCheck(void);
    extern void fbatHitCheck(s32 flags, s32 hitInfo);
    extern void marioChgMotJump2(void);
    extern void animPoseSetAnim(s32 poseId, void* animName, s32 force);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern f32 angleABf(f64 x0, f64 z0, f64 x1, f64 z1);
    extern f32 reviseAngle(f64 angle);
    extern void* camGetPtr(s32 camId);
    extern f64 atan2(f64 y, f64 x);
    extern void effSmallStarEntry();
    extern void psndSFXOn_3D(char* id, void* pos);
    extern s32 marioChkTalkable(void);
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 marioGetDispDir(void);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern s32 hitCheckFilter(f64 x, f64 y, f64 z, f64 dx, f64 dy, f64 dz, s32 flags,
                              void* outX, void* outY, void* outZ, void* dist,
                              void* outNX, void* outNY, void* outNZ);
    extern void evtStopID(s32 id);
    extern void evtStartID(s32 id);
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
    extern char str_SFX_BTL_FLD_ENEMY_DA_802c140c[];
    extern char str_SFX_BTL_FLD_MARIO_DA_802c1428[];

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
    f32 px;
    f32 py;
    f32 pz;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 nw;
    f32 npcX;
    f32 npcY;
    f32 npcZ;
    f32 width;
    f32 height;
    f32 radius;
    f32 angle;
    f32 dispDir;
    f32 camDir;
    f32 relAngle;
    f32 targetRot;
    f32 delta;
    f32 sinv;
    f32 cosv;
    f32 localPos[3];
    f32 localDir[3];
    f32 localOut[4];

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
            case 0x800000:
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
                                  *(f32*)((s32)npc + 0x94), float_0_8041fc7c, float_neg1_8041fcd8);
                if (npc != 0) {
                    psndSFXOn_3D(str_SFX_BTL_FLD_ENEMY_DA_802c140c, (void*)((s32)npc + 0x8C));
                }
                break;
            case 0x10000000:
                effSmallStarEntry(3, 3, *(f32*)((s32)player + 0x8C),
                                  *(f32*)((s32)player + 0x90) + *(f32*)((s32)player + 0x1BC),
                                  *(f32*)((s32)player + 0x94), float_0_8041fc7c, float_neg1_8041fcd8);
                psndSFXOn_3D(str_SFX_BTL_FLD_MARIO_DA_802c1428, (void*)((s32)player + 0x8C));
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
                px = *(f32*)((s32)player + 0x8C);
                py = *(f32*)((s32)player + 0x90);
                pz = *(f32*)((s32)player + 0x94);
                npcX = *(f32*)((s32)npcEntry + 0x8C);
                npcY = *(f32*)((s32)npcEntry + 0x90);
                npcZ = *(f32*)((s32)npcEntry + 0x94);
                width = *(f32*)((s32)npcEntry + 0x14C);
                height = *(f32*)((s32)npcEntry + 0x150);
                talkable = 0;

                if (npcY <= py + *(f32*)((s32)player + 0xFC) && py <= npcY + height) {
                    localPos[0] = px;
                    localPos[1] = npcY;
                    localPos[2] = pz;
                    dist = PSVECDistance(localPos, (void*)((s32)npcEntry + 0x8C));
                    radius = float_0p5_8041fc80 * (*(f32*)((s32)player + 0xF8) + width) + float_10_8041fc98;
                    if (dist <= radius && dist < bestDist) {
                        nearestNpc = npcEntry;
                    }

                    facing = 0;
                    dispDir = reviseAngle(marioGetDispDir() - *(f32*)((s32)camGetPtr(4) + 0x114));
                    for (hit = 0; hit < 3; hit++) {
                        if (hit == 0) {
                            angle = angleABf(px, pz, npcX, npcZ);
                        } else if (hit == 1) {
                            angle = angleABf(px, pz, npcX + float_0p5_8041fc80 * width, npcZ);
                        } else {
                            angle = angleABf(px, pz, npcX - float_0p5_8041fc80 * width, npcZ);
                        }
                        angle = reviseAngle(angle - *(f32*)((s32)camGetPtr(4) + 0x114));
                        if ((float_180_8041fccc <= angle && float_180_8041fccc <= dispDir) ||
                            (angle < float_180_8041fccc && dispDir <= float_180_8041fccc)) {
                            facing = 1;
                            break;
                        }
                    }

                    if (facing != 0) {
                        localPos[0] = px;
                        localPos[1] = npcY;
                        localPos[2] = pz;
                        dist = PSVECDistance(localPos, (void*)((s32)npcEntry + 0x8C));
                        if (dist <= radius) {
                            angle = angleABf(float_0_8041fc7c, float_0_8041fc7c, npcX - px, npcZ - pz);
                            sincosf(angle, &sinv, &cosv);
                            localOut[0] = 0.0f;
                            localOut[1] = 0.0f;
                            localOut[2] = 0.0f;
                            localOut[3] = dist;
                            hit = hitCheckFilter(px,
                                                 py + float_0p75_8041fcd4 * *(f32*)((s32)player + 0xFC),
                                                 pz,
                                                 sinv, float_0_8041fc7c, cosv, 0,
                                                 &nx, &ny, &nz, &localOut[3],
                                                 &localDir[0], &localDir[1], &localDir[2]);
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

    switch (flags) {
        case 0x20000:
        case 0x40000:
        case 0x80000:
            marioChgMotJump2();
            break;
    }
    effSmallStarEntry();
}

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
    extern f64 __frsqrte(f64 x);

    extern f32 float_0_8041fc7c;
    extern f32 float_0p75_8041fcd4;
    extern f32 float_0p2_8041fcec;
    extern f32 float_10_8041fc98;
    extern f32 float_0p5_8041fc80;
    extern f32 float_180_8041fccc;
    extern f32 float_100_8041fcf0;
    extern f32 float_0p4_8041fcf4;
    extern f32 float_45_8041fcf8;

    u32 flags;
    f32 x;
    f32 y;
    f32 z;
    f32 sinDir;
    f32 cosDir;
    f32 lenSq;
    f32 moveLen;
    f32 dist;
    f32 nyAdd;
    f32 nx;
    f32 ny;
    f32 nz;
    s32 outA;
    s32 outB;
    s32 outC;
    s32 hit;
    f32 dot;
    f32 normalLenSq;
    f32 normalLen;
    f32 slopeAngle;
    f64 d;
    f64 inv;

    flags = *(u32*)npc;
    x = *(f32*)((s32)npc + 0x8C);
    y = *(f32*)((s32)npc + 0x90);
    z = *(f32*)((s32)npc + 0x94);

    *outX = x;
    *outZ = z;

    if (moveX == float_0_8041fc7c && moveZ == float_0_8041fc7c) {
        return 0;
    }

    sincosf(angleABf(float_0_8041fc7c, float_0_8041fc7c, moveX, moveZ), &sinDir, &cosDir);

    lenSq = moveX * moveX + moveZ * moveZ;
    if (lenSq > float_0_8041fc7c) {
        d = (f64)lenSq;
        inv = __frsqrte(d);
        inv = 0.5 * inv * (3.0 - d * inv * inv);
        inv = 0.5 * inv * (3.0 - d * inv * inv);
        inv = 0.5 * inv * (3.0 - d * inv * inv);
        moveLen = (f32)(d * inv);
    } else {
        moveLen = float_0_8041fc7c;
    }

    dist = float_0p75_8041fcd4 * *(f32*)((s32)npc + 0x14C) + moveLen;
    nyAdd = float_0p2_8041fcec * *(f32*)((s32)npc + 0x150);
    if (nyAdd > float_10_8041fc98) {
        nyAdd = float_10_8041fc98;
    }

    if (flags & 0x2000) {
        hit = hitCheckAttr(x, y + nyAdd, z, sinDir, float_0_8041fc7c, cosDir, 4,
                           &outA, &outB, &outC, &dist, &nx, &ny, &nz);
    } else {
        hit = hitCheckFilter(x, y + nyAdd, z, sinDir, float_0_8041fc7c, cosDir, 0,
                             &outA, &outB, &outC, &dist, &nx, &ny, &nz);
    }

    if (hit != 0) {
        dot = moveX * nx + moveZ * nz;
        *outX += float_0p5_8041fc80 * -(dot * nx - moveX);
        *outZ += float_0p5_8041fc80 * -(dot * nz - moveZ);

        if (flags & 0x10000) {
            f32 baseAngle;
            f32 normalAngle;
            f32 delta;
            baseAngle = reviseAngle(float_180_8041fccc + *outAngle);
            normalAngle = reviseAngle(angleABf(float_0_8041fc7c, float_0_8041fc7c,
                                               float_100_8041fcf0 * nx,
                                               float_100_8041fcf0 * nz));
            delta = reviseAngle(normalAngle - baseAngle);
            *outAngle = (f32)(s32)reviseAngle(normalAngle + delta);
        }

        return hit;
    }

    dist = float_0p75_8041fcd4 * *(f32*)((s32)npc + 0x14C) + moveLen;
    nyAdd = float_0p4_8041fcf4 * *(f32*)((s32)npc + 0x150);

    if (flags & 0x2000) {
        hit = hitCheckAttr(x, y + nyAdd, z, sinDir, float_0_8041fc7c, cosDir, 4,
                           &outA, &outB, &outC, &dist, &nx, &ny, &nz);
    } else {
        hit = hitCheckFilter(x, y + nyAdd, z, sinDir, float_0_8041fc7c, cosDir, 0,
                             &outA, &outB, &outC, &dist, &nx, &ny, &nz);
    }

    if (hit != 0) {
        normalLenSq = nx * nx + nz * nz;
        if (normalLenSq > float_0_8041fc7c) {
            d = (f64)normalLenSq;
            inv = __frsqrte(d);
            inv = 0.5 * inv * (3.0 - d * inv * inv);
            inv = 0.5 * inv * (3.0 - d * inv * inv);
            inv = 0.5 * inv * (3.0 - d * inv * inv);
            normalLen = (f32)(d * inv);
        } else {
            normalLen = float_0_8041fc7c;
        }

        slopeAngle = angleABf(float_0_8041fc7c, float_0_8041fc7c, normalLen, -ny);
        if (slopeAngle >= float_45_8041fcf8) {
            dot = moveX * nx + moveZ * nz;
            *outX += float_0p5_8041fc80 * -(dot * nx - moveX);
            *outZ += float_0p5_8041fc80 * -(dot * nz - moveZ);

            if (flags & 0x10000) {
                f32 baseAngle;
                f32 normalAngle;
                f32 delta;
                baseAngle = reviseAngle(float_180_8041fccc + *outAngle);
                normalAngle = reviseAngle(angleABf(float_0_8041fc7c, float_0_8041fc7c,
                                                   float_100_8041fcf0 * nx,
                                                   float_100_8041fcf0 * nz));
                delta = reviseAngle(normalAngle - baseAngle);
                *outAngle = (f32)(s32)reviseAngle(normalAngle + delta);
            }

            return hit;
        }
    }

    dist = float_0p75_8041fcd4 * *(f32*)((s32)npc + 0x14C) + moveLen;
    nyAdd = float_0p75_8041fcd4 * *(f32*)((s32)npc + 0x150);

    if (flags & 0x2000) {
        hit = hitCheckAttr(x, y + nyAdd, z, sinDir, float_0_8041fc7c, cosDir, 4,
                           &outA, &outB, &outC, &dist, &nx, &ny, &nz);
    } else {
        hit = hitCheckFilter(x, y + nyAdd, z, sinDir, float_0_8041fc7c, cosDir, 0,
                             &outA, &outB, &outC, &dist, &nx, &ny, &nz);
    }

    if (hit == 0) {
        *outX += moveX;
        *outZ += moveZ;
        return 0;
    }

    dot = moveX * nx + moveZ * nz;
    *outX += float_0p5_8041fc80 * -(dot * nx - moveX);
    *outZ += float_0p5_8041fc80 * -(dot * nz - moveZ);

    if (flags & 0x10000) {
        f32 baseAngle;
        f32 normalAngle;
        f32 delta;
        baseAngle = reviseAngle(float_180_8041fccc + *outAngle);
        normalAngle = reviseAngle(angleABf(float_0_8041fc7c, float_0_8041fc7c,
                                           float_100_8041fcf0 * nx,
                                           float_100_8041fcf0 * nz));
        delta = reviseAngle(normalAngle - baseAngle);
        *outAngle = (f32)(s32)reviseAngle(normalAngle + delta);
    }

    return hit;
}

void fbatBattleMode(void) {
    extern void* pouchGetPtr(void);
    extern void* marioGetPtr(void);
    extern void evtStopID(s32 id);
    extern s32 pouchEquipCheckBadge(s32 badgeId);
    extern void marioFBattlePrepare(void);
    extern void seqSetSeq(s32 seq, void* map, void* bero);
    extern void unk_JP_US_EU_01_800591b4(void);
    extern void BattleInformationSetResult(void* info, s32 result);
    extern s32 BattleInformationGetResult(void* info);
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    extern void BattleMajinaiDone(void);
    extern void marioKeyOff(void);
    extern void marioKeyOn(void);
    extern s32 evtCheckID(s32 id);
    extern void evtStartID(s32 id);
    extern void animPoseSetLocalTimeRate(s32 poseId, f32 rate);
    extern void marioFBattlePost(void);
    extern void marioChgMoveMotion(void);
    extern void npcGroupDead(void* npc, s32 wKpaScoreType);
    extern void animPoseSetAnim(s32 poseId, void* name, s32 force);
    extern void* itemEntry(void* name, s32 id, u32 mode, s32 wasCollectedExpr, void* pickupScript, f32 x, f32 y, f32 z);
    extern void itemFlagOn(void* itemEntry, u16 bitMask);
    extern u16 marioSetMutekiTime(s32 msec);
    extern s32 sysMsec2Frame(s32 msec);
    extern f32 float_0_8041fc7c;
    extern f32 float_1_8041fc94;
    extern s32 dbg_battleskip;
    extern s32 _evt_majinai_coin_up_effect[];

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
            if (*(s32*)((s32)npc + 0x28C) != 0) hasItems = 1;
            if (*(s32*)((s32)npc + 0x290) != 0) hasItems = 1;
            if (*(s32*)((s32)npc + 0x294) != 0) hasItems = 1;
            if (*(s32*)((s32)npc + 0x298) != 0) hasItems = 1;
            if (*(s32*)((s32)npc + 0x29C) != 0) hasItems = 1;
            if (*(s32*)((s32)npc + 0x2A0) != 0) hasItems = 1;
            if (*(s32*)((s32)npc + 0x2A4) != 0) hasItems = 1;
            if (*(s32*)((s32)npc + 0x2A8) != 0) hasItems = 1;

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
                        item = itemEntry(0, itemId, 0xB, -1, 0, *(f32*)((s32)npc + 0x8C), *(f32*)((s32)npc + 0x90), *(f32*)((s32)npc + 0x94));
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

u8 npcCheckHitMarioSide(s32 pNpc) {
    extern void* marioGetPtr(void);
    extern f32 PSVECDistance(void*, void*);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 compAngle(f32, f32);
    extern s32 marioChkWallAround(void*, s32, f32, f32, f32, f32);
    extern f64 sqrt(f64);
    extern const f32 float_40_8041fd0c;
    extern const f32 float_6p2832_8041fd10;
    extern const f32 float_360_8041fcc0;
    extern const f32 float_2_8041fd34;
    void* mario;
    f32 pos[3];
    f32 dist;
    f32 radius;
    f32 angle;
    f32 diff;
    f32 pushX;
    f32 pushZ;
    f32 len;
    f32 f;

    mario = marioGetPtr();
    if (*(f32*)((s32)mario + 0x90) + *(f32*)((s32)mario + 0xFC) < *(f32*)(pNpc + 0x90)) {
        return 0;
    }
    if (*(f32*)(pNpc + 0x90) + *(f32*)(pNpc + 0x150) < *(f32*)((s32)mario + 0x90)) {
        return 0;
    }
    pos[0] = *(f32*)((s32)mario + 0x8C);
    pos[1] = *(f32*)(pNpc + 0x90);
    pos[2] = *(f32*)((s32)mario + 0x94);
    dist = PSVECDistance(pos, (void*)(pNpc + 0x8C));
    radius = (5.0f + *(f32*)((s32)mario + 0xF8) + *(f32*)(pNpc + 0x14C)) * 0.5f;
    if (dist > radius) {
        return 0;
    }
    *(f32*)(pNpc + 0x8C) = *(f32*)(pNpc + 0x98);
    *(f32*)(pNpc + 0x94) = *(f32*)(pNpc + 0xA0);
    dist = PSVECDistance(pos, (void*)(pNpc + 0x8C));
    if (dist > radius) {
        return 0;
    }
    *(void**)((s32)mario + 0x2A0) = (void*)pNpc;
    angle = angleABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94), *(f32*)(pNpc + 0x8C), *(f32*)(pNpc + 0x94));
    diff = compAngle(angle, *(f32*)((s32)mario + 0x1A4));
    if (diff < 0.0f) {
        diff = -diff;
    }
    if (diff >= float_40_8041fd0c) {
        f = (float_6p2832_8041fd10 * angle) / float_360_8041fcc0;
    } else {
        f = (float_6p2832_8041fd10 * *(f32*)((s32)mario + 0x1A4)) / float_360_8041fcc0;
    }
    pushX = *(f32*)(pNpc + 0x8C) - *(f32*)((s32)mario + 0x8C);
    pushZ = *(f32*)(pNpc + 0x94) - *(f32*)((s32)mario + 0x94);
    len = (f32)sqrt((double)(pushX * pushX + pushZ * pushZ));
    if (len > 0.0f) {
        pushX = pushX * (float_2_8041fd34 / len);
        pushZ = pushZ * (float_2_8041fd34 / len);
    }
    *(f32*)((s32)mario + 0x8C) += pushX;
    *(f32*)((s32)mario + 0x94) += pushZ;
    marioChkWallAround((void*)((s32)mario + 0x8C), 0, 0.0f, 160.0f + f, *(f32*)((s32)mario + 0xF8), 0.0f);
    return 1;
}

u8 _npcHitCheckHammerAllMotion(double radius, double angle, s32 npc, f32* outDist) {
    extern void* marioGetPtr(void);
    extern void sincosf(f32 angle, f32* outSin, f32* outCos);
    extern f64 sqrt(f64);
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 y;
    f32 top;
    s32 i;
    s32 ret;
    void* mario;
    f32 angleStep[12];

    angleStep[0] = 0.0f;
    angleStep[1] = 10.0f;
    angleStep[2] = 20.0f;
    angleStep[3] = 30.0f;
    angleStep[4] = 40.0f;
    angleStep[5] = 50.0f;
    angleStep[6] = 60.0f;
    angleStep[7] = 70.0f;
    angleStep[8] = 80.0f;
    angleStep[9] = 90.0f;
    angleStep[10] = 100.0f;
    angleStep[11] = 110.0f;
    ret = 0;
    dist = 0.0f;
    for (i = 0; i <= 11; i++) {
        mario = marioGetPtr();
        sincosf((f32)angle, &sinA, &cosA);
        sincosf(angleStep[i], &sinB, &cosB);
        y = (f32)((double)*(f32*)((s32)mario + 0x90) + radius * (double)cosB);
        dx = (sinB * (f32)((double)sinA * radius) + *(f32*)((s32)mario + 0x8C)) - *(f32*)(npc + 0x8C);
        dz = (sinB * (f32)((double)cosA * radius) + *(f32*)((s32)mario + 0x94)) - *(f32*)(npc + 0x94);
        dy = y - *(f32*)(npc + 0x90);
        top = *(f32*)(npc + 0x90) + *(f32*)(npc + 0x150);
        if (y > *(f32*)(npc + 0x90) && y > top) {
            dy = y - top;
        }
        dist = (f32)sqrt((double)(dx * dx + dy * dy + dz * dz));
        if (dist < (0.5f * *(f32*)(npc + 0x14C) + 12.0f)) {
            ret = npc;
            break;
        }
    }
    *outDist = dist;
    return ret != 0;
}

u8 _npcGroupDead(u32* param_1, s16 param_2) {
    extern void _npcDead(u32*, s16);
    extern void* fbatGetPointer(void);
    extern void* gpGlobals;
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
                        *(u32*)(fbat + 0x4C) |= 1 << (*(u8*)((s32)child + 0x314) & 0x3F);
                    }
                }
            }
            grand[0x50] |= 0x10;
            grand[0x75] |= 4;
            *(s16*)((s32)grand + 0x31C) = param_2;
            if ((*grand & 4) != 0) {
                fbat = (s32)fbatGetPointer();
                *(u32*)(fbat + 0x4C) |= 1 << (*(u8*)((s32)grand + 0x314) & 0x3F);
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
                    *(u32*)(*(s32*)((s32)gpGlobals + 0x168) + 0x4C) |= 1 << (*(u8*)((s32)grand + 0x314) & 0x3F);
                }
            }
            grand = (u32*)cur[0xCD];
            if (grand != 0) {
                grand[0x50] |= 0x10;
                grand[0x75] |= 4;
                *(s16*)((s32)grand + 0x31C) = param_2;
                if ((*grand & 4) != 0) {
                    *(u32*)(*(s32*)((s32)gpGlobals + 0x168) + 0x4C) |= 1 << (*(u8*)((s32)grand + 0x314) & 0x3F);
                }
            }
        }
        child[0x50] |= 0x10;
        child[0x75] |= 4;
        *(s16*)((s32)child + 0x31C) = param_2;
        if ((*child & 4) != 0) {
            *(u32*)(*(s32*)((s32)gpGlobals + 0x168) + 0x4C) |= 1 << (*(u8*)((s32)child + 0x314) & 0x3F);
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
                *(u32*)(*(s32*)((s32)gpGlobals + 0x168) + 0x4C) |= 1 << (*(u8*)((s32)child + 0x314) & 0x3F);
            }
        }
        child = (u32*)cur[0xCD];
        if (child != 0) {
            child[0x50] |= 0x10;
            child[0x75] |= 4;
            *(s16*)((s32)child + 0x31C) = param_2;
            if ((*child & 4) != 0) {
                *(u32*)(*(s32*)((s32)gpGlobals + 0x168) + 0x4C) |= 1 << (*(u8*)((s32)child + 0x314) & 0x3F);
            }
        }
    }
    param_1[0x50] |= 0x10;
    param_1[0x75] |= 4;
    *(s16*)((s32)param_1 + 0x31C) = param_2;
    if ((*param_1 & 4) != 0) {
        *(u32*)(*(s32*)((s32)gpGlobals + 0x168) + 0x4C) |= 1 << (*(u8*)((s32)param_1 + 0x314) & 0x3F);
    }
    return 0;
}

u8 npcDisp_offscreen(s32 cameraId, void* npc) {
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
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
        *(u8*)((s32)&color + 0) = r >> 1;
        *(u8*)((s32)&color + 1) = g >> 1;
        *(u8*)((s32)&color + 2) = b >> 1;
        color2 = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)npc + 0x104), &color2);
        animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), 0x40);
    }

    disp = dispGetCurWork();
    if (*(u8*)((s32)disp + 1) == 1) {
        animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 2, rot, float_2_8041fd34);
    } else if (*(u8*)((s32)disp + 1) == 0) {
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
    } else if (*(u8*)((s32)disp + 1) < 3) {
        animPoseDrawMtx(*(s32*)((s32)npc + 0x104), mtx, 3, rot, float_2_8041fd34);
    }
    sysWaitDrawSync();
    GXReadBoundingBox(&x0, &y0, &x1, &y1);
    offscreenAddBoundingBox(*(s32*)((s32)npc + 0x110), x0, y0, x1, y1);
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
    extern s32 strcmp(const char*, const char*);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32);
    extern void partyChgPoseId(void*, s32);
    extern void marioChgPose(char*);
    extern void animPoseSetAnim(s32, char*, s32);
    extern char str_x_party_802c1444[];
    extern char str_party_8041fd40[];
    extern char str_extparty_802c1450[];
    extern char str_mario_8041fd48[];
    s32 wp;
    s32 count;
    s32 i;
    void* npc;
    void* party;

    if (*stayPose == 0 || strcmp(stayPose, str_x_party_802c1444) == 0) {
        return;
    }
    if (strcmp(stayPose, str_party_8041fd40) == 0) {
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            partyChgPoseId(party, 1);
        }
        return;
    }
    if (strcmp(stayPose, str_extparty_802c1450) == 0) {
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

void npcSetTalkPose(char* talkPose) {
    extern s32 strcmp(const char*, const char*);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32);
    extern void partyChgPoseId(void*, s32);
    extern void marioChgPose(char*);
    extern void animPoseSetAnim(s32, char*, s32);
    extern char str_x_party_802c1444[];
    extern char str_party_8041fd40[];
    extern char str_extparty_802c1450[];
    extern char str_mario_8041fd48[];
    s32 wp;
    s32 count;
    s32 i;
    void* npc;
    void* party;

    if (*talkPose == 0 || strcmp(talkPose, str_x_party_802c1444) == 0) {
        return;
    }
    if (strcmp(talkPose, str_party_8041fd40) == 0) {
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            partyChgPoseId(party, 7);
        }
        return;
    }
    if (strcmp(talkPose, str_extparty_802c1450) == 0) {
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

u8 npcRecoveryFiledNpc(void) {
    extern void* release_wp;
    extern s32 animPoseEntry(char*, s32);
    extern void animPoseSetAnim(s32, char*, s32);
    extern void* animPoseGetAnimPosePtr(s32);
    extern void animPoseWorldPositionEvalOn(s32);
    extern void animPoseWorldMatrixEvalOn(s32);
    extern void animPoseMain(s32);
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
    extern void* release_wp;
    extern void* animPoseGetAnimPosePtr(s32);
    extern void* animPoseGetAnimBaseDataPtr(s32);
    extern char* animPoseGetCurrentAnim(s32);
    extern void animPoseRelease(s32);
    extern char* strcpy(char*, const char*);
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

void npcSetBattleInfo(void* npc, s32 info) {
    ;
}


u8 fbatSetAttackAnnounce(s32 flag) {
    extern char str_fb_sensei_shita_802c13e8[];
    extern char str_fb_sensei_sareta_802c13f8[];
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
            *(char**)(data + 0x568) = str_fb_sensei_shita_802c13e8;
            *(s32*)(data + 0x558) = 2;
            *(s32*)(data + 0x56C) = dat_8041fc70;
            break;
        case 0x10000000:
            *(char**)(data + 0x568) = str_fb_sensei_sareta_802c13f8;
            *(s32*)(data + 0x558) = 1;
            *(s32*)(data + 0x56C) = dat_8041fc74;
            break;
        default:
            *(s32*)(data + 0x558) = 0;
            break;
    }
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
    extern char* msgSearch(char*);
    extern u32 FontGetMessageWidthLine(char*, s16*);
    extern s32 irand(s32);
    extern void windowDispGX_Waku_col(double, double, double, double, double, u16, u32*);
    extern void FontDrawStart(void);
    extern void FontDrawEdge(void);
    extern void FontDrawColor(u32*);
    extern void FontDrawScale(f32);
    extern void FontDrawStringShake(double, double, char*);
    extern const f32 float_0p1_8041fcdc;
    extern const f32 float_1_8041fc94;
    extern const f32 float_3_8041fce0;
    extern u32 dat_8041fc78;
    char lines[516];
    s16 lineCount[2];
    u32 color;
    u32 fontColor;
    char* msg;
    char* out;
    char* dst;
    s32 i;
    s32 j;
    s32 r0;
    s32 r1;
    u32 width;
    u32 halfWidth;
    u32 originalLines;
    f32 x;
    f32 y;
    char c;

    msg = msgSearch(*(char**)((s32)param_2 + 0x18));
    x = *(f32*)((s32)param_2 + 0x10);
    y = *(f32*)((s32)param_2 + 0x14);
    width = FontGetMessageWidthLine(msg, lineCount);
    lineCount[0]++;
    originalLines = (u32)lineCount[0];
    r0 = irand(10000);
    r1 = irand(10000);
    if (lineCount[0] > 4) {
        lineCount[0] = 4;
    }
    out = lines;
    dst = out;
    for (i = 0; i < lineCount[0]; i++, dst += 0x80) {
        for (j = 0; j < 0x80; j++) {
            c = *msg++;
            if (c == 0 || c == '\n') {
                dst[j] = 0;
                break;
            }
            dst[j] = c;
        }
    }

    halfWidth = (width >> 1) & 0x7FFF;
    color = *(u32*)((s32)param_2 + 0x1C);
    windowDispGX_Waku_col(
        (double)(float_0p1_8041fcdc * (((x + (f32)(r0 % 5)) - 10.0f) - (f32)halfWidth)),
        (double)((y + (f32)(r1 % 5)) * float_0p1_8041fcdc),
        (double)((f32)((width & 0xFFFF) + 0x14) * float_0p1_8041fcdc),
        (double)((f32)(originalLines * 0x1D + 3) * float_0p1_8041fcdc),
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
    for (i = 0; i < lineCount[0]; i++) {
        FontDrawStringShake((double)x, (double)(float_0p1_8041fcdc * ((y + (f32)(-i * 0x1D)) - float_3_8041fce0)), out);
        out += 0x80;
    }
    return 0;
}

u8 npcMoveG(void* pNpc) {
    extern void* hitCheckFilter(double, double, double, double, double, double, s32, void*, void*, void*, void*, void*, void*, void*);
    f32 oldYVel;
    f32 addY;
    f32 walking;
    f32 remain;
    f32 minAdd;
    f32 heightPart;
    f32 hitY;
    f32 hitDist;
    s32 hitTmp[3];
    s32 hitA;
    s32 hitB;
    s32 hitC;
    s32 hitD;
    void* hit;
    u32 jumpFlags;
    u32 dampedJump;

    oldYVel = *(f32*)((s32)pNpc + 0x1B8);
    *(f32*)((s32)pNpc + 0x1B8) = 0.0f;
    jumpFlags = *(u32*)((s32)pNpc + 0x1D4);
    dampedJump = jumpFlags & 0x100000;
    if (dampedJump != 0 && (jumpFlags & 0x10000000) == 0) {
        oldYVel *= 0.5f;
    }

    addY = 0.0f;
    if ((*(u32*)pNpc & 0x20000) != 0) {
        remain = *(f32*)((s32)pNpc + 0x180);
        walking = *(f32*)((s32)pNpc + 0x1A4);
        if (dampedJump == 0 || (jumpFlags & 0x10000000) != 0) {
            addY = *(f32*)((s32)pNpc + 0x1C4) * walking +
                   -980.0f * *(f32*)((s32)pNpc + 0x1CC) * (remain * remain - (remain - walking) * (remain - walking));
        } else {
            addY = 0.4f * *(f32*)((s32)pNpc + 0x1C4) * walking +
                   0.15f * -980.0f * *(f32*)((s32)pNpc + 0x1CC) * (remain * remain - (remain - walking) * (remain - walking));
        }
        addY *= *(f32*)((s32)pNpc + 0x1C8);
        if (*(f32*)((s32)pNpc + 0x1D0) > 0.0f) {
            if (dampedJump == 0) {
                minAdd = -*(f32*)((s32)pNpc + 0x1D0) * walking;
            } else {
                minAdd = 0.5f * -*(f32*)((s32)pNpc + 0x1D0) * walking;
            }
            if (addY < minAdd) {
                addY = minAdd;
            }
        }
    }

    addY += oldYVel;
    if ((*(u32*)pNpc & 0x10) == 0) {
        *(f32*)((s32)pNpc + 0x90) += addY;
        if (*(f32*)((s32)pNpc + 0x90) < -1000.0f) {
            *(f32*)((s32)pNpc + 0x90) = -1000.0f;
        }
    } else if (addY != 0.0f) {
        if (addY > 0.0f) {
            *(u32*)((s32)pNpc + 0x1D4) |= 1;
            *(f32*)((s32)pNpc + 0x90) += addY;
        } else {
            heightPart = 0.8f * *(f32*)((s32)pNpc + 0x150);
            hitDist = heightPart;
            if (addY < 0.0f) {
                hitDist += -addY;
            } else {
                hitDist += addY;
            }
            hit = hitCheckFilter((double)*(f32*)((s32)pNpc + 0x8C),
                                 (double)(*(f32*)((s32)pNpc + 0x90) + heightPart),
                                 (double)*(f32*)((s32)pNpc + 0x94),
                                 0.0, -1.0, 0.0, 0,
                                 hitTmp, &hitY, &hitA, &hitDist, &hitB, &hitC, &hitD);
            *(void**)((s32)pNpc + 0x300) = hit;
            if (hit == 0) {
                hit = hitCheckFilter((double)(0.5f * *(f32*)((s32)pNpc + 0x14C) + *(f32*)((s32)pNpc + 0x8C)),
                                     (double)(*(f32*)((s32)pNpc + 0x90) + heightPart),
                                     (double)*(f32*)((s32)pNpc + 0x94),
                                     0.0, -1.0, 0.0, 0,
                                     hitTmp, &hitY, &hitA, &hitDist, &hitB, &hitC, &hitD);
                *(void**)((s32)pNpc + 0x300) = hit;
                if (hit == 0) {
                    hit = hitCheckFilter((double)(*(f32*)((s32)pNpc + 0x8C) - 0.5f * *(f32*)((s32)pNpc + 0x14C)),
                                         (double)(*(f32*)((s32)pNpc + 0x90) + heightPart),
                                         (double)*(f32*)((s32)pNpc + 0x94),
                                         0.0, -1.0, 0.0, 0,
                                         hitTmp, &hitY, &hitA, &hitDist, &hitB, &hitC, &hitD);
                    *(void**)((s32)pNpc + 0x300) = hit;
                }
            }
            if (hit == 0) {
                *(u32*)((s32)pNpc + 0x1D4) |= 1;
                *(f32*)((s32)pNpc + 0x90) += addY;
            } else {
                *(u32*)((s32)pNpc + 0x1D4) &= ~1;
                *(f32*)((s32)pNpc + 0x90) = hitY;
                *(s32*)((s32)pNpc + 0x17C) = 0;
                *(s32*)((s32)pNpc + 0x178) = 0;
            }
        }
    }
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
