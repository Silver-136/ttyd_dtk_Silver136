#include "countdown.h"

extern void* gp;
void* wp;

s32 evtGetValue(void* event, s32 value);
s32 psndSFXOn(const char* name);
void psndSFXOff(s32 id);
extern const char str_SFX_STG2_BOMB_TIMER1_802fde48[];
extern const char str_SFX_STG7_ALART_WARNI_802fde60[];
extern f32 float_0_80426800;
extern f32 float_180_80426824;
extern f32 float_neg180_80426828;

void countDownInit(void) {
    wp = (void*)((s32)gp + 0x68);
    memset(wp, 0, 0x40);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void countDownSaveReStart(void) {
    void* work = wp;
    s64 elapsed;
    u32 ticksPerMs;
    s32 remaining;
    s32 restartTime;
    s32 startTime;

    if ((*(u16*)work & 1) != 0) {
        ticksPerMs = (((*(u32*)0x800000F8 >> 2) * 0x10624DD3U) >> 6);
        elapsed = *(s64*)((s32)work + 8) / (s32)ticksPerMs;
        startTime = *(s32*)((s32)work + 0x18);
        remaining = startTime - (s32)elapsed;
        if (remaining < 0) {
            remaining = 0;
        }
        restartTime = *(s32*)((s32)work + 0x1C);
        if ((u32)remaining <= (u32)restartTime) {
            *(u64*)((s32)work + 8) = (u64)ticksPerMs * (u32)(startTime - restartTime);
        }

        *(s32*)((s32)wp + 0x10) = *(s32*)((s32)gp + 0x38);
        *(s32*)((s32)wp + 0x14) = *(s32*)((s32)gp + 0x3C);
        if ((s32)evtGetValue(0, (s32)0xF5DE0180) < 0x160) {
            *(s32*)((s32)wp + 0x38) = psndSFXOn(str_SFX_STG2_BOMB_TIMER1_802fde48);
        } else {
            *(s32*)((s32)wp + 0x38) = psndSFXOn(str_SFX_STG7_ALART_WARNI_802fde60);
        }
        *(u16*)wp &= ~2;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void countDownStart(s32 time, s32 restartTime) {
    s32 sfxTime;
    void* work;
    void* global;
    s32 camA;
    s32 camB;
    f32 negAngle;
    f32 angle;
    f32 zero;

    zero = float_0_80426800;
    angle = float_180_80426824;
    *(s32*)((s32)wp + 0x18) = time;
    negAngle = float_neg180_80426828;
    *(s32*)((s32)wp + 0x1C) = restartTime;
    global = gp;
    work = wp;
    camA = *(s32*)((s32)global + 0x38);
    camB = *(s32*)((s32)global + 0x3C);
    *(s32*)((s32)work + 0x14) = camB;
    *(s32*)((s32)work + 0x10) = camA;
    work = wp;
    *(s32*)((s32)work + 0xC) = 0;
    *(s32*)((s32)work + 0x8) = 0;
    *(s32*)((s32)wp + 0x30) = 0;
    *(s32*)((s32)wp + 0x34) = 0;
    *(f32*)((s32)wp + 0x28) = zero;
    *(f32*)((s32)wp + 0x2C) = zero;
    *(f32*)((s32)wp + 0x20) = angle;
    *(f32*)((s32)wp + 0x24) = negAngle;
    *(u16*)wp = 0;
    *(u16*)wp |= 1;

    sfxTime = (s32)evtGetValue(0, (s32)0xF5DE0180);
    if (sfxTime < 0x160) {
        *(s32*)((s32)wp + 0x38) = psndSFXOn(str_SFX_STG2_BOMB_TIMER1_802fde48);
    } else {
        *(s32*)((s32)wp + 0x38) = psndSFXOn(str_SFX_STG7_ALART_WARNI_802fde60);
    }
}

void countDownSetRestartTime(s32 time) {
    *(s32*)((s32)wp + 0x1C) = time;
}

/* stub-fill: countDownMain | missing_definition | ghidra_signature */
void countDownMain(void) {
    extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 time, s32 maxTime);
    extern s32 marioStGetSystemLevel(void);
    extern void dispEntry(s32 cameraId, s32 mode, void* callback, void* param, f32 z);
    extern void countDownDisp(void);
    extern f32 float_300_8042681c;
    extern f32 float_1000_80426820;
    void* work = wp;
    s32 state;
    s32 frame;
    s32 delta;
    u64 elapsed;
    u32 ticksPerMs;

    if ((*(u16*)work & 1) == 0) {
        return;
    }

    state = *(s32*)((s32)work + 0x30);
    switch (state) {
        case 0:
            frame = *(s32*)((s32)work + 0x34) + 1;
            *(s32*)((s32)work + 0x34) = frame;
            if (frame < 30) {
                *(f32*)((s32)work + 0x28) =
                    intplGetValue(float_300_8042681c, float_0_80426800, 0xB, frame, 30);
            } else {
                *(s32*)((s32)work + 0x34) = 0;
                *(s32*)((s32)work + 0x30) += 1;
            }
            break;
        case 1:
            if ((*(u16*)work & 2) != 0) {
                psndSFXOff(*(s32*)((s32)work + 0x38));
                *(s32*)((s32)work + 0x38) = -1;
            }
            break;
        case 2:
            frame = *(s32*)((s32)work + 0x34) + 1;
            *(s32*)((s32)work + 0x34) = frame;
            if (frame < 30) {
                *(f32*)((s32)work + 0x28) =
                    intplGetValue(float_0_80426800, float_300_8042681c, 0xB, frame, 30);
            } else {
                *(s32*)((s32)work + 0x34) = 0;
                *(s32*)((s32)work + 0x30) += 1;
                *(u16*)work &= ~1;
                psndSFXOff(*(s32*)((s32)work + 0x38));
            }
            break;
    }

    if ((*(u16*)wp & 8) == 0 && marioStGetSystemLevel() == 0) {
        work = wp;
        delta = *(s32*)((s32)gp + 0x3C) - *(s32*)((s32)work + 0x14);
        *(u64*)((s32)work + 8) += (s64)delta;
    }

    work = wp;
    *(s32*)((s32)work + 0x10) = *(s32*)((s32)gp + 0x38);
    *(s32*)((s32)work + 0x14) = *(s32*)((s32)gp + 0x3C);
    ticksPerMs = (((*(u32*)0x800000F8 >> 2) * 0x10624DD3U) >> 6);
    elapsed = *(u64*)((s32)work + 8) / ticksPerMs;
    if (elapsed >= (u32)*(s32*)((s32)work + 0x18)) {
        *(u16*)work |= 2;
    }
    dispEntry(8, 0, countDownDisp, 0, float_1000_80426820);
}

/* CHATGPT STUB FILL: main/countdown 20260624_184008 */

/* stub-fill: countDownDisp | missing_definition | ghidra_signature */
void countDownDisp(void) {
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern u8 iconDispGxCol(void*, s32, s32, void*);
    extern void* wp;
    u8* work = wp;
    f32 mtx[3][4];
    f32 scale[3][4];
    f32 trans[3][4];
    s32 digits[3];
    volatile s32 baseColor0 = -1;
    volatile s32 baseColor1 = -1;
    volatile s32 baseColor2 = -1;
    s32 headerColor;
    s32 digitColor;
    s32 separatorColor;
    s32 millisColor;
    s32 remaining;
    s32 seconds;
    s32 millis;
    u32 ticksPerMs;
    s32 i;
    s32 x;
    s32 q;
    s32* digit;

    ticksPerMs = (u32)(((u64)(*(u32*)0x800000F8 >> 2) * 0x10624DD3U) >> 38);
    remaining = *(s32*)(work + 0x18) -
                (s32)(*(s64*)(work + 8) / ticksPerMs);
    if ((*(u16*)work & 4) == 0) {
        if ((*(u16*)work & 2) != 0) {
            remaining = 0;
        }

        q = remaining / 1000 + (remaining >> 31);
        seconds = q - (q >> 31);
        millis = remaining + seconds * -1000;
        PSMTXTrans(trans,
                   *(f32*)(work + 0x20) + *(f32*)(work + 0x28),
                   *(f32*)(work + 0x24) + *(f32*)(work + 0x2C),
                   0.0f);
        PSMTXScale(scale, 2.5f, 1.0f, 1.0f);
        PSMTXConcat(trans, scale, mtx);
        headerColor = -103;
        iconDispGxCol(mtx, 0x10, 0x1FD, &headerColor);

        q = seconds / 100 + (seconds >> 31);
        digits[0] = q - (q >> 31);
        seconds = seconds + digits[0] * -100;
        q = seconds / 10 + (seconds >> 31);
        digits[1] = q - (q >> 31);
        digits[2] = seconds + digits[1] * -10;
        x = -50;
        digit = digits;
        for (i = 0; i < 3; i++, digit++, x += 30) {
            PSMTXTrans(mtx, (f32)x, 8.0f, 0.0f);
            PSMTXConcat(trans, mtx, mtx);
            digitColor = baseColor2;
            iconDispGxCol(mtx, 0x10, *digit + 0x1FE, &digitColor);
        }

        PSMTXTrans(mtx, 28.0f, 10.0f, 0.0f);
        PSMTXConcat(trans, mtx, mtx);
        separatorColor = baseColor1;
        iconDispGxCol(mtx, 0x10, 0x208, &separatorColor);

        q = millis / 100 + (millis >> 31);
        digits[0] = q - (q >> 31);
        millis = millis + digits[0] * -100;
        q = millis / 10 + (millis >> 31);
        digits[1] = q - (q >> 31);
        x = 40;
        digit = digits;
        for (i = 0; i < 2; i++, digit++, x += 15) {
            PSMTXTrans(mtx, (f32)x, 8.0f, 0.0f);
            PSMTXScale(scale, 0.5f, 0.5f, 0.5f);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(trans, mtx, mtx);
            millisColor = baseColor0;
            iconDispGxCol(mtx, 0x10, *digit + 0x1FE, &millisColor);
        }
    }
}

s32 countDownGetStatus(void) {
    u32 flags = *(u16*)wp;
    if (flags & 1) {
        goto active;
    }
    return -1;
active:
    return (flags >> 1) & 1;
}

void countDownEnd(void) {
    if (*(u16*)wp & 1) {
        *(s32*)((s32)wp + 0x30) = 2;
    }
}

void countDownStop(void) {
    *(u16*)wp |= 8;
    psndSFXOff(*(s32*)((s32)wp + 0x38));
}

void countDownFlagOn(u16 flag) {
    *(u16*)wp |= flag;
}

void countDownFlagOff(u16 flag) {
    *(u16*)wp &= ~flag;
}

const u64 double_to_int_802fde40 = 0x4330000080000000ULL;
const char str_SFX_STG2_BOMB_TIMER1_802fde48[24] = "SFX_STG2_BOMB_TIMER1";
const char str_SFX_STG7_ALART_WARNI_802fde60[] = "SFX_STG7_ALART_WARNING1";
