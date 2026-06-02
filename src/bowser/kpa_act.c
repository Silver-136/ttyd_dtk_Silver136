#include "bowser/kpa_act.h"

s32 kpa_chg_pose(void* evt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern void marioChgPose(s32 pose);

    s32* args = *(s32**)((s32)evt + 0x18);
    marioChgPose(evtGetValue(evt, args[0]));
    return 2;
}

void kpaAddScore(s32 score) {
    extern void pouchAddKpaScore(s32 score);

    if (score > 10000) {
        score = 10000;
    }
    pouchAddKpaScore(score);
}

void kpaAddCoin(s32 coin) {
    extern s32 pouchAddKpaCoin(s32 coin);
    extern void pouchAddKpaScore(s32 score);

    if (pouchAddKpaCoin(coin) != 0) {
        pouchAddKpaScore(1000);
    }
    {
        s32 score = coin * 500;

        if (score > 10000) {
            score = 10000;
        }
        pouchAddKpaScore(score);
    }
}
