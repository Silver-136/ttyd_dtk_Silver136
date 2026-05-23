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
