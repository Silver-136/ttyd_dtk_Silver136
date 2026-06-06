#include "countdown.h"

extern void* gp;
void* wp;

void* memset(void* dst, int value, u32 size);
s32 evtGetValue(void* event, s32 value);
s32 psndSFXOn(const char* name);
void psndSFXOff(s32 id);
extern const char str_SFX_STG2_BOMB_TIMER1_802fde48[];
extern const char str_SFX_STG7_ALART_WARNI_802fde60[];
extern f32 float_0_80426800;
extern f32 float_180_80426824;
extern f32 float_neg180_80426828;

void countDownSetRestartTime(s32 time) {
    *(s32*)((s32)wp + 0x1C) = time;
}

void countDownFlagOff(u16 flag) {
    *(u16*)wp &= ~flag;
}

void countDownFlagOn(u16 flag) {
    *(u16*)wp |= flag;
}

void countDownStop(void) {
    *(u16*)wp |= 8;
    psndSFXOff(*(s32*)((s32)wp + 0x38));
}

void countDownEnd(void) {
    if (*(u16*)wp & 1) {
        *(s32*)((s32)wp + 0x30) = 2;
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

void countDownInit(void) {
    wp = (void*)((s32)gp + 0x68);
    memset(wp, 0, 0x40);
}

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
