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
    s32 addr = (s32)npc + index * 4;
    return *(void**)(addr + 0x2CC);
}

void npcSetBtlSetupWork(void* npc, s32 index, void* value) {
    s32 addr = (s32)npc + index * 4;
    *(void**)(addr + 0x2CC) = value;
}

void npcSetSlave(void* master, void* slave, s32 index) {
    s32 addr;
    *(void**)((s32)slave + 0x32C) = master;
    addr = (s32)master + index * 4;
    *(void**)(addr + 0x330) = slave;
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


u8 npcDisp_blur(void) {
    return 0;
}


u8 npcDisp_offscreen_xlu(void) {
    return 0;
}


u8 npcDisp(void) {
    return 0;
}


u8 mtx_setup(void) {
    return 0;
}


void _npcDeleteGroup(void* npc) {
}


u8 npcDisp_xlu(void) {
    return 0;
}


u8 fbatTalkMode(void) {
    return 0;
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
}


void npcSetTalkPose(char* talkPose) {
}


u8 npcRecoveryFiledNpc(void) {
    return 0;
}


void npcBlurOn(char* name) {
}


u8 npcReleaseFiledNpc(void) {
    return 0;
}


void npcSetBattleInfo(void* npc, s32 info) {
}


u8 fbatSetAttackAnnounce(s32 param_1) {
    return 0;
}


s32 npcWaitAllInitEvtEnd(void) {
    return 0;
}


void npcExecAllInitEvt(void) {
}


void npcExecAllReglEvt(void) {
}


s32 _majinai_effect(void* pEvt, int param_2) {
    return 0;
}


void npcStartForEvent(void) {
}


void* npcNameToPtr_NoAssert(s32 name) {
    return 0;
}


void npcStopForEvent(void) {
}


void npcBlurOff(char* name) {
}


void npcGetBackItemEntry(void* npc) {
}


void* npcNameToPtr(s32 name) {
    return 0;
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
}


void npcDelete(void* npc) {
}


void npcSetColor(char* name, void* color) {
}


s32 npcEntry(char* name, char* modelName) {
    return 0;
}


s32 npcCalcScore(void* npc) {
    return 0;
}


s32 npcKoopaModeMobjBoundDeadCheck(void* hit) {
    return 0;
}


void npcGroupDead(void* npc, s32 wKpaScoreType) {
}


s32 npcCheckInterrupt(void* pNpc) {
    return 0;
}


s32 npcCheckBlow(void* npc) {
    return 0;
}
