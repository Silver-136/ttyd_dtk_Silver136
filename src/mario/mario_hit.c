#include "mario/mario_hit.h"
#include "bowser/koopa_hit.h"
#include "bowser/koopa_motion.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "party/party_vivian.h"

s32 chkfilterVec(s32 param_1, int param_2);
s32 chkfilterVecRoll(s32 param_1, int param_2);
s32 chkfilterVecVivian(s32 param_1, int param_2);
extern void marioChgMot(s32 motion);
extern void marioSetFallPara(void);
extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
extern f64 __fabs(f64 value);
extern f64 angleABf(f64 x1, f64 y1, f64 x2, f64 y2);
extern f64 sin(f64 angle);
extern f64 cos(f64 angle);
extern f32 toMovedir(f64 angle);
extern f64 sqrt(f64 value);
extern s32 strcmp(const char* a, const char* b);
extern void marioSearchFrontWall_staying(f64);
extern f64 revise360(f64 angle);
extern s32 jabaraNoHitChk(void);

void marioHitCheckWidth(void) {
    extern f32 float_18_80420960;
    extern f32 float_1_804208c4;
    extern f32 float_6_8042099c;
    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;
    f32 target;
    f32 width;

    if (!(flags & 0x100000)) {
        if (flags & 0x1000000) {
            target = float_18_80420960;
        } else {
            target = *(f32*)((s32)mario + 0x1B8);
        }
        width = *(f32*)((s32)mario + 0x1C0) + float_1_804208c4;
        *(f32*)((s32)mario + 0x1C0) = width;
        if (width > target) {
            *(f32*)((s32)mario + 0x1C0) = target;
        }
    } else {
        width = *(f32*)((s32)mario + 0x1C0);
        if (width > float_6_8042099c) {
            width -= float_1_804208c4;
            *(f32*)((s32)mario + 0x1C0) = width;
            if (width < float_6_8042099c) {
                *(f32*)((s32)mario + 0x1C0) = float_6_8042099c;
            }
        }
        width = *(f32*)((s32)mario + 0x1C0);
        if (width < float_6_8042099c) {
            width += float_1_804208c4;
            *(f32*)((s32)mario + 0x1C0) = width;
            if (width > float_6_8042099c) {
                *(f32*)((s32)mario + 0x1C0) = float_6_8042099c;
            }
        }
    }
}

s32 chkFilterAttr2(void* a, void* b) {
    u32 attr = *(u32*)((s32)b + 0x4);
    return ((attr & *(u32*)((s32)a + 0x4)) == 0 || (attr & *(u32*)((s32)a + 0x8)) != 0);
}

s32 chkfilterVec(s32 param_1, int param_2) {
    typedef struct MobjEntry {
        u8 pad[0x15];
        char mAnimPoseName[16];
    } MobjEntry;
    extern MobjEntry* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, char* b);
    extern char str_MOBJ_HiddenHatenaBlo_802c3dec[];
    extern char str_MOBJ_HiddenBadgeBloc_802c3e04[];
    extern char str_MOBJ_Hidden10CountBl_802c3e1c[];
    u32 attr;
    MobjEntry* mobj;

    attr = *(u32*)(param_2 + 4);
    if ((attr & 0x80080) != 0) {
        return 1;
    }
    if ((attr & 0x800000) == 0 && (attr & 4) != 0) {
        return 0;
    }
    if ((attr & 0x80000000) == 0) {
        return 1;
    }
    mobj = mobjHitObjPtrToPtr(param_2);
    if (strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenHatenaBlo_802c3dec) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenBadgeBloc_802c3e04) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_Hidden10CountBl_802c3e1c) == 0) {
        mobjHitObjPtrToPtr(param_2);
        return 0;
    }
    return 1;
}

s32 chkfilterVecRoll(s32 param_1, int param_2) {
    typedef struct MobjEntry {
        u8 pad[0x15];
        char mAnimPoseName[16];
    } MobjEntry;
    extern MobjEntry* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, char* b);
    extern char str_MOBJ_HiddenHatenaBlo_802c3dec[];
    extern char str_MOBJ_HiddenBadgeBloc_802c3e04[];
    extern char str_MOBJ_Hidden10CountBl_802c3e1c[];
    u32 attr;
    MobjEntry* mobj;

    attr = *(u32*)(param_2 + 4);
    if ((attr & 0x80) != 0) {
        return 1;
    }
    if ((attr & 0x800000) == 0 && (attr & 0x80004) != 0) {
        return 0;
    }
    if ((attr & 0x80000000) == 0) {
        return 1;
    }
    mobj = mobjHitObjPtrToPtr(param_2);
    if (strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenHatenaBlo_802c3dec) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenBadgeBloc_802c3e04) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_Hidden10CountBl_802c3e1c) == 0) {
        mobjHitObjPtrToPtr(param_2);
        return 0;
    }
    return 1;
}

s32 chkfilterVecVivian(s32 param_1, int param_2) {
    typedef struct MobjEntry {
        u8 pad[0x15];
        char mAnimPoseName[16];
    } MobjEntry;
    extern MobjEntry* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, char* b);
    extern char str_MOBJ_HiddenHatenaBlo_802c3dec[];
    extern char str_MOBJ_HiddenBadgeBloc_802c3e04[];
    extern char str_MOBJ_Hidden10CountBl_802c3e1c[];
    u32 attr;
    MobjEntry* mobj;

    attr = *(u32*)(param_2 + 4);
    if ((attr & 0x80804) != 0 && (attr & 0x800000) == 0) {
        return 0;
    }
    if ((attr & 0x80000000) == 0) {
        return 1;
    }
    mobj = mobjHitObjPtrToPtr(param_2);
    if (strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenHatenaBlo_802c3dec) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenBadgeBloc_802c3e04) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_Hidden10CountBl_802c3e1c) == 0) {
        mobjHitObjPtrToPtr(param_2);
        return 0;
    }
    return 1;
}

int marioHitCheckVec(void* param_1, void* param_2, float* param_3, void* param_4, float* param_5) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_1000_804208a8;

    HitCheckArg hit;
    s32 result;

    hit.start = *(Vec*)param_1;
    hit.end = *(Vec*)param_2;
    hit.radius = *param_5;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        f32 radius = *param_5;
        *param_5 = (f32)(s32)(radius * float_1000_804208a8 + (radius >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        if (param_3 != 0) {
            f32 x;
            f32 y;
            f32 z;

            *(Vec*)param_3 = hit.normal;
            x = param_3[0];
            param_3[0] = (f32)(s32)(x * float_1000_804208a8 + (x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
            y = param_3[1];
            param_3[1] = (f32)(s32)(y * float_1000_804208a8 + (y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
            z = param_3[2];
            param_3[2] = (f32)(s32)(z * float_1000_804208a8 + (z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        }
        if (param_4 != 0) {
            *(Vec*)param_4 = hit.hitPos;
        }
    }
    return result;
}

s32 marioHitCheck(f64 x0, f64 y0, f64 z0, f64 x1, f64 y1, f64 z1, f32* outNx, f32* outNy, f32* outNz,
                  f32* radius, f32* outHx, f32* outHy, f32* outHz) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern Vec vec3_802c3b78;
    extern Vec vec3_802c3b84;
    extern f32 float_0_804208ac;
    extern f32 float_1000_804208a8;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;

    HitCheckArg hit;
    Vec roundedHit;
    f32 r;
    f32 roundBias;
    s32 result;

    hit.start = vec3_802c3b78;
    hit.end = vec3_802c3b84;
    hit.start.x = (f32)x0;
    hit.start.y = (f32)y0;
    hit.start.z = (f32)z0;
    hit.end.x = (f32)x1;
    hit.end.y = (f32)y1;
    hit.end.z = (f32)z1;
    r = *radius;
    hit.radius = r;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if (*(u32*)marioGetPtr() & 0x01000000) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        roundBias = float_neg0p5_804208d0;
        if (r >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        r = (f32)(s32)(r * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.x >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.x = (f32)(s32)(hit.normal.x * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.y >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.y = (f32)(s32)(hit.normal.y * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.z >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.z = (f32)(s32)(hit.normal.z * float_1000_804208a8 + roundBias) / float_1000_804208a8;
        roundedHit = hit.hitPos;
    }

    *radius = r;
    *outNx = hit.normal.x;
    *outNy = hit.normal.y;
    *outNz = hit.normal.z;
    *outHx = roundedHit.x;
    *outHy = roundedHit.y;
    *outHz = roundedHit.z;
    return result;
}

s32 marioHitCheck2(void* start, void* end, f32* outNormal, void* outHitPos, f32* radius) {
    typedef struct Vec {
        f32 x, y, z;
    } Vec;
    typedef struct HitCheckArg {
        u32 flags;
        u32 attrMask;
        u32 pad;
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern s32 marioGetJabaraState(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkFilterAttr2(s32, s32);
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_1000_804208a8;

    void* player = marioGetPtr();
    f32 value = *radius;
    s32 result;

#define ROUND_1000(v) ((f32)(s32)((v) * float_1000_804208a8 + \
        (((v) >= float_0_804208ac) ? float_0p5_804208cc : float_neg0p5_804208d0)) / \
        float_1000_804208a8)

#define FINISH_HIT(arg_) do { \
        value = (arg_).radius; \
        value = ROUND_1000(value); \
        if (outNormal != 0) { \
            *(Vec*)outNormal = (arg_).normal; \
            outNormal[0] = ROUND_1000(outNormal[0]); \
            outNormal[1] = ROUND_1000(outNormal[1]); \
            outNormal[2] = ROUND_1000(outNormal[2]); \
        } \
        if (outHitPos != 0) { \
            *(Vec*)outHitPos = (arg_).hitPos; \
        } \
    } while (0)

    if (*(u16*)((s32)player + 0x2E) == 0x14) {
        goto jabara_motion;
    }

    if ((*(u32*)player & 0x00020000) == 0) {
        HitCheckArg hit;

        hit.start = *(Vec*)start;
        hit.end = *(Vec*)end;
        hit.flags = 4;
        hit.attrMask = 0x800000;
        hit.radius = value;

        result = hitCheckVecFilter(&hit, chkFilterAttr2);
        if (result != 0) {
            FINISH_HIT(hit);
        }
        *radius = value;
        return result;
    } else {
        HitCheckArg hit;

        hit.start = *(Vec*)start;
        hit.end = *(Vec*)end;
        hit.radius = value;

        player = marioGetPtr();
        if (*(u16*)((s32)player + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else {
            player = marioGetPtr();
            if ((*(u32*)player & 0x01000000) == 0) {
                result = hitCheckVecFilter(&hit, chkfilterVec);
            } else {
                result = hitCheckVecFilter(&hit, chkfilterVecRoll);
            }
        }

        if (result != 0) {
            FINISH_HIT(hit);
        }
        *radius = value;
        return result;
    }

jabara_motion:
    if (marioGetJabaraState() == 1) {
        HitCheckArg hit;

        hit.start = *(Vec*)start;
        hit.end = *(Vec*)end;
        hit.flags = 4;
        hit.attrMask = 0x800000;
        hit.radius = value;

        result = hitCheckVecFilter(&hit, chkFilterAttr2);
        if (result != 0) {
            FINISH_HIT(hit);
        }
        *radius = value;
        return result;
    } else {
        HitCheckArg hit;

        hit.start = *(Vec*)start;
        hit.end = *(Vec*)end;
        hit.radius = value;

        player = marioGetPtr();
        if (*(u16*)((s32)player + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else {
            player = marioGetPtr();
            if ((*(u32*)player & 0x01000000) == 0) {
                result = hitCheckVecFilter(&hit, chkfilterVec);
            } else {
                result = hitCheckVecFilter(&hit, chkfilterVecRoll);
            }
        }

        if (result != 0) {
            FINISH_HIT(hit);
        }
        *radius = value;
        return result;
    }

#undef FINISH_HIT
#undef ROUND_1000
}

void clear_hitobj_ride(void) {

    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x1E8) = 0;
    if ((s32)*(u8*)((s32)mario + 0x3C) == 2) {
        kpaClearHitobjRide();
    }
}

u32 marioChkBero(int param_1) {
    void* mario;
    u32 attr;

    attr = hitGetAttr((void*)param_1);
    mario = marioGetPtr();
    if ((attr & 0x80000) != 0 && (*(u32*)mario & 0x1000000) == 0) {
        return 0;
    }
    if ((attr & 0x80) == 0) {
        return (attr >> 23) & 1;
    }
    return *(u16*)((s32)mario + 0x2E) == 0x19;
}


u8 marioSearchUnder(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern Vec vec3_802c3b9c;
    extern f32 float_0p5_804208cc;
    extern f32 float_1000_804208a8;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg3000_80420980;

    void* mario = marioGetPtr();
    f32 height = *(f32*)((s32)mario + 0x1BC);
    HitCheckArg hit;
    Vec pos;
    s32 result;

    if ((*(u32*)mario & 0x01000000) != 0) {
        height *= float_0p5_804208cc;
    }

    pos.x = *(f32*)((s32)mario + 0x8C);
    pos.y = *(f32*)((s32)mario + 0x90) + height;
    pos.z = *(f32*)((s32)mario + 0x94);
    hit.start = pos;
    hit.end = vec3_802c3b9c;
    hit.radius = float_1000_804208a8;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        f32 x = hit.normal.x;
        f32 y = hit.normal.y;
        f32 z = hit.normal.z;

        hit.normal.x = (f32)(s32)(x * float_1000_804208a8 + (x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        hit.normal.y = (f32)(s32)(y * float_1000_804208a8 + (y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        hit.normal.z = (f32)(s32)(z * float_1000_804208a8 + (z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
    }

    *(f32*)((s32)mario + 0x1C4) = result != 0 ? hit.normal.y : float_neg3000_80420980;
    return result;
}

u8 searchUnder2(double param_1, double param_2, double param_3, float* param_4) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern Vec vec3_802c3ba8;
    extern Vec vec3_802c3bb4;
    extern f32 float_37_8042097c;
    extern f32 float_1037_80420998;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_1000_804208a8;
    extern f32 float_neg1_804208e4;

    Vec pos = vec3_802c3ba8;
    HitCheckArg hit;
    s32 result;

    pos.x = (f32)param_1;
    pos.y = (f32)param_2;
    pos.z = (f32)param_3;
    pos.y += float_37_8042097c;
    hit.start = pos;
    hit.end = vec3_802c3bb4;
    hit.radius = float_1037_80420998;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        f32 x = hit.normal.x;
        f32 y = hit.normal.y;
        f32 z = hit.normal.z;

        hit.normal.x = (f32)(s32)(x * float_1000_804208a8 + (x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        hit.normal.y = (f32)(s32)(y * float_1000_804208a8 + (y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        hit.normal.z = (f32)(s32)(z * float_1000_804208a8 + (z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
    }

    if (result != 0) {
        *param_4 = hit.normal.y;
    } else {
        *param_4 = float_neg1_804208e4;
    }
    return result;
}


void marioChkGnd(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern Vec vec3_802c3b78;
    extern void* marioSearchGround(f64, f64, f32*, f32*, f32*);
    extern void* marioSearchGroundRoll(f64, f64, f32*, f32*, f32*);
    extern void* kpaSearchGround(f64, f32*, f32*, f32*);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 marioChkFrontStep(f32*, void*, void*);
    extern void marioChgMotSub(s32 motion, s32 param);
    extern s32 marioChkShipMoveMode(void);
    extern void marioShipChgFall(void);
    extern void roll_upstairs_on(void);
    extern void set_damage_root_ypos(f64 y);
    extern void* kpaChkHitobjRide(void);
    extern f64 __frsqrte(f64);
    extern f32 __float_nan;
    extern f32 roll_grx;
    extern f32 roll_grz;
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_5_804208c0;
    extern f32 float_10_80420904;
    extern f32 float_11_804208b4;
    extern f32 float_30_80420928;
    extern f32 float_37_8042097c;
    extern f32 float_47_8042095c;
    extern f32 float_100_80420900;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;
    extern f32 float_5000_80420984;
    extern f32 float_neg0p4_80420988;
    extern f32 float_neg0p06_8042098c;
    extern f32 float_neg0p04_80420990;
    extern f32 float_neg0p02_80420994;
    extern f32 float_neg3000_80420980;

    u8* vecBase;
    void* player;
    void* probePlayer;
    void* hit;
    void* underHit;

    HitCheckArg underVivian;
    HitCheckArg underNormal;
    HitCheckArg tubeHit;

    Vec underStartVivian;
    Vec underNormalVivian;
    Vec underHitPosVivian;
    Vec underStartNormal;
    Vec underRoundedNormal;
    Vec underHitPosNormal;
    Vec tubeStart;
    Vec tubeRoundedNormal;
    Vec tubeHitPos;

    f32 groundY;
    f32 drop;
    f32 angle;
    f32 stepY;
    f32 collisionHeight;
    f32 roundBias;
    f32 tubeAngleX;
    f32 tubeAngleZ;
    f32 tubeLengthSq;
    f32 tubeFlatLength;
    f32 tubeRadius;
    u8 stepA[4];
    u8 stepB[4];
    s32 tubeResult;

    vecBase = (u8*)&vec3_802c3b78;
    player = marioGetPtr();

    if (vivianGetStatus() == 2 ||
        vivianGetStatus() == 1 ||
        vivianGetStatus() == 3) {
        hit = marioSearchGround(
            float_37_8042097c,
            float_0_804208ac,
            &groundY,
            &drop,
            &angle);
        *(void**)((s32)player + 0x1E8) = hit;

        /*
         * Target owns a complete first under-foot HitCheckArg and a separate
         * temporary Vec family for this Vivian-status path.
         */
        probePlayer = marioGetPtr();
        collisionHeight = *(f32*)((s32)probePlayer + 0x1BC);
        if ((*(u32*)probePlayer & 0x01000000) != 0) {
            collisionHeight *= float_0p5_804208cc;
        }

        underStartVivian.x = *(f32*)((s32)probePlayer + 0x8C);
        underStartVivian.y =
            *(f32*)((s32)probePlayer + 0x90) + collisionHeight;
        underStartVivian.z = *(f32*)((s32)probePlayer + 0x94);

        underVivian.start = underStartVivian;
        underVivian.end = *(Vec*)(vecBase + 0x24);
        underVivian.radius = float_1000_804208a8;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            underHit =
                (void*)hitCheckVecFilter(&underVivian, chkfilterVecVivian);
        } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
            underHit =
                (void*)hitCheckVecFilter(&underVivian, chkfilterVecRoll);
        } else {
            underHit =
                (void*)hitCheckVecFilter(&underVivian, chkfilterVec);
        }

        if (underHit != 0) {
            underNormalVivian = underVivian.normal;

            roundBias =
                underNormalVivian.x >= float_0_804208ac
                    ? float_0p5_804208cc
                    : float_neg0p5_804208d0;
            underNormalVivian.x =
                (f32)(s32)(float_1000_804208a8 * underNormalVivian.x +
                           roundBias) /
                float_1000_804208a8;

            roundBias =
                underNormalVivian.y >= float_0_804208ac
                    ? float_0p5_804208cc
                    : float_neg0p5_804208d0;
            underNormalVivian.y =
                (f32)(s32)(float_1000_804208a8 *
                               underNormalVivian.y +
                           roundBias) /
                float_1000_804208a8;

            roundBias =
                underNormalVivian.z >= float_0_804208ac
                    ? float_0p5_804208cc
                    : float_neg0p5_804208d0;
            underNormalVivian.z =
                (f32)(s32)(float_1000_804208a8 *
                               underNormalVivian.z +
                           roundBias) /
                float_1000_804208a8;

            underHitPosVivian = underVivian.hitPos;
        }

        if (underHit != 0) {
            *(f32*)((s32)probePlayer + 0x1C4) =
                underNormalVivian.y;
        } else {
            *(f32*)((s32)probePlayer + 0x1C4) =
                float_neg3000_80420980;
        }
        *(void**)((s32)player + 0x1FC) = underHit;
        return;
    }

    *(void**)((s32)player + 0x1E8) = 0;
    if (*(s8*)((s32)player + 0x3C) == 2) {
        kpaClearHitobjRide();
    }

    /*
     * The ordinary path has a second, distinct under-foot HitCheckArg in the
     * target.  Keep it separate from the Vivian-status copy above.
     */
    probePlayer = marioGetPtr();
    collisionHeight = *(f32*)((s32)probePlayer + 0x1BC);
    if ((*(u32*)probePlayer & 0x01000000) != 0) {
        collisionHeight *= float_0p5_804208cc;
    }

    underStartNormal.x = *(f32*)((s32)probePlayer + 0x8C);
    underStartNormal.y =
        *(f32*)((s32)probePlayer + 0x90) + collisionHeight;
    underStartNormal.z = *(f32*)((s32)probePlayer + 0x94);

    underNormal.start = underStartNormal;
    underNormal.end = *(Vec*)(vecBase + 0x24);
    underNormal.radius = float_1000_804208a8;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        underHit =
            (void*)hitCheckVecFilter(&underNormal, chkfilterVecVivian);
    } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
        underHit =
            (void*)hitCheckVecFilter(&underNormal, chkfilterVecRoll);
    } else {
        underHit =
            (void*)hitCheckVecFilter(&underNormal, chkfilterVec);
    }

    if (underHit != 0) {
        underRoundedNormal = underNormal.normal;

        roundBias =
            underRoundedNormal.x >= float_0_804208ac
                ? float_0p5_804208cc
                : float_neg0p5_804208d0;
        underRoundedNormal.x =
            (f32)(s32)(float_1000_804208a8 *
                           underRoundedNormal.x +
                       roundBias) /
            float_1000_804208a8;

        roundBias =
            underRoundedNormal.y >= float_0_804208ac
                ? float_0p5_804208cc
                : float_neg0p5_804208d0;
        underRoundedNormal.y =
            (f32)(s32)(float_1000_804208a8 *
                           underRoundedNormal.y +
                       roundBias) /
            float_1000_804208a8;

        roundBias =
            underRoundedNormal.z >= float_0_804208ac
                ? float_0p5_804208cc
                : float_neg0p5_804208d0;
        underRoundedNormal.z =
            (f32)(s32)(float_1000_804208a8 *
                           underRoundedNormal.z +
                       roundBias) /
            float_1000_804208a8;

        underHitPosNormal = underNormal.hitPos;
    }

    if (underHit != 0) {
        *(f32*)((s32)probePlayer + 0x1C4) =
            underRoundedNormal.y;
    } else {
        *(f32*)((s32)probePlayer + 0x1C4) =
            float_neg3000_80420980;
    }
    *(void**)((s32)player + 0x1FC) = underHit;

    /* Target exits immediately for jumping/falling, disabled floors, or y>=5000. */
    if ((*(u32*)player & 0x00060000) != 0) {
        return;
    }

    probePlayer = marioGetPtr();
    if ((*(u32*)probePlayer & 0x100) != 0) {
        return;
    }
    if (*(f32*)((s32)probePlayer + 0x90) >= float_5000_80420984) {
        return;
    }

    /*
     * 0x200 is ForceFall.  Tube mode is chosen before the Bowser/normal search
     * split in the target.
     */
    if ((*(u32*)player & 0x200) == 0) {
        if ((*(u32*)player & 0x01000000) != 0) {
            hit = marioSearchGroundRoll(
                float_11_804208b4,
                *(f32*)((s32)player + 0x7C),
                &groundY,
                &drop,
                &angle);
        } else if (*(s8*)((s32)player + 0x3C) == 2) {
            hit = kpaSearchGround(
                float_11_804208b4,
                &groundY,
                &drop,
                &angle);
        } else {
            hit = marioSearchGround(
                float_11_804208b4,
                *(f32*)((s32)player + 0x7C),
                &groundY,
                &drop,
                &angle);
        }

        if (hit != 0) {
            *(void**)((s32)player + 0x1E8) = hit;
            *(f32*)((s32)player + 0x1C4) =
                (f32)(s32)(float_100_80420900 * groundY +
                           float_0p5_804208cc) /
                float_100_80420900;
        }
    } else {
        groundY = *(f32*)((s32)player + 0x90);

        if (*(s8*)((s32)player + 0x3C) == 2) {
            if (*(s16*)((s32)player + 0x50) == 0) {
                marioChgMot(0xA);
                marioSetFallPara();
                return;
            }
        } else if (*(s16*)((s32)player + 0x50) == 0) {
            marioChgMot(0xA);
            marioSetFallPara();
            return;
        }
    }

    if (*(u16*)((s32)player + 0x2E) == 0x19) {
        if (marioChkShipMoveMode() != 0) {
            if (*(void**)((s32)player + 0x1E8) == 0) {
                marioShipChgFall();
            } else {
                *(u32*)player &= ~0x40000;
            }
        }
        return;
    }

    if (*(u16*)((s32)player + 0x2E) != 0x1F &&
        *(void**)((s32)player + 0x1E8) != 0 &&
        (hitGetAttr(*(void**)((s32)player + 0x1E8)) & 0x200) != 0) {
        if (*(f32*)((s32)player + 0x7C) >= float_0_804208ac) {
            marioSetFallPara();
        }
        set_damage_root_ypos((f64)*(f32*)((s32)player + 0x90));
        marioChgMot(0x1F);
        return;
    }

    if (*(void**)((s32)player + 0x1E8) != 0 &&
        (hitGetAttr(*(void**)((s32)player + 0x1E8)) & 0x800) != 0) {
        if (vivianGetStatus() != 2 &&
            vivianGetStatus() != 1 &&
            vivianGetStatus() != 3) {
            set_damage_root_ypos((f64)*(f32*)((s32)player + 0x90));
            marioChgMot(0x20);
        }
        return;
    }

    /*
     * Target duplicates the step test around TubeMode rather than sharing one
     * call and selecting the action afterward.
     */
    if (*(s16*)((s32)player + 0x50) == 0) {
        if ((*(u32*)player & 0x01000000) == 0) {
            if (marioChkFrontStep(&stepY, stepA, stepB) != 0) {
                f32 rise;

                rise = stepY - *(f32*)((s32)player + 0x90);
                if (rise <= float_11_804208b4 &&
                    rise >= float_5_804208c0 &&
                    *(f32*)((s32)player + 0x218) <= float_30_80420928) {
                    *(f32*)((s32)player + 0x1C8) = stepY;
                    if (*(f32*)((s32)player + 0x180) !=
                        float_0_804208ac) {
                        if ((*(u32*)((s32)player + 4) &
                             0x01000000) == 0) {
                            marioChgMot(0xC);
                        } else {
                            marioChgMotSub(0xC, 0);
                        }
                        return;
                    }
                }
            }
        } else {
            if (marioChkFrontStep(&stepY, stepA, stepB) != 0) {
                f32 rise;

                rise = stepY - *(f32*)((s32)player + 0x90);
                if (rise <= float_11_804208b4 &&
                    rise >= float_5_804208c0 &&
                    *(f32*)((s32)player + 0x218) <= float_30_80420928) {
                    *(f32*)((s32)player + 0x1C8) = stepY;
                    if (*(f32*)((s32)player + 0x180) !=
                        float_0_804208ac) {
                        roll_upstairs_on();
                        return;
                    }
                }
            }
        }
    }

    *(f32*)((s32)player + 0x90) = groundY;

    if (*(s8*)((s32)player + 0x3C) == 2) {
        hit = kpaChkHitobjRide();
    } else {
        hit = *(void**)((s32)player + 0x1E8);
    }

    if (hit != 0) {
        if ((*(u32*)player & 0x01000000) != 0) {
            *(f32*)((s32)player + 0x164) =
                *(f32*)((s32)player + 0x114) -
                *(f32*)((s32)player + 0x90);

            tubeStart.x = *(f32*)((s32)player + 0x8C);
            tubeStart.y =
                *(f32*)((s32)player + 0x90) + float_10_80420904;
            tubeStart.z = *(f32*)((s32)player + 0x94);

            tubeHit.start = tubeStart;
            tubeHit.end = *(Vec*)(vecBase + 0x18);
            tubeHit.radius = float_47_8042095c;

            if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
                tubeResult =
                    hitCheckVecFilter(&tubeHit, chkfilterVecVivian);
            } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
                tubeResult =
                    hitCheckVecFilter(&tubeHit, chkfilterVecRoll);
            } else {
                tubeResult =
                    hitCheckVecFilter(&tubeHit, chkfilterVec);
            }

            if (tubeResult != 0) {
                tubeRoundedNormal = tubeHit.normal;

                roundBias =
                    tubeRoundedNormal.x >= float_0_804208ac
                        ? float_0p5_804208cc
                        : float_neg0p5_804208d0;
                tubeRoundedNormal.x =
                    (f32)(s32)(float_1000_804208a8 *
                                   tubeRoundedNormal.x +
                               roundBias) /
                    float_1000_804208a8;

                roundBias =
                    tubeRoundedNormal.y >= float_0_804208ac
                        ? float_0p5_804208cc
                        : float_neg0p5_804208d0;
                tubeRoundedNormal.y =
                    (f32)(s32)(float_1000_804208a8 *
                                   tubeRoundedNormal.y +
                               roundBias) /
                    float_1000_804208a8;

                roundBias =
                    tubeRoundedNormal.z >= float_0_804208ac
                        ? float_0p5_804208cc
                        : float_neg0p5_804208d0;
                tubeRoundedNormal.z =
                    (f32)(s32)(float_1000_804208a8 *
                                   tubeRoundedNormal.z +
                               roundBias) /
                    float_1000_804208a8;

                tubeHitPos = tubeHit.hitPos;
            }

            if (tubeResult != 0) {
                tubeAngleX = (f32)(
                    angleABf(
                        float_0_804208ac,
                        float_0_804208ac,
                        float_100_80420900 * tubeRoundedNormal.z,
                        float_100_80420900 * tubeRoundedNormal.y) -
                    float_180_804208d4);
                tubeAngleX =
                    (f32)(s32)(float_100_80420900 * tubeAngleX +
                               float_0p5_804208cc) /
                    float_100_80420900;

                tubeAngleZ = (f32)(
                    angleABf(
                        float_0_804208ac,
                        float_0_804208ac,
                        float_100_80420900 * tubeRoundedNormal.x,
                        float_100_80420900 * tubeRoundedNormal.y) -
                    float_180_804208d4);
                tubeAngleZ =
                    (f32)(s32)(float_100_80420900 * tubeAngleZ +
                               float_0p5_804208cc) /
                    float_100_80420900;

                tubeLengthSq =
                    tubeRoundedNormal.x * tubeRoundedNormal.x +
                    tubeRoundedNormal.z * tubeRoundedNormal.z;

                {
                    f64 d;
                    f64 inv;
                    f64 half;
                    f64 three;

                    d = (f64)tubeLengthSq;

                    if (d <= *(f64*)(vecBase + 0x208)) {
                        if (*(f64*)(vecBase + 0x208) <= d) {
                            f32 classifyValue;
                            u32 bits;
                            u32 kindBits;
                            s32 kind;

                            classifyValue = tubeLengthSq;
                            bits = *(u32*)&classifyValue;
                            kindBits = bits & 0x7F800000;

                            if (kindBits == 0x7F800000) {
                                if ((bits & 0x7FFFFF) == 0) {
                                    kind = 2;
                                } else {
                                    kind = 1;
                                }
                            } else if (kindBits < 0x7F800000) {
                                if (kindBits == 0) {
                                    if ((bits & 0x7FFFFF) == 0) {
                                        kind = 3;
                                    } else {
                                        kind = 5;
                                    }
                                } else {
                                    kind = 4;
                                }
                            } else {
                                kind = 4;
                            }

                            if (kind == 1) {
                                d = (f64)__float_nan;
                            }
                        } else {
                            d = (f64)__float_nan;
                        }

                        tubeFlatLength = (f32)d;
                    } else {
                        half = *(f64*)(vecBase + 0x1F8);
                        three = *(f64*)(vecBase + 0x200);
                        inv = __frsqrte(d);
                        inv =
                            half * inv *
                            -(d * inv * inv - three);
                        inv =
                            half * inv *
                            -(d * inv * inv - three);
                        tubeFlatLength = (f32)(
                            d * half * inv *
                            -(d * inv * inv - three));
                    }
                }

                angleABf(
                    float_0_804208ac,
                    float_0_804208ac,
                    tubeFlatLength,
                    -tubeRoundedNormal.y);
            }

            if (tubeResult != 0) {
                roll_grx = tubeAngleX;
                roll_grz = tubeAngleZ;
            }
        }
    } else {
        u32 flags;

        flags = *(u32*)player;

        if ((flags & 0x01000000) == 0) {
            if ((flags & 0x20000) == 0) {
                if (drop < float_0_804208ac) {
                    marioChgMot(0xA);
                    marioSetFallPara();
                }
                if (drop > float_0_804208ac) {
                    marioChgMot(0xB);
                    marioSetFallPara();
                }
            }
        } else if ((flags & 0x20000) == 0) {
            *(u32*)player = flags & ~0x40000;
            *(u32*)player |= 0x20000;
            *(f32*)((s32)player + 0x7C) = float_neg0p4_80420988;
            *(f32*)((s32)player + 0x80) = float_neg0p06_8042098c;
            *(f32*)((s32)player + 0x84) = float_neg0p04_80420990;
            *(f32*)((s32)player + 0x88) = float_neg0p02_80420994;
        }
    }
}

void marioChkGnd2(void) {
    extern void* marioSearchGround(f32 param_1, void* yOut, f32 velocityY, void* param_4, void* param_5);
    extern void* kpaSearchGround(f32 param_1, void* yOut, void* param_3, void* param_4);
    extern s32 kpaFireAttackCheck(void);
    extern s32 marioBgmodeChk(void);
    extern void camFollowYOn(void);
    extern void kpaCamFollow(void);
    extern f32 float_0p1_804208ec;
    extern f32 float_0_804208ac;
    extern f32 float_neg10_8042090c;
    void* mario;
    void* hit;
    u8 stack_18[4];
    f32 groundY;
    u8 stack_10[12];
    f32 diff;
    s32 x;
    s32 y;

    mario = marioGetPtr();
    if ((*(u32*)mario & 0x10000) == 0) {
        if (*(s8*)((s32)mario + 0x3C) == 2) {
            hit = kpaSearchGround(float_0p1_804208ec, &groundY, stack_10, stack_18);
        } else {
            hit = marioSearchGround(float_0p1_804208ec, &groundY, *(f32*)((s32)mario + 0x7C), stack_10, stack_18);
        }
        if (hit != 0) {
            diff = *(f32*)((s32)mario + 0x90) - groundY;
            if (diff < float_0_804208ac && diff >= float_neg10_8042090c) {
                *(void**)((s32)mario + 0x1E8) = hit;
                *(f32*)((s32)mario + 0x90) = groundY;
                *(u32*)mario &= ~0xC0;
                *(u16*)((s32)mario + 0x50) = 0;
                *(u16*)((s32)mario + 0x52) = 0;
                if (*(s8*)((s32)mario + 0x3C) == 2) {
                    if (*(f32*)((s32)mario + 0x194) == float_0_804208ac && kpaFireAttackCheck() != 0) {
                        marioChgMot(0);
                    } else {
                        x = *(s8*)((s32)mario + 0x252);
                        y = *(s8*)((s32)mario + 0x253);
                        if (x * x + y * y <= 0xBD1) {
                            marioChgMot(1);
                        } else {
                            marioChgMot(2);
                        }
                    }
                    quake_kpaLandOn();
                } else {
                    marioChgMot(0);
                }
                if (*(s8*)((s32)mario + 0x3C) == 2) {
                    kpaCamFollow();
                } else if (marioBgmodeChk() == 0) {
                    camFollowYOn();
                }
            }
        }
    }
}

s32 marioSearchGround(f64 velocityY, f64 maxDrop, f32* outY, f32* outDrop, f32* outAngle) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 __frsqrte(f64);
    extern f32 __float_nan;
    extern f64 __fabs(f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern f64 revise360(f64);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern void marioUpdateCamCollision(Vec*);
    extern Vec vec3_802c3b78;
    extern f32 float_0_804208ac;
    extern f32 float_0p01_80420964;
    extern f32 float_0p375_80420978;
    extern f32 float_0p425_80420974;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg1_804208e4;
    extern f32 float_2_804208f0;
    extern f32 float_3p1416_804208e8;
    extern f32 float_5_804208c0;
    extern f32 float_11_804208b4;
    extern f32 float_50_80420954;
    extern f32 float_75_80420950;
    extern f32 float_100_80420900;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;
    extern f32 float_neg10000_80420968;

    Vec* vecBase = &vec3_802c3b78;
    void* player = marioGetPtr();
    void* orientPlayer;
    HitCheckArg hit;
    Vec startWork;
    Vec startPos;
    Vec endPos;
    Vec normalPos;
    Vec hitPos;
    Vec cameraPosA;
    Vec cameraPosB;
    f32 currentY;
    f32 playerX;
    f32 playerZ;
    f32 velocityPad;
    f32 dropBase;
    f32 startY;
    f32 hitRadius;
    f32 direction;
    f32 sideX;
    f32 sideZ;
    f32 negSideX;
    f32 negSideZ;
    f32 offsetX;
    f32 offsetZ;
    f32 bestY;
    f32 bias;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 threshold;
    f32 lengthSq;
    f32 flatLength;
    f32 slope;
    f32 hitY;
    f32 angleValue;
    s32 result;
    s32 bestResult;
    s32 i;

    *(f32*)((s32)player + 0x21C) = float_0_804208ac;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = float_0_804208ac;
    *(f32*)((s32)player + 0x218) = float_0_804208ac;

    if ((*(u32*)player & 0x200) != 0) {
        *outY = *(f32*)((s32)player + 0x90);
        return 0;
    }
    if ((*(u32*)player & 0x100) != 0) {
        *outY = *(f32*)((s32)player + 0x90);
        return 0;
    }

    currentY = *(f32*)((s32)player + 0x90);
    velocityPad = (f32)__fabs(velocityY) + float_0p01_80420964;
    dropBase = float_5_804208c0 + (f32)__fabs(maxDrop);

    *outY = currentY;
    *outDrop = float_neg1_804208e4;

    if (*(u16*)((s32)player + 0x2E) == 0x13) {
        direction = toMovedir(*(f32*)((s32)player + 0x1B4));
    } else if (*(f32*)((s32)player + 0x180) != float_0_804208ac) {
        direction = (f32)revise360(
            *(f32*)((s32)player + 0x1A4) -
            *(f32*)((s32)player + 0x19C));
    } else {
        direction = toMovedir(*(f32*)((s32)player + 0x1AC));
    }

    direction = float_3p1416_804208e8 * direction /
                float_180_804208d4;

    if ((*(u32*)player & 0x00100000) != 0) {
        sideX = float_0p425_80420974 *
                *(f32*)((s32)player + 0x1B8) *
                (f32)sin(direction);
        sideZ = float_0p425_80420974 *
                *(f32*)((s32)player + 0x1B8) *
                -(f32)cos(direction);
    } else {
        sideX = float_0p375_80420978 *
                *(f32*)((s32)player + 0x1B8) *
                (f32)sin(direction);
        sideZ = float_0p375_80420978 *
                *(f32*)((s32)player + 0x1B8) *
                -(f32)cos(direction);
    }

    currentY = *(f32*)((s32)player + 0x90);
    negSideZ = -sideZ;
    negSideX = -sideX;
    startY = currentY + dropBase;
    hitRadius = dropBase + velocityPad;
    playerX = *(f32*)((s32)player + 0x8C);
    playerZ = *(f32*)((s32)player + 0x94);
    endPos = *(Vec*)((u8*)vecBase + 0x48);

    bestY = float_neg10000_80420968;
    bestResult = 0;

    for (i = 0; i < 5; i++) {
        if (i == 0) {
            offsetX = float_0_804208ac;
            offsetZ = float_0_804208ac;
        } else if (i == 1) {
            offsetX = sideX;
            offsetZ = sideZ;
        } else if (i == 2) {
            offsetX = negSideZ;
            offsetZ = sideX;
        } else if (i == 3) {
            offsetX = sideZ;
            offsetZ = negSideX;
        } else {
            offsetX = negSideX;
            offsetZ = negSideZ;
        }

        startWork = *(Vec*)((u8*)vecBase + 0x54);
        startWork.x = playerX + offsetX;
        startWork.y = startY;
        startWork.z = playerZ + offsetZ;
        startPos = startWork;

        hit.start = startPos;
        hit.end = endPos;
        hit.radius = hitRadius;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
            result = hitCheckVecFilter(&hit, chkfilterVecRoll);
        } else {
            result = hitCheckVecFilter(&hit, chkfilterVec);
        }

        if (result == 0) {
            continue;
        }

        normalPos = hit.normal;

        bias = normalPos.x >= float_0_804208ac ?
               float_0p5_804208cc : float_neg0p5_804208d0;
        nx = (f32)(s32)(float_1000_804208a8 * normalPos.x + bias) /
             float_1000_804208a8;

        bias = normalPos.y >= float_0_804208ac ?
               float_0p5_804208cc : float_neg0p5_804208d0;
        ny = (f32)(s32)(float_1000_804208a8 * normalPos.y + bias) /
             float_1000_804208a8;

        bias = normalPos.z >= float_0_804208ac ?
               float_0p5_804208cc : float_neg0p5_804208d0;
        nz = (f32)(s32)(float_1000_804208a8 * normalPos.z + bias) /
             float_1000_804208a8;

        hitPos = hit.hitPos;

        if ((*(u32*)((s32)player + 4) & 0x01000000) == 0) {
            if (*(u16*)((s32)player + 0x2E) == 0x10) {
                threshold = float_75_80420950;
            } else {
                threshold = float_50_80420954;
            }
        } else {
            threshold = float_75_80420950;
        }

        /*
         * The target performs this orientation/normal-derived block once
         * before the slope rejection and, if the hit survives, a second time
         * before accepting the ground hit.
         */
        orientPlayer = marioGetPtr();

        angleValue = (f32)(angleABf(
            float_0_804208ac, float_0_804208ac,
            float_100_80420900 * nz,
            float_100_80420900 * ny) - float_180_804208d4);
        angleValue =
            (f32)(s32)(float_100_80420900 * angleValue +
                       float_0p5_804208cc) /
            float_100_80420900;
        *(f32*)((s32)orientPlayer + 0x21C) = angleValue;
        *(f32*)((s32)orientPlayer + 0x220) = float_0_804208ac;

        angleValue = (f32)(angleABf(
            float_0_804208ac, float_0_804208ac,
            float_100_80420900 * nx,
            float_100_80420900 * ny) - float_180_804208d4);
        angleValue =
            (f32)(s32)(float_100_80420900 * angleValue +
                       float_0p5_804208cc) /
            float_100_80420900;
        *(f32*)((s32)orientPlayer + 0x224) = angleValue;

        lengthSq = nx * nx + nz * nz;
        {
            if (lengthSq > float_0_804208ac) {
                f64 d;
                f64 inv;
                f64 half;
                f64 three;

                d = (f64)lengthSq;
                inv = __frsqrte(d);
                half = *(f64*)((u8*)vecBase + 0x1F8);
                three = *(f64*)((u8*)vecBase + 0x200);

                inv = half * inv * -(d * inv * inv - three);
                inv = half * inv * -(d * inv * inv - three);
                flatLength = (f32)(
                    d * half * inv *
                    -(d * inv * inv - three));
            } else if ((f64)lengthSq <
                       *(f64*)((u8*)vecBase + 0x208)) {
                flatLength = __float_nan;
            } else {
                f32 classifyValue;
                u32 bits;
                u32 kindBits;
                s32 kind;

                classifyValue = lengthSq;
                bits = *(u32*)&classifyValue;
                kindBits = bits & 0x7F800000;

                if (kindBits == 0x7F800000) {
                    if ((bits & 0x7FFFFF) == 0) {
                        kind = 2;
                    } else {
                        kind = 1;
                    }
                } else if (kindBits < 0x7F800000) {
                    if (kindBits == 0) {
                        if ((bits & 0x7FFFFF) == 0) {
                            kind = 3;
                        } else {
                            kind = 5;
                        }
                    } else {
                        kind = 4;
                    }
                } else {
                    kind = 4;
                }

                if (kind == 1) {
                    flatLength = __float_nan;
                } else {
                    flatLength = lengthSq;
                }
            }
        }

        slope = angleABf(
            float_0_804208ac, float_0_804208ac,
            flatLength, -ny);
        if (slope >= threshold) {
            result = 0;
        }

        if (result == 0) {
            continue;
        }

        orientPlayer = marioGetPtr();

        angleValue = (f32)(angleABf(
            float_0_804208ac, float_0_804208ac,
            float_100_80420900 * nz,
            float_100_80420900 * ny) - float_180_804208d4);
        angleValue =
            (f32)(s32)(float_100_80420900 * angleValue +
                       float_0p5_804208cc) /
            float_100_80420900;
        *(f32*)((s32)orientPlayer + 0x21C) = angleValue;
        *(f32*)((s32)orientPlayer + 0x220) = float_0_804208ac;

        angleValue = (f32)(angleABf(
            float_0_804208ac, float_0_804208ac,
            float_100_80420900 * nx,
            float_100_80420900 * ny) - float_180_804208d4);
        angleValue =
            (f32)(s32)(float_100_80420900 * angleValue +
                       float_0p5_804208cc) /
            float_100_80420900;
        *(f32*)((s32)orientPlayer + 0x224) = angleValue;

        lengthSq = nx * nx + nz * nz;
        {
            if (lengthSq > float_0_804208ac) {
                f64 d;
                f64 inv;
                f64 half;
                f64 three;

                d = (f64)lengthSq;
                inv = __frsqrte(d);
                half = *(f64*)((u8*)vecBase + 0x1F8);
                three = *(f64*)((u8*)vecBase + 0x200);

                inv = half * inv * -(d * inv * inv - three);
                inv = half * inv * -(d * inv * inv - three);
                flatLength = (f32)(
                    d * half * inv *
                    -(d * inv * inv - three));
            } else if ((f64)lengthSq <
                       *(f64*)((u8*)vecBase + 0x208)) {
                flatLength = __float_nan;
            } else {
                f32 classifyValue;
                u32 bits;
                u32 kindBits;
                s32 kind;

                classifyValue = lengthSq;
                bits = *(u32*)&classifyValue;
                kindBits = bits & 0x7F800000;

                if (kindBits == 0x7F800000) {
                    if ((bits & 0x7FFFFF) == 0) {
                        kind = 2;
                    } else {
                        kind = 1;
                    }
                } else if (kindBits < 0x7F800000) {
                    if (kindBits == 0) {
                        if ((bits & 0x7FFFFF) == 0) {
                            kind = 3;
                        } else {
                            kind = 5;
                        }
                    } else {
                        kind = 4;
                    }
                } else {
                    kind = 4;
                }

                if (kind == 1) {
                    flatLength = __float_nan;
                } else {
                    flatLength = lengthSq;
                }
            }
        }

        slope = angleABf(
            float_0_804208ac, float_0_804208ac,
            flatLength, -ny);
        *(f32*)((s32)player + 0x218) = slope;

        hitY = hitPos.y;

        if ((currentY <= hitY ||
             __fabs(hitY - currentY) < float_2_804208f0) &&
            __fabs(hitY - currentY) <= float_11_804208b4 &&
            hitY > bestY) {
            *outY = hitY;
            bestY = hitY;
            *outDrop = float_0_804208ac;
            *outAngle = angleABf(
                playerX, playerZ, hitPos.x, hitPos.z);

            cameraPosA = hitPos;
            marioUpdateCamCollision(&cameraPosA);
            bestResult = result;
        } else if (__fabs(hitY - currentY) <= __fabs(maxDrop) &&
                   hitY > bestY) {
            *outY = hitY;
            bestY = hitY;
            *outDrop = float_0_804208ac;
            *outAngle = angleABf(
                playerX, playerZ, hitPos.x, hitPos.z);

            cameraPosB = hitPos;
            marioUpdateCamCollision(&cameraPosB);
            bestResult = result;
        } else {
            *outDrop = __fabs(currentY - hitY);
            *outAngle = angleABf(
                playerX, playerZ, hitPos.x, hitPos.z);
        }
    }

    return bestResult;
}

s32 marioSearchGroundRoll(f64 velocityY, f64 maxDrop, f32* outY, f32* outDrop, f32* outAngle) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern s32 hitCheckVecFilter(HitCheckArg* hit, void* filter);
    extern Vec vec3_802c3b78;
    extern Vec vec3_802c3bd8;
    extern f32 float_0_804208ac;
    extern f32 float_0p01_80420964;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg1_804208e4;
    extern f32 float_2_804208f0;
    extern f32 float_3_804208f8;
    extern f32 float_3p1416_804208e8;
    extern f32 float_8p64_8042096c;
    extern f32 float_10_80420904;
    extern f32 float_11_804208b4;
    extern f32 float_75_80420950;
    extern f32 float_90_80420924;
    extern f32 float_100_80420900;
    extern f32 float_105_80420970;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;
    extern f32 float_neg10000_80420968;
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void movePos(f32* x, f32* z, f32 distance, f32 angle);

    Vec* vecBase = &vec3_802c3b78;
    void* player = marioGetPtr();
    HitCheckArg hit;
    Vec startPos;
    Vec endPos;
    f32 offsetX;
    f32 offsetZ;
    f32 angle;
    f32 currentY;
    f32 playerX;
    f32 playerZ;
    f32 collisionY;
    f32 hitRadius;
    f32 tolerance;
    f32 bestY;
    f32 bias;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 flatLength;
    f32 slope;
    f32 hitY;
    s32 first;
    s32 last;
    s32 result;
    s32 bestResult;
    s32 i;

    *(f32*)((s32)player + 0x21C) = float_0_804208ac;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = float_0_804208ac;
    if (*(u32*)player & 0x200) {
        *outY = *(f32*)((s32)player + 0x90);
        return 0;
    }
    if (*(u32*)player & 0x100) {
        *outY = *(f32*)((s32)player + 0x90);
        return 0;
    }

    collisionY = *(f32*)((s32)player + 0x1BC);
    currentY = *(f32*)((s32)player + 0x90);
    playerX = *(f32*)((s32)player + 0x8C);
    playerZ = *(f32*)((s32)player + 0x94);
    *outY = currentY;
    *outDrop = float_neg1_804208e4;
    hitRadius = collisionY + (f32)__fabs(velocityY) + float_0p01_80420964;
    endPos = *(Vec*)((u8*)vecBase + 0x60);

    if (__fabs(*(f32*)((s32)player + 0x218)) >= float_0p01_80420964) {
        first = 4;
        last = 7;
        tolerance = float_10_80420904;
    } else {
        first = 0;
        last = 4;
        tolerance = float_2_804208f0;
    }

    bestY = float_neg10000_80420968;
    bestResult = 0;
    for (i = first; i < last; i++) {
        if (i == 0) goto probe0;
        if (i == 1) goto probe1;
        if (i == 2) goto probe2;
        if (i == 3) goto probe3;
        if (i == 4) goto probe4;
        if (i == 5) goto probe5;
        goto probe6;
probe0:
        offsetX = float_0_804208ac;
        offsetZ = float_0_804208ac;
        goto probe_ready;
probe1:
        angle = float_3p1416_804208e8 *
                (float_90_80420924 + *(f32*)((s32)player + 0x1A4)) /
                float_180_804208d4;
        offsetX = float_8p64_8042096c * (f32)sin(angle);
        offsetZ = -float_8p64_8042096c * (f32)cos(angle);
        goto probe_ready;
probe2:
        angle = float_3p1416_804208e8 *
                (*(f32*)((s32)player + 0x1A4) - float_90_80420924) /
                float_180_804208d4;
        offsetX = float_8p64_8042096c * (f32)sin(angle);
        offsetZ = -float_8p64_8042096c * (f32)cos(angle);
        goto probe_ready;
probe3:
        offsetX = float_0_804208ac;
        offsetZ = float_0_804208ac;
        movePos(&offsetX, &offsetZ, float_3_804208f8,
                *(f32*)((s32)player + 0x1A4));
        goto probe_ready;
probe4:
        offsetX = float_0_804208ac;
        offsetZ = float_0_804208ac;
        goto probe_ready;
probe5:
        angle = float_3p1416_804208e8 *
                (float_90_80420924 + *(f32*)((s32)player + 0x1A4)) /
                float_180_804208d4;
        offsetX = float_8p64_8042096c * (f32)sin(angle);
        offsetZ = -float_8p64_8042096c * (f32)cos(angle);
        movePos(&offsetX, &offsetZ, float_3_804208f8,
                *(f32*)((s32)player + 0x1A4));
        goto probe_ready;
probe6:
        angle = float_3p1416_804208e8 *
                (*(f32*)((s32)player + 0x1A4) - float_90_80420924) /
                float_180_804208d4;
        offsetX = float_8p64_8042096c * (f32)sin(angle);
        offsetZ = -float_8p64_8042096c * (f32)cos(angle);
        movePos(&offsetX, &offsetZ, float_3_804208f8,
                *(f32*)((s32)player + 0x1A4));
probe_ready:
        startPos = *(Vec*)((u8*)vecBase + 0x6C);
        startPos.x = playerX + offsetX;
        startPos.y = currentY + collisionY;
        startPos.z = playerZ + offsetZ;
        hit.start = startPos;
        hit.end = endPos;
        hit.radius = hitRadius;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else if (*(u32*)marioGetPtr() & 0x01000000) {
            result = hitCheckVecFilter(&hit, chkfilterVecRoll);
        } else {
            result = hitCheckVecFilter(&hit, chkfilterVec);
        }
        if (result == 0) {
            continue;
        }

        bias = hit.normal.x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        nx = (f32)(s32)(hit.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        ny = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        nz = (f32)(s32)(hit.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;

        *(f32*)((s32)player + 0x21C) =
            (f32)(angleABf(0.0, 0.0, float_100_80420900 * nz,
                           float_100_80420900 * ny) - float_180_804208d4);
        *(f32*)((s32)player + 0x220) = 0.0f;
        *(f32*)((s32)player + 0x224) =
            (f32)(angleABf(0.0, 0.0, float_100_80420900 * nx,
                           float_100_80420900 * ny) - float_180_804208d4);
        flatLength = (f32)sqrt(nx * nx + nz * nz);
        slope = (f32)angleABf(0.0, 0.0, flatLength, -ny);

        if (slope > float_105_80420970 || slope < float_75_80420950) {
            *(f32*)((s32)player + 0x218) = slope;
            hitY = hit.hitPos.y;
            if (currentY <= hitY || __fabs(hitY - currentY) < tolerance) {
                if (__fabs(hitY - currentY) <= float_11_804208b4 && hitY > bestY) {
                    *outY = hitY;
                    *outDrop = float_0_804208ac;
                    *outAngle = (f32)angleABf(playerX, playerZ,
                                              hit.hitPos.x, hit.hitPos.z);
                    bestY = hitY;
                    bestResult = result;
                }
            } else if (__fabs(hitY - currentY) <= __fabs(maxDrop)) {
                if (hitY > bestY) {
                    *outY = hitY;
                    *outDrop = float_0_804208ac;
                    *outAngle = (f32)angleABf(playerX, playerZ,
                                              hit.hitPos.x, hit.hitPos.z);
                    bestY = hitY;
                    bestResult = result;
                }
            } else {
                *outDrop = (f32)__fabs(currentY - hitY);
                *outAngle = (f32)angleABf(playerX, playerZ,
                                          hit.hitPos.x, hit.hitPos.z);
            }
        }
    }
    return bestResult;
}

void marioChkToge(void) {
    extern void set_damage_root_ypos(f64 y);
    extern f32 float_0p5_804208cc;
    void* mario;
    void* hit;
    u32 attr;
    s32 status;

    mario = marioGetPtr();
    status = vivianGetStatus();
    if (status != 2 && vivianGetStatus() != 1) {
        if (vivianGetStatus() == 3) {
            return;
        }
        hit = *(void**)((s32)mario + 0x1E4);
        if (hit != 0) {
            attr = hitGetAttr(hit);
            if (*(u16*)((s32)mario + 0x2E) != 0x20 &&
                (attr & 0x800) != 0 &&
                *(f32*)((s32)mario + 0x214) < float_0p5_804208cc * *(f32*)((s32)mario + 0x1B8)) {
                set_damage_root_ypos(*(f32*)((s32)mario + 0x90));
                marioChgMot(0x20);
            }
        }
        hit = *(void**)((s32)mario + 0x1F4);
        if (hit != 0) {
            attr = hitGetAttr(hit);
            if (*(u16*)((s32)mario + 0x2E) != 0x20 && (attr & 0x800) != 0) {
                set_damage_root_ypos(*(f32*)((s32)mario + 0x90));
                marioChgMot(0x20);
            }
        }
        hit = *(void**)((s32)mario + 0x1F8);
        if (hit != 0) {
            attr = hitGetAttr(hit);
            if (*(u16*)((s32)mario + 0x2E) != 0x20 && (attr & 0x800) != 0) {
                set_damage_root_ypos(*(f32*)((s32)mario + 0x90));
                marioChgMot(0x20);
            }
        }
    }
}

void marioChkLandon(f32 velocityY, s32 param_2) {
    extern void marioLandonChk_old(f32 velocityY, s32 param_1);
    extern void marioSearchGround(s32 param_1, f32 velocityY1, f32 velocityY2, void* param_4, void* param_5);
    extern void marioSearchGroundRoll(s32 param_1, f32 velocityY1, f32 velocityY2, void* param_4, void* param_5);
    void* mario;
    u8 stack_18[4];
    u8 stack_14[8];

    mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) != 2) {
        if ((*(u32*)mario & 0x1000000) == 0) {
            marioSearchGround(param_2, velocityY, velocityY, stack_14, stack_18);
        } else {
            marioSearchGroundRoll(param_2, velocityY, velocityY, stack_14, stack_18);
        }
    } else {
        marioLandonChk_old(velocityY, param_2);
    }
}

s32 marioLandonChk_old(f64 velocityY, f32* outY) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern f32 toMovedir(f64 angle);
    extern f64 revise360(f64 angle);
    extern s32 kpaSearchGround(f64 length, f32* outY, void* outA, void* outB);
    extern f64 sin(f64 angle);
    extern f64 cos(f64 angle);
    extern f64 sqrt(f64 value);
    extern f64 __frsqrte(f64 value);
    extern f64 __fabs(f64 value);
    extern f64 angleABf(f64 x1, f64 y1, f64 x2, f64 y2);
    extern s32 hitCheckVecFilter(HitCheckArg* hit, void* filter);
    extern Vec vec3_802c3b78;
    extern Vec vec3_802c3b84;
    extern f32 float_0_804208ac;
    extern f32 float_0p1_804208ec;
    extern f32 float_0p01_80420964;
    extern f32 float_0p75_804208e0;
    extern f32 float_11_804208b4;
    extern f32 float_0p25_804208dc;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg1_804208e4;
    extern f32 float_3p1416_804208e8;
    extern f32 float_100_80420900;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;
    extern f64 double_0p5_802c3d70;
    extern f64 double_3_802c3d78;
    extern f64 double_0_802c3d80;
    extern f32 __float_nan;

    void* player = marioGetPtr();
    HitCheckArg hit;
    f32 groundY;
    u8 scratchA[4];
    u8 scratchB[4];
    f32 direction;
    f32 radians;
    f32 sideX;
    f32 sideZ;
    f32 offsetX;
    f32 offsetZ;
    f32 height;
    f32 radius;
    f32 roundedRadius;
    volatile f32 normal[3];
    f32 hitPos[3];
    f32 flatLength;
    f64 lengthSq;
    f64 invLength;
    union {
        f32 f;
        u32 u;
    } classifiedLength;
    f32 vertical;
    f32 bias;
    s32 result;
    s32 i;

    *(f32*)((s32)player + 0x21C) = float_0_804208ac;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = float_0_804208ac;
    *(f32*)((s32)player + 0x218) = float_0_804208ac;

    if (*(u32*)player & 0x200) {
        *outY = *(f32*)((s32)player + 0x90);
        return 0;
    }
    if (*(u32*)player & 0x100) {
        *outY = *(f32*)((s32)player + 0x90);
        return 0;
    }

    radius = (f32)__fabs(velocityY) + float_0p01_80420964;
    height = float_0p75_804208e0 * *(f32*)((s32)player + 0x1BC);
    if (*(f32*)((s32)player + 0x180) == float_0_804208ac) {
        direction = toMovedir(*(f32*)((s32)player + 0x1AC));
    } else {
        direction = (f32)revise360(*(f32*)((s32)player + 0x1A4) - *(f32*)((s32)player + 0x19C));
    }
    radians = (float_3p1416_804208e8 * direction) / float_180_804208d4;
    sideX = float_0p25_804208dc * *(f32*)((s32)player + 0x1B8) * (f32)sin(radians);
    sideZ = float_0p25_804208dc * *(f32*)((s32)player + 0x1B8) * -(f32)cos(radians);

    if (*(s8*)((s32)player + 0x3C) == 2) {
        result = kpaSearchGround(float_11_804208b4, &groundY, scratchA, scratchB);
        *outY = groundY;
        return result;
    }

    radius += height;
    for (i = 0; i < 5; i++) {
        if (i == 0) {
            offsetX = float_0_804208ac;
            offsetZ = float_0_804208ac;
        } else if (i == 1) {
            offsetX = sideX;
            offsetZ = sideZ;
        } else if (i == 2) {
            offsetX = -sideZ;
            offsetZ = sideX;
        } else if (i == 3) {
            offsetX = sideZ;
            offsetZ = -sideX;
        } else {
            offsetX = -sideX;
            offsetZ = -sideZ;
        }

        hit.start = vec3_802c3b78;
        hit.end = vec3_802c3b84;
        hit.start.x = *(f32*)((s32)player + 0x8C) + offsetX;
        hit.start.y = *(f32*)((s32)player + 0x90) + height;
        hit.start.z = *(f32*)((s32)player + 0x94) + offsetZ;
        hit.end.x = float_0_804208ac;
        hit.end.y = float_neg1_804208e4;
        hit.end.z = float_0_804208ac;
        hit.radius = radius;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else if (*(u32*)marioGetPtr() & 0x01000000) {
            result = hitCheckVecFilter(&hit, chkfilterVecRoll);
        } else {
            result = hitCheckVecFilter(&hit, chkfilterVec);
        }
        if (result == 0) {
            continue;
        }

        bias = hit.radius >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
        roundedRadius = (f32)(s32)(hit.radius * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.x >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
        normal[0] = (f32)(s32)(hit.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.y >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
        normal[1] = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.z >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
        normal[2] = (f32)(s32)(hit.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;
        hitPos[0] = hit.hitPos.x;
        hitPos[1] = hit.hitPos.y;
        hitPos[2] = hit.hitPos.z;

        *(f32*)((s32)player + 0x21C) =
            (f32)(s32)(float_100_80420900 * (f32)(angleABf(0.0, 0.0,
                float_100_80420900 * hitPos[2], float_100_80420900 * hitPos[1]) - float_180_804208d4)
                + float_0p5_804208cc) / float_100_80420900;
        *(f32*)((s32)player + 0x220) = float_0_804208ac;
        *(f32*)((s32)player + 0x224) =
            (f32)(s32)(float_100_80420900 * (f32)(angleABf(0.0, 0.0,
                float_100_80420900 * hitPos[0], float_100_80420900 * hitPos[1]) - float_180_804208d4)
                + float_0p5_804208cc) / float_100_80420900;
        lengthSq = hitPos[0] * hitPos[0] + hitPos[2] * hitPos[2];
        if (lengthSq > double_0_802c3d80) {
            invLength = __frsqrte(lengthSq);
            invLength = double_0p5_802c3d70 * invLength *
                (double_3_802c3d78 - lengthSq * invLength * invLength);
            invLength = double_0p5_802c3d70 * invLength *
                (double_3_802c3d78 - lengthSq * invLength * invLength);
            invLength = double_0p5_802c3d70 * invLength *
                (double_3_802c3d78 - lengthSq * invLength * invLength);
            flatLength = (f32)(lengthSq * invLength);
        } else if (lengthSq < double_0_802c3d80) {
            flatLength = __float_nan;
        } else {
            classifiedLength.f = (f32)lengthSq;
            if ((classifiedLength.u & 0x7F800000) == 0x7F800000 &&
                (classifiedLength.u & 0x007FFFFF) != 0) {
                flatLength = __float_nan;
            } else {
                flatLength = classifiedLength.f;
            }
        }
        *(f32*)((s32)player + 0x218) = (f32)angleABf(0.0, 0.0, flatLength, -hitPos[1]);

        vertical = (f32)(s32)(float_100_80420900 *
            ((*(f32*)((s32)player + 0x90) + height - normal[1]) - roundedRadius)
            + float_0p5_804208cc) / float_100_80420900;
        if (__fabs(vertical) < float_0p1_804208ec) {
            *outY = (f32)(s32)(float_100_80420900 * normal[1] + float_0p5_804208cc) /
                float_100_80420900;
            return result;
        }
    }

    return 0;
}

s32 marioChkLandon2(f64 velocityY, f64 yOffset, f32* outY) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern f32 toMovedir(f64 angle);
    extern f64 sin(f64 angle);
    extern f64 cos(f64 angle);
    extern f64 sqrt(f64 value);
    extern f64 __frsqrte(f64 value);
    extern f64 __fabs(f64 value);
    extern f64 angleABf(f64 x1, f64 y1, f64 x2, f64 y2);
    extern s32 hitCheckVecFilter(HitCheckArg* hit, void* filter);
    extern Vec vec3_802c3bf0;
    extern f32 float_0_804208ac;
    extern f32 float_0p1_804208ec;
    extern f32 float_0p25_804208dc;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_3p1416_804208e8;
    extern f32 float_100_80420900;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;
    extern f64 double_0p5_802c3d70;
    extern f64 double_3_802c3d78;
    extern f64 double_0_802c3d80;
    extern f32 __float_nan[];

    void* player = marioGetPtr();
    HitCheckArg hit;
    f32 dir;
    f32 radians;
    f32 sideX;
    f32 sideZ;
    f32 height;
    f32 probeRadius;
    f32 reach;
    f32 bias;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 flatLength;
    f32 lengthSqF;
    f64 lengthSq;
    f64 invLength;
    u32 lengthBits;
    s32 lengthExp;
    s32 lengthKind;
    f32 hitY;
    f32 angle;
    s32 result;
    s32 i;

    *(f32*)((s32)player + 0x218) = float_0_804208ac;
    *(f32*)((s32)player + 0x21C) = float_0_804208ac;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = float_0_804208ac;

    if (*(u32*)player & 0x100) {
        *outY = *(f32*)((s32)player + 0x90);
        return 0;
    }
    if (*(u32*)player & 0x200) {
        *outY = *(f32*)((s32)player + 0x90);
        return 0;
    }

    height = float_0p5_804208cc * *(f32*)((s32)player + 0x1BC);
    probeRadius = height + (f32)__fabs(velocityY) + float_0p1_804208ec;
    if (*(f32*)((s32)player + 0x180) == float_0_804208ac) {
        dir = toMovedir(*(f32*)((s32)player + 0x1AC));
    } else {
        dir = *(f32*)((s32)player + 0x1A4);
    }
    radians = (float_3p1416_804208e8 * dir) / float_180_804208d4;
    reach = float_0p25_804208dc * *(f32*)((s32)player + 0x1B8);
    sideX = reach * (f32)sin(radians);
    sideZ = reach * -(f32)cos(radians);

    for (i = 0; i < 5; i++) {
        f32 offsetX;
        f32 offsetZ;
        if (i == 0) {
            offsetX = 0.0f;
            offsetZ = 0.0f;
        } else if (i == 1) {
            offsetX = sideX;
            offsetZ = sideZ;
        } else if (i == 2) {
            offsetX = -sideZ;
            offsetZ = -sideX;
        } else if (i == 3) {
            offsetX = sideZ;
            offsetZ = -sideX;
        } else {
            offsetX = -sideZ;
            offsetZ = -sideX;
        }
        hit.start.x = *(f32*)((s32)player + 0x8C) + offsetX;
        hit.start.y = *(f32*)((s32)player + 0x90) + (f32)yOffset + height;
        hit.start.z = *(f32*)((s32)player + 0x94) + offsetZ;
        hit.end = vec3_802c3bf0;
        hit.radius = probeRadius;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else if (*(u32*)marioGetPtr() & 0x01000000) {
            result = hitCheckVecFilter(&hit, chkfilterVecRoll);
        } else {
            result = hitCheckVecFilter(&hit, chkfilterVec);
        }
        if (result == 0) {
            continue;
        }

        bias = hit.radius >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        hit.radius = (f32)(s32)(hit.radius * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        nx = (f32)(s32)(hit.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        ny = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        nz = (f32)(s32)(hit.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;

        angle = (f32)(angleABf(0.0, 0.0, float_100_80420900 * nz,
                              float_100_80420900 * ny) - float_180_804208d4);
        *(f32*)((s32)player + 0x21C) =
            (f32)(s32)(float_100_80420900 * angle + float_0p5_804208cc) /
            float_100_80420900;
        *(f32*)((s32)player + 0x220) = 0.0f;
        angle = (f32)(angleABf(0.0, 0.0, float_100_80420900 * nx,
                              float_100_80420900 * ny) - float_180_804208d4);
        *(f32*)((s32)player + 0x224) =
            (f32)(s32)(float_100_80420900 * angle + float_0p5_804208cc) /
            float_100_80420900;
        lengthSqF = nx * nx + nz * nz;
        lengthSq = lengthSqF;
        if (lengthSqF > float_0_804208ac) {
            invLength = __frsqrte(lengthSq);
            invLength = double_0p5_802c3d70 * invLength *
                -(lengthSq * invLength * invLength - double_3_802c3d78);
            invLength = double_0p5_802c3d70 * invLength *
                -(lengthSq * invLength * invLength - double_3_802c3d78);
            flatLength = (f32)(lengthSq * double_0p5_802c3d70 * invLength *
                -(lengthSq * invLength * invLength - double_3_802c3d78));
        } else if (lengthSq < double_0_802c3d80) {
            flatLength = __float_nan[0];
        } else {
            lengthBits = *(u32*)&lengthSqF;
            lengthExp = (s32)(lengthBits & 0x7F800000);

            if (lengthExp >= 0x7F800000) {
                if (lengthExp == 0x7F800000) {
                    if ((lengthBits & 0x7FFFFF) != 0) {
                        lengthKind = 1;
                    } else {
                        lengthKind = 2;
                    }
                } else {
                    lengthKind = 4;
                }
            } else if (lengthExp == 0) {
                if ((lengthBits & 0x7FFFFF) != 0) {
                    lengthKind = 5;
                } else {
                    lengthKind = 3;
                }
            } else {
                lengthKind = 4;
            }

            if (lengthKind == 1) {
                flatLength = __float_nan[0];
            } else {
                flatLength = lengthSqF;
            }
        }

        *(f32*)((s32)player + 0x218) =
            (f32)angleABf(0.0, 0.0, flatLength, -ny);

        hitY = hit.start.y - hit.radius;
        if (__fabs(hitY - hit.hitPos.y) < float_0p1_804208ec) {
            *outY = hit.hitPos.y;
            return result;
        }
    }

    return 0;
}

f64 marioChkOverhead(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern u8 marioSearchHead_jabara(void);
    extern u8 marioSearchHead(void);
    extern u8 marioSearchHead_roll(void);
    extern void* kpaSearchHead(void*, void*);
    extern void* mobjHitObjPtrToPtr(s32);
    extern s32 strcmp(char*, char*);
    extern void marioItemGetDisable(void);
    extern s32 kpaGetBodyStatus(void);
    extern void marioClearJumpPara(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern f64 partyGetHeight(void*);
    extern s32 marioChkSlitEnd(void);
    extern void marioChgStayMotion(void);
    extern void psndSFXOn_3D(s32, void*);
    extern char str_MOBJ_PowerUpBlock_802c3d98[];
    extern char str_MOBJ_SaveBlock_802c3dac[];
    extern char str_MOBJ_RecoveryBlock_802c3dbc[];
    extern char str_MOBJ_CoinTakenRecove_802c3dd0[];
    extern f32 float_0_804208ac;
    extern f32 float_5_804208c0;
    extern f32 float_1_804208c4;
    extern f32 float_10_80420904;
    extern f32 float_40_80420908;
    extern f32 float_47_8042095c;
    extern f32 float_0p5_804208cc;

    void* player;
    void* hit;
    void* mobj;
    void* party;
    Vec pos;
    f32 height;
    f32 newY;
    f32 vel;
    s32 powerBlock;
    s32 motion;
    u8 temp[4];

    player = marioGetPtr();
    if (*(u16*)((s32)player + 0x2E) == 8) {
        return *(f32*)((s32)player + 0x90);
    }

    if (*(u16*)((s32)player + 0x2E) == 0x14) {
        pos = *(Vec*)((s32)player + 0x8C);
        height = *(f32*)((s32)player + 0x1BC);
        if (jabaraNoHitChk() == 0) {
            height += float_5_804208c0;
            hit = ((void* (*)(f64, void*, void*, void*))marioSearchHead_jabara)(float_0_804208ac, &pos, &height, temp);
            *(void**)((s32)player + 0x1F8) = hit;
            if (*(void**)((s32)player + 0x1F8) == 0) {
                if (*(u32*)player & 0x10000) {
                    vel = *(f32*)((s32)player + 0x7C);
                } else {
                    vel = float_10_80420904;
                }
                hit = ((void* (*)(f64, void*, void*))marioSearchHead)(vel, &pos, &height);
                *(void**)((s32)player + 0x1F8) = hit;
            }
        }
    } else if (*(u32*)player & 0x01000000) {
        pos = *(Vec*)((s32)player + 0x8C);
        hit = ((void* (*)(f64, void*, void*))marioSearchHead_roll)(float_40_80420908, &pos, &height);
        *(void**)((s32)player + 0x1F8) = hit;
    } else {
        pos = *(Vec*)((s32)player + 0x8C);
        height = *(f32*)((s32)player + 0x1BC);
        if (*(u32*)player & 0x10000) {
            vel = *(f32*)((s32)player + 0x7C);
        } else {
            vel = float_10_80420904;
        }
        if (*(s8*)((s32)player + 0x3C) == 2) {
            hit = kpaSearchHead(&pos, &height);
        } else {
            hit = ((void* (*)(f64, void*, void*))marioSearchHead)(vel, &pos, &height);
        }
        *(void**)((s32)player + 0x1F8) = hit;
    }

    *(f32*)((s32)player + 0x8C) = pos.x;
    hit = *(void**)((s32)player + 0x1F8);
    if (hit == 0) {
        if (!(*(u32*)player & 0x01000000)) {
            if (height >= float_0_804208ac && height <= float_5_804208c0) {
                *(u32*)player |= 0x08000000;
            } else {
                *(u32*)player &= ~0x08000000;
            }
        } else if (height >= float_0_804208ac &&
                   (height + *(f32*)((s32)player + 0x1BC)) <= float_47_8042095c) {
            *(u32*)player |= 0x08000000;
        } else {
            *(u32*)player &= ~0x08000000;
        }
        return pos.y;
    }

    powerBlock = 0;
    if (hitGetAttr(hit) & 0x80000000) {
        mobj = mobjHitObjPtrToPtr((s32)hit);
        if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_PowerUpBlock_802c3d98) == 0 ||
            strcmp((char*)((s32)mobj + 0x15), str_MOBJ_SaveBlock_802c3dac) == 0 ||
            strcmp((char*)((s32)mobj + 0x15), str_MOBJ_RecoveryBlock_802c3dbc) == 0 ||
            strcmp((char*)((s32)mobj + 0x15), str_MOBJ_CoinTakenRecove_802c3dd0) == 0) {
            powerBlock = 1;
        }
    }
    if (powerBlock) {
        *(f32*)((s32)player + 0x180) = float_0_804208ac;
        *(u32*)player |= 0x80;
    }
    *(u32*)player |= 0x08000000;

    if (*(s8*)((s32)player + 0x3C) == 2) {
        marioItemGetDisable();
        if (kpaGetBodyStatus() == 0) {
            marioClearJumpPara();
        }
    }

    player = marioGetPtr();
    height = *(f32*)((s32)player + 0x1BC);
    if (*(u16*)((s32)player + 0x2E) == 0x1A) {
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            height = (f32)partyGetHeight(party);
        }
    }
    newY = (f32)(s32)((pos.y - height) * float_10_80420904 + float_0p5_804208cc) / float_10_80420904 - float_1_804208c4;
    if (!(*(u32*)player & 0x20000) && newY < *(f32*)((s32)player + 0x120)) {
        newY = *(f32*)((s32)player + 0x120);
        if (!(*(u32*)player & 0x01000000) && *(u16*)((s32)player + 0x2E) != 0x19 &&
            *(u16*)((s32)player + 0x2E) != 0x18 && *(u16*)((s32)player + 0x2E) != 0x1C &&
            *(s8*)((s32)player + 0x3C) != 2 && marioChkSlitEnd() == 0) {
            if (*(f32*)((s32)player + 0x180) == float_0_804208ac) {
                if (*(u16*)((s32)player + 0x2E) != 0) {
                    marioChgStayMotion();
                }
            } else {
                if (*(f32*)((s32)player + 0x180) <= *(f32*)((s32)player + 0x184)) {
                    motion = 1;
                } else {
                    motion = 2;
                }
                if (motion != *(u16*)((s32)player + 0x30)) {
                    marioChgMot(motion);
                }
            }
        }
    }
    if (*(u16*)((s32)player + 0x2E) == 3 || *(u16*)((s32)player + 0x2E) == 6) {
        psndSFXOn_3D(0x156, (void*)((s32)player + 0x8C));
        marioClearJumpPara();
        *(u32*)player &= 0xF87FFFFF;
        if (*(s8*)((s32)player + 0x3C) != 2) {
            marioChgMot(10);
            marioSetFallPara();
        }
        *(f32*)((s32)player + 0x90) = newY;
    }
    return newY;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
double marioChkOverheadJabara(void* param_1, float* param_2) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* marioGetPtr(void);
    extern s32 jabaraNoHitChk(void);
    extern void* marioSearchHead_jabara(f64, f32*, f32*, s32*);
    extern void* marioSearchHead(f64, f32*, f32*);
    extern void marioChgMot(s32);
    extern f32 float_0_804208ac;
    extern f32 float_0p1_804208ec;
    extern f32 float_neg1_804208e4;
    extern f32 float_5_804208c0;
    u8* player = marioGetPtr();
    Vec pos;
    f32 height;
    s32* side = param_1;

    *side = 0;
    *param_2 = float_neg1_804208e4;
    pos = *(Vec*)(player + 0x8C);
    height = *(f32*)(player + 0x1BC);
    if (*(u16*)(player + 0x2E) == 8) {
        return (double)pos.y;
    } else {
        if (jabaraNoHitChk() == 0) {
            height += float_5_804208c0;
            *(void**)(player + 0x1F8) = marioSearchHead_jabara(
                float_0_804208ac, &pos.x, &height, side);
            if (*(void**)(player + 0x1F8) == 0) {
                height = *(f32*)(player + 0x1BC) + float_5_804208c0;
                *(void**)(player + 0x1F8) = marioSearchHead(
                    float_0_804208ac, &pos.x, &height);
            }
        }
        *(f32*)(player + 0x8C) = pos.x;
        if (*(void**)(player + 0x1F8) == 0) {
            if (height <= float_0_804208ac) {
                *(u32*)player &= ~0x08000000;
            } else {
                *param_2 = height;
                *(u32*)player |= 0x08000000;
            }
        } else {
            f32 adjust = float_0p1_804208ec;
            *(u32*)player |= 0x08000000;
            pos.y = (pos.y - *(f32*)(player + 0x1BC)) - adjust;
            if (*(u16*)(player + 0x2E) == 3) {
                marioChgMot(0);
            }
        }
    }
    return (double)pos.y;
}
extern f64 __frsqrte(f64 value);
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void* marioSearchHead(f32 velocityY, f32* pos, f32* outHeight) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef union FloatBits {
        f32 f;
        u32 u;
    } FloatBits;
    typedef struct HeadProbe {
        f32 radius;
        Vec temp;
        Vec normal;
        Vec hitPos;
        Vec end;
        Vec start;
    } HeadProbe;

    extern void* marioGetPtr(void);
    extern u8 marioHitCheck2(void);
    extern Vec vec3_802c3b78;
    extern f32 __float_nan[];
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg1_804208e4;
    extern f32 float_neg10_8042090c;
    extern f32 float_3p1416_804208e8;
    extern f32 float_180_804208d4;
    extern f32 float_50_80420954;
    extern f32 float_75_80420950;
    extern f32 float_100_80420900;
    extern f32 float_0p05_80420958;

    void* mario;
    void* angleMario;
    void* hit;
    u8* vecBase;
    HeadProbe probe;
    FloatBits classifiedLength;
    f32 height;
    f32 dir;
    f32 rad;
    f32 sideX;
    f32 sideZ;
    f32 yHalf;
    f32 offX;
    f32 offZ;
    f32 hitY;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 normalY100;
    f32 flatLength;
    f32 lengthSqF;
    f64 lengthSq;
    f64 invLength;
    f64 slopeAngle;
    s32 lengthBits;
    s32 lengthExp;
    s32 lengthKind;
    s32 i;

    vecBase = (u8*)&vec3_802c3b78;
    mario = marioGetPtr();

    if (*(f32*)((s32)mario + 0x180) != float_0_804208ac) {
        dir = *(f32*)((s32)mario + 0x1A4);
    } else {
        dir = toMovedir(*(f32*)((s32)mario + 0x1AC));
    }

    rad = (float_3p1416_804208e8 * dir) / float_180_804208d4;
    sideX = float_0p5_804208cc *
        ((*(f32*)((s32)mario + 0x1B8) - float_0p5_804208cc) *
         (f32)sin(rad));

    sideZ = (f32)cos(rad);
    height = *outHeight;
    probe.radius = height + velocityY;
    sideZ = float_0p5_804208cc *
        ((*(f32*)((s32)mario + 0x1B8) - float_0p5_804208cc) * -sideZ);

    *outHeight = float_neg1_804208e4;
    yHalf = float_0p5_804208cc * height;

    for (i = 0; i < 5; i++) {
        if (i == 0) {
            offX = float_0_804208ac;
            offZ = float_0_804208ac;
        } else if (i == 1) {
            offX = sideX;
            offZ = sideZ;
        } else if (i == 2) {
            offX = -sideZ;
            offZ = sideX;
        } else if (i == 3) {
            offX = sideZ;
            offZ = -sideX;
        } else if (i == 4) {
            offX = -sideX;
            offZ = -sideZ;
        }

        probe.temp = *(Vec*)(vecBase + 0xC0);
        probe.temp.x = pos[0] + offX;
        probe.temp.y = pos[1] + yHalf;
        probe.temp.z = pos[2] + offZ;
        probe.start = probe.temp;
        probe.end = *(Vec*)(vecBase + 0xCC);

        hit = ((void* (*)(void*, void*, void*, void*, void*))marioHitCheck2)(
            &probe.start, &probe.end, &probe.hitPos, &probe.normal, &probe.radius);

        if (hit != 0) {
            nx = probe.normal.x;
            ny = probe.normal.y;
            nz = probe.normal.z;

            angleMario = marioGetPtr();
            normalY100 = float_100_80420900 * ny;

            *(f32*)((s32)angleMario + 0x21C) =
                (f32)(s32)(float_100_80420900 *
                    ((f32)angleABf(
                        float_0_804208ac, float_0_804208ac,
                        float_100_80420900 * nz, normalY100) -
                     float_180_804208d4) +
                    float_0p5_804208cc) /
                float_100_80420900;
            *(f32*)((s32)angleMario + 0x220) = float_0_804208ac;

            *(f32*)((s32)angleMario + 0x224) =
                (f32)(s32)(float_100_80420900 *
                    ((f32)angleABf(
                        float_0_804208ac, float_0_804208ac,
                        float_100_80420900 * nx, normalY100) -
                     float_180_804208d4) +
                    float_0p5_804208cc) /
                float_100_80420900;

            lengthSqF = nz * nz + nx * nx;
            lengthSq = (f64)lengthSqF;
            if (lengthSqF > float_0_804208ac) {
                invLength = __frsqrte(lengthSq);
                invLength = *(f64*)(vecBase + 0x1F8) * invLength *
                    -(lengthSq * invLength * invLength -
                      *(f64*)(vecBase + 0x200));
                invLength = *(f64*)(vecBase + 0x1F8) * invLength *
                    -(lengthSq * invLength * invLength -
                      *(f64*)(vecBase + 0x200));
                flatLength = (f32)(
                    lengthSq * *(f64*)(vecBase + 0x1F8) * invLength *
                    -(lengthSq * invLength * invLength -
                      *(f64*)(vecBase + 0x200)));
            } else if (lengthSq < *(f64*)(vecBase + 0x208)) {
                flatLength = __float_nan[0];
            } else {
                classifiedLength.f = lengthSqF;
                lengthBits = (s32)classifiedLength.u;
                lengthExp = lengthBits & 0x7F800000;

                if (lengthExp == 0x7F800000) {
                    if ((lengthBits & 0x7FFFFF) != 0) {
                        lengthKind = 1;
                    } else {
                        lengthKind = 2;
                    }
                } else if (lengthExp >= 0x7F800000) {
                    lengthKind = 4;
                } else if (lengthExp == 0) {
                    if ((lengthBits & 0x7FFFFF) != 0) {
                        lengthKind = 5;
                    } else {
                        lengthKind = 3;
                    }
                } else {
                    lengthKind = 4;
                }

                if (lengthKind == 1) {
                    flatLength = __float_nan[0];
                } else {
                    flatLength = lengthSqF;
                }
            }

            slopeAngle = angleABf(
                float_0_804208ac, float_0_804208ac,
                flatLength, -ny);

            if ((*(u32*)mario & 0x01000000) == 0) {
                f32 limit;

                if ((*(u32*)((s32)mario + 4) & 0x01000000) != 0) {
                    limit = float_75_80420950;
                } else {
                    limit = float_50_80420954;
                }

                if (slopeAngle <= (f64)(float_180_804208d4 - limit) &&
                    (f64)limit <= slopeAngle) {
                    hit = 0;
                }
            }
        }

        if ((*(u32*)mario & 0x01000000) != 0) {
            if (hit != 0) {
                hitY = -((float_0p05_80420958 * height) -
                    (f32)(s32)(
                        float_100_80420900 * probe.hitPos.y + yHalf) /
                    float_100_80420900);

                if (hitY <= pos[1] + height) {
                    pos[1] = hitY;
                    *outHeight = float_0_804208ac;
                    return hit;
                }

                *outHeight = (hitY - pos[1]) - height;
                return 0;
            }
        } else if (hit != 0) {
            hitY = (f32)(s32)(
                float_100_80420900 * probe.hitPos.y + yHalf) /
                float_100_80420900;

            if (*(f32*)((s32)mario + 0x7C) < float_neg10_8042090c) {
                *outHeight = (hitY - pos[1]) - height;
                return 0;
            }

            if (hitY <= pos[1] + height) {
                pos[1] = hitY;
                *outHeight = float_0_804208ac;
                return hit;
            }

            *outHeight = (hitY - pos[1]) - height;
            return 0;
        }
    }

    return 0;
}

s32 marioSearchHead_roll(f64 heightAdd, f32* pos, f32* outHeight) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void movePos(f64, f64, void*, void*);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern Vec vec3_802c3c50;
    extern Vec vec3_802c3c5c;
    extern f32 float_neg1_804208e4;
    extern f32 float_1_804208c4;
    extern f32 float_0_804208ac;
    extern f32 float_3p1416_804208e8;
    extern f32 float_180_804208d4;
    extern f32 float_90_80420924;
    extern f32 float_9_8042094c;
    extern f32 float_3_804208f8;
    extern f32 float_1000_804208a8;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;

    void* mario;
    HitCheckArg hit;
    Vec start;
    Vec offset;
    Vec normal;
    Vec hitPos;
    f32 playerHeight;
    f32 maxY;
    f32 delta;
    f32 bias;
    f64 rad;
    s32 i;
    s32 result;

    mario = marioGetPtr();
    *outHeight = float_neg1_804208e4;
    playerHeight = *(f32*)((s32)mario + 0x1BC);
    maxY = playerHeight + (f32)heightAdd;

    for (i = 0; i < 5; i++) {
        if (i == 2) {
            rad = (f64)((float_3p1416_804208e8 * (*(f32*)((s32)mario + 0x1A4) - float_90_80420924)) / float_180_804208d4);
            offset.x = float_9_8042094c * (f32)sin(rad);
            offset.z = -(float_9_8042094c * (f32)cos(rad));
            movePos(float_3_804208f8, *(f32*)((s32)mario + 0x1A4), &offset.x, &offset.z);
        } else if (i < 2) {
          if (i == 0) {
            offset.x = float_0_804208ac;
            offset.z = float_0_804208ac;
          } else if (i > -1) {
            rad = (f64)((float_3p1416_804208e8 * (float_90_80420924 + *(f32*)((s32)mario + 0x1A4))) / float_180_804208d4);
            offset.x = float_9_8042094c * (f32)sin(rad);
            offset.z = -(float_9_8042094c * (f32)cos(rad));
            movePos(float_3_804208f8, *(f32*)((s32)mario + 0x1A4), &offset.x, &offset.z);
          }
        } else if (i == 4) {
            rad = (f64)((float_3p1416_804208e8 * (*(f32*)((s32)mario + 0x1A4) - float_90_80420924)) / float_180_804208d4);
            offset.x = float_9_8042094c * (f32)sin(rad);
            offset.z = -(float_9_8042094c * (f32)cos(rad));
            movePos(float_3_804208f8, float_180_804208d4 + *(f32*)((s32)mario + 0x1A4), &offset.x, &offset.z);
        } else if (i < 4) {
            rad = (f64)((float_3p1416_804208e8 * (float_90_80420924 + *(f32*)((s32)mario + 0x1A4))) / float_180_804208d4);
            offset.x = float_9_8042094c * (f32)sin(rad);
            offset.z = -(float_9_8042094c * (f32)cos(rad));
            movePos(float_3_804208f8, float_180_804208d4 + *(f32*)((s32)mario + 0x1A4), &offset.x, &offset.z);
        }

        start.x = pos[0] + offset.x;
        start.y = pos[1] + float_1_804208c4;
        start.z = pos[2] + offset.z;
        hit.start = start;
        hit.end = vec3_802c3c5c;
        hit.radius = maxY;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else if (*(u32*)marioGetPtr() & 0x01000000) {
            result = hitCheckVecFilter(&hit, chkfilterVecRoll);
        } else {
            result = hitCheckVecFilter(&hit, chkfilterVec);
        }

        if (result != 0) {
            bias = hit.normal.x >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
            normal.x = (f32)(s32)(hit.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;
            bias = hit.normal.y >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
            normal.y = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;
            bias = hit.normal.z >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
            normal.z = (f32)(s32)(hit.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;
            if (&hitPos != NULL) {
                hitPos = hit.hitPos;
            }
            if (normal.y <= pos[1] + playerHeight) {
                pos[1] = (normal.y - playerHeight) - float_1_804208c4;
                *outHeight = float_0_804208ac;
                return result;
            }
            delta = (normal.y - pos[1]) - playerHeight;
            if (*outHeight >= float_0_804208ac) {
                if (delta < *outHeight) {
                    *outHeight = delta;
                }
            } else {
                *outHeight = delta;
            }
        }
    }
    return 0;
}

void* marioSearchHead_jabara(f64 height, f32* position, f32* distance, s32* side) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern s32 marioHitCheck2(void*, void*, f32*, void*, f32*);
    extern Vec vec3_802c3c68;
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg1_804208e4;
    extern f32 float_3p1416_804208e8;
    extern f32 float_180_804208d4;
    extern f32 float_neg23p7_80420944;
    extern f32 float_5p8_80420948;
    u8* player = marioGetPtr();
    Vec start;
    Vec end;
    Vec normal;
    Vec hitPos;
    void* hit;
    f32 offset = float_neg23p7_80420944;
    f32 baseDistance = *distance;
    f32 step = float_5p8_80420948;
    f32 pi = float_3p1416_804208e8;
    f32 divisor = float_180_804208d4;
    f32 half = float_0p5_804208cc;
    s32 i;

    *side = 0;
    *distance = float_neg1_804208e4;
    {
        Vec* src = &vec3_802c3c68;
        end.x = src->x;
        end.y = src->y;
        end.z = src->z;
    }
    i = 0;
    do {
        f32 radius = (f32)((f64)baseDistance + height);
        start.x = position[0] + offset * (f32)sin(pi * *(f32*)(player + 0x19C) / divisor);
        start.z = position[2] - offset * (f32)cos(pi * *(f32*)(player + 0x19C) / divisor);
        start.y = position[1] + half;
        hit = (void*)marioHitCheck2(&start, &end, (f32*)&normal, &hitPos, &radius);
        if (hit != 0) {
            u32 attr = hitGetAttr(hit);
            if ((attr & 0x40000) == 0) {
                if (i == 3) {
                    if (hitPos.y <= (f32)((f64)position[1] + baseDistance)) {
                        position[1] = hitPos.y;
                        *distance = float_0_804208ac;
                        *side = 0;
                        return hit;
                    }
                    *distance = (f32)((f64)(hitPos.y - position[1]) - baseDistance);
                }
            } else {
                if (hitPos.y <= position[1] + baseDistance) {
                    position[1] = hitPos.y;
                    *distance = float_0_804208ac;
                    *side = start.z - position[2] >= float_0_804208ac ? 1 : -1;
                    return hit;
                }
                *distance = (f32)((f64)(hitPos.y - position[1]) - baseDistance);
            }
        }
        i++;
        offset += step;
    } while (i < 8);
    return 0;
}

void marioCheckWall(f64 speed, f64 angle) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void dou10_yoko_yari(f32*);
    extern void* searchFrontWall(f64, f64, f64, Vec*, f32*);
    extern char* hitGetName(void*);
    extern s32 strncmp(const char*, const char*, u32);
    extern void movePos(f64, f64, f32*, f32*);
    extern s32 marioChkWallAround(f64, f64, f64, f64, Vec*, s32);
    extern char str_dokan_8042093c[];
    extern f32 float_0_804208ac;
    extern f32 float_1000_804208a8;
    u8* player = marioGetPtr();
    Vec pos;
    void* hit;
    u32 attr;
    f32 distance;
    s32 limit;

    *(f32*)(player + 0x214) = float_1000_804208a8;
    if (speed == float_0_804208ac || *(s16*)(player + 0x50) != 0) {
        *(f32*)(player + 0x1CC) = float_0_804208ac;
    } else {
        if (*(f32*)(player + 0x1CC) < (f32)speed) {
            *(f32*)(player + 0x1CC) = (f32)speed;
            *(f32*)(player + 0x1D0) = (f32)angle;
            *(s32*)(player + 0x1D4) = 14;
        } else if (--*(s32*)(player + 0x1D4) < 1) {
            *(s32*)(player + 0x1D4) = 0;
            *(f32*)(player + 0x1CC) = (f32)speed;
            *(f32*)(player + 0x1D0) = (f32)angle;
        }
        *(f32*)(player + 0x1A0) = *(f32*)(player + 0x1D0);
    }

    if (speed == float_0_804208ac) {
        marioSearchFrontWall_staying(angle);
        dou10_yoko_yari((f32*)(player + 0x214));
        return;
    }

    pos = *(Vec*)(player + 0x8C);
    *(void**)(player + 0x210) = 0;
    hit = searchFrontWall(speed, angle, *(f32*)(player + 0x1C0), &pos, &distance);
    if (hit == 0) {
        *(Vec*)(player + 0x8C) = pos;
    } else {
        *(void**)(player + 0x1E4) = *(void**)(player + 0x210) != 0 ?
                                    *(void**)(player + 0x210) : hit;
        attr = hitGetAttr(hit);
        if (((attr & 0x80000) != 0 && (*(u32*)player & 0x01000000) == 0) ||
            ((attr & 0x80) != 0 && *(u16*)(player + 0x2E) != 0x19) ||
            ((attr & 0x80) == 0 && ((attr >> 23) & 1) == 0)) {
            if (speed != float_0_804208ac) {
                *(void**)(player + 0x1F0) = hit;
                *(void**)(player + 0x1F4) = hit;
            }
            movePos(speed, angle, (f32*)(player + 0x8C), (f32*)(player + 0x94));
        } else {
            *(f32*)(player + 0x214) = distance;
            *(void**)(player + 0x1F4) = hit;
            limit = strncmp(hitGetName(hit), str_dokan_8042093c, 5) == 0 ? 10 : 20;
            *(s8*)(player + 0x3F) += 1;
            if (*(s8*)(player + 0x3F) >= limit) {
                *(s8*)(player + 0x3F) = (s8)limit;
                attr = hitGetAttr(hit);
                if ((attr & 0x80) == 0 || *(u16*)(player + 0x2E) == 0x19) {
                    *(void**)(player + 0x1F0) = hit;
                }
            }
            *(Vec*)(player + 0x8C) = pos;
        }
    }
    marioChkWallAround(speed, angle, *(f32*)(player + 0x1C0),
                       *(f32*)(player + 0x1BC), (Vec*)(player + 0x8C), 0);
    dou10_yoko_yari((f32*)(player + 0x214));
}

void marioSearchFrontWall_staying(f64 angle) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* searchFrontWall(f64, f64, f64, Vec*, f32*);
    extern f64 toMovedir(f64);
    extern void movePos(f64, f64, f32*, f32*);
    extern s32 marioChkWallAround(f64, f64, f64, f64, Vec*, s32);
    extern void* gp;
    extern char str_dou_10_802c3d88[];
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_180_804208d4;
    u8* player = marioGetPtr();
    Vec pos;
    void* hit;
    f32 distance;
    f32 radius;
    f32 direction;

    *(s8*)(player + 0x3F) = 0;
    radius = float_0p5_804208cc * *(f32*)(player + 0x1B8);
    if ((*(u32*)player & 0x00100000) != 0) {
        radius = float_0_804208ac;
    }

    pos = *(Vec*)(player + 0x8C);
    hit = searchFrontWall(radius,
                          *(f32*)(player + 0x1A4) - *(f32*)(player + 0x19C),
                          *(f32*)(player + 0x1C0), &pos, &distance);
    *(void**)(player + 0x1E4) = *(u16*)(player + 0x2E) == 0x13 ? 0 : hit;

    if (hit == 0) {
        direction = (f32)toMovedir(*(f32*)(player + 0x1AC));
        pos = *(Vec*)(player + 0x8C);
        hit = searchFrontWall(radius, direction, *(f32*)(player + 0x1C0), &pos, &distance);
        *(void**)(player + 0x1E4) = *(u16*)(player + 0x2E) == 0x13 ? 0 : hit;
    } else if (strcmp((char*)gp + 0x12C, str_dou_10_802c3d88) == 0 &&
               vivianGetStatus() != 2 && vivianGetStatus() != 1 &&
               vivianGetStatus() != 3) {
        direction = (f32)toMovedir(*(f32*)(player + 0x1AC));
        pos = *(Vec*)(player + 0x8C);
        movePos(float_0p5_804208cc * *(f32*)(player + 0x1B8), direction,
                &pos.x, &pos.z);
        hit = searchFrontWall(*(f32*)(player + 0x1B8),
                              direction + float_180_804208d4,
                              *(f32*)(player + 0x1C0), &pos, &radius);
        if (hit != 0 && radius <= *(f32*)(player + 0x1B8)) {
            *(void**)(player + 0x1E4) = hit;
            distance = float_0p5_804208cc * radius;
        }
    }

    if (*(void**)(player + 0x1E4) != 0) {
        *(f32*)(player + 0x214) = distance;
    }
    if ((*(u16*)(player + 0x24C) & 0x100) != 0) {
        *(void**)(player + 0x1E0) = 0;
        pos = *(Vec*)(player + 0x8C);
        if (searchFrontWall(radius, *(f32*)(player + 0x1A4),
                            *(f32*)(player + 0x1C0), &pos, &distance) != 0) {
            *(void**)(player + 0x1E0) = *(void**)(player + 0x1E4);
        }
    }

    pos = *(Vec*)(player + 0x8C);
    marioChkWallAround(float_0_804208ac, angle, *(f32*)(player + 0x1C0),
                       *(f32*)(player + 0x1BC), &pos, 0);
    *(f32*)(player + 0x8C) = pos.x;
    *(f32*)(player + 0x94) = pos.z;
}

void marioCheckWall_roll(f64 speed, f64 angle) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern u8 marioSearchFrontWall_staying(void);
    extern u8 searchFrontWall(s64, s64, s64, void*, s32);
    extern void movePos(f64, f64, void*, void*);
    extern s32 marioChkWallAround(void*, s32, f32, f32, f32);
    extern Vec vec3_802c3c74;
    extern f32 float_0_804208ac;
    extern f32 float_1000_804208a8;
    extern f32 float_30_80420928;
    extern f32 float_16p8_80420938;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;

    void* mario;
    void* hit;
    HitCheckArg check;
    Vec pos;
    f32 rounded;
    f32 bias;
    u32 attr;

    mario = marioGetPtr();
    *(f32*)((s32)mario + 0x214) = float_1000_804208a8;

    if (speed == float_0_804208ac || *(s16*)((s32)mario + 0x50) != 0) {
        *(f32*)((s32)mario + 0x1CC) = float_0_804208ac;
    } else if (*(f32*)((s32)mario + 0x1CC) >= (f32)speed) {
        *(s32*)((s32)mario + 0x1D4) = *(s32*)((s32)mario + 0x1D4) - 1;
        if (*(s32*)((s32)mario + 0x1D4) < 1) {
            *(s32*)((s32)mario + 0x1D4) = 0;
            *(f32*)((s32)mario + 0x1CC) = (f32)speed;
            *(f32*)((s32)mario + 0x1D0) = (f32)angle;
        }
    } else {
        *(f32*)((s32)mario + 0x1CC) = (f32)speed;
        *(f32*)((s32)mario + 0x1D0) = (f32)angle;
        *(s32*)((s32)mario + 0x1D4) = 0xE;
    }

    if (speed != float_0_804208ac && *(s16*)((s32)mario + 0x50) == 0) {
        *(f32*)((s32)mario + 0x1A0) = *(f32*)((s32)mario + 0x1D0);
    }

    if ((*(u32*)mario & 0x10000) != 0) {
        check.start = *(Vec*)((s32)mario + 0x8C);
        check.end = vec3_802c3c74;
        check.radius = float_30_80420928;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
        } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
        } else {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
        }

        if (hit != 0) {
            bias = float_neg0p5_804208d0;
            if (check.radius >= float_0_804208ac) {
                bias = float_0p5_804208cc;
            }
            rounded = (f32)(s32)(check.radius * float_1000_804208a8 + bias) / float_1000_804208a8;

            bias = float_neg0p5_804208d0;
            if (check.normal.x >= float_0_804208ac) {
                bias = float_0p5_804208cc;
            }
            check.normal.x = (f32)(s32)(check.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;

            bias = float_neg0p5_804208d0;
            if (check.normal.y >= float_0_804208ac) {
                bias = float_0p5_804208cc;
            }
            check.normal.y = (f32)(s32)(check.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;

            bias = float_neg0p5_804208d0;
            if (check.normal.z >= float_0_804208ac) {
                bias = float_0p5_804208cc;
            }
            check.normal.z = (f32)(s32)(check.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;

            if (rounded > float_16p8_80420938) {
                hit = 0;
            }
        }

        *(void**)((s32)mario + 0x1F8) = hit;
        if (hit != 0) {
            *(f32*)((s32)mario + 0x90) = check.normal.y - *(f32*)((s32)mario + 0x1BC);
        }
    }

    if (speed == float_0_804208ac) {
        ((void (*)(f64))marioSearchFrontWall_staying)(angle);
        return;
    }

    if (*(void**)((s32)mario + 0x1F8) != 0) {
        *(f32*)((s32)mario + 0x180) = float_0_804208ac;
        *(f32*)((s32)mario + 0x7C) = float_0_804208ac;
        *(f32*)((s32)mario + 0x80) = float_0_804208ac;
        *(f32*)((s32)mario + 0x84) = float_0_804208ac;
        *(f32*)((s32)mario + 0x88) = float_0_804208ac;
        speed = float_0_804208ac;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    rounded = float_0_804208ac;
    hit = ((void* (*)(f64, f64, f64, void*, void*))searchFrontWall)(speed, angle, angle, &pos, &rounded);
    if (hit == 0) {
        *(Vec*)((s32)mario + 0x8C) = pos;
    } else {
        *(void**)((s32)mario + 0x1E4) = hit;
        attr = hitGetAttr(hit);
        if ((attr & 0x80000) == 0 || (*(u32*)marioGetPtr() & 0x01000000) != 0) {
            if ((attr & 0x80) == 0) {
                attr = (attr >> 0x17) & 1;
            } else {
                attr = (*(u16*)((s32)mario + 0x2E) == 0x19);
            }
        } else {
            attr = 0;
        }

        if (attr == 0) {
            *(f32*)((s32)mario + 0x214) = rounded;
            *(void**)((s32)mario + 0x1FC) = hit;
            *(s8*)((s32)mario + 0x3F) = *(s8*)((s32)mario + 0x3F) + 1;
            if (*(s8*)((s32)mario + 0x3F) > 0x13) {
                *(s8*)((s32)mario + 0x3F) = 0x14;
                attr = hitGetAttr(hit);
                if ((attr & 0x80) == 0 || *(u16*)((s32)marioGetPtr() + 0x2E) == 0x19) {
                    *(void**)((s32)mario + 0x1F0) = hit;
                }
            }
            *(Vec*)((s32)mario + 0x8C) = pos;
        } else {
            if (speed != float_0_804208ac) {
                *(void**)((s32)mario + 0x1F0) = hit;
                *(void**)((s32)mario + 0x1FC) = hit;
            }
            movePos(speed, angle, (void*)((s32)mario + 0x8C), (void*)((s32)mario + 0x94));
        }
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    ((s32 (*)(f64, f64, f64, f64, void*, s32))marioChkWallAround)(speed, angle, speed, angle, &pos, 0);
    *(f32*)((s32)mario + 0x8C) = pos.x;
    *(f32*)((s32)mario + 0x94) = pos.z;
}

s32 marioChkFront(f64 angle, f64 maxDist, f32* outDist, f32* pos) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern f64 distABf(f64, f64, f64, f64);
    extern Vec vec3_802c3c80;
    extern f32 float_0_804208ac;
    extern f32 float_1000_804208a8;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg1_804208e4;

    HitCheckArg hit;
    Vec hitPos;
    f32 s;
    f32 c;
    f32 roundedDist;
    f32 roundBias;
    s32 result;

    sincosf((f32)angle, &c, &s);
    hit.start = *(Vec*)pos;
    hit.end = vec3_802c3c80;
    hit.end.x = c;
    hit.end.z = s;
    hit.radius = (f32)maxDist;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if (*(u32*)marioGetPtr() & 0x01000000) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        roundedDist = hit.radius;
        roundBias = float_neg0p5_804208d0;
        if (roundedDist >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        roundedDist = (f32)(s32)(roundedDist * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.x >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.x = (f32)(s32)(hit.normal.x * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.y >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.y = (f32)(s32)(hit.normal.y * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.z >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.z = (f32)(s32)(hit.normal.z * float_1000_804208a8 + roundBias) / float_1000_804208a8;
        hitPos = hit.hitPos;
    }

    if (result == 0 || maxDist < roundedDist) {
        *outDist = float_neg1_804208e4;
        return 0;
    }

    *outDist = (f32)distABf(pos[0], pos[2], hit.normal.x, hit.normal.z);
    return result;
}

void* searchFrontWall(f64 speed, f64 angle, f64 radius, void* position, s32 outParam) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern s32 vivianGetStatus(void);
    extern void sincosf(f32, f32*, f32*);
    extern s32 marioChkSts(u32);
    extern s32 strcmp(const char*, const char*);
    extern s32 kpaGetLevel(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern f64 partyGetHeight(void*);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, s32);
    extern s32 chkfilterVecRoll(s32, s32);
    extern s32 chkfilterVec(s32, s32);
    extern s32 chkBalloon(void*);
    extern f64 fabs(f64);
    extern void* gp;
    extern char str_eki_04_802c3d90[];
    extern f32 float_0_804208ac;
    extern f32 float_0p1_804208ec;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_0p75_804208e0;
    extern f32 float_7_80420934;
    extern f32 float_11_804208b4;
    extern f32 float_12_80420930;
    extern f32 float_26p64_8042092c;
    extern f32 float_30_80420928;
    extern f32 float_45_804208bc;
    extern f32 float_1000_804208a8;
    u8* player = marioGetPtr();
    Vec* pos = position;
    HitCheckArg check;
    f32 heights[4];
    f32 sinv;
    f32 cosv;
    f32 rightSin;
    f32 rightCos;
    f32 leftSin;
    f32 leftCos;
    f32 bestDistance;
    f32 sideDistance;
    f32 probeRadius;
    f32 correctionX = 0.0f;
    f32 correctionZ = 0.0f;
    void* hit;
    void* bestHit = 0;
    s32 count = 2;
    s32 rightBlocked = 0;
    s32 leftBlocked = 0;
    s32 i;

    if (vivianGetStatus() == 2) {
        return 0;
    }
    sincosf((f32)angle, &sinv, &cosv);
    if (marioChkSts(0x10) != 0 &&
        strcmp((char*)gp + 0x12C, str_eki_04_802c3d90) == 0 &&
        ((f32)fabs(*(f32*)(player + 0x21C)) >= float_45_804208bc ||
         (f32)fabs(*(f32*)(player + 0x224)) >= float_45_804208bc)) {
        pos->x += (f32)speed * sinv;
        pos->z += (f32)speed * cosv;
        return 0;
    }

    if (*(s8*)(player + 0x3C) == 2) {
        if ((*(u32*)(player + 0x14) & 1) == 0) {
            heights[0] = float_0p75_804208e0 * *(f32*)(player + 0x1BC);
            heights[1] = float_11_804208b4;
        } else {
            heights[0] = float_0p75_804208e0 * *(f32*)(player + 0x1BC);
            heights[1] = float_0p5_804208cc * *(f32*)(player + 0x1BC);
            heights[2] = float_11_804208b4;
            count = 3;
            if (kpaGetLevel() == 2) {
                heights[3] = float_30_80420928;
                count = 4;
            }
        }
    } else if (*(u16*)(player + 0x2E) == 0x1A) {
        f32 height = *(f32*)(player + 0x1BC);
        void* party;

        player = marioGetPtr();
        if (*(u16*)(player + 0x2E) == 0x1A) {
            party = partyGetPtr(marioGetPartyId());
            if (party != 0) {
                height = (f32)partyGetHeight(party);
            }
        }
        heights[0] = float_0p75_804208e0 * height;
        heights[1] = float_11_804208b4;
    } else if ((*(u32*)player & 0x01000000) != 0) {
        heights[0] = float_12_80420930;
        heights[1] = float_11_804208b4;
        if (*(s16*)(player + 0x50) != 0) {
            heights[2] = float_7_80420934;
            count = 3;
        }
    } else {
        heights[0] = float_26p64_8042092c;
        heights[1] = float_11_804208b4;
        if (*(s16*)(player + 0x50) != 0) {
            heights[2] = float_0p1_804208ec;
            count = 3;
        }
    }

    probeRadius = float_0p5_804208cc * (f32)radius;
    bestDistance = (f32)speed + probeRadius;
    for (i = 0; i < count; i++) {
        check.start = *pos;
        check.start.y = pos->y + heights[i];
        check.end.x = sinv;
        check.end.y = float_0_804208ac;
        check.end.z = cosv;
        check.radius = bestDistance;
        if (*(u16*)(player + 0x2E) == 0x1C) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
        } else if ((*(u32*)player & 0x01000000) != 0) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
        } else {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
        }
        if (hit != 0) {
            f32 bias = check.radius >= float_0_804208ac ?
                       float_0p5_804208cc : float_neg0p5_804208d0;
            f32 distance = (f32)(s32)(check.radius * float_1000_804208a8 + bias) /
                           float_1000_804208a8;
            bias = check.normal.x >= float_0_804208ac ?
                   float_0p5_804208cc : float_neg0p5_804208d0;
            check.normal.x = (f32)(s32)(check.normal.x * float_1000_804208a8 + bias) /
                             float_1000_804208a8;
            bias = check.normal.y >= float_0_804208ac ?
                   float_0p5_804208cc : float_neg0p5_804208d0;
            check.normal.y = (f32)(s32)(check.normal.y * float_1000_804208a8 + bias) /
                             float_1000_804208a8;
            bias = check.normal.z >= float_0_804208ac ?
                   float_0p5_804208cc : float_neg0p5_804208d0;
            check.normal.z = (f32)(s32)(check.normal.z * float_1000_804208a8 + bias) /
                             float_1000_804208a8;
            if (chkBalloon(hit) != 0) {
                *(void**)(player + 0x210) = hit;
            }
            if (bestHit == 0 || distance < bestDistance) {
                bestHit = hit;
                bestDistance = distance;
                correctionX = float_0p5_804208cc *
                    -((speed * sinv * check.hitPos.x + speed * cosv * check.hitPos.z) *
                      check.hitPos.x - speed * sinv);
                correctionZ = float_0p5_804208cc *
                    -((speed * sinv * check.hitPos.x + speed * cosv * check.hitPos.z) *
                      check.hitPos.z - speed * cosv);
            }
        }
    }

    sincosf((f32)angle + float_45_804208bc, &rightSin, &rightCos);
    sideDistance = probeRadius;
    for (i = 0; i < count; i++) {
        check.start = *pos;
        check.start.y = pos->y + heights[i];
        check.end.x = rightSin;
        check.end.y = float_0_804208ac;
        check.end.z = rightCos;
        check.radius = probeRadius;
        if (*(u16*)(player + 0x2E) == 0x1C) {
            rightBlocked = hitCheckVecFilter(&check, chkfilterVecVivian);
        } else if ((*(u32*)player & 0x01000000) != 0) {
            rightBlocked = hitCheckVecFilter(&check, chkfilterVecRoll);
        } else {
            rightBlocked = hitCheckVecFilter(&check, chkfilterVec);
        }
        if (rightBlocked != 0) {
            f32 bias = check.radius >= float_0_804208ac ?
                       float_0p5_804208cc : float_neg0p5_804208d0;
            sideDistance = (f32)(s32)(check.radius * float_1000_804208a8 + bias) /
                           float_1000_804208a8;
            break;
        }
    }
    if (rightBlocked != 0) {
        sincosf((f32)angle - float_45_804208bc, &leftSin, &leftCos);
        for (i = 0; i < count; i++) {
            check.start = *pos;
            check.start.y = pos->y + heights[i];
            check.end.x = leftSin;
            check.end.y = float_0_804208ac;
            check.end.z = leftCos;
            check.radius = probeRadius;
            if (*(u16*)(player + 0x2E) == 0x1C) {
                leftBlocked = hitCheckVecFilter(&check, chkfilterVecVivian);
            } else if ((*(u32*)player & 0x01000000) != 0) {
                leftBlocked = hitCheckVecFilter(&check, chkfilterVecRoll);
            } else {
                leftBlocked = hitCheckVecFilter(&check, chkfilterVec);
            }
        }
    }

    if (bestHit == 0) {
        if (leftBlocked == 0) {
            pos->x += (f32)speed * sinv;
            pos->z += (f32)speed * cosv;
        }
    } else if (leftBlocked == 0) {
        *(f32*)outParam = sideDistance;
        pos->x += correctionX;
        pos->z += correctionZ;
    } else {
        *(f32*)outParam = float_0_804208ac;
    }
    return bestHit;
}

void* dou10_yoko_yari(float* outSpeed) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern s32 strcmp(char*, char*);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern void movePos(f64, f64, void*, void*);
    extern void* gp;
    extern char str_dou_10_802c3d88[];
    extern Vec vec3_802c3cb0;
    extern f32 float_2_804208f0;
    extern f32 float_60_80420910;
    extern f32 float_100_80420900;
    extern f32 float_0p8_80420914;
    extern f32 float_20_80420918;
    extern f32 float_8_8042091c;
    extern f32 float_270_80420920;
    extern f32 float_90_80420924;
    extern f32 float_0_804208ac;

    void* mario;
    void* hit;
    HitCheckArg check;
    Vec pos;
    f32 sideOffset;
    u8 temp[4];

    mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) == 0x16) {
        return 0;
    }
    toMovedir(*(f32*)((s32)mario + 0x1AC));
    hit = *(void**)((s32)mario + 0x1E4);
    if (hit != 0) {
        return hit;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    if (strcmp((char*)((s32)gp + 0x12C), str_dou_10_802c3d88) != 0) {
        return hit;
    }
    if (vivianGetStatus() == 2 || vivianGetStatus() == 1 || vivianGetStatus() == 3) {
        return hit;
    }

    if (*(u32*)mario & 0x100000) {
        sideOffset = float_2_804208f0;
    } else {
        sideOffset = float_8_8042091c;
    }

    check.start = pos;
    check.start.y += float_60_80420910;
    check.end = vec3_802c3cb0;
    check.radius = float_100_80420900;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
    } else if (*(u32*)marioGetPtr() & 0x01000000) {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
    } else {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
    }
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    movePos(sideOffset, float_270_80420920, &pos, &pos.z);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    movePos(sideOffset, float_90_80420924, &pos, &pos.z);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    return hit;
}

int dou10_yoko_yari2(float* outSpeed) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern s32 strcmp(char*, char*);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern void movePos(f64, f64, void*, void*);
    extern void* gp;
    extern char str_dou_10_802c3d88[];
    extern Vec vec3_802c3cbc;
    extern f32 float_60_80420910;
    extern f32 float_100_80420900;
    extern f32 float_0p8_80420914;
    extern f32 float_20_80420918;
    extern f32 float_8_8042091c;
    extern f32 float_270_80420920;
    extern f32 float_90_80420924;
    extern f32 float_0_804208ac;

    void* mario;
    void* hit;
    HitCheckArg check;
    Vec pos;
    u8 temp[4];

    mario = marioGetPtr();
    toMovedir(*(f32*)((s32)mario + 0x1AC));
    pos = *(Vec*)((s32)mario + 0x8C);
    if (strcmp((char*)((s32)gp + 0x12C), str_dou_10_802c3d88) != 0) {
        return 0;
    }

    check.start = pos;
    check.start.y += float_60_80420910;
    check.end = vec3_802c3cbc;
    check.radius = float_100_80420900;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
    } else if (*(u32*)marioGetPtr() & 0x01000000) {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
    } else {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
    }
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    movePos(float_8_8042091c, float_270_80420920, &pos, &pos.z);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    movePos(float_8_8042091c, float_90_80420924, &pos, &pos.z);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    return (s32)hit;
}

void* N_dou10_yoko_yari3(void) {
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    typedef struct HitCheckArgLocal {
        u8 pad[0xC];
        VecLocal start;
        VecLocal end;
        VecLocal normal;
        VecLocal hitPos;
        f32 radius;
    } HitCheckArgLocal;
    extern void* gp;
    extern char str_dou_10_802c3d88[];
    extern void movePos(f64, f64, f32*, f32*);
    extern void* hitCheckVecFilter(void*, void*);
    extern f32 float_10_80420904;
    extern f32 float_neg10_8042090c;
    extern f32 float_5_804208c0;
    extern f32 float_0_804208ac;
    extern f32 float_40_80420908;
    extern const VecLocal vec3_802c3b78;
    const u8* catalog = (const u8*)&vec3_802c3b78;
    u8* player = marioGetPtr();
    HitCheckArgLocal check;
    VecLocal start1;
    VecLocal end1;
    VecLocal start2;
    VecLocal end2;
    f32 x;
    f32 y;
    f32 z;
    f32 sinv;
    f32 cosv;
    void* hit;

    if (strcmp((char*)gp + 0x12C, str_dou_10_802c3d88) != 0) {
        return 0;
    }
    x = *(f32*)(player + 0x8C);
    y = *(f32*)(player + 0x90);
    z = *(f32*)(player + 0x94);
    movePos(float_10_80420904, *(f32*)(player + 0x1A4), &x, &z);
    start1 = *(const VecLocal*)(catalog + 0x150);
    start1.x = x;
    start1.y = y + float_5_804208c0;
    start1.z = z;
    check.start = start1;
    end1 = *(const VecLocal*)(catalog + 0x15C);
    sincosf(float_0_804208ac, &sinv, &cosv);
    end1.x = sinv;
    end1.z = cosv;
    check.end = end1;
    check.radius = float_40_80420908;
    hit = hitCheckVecFilter(&check, 0);
    if (hit == 0) {
        x = *(f32*)(player + 0x8C);
        z = *(f32*)(player + 0x94);
        movePos(float_neg10_8042090c, *(f32*)(player + 0x1A4), &x, &z);
        start2 = *(const VecLocal*)(catalog + 0x168);
        start2.x = x;
        start2.y = y + float_5_804208c0;
        start2.z = z;
        check.start = start2;
        end2 = *(const VecLocal*)(catalog + 0x174);
        sincosf(float_0_804208ac, &sinv, &cosv);
        end2.x = sinv;
        end2.z = cosv;
        check.end = end2;
        check.radius = float_40_80420908;
        hit = hitCheckVecFilter(&check, 0);
    }
    if (hit != 0) {
        *(void**)(player + 0x1E4) = hit;
    }
    return hit;
}

s32 marioChkFrontStep(f32* outY, f32* outDrop, f32* outAngle) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern f64 revise360(f64);
    extern f64 __frsqrte(f64);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern Vec vec3_802c3b78;
    extern Vec vec3_802c3d04;
    extern f32 float_0_804208ac;
    extern f32 float_0p1_804208ec;
    extern f32 float_0p265_804208fc;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg1_804208e4;
    extern f32 float_2_804208f0;
    extern f32 float_3_804208f8;
    extern f32 float_3p1416_804208e8;
    extern f32 float_11_804208b4;
    extern f32 float_18p5_804208f4;
    extern f32 float_100_80420900;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;
    extern f64 double_0p5_802c3d70;
    extern f64 double_3_802c3d78;
    extern f64 double_0_802c3d80;
    extern f32 __float_nan;

    void* player;
    u8* vecBase;
    HitCheckArg hit;
    f32 height;
    f32 reach;
    f32 dir;
    f32 rad;
    f32 sx;
    f32 cz;
    f32 bias;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 flatLen;
    f64 lengthSq;
    f64 invLength;
    union {
        f32 f;
        u32 u;
    } classifiedLength;
    u32 result;

    vecBase = (u8*)&vec3_802c3b78;
    player = marioGetPtr();
    {
        f32 zero = float_0_804208ac;
        *(f32*)((s32)player + 0x21C) = zero;
        *(f32*)((s32)player + 0x220) = zero;
        *(f32*)((s32)player + 0x224) = zero;
        *(f32*)((s32)player + 0x218) = zero;
    }

    if ((*(u32*)player & 0x01000000) != 0) {
        height = float_18p5_804208f4;
    } else {
        height = float_0p5_804208cc * *(f32*)((s32)player + 0x1BC);
    }

    *outY = *(f32*)((s32)player + 0x90);
    *outDrop = float_neg1_804208e4;

    if (*(f32*)((s32)player + 0x180) != float_0_804208ac) {
        dir = *(f32*)((s32)player + 0x1A4);
    } else {
        dir = toMovedir(*(f32*)((s32)player + 0x1AC));
    }
    dir = (f32)revise360((f32)(s32)(float_0p5_804208cc + dir));

    if ((*(u32*)player & 0x01000000) != 0) {
        reach = float_3_804208f8;
    } else {
        reach = float_0p265_804208fc * *(f32*)((s32)player + 0x1B8);
    }

    rad = (float_3p1416_804208e8 * dir) / float_180_804208d4;
    sx = (f32)sin(rad);
    cz = (f32)cos(rad);

    hit.start = *(Vec*)(vecBase + 0x180);
    hit.start.x = *(f32*)((s32)player + 0x8C) + reach * sx;
    hit.start.y = *(f32*)((s32)player + 0x90) + height;
    hit.start.z = *(f32*)((s32)player + 0x94) + reach * -cz;
    hit.end = *(Vec*)(vecBase + 0x18C);
    hit.radius = float_0p1_804208ec + float_11_804208b4 + height;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result == 0) {
        return 0;
    }

    bias = float_neg0p5_804208d0;
    if (hit.radius >= float_0_804208ac) {
        bias = float_0p5_804208cc;
    }
    hit.radius = (f32)(s32)(hit.radius * float_1000_804208a8 + bias) / float_1000_804208a8;

    bias = float_neg0p5_804208d0;
    if (hit.normal.x >= float_0_804208ac) {
        bias = float_0p5_804208cc;
    }
    nx = (f32)(s32)(hit.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;

    bias = float_neg0p5_804208d0;
    if (hit.normal.y >= float_0_804208ac) {
        bias = float_0p5_804208cc;
    }
    ny = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;

    bias = float_neg0p5_804208d0;
    if (hit.normal.z >= float_0_804208ac) {
        bias = float_0p5_804208cc;
    }
    nz = (f32)(s32)(hit.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;

    hx = hit.hitPos.x;
    hy = hit.hitPos.y;
    hz = hit.hitPos.z;

    *(f32*)((s32)player + 0x21C) = (f32)(s32)(float_100_80420900 *
        ((f32)angleABf(float_0_804208ac, float_0_804208ac, float_100_80420900 * hz, float_100_80420900 * hy) - float_180_804208d4) +
        float_0p5_804208cc) / float_100_80420900;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = (f32)(s32)(float_100_80420900 *
        ((f32)angleABf(float_0_804208ac, float_0_804208ac, float_100_80420900 * hx, float_100_80420900 * hy) - float_180_804208d4) +
        float_0p5_804208cc) / float_100_80420900;

    lengthSq = hx * hx + hz * hz;
    if (lengthSq > *(f64*)(vecBase + 0x208)) {
        invLength = __frsqrte(lengthSq);
        invLength = *(f64*)(vecBase + 0x1F8) * invLength *
            (*(f64*)(vecBase + 0x200) - lengthSq * invLength * invLength);
        invLength = *(f64*)(vecBase + 0x1F8) * invLength *
            (*(f64*)(vecBase + 0x200) - lengthSq * invLength * invLength);
        invLength = *(f64*)(vecBase + 0x1F8) * invLength *
            (*(f64*)(vecBase + 0x200) - lengthSq * invLength * invLength);
        flatLen = (f32)(lengthSq * invLength);
    } else if (lengthSq < *(f64*)(vecBase + 0x208)) {
        flatLen = __float_nan;
    } else {
        classifiedLength.f = (f32)lengthSq;
        if ((classifiedLength.u & 0x7F800000) == 0x7F800000 &&
            (classifiedLength.u & 0x007FFFFF) != 0) {
            flatLen = __float_nan;
        } else {
            flatLen = classifiedLength.f;
        }
    }
    *(f32*)((s32)player + 0x218) = (f32)angleABf(float_0_804208ac, float_0_804208ac, flatLen, -hy);

    if (*(f32*)((s32)player + 0x90) <= ny ||
        ((ny - *(f32*)((s32)player + 0x90) < float_2_804208f0) &&
         (*(f32*)((s32)player + 0x90) - ny < float_2_804208f0))) {
        *outY = ny;
        *outDrop = float_0_804208ac;
        *outAngle = (f32)angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), nx, nz);
        return result;
    }

    *outDrop = *(f32*)((s32)player + 0x90) - ny;
    *outAngle = (f32)angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), nx, nz);
    return 0;
}

s32 marioChkWallAround(f64 speed, f64 angle, f64 radius, f64 height,
                       void* position, s32 mode) {
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    typedef struct HitCheckArgLocal {
        u8 pad[0xC];
        VecLocal start;
        VecLocal end;
        VecLocal normal;
        VecLocal hitPos;
        f32 radius;
    } HitCheckArgLocal;
    extern s32 kpaGetStageViewType(void);
    extern void* hitCheckVecFilter(void*, void*);
    extern f32 PSVECMag(void*);
    extern void PSVECScale(void*, void*, f32);
    extern void PSVECAdd(void*, void*, void*);
    extern f32 PSVECDotProduct(void*, void*);
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_1_804208c4;
    extern f32 float_11_804208b4;
    extern f32 float_360_804208c8;
    extern f32 float_180_804208d4;
    extern f32 float_0p125_804208d8;
    extern f32 float_0p25_804208dc;
    extern f32 float_1000_804208a8;
    extern f32 float_neg0p5_804208d0;
    VecLocal* pos = position;
    VecLocal correction = { 0.0f, 0.0f, 0.0f };
    VecLocal firstCorrection;
    VecLocal secondPos;
    HitCheckArgLocal low;
    HitCheckArgLocal high;
    void* lowHit;
    void* highHit;
    f32 lowDist;
    f32 highDist;
    f32 sinv;
    f32 cosv;
    f32 step;
    f32 probeRadius;
    f32 limit;
    s32 count;
    s32 collided = 0;
    s32 i;

    count = kpaGetStageViewType() == 0 ? 2 : 8;
    step = float_360_804208c8 / (f32)count;
    probeRadius = (f32)radius * float_0p5_804208cc;

    for (i = 0; i < count; i++) {
        u32 attr;
        sincosf((f32)height, &sinv, &cosv);
        low.start.x = pos->x;
        low.start.y = pos->y + (f32)angle - float_1_804208c4;
        low.start.z = pos->z;
        low.end.x = sinv;
        low.end.y = float_0_804208ac;
        low.end.z = cosv;
        low.radius = probeRadius;
        high = low;
        high.start.y = pos->y + float_11_804208b4;

        if (*(u16*)((u8*)marioGetPtr() + 0x2E) == 0x1C) {
            lowHit = hitCheckVecFilter(&low, chkfilterVecVivian);
            highHit = hitCheckVecFilter(&high, chkfilterVecVivian);
        } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
            lowHit = hitCheckVecFilter(&low, chkfilterVecRoll);
            highHit = hitCheckVecFilter(&high, chkfilterVecRoll);
        } else {
            lowHit = hitCheckVecFilter(&low, chkfilterVec);
            highHit = hitCheckVecFilter(&high, chkfilterVec);
        }
        lowDist = lowHit != 0 ?
            (f32)(s32)(low.radius * float_1000_804208a8 +
                (low.radius >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0)) /
                float_1000_804208a8 : probeRadius;
        highDist = highHit != 0 ?
            (f32)(s32)(high.radius * float_1000_804208a8 +
                (high.radius >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0)) /
                float_1000_804208a8 : probeRadius;

        if (lowHit != 0) {
            void* player;
            s32 invalid;
            attr = hitGetAttr(lowHit);
            player = marioGetPtr();
            if (((attr & 0x80000) != 0) && ((*(u32*)player & 0x01000000) == 0)) {
                invalid = 0;
            } else if ((attr & 0x80) == 0) {
                invalid = (attr >> 23) & 1;
            } else {
                invalid = *(u16*)((u8*)player + 0x2E) == 0x19;
            }
            if (invalid != 0) {
                lowHit = 0;
            }
        }
        if (highHit != 0) {
            void* player;
            s32 invalid;
            attr = hitGetAttr(highHit);
            player = marioGetPtr();
            if (((attr & 0x80000) != 0) && ((*(u32*)player & 0x01000000) == 0)) {
                invalid = 0;
            } else if ((attr & 0x80) == 0) {
                invalid = (attr >> 23) & 1;
            } else {
                invalid = *(u16*)((u8*)player + 0x2E) == 0x19;
            }
            if (invalid != 0) {
                highHit = 0;
            }
        }
        if (lowHit == 0 || highHit == 0) {
            if (highHit != 0) {
                lowHit = highHit;
                lowDist = highDist;
            }
        } else if (highDist < lowDist) {
            lowHit = highHit;
            lowDist = highDist;
        }
        if (lowHit != 0) {
            f32 distance = probeRadius - lowDist;
            if (distance < float_0_804208ac) {
                distance = -distance;
            }
            sincosf(float_180_804208d4 + (f32)height, &sinv, &cosv);
            correction.x += sinv * distance;
            correction.z += cosv * distance;
            collided = 1;
        }
        height = (f32)height + step;
    }

    if (!collided) {
        return 0;
    }
    limit = float_0p125_804208d8 * (f32)radius;
    if (mode == 0) {
        void* player = marioGetPtr();
        u16 motion = *(u16*)((u8*)player + 0x2E);
        if (motion == 0x0A || motion == 0x0B) {
            limit = float_0p25_804208dc * (f32)radius;
        }
    }
    if (limit < float_1_804208c4) {
        limit = float_1_804208c4;
    }
    firstCorrection = correction;
    {
        f32 magnitude = PSVECMag(&correction);
        if (magnitude > limit) {
            PSVECScale(&correction, &correction, limit / magnitude);
        }
    }
    PSVECAdd(pos, &correction, pos);
    secondPos = *pos;

    correction.x = 0.0f;
    correction.y = 0.0f;
    correction.z = 0.0f;
    for (i = 0; i < count; i++) {
        u32 attr;
        sincosf((f32)height, &sinv, &cosv);
        low.start.x = secondPos.x;
        low.start.y = secondPos.y + (f32)angle - float_1_804208c4;
        low.start.z = secondPos.z;
        low.end.x = sinv;
        low.end.y = float_0_804208ac;
        low.end.z = cosv;
        low.radius = probeRadius;
        high = low;
        high.start.y = secondPos.y + float_11_804208b4;
        if (*(u16*)((u8*)marioGetPtr() + 0x2E) == 0x1C) {
            lowHit = hitCheckVecFilter(&low, chkfilterVecVivian);
            highHit = hitCheckVecFilter(&high, chkfilterVecVivian);
        } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
            lowHit = hitCheckVecFilter(&low, chkfilterVecRoll);
            highHit = hitCheckVecFilter(&high, chkfilterVecRoll);
        } else {
            lowHit = hitCheckVecFilter(&low, chkfilterVec);
            highHit = hitCheckVecFilter(&high, chkfilterVec);
        }
        lowDist = lowHit != 0 ?
            (f32)(s32)(low.radius * float_1000_804208a8 +
                (low.radius >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0)) /
                float_1000_804208a8 : probeRadius;
        highDist = highHit != 0 ?
            (f32)(s32)(high.radius * float_1000_804208a8 +
                (high.radius >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0)) /
                float_1000_804208a8 : probeRadius;

        if (lowHit != 0) {
            void* player;
            s32 invalid;
            attr = hitGetAttr(lowHit);
            player = marioGetPtr();
            if (((attr & 0x80000) != 0) && ((*(u32*)player & 0x01000000) == 0)) {
                invalid = 0;
            } else if ((attr & 0x80) == 0) {
                invalid = (attr >> 23) & 1;
            } else {
                invalid = *(u16*)((u8*)player + 0x2E) == 0x19;
            }
            if (invalid != 0) {
                lowHit = 0;
            }
        }
        if (highHit != 0) {
            void* player;
            s32 invalid;
            attr = hitGetAttr(highHit);
            player = marioGetPtr();
            if (((attr & 0x80000) != 0) && ((*(u32*)player & 0x01000000) == 0)) {
                invalid = 0;
            } else if ((attr & 0x80) == 0) {
                invalid = (attr >> 23) & 1;
            } else {
                invalid = *(u16*)((u8*)player + 0x2E) == 0x19;
            }
            if (invalid != 0) {
                highHit = 0;
            }
        }
        if (lowHit == 0 || highHit == 0) {
            if (highHit != 0) {
                lowHit = highHit;
                lowDist = highDist;
            }
        } else if (highDist < lowDist) {
            lowHit = highHit;
            lowDist = highDist;
        }
        if (lowHit != 0) {
            f32 distance = probeRadius - lowDist;
            if (distance < float_0_804208ac) distance = -distance;
            sincosf(float_180_804208d4 + (f32)height, &sinv, &cosv);
            correction.x += sinv * distance;
            correction.z += cosv * distance;
        }
        height = (f32)height + step;
    }
    if (PSVECDotProduct(&firstCorrection, &correction) < float_0_804208ac) {
        PSVECScale(&correction, &correction, float_0p5_804208cc);
        PSVECAdd(pos, &correction, pos);
    }
    return 1;
}

s32 marioSlitChkWallAround(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern Vec vec3_802c3b78;
    extern f32 float_0_804208ac;
    extern f32 float_1_804208c4;
    extern f32 float_5_804208c0;
    extern f32 float_11_804208b4;
    extern f32 float_13_804208b8;
    extern f32 float_45_804208bc;

    void* player;
    HitCheckArg check;
    s32 blocked[8];
    f32 s;
    f32 c;
    f32 angle;
    f32 x;
    f32 y;
    f32 z;
    void* hit;
    u32 attr;
    s32 i;

    player = marioGetPtr();
    x = *(f32*)((s32)player + 0x8C);
    y = *(f32*)((s32)player + 0x90) + float_11_804208b4;
    z = *(f32*)((s32)player + 0x94);

    angle = float_0_804208ac;
    for (i = 0; i < 8; i++, angle += float_45_804208bc) {
        blocked[i] = 0;
        sincosf(angle, &s, &c);
        check.start.x = x;
        check.start.y = y;
        check.start.z = z;
        check.end = vec3_802c3b78;
        check.end.x = s;
        check.end.z = c;
        check.radius = float_13_804208b8;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
        } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
        } else {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
        }

        if (hit != 0) {
            attr = hitGetAttr(hit);
            if ((attr & 0x80000) == 0 || (*(u32*)marioGetPtr() & 0x01000000) != 0) {
                if ((attr & 0x80) == 0) {
                    attr = (attr >> 0x17) & 1;
                } else {
                    attr = (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x19);
                }
            } else {
                attr = 0;
            }
            if (attr == 0) {
                blocked[i] = 1;
            }
        }
    }

    y = *(f32*)((s32)player + 0x90) + float_5_804208c0;
    angle = float_0_804208ac;
    for (i = 0; i < 8; i++, angle += float_45_804208bc) {
        if (blocked[i] == 0) {
            sincosf(angle, &s, &c);
            check.start.x = x;
            check.start.y = y;
            check.start.z = z;
            check.end = vec3_802c3b78;
            check.end.x = s;
            check.end.z = c;
            check.radius = float_13_804208b8;

            if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
            } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
            } else {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
            }

            if (hit != 0) {
                attr = hitGetAttr(hit);
                if ((attr & 0x80000) == 0 || (*(u32*)marioGetPtr() & 0x01000000) != 0) {
                    if ((attr & 0x80) == 0) {
                        attr = (attr >> 0x17) & 1;
                    } else {
                        attr = (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x19);
                    }
                } else {
                    attr = 0;
                }
                if (attr == 0) {
                    blocked[i] = 1;
                }
            }
        }
    }

    y = *(f32*)((s32)player + 0x90) + (*(f32*)((s32)player + 0x1BC) - float_1_804208c4);
    angle = float_0_804208ac;
    for (i = 0; i < 8; i++, angle += float_45_804208bc) {
        if (blocked[i] == 0) {
            sincosf(angle, &s, &c);
            check.start.x = x;
            check.start.y = y;
            check.start.z = z;
            check.end = vec3_802c3b78;
            check.end.x = s;
            check.end.z = c;
            check.radius = float_13_804208b8;

            if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
            } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
            } else {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
            }

            if (hit != 0) {
                attr = hitGetAttr(hit);
                if ((attr & 0x80000) == 0 || (*(u32*)marioGetPtr() & 0x01000000) != 0) {
                    if ((attr & 0x80) == 0) {
                        attr = (attr >> 0x17) & 1;
                    } else {
                        attr = (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x19);
                    }
                } else {
                    attr = 0;
                }
                if (attr == 0) {
                    blocked[i] = 1;
                }
            }
        }
    }

    for (i = 0; i < 8; i++) {
        if (blocked[i] == 0 &&
            blocked[(i + 1) & 7] == 0 &&
            blocked[(i + 2) & 7] == 0) {
            return 0;
        }
    }

    return 1;
}

void marioCheckWallShip(f64 speed, f64 angle) {
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    extern void* searchFrontWall(f64, f64, f64, void*, f32*);
    extern void movePos(f64, f64, f32*, f32*);
    extern s32 marioChkWallAround(f64, f64, f64, f64, void*, s32);
    extern void PSVECSubtract(void*, void*, void*);
    extern f32 PSVECMag(void*);
    extern void PSVECScale(void*, void*, f32);
    extern void PSVECAdd(void*, void*, void*);
    extern f32 float_0_804208ac;
    extern f32 float_1000_804208a8;
    u8* player = marioGetPtr();
    VecLocal oldPos;
    VecLocal delta;
    f32 wallDistance;
    void* hit;
    u32 attr;
    s32 passable;

    *(f32*)(player + 0x214) = float_1000_804208a8;
    if (speed == float_0_804208ac || *(s16*)(player + 0x50) != 0) {
        *(f32*)(player + 0x1CC) = float_0_804208ac;
    } else if (speed > *(f32*)(player + 0x1CC)) {
        *(f32*)(player + 0x1CC) = (f32)speed;
        *(f32*)(player + 0x1D0) = (f32)angle;
        *(s32*)(player + 0x1D4) = 14;
    } else {
        *(s32*)(player + 0x1D4) -= 1;
        if (*(s32*)(player + 0x1D4) < 1) {
            *(s32*)(player + 0x1D4) = 0;
            *(f32*)(player + 0x1CC) = (f32)speed;
            *(f32*)(player + 0x1D0) = (f32)angle;
        }
    }
    if (speed != float_0_804208ac && *(s16*)(player + 0x50) == 0) {
        *(f32*)(player + 0x1A0) = *(f32*)(player + 0x1D0);
    }
    if (speed == float_0_804208ac) {
        marioSearchFrontWall_staying(angle);
        return;
    }

    oldPos = *(VecLocal*)(player + 0x8C);
    hit = searchFrontWall(speed, angle, *(f32*)(player + 0x1C0), &oldPos, &wallDistance);
    if (hit != 0) {
        *(void**)(player + 0x1E4) = hit;
        attr = hitGetAttr(hit);
        passable = 0;
        if ((attr & 0x80000) == 0 || (*(u32*)player & 0x01000000) != 0) {
            if ((attr & 0x80) == 0) {
                passable = (attr >> 23) & 1;
            } else {
                passable = (*(u16*)(player + 0x2E) == 0x19);
            }
        }
        if (passable != 0) {
            *(void**)(player + 0x1F0) = hit;
            *(void**)(player + 0x1F4) = hit;
            movePos(speed, angle, (f32*)(player + 0x8C), (f32*)(player + 0x94));
            oldPos = *(VecLocal*)(player + 0x8C);
            return;
        }
        if (speed == float_0_804208ac) {
            *(s8*)(player + 0x3F) = 0;
        } else {
            *(f32*)(player + 0x214) = wallDistance;
            *(void**)(player + 0x1F4) = hit;
            *(s8*)(player + 0x3F) += 1;
            if (*(s8*)(player + 0x3F) > 19) {
                *(s8*)(player + 0x3F) = 20;
                attr = hitGetAttr(hit);
                if ((attr & 0x80) == 0 || *(u16*)(player + 0x2E) == 0x19) {
                    *(void**)(player + 0x1F0) = hit;
                }
            }
        }
        *(VecLocal*)(player + 0x8C) = oldPos;
    }

    *(VecLocal*)(player + 0x8C) = oldPos;
    marioChkWallAround(speed, angle, *(f32*)(player + 0x1C0), angle,
                       player + 0x8C, 0);
    PSVECSubtract(player + 0x8C, &oldPos, &delta);
    if (PSVECMag(&delta) > *(f32*)(player + 0x188)) {
        f32 magnitude = PSVECMag(&delta);
        PSVECScale(&delta, &delta, *(f32*)(player + 0x188) / magnitude);
        PSVECAdd(&oldPos, &delta, player + 0x8C);
    }
}

typedef struct VecData {
    f32 x;
    f32 y;
    f32 z;
} VecData;

const VecData vec3_802c3b78 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3b84 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3b90 = { 0.0f, -1.0f, 0.0f };
const VecData vec3_802c3b9c = { 0.0f, -1.0f, 0.0f };
const VecData vec3_802c3ba8 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3bb4 = { 0.0f, -1.0f, 0.0f };
const VecData vec3_802c3bc0 = { 0.0f, -1.0f, 0.0f };
const VecData vec3_802c3bcc = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3bd8 = { 0.0f, -1.0f, 0.0f };
const VecData vec3_802c3be4 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3bf0 = { 0.0f, -1.0f, 0.0f };
const VecData vec3_802c3bfc = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3c08 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3c14 = { 0.0f, -1.0f, 0.0f };
const VecData vec3_802c3c20 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3c2c = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3c38 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3c44 = { 0.0f, 1.0f, 0.0f };
const VecData vec3_802c3c50 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3c5c = { 0.0f, 1.0f, 0.0f };
const VecData vec3_802c3c68 = { 0.0f, 1.0f, 0.0f };
const VecData vec3_802c3c74 = { 0.0f, 1.0f, 0.0f };
const VecData vec3_802c3c80 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3c8c = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3c98 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3ca4 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3cb0 = { 0.0f, -1.0f, 0.0f };
const VecData vec3_802c3cbc = { 0.0f, -1.0f, 0.0f };
const VecData vec3_802c3cc8 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3cd4 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3ce0 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3cec = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3cf8 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3d04 = { 0.0f, -1.0f, 0.0f };
const VecData vec3_802c3d10 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3d1c = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3d28 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3d34 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3d40 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3d4c = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3d58 = { 0.0f, 0.0f, 0.0f };

const char str_dou_10_802c3d88[] = "dou_10";
const char str_eki_04_802c3d90[] = "eki_04";
const char str_MOBJ_PowerUpBlock_802c3d98[] = "MOBJ_PowerUpBlock";
const char str_MOBJ_SaveBlock_802c3dac[] = "MOBJ_SaveBlock";
const char str_MOBJ_RecoveryBlock_802c3dbc[] = "MOBJ_RecoveryBlock";
const char str_MOBJ_CoinTakenRecove_802c3dd0[] = "MOBJ_CoinTakenRecoveryBlock";
const char str_MOBJ_HiddenHatenaBlo_802c3dec[] = "MOBJ_HiddenHatenaBlock";
const char str_MOBJ_HiddenBadgeBloc_802c3e04[] = "MOBJ_HiddenBadgeBlock";
const char str_MOBJ_Hidden10CountBl_802c3e1c[] = "MOBJ_Hidden10CountBlock";

const f32 float_1000_804208a8 = 1000.0f;
const f32 float_0_804208ac = 0.0f;
const f32 float_4_804208b0 = 4.0f;
const f32 float_11_804208b4 = 11.0f;
const f32 float_13_804208b8 = 13.0f;
const f32 float_45_804208bc = 45.0f;
const f32 float_5_804208c0 = 5.0f;
const f32 float_1_804208c4 = 1.0f;
const f32 float_360_804208c8 = 360.0f;
const f32 float_0p5_804208cc = 0.5f;
const f32 float_neg0p5_804208d0 = -0.5f;
const f32 float_180_804208d4 = 180.0f;
const f32 float_0p125_804208d8 = 0.125f;
const f32 float_0p25_804208dc = 0.25f;
const f32 float_0p75_804208e0 = 0.75f;
const f32 float_neg1_804208e4 = -1.0f;
const f32 float_3p1416_804208e8 = 3.141592f;
const f32 float_0p1_804208ec = 0.1f;
const f32 float_2_804208f0 = 2.0f;
const f32 float_18p5_804208f4 = 18.5f;
const f32 float_3_804208f8 = 3.0f;
const f32 float_0p265_804208fc = 0.265f;
const f32 float_100_80420900 = 100.0f;
const f32 float_10_80420904 = 10.0f;
const f32 float_40_80420908 = 40.0f;
const f32 float_neg10_8042090c = -10.0f;
const f32 float_60_80420910 = 60.0f;
const f32 float_0p8_80420914 = 0.8f;
const f32 float_20_80420918 = 20.0f;
const f32 float_8_8042091c = 8.0f;
const f32 float_270_80420920 = 270.0f;
const f32 float_90_80420924 = 90.0f;
const f32 float_30_80420928 = 30.0f;
const f32 float_26p64_8042092c = 26.64f;
const f32 float_12_80420930 = 12.0f;
const f32 float_7_80420934 = 7.0f;
const f32 float_16p8_80420938 = 16.8f;
const char str_dokan_8042093c[] = "dokan";
const f32 float_neg23p7_80420944 = -23.7f;
const f32 float_5p8_80420948 = 5.8f;
const f32 float_9_8042094c = 9.0f;
const f32 float_75_80420950 = 75.0f;
const f32 float_50_80420954 = 50.0f;
const f32 float_0p05_80420958 = 0.05f;
const f32 float_47_8042095c = 47.0f;
const f32 float_18_80420960 = 18.0f;
const f32 float_0p01_80420964 = 0.01f;
const f32 float_neg10000_80420968 = -10000.0f;
const f32 float_8p64_8042096c = 8.64f;
const f32 float_105_80420970 = 105.0f;
const f32 float_0p425_80420974 = 0.425f;
const f32 float_0p375_80420978 = 0.375f;
const f32 float_37_8042097c = 37.0f;
const f32 float_neg3000_80420980 = -3000.0f;
const f32 float_5000_80420984 = 5000.0f;
const f32 float_neg0p4_80420988 = -0.4f;
const f32 float_neg0p06_8042098c = -0.06f;
const f32 float_neg0p04_80420990 = -0.04f;
const f32 float_neg0p02_80420994 = -0.02f;
const f32 float_1037_80420998 = 1037.0f;
const f32 float_6_8042099c = 6.0f;
