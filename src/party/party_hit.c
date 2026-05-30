#include "party/party_hit.h"

s32 unk_800c27c0(void* flags, void* mario) {
    extern f32 float_6_80421518;
    extern s32 marioChkWallAround(void* mario, s32 arg, f32 a, f32 b, f32 radius);
    f32 a;
    f32 b;
    f32 radius;

    if (*(u32*)flags & 0x1000) {
        radius = float_6_80421518;
    }
    return marioChkWallAround(mario, 1, a, b, radius);
}


u8 partySearchGround(s64 param_1, s64 param_2, s32 pParty) {
    return 0;
}


u8 partyChkFrontStep(s64 param_1, s32 pParty, s32 param_3, s32 param_4, s32 param_5) {
    return 0;
}


u8 partySearchFrontWall(s64 param_1, s64 param_2, s32 pParty, s32 param_4) {
    return 0;
}


u8 partySearchHead(s64 param_1, void* pParty, s32 param_3, s32 param_4) {
    return 0;
}


u8 partyHitCheck(void* pParty, void* param_2, void* param_3, float* param_4, void* param_5, float* param_6) {
    return 0;
}


u8 partyChkGnd(void* pParty) {
    return 0;
}


u8 partyNrmToAngle(float* param_1) {
    return 0;
}


s32 chkfilterYoshi(s32 param_1, int param_2) {
    return 0;
}


s32 chkfilterNokotaro(s32 param_1, int param_2) {
    return 0;
}


s32 chkfilter(s32 param_1, int param_2) {
    return 0;
}


void* partySearchWallFront(s64 param_1, s64 param_2, s32 pParty, void* partyPos) {
    return 0;
}
