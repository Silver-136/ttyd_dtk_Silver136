#include "bowser/koopa_motion.h"

void quake_kpaHipAttack(void) {
    extern void* evtEntry(void* script, s32 type, s32 flags);
    extern s32 quake_kpaHipAttack_evt[8];
    evtEntry(quake_kpaHipAttack_evt, 0, 0);
}

void quake_kpaLandOn(void) {
    extern void* marioGetPtr(void);
    extern s32 kpaGetStageType(void);
    extern void psndSFXOn_3D(s32 id, void* pos);
    extern s32 kpaGetLevel(void);
    extern void* evtEntry(void* script, s32 type, s32 flags);
    extern s32 quake_kpaLandOn_evt[8];

    void* mario = marioGetPtr();
    if (kpaGetStageType() != 1) {
        psndSFXOn_3D(0x81D, (void*)((u8*)mario + 0x8C));
        if (kpaGetLevel() >= 2) {
            evtEntry(quake_kpaLandOn_evt, 0, 0);
        }
    }
}

s32 kpaPowDown(void) {
    extern void* marioGetPtr(void);
    extern void kpaCancelAttackMode(void);
    extern s32 kpaGetLevel(void);
    extern s32 sysMsec2Frame(s32 msec);
    extern void marioChgMot(s32 mot);

    void* mario = marioGetPtr();
    void* work;

    kpaCancelAttackMode();
    if (kpaGetLevel() > 0) {
        work = *(void**)((s32)mario + 0x298);
        *(s32*)((s32)work + 4) = *(s32*)((s32)work + 4) - 1;
        *(s16*)((s32)mario + 0x78) = sysMsec2Frame(3000);
        marioChgMot(0x27);
        return 1;
    }
    work = *(void**)((s32)mario + 0x298);
    *(u32*)work |= 0x10000000;
    return 0;
}

s32 kpaPowUp(void) {
    extern void* marioGetPtr(void);
    extern s32 kpaGetLevel(void);
    extern void kpaCancelAttackMode(void);
    extern void marioChgMot(s32 mot);
    extern void kpaMSetMutekiTimer(void);
    extern void kpaMutekiOn(void);

    void* mario = marioGetPtr();
    void* work;

    if (kpaGetLevel() < 3) {
        work = *(void**)((s32)mario + 0x298);
        *(s32*)((s32)work + 4) = *(s32*)((s32)work + 4) + 1;
        kpaCancelAttackMode();
        marioChgMot(0x26);
        return 1;
    }
    kpaMSetMutekiTimer();
    kpaMutekiOn();
    return 0;
}

s32 kpaHipAttackOk(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();

    if (*(u16*)((s32)mario + 0x2E) == 0x10) {
        return 0;
    }
    return ((*(u32*)*(void**)((s32)mario + 0x298) >> 4) & 1) ^ 1;
}
