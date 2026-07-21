#include "event/evt_batstage.h"

typedef struct BatstageVec3 {
    f32 x;
    f32 y;
    f32 z;
} BatstageVec3;

#define COPY_BATSTAGE_NAMES(dstBase, srcBase)       \
    do {                                            \
        u32* copySrc = (u32*)((s32)(srcBase));      \
        u32 copyA;                                  \
        u32 copyB;                                  \
        u32* copyDst = (u32*)((s32)(dstBase));      \
        s32 copyI;                                  \
        for (copyI = 0; copyI < 0xC; copyI++) {     \
            copyA = copySrc[0];                     \
            copyB = copySrc[1];                     \
            copyDst[0] = copyA;                     \
            copyDst[1] = copyB;                     \
            copySrc += 2;                           \
            copyDst += 2;                           \
        }                                           \
    } while (0)
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_batstage_set_stg_dark(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void BattleStageSetStgDark(u8 id, s32 value1, s32 value2);
    s32* args = *(s32**)((s32)event + 0x18);
    u8 id = evtGetValue(event, args[0]);
    s32 value1 = evtGetValue(event, args[1]);
    s32 value2 = evtGetValue(event, args[2]);

    BattleStageSetStgDark(id, value1, value2);
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_batstage_set_stg_dark_base(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void BattleStageSetStgDarkBase(u8 id, s32 value1, s32 value2);
    s32* args = *(s32**)((s32)event + 0x18);
    u8 id = evtGetValue(event, args[0]);
    s32 value1 = evtGetValue(event, args[1]);
    s32 value2 = evtGetValue(event, args[2]);

    BattleStageSetStgDarkBase(id, value1, value2);
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_batstage_return_stg_dark_base(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void BattleStageReturnStgDarkBase(s32 value1, s32 value2);

    s32* args = *(s32**)((s32)event + 0x18);
    s32 value1 = evtGetValue(event, args[0]);
    s32 value2 = evtGetValue(event, args[1]);

    BattleStageReturnStgDarkBase(value1, value2);
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_batstage_set_aud_dark(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void BattleStageSetAudDark(u8 id, s32 value1, s32 value2);
    s32* args = *(s32**)((s32)event + 0x18);
    u8 id = evtGetValue(event, args[0]);
    s32 value1 = evtGetValue(event, args[1]);
    s32 value2 = evtGetValue(event, args[2]);

    BattleStageSetAudDark(id, value1, value2);
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_batstage_set_aud_dark_base(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void BattleStageSetAudDarkBase(u8 id, s32 value1, s32 value2);
    s32* args = *(s32**)((s32)event + 0x18);
    u8 id = evtGetValue(event, args[0]);
    s32 value1 = evtGetValue(event, args[1]);
    s32 value2 = evtGetValue(event, args[2]);

    BattleStageSetAudDarkBase(id, value1, value2);
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_batstage_return_aud_dark_base(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void BattleStageReturnAudDarkBase(s32 value1, s32 value2);

    s32* args = *(s32**)((s32)event + 0x18);
    s32 value1 = evtGetValue(event, args[0]);
    s32 value2 = evtGetValue(event, args[1]);

    BattleStageReturnAudDarkBase(value1, value2);
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


s32 evt_batstage_light_set_player_on(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 BattleStageGetLightNumberByName(char* name);
    extern s32 BattleStageSysLightEntry(char* name);
    extern void BattleStageLightSetLightColor(s32 lightId, u8 r, u8 g, u8 b, s32 fadeFrames, s32 unk);
    extern void BattleStageLightSetWidth(s32 lightId, s32 width, s32 fadeFrames, s32 unk);
    extern void BattleStageLightSetPosition(s32 lightId, f32 x, f32 y, f32 z, s32 unk1, s32 unk2);
    extern void BattleStageLightSetUpVec(s32 lightId, f32 x, f32 y, f32 z, s32 unk1, s32 unk2);
    extern void BattleStageLightSetTarget(s32 lightId, f32 x, f32 y, f32 z, s32 unk1, s32 unk2);
    extern void BattleStageLightSetTargetByUnit(s32 lightId, f32 x, f32 y, f32 z, s32 unitId, s32 fadeFrames, s32 unk);
    extern void BattleStageSetStgDark(s32 value1, s32 value2, s32 value3);
    extern void BattleStageSetAudDark(s32 value1, s32 value2, s32 value3);
    extern char dat_802f38f8[];
    extern u8 unk_804295d8[];
    extern void* _battleWorkPointer;

    s32* args;
    s32 mode;
    char* base;
    register void* battleWork;

    u8 active[3];
    u8 colors[9];

    BatstageVec3 pos[3];
    BatstageVec3 target[3];
    BatstageVec3 unitTarget[3];
    BatstageVec3 up[3];

    char names[3][0x20];

    u8* activePtr;
    char* namePtr;
    u8* colorPtr;
    BatstageVec3* posPtr;
    BatstageVec3* upPtr;
    BatstageVec3* targetPtr;
    BatstageVec3* unitTargetPtr;

    s32 i;
    s32 lightId;

    args = *(s32**)((s32)event + 0x18);
    base = dat_802f38f8;
    battleWork = _battleWorkPointer;

    mode = evtGetValue(event, args[0]);

    COPY_BATSTAGE_NAMES(names, base);

    pos[0] = *(BatstageVec3*)((s32)base + 0x60);
    pos[1] = *(BatstageVec3*)((s32)base + 0x6C);
    pos[2] = *(BatstageVec3*)((s32)base + 0x78);

    target[0] = *(BatstageVec3*)((s32)base + 0x84);
    target[1] = *(BatstageVec3*)((s32)base + 0x90);
    target[2] = *(BatstageVec3*)((s32)base + 0x9C);

    unitTarget[0] = *(BatstageVec3*)((s32)base + 0xA8);
    unitTarget[1] = *(BatstageVec3*)((s32)base + 0xB4);
    unitTarget[2] = *(BatstageVec3*)((s32)base + 0xC0);

    up[0] = *(BatstageVec3*)((s32)base + 0xCC);
    up[1] = *(BatstageVec3*)((s32)base + 0xD8);
    up[2] = *(BatstageVec3*)((s32)base + 0xE4);

    *(u32*)((s32)colors + 0) = *(u32*)((s32)base + 0xF0);
    *(u32*)((s32)colors + 4) = *(u32*)((s32)base + 0xF4);
    *(u8*)((s32)colors + 8) = *(u8*)((s32)base + 0xF8);

    *(u16*)((s32)active + 0) = *(u16*)((s32)unk_804295d8 + 0);
    active[2] = *(u8*)((s32)unk_804295d8 + 2);

    if (mode == 1) {
        active[2] = 1;
        colors[6] = 0xFF;
        colors[7] = 0xFF;
        colors[8] = 0xFF;
    } else {
        active[0] = 1;
        active[1] = 1;
        active[2] = 1;
    }

    activePtr = active;
    namePtr = (char*)names;
    colorPtr = colors;
    posPtr = pos;
    upPtr = up;
    targetPtr = target;
    unitTargetPtr = unitTarget;

    for (i = 0; i < 3; i++) {
        if (*activePtr != 0) {
            lightId = BattleStageGetLightNumberByName(namePtr);
            if (lightId == -1) {
                lightId = BattleStageSysLightEntry(namePtr);
            }

            BattleStageLightSetLightColor(
                lightId,
                colorPtr[0],
                colorPtr[1],
                colorPtr[2],
                0x1E,
                0
            );

            BattleStageLightSetWidth(lightId, 0x78, 1, 0);

            BattleStageLightSetPosition(
                lightId,
                posPtr->x,
                posPtr->y,
                posPtr->z,
                0,
                0
            );

            BattleStageLightSetUpVec(
                lightId,
                upPtr->x,
                upPtr->y,
                upPtr->z,
                0,
                0
            );

            BattleStageLightSetTarget(
                lightId,
                targetPtr->x,
                targetPtr->y,
                targetPtr->z,
                0,
                0
            );

            BattleStageLightSetTargetByUnit(
                lightId,
                unitTargetPtr->x,
                unitTargetPtr->y,
                unitTargetPtr->z,
                *(s32*)((s32)battleWork + 0x420),
                0x1E,
                5
            );
        }

        activePtr++;
        namePtr += 0x20;
        colorPtr += 3;
        posPtr++;
        upPtr++;
        targetPtr++;
        unitTargetPtr++;
    }

    BattleStageSetStgDark(0x8C, 0x1E, 0);
    BattleStageSetAudDark(0x8C, 0, 0);

    return 2;
}
s32 evt_batstage_light_set_player_off(void* event, s32 isFirstCall) {
    extern void BattleStageReturnAudDarkBase(s32 value1, s32 value2);
    extern void BattleStageReturnStgDarkBase(s32 value1, s32 value2);
    extern s32 BattleStageGetLightNumberByName(const char* name);
    extern void BattleStageLightRelease(s32 lightId, s32 fadeFrames, s32 unk);
    extern char dat_802f38f8[];

    char* base = dat_802f38f8;
    s32 lightId;

    BattleStageReturnAudDarkBase(0, 0);
    BattleStageReturnStgDarkBase(0x1E, 0);

    lightId = BattleStageGetLightNumberByName(base + 0xFC);
    if (lightId != -1) {
        BattleStageLightRelease(lightId, 0x1E, 0);
    }

    lightId = BattleStageGetLightNumberByName(base + 0x110);
    if (lightId != -1) {
        BattleStageLightRelease(lightId, 0x1E, 0);
    }

    lightId = BattleStageGetLightNumberByName(base + 0x124);
    if (lightId != -1) {
        BattleStageLightRelease(lightId, 0x1E, 0);
    }

    return 2;
}
