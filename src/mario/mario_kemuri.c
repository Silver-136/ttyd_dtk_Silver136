#include "mario/mario_kemuri.h"

void* marioGetPtr(void);
u32 hitGetAttr(void* hitObj);
void* effKemuriEntry(s32 kind, f32 x, f32 y, f32 z, f32 zero);
u8 kemuri_mugi(void);
u8 mario_kemuri_landon(void);
extern f32 float_0_80422a38;
extern f32 float_0p6_80422a3c;

u8 mario_kemuri(void) {
    extern void* marioGetPtr(void);
    extern s32 marioStGetSystemLevel(void);
    extern u32 hitGetAttr(void* hitObj);
    extern u8 kemuri_mugi(void);
    extern u8 mario_kemuri_landon(void);
    extern u8 kemuri(void);
    extern f32 reviseAngle(f32 angle);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern void effKemuri7N64Entry(s32 kind, f32 x, f32 y, f32 z, f32 dir, f32 side);
    extern s32 kpaGetBodyStatus(void);
    extern f32 float_6p2832_80422a24;
    extern f32 float_360_80422a28;
    extern f32 float_0p2_80422a2c;
    extern f32 float_0p5_80422a44;
    static s32 ct_481;
    static s32 left_or_right_482;
    void* mario;
    u32 attr;
    f32 s;
    f32 c;
    f32 speed;

    mario = marioGetPtr();
    attr = 0;
    if (marioStGetSystemLevel() != 0) {
        return 0;
    }
    if ((*(u32*)mario & 0x01000000) != 0) {
        return 0;
    }
    if (*(u16*)((s32)mario + 0x2E) == 0x1D) {
        return 0;
    }

    if (*(u16*)((s32)mario + 0x2E) == 3) {
        if (*(void**)((s32)mario + 0x1EC) != 0) {
            attr = hitGetAttr(*(void**)((s32)mario + 0x1EC));
        }
    } else if (*(void**)((s32)mario + 0x1E8) != 0) {
        attr = hitGetAttr(*(void**)((s32)mario + 0x1E8));
    }

    if ((attr & 0x1000) != 0) {
        kemuri_mugi();
        return 0;
    }
    if ((attr & 0x100) != 0) {
        mario_kemuri_landon();
        return 0;
    }
    if ((attr & 0x100000) != 0) {
        mario = marioGetPtr();
        if (*(u16*)((s32)mario + 0x2E) != 2 && *(u16*)((s32)mario + 0x2E) != 1) {
            ct_481 = 8;
            return 0;
        }
        if (ct_481++ > 8) {
            ct_481 = 0;
            sincosf((float_6p2832_80422a24 * reviseAngle(-*(f32*)((s32)mario + 0x1A4))) / float_360_80422a28, &s, &c);
            speed = *(f32*)((s32)mario + 0x1B8);
            effKemuri7N64Entry(0,
                               *(f32*)((s32)mario + 0x8C) + float_0p2_80422a2c * (speed * s),
                               *(f32*)((s32)mario + 0x90) + float_0p5_80422a44,
                               *(f32*)((s32)mario + 0x94) + float_0p2_80422a2c * (speed * c),
                               -*(f32*)((s32)mario + 0x1A4),
                               (f32)left_or_right_482);
            left_or_right_482 = left_or_right_482 == 0;
        }
        return 0;
    }

    if ((*(u32*)mario & 0x01000000) == 0 && kpaGetBodyStatus() <= 0 && *(u8*)((s32)mario + 0x3C) != 1) {
        kemuri();
    }
    return 0;
}

void mario_simple_kemuri(void) {
    void* mario;
    void* effect;

    mario = marioGetPtr();
    effect = effKemuriEntry(0,
                            *(f32*)((s32)mario + 0x8C),
                            *(f32*)((s32)mario + 0x90),
                            *(f32*)((s32)mario + 0x94),
                            float_0_80422a38);
    if ((*(u32*)mario & 0x02000000) != 0) {
        *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0x74) = float_0p6_80422a3c;
    }
}

void unk_8013587c(void) {
    void* mario;
    void* hitObj;
    void* effect;
    void* effectWork;
    u32 flags;
    u32 attr;

    mario = marioGetPtr();
    hitObj = *(void**)((s32)mario + 0x1E8);
    if (hitObj != 0) {
        flags = *(u32*)((s32)mario + 0xC);
        if ((flags & 0x20000) != 0) {
            *(u32*)((s32)mario + 0xC) = flags & ~0x20000;
        } else {
            attr = hitGetAttr(hitObj);
            if ((attr & 0x1000) != 0) {
                kemuri_mugi();
            } else if ((attr & 0x100) != 0) {
                mario_kemuri_landon();
            } else {
                effect = effKemuriEntry(0,
                                        *(f32*)((s32)mario + 0x8C),
                                        *(f32*)((s32)mario + 0x90),
                                        *(f32*)((s32)mario + 0x94),
                                        float_0_80422a38);
                if ((*(u32*)mario & 0x02000000) != 0) {
                    effectWork = *(void**)((s32)effect + 0xC);
                    *(f32*)((s32)effectWork + 0x74) = float_0p6_80422a3c;
                }
            }
        }
    }
}

u8 mario_kemuri_landon(void) {
    extern void* marioGetPtr(void);
    extern s32 strcmp(const char* a, const char* b);
    extern void* gp;
    extern char str_mri_05_802f3738[];
    extern void effWaterDamageN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 scale);
    extern void effRippleEntry(s32 type, f32 x, f32 y, f32 z);
    extern s32 rand(void);
    extern f32 float_1_80422a4c;
    extern f32 float_11_80422a60;
    static s32 cnt_491;
    static s32 cnt2_492;
    void* mario;
    s32 add;
    u16 state;
    s32 r;

    mario = marioGetPtr();
    add = 0;
    if (strcmp((const char*)((s32)gp + 0x12C), str_mri_05_802f3738) == 0) {
        add = 10;
    }

    if (*(u16*)((s32)mario + 0x2E) == 3 && *(s16*)((s32)mario + 0x50) == 1 && cnt2_492 == 10) {
        effWaterDamageN64Entry(add + 5, 30,
                               *(f32*)((s32)mario + 0x8C),
                               *(f32*)((s32)mario + 0x90),
                               *(f32*)((s32)mario + 0x94),
                               float_1_80422a4c);
        cnt2_492 = 0;
        effRippleEntry(0,
                       *(f32*)((s32)mario + 0x8C),
                       *(f32*)((s32)mario + 0x90) + float_11_80422a60,
                       *(f32*)((s32)mario + 0x94));
    }
    if (++cnt2_492 > 10) {
        cnt2_492 = 10;
    }

    state = *(u16*)((s32)mario + 0x2E);
    if (state == 0 || state == 1 || state == 2 || state == 13) {
        if (state == 13) {
            cnt_491 = 0;
        }
        if (--cnt_491 < 0) {
            r = rand();
            cnt_491 = (r % 20) + 30;
            effRippleEntry(0,
                           *(f32*)((s32)mario + 0x8C),
                           *(f32*)((s32)mario + 0x90) + float_11_80422a60,
                           *(f32*)((s32)mario + 0x94));
        }
        if (state == 1 || state == 2 || state == 13) {
            if ((*(s32*)((s32)gp + 0x1C) % 20) == 0 || state == 13) {
                effWaterDamageN64Entry(add + 7, 30,
                                       *(f32*)((s32)mario + 0x8C),
                                       *(f32*)((s32)mario + 0x90) + float_11_80422a60,
                                       *(f32*)((s32)mario + 0x94),
                                       float_1_80422a4c);
            }
        }
    }
    return 0;
}


u8 kemuri_mugi(void) {
    extern void* marioGetPtr(void);
    extern void* gp;
    extern s32 strncmp(const char* a, const char* b, u32 n);
    extern void effWaterDamageN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 scale);
    extern void effMugiKemuriEntry(f32 x, f32 y, f32 z, f32 dir);
    extern void effMugiKemuri2Entry(s32 kind, f32 x, f32 y, f32 z, f32 dir, f32 rand, f32 side);
    extern f32 reviseAngle(f32 angle);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern s32 rand(void);
    extern char str_mri_80422a5c[];
    extern f32 float_11_80422a60;
    extern f32 float_0p8_80422a64;
    extern f32 float_14_80422a68;
    extern f32 float_35_80422a6c;
    extern f32 float_6p2832_80422a24;
    extern f32 float_360_80422a28;
    extern f32 float_neg0p4_80422a70;
    extern f32 float_3_80422a74;
    extern f32 float_5_80422a78;
    static f32 dir_519;
    static s32 ct_520;
    static s32 ct2_521;
    static s32 left_or_right_522;
    void* mario;
    u16 state;
    f32 s;
    f32 c;
    f32 x;
    f32 y;
    f32 z;
    s32 r;

    mario = marioGetPtr();
    if (strncmp((const char*)((s32)gp + 0x12C), str_mri_80422a5c, 3) == 0) {
        state = *(u16*)((s32)mario + 0x2E);
        if (state == 0 && (*(s32*)((s32)gp + 0x1C) % 30) == 0) {
            effWaterDamageN64Entry(0x10, 30,
                                   *(f32*)((s32)mario + 0x8C),
                                   *(f32*)((s32)mario + 0x90) + float_11_80422a60,
                                   *(f32*)((s32)mario + 0x94),
                                   float_0p8_80422a64);
        }
        if (state == 1 || state == 2 || state == 13) {
            if ((*(s32*)((s32)gp + 0x1C) % 20) == 0 || state == 13) {
                effWaterDamageN64Entry(0x11, 30,
                                       *(f32*)((s32)mario + 0x8C),
                                       *(f32*)((s32)mario + 0x90) + float_11_80422a60,
                                       *(f32*)((s32)mario + 0x94),
                                       float_0p8_80422a64);
            }
        }
        return 0;
    }

    if (*(u16*)((s32)mario + 0x2E) == 3 && *(s16*)((s32)mario + 0x50) == 1 && ct2_521 == 10) {
        effMugiKemuriEntry(*(f32*)((s32)mario + 0x8C),
                           *(f32*)((s32)mario + 0x90) + float_14_80422a68,
                           *(f32*)((s32)mario + 0x94),
                           dir_519);
        dir_519 = reviseAngle(dir_519 + float_35_80422a6c);
        ct2_521 = 0;
        return 0;
    }
    if (++ct2_521 > 10) {
        ct2_521 = 10;
    }

    state = *(u16*)((s32)mario + 0x2E);
    if (state != 2 && state != 1) {
        ct_520 = 2;
        return 0;
    }
    if (ct_520++ <= 2) {
        return 0;
    }
    ct_520 = 0;
    sincosf((float_6p2832_80422a24 * reviseAngle(-*(f32*)((s32)mario + 0x1A4))) / float_360_80422a28, &s, &c);
    x = *(f32*)((s32)mario + 0x8C) + float_neg0p4_80422a70 * (*(f32*)((s32)mario + 0x1B8) * s);
    y = *(f32*)((s32)mario + 0x90) + float_3_80422a74;
    z = *(f32*)((s32)mario + 0x94) + float_neg0p4_80422a70 * (*(f32*)((s32)mario + 0x1B8) * c);
    r = rand() % 10;
    effMugiKemuri2Entry(0, x, y, z,
                        -*(f32*)((s32)mario + 0x1A4) + (f32)r - float_5_80422a78,
                        (f32)r,
                        (f32)left_or_right_522);
    left_or_right_522 = left_or_right_522 == 0;
    return 0;
}

u8 kemuri(void) {
    extern void* marioGetPtr(void);
    extern void* effKemuriEntry(s32 kind, f32 x, f32 y, f32 z, f32 dir);
    extern void* effKemuri2Entry(s32 kind, f32 x, f32 y, f32 z, f32 dir);
    extern f32 reviseAngle(f32 angle);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern s32 kpaGetLevel(void);
    extern f32 kpaGetBaseScale(void);
    extern s32 kpaGetStageViewType(void);
    extern f32 toMovedirSimple(f32 angle);
    extern f32 float_0_80422a38;
    extern f32 float_0p6_80422a3c;
    extern f32 float_6p2832_80422a24;
    extern f32 float_360_80422a28;
    extern f32 float_0p2_80422a2c;
    extern f32 float_1p5_80422a30;
    extern f32 float_10_80422a34;
    extern f32 float_180_80422a40;
    extern f32 float_0p5_80422a44;
    extern f32 float_15_80422a48;
    extern f32 float_1_80422a4c;
    extern f32 float_20_80422a50;
    extern f32 float_50_80422a54;
    extern f32 float_4_80422a58;
    static s32 ct_552;
    static f32 dir_551;
    void* mario;
    void* effect;
    f32 s;
    f32 c;
    f32 speed;
    f32 dist;
    f32 scale;
    s32 level;

    mario = marioGetPtr();
    if (*(s16*)((s32)mario + 0x54) != 0 &&
        *(void**)((s32)mario + 0x20C) == 0 &&
        *(void**)((s32)mario + 0x1E8) != 0) {
        if ((*(u32*)((s32)mario + 0xC) & 0x20000) != 0) {
            *(u32*)((s32)mario + 0xC) &= ~0x20000;
            return 0;
        }
        if (*(u8*)((s32)mario + 0x3C) == 2) {
            effect = effKemuriEntry(0,
                                    *(f32*)((s32)mario + 0x8C),
                                    *(f32*)((s32)mario + 0x90),
                                    *(f32*)((s32)mario + 0x94) + float_10_80422a34,
                                    float_0_80422a38);
            kpaGetLevel();
            *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0x74) = kpaGetBaseScale();
        } else {
            effKemuriEntry(0,
                           *(f32*)((s32)mario + 0x8C),
                           *(f32*)((s32)mario + 0x90),
                           *(f32*)((s32)mario + 0x94),
                           dir_551);
        }
        return 0;
    }

    if (*(u16*)((s32)mario + 0x2E) != 2) {
        ct_552 = 7;
        return 0;
    }

    if (*(u8*)((s32)mario + 0x3C) != 2) {
        if (ct_552++ <= 6) {
            return 0;
        }
        ct_552 = 0;
        sincosf((float_6p2832_80422a24 * reviseAngle(-*(f32*)((s32)mario + 0x1A4))) / float_360_80422a28, &s, &c);
        speed = *(f32*)((s32)mario + 0x1B8);
        effect = effKemuri2Entry(0,
                                 *(f32*)((s32)mario + 0x8C) + float_0p2_80422a2c * (speed * s),
                                 *(f32*)((s32)mario + 0x90) + float_1p5_80422a30,
                                 *(f32*)((s32)mario + 0x94) + float_0p2_80422a2c * (speed * c),
                                 0.0f);
        if ((*(u32*)mario & 0x02000000) != 0) {
            *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0x74) = float_0p6_80422a3c;
        }
        return 0;
    }

    if (--ct_552 > 0) {
        return 0;
    }
    sincosf((float_6p2832_80422a24 * reviseAngle(-*(f32*)((s32)mario + 0x1A4))) / float_360_80422a28, &s, &c);
    if (kpaGetStageViewType() == 0) {
        toMovedirSimple(float_180_80422a40 + *(f32*)((s32)mario + 0x1B0));
    } else {
        toMovedirSimple(float_180_80422a40 + *(f32*)((s32)mario + 0x1AC));
    }
    sincosf((float_6p2832_80422a24 * reviseAngle(-*(f32*)((s32)mario + 0x1A4))) / float_360_80422a28, &s, &c);
    level = kpaGetLevel();
    dist = float_10_80422a34;
    scale = float_0p5_80422a44;
    if (level == 1) {
        ct_552 = 7;
        dist = float_15_80422a48;
        scale = float_1_80422a4c;
    } else if (level == 2) {
        ct_552 = 10;
        dist = float_20_80422a50;
        scale = 2.0f;
    } else if (level == 3) {
        ct_552 = 10;
        dist = float_50_80422a54;
        scale = float_4_80422a58;
    } else {
        ct_552 = 4;
    }
    effect = effKemuri2Entry(0,
                             *(f32*)((s32)mario + 0x8C) + dist * s,
                             *(f32*)((s32)mario + 0x90) + float_1p5_80422a30,
                             *(f32*)((s32)mario + 0x94) + dist * c,
                             0.0f);
    *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0x74) = scale;
    return 0;
}

u8 party_kemuri(void* party) {
    extern s32 marioStGetSystemLevel(void);
    extern u32 hitGetAttr(void* hitObj);
    extern f32 reviseAngle(f32 angle);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern void* effKemuri2Entry(s32 kind, f32 x, f32 y, f32 z, f32 dir);
    extern f32 float_2_80422a20;
    extern f32 float_6p2832_80422a24;
    extern f32 float_360_80422a28;
    extern f32 float_0p2_80422a2c;
    extern f32 float_1p5_80422a30;
    f32 s;
    f32 c;
    f32 angle;
    f32 speed;
    u8 count;

    if (marioStGetSystemLevel() != 0) {
        return 0;
    }
    if (*(u8*)((s32)party + 0x34) == 10) {
        return 0;
    }
    if (*(u8*)((s32)party + 0x32) == 1) {
        return 0;
    }

    if (*(u8*)((s32)party + 0x34) == 3 && *(u8*)((s32)party + 0x31) == 4) {
        if (*(void**)((s32)party + 0x138) == 0) {
            *(u8*)((s32)party + 0x2C) = 4;
            return 0;
        }
        if ((hitGetAttr(*(void**)((s32)party + 0x138)) & 0x100) != 0) {
            return 0;
        }
        if (*(f32*)((s32)party + 0x104) < float_2_80422a20) {
            return 0;
        }
    } else {
        if (*(u8*)((s32)party + 0x36) != 1) {
            return 0;
        }
        if (*(void**)((s32)party + 0x138) == 0) {
            *(u8*)((s32)party + 0x2C) = 4;
            return 0;
        }
        if ((hitGetAttr(*(void**)((s32)party + 0x138)) & 0x100) != 0) {
            return 0;
        }
        if (*(f32*)((s32)party + 0x104) < float_2_80422a20) {
            return 0;
        }
    }

    count = *(u8*)((s32)party + 0x2C);
    *(u8*)((s32)party + 0x2C) = count + 1;
    if ((s8)count <= 6) {
        return 0;
    }
    *(u8*)((s32)party + 0x2C) = 0;

    angle = reviseAngle(-*(f32*)((s32)party + 0x100));
    sincosf((float_6p2832_80422a24 * angle) / float_360_80422a28, &s, &c);
    speed = *(f32*)((s32)party + 0xF4);
    effKemuri2Entry(0,
                    *(f32*)((s32)party + 0x58) + float_0p2_80422a2c * (speed * s),
                    *(f32*)((s32)party + 0x5C) + float_1p5_80422a30,
                    *(f32*)((s32)party + 0x60) + float_0p2_80422a2c * (speed * c),
                    0.0f);
    return 0;
}
