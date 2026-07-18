#include "statuswindow.h"

void* wp;
s32 valueCheck(void);
void statusGetValue(void* out);
void statusWinForceOff(void);
void* __memAlloc(s32 heap, u32 size);
extern s16 alwaysDt[];
extern f32 float_0_80422be8;
extern f32 float_1_80422bf4;
extern f32 float_neg304_80422c68;
extern f32 float_208_80422c6c;
extern f32 float_334_80422c70;

void statusWinDispOff(void) {
    *(u16*)((s32)wp + 4) |= 0x1000;
}

void statusWinDispOn(void) {
    *(u16*)((s32)wp + 4) &= ~0x1000;
}

void N_statusClearBlink(void) {
    *(s32*)((s32)wp + 0x80) = 0;
}

void statusWinForceCloseClear(void) {
    *(u16*)((s32)wp + 4) &= ~0xA0;
}

void statusFPBlink(s32 value) {
    *(u32*)((s32)wp + 0x80) |= 2;
    *(s32*)((s32)wp + 0x88) = value;
}

void statusMarioHPBlink(s32 value) {
    *(u32*)((s32)wp + 0x80) |= 1;
    *(s32*)((s32)wp + 0x88) = value;
}

void statusPartyHPBlink(s32 value) {
    *(u32*)((s32)wp + 0x80) |= 8;
    *(s32*)((s32)wp + 0x88) = value;
}

s32 statusWinCheck(void) {
    s32 value = *(u16*)((s32)wp + 4) & 0x11;
    return ((u32)(-value) | (u32)value) >> 31;
}

s32 statusWinCheckUpdate(void) {
    s32 value = valueCheck();
    return ((u32)(-value) | (u32)value) >> 31;
}

void statusAPBlink(s32 value) {
    *(s32*)((s32)wp + 0x80) = 0;
    *(u32*)((s32)wp + 0x80) |= 4;
    *(s32*)((s32)wp + 0x88) = value;
}

void statusWinClose(void) {
    *(u16*)((s32)wp + 4) |= 0xA;
    *(u16*)((s32)wp + 4) &= ~5;
}

void statusWinOpen(void) {
    *(u16*)((s32)wp + 4) &= ~0xA;
    *(u16*)((s32)wp + 4) |= 5;
}

void statusWinForceUpdateCoin(void) {
    u8 values[0x14];
    statusGetValue(values);
    *(s16*)((s32)wp + 0x60) = *(s16*)(values + 0x10);
}

void statusWinForceClose(void) {
    *(u16*)((s32)wp + 4) &= ~5;
    *(u16*)((s32)wp + 4) &= ~0x50;
    *(u16*)((s32)wp + 4) |= 0xA0;
}

void statusWinForceOpen(void) {
    *(u16*)((s32)wp + 4) &= ~0xA;
    *(u16*)((s32)wp + 4) &= ~0xA0;
    *(u16*)((s32)wp + 4) |= 0x50;
}


void statusWinDisp(void) {
    typedef struct StatusIcon {
        s16 x;
        s16 y;
        u16 iconId;
        u16 pad;
        f32 scale;
    } StatusIcon;
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern void iconDispGxCol(f32 mtx[3][4], s32 flags, s32 iconId, u32* color);
    extern void iconNumberDispGx(f32 mtx[3][4], s32 value, s32 type, u32* color);
    extern void gaugeDisp(f32 x, f32 y, s32 value);
    extern u32 dat_80422bcc;
    extern u32 dat_80422bc8;
    StatusIcon* icon;
    f32 trans[3][4];
    f32 scale[3][4];
    f32 x;
    f32 y;
    u32 color;
    s32 blink;
    s32 i;

    x = *(f32*)((char*)wp + 0x20);
    y = *(f32*)((char*)wp + 0x24);
    blink = 1;
    if (*(s32*)((char*)wp + 0x84) > 0) {
        *(s32*)((char*)wp + 0x84) -= 1;
        blink = (((*(s32*)((char*)wp + 0x84) / 16) % 16) > 5);
    }

    icon = (StatusIcon*)alwaysDt;
    for (i = 0; i < 13; i++, icon++) {
        if (blink ||
            (((*(u32*)((char*)wp + 0x80) & 1) == 0 || i != 1) &&
             ((*(u32*)((char*)wp + 0x80) & 2) == 0 || i != 4) &&
             ((*(u32*)((char*)wp + 0x80) & 0x10) == 0 || i != 8) &&
             ((*(u32*)((char*)wp + 0x80) & 4) == 0 || i != 12))) {
            PSMTXTrans(trans, x + (f32)icon->x, y - (f32)icon->y, 0.0f);
            PSMTXScale(scale, icon->scale, icon->scale, icon->scale);
            PSMTXConcat(trans, scale, trans);
            color = dat_80422bcc;
            iconDispGxCol(trans, 0x10, icon->iconId, &color);
        }
    }

    if (((*(u32*)((char*)wp + 0x80) & 1) == 0) || blink) {
        PSMTXTrans(trans, ((120.0f + x) - 6.0f) - 10.0f, 2.0f + (y - 38.0f), 0.0f);
        color = dat_80422bc8;
        iconNumberDispGx(trans, *(s16*)((char*)wp + 0x50), 0, &color);
    }
    PSMTXTrans(trans, ((186.0f + x) - 6.0f) - 10.0f, 3.0f + (y - 38.0f), 0.0f);
    color = dat_80422bc8;
    iconNumberDispGx(trans, *(s16*)((char*)wp + 0x52), 1, &color);
    if (((*(u32*)((char*)wp + 0x80) & 2) == 0) || blink) {
        PSMTXTrans(trans, (292.0f + x) - 6.0f, 2.0f + (y - 38.0f), 0.0f);
        color = dat_80422bc8;
        iconNumberDispGx(trans, *(s16*)((char*)wp + 0x54), 0, &color);
    }
    PSMTXTrans(trans, (358.0f + x) - 6.0f, 3.0f + (y - 38.0f), 0.0f);
    color = dat_80422bc8;
    iconNumberDispGx(trans, *(s16*)((char*)wp + 0x56), 1, &color);
    PSMTXTrans(trans, 462.0f + x, 2.0f + (y - 38.0f), 0.0f);
    color = dat_80422bc8;
    iconNumberDispGx(trans, *(s16*)((char*)wp + 0x5E), 0, &color);
    if (((*(u32*)((char*)wp + 0x80) & 0x10) == 0) || blink) {
        PSMTXTrans(trans, 572.0f + x, 2.0f + (y - 38.0f), 0.0f);
        color = dat_80422bc8;
        iconNumberDispGx(trans, *(s16*)((char*)wp + 0x60), 0, &color);
    }
    if (((*(u32*)((char*)wp + 0x80) & 4) == 0) || blink) {
        gaugeDisp(260.0f + x, 4.0f + (y - 87.0f), *(s16*)((char*)wp + 0x62));
    }
}

void statusWinMain(void) {
    extern void* marioGetPtr(void);
    extern s32 strcmp(const char*, const char*);
    extern s32 seqGetSeq(void);
    extern s32 marioChkCtrl(void);
    extern s32 marioChkKey(void);
    extern f32 PSVECDistance(void*, void*);
    extern void psndSFXOn(s32);
    extern double cos(double);
    extern void dispEntry(s32, s32, void*, s32, f32);
    extern void valueUpdate(void);
    extern void* gp;
    extern char str_yuu_80422c5c[];
    extern f32 float_16_80422c48;
    extern f32 float_28_80422c60;
    extern f32 float_60_80422bec;
    extern f32 float_3p1416_80422bf0;
    extern f32 float_0p5_80422bf8;
    extern f32 float_500_80422c64;
    void* player;
    s32 seq;
    u16 flags;
    f32 ratio;
    f32 invRatio;

    player = marioGetPtr();
    if (*(s8*)((s32)player + 0x3C) != 0) {
        *(u16*)((s32)wp + 4) = 2;
        *(f32*)((s32)wp + 0x0C) = float_1_80422bf4;
        *(f32*)((s32)wp + 0x08) = float_1_80422bf4;
        *(f32*)((s32)wp + 0x18) = float_0_80422be8;
        *(f32*)((s32)wp + 0x10) = float_0_80422be8;
        return;
    }

    if (strcmp((const char*)((s32)gp + 0x13C), str_yuu_80422c5c) == 0) {
        return;
    }

    seq = seqGetSeq();
    if (seq != 4) {
        if (seq >= 2 && seq < 4) {
            if (marioChkCtrl() == 0 || marioChkKey() == 0 || (*(u32*)((s32)player + 0) & 1) == 0) {
                flags = *(u16*)((s32)wp + 4);
                if ((flags & 0x11) != 0) {
                    *(u16*)((s32)wp + 4) = flags | 0xA;
                    *(u16*)((s32)wp + 4) &= ~5;
                }
                valueCheck();
                *(s32*)((s32)wp + 0x74) = 0;
                *(s32*)((s32)wp + 0x78) = 0;
            } else {
                if (PSVECDistance((void*)((s32)wp + 0x68), (void*)((s32)player + 0x8C)) < float_1_80422bf4) {
                    *(s32*)((s32)wp + 0x74) += 1;
                    if (*(s32*)((s32)wp + 0x74) > *(s32*)((s32)gp + 4) * 10) {
                        *(s32*)((s32)wp + 0x74) = *(s32*)((s32)gp + 4) * 10;
                        if (*(s32*)((s32)wp + 0x7C) == 0 && *(s32*)((s32)wp + 0x78) == 0 &&
                            (*(u16*)((s32)wp + 4) & 0x11) == 0) {
                            *(u16*)((s32)wp + 4) &= ~0xA;
                            *(u16*)((s32)wp + 4) |= 5;
                            *(u16*)((s32)wp + 4) |= 0x100;
                        }
                    }
                } else {
                    *(s32*)((s32)wp + 0x74) = 0;
                    *(s32*)((s32)wp + 0x7C) = 0;
                    if (*(s32*)((s32)wp + 0x78) == 0 && (*(u16*)((s32)wp + 4) & 0x11) != 0) {
                        *(u16*)((s32)wp + 4) |= 0xA;
                        *(u16*)((s32)wp + 4) &= ~5;
                    }
                }

                if ((*(u16*)((s32)player + 0x24C) & 0x10) != 0) {
                    flags = *(u16*)((s32)wp + 4);
                    if ((flags & 0x11) == 0) {
                        *(u16*)((s32)wp + 4) = flags & ~0xA;
                        *(u16*)((s32)wp + 4) |= 5;
                        *(s32*)((s32)wp + 0x78) = *(s32*)((s32)gp + 4) * 10;
                        psndSFXOn(0x2A);
                        *(u16*)((s32)wp + 4) |= 0x100;
                    } else {
                        *(u16*)((s32)wp + 4) = flags | 0xA;
                        *(u16*)((s32)wp + 4) &= ~5;
                        *(s32*)((s32)wp + 0x78) = *(s32*)((s32)gp + 4) * 10;
                        *(s32*)((s32)wp + 0x7C) = 1;
                        psndSFXOn(0x2B);
                    }
                }

                if (valueCheck() != 0) {
                    *(s32*)((s32)wp + 0x78) = *(s32*)((s32)gp + 4) << 1;
                    if ((*(u16*)((s32)wp + 4) & 0x11) == 0) {
                        *(u16*)((s32)wp + 4) &= ~0xA;
                        *(u16*)((s32)wp + 4) |= 5;
                    }
                }
            }

            if (*(s32*)((s32)wp + 0x78) != 0) {
                *(s32*)((s32)wp + 0x78) -= 1;
            }
            *(f32*)((s32)wp + 0x68) = *(f32*)((s32)player + 0x8C);
            *(f32*)((s32)wp + 0x6C) = *(f32*)((s32)player + 0x90);
            *(f32*)((s32)wp + 0x70) = *(f32*)((s32)player + 0x94);
            if ((*(u16*)((s32)wp + 4) & 0x10) != 0) {
                *(u16*)((s32)wp + 4) &= ~0xA;
            }
            if ((*(u16*)((s32)wp + 4) & 0x20) != 0) {
                *(u16*)((s32)wp + 4) &= ~5;
            }
        } else if ((*(u16*)((s32)wp + 4) & 0x11) != 0) {
            *(u16*)((s32)wp + 4) |= 0xA;
            *(u16*)((s32)wp + 4) &= ~5;
        }
    }

    flags = *(u16*)((s32)wp + 4);
    if ((flags & 4) != 0 || (flags & 8) != 0 || (flags & 0x40) != 0 || (flags & 0x80) != 0) {
        *(f32*)((s32)wp + 0x14) = *(f32*)((s32)wp + 0x10);
        if ((*(u16*)((s32)wp + 4) & 0x44) != 0) {
            *(f32*)((s32)wp + 0x18) = float_1_80422bf4;
            *(f32*)((s32)wp + 0x0C) = float_16_80422c48 / *(f32*)((s32)wp + 0x1C);
        } else {
            *(f32*)((s32)wp + 0x18) = float_0_80422be8;
            *(f32*)((s32)wp + 0x0C) = float_28_80422c60 / *(f32*)((s32)wp + 0x1C);
        }
        *(f32*)((s32)wp + 0x08) = float_0_80422be8;
        *(u16*)((s32)wp + 4) &= ~4;
        *(u16*)((s32)wp + 4) &= ~8;
        *(u16*)((s32)wp + 4) &= ~0x40;
        *(u16*)((s32)wp + 4) &= ~0x80;
    }

    *(f32*)((s32)wp + 0x08) += float_60_80422bec / (f32)*(s32*)((s32)gp + 4);
    if (*(f32*)((s32)wp + 0x08) < *(f32*)((s32)wp + 0x0C)) {
        *(f32*)((s32)wp + 0x10) =
            (*(f32*)((s32)wp + 0x18) - *(f32*)((s32)wp + 0x14)) *
            (float_1_80422bf4 - (f32)cos((float_3p1416_80422bf0 * *(f32*)((s32)wp + 0x08)) / *(f32*)((s32)wp + 0x0C))) *
            float_0p5_80422bf8 + *(f32*)((s32)wp + 0x14);
    } else {
        *(f32*)((s32)wp + 0x08) = *(f32*)((s32)wp + 0x0C);
        *(f32*)((s32)wp + 0x10) = *(f32*)((s32)wp + 0x18);
        if (*(f32*)((s32)wp + 0x10) == float_0_80422be8) {
            *(u16*)((s32)wp + 4) &= ~0x100;
        }
    }

    ratio = *(f32*)((s32)wp + 0x10);
    invRatio = float_1_80422bf4 - ratio;
    *(f32*)((s32)wp + 0x20) = *(f32*)((s32)wp + 0x30) * ratio + *(f32*)((s32)wp + 0x40) * invRatio;
    *(f32*)((s32)wp + 0x24) = *(f32*)((s32)wp + 0x34) * ratio + *(f32*)((s32)wp + 0x44) * invRatio;
    valueUpdate();
    if ((*(u16*)((s32)wp + 4) & 0x1000) == 0) {
        dispEntry(8, 0, statusWinDisp, 0, float_500_80422c64);
    }
}

void valueUpdate(void) {
    extern void psndSFXOn(const char*);
    extern const char str_SFX_COUNT_COIN1_802f3a50[];
    static s32 count;
    s16 values[12];
    s32 step;
    s32 diff;
    s32 target;
    s32 current;

    statusGetValue(values);

    count = (count + 1) % 0x4B0;
    if ((count & 1) != 0) {
        return;
    }
    if ((*(u16*)((s32)wp + 4) & 0x11) == 0) {
        return;
    }

    diff = values[8] - *(s16*)((s32)wp + 0x60);
    step = 0;
    if (diff != 0) {
        step = diff / 10;
        if (step == 0) {
            if (*(s16*)((s32)wp + 0x60) < values[8]) {
                step = 1;
            } else {
                step = -1;
            }
        }
    }
    if (step != 0 && (count % 8) == 0) {
        psndSFXOn(str_SFX_COUNT_COIN1_802f3a50);
    }

    target = values[0];
    current = *(s16*)((s32)wp + 0x50);
    diff = target - current;
    step = 0;
    if (diff != 0) {
        step = diff / 10;
        if (step == 0) {
            step = current < target ? 1 : -1;
        }
    }
    *(s16*)((s32)wp + 0x50) += step;

    target = values[1];
    current = *(s16*)((s32)wp + 0x52);
    diff = target - current;
    step = 0;
    if (diff != 0) {
        step = diff / 10;
        if (step == 0) {
            step = current < target ? 1 : -1;
        }
    }
    *(s16*)((s32)wp + 0x52) += step;

    target = values[2];
    current = *(s16*)((s32)wp + 0x54);
    diff = target - current;
    step = 0;
    if (diff != 0) {
        step = diff / 10;
        if (step == 0) {
            step = current < target ? 1 : -1;
        }
    }
    *(s16*)((s32)wp + 0x54) += step;

    target = values[3];
    current = *(s16*)((s32)wp + 0x56);
    diff = target - current;
    step = 0;
    if (diff != 0) {
        step = diff / 10;
        if (step == 0) {
            step = current < target ? 1 : -1;
        }
    }
    *(s16*)((s32)wp + 0x56) += step;

    target = values[9];
    current = *(s16*)((s32)wp + 0x62);
    diff = target - current;
    step = 0;
    if (diff != 0) {
        step = diff / 10;
        if (step == 0) {
            step = current < target ? 1 : -1;
        }
    }
    *(s16*)((s32)wp + 0x62) += step;

    target = values[10];
    current = *(s16*)((s32)wp + 0x64);
    diff = target - current;
    step = 0;
    if (diff != 0) {
        step = diff / 10;
        if (step == 0) {
            step = current < target ? 1 : -1;
        }
    }
    *(s16*)((s32)wp + 0x64) += step;

    target = values[7];
    current = *(s16*)((s32)wp + 0x5E);
    diff = target - current;
    step = 0;
    if (diff != 0) {
        step = diff / 10;
        if (step == 0) {
            step = current < target ? 1 : -1;
        }
    }
    *(s16*)((s32)wp + 0x5E) += step;

    target = values[8];
    current = *(s16*)((s32)wp + 0x60);
    diff = target - current;
    step = 0;
    if (diff != 0) {
        step = diff / 10;
        if (step == 0) {
            step = current < target ? 1 : -1;
        }
    }
    *(s16*)((s32)wp + 0x60) += step;

    *(s16*)((s32)wp + 0x58) = values[4];
    if (*(s16*)((s32)wp + 0x58) != 0) {
        target = values[5];
        current = *(s16*)((s32)wp + 0x5A);
        diff = target - current;
        step = 0;
        if (diff != 0) {
            step = diff / 10;
            if (step == 0) {
                step = current < target ? 1 : -1;
            }
        }
        *(s16*)((s32)wp + 0x5A) += step;

        target = values[6];
        current = *(s16*)((s32)wp + 0x5C);
        diff = target - current;
        step = 0;
        if (diff != 0) {
            step = diff / 10;
            if (step == 0) {
                step = current < target ? 1 : -1;
            }
        }
        *(s16*)((s32)wp + 0x5C) += step;
    }

    if (*(s16*)((s32)wp + 0x5E) > 99) {
        *(s16*)((s32)wp + 0x5E) = 99;
    }
    if (*(s16*)((s32)wp + 0x60) > 999) {
        *(s16*)((s32)wp + 0x60) = 999;
    }
}

u8 gaugeDisp(s32 value, f32 x, f32 y) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void* pouchGetPtr(void);
    extern void iconDispGx(LocalVec3* pos, s32 size, s32 iconId, f32 scale);
    extern u16 gauge_wakka[];
    extern u16 gauge_back[];
    extern f32 float_0_80422be8;
    extern f32 float_1_80422bf4;
    extern f32 float_12_80422c58;
    s32 max;
    s32 i;
    s32 enabled;
    s32 full;
    s32 rest;
    s32 fill;
    LocalVec3 pos;

    max = 0;
    for (i = 0; i < 8; i++) {
        if ((*(u16*)((s32)pouchGetPtr() + 0x8C) & (1 << i)) != 0) {
            max += 100;
        }
    }
    if (value > max) {
        value = max;
    }
    if (value < 0) {
        value = 0;
    }

    full = value / 100;
    rest = value % 100;
    fill = (rest * 15) / 100;
    if (rest != 0 && value != 0 && fill == 0) {
        fill = 1;
    }

    if (fill != 0) {
        pos.x = x + (f32)(full << 5);
        pos.y = y;
        pos.z = float_0_80422be8;
        iconDispGx(&pos, 0x10, gauge_wakka[fill], float_1_80422bf4);
    }

    y += float_12_80422c58;
    for (i = 0; i < 8; i++) {
        if ((*(u16*)((s32)pouchGetPtr() + 0x8C) & (1 << i)) != 0) {
            pos.x = x + (f32)(i << 5);
            pos.y = y;
            pos.z = float_0_80422be8;
            if (i < full) {
                enabled = gauge_back[i];
            } else {
                enabled = 0x1C7;
            }
            iconDispGx(&pos, 0x10, enabled, float_1_80422bf4);
        }
    }
    return 0;
}

s32 valueCheck(void) {
    extern void* gp;
    s16 values[12];
    void* work;
    s32 changed;
    u32 value;

    changed = 0;
    statusGetValue(values);

    work = wp;
    if (*(s16*)((s32)work + 0x50) != values[0]) {
        *(u32*)((s32)work + 0x84) |= 1;
        changed = 1;
    }

    work = wp;
    if (*(s16*)((s32)work + 0x52) != values[1]) {
        changed = 1;
    }
    if (*(s16*)((s32)work + 0x54) != values[2]) {
        *(u32*)((s32)work + 0x84) |= 2;
        changed = 1;
    }

    work = wp;
    if (*(s16*)((s32)work + 0x56) != values[3]) {
        changed = 1;
    }
    if (*(s16*)((s32)work + 0x62) != values[9]) {
        changed = 1;
    }
    if (*(s16*)((s32)work + 0x64) != values[10]) {
        changed = 1;
    }

    if (*(s16*)((s32)work + 0x58) != values[4]) {
        *(s16*)((s32)work + 0x58) = values[4];
    } else if (*(s16*)((s32)work + 0x58) != 0) {
        if (*(s16*)((s32)work + 0x5A) != values[5]) {
            *(u32*)((s32)work + 0x84) |= 8;
            changed = 1;
        }
        if (*(s16*)((s32)wp + 0x5C) != values[6]) {
            changed = 1;
        }
    }

    work = wp;
    if (*(s16*)((s32)work + 0x5E) != values[7]) {
        changed = 1;
    }
    if (*(s16*)((s32)work + 0x60) != values[8]) {
        *(u32*)((s32)work + 0x84) |= 0x10;
        changed = 1;
    }

    if (changed == 0) {
        work = wp;
        value = *(u32*)((s32)work + 0x84);
        if (value != 0U) {
            *(u32*)((s32)work + 0x80) = value;
            *(s32*)((s32)wp + 0x84) = 0;
            *(s32*)((s32)wp + 0x88) = *(s32*)((s32)gp + 4) << 1;
        }
    } else {
        *(s32*)((s32)wp + 0x80) = 0;
    }

    return changed;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void statusGetValue(void* out) {
    extern void* gp;
    extern void* _battleWorkPointer;
    extern s32 pouchGetHP(void);
    extern s32 pouchGetMaxHP(void);
    extern s32 pouchGetFP(void);
    extern s32 pouchGetMaxFP(void);
    extern s32 pouchGetAP(void);
    extern s32 pouchGetMaxAP(void);
    extern s32 pouchGetStarPoint(void);
    extern s32 pouchGetCoin(void);
    extern void* pouchGetPtr(void);
    extern s32 marioBgmodeChk(void);
    extern s32 marioGetParty(void);
    extern void* marioGetPtr(void);
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 partyId);
    s16* values = out;

    if (*(s32*)((s32)gp + 0x14) == 0) {
        void* pouch;
        s32 partyId;
        s32 offset;

        values[0] = pouchGetHP();
        values[1] = pouchGetMaxHP();
        values[2] = pouchGetFP();
        values[3] = pouchGetMaxFP();
        values[9] = pouchGetAP();
        values[10] = pouchGetMaxAP();
        values[7] = pouchGetStarPoint();
        values[8] = pouchGetCoin();
        if (marioBgmodeChk() == 0) {
            partyId = (s16)marioGetParty();
        } else {
            partyId = *(s8*)((s32)marioGetPtr() + 0x247);
        }
        values[4] = partyId;

        pouch = pouchGetPtr();
        offset = values[4] * 0xE;
        values[5] = *(s16*)((s32)pouch + offset + 6);

        pouch = pouchGetPtr();
        offset = values[4] * 0xE;
        values[6] = *(s16*)((s32)pouch + offset + 2);
    } else {
        void* battleWork = _battleWorkPointer;
        void* mario = BattleGetMarioPtr(battleWork);
        void* party = BattleGetPartyPtr(battleWork);

        values[0] = *(s16*)((s32)mario + 0x10C);
        values[1] = *(s16*)((s32)mario + 0x108);
        values[2] = *(s16*)((s32)mario + 0x112);
        values[3] = *(s16*)((s32)mario + 0x10E);
        values[9] = pouchGetAP();
        values[10] = pouchGetMaxAP();
        values[7] = pouchGetStarPoint();
        values[8] = pouchGetCoin();
        if (party != 0) {
            values[4] = BattleTransPartyId(*(s32*)((s32)party + 8));
            values[5] = *(s16*)((s32)party + 0x10C);
            values[6] = *(s16*)((s32)party + 0x108);
        } else {
            values[4] = 0;
            values[5] = 0;
            values[6] = 0;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void statusWinReInit(void) {
    void* work = wp;
    s32 zero = 0;
    u16 flags;

    *(s32*)((s32)work + 0x74) = zero;
    *(s32*)((s32)wp + 0x78) = zero;

    flags = *(u16*)((s32)wp + 4);
    if ((flags & 0x10) != 0) {
        *(u16*)((s32)wp + 4) = flags & ~0x10;
        *(u16*)((s32)wp + 4) &= ~0xA;
        *(u16*)((s32)wp + 4) |= 5;
    } else if ((flags & 1) != 0) {
        *(u16*)((s32)wp + 4) &= ~0xA;
        *(u16*)((s32)wp + 4) |= 5;
    }

    flags = *(u16*)((s32)wp + 4);
    if ((flags & 0x20) != 0) {
        *(u16*)((s32)wp + 4) = flags & ~0x20;
        *(u16*)((s32)wp + 4) |= 0xA;
        *(u16*)((s32)wp + 4) &= ~5;
    } else if ((flags & 2) != 0) {
        *(u16*)((s32)wp + 4) |= 0xA;
        *(u16*)((s32)wp + 4) &= ~5;
    }
}

void statusWinForceOff(void) {
    u16 flags = *(u16*)((s32)wp + 4);

    if ((flags & 0x10) != 0) {
        *(u16*)((s32)wp + 4) = flags & ~0x10;
        *(u16*)((s32)wp + 4) &= ~0xA;
        *(u16*)((s32)wp + 4) |= 5;
    } else if ((flags & 1) != 0) {
        *(u16*)((s32)wp + 4) &= ~0xA;
        *(u16*)((s32)wp + 4) |= 5;
    }

    flags = *(u16*)((s32)wp + 4);
    if ((flags & 0x20) != 0) {
        *(u16*)((s32)wp + 4) = flags & ~0x20;
        *(u16*)((s32)wp + 4) |= 0xA;
        *(u16*)((s32)wp + 4) &= ~5;
    } else if ((flags & 2) != 0) {
        *(u16*)((s32)wp + 4) |= 0xA;
        *(u16*)((s32)wp + 4) &= ~5;
    }
}

void statusWinForceUpdate(void) {
    s16 values[11];

    statusGetValue(values);
    *(s16*)((s32)wp + 0x50) = values[0];
    *(s16*)((s32)wp + 0x52) = values[1];
    *(s16*)((s32)wp + 0x54) = values[2];
    *(s16*)((s32)wp + 0x56) = values[3];
    *(s16*)((s32)wp + 0x62) = values[9];
    *(s16*)((s32)wp + 0x64) = values[10];
    *(s16*)((s32)wp + 0x5E) = values[7];
    *(s16*)((s32)wp + 0x60) = values[8];
    *(s16*)((s32)wp + 0x58) = values[4];
    if (*(s16*)((s32)wp + 0x58) != 0) {
        *(s16*)((s32)wp + 0x5A) = values[5];
        *(s16*)((s32)wp + 0x5C) = values[6];
    }
}


void statusWinInit(void) {
    f32 one;
    f32 negX;
    f32 posY;
    f32 posY2;

    wp = __memAlloc(0, 0x8C);
    memset(wp, 0, 0x8C);

    one = float_1_80422bf4;
    negX = float_neg304_80422c68;
    posY = float_208_80422c6c;
    posY2 = float_334_80422c70;

    *(f32*)((s32)wp + 0x1C) = one;
    *(f32*)((s32)wp + 0x30) = negX;
    *(f32*)((s32)wp + 0x34) = posY;
    *(f32*)((s32)wp + 0x40) = negX;
    *(f32*)((s32)wp + 0x44) = posY2;
    *(s32*)((s32)wp + 0x74) = 0;
    *(s32*)((s32)wp + 0x78) = 0;
    *(u16*)((s32)wp + 4) &= ~0xA;
    *(u16*)((s32)wp + 4) |= 5;
}


void statusGetApPos(f32* out) {
    typedef struct StatusVec {
        f32 x;
        f32 y;
        f32 z;
    } StatusVec;
    StatusVec pos;
    s16 xOffset = alwaysDt[0x90 / 2];
    s16 yOffset = alwaysDt[0x92 / 2];
    void* work = wp;

    pos.x = *(f32*)((s32)work + 0x20) + xOffset;
    pos.y = *(f32*)((s32)work + 0x24) - yOffset;
    pos.z = float_0_80422be8;
    *(StatusVec*)out = pos;
}
