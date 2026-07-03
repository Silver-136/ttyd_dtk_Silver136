#include "action/ac_timing_a.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct TimingADispWork {
    char pad_00[0x14];
    f32 x;
    f32 y;
    char pad_1c[4];
    s32 timer;
    char pad_24[0x28];
    s32 wait;
} TimingADispWork;

s32 BattleActionCommandGetDifficulty(void);
extern void* _battleWorkPointer;
void* camGetPtr(s32 cameraId);
void iconDispGx(Vec3* pos, s32 flags, s32 iconId, f32 scale);
f32 intplGetValue(s32 mode, s32 current, s32 total, f32 start, f32 end);
void* memset(void* ptr, int value, u32 size);
extern Vec3 vec3_802f01c8[];
extern Vec3 vec3_802f01d4[];
extern f32 float_neg200_80422710;
extern f32 float_50_80422714;
extern f32 float_1_80422718;
extern f32 float_neg300_8042271c;
extern f32 float_30_80422720;
extern f32 float_0_80422724;
s32* _ac_timing_a_timing_tbl[4];

void battleAcDelete_TimingA(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3E8;
}

s32 battleAcTimingA_GetSuccessFrame(void* obj) {
    s32 difficulty;
    s32* entry;
    s32 frame;
    s32 result;

    difficulty = BattleActionCommandGetDifficulty();

    frame = *(s32*)((s32)obj + 0x1CB4);
    entry = *(s32**)((s32)_ac_timing_a_timing_tbl + difficulty * 4);
    result = entry[1];

    if ((*(s32*)((s32)obj + 0x1F50) & 2) != 0) {
        if (frame >= 100000) {
            return 0;
        }

loop:
        if (frame <= entry[0]) {
            goto done;
        }

        entry += 2;
        goto loop;

done:
        return entry[1];
    }

    return result;
}


s32 battleAcResult_TimingA(void* battleWork) {
    extern s32 BattlePadCheckRecordTrigger(s32, s32);
    extern s32 irand(s32);
    s32 successFrame;
    s32 count;
    s32 i;

    successFrame = battleAcTimingA_GetSuccessFrame(battleWork);
    count = 0;

    if ((*(u32*)((s32)battleWork + 0x1C94) & 1) != 0 &&
        (*(u32*)((s32)*(void**)((s32)battleWork + 0x1C90) + 0x27C) & 0x10) != 0) {
        if (irand(100) >= 0) {
            *(s32*)((s32)battleWork + 0x1CB8) = 0;
        } else {
            *(s32*)((s32)battleWork + 0x1CB8) = 2;
            *(s32*)((s32)battleWork + 0x1CB4) = *(s32*)((s32)battleWork + 0x1CB4) + 1;
        }
        return *(s32*)((s32)battleWork + 0x1CB8);
    }

    if (*(u8*)((s32)*(void**)((s32)battleWork + 0x1C90) + 0x307) != 0) {
        *(s32*)((s32)battleWork + 0x1CB8) = 2;
        *(s32*)((s32)battleWork + 0x1CB4) = *(s32*)((s32)battleWork + 0x1CB4) + 1;
        return *(s32*)((s32)battleWork + 0x1CB8);
    }

    *(s32*)((s32)battleWork + 0x1CB8) = 0;
    if ((*(u32*)((s32)battleWork + 0xEF4) & 0x01000000) != 0) {
        *(s32*)((s32)battleWork + 0x1CE8) = 3;
        *(s32*)((s32)battleWork + 0x1CB8) = 2;
        *(s32*)((s32)battleWork + 0x1CB4) = *(s32*)((s32)battleWork + 0x1CB4) + 1;
        return *(s32*)((s32)battleWork + 0x1CB8);
    }

    for (i = 0; i < 0x14; i++) {
        if (BattlePadCheckRecordTrigger(i, 0x100) != 0) {
            count++;
        }
    }

    if (count >= 2) {
        *(s32*)((s32)battleWork + 0x1CE8) = 1;
        return *(s32*)((s32)battleWork + 0x1CB8);
    }

    for (i = 0; i < successFrame; i++) {
        if (BattlePadCheckRecordTrigger(i, 0x100) != 0) {
            break;
        }
    }

    if (i >= successFrame) {
        if (count > 0) {
            *(s32*)((s32)battleWork + 0x1CE8) = 1;
        } else {
            *(s32*)((s32)battleWork + 0x1CE8) = 2;
        }
        return *(s32*)((s32)battleWork + 0x1CB8);
    }

    *(s32*)((s32)battleWork + 0x1CE8) = 3;
    *(s32*)((s32)battleWork + 0x1CB8) = 2;
    *(s32*)((s32)battleWork + 0x1CB4) = *(s32*)((s32)battleWork + 0x1CB4) + 1;
    return *(s32*)((s32)battleWork + 0x1CB8);
}

void actionCommandDisp(f32 x, f32 y) {
    void* battleWork;
    volatile s32* dispCount;
    Vec3 posA;
    Vec3 posB;

    battleWork = _battleWorkPointer;
    dispCount = (volatile s32*)((s32)battleWork + 0x1F20);
    camGetPtr(8);

    if ((*(u32*)((s32)battleWork + 0x1F50) & 1) != 0) {
        posA = vec3_802f01c8[0];
        posA.x = float_neg200_80422710 + x;
        posA.y = float_50_80422714 + y;
        iconDispGx(&posA, 0x10, 0x6D, float_1_80422718);
    } else {
        posB = vec3_802f01d4[0];
        posB.x = float_neg200_80422710 + x;
        posB.y = float_50_80422714 + y;
        iconDispGx(&posB, 0x10, 0x6C, float_1_80422718);
    }

    *dispCount += 1;
}


s32 battleAcMain_TimingA(int param_1) {
    void* work;
    TimingADispWork* dispWork;
    volatile s32* wait;

    work = (void*)param_1;
    wait = (volatile s32*)(param_1 + 0x1F4C);
    dispWork = (TimingADispWork*)(param_1 + 0x1F20);

    switch (*(s32*)((s32)work + 0x1C9C)) {
        case 0:
            memset(dispWork, 0, 0x2C);
            dispWork->x = float_neg300_8042271c;
            dispWork->y = float_0_80422724;
            dispWork->timer = 0x14;
            *(s32*)((s32)work + 0x1C9C) = 0x63;
            *(u32*)((s32)work + 0x1F50) &= ~1;
            *(s32*)((s32)work + 0x1CE8) = 0;
            break;
        case 0x64:
            break;
        case 0x3E8:
            wait[2] = 0x3C;
            *(s32*)((s32)work + 0x1C9C) = 0x3E9;
            /* fallthrough */
        case 0x3E9:
            wait[2] -= 1;
            if (wait[2] <= 0) {
                *(s32*)((s32)work + 0x1C9C) = 0x3EA;
            }
            break;
        case 0x3EA:
            *(s32*)((s32)work + 0x1CA0) = 0;
            *(s32*)((s32)work + 0x1CA8) = 0;
            *(s32*)((s32)work + 0x1CA4) = 0;
            *(s32*)((s32)work + 0x1CAC) = 0;
            return 0;
        default:
            break;
    }

    return 1;
}


void battleAcDisp_TimingA(s32 param_1, void* battleWork) {
    volatile TimingADispWork* dispWork;
    s32 timer;

    dispWork = (volatile TimingADispWork*)((s32)battleWork + 0x1F20);

    switch (*(s32*)((s32)battleWork + 0x1C9C)) {
        case 0x63:
            timer = dispWork->timer;
            dispWork->x = intplGetValue(4, 0x14 - timer, 0x14,
                                        float_neg300_8042271c, float_30_80422720);
            /* fallthrough */
        case 0x64:
            dispWork->x = float_30_80422720;
            actionCommandDisp(dispWork->x, dispWork->y);
            timer = dispWork->timer;
            if (timer > 0) {
                dispWork->timer = timer - 1;
            }
            break;
        case 0x3E8:
        case 0x3E9:
            timer = dispWork->timer;
            if (timer >= 0x28) {
                dispWork->x = intplGetValue(4, timer - 0x28, 0x14,
                                            float_30_80422720, float_neg300_8042271c);
            } else {
                dispWork->x = float_30_80422720;
            }
            actionCommandDisp(dispWork->x, dispWork->y);
            timer = dispWork->timer;
            if (timer < 0x3C) {
                dispWork->timer = timer + 1;
            }
            break;
        default:
            break;
    }
}
