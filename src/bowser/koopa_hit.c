#include "bowser/koopa_hit.h"

s32 kpaFlameHitFilter(void* unused, void* obj);

void* marioGetPtr(void);
void* hitCheckSphereFilter(void* filter, f32 x, f32 y, f32 z, f32 radius);

s32 kpaSearchFrontUnder(void) {
    return 0;
}

void* kpaGetHitFlame(void) {
    void* mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) != 2) {
        return 0;
    }
    return *(void**)(*(s32*)((s32)mario + 0x298) + 0x18);
}

void* kpaGetHitobj(void) {
    void* mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) != 2) {
        return 0;
    }
    return *(void**)(*(s32*)((s32)mario + 0x298) + 0x1C);
}

void* kpaGetHitobjHead(s32 index) {
    s32 offset = index * 4;
    void* base = *(void**)((s32)marioGetPtr() + 0x298);
    return *(void**)((s32)base + offset + 0x20);
}

void* kpaGetHitobjRide(s32 index) {
    s32 offset = index * 4;
    void* base = *(void**)((s32)marioGetPtr() + 0x298);
    return *(void**)((s32)base + offset + 0x4C);
}

void* kpaFlameHitCheck(f32 x, f32 y, f32 z, f32 radius) {
    marioGetPtr();
    return hitCheckSphereFilter(kpaFlameHitFilter, x, y, z, radius);
}

typedef struct KpaMobjName {
    u8 pad[0x15];
    char name[1];
} KpaMobjName;

s32 kpaFlameHitFilter(void* unused, void* obj) {
    extern KpaMobjName* mobjHitObjPtrToPtr(void* hitObj);
    extern s32 strcmp(const char* str1, const char* str2);
    extern const char str_MOBJ_HiddenHatenaBlo_802fec9c[];
    extern const char str_MOBJ_Block_802fecb4[];

    KpaMobjName* mobj = mobjHitObjPtrToPtr(obj);
    s32 cmp;

    if (strcmp(mobj->name, str_MOBJ_HiddenHatenaBlo_802fec9c) == 0) {
        return 0;
    }
    cmp = strcmp(mobj->name, str_MOBJ_Block_802fecb4);
    return ((u32)(-cmp) | (u32)cmp) >> 31;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* kpaGetHitobjHead2(void) {
    s32 offset = 0;
    s32 i = 0;
    void* obj = 0;
    for (; i < 10; i++, offset += 4) {
        void* base = *(void**)((s32)marioGetPtr() + 0x298);
        s32 index = offset + 0x20;
        obj = *(void**)((s32)base + index);
        if (obj != 0) {
            break;
        }
    }
    return obj;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void kpaClearHitobjRide(void) {
    void* mario = marioGetPtr();

    *(void**)(*(s32*)((s32)mario + 0x298) + 0x4C) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x50) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x54) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x58) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x5C) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x60) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x64) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x68) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x6C) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x70) = 0;
}

void* kpaChkHitobjRide(void) {
    void* work = *(void**)((s32)marioGetPtr() + 0x298);
    void* obj;

    obj = *(void**)((s32)work + 0x4C);
    if (obj == 0) {
        obj = *(void**)((s32)work + 0x50);
        if (obj == 0) {
            obj = *(void**)((s32)work + 0x54);
            if (obj == 0) {
                obj = *(void**)((s32)work + 0x58);
                if (obj == 0) {
                    obj = *(void**)((s32)work + 0x5C);
                    if (obj == 0) {
                        obj = *(void**)((s32)work + 0x60);
                        if (obj == 0) {
                            obj = *(void**)((s32)work + 0x64);
                            if (obj == 0) {
                                obj = *(void**)((s32)work + 0x68);
                                if (obj == 0) {
                                    obj = *(void**)((s32)work + 0x6C);
                                    if (obj == 0) {
                                        obj = *(void**)((s32)work + 0x70);
                                        if (obj != 0) {
                                            return obj;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return obj;
}

s32 kpaSearchHead(void* param2, f32* outValue, f32 height) {
    extern f32 toMovedir(f32 angle);
    extern s32 kpaGetLevel(void);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 ax, f32 ay, f32 az, s32 flags,
                              f32* outZ, f32* outY, f32* outX, f32* outDist,
                              void* outA, void* outB, void* outC);
    extern s32* headchkofs_581[4];
    extern f32 float_0_804273c0;
    extern f32 float_neg1_804273c4;
    extern f32 float_1000_804273b4;
    extern f32 float_2_804273c8;
    extern f32 float_35_804273cc;
    extern f32 float_10_804273d0;
    extern f32 float_3p1416_804273b8;
    extern f32 float_180_804273bc;
    extern f32 float_1_804273d4;
    extern f32 float_100_804273d8;
    extern f32 float_0p5_804273b0;

    void* mario;
    f32 direction;
    s32 result;
    f32 checkHeight;
    s32 i;
    s32 offset;
    s32 zero;
    f32 resetValue;
    void* kpaWork;
    s32* headData;
    s32 count;
    f32 angleRad;
    s32 hitOffset;
    f32 outX;
    f32 outY;
    f32 outZ;
    f32 dist;
    f32 scratchA;
    f32 scratchB;
    f32 scratchC;

    mario = marioGetPtr();
    if ((*(u32*)((s32)mario + 0x0) & 0x00020000) != 0) {
        return 0;
    }

    if (*(f32*)((s32)mario + 0x180) != float_0_804273c0) {
        direction = *(f32*)((s32)mario + 0x1A4);
    } else {
        direction = toMovedir(*(f32*)((s32)mario + 0x1AC));
    }

    result = 0;
    checkHeight = *outValue;
    i = 0;
    offset = 0;
    zero = 0;
    resetValue = float_1000_804273b4;
    *outValue = float_neg1_804273c4;

    do {
        kpaWork = *(void**)((s32)marioGetPtr() + 0x298);
        *(s32*)((s32)kpaWork + offset + 0x20) = zero;
        kpaWork = *(void**)((s32)marioGetPtr() + 0x298);
        i++;
        *(f32*)((s32)kpaWork + offset + 0x78) = resetValue;
        offset += 4;
    } while (i < 10);

    headData = headchkofs_581[kpaGetLevel()];
    count = *headData;
    headData++;

    if ((*(u32*)((s32)mario + 0x0) & 0x00010000) == 0) {
        switch (kpaGetLevel()) {
            case 0:
                checkHeight = float_2_804273c8;
                break;
            case 1:
                checkHeight = float_0_804273c0;
                break;
            case 2:
                checkHeight = float_0_804273c0;
                break;
            case 3:
                checkHeight = float_35_804273cc;
                break;
        }
    } else {
        if (kpaGetLevel() == 0) {
            if (*(u16*)((s32)mario + 0x2E) == 0x25) {
                checkHeight = float_10_804273d0;
            } else {
                checkHeight = height;
            }
        } else {
            checkHeight = height;
        }
    }

    angleRad = (float_3p1416_804273b8 * direction) / float_180_804273bc;
    hitOffset = 0;

    for (i = 0; i < count; i++, hitOffset += 4) {
        s32 xOff;
        s32 yOff;
        s32 require;
        f32 sinValue;
        f32 xDelta;
        f32 cosValue;
        f32 yDelta;
        f32 zDelta;
        f32 checkX;
        f32 checkY;
        f32 checkZ;
        s32 hit;
        s32 rounded;

        sinValue = (f32)sin(angleRad);
        xOff = headData[0];
        xDelta = (f32)xOff * sinValue;

        cosValue = (f32)cos(angleRad);
        xOff = headData[0];
        yOff = headData[1];
        require = headData[2];
        headData += 3;
        zDelta = (f32)xOff * -cosValue;
        yDelta = (f32)yOff;

        if (require != 0 || ((*(u32*)((s32)mario + 0x0) & 0x00010000) != 0)) {
            checkX = *(f32*)((s32)mario + 0x8C) + xDelta;
            checkY = *(f32*)((s32)mario + 0x90) + yDelta;
            checkZ = *(f32*)((s32)mario + 0x94) + zDelta;
            dist = checkHeight;
            hit = hitCheckFilter(checkX, checkY, checkZ,
                                 float_0_804273c0, float_1_804273d4, float_0_804273c0,
                                 0, &outZ, &outY, &outX, &dist, &scratchA, &scratchB, &scratchC);
            if (hit != 0) {
                rounded = (s32)(float_100_804273d8 * outY + float_0p5_804273b0);
                outY = (f32)rounded / float_100_804273d8;
                if ((checkY + checkHeight) >= outY) {
                    *(f32*)((s32)param2 + 4) = outY;
                    result = hit;
                    *outValue = float_0_804273c0;
                    kpaWork = *(void**)((s32)marioGetPtr() + 0x298);
                    *(s32*)((s32)kpaWork + hitOffset + 0x20) = hit;
                } else {
                    *outValue = outY - *(f32*)((s32)param2 + 4) - checkHeight;
                }
            }
        }
    }

    return result;
}


/* CHATGPT STUB FILL: main/bowser/koopa_hit 20260624_184128 */

/* stub-fill: kpaHitCheck | missing_definition | ghidra_signature */
u8 kpaHitCheck(void) {
    return 0;
}

/* stub-fill: kpaSearchGround | missing_definition | ghidra_signature */
u8 kpaSearchGround(void) {
    return 0;
}

/* stub-fill: kpaEnemyHitChk | missing_definition | ghidra_signature */
u8 kpaEnemyHitChk(void) {
    return 0;
}

/* stub-fill: kpaEnemyFireHitChk | missing_definition | ghidra_signature */
u8 kpaEnemyFireHitChk(void) {
    return 0;
}
