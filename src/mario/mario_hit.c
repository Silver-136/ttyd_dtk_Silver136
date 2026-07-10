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
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
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
    extern void* marioGetPtr(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
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
    extern void* marioGetPtr(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
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

u8 N_dou10_yoko_yari3(void) {
    return 0;
}


u8 marioSearchHead_jabara(void) {
    return 0;
}


double marioChkOverheadJabara(void* param_1, float* param_2) {
    return 0.0;
}


void marioChkGnd2(void) {
    extern void* marioGetPtr(void);
    extern void* marioSearchGround(f32 param_1, void* yOut, f32 velocityY, void* param_4, void* param_5);
    extern void* kpaSearchGround(f32 param_1, void* yOut, void* param_3, void* param_4);
    extern s32 kpaFireAttackCheck(void);
    extern void marioChgMot(s32 motion);
    extern void quake_kpaLandOn(void);
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

void marioChkToge(void) {
    extern void* marioGetPtr(void);
    extern s32 vivianGetStatus(void);
    extern u32 hitGetAttr(void* hit);
    extern void set_damage_root_ypos(f64 y);
    extern void marioChgMot(s32 motion);
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

void marioHitCheckWidth(void) {
    extern void* marioGetPtr(void);
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

void marioChkLandon(f32 velocityY, s32 param_2) {
    extern void* marioGetPtr(void);
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

u32 marioChkBero(int param_1) {
    extern u32 hitGetAttr(void* hit);
    extern void* marioGetPtr(void);
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

