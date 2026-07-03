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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyHitCheck(void* pParty, unsigned int* param_2, unsigned int* param_3, float* param_4, float* param_5, float* param_6) {
    typedef struct RawVec {
        unsigned int x;
        unsigned int y;
        unsigned int z;
    } RawVec;
    typedef struct Vec {
        float x;
        float y;
        float z;
    } Vec;
    typedef struct HitWork {
        char unk0[0xC];
        RawVec start;
        RawVec end;
        Vec hitPos;
        Vec hitNormal;
        float dist;
    } HitWork;
    extern int yoshiGetStatus(void);
    extern int hitCheckVecFilter(void* work, void* filter);
    extern int chkfilterYoshi(int param_1, int param_2);
    extern int chkfilterNokotaro(int param_1, int param_2);
    extern int chkfilter(int param_1, int param_2);

    volatile RawVec inputCopy;
    HitWork work;
    float value;
    float bias;
    int result;

    inputCopy.x = param_3[0];
    inputCopy.y = param_3[1];
    inputCopy.z = param_3[2];
    work.start.x = param_2[0];
    work.start.y = param_2[1];
    work.start.z = param_2[2];
    work.end.x = inputCopy.x;
    work.end.y = inputCopy.y;
    work.end.z = inputCopy.z;
    work.dist = *param_6;

    if (yoshiGetStatus() != 0) {
        result = hitCheckVecFilter(&work, chkfilterYoshi);
    } else if (*(signed char*)((int)pParty + 0x31) == 2 && (*(unsigned int*)pParty & 0x100) != 0) {
        result = hitCheckVecFilter(&work, chkfilterNokotaro);
    } else {
        result = hitCheckVecFilter(&work, chkfilter);
    }

    if ((unsigned int)result != 0) {
        value = work.dist;
        bias = -0.5f;
        if (value >= 0.0f) {
            bias = 0.5f;
        }
        *param_6 = (float)(int)(value * 1000.0f + bias) / 1000.0f;

        value = work.hitPos.x;
        bias = -0.5f;
        if (value >= 0.0f) {
            bias = 0.5f;
        }
        param_4[0] = (float)(int)(value * 1000.0f + bias) / 1000.0f;

        value = work.hitPos.y;
        bias = -0.5f;
        if (value >= 0.0f) {
            bias = 0.5f;
        }
        param_4[1] = (float)(int)(value * 1000.0f + bias) / 1000.0f;

        value = work.hitPos.z;
        bias = -0.5f;
        if (value >= 0.0f) {
            bias = 0.5f;
        }
        param_4[2] = (float)(int)(value * 1000.0f + bias) / 1000.0f;

        param_5[0] = work.hitNormal.x;
        param_5[1] = work.hitNormal.y;
        param_5[2] = work.hitNormal.z;
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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


/* CHATGPT FALLBACK MISSING STUBS: main/party/party_hit 20260624_191429 */

/* fallback stub-fill: map=unk_800c2010 addr=0x800c2010 size=0x000007b0 */
int unk_800c2010() {
    return 0;
}
