#include "mario/mario_hit.h"

s32 chkFilterAttr2(void* a, void* b) {
    u32 attr = *(u32*)((s32)b + 0x4);
    return ((attr & *(u32*)((s32)a + 0x4)) == 0 || (attr & *(u32*)((s32)a + 0x8)) != 0);
}

void clear_hitobj_ride(void) {
    extern void* marioGetPtr(void);
    extern void kpaClearHitobjRide(void);

    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x1E8) = 0;
    if ((s32)*(u8*)((s32)mario + 0x3C) == 2) {
        kpaClearHitobjRide();
    }
}


u8 marioChkGnd(void) {
    return 0;
}


u8 marioSearchGround(void) {
    return 0;
}


s32 marioChkWallAround(void* mario, s32 arg, f32 a, f32 b, f32 radius) {
    return 0;
}


u8 searchFrontWall(s64 speed, s64 angle, s64 param_3, void* position, s32 param_5) {
    return 0;
}


u8 marioHitCheck2(void) {
    return 0;
}


u8 marioSearchGroundRoll(void) {
    return 0;
}


u8 marioLandonChk_old(void) {
    return 0;
}


u8 marioChkLandon2(void) {
    return 0;
}


u8 marioChkFrontStep(void) {
    return 0;
}


u8 marioSearchHead(void) {
    return 0;
}


u8 marioSlitChkWallAround(void) {
    return 0;
}


u8 marioCheckWall_roll(void) {
    return 0;
}


u8 marioSearchHead_roll(void) {
    return 0;
}


u8 marioChkOverhead(void) {
    return 0;
}


u8 dou10_yoko_yari(void) {
    return 0;
}


int dou10_yoko_yari2(void* param_1) {
    return 0;
}


u8 marioChkFront(void) {
    return 0;
}


u8 marioHitCheck(void) {
    return 0;
}


u8 marioCheckWallShip(void) {
    return 0;
}


u8 marioSearchFrontWall_staying(void) {
    return 0;
}


u8 marioCheckWall(s64 speed, s64 angle) {
    return 0;
}


int marioHitCheckVec(void* param_1, void* param_2, float* param_3, void* param_4, float* param_5) {
    return 0;
}


u8 searchUnder2(double param_1, double param_2, double param_3, float* param_4) {
    return 0;
}


u8 marioSearchUnder(void) {
    return 0;
}


u8 N_dou10_yoko_yari3(void) {
    return 0;
}


u8 marioSearchHead_jabara(void) {
    return 0;
}


double marioChkOverheadJabara(void* param_1, float* param_2) {
    return 0.0;
}


u8 marioChkGnd2(void) {
    return 0;
}


u8 marioChkToge(void) {
    return 0;
}


s32 chkfilterVecRoll(s32 param_1, int param_2) {
    return 0;
}


s32 chkfilterVec(s32 param_1, int param_2) {
    return 0;
}


s32 chkfilterVecVivian(s32 param_1, int param_2) {
    return 0;
}


u8 marioHitCheckWidth(void) {
    return 0;
}


u8 marioChkLandon(float velocityY, s32 param_2) {
    return 0;
}


u32 marioChkBero(int param_1) {
    return 0;
}
