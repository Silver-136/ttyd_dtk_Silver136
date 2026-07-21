#include "battle/battle_audience.h"
extern void* _battleWorkPointer;

extern void psndSFXOn(const char* name);
extern s32 irand(s32 max);
extern s32 psndSFXOn_3D(const char* name, void* pos);
extern void psndSFXOff(s32 id);
extern void psndSFX_vol(s32 id, s32 vol);
extern s32 psndSFX_get_vol(s32 sfxId);
extern char* strcpy(char* dst, const char* src);
extern void psndSFX_pit(s32 sfxId, s32 pitch);

extern const char str_btl_msg_audience_pun_802f9e28[];
extern s32 haiti_data[];

extern void* pouchGetPtr(void);
extern s32 irand(s32 max);
extern s32 psndSFX_get_vol(s32 sfxId);
extern void* pouchGetPtr(void);
extern s32 unk_801a21e0(void);
extern s32 unk_801a23e0(void);

extern s32 BattleAudience_Entry(s32 id, s32 type, s32 arg);
extern void BattleAudienceAddAudienceNum(s32 num);
extern s32 BattleAudience_GetEscapeChangeOK(s32 id);
extern void BattleAudience_ChangeStatus(s32 id, s32 status);
s32 BattleAudience_GetExist(s32 id);
s32 BattleAudience_GetSysCtrl(s32 id);
void BattleAudience_WinSetActive(s32 active);
u8 BattleAudienceCtrlProcess(void);
u8 BattleAudienceItemCtrlProcess(void);
u8 BattleAudienceApSrcCtrlProcess(void);
u8 BattleAudienceWinCtrlProcess(void);
u8 BattleAudienceSoundMain(void);
u8 BattleAudienceAnimProcess(void);
u8 BattleAudienceDispAudience(void);
u8 BattleAudienceDispItem(void);
u8 BattleAudienceDispApSrc(void);
u8 BattleAudienceDispWin(void);
s32 BattleAudience_NoUsedFCHaitiRand(void);
void BattleAudienceSoundStop(s32 id);
void BattleAudience_SetAnim(s32 memberIdx, s32 animIdx, s32 restart);
s32 BattleAudience_GetAudienceNum(void);
s32 BattleAudience_GetPPAudienceNum_L(void);
s32 BattleAudience_GetPPAudienceNum_R(void);
s32 BattleAudience_GetPPAudienceNum_Sub(s32 id);
s32 BattleAudienceNumToTargetSub(void);
u8 BattleAudienceAddTargetNumSub(void);
void BattleAudienceSoundCheer(s32 vol, s32 frames);
s32 BattleAudienceSound1(const char* name, s32 kind, s32 arg);
s32 BattleAudienceSound2(const char* name, s32 kind);

#define BA_SOUND_INIT(sound, duration, delay)                         \
    do {                                                              \
        s32 vol;                                                      \
        *(u32*)(sound) |= 1;                                          \
        *(s32*)((s32)(sound) + 0xC) = (duration);                     \
        *(s32*)((s32)(sound) + 0x10) = (delay);                       \
        vol = psndSFX_get_vol(*(s32*)((s32)(sound) + 0x4));           \
        *(u8*)((s32)(sound) + 0x1D) = vol;                            \
        *(u8*)((s32)(sound) + 0x1C) = vol;                            \
        *(s32*)((s32)(sound) + 0x18) = 0;                             \
        *(s32*)((s32)(sound) + 0x14) = 0;                             \
        *(u8*)((s32)(sound) + 0x1E) = 0x64;                           \
    } while (0)

extern const f32 float_0_80424988;
extern const f32 float_1_80424990;
extern const f32 float_26_804249c0;
extern const f32 float_100_804249bc;
extern const f32 float_200_804249b8;
extern const f32 float_250_804249c4;
extern const f32 float_75_804249c8;
extern void BattleAudienceSoundClap(s32 vol, s32 frames);
extern void BattleAudienceSoundCheerKind(s32 kind, s32 vol, s32 frames);
extern void BattleAudienceSoundBooingKind(s32 kind);
extern void BattleStatusWindowAPRecoveryOn(void);

extern const char str_SFX_AUDIENCE_PAKU_EA_802fa338[];
extern const char str_SFX_AUDIENCE_BOMB_BU_802fa350[];
extern const char str_SFX_AUDIENCE_BOMB_RE_802fa36c[];
extern const char str_SFX_AUDIENCE_SHELL_T_802fa388[];
extern const char str_SFX_AUDIENCE_PANSY_S_802fa3a8[];
extern const char str_SFX_AUDIENCE_ZZZ1_802fa3c4[];
extern const char str_SFX_AUDIENCE_HANDBEA_802fa474[];
extern const char str_SFX_AUDIENCE_CALL_AL_802fa48c[];
extern const char str_SFX_AUDIENCE_CALL_AL_802fa4a4[];

extern void weaponGetPowerFromMarioHammerLv(void);
extern void BattleAudienceJoy(s32 kind);
extern s32 BattleAudience_GetAudienceNoFromOffset(s32 id, s32 offset, s32 arg);
extern void BattleAudienceSoundGetInfo1(s32 kind, s32 arg, u8* vol, void* pos);
extern void BattleAudienceSoundGetInfo2(s32 kind, void* pos);
extern void BattleAudienceSoundBooing(void);

extern const f32 float_neg0p05_80424984;
extern const f32 float_0p02_8042498c;
extern const f32 float_0p01_80424998;
extern const f32 float_5_8042499c;
extern const f32 float_0p15_804249a0;
extern const f32 float_neg0p2_804249a8;
extern const f32 float_neg0p25_804249b0;
extern void BattleAudienceAddTargetNum(f32 value, f32 unused);
extern void BattleAudienceAddPhaseEvtList(s32 id);
extern s32 BattleAudience_GetPPAudienceNum_RL_Sub(s32 side);
extern void BattleAudienceSoundWhistleKind(s32 kind);

extern const f32 float_0_80424988;
extern const f32 float_0p05_80424994;
extern const f32 float_neg0p1_804249a4;

typedef struct BattleAudienceWorkPartial {
    u8 pad_000[0x48];
    f32 posX;          // 0x48
    f32 posY;          // 0x4C
    f32 posZ;          // 0x50

    u8 pad_054[0x9C - 0x54];
    f32 rotX;          // 0x9C
    f32 rotY;          // 0xA0
    f32 rotZ;          // 0xA4

    f32 rotOffsetX;    // 0xA8
    f32 rotOffsetY;    // 0xAC
    f32 rotOffsetZ;    // 0xB0
} BattleAudienceWorkPartial;

void tplRead(char* tplName) {
    extern s32 getMarioStDvdRoot(void);
    extern void* fileAllocf(s32, const char*, ...);
    extern const char str_PCTs_PCTs_80424a88[];

    fileAllocf(4, str_PCTs_PCTs_80424a88, getMarioStDvdRoot(), tplName);
}

void* BattleAudienceBaseGetPtr(void) {
    return (void*)((s32)_battleWorkPointer + 0x2858);
}

void* BattleAudienceGetPtr(s32 id) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    return (void*)((s32)base + id * 0x134 + 0x1BC);
}

void* BattleAudienceItemGetPtr(s32 id) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    return (void*)((s32)base + id * 0x48 + 0xF25C);
}

void* BattleAudienceWinGetPtr(void) {
    return (void*)((s32)BattleAudienceBaseGetPtr() + 0x1375C);
}

void* BattleAudienceSoundGetPtr(s32 id) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    return (void*)((s32)base + id * 0x24 + 0x133FC);
}

void BattleAudience_Init(void) {
    extern void* memset(void* dest, s32 val, u32 size);
    extern void BattleAudienceSettingAudience(void);
    extern void* tplRead(const char* path);
    extern void BattleAudienceGuestTPLRead(s32 guestId, s32 kind, const char* path);
    extern void BattleAudienceSetThrowItemMax(void);
    extern void BattleAudienceNumToTarget(void);
    extern void BattleAudienceSoundSetVol(s32 id, s32 vol, s32 frames);
    extern u8 BattleAudienceSoundNoiseAlways(void);
    extern const char str_battle_audience_audi_802fa710[];
    extern const char str_audience_luigi_tpl_802fa734[];
    void* base;
    void* win;
    void* audience;
    s32 row;
    s32 col;
    s32 id;

    base = BattleAudienceBaseGetPtr();
    win = BattleAudienceWinGetPtr();
    pouchGetPtr();

    memset(base, 0, 0x13914);
    BattleAudienceSettingAudience();
    *(void**)((s32)base + 0xC) = tplRead(str_battle_audience_audi_802fa710);
    BattleAudienceGuestTPLRead(0, 0xC, str_audience_luigi_tpl_802fa734);
    BattleAudienceSetThrowItemMax();

    *(s32*)((s32)base + 0x13908) = 0;
    *(s32*)((s32)base + 0x1378C) = 0;
    *(s32*)((s32)base + 0x13788) = 0;
    *(s32*)((s32)base + 0x13784) = 0;

    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            id = (row * 20) + col + 10;
            if ((u8)BattleAudience_GetExist(id) == 1) {
                audience = BattleAudienceGetPtr(id);
                if (*(u8*)((s32)audience + 0x19) != 0xC && *(u8*)((s32)audience + 0x19) != 0xB) {
                    (*(s32*)((s32)base + 0x13788))++;
                }
            }
        }
    }

    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            id = (row * 20) + col;
            if ((u8)BattleAudience_GetExist(id) == 1) {
                audience = BattleAudienceGetPtr(id);
                if (*(u8*)((s32)audience + 0x19) != 0xC && *(u8*)((s32)audience + 0x19) != 0xB) {
                    (*(s32*)((s32)base + 0x1378C))++;
                }
            }
        }
    }

    *(s32*)((s32)base + 0x13784) =
        *(s32*)((s32)base + 0x13788) + *(s32*)((s32)base + 0x1378C);
    *(f32*)((s32)win + 0x18) = *(s32*)((s32)base + 0x13784);

    BattleAudienceSoundNoiseAlways();
    BattleAudienceSoundSetVol(2, 0x3C, 0xB4);
    *(s32*)((s32)base + 0x13910) = 0;
    BattleAudienceNumToTarget();
}

void BattleAudience_Main(void) {
    extern void* g_BattleWork;
    u8* battle;
    u8* base;

    battle = g_BattleWork;
    base = BattleAudienceBaseGetPtr();
    if ((*(u32*)base & 0x10000) == 0 && (*(u32*)(battle + 0x4) & 0x80) != 0) {
        BattleAudience_WinSetActive(1);
    }
    BattleAudienceCtrlProcess();
    BattleAudienceItemCtrlProcess();
    BattleAudienceApSrcCtrlProcess();
    BattleAudienceWinCtrlProcess();
    BattleAudienceSoundMain();
    if ((*(u32*)(battle + 0x4) & 0x80) == 0) {
        *(u32*)base &= ~0x10000;
    } else {
        *(u32*)base |= 0x10000;
    }
}

void BattleAudience_ActInit(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(s32*)((s32)base + 0x137D4) = 0;
    *(u32*)base &= ~0x10;
    *(u32*)base &= ~0x20;
    *(u32*)base &= ~0x40;
    *(u32*)base &= ~0x80;
    *(u32*)base &= ~0x100;
    *(u32*)base &= ~0x200;
    *(u32*)base &= ~0x400;
    *(u32*)base &= ~0x800;
    *(u32*)base &= ~0x1000;
}

u8 BattleAudience_PerAct(void) {
    extern void BattleAudienceCheer(s32 kind);
    extern void BattleAudienceAddPuni(s32 num);
    extern void* evtEntry(void* evt, s32 order, u32 flags);
    extern void BattleAudience_ActInit(void);
    extern void* msg_puni_all_escape;
    extern const f32 float_neg5_80424a80;
    extern const f32 float_25_80424a4c;
    extern const f32 float_40_80424a00;
    extern const f32 float_20_80424a18;
    extern const f32 float_15_80424a64;
    extern const f32 float_neg15_80424a84;
    extern const f32 float_neg20_80424a10;
    extern const f32 float_neg10_80424a6c;
    extern const f32 float_30_80424a60;

    u8* base;
    u8* apInfo;
    u32 flags;
    f32 delta;
    f32 value;
    s32 positive;
    s32 streak;

    base = BattleAudienceBaseGetPtr();
    pouchGetPtr();
    delta = float_0_80424988;
    value = float_0_80424988;
    positive = 0;

    flags = *(u32*)base;
    if ((flags & 0x100) == 0) {
        BattleAudienceCheer(2);
    } else if ((flags & 0x200) == 0) {
        BattleAudienceCheer(3);
    }

    flags = *(u32*)base;
    if ((flags & 0x40000) == 0) {
        if (((flags & 0x400) == 0) && ((flags & 0x800) != 0)) {
            delta -= float_5_8042499c;
            BattleAudienceAddPhaseEvtList(5);
        }

        apInfo = *(u8**)(base + 0x137B4);
        if (apInfo != 0) {
            flags = *(u32*)base;
            if ((flags & 0x10) != 0 || (flags & 0x20) != 0) {
                if ((flags & 0x10) == 0) {
                    if ((flags & 0x1000) == 0) {
                        *(s32*)(base + 0x137AC) = 0;
                        positive = 0;
                        streak = *(s32*)(base + 0x137B0) + 1;
                        if (streak > 5) {
                            streak = 5;
                        }
                        *(s32*)(base + 0x137B0) = streak;
                        delta += float_neg5_80424a80;
                        value = float_neg5_80424a80;
                    }
                } else {
                    positive = 1;
                    streak = *(s32*)(base + 0x137AC) + 1;
                    if (streak > 2) {
                        streak = 2;
                    }
                    *(s32*)(base + 0x137AC) = streak;
                    *(s32*)(base + 0x137B0) = 0;
                    value = (f32)streak * *(f32*)(apInfo + 0x14);
                    delta += value;
                }
            }
        }

        flags = *(u32*)base;
        if ((flags & 0x40) != 0 || (flags & 0x80) != 0) {
            if ((flags & 0x40) == 0) {
                *(s32*)(base + 0x137AC) = 0;
                positive = 0;
            } else {
                positive = 1;
                streak = *(s32*)(base + 0x137AC) + 1;
                if (streak > 2) {
                    streak = 2;
                }
                *(s32*)(base + 0x137AC) = streak;
                *(s32*)(base + 0x137B0) = 0;
                value = (f32)streak;
                delta += value;
            }
        }

        if (positive == 1) {
            if (value >= float_25_80424a4c) {
                BattleAudienceAddPhaseEvtList(0);
            }
            if (value >= float_40_80424a00) {
                BattleAudienceAddPhaseEvtList(3);
            }
            if (value >= float_20_80424a18) {
                BattleAudienceAddPhaseEvtList(8);
            }
            if (value >= float_15_80424a64) {
                BattleAudienceAddPhaseEvtList(0xD);
            }
        } else {
            if (value <= float_neg15_80424a84) {
                BattleAudienceAddPhaseEvtList(1);
            }
            if (value <= float_neg20_80424a10) {
                BattleAudienceAddPhaseEvtList(2);
            }
            if (value <= float_neg20_80424a10) {
                BattleAudienceAddPhaseEvtList(5);
            }
            if (value <= float_neg15_80424a84) {
                BattleAudienceAddPhaseEvtList(9);
            }
            if (value <= float_neg20_80424a10) {
                BattleAudienceAddPhaseEvtList(0xB);
            }
            if (value <= float_neg10_80424a6c) {
                BattleAudienceAddPhaseEvtList(0xD);
            }
        }

        apInfo = *(u8**)(base + 0x137B4);
        if (apInfo != 0) {
            flags = *(u32*)base;
            if ((flags & 0x100) != 0 || (flags & 0x200) != 0) {
                if ((flags & 0x100) != 0) {
                    value = (f32)((u8)*(u8*)(apInfo + 0x18) * *(s32*)(base + 0x137A8) * 2);
                    delta += value;
                    if (value >= float_30_80424a60) {
                        BattleAudienceAddPhaseEvtList(0);
                    }
                    if (value >= float_40_80424a00) {
                        BattleAudienceAddPhaseEvtList(3);
                    }
                    if (value >= float_25_80424a4c) {
                        BattleAudienceAddPhaseEvtList(8);
                    }
                    if (value >= float_20_80424a18) {
                        BattleAudienceAddPhaseEvtList(0xD);
                    }
                    BattleAudienceAddPuni(10);
                }
                if ((*(u32*)base & 0x200) != 0) {
                    delta -= float_5_8042499c;
                }
            }
        }

        if ((*(u32*)base & 0x8000) != 0) {
            *(void**)(base + 4) = evtEntry(&msg_puni_all_escape, 0, 0x20);
            *(u32*)base &= ~0x8000;
        }

        BattleAudienceAddTargetNum(*(f32*)(base + 0x13778) * float_0p01_80424998 * delta, float_0_80424988);
        BattleAudience_ActInit();
    }
    return 0;
}

s32 BattleAudience_CheckReaction(void) {
    extern s32 evtCheckID(s32 eventId);
    void* base = BattleAudienceBaseGetPtr();
    void* event;

    BattleAudienceGetPtr(0);
    event = *(void**)((s32)base + 4);
    if (event != NULL) {
        if (evtCheckID(*(s32*)((s32)event + 0x15C)) != 0) {
            return 1;
        }
    }
    *(s32*)((s32)base + 4) = 0;
    return 0;
}

u32 check_exe_phase_evt_status(int param_1, u32 param_2) {
    u8* member;
    u8 status;

    member = BattleAudienceGetPtr(param_1);
    if (BattleAudience_GetSysCtrl(param_1) == 0 || member[0x1B] != param_2) {
        return 0;
    }
    status = member[0x19];
    if (status == 0xD || status == 0xC || status == 0xB ||
        status == 0x15 || status == 0x16) {
        return 0;
    }
    return (*(u32*)member >> 3) & 1;
}

void BattleAudience_PerPhase(int phaseId) {
    extern void* BattleAudienceBaseGetPtr(void);
    extern void* BattleAudienceGetPtr(s32);
    extern u8 BattleAudience_GetExist(s32);
    extern void* BattleAudienceItemGetPtr(s32);
    extern void BattleAudience_ChangeStatus(s32, s32);
    extern void BattleAudienceSetThrowItemMax(void);
    u8* base;
    u8* member;
    u8* item;
    s32 i;

    base = BattleAudienceBaseGetPtr();
    if (phaseId == 0x4000002 && (*(u32*)base & 0x40000) == 0) {
        for (i = 0; i < 200; i++) {
            member = BattleAudienceGetPtr(i);
            if (BattleAudience_GetExist(i) == 1 && member[0x19] == 7) {
                if (*(s16*)(member + 0x1C) != -1) {
                    item = BattleAudienceItemGetPtr(*(s16*)(member + 0x1C));
                    *(u32*)item = 0;
                }
                *(s16*)(member + 0x1C) = -1;
                BattleAudience_ChangeStatus(i, 0);
            }
        }
        for (i = 0; i < 100; i++) {
            item = BattleAudienceItemGetPtr(i);
            if (*(s32*)(item + 4) < 2) {
                item = BattleAudienceItemGetPtr(i);
                *(u32*)item = 0;
            }
        }
    }
    if (phaseId == 0x4000004) {
        BattleAudienceSetThrowItemMax();
        *(u32*)base |= 8;
        *(s32*)(base + 0x138E0) = 0;
    }
}


s32 BattleAudience_CheckReactionPerPhase(void) {
    extern void* pouchGetPtr(void);
    extern void* evtEntry(void*, s32, u32);
    extern s32 evtCheckID(s32);
    extern void* msg_heavy_bomb_fire;
    extern void* msg_puni_all_escape;
    u8* base;
    u8* member;
    s32* state;
    s32 count;
    s32 i;

    base = BattleAudienceBaseGetPtr();
    pouchGetPtr();
    if ((*(u32*)base & 8) == 0) {
        return 0;
    }
    state = (s32*)(base + 0x137DC);
    switch (*state) {
        case 0:
            *state = 5;
            if ((*(u32*)base & 0x40000) != 0) {
                *state = 15;
                return 1;
            }
        case 5:
            count = 0;
            for (i = 0; i < 200; i++) {
                member = BattleAudienceGetPtr(i);
                if (BattleAudience_GetSysCtrl(i) == 1 && member[0x1B] == 9 &&
                    member[0x19] == 0x12) {
                    (*(s32*)(member + 0x12C))--;
                    if (*(s32*)(member + 0x12C) < 1) {
                        BattleAudience_ChangeStatus(i, 0x13);
                        count++;
                    }
                }
            }
            *state = count < 1 ? 7 : 6;
        case 6:
        case 7:
            for (i = 0; i < 200; i++) {
                member = BattleAudienceGetPtr(i);
                if (BattleAudience_GetSysCtrl(i) == 1 && member[0x1B] == 9 &&
                    member[0x19] == 0x13) {
                    break;
                }
            }
            if (i == 200) {
                *state = *state == 6 ? 8 : 13;
            }
            break;
        case 8:
            *(void**)(base + 4) = evtEntry(&msg_heavy_bomb_fire, 0, 0x20);
            *state = 9;
            break;
        case 9:
            if (!evtCheckID(*(s32*)(*(u8**)(base + 4) + 0x15C))) {
                *(s32*)(base + 4) = 0;
                *state = 10;
            }
            break;
        case 10:
            if ((*(u32*)base & 0x8000) == 0) {
                *state = 15;
            } else {
                *(void**)(base + 4) = evtEntry(&msg_puni_all_escape, 0, 0x20);
                *(u32*)base &= ~0x8000;
                *state = 11;
            }
            break;
        case 11:
            if (!evtCheckID(*(s32*)(*(u8**)(base + 4) + 0x15C))) {
                *(s32*)(base + 4) = 0;
                *state = 15;
            }
            break;
        case 13:
            *(s32*)(base + 0x137D8) = 0;
            *state = 14;
            break;
        case 14:
            *state = 15;
            break;
        case 15:
            *(u32*)base &= ~8;
            *(s32*)(base + 0x137D8) = 0;
            return 1;
    }
    return 0;
}

u8 BattleAudience_Disp(void) {
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void* pouchGetPtr(void);
    u8* base;
    u8* pouch;

    BattleAudienceBaseGetPtr();
    BattleAudienceAnimProcess();
    base = BattleAudienceBaseGetPtr();
    if ((*(u32*)base & 0x20000) == 0) {
        dispEntry(4, 1, BattleAudienceDispAudience, 0, 0.0f);
        dispEntry(4, 2, BattleAudienceDispAudience, 0, 0.0f);
        dispEntry(4, 1, BattleAudienceDispItem, 0, 0.0f);
        dispEntry(8, 0, BattleAudienceDispApSrc, 0, 499.0f);
    }
    pouch = pouchGetPtr();
    if (*(u16*)(pouch + 0x8C) != 0) {
        dispEntry(8, 1, BattleAudienceDispWin, 0, 499.0f);
    }
    return 0;
}

void BattleAudience_End(void) {
    extern void fileFree(void* file);
    extern void pouchSetAudienceNum(f32 value);
    extern void BattleFree(void* ptr);
    u8* base;
    u8* pouch;
    s32 maxAudience;
    s32 i;

    base = BattleAudienceBaseGetPtr();
    for (i = 0; i < 2; i++) {
        if (*(void**)(base + 0x10 + i * 4) != 0) {
            fileFree(*(void**)(base + 0x10 + i * 4));
        }
    }
    if (*(void**)(base + 0xC) != 0) {
        fileFree(*(void**)(base + 0xC));
    }
    pouch = pouchGetPtr();
    if (*(u16*)(pouch + 0x8C) != 0) {
        if (*(f32*)(base + 0x1377C) < float_0_80424988) {
            *(f32*)(base + 0x1377C) = float_0_80424988;
        }
        maxAudience = *(s32*)(base + 0x13790);
        if (*(f32*)(base + 0x1377C) > (f32)maxAudience) {
            *(f32*)(base + 0x1377C) = (f32)maxAudience;
        }
        pouchSetAudienceNum(*(f32*)(base + 0x1377C));
    }
    if (*(void**)(base + 0x137C8) != 0) {
        BattleFree(*(void**)(base + 0x137C8));
        *(void**)(base + 0x137C8) = 0;
    }
}

u8 BattleAudienceSettingAudience(void) {
    extern f32 pouchGetAudienceNum(void);
    extern void* memset(void*, s32, u32);
    extern void* memcpy(void*, const void*, u32);
    extern const u8 audience_kind[];
    u8* base;
    u8* pouch;
    s32 weights[12];
    s32 counts[13];
    s32 order[12];
    s32 roster[200];
    s32 target;
    s32 totalWeight;
    s32 assigned;
    s32 slot;
    s32 kind;
    s32 temp;
    s32 i;
    s32 j;

    base = BattleAudienceBaseGetPtr();
    pouch = pouchGetPtr();
    if (*(s16*)(pouch + 0x88) == 0) {
        *(s32*)(base + 0x13790) = 50;
    } else if (*(s16*)(pouch + 0x88) == 1) {
        *(s32*)(base + 0x13790) = 100;
    } else if (*(s16*)(pouch + 0x88) == 2) {
        *(s32*)(base + 0x13790) = 150;
    } else {
        *(s32*)(base + 0x13790) = 200;
    }
    if (*(u16*)(pouch + 0x8C) == 0) {
        *(s32*)(base + 0x13790) = 0;
    }
    *(f32*)(base + 0x13778) = 1.0f;
    *(f32*)(base + 0x1377C) = pouchGetAudienceNum() +
        0.01f * (pouchGetAudienceNum() - *(f32*)(base + 0x13778)) *
        (f32)(irand(10) + irand(10) - 13);
    if (*(f32*)(base + 0x1377C) < 0.0f) {
        *(f32*)(base + 0x1377C) = 0.0f;
    }
    if (*(f32*)(base + 0x1377C) > (f32)*(s32*)(base + 0x13790)) {
        *(f32*)(base + 0x1377C) = (f32)*(s32*)(base + 0x13790);
    }
    *(f32*)(base + 0x13780) = 0.0f;
    for (i = 0; i < 12; i++) {
        weights[i] = 1;
        order[i] = i;
    }
    for (i = 0; i < 24; i++) {
        j = irand(12);
        kind = irand(12);
        temp = order[j];
        order[j] = order[kind];
        order[kind] = temp;
    }
    memset(counts, 0, 13 * sizeof(s32));
    target = (s32)(*(f32*)(base + 0x1377C) + *(f32*)(base + 0x13780));
    if (target < 0) {
        target = 0;
    }
    if (target > *(s32*)(base + 0x13790)) {
        target = *(s32*)(base + 0x13790);
    }
    totalWeight = 0;
    for (i = 0; i < 12; i++) {
        totalWeight += weights[i];
    }
    assigned = 0;
    for (i = 0; i < 12; i++) {
        counts[i] = target * weights[i] / totalWeight;
        if ((audience_kind[i * 0x20] & 1) != 0) {
            counts[i] /= 2;
            assigned += counts[i] * 2;
        } else {
            assigned += counts[i];
        }
    }
    i = 0;
    while (assigned < target && i < 144) {
        kind = order[i % 12];
        if ((audience_kind[kind * 0x20] & 1) == 0) {
            counts[kind]++;
            assigned++;
        } else if (assigned + 2 <= target) {
            counts[kind]++;
            assigned += 2;
        }
        i++;
    }
    slot = 0;
    for (kind = 0; kind < 12; kind++) {
        for (i = 0; i < counts[kind]; i++) {
            roster[slot++] = kind;
            if ((audience_kind[kind * 0x20] & 1) != 0) {
                roster[slot++] = kind;
            }
        }
    }
    for (i = 0; i < 150 && slot > 0; i++) {
        j = irand(slot);
        kind = irand(slot);
        temp = roster[j];
        roster[j] = roster[kind];
        roster[kind] = temp;
    }
    for (i = 0; i < slot; i++) {
        j = BattleAudience_NoUsedFCHaitiRand();
        if (j == -1) {
            break;
        }
        BattleAudience_Entry(j, (u8)roster[i], 0);
    }
    memcpy(base + 0x13794, weights, 0x30);
    return 0;
}


void BattleAudienceGuestTPLRead(s32 index, u32 memberKind, char* tplName) {
    extern s32 sprintf(char* dst, const char* format, ...);
    extern void tplRead(char* tplName);
    extern const char str_battle_audience_PCTs_802fa6fc[];
    u8* base;
    char path[264];

    base = BattleAudienceBaseGetPtr();
    if (index < 2) {
        sprintf(path, str_battle_audience_PCTs_802fa6fc, tplName);
        tplRead(path);
        *(char**)(base + 0x10 + index * 4) = path;
        base[0x18 + index] = (u8)memberKind;
    }
}

u8 BattleAudienceCtrlProcess(void) {
    extern s32 BattleAudience_GetExist(s32 id);
    extern void* BattleAudienceGetPtr(s32 id);
    extern void BattleAudienceCtrlProcessKinopio(s32 id);
    extern s32 BattleAudienceItemOn(s32 memberIdx, s32 itemType, s32 num);

    void* battleWork;
    u8* base;
    u8* audience;
    s32 row;
    s32 col;
    s32 id;
    s32 i;
    s32 j;
    s32 temp;
    s32 chance;
    s32 candidates[200];

    battleWork = _battleWorkPointer;
    base = BattleAudienceBaseGetPtr();
    *(s32*)(base + 0x13784) = 0;
    *(s32*)(base + 0x13788) = 0;
    *(s32*)(base + 0x1378C) = 0;

    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            id = row * 20 + col + 10;
            if ((u8)BattleAudience_GetExist(id) == 1) {
                audience = BattleAudienceGetPtr(id);
                if (audience[0x19] != 0xC && audience[0x19] != 0xB) {
                    (*(s32*)(base + 0x13788))++;
                }
            }
        }
    }

    for (row = 0; row < 10; row++) {
        for (col = 0; col < 10; col++) {
            id = row * 20 + col;
            if ((u8)BattleAudience_GetExist(id) == 1) {
                audience = BattleAudienceGetPtr(id);
                if (audience[0x19] != 0xC && audience[0x19] != 0xB) {
                    (*(s32*)(base + 0x1378C))++;
                }
            }
        }
    }

    *(s32*)(base + 0x13784) = *(s32*)(base + 0x13788) + *(s32*)(base + 0x1378C);

    audience = BattleAudienceGetPtr(0);
    for (i = 0; i < 200; i++, audience += 0x134) {
        if ((*(u32*)audience & 0x10) == 0) {
            if ((u8)BattleAudience_GetExist(i) != 0) {
                switch (audience[0x18]) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                        BattleAudienceCtrlProcessKinopio(i);
                        break;
                }
            }
        } else {
            *(f32*)(audience + 0x4C) += *(f32*)(audience + 0x58);
            *(f32*)(audience + 0x58) += *(f32*)(audience + 0x5C);
            if (*(f32*)(audience + 0x4C) <= float_0_80424988) {
                *(f32*)(audience + 0x4C) = float_0_80424988;
                *(f32*)(audience + 0x58) = float_0_80424988;
                *(f32*)(audience + 0x5C) = float_0_80424988;
            }
        }
    }

    if ((*(u32*)((s32)battleWork + 0xEF4) & 0x20000) != 0) {
        *(s32*)(base + 0x137C4) = 10000;
    }
    if (((*(u32*)((s32)battleWork + 0xF0) & 0x80) != 0) &&
        (*(s32*)(base + 0x137C0) < *(s32*)(base + 0x137C4))) {
        if (*(s32*)(base + 0x137C0) == 1) {
            chance = 6000;
        } else if (*(s32*)(base + 0x137C0) < 1) {
            chance = 0x708;
        } else {
            chance = 30000;
        }
        if ((*(u32*)((s32)battleWork + 0xEF4) & 0x20000) != 0) {
            chance = 0x3C;
        }
        if (irand(chance) == 0) {
            for (i = 0; i < 200; i++) {
                candidates[i] = i;
            }
            for (i = 0; i < 1000; i++) {
                id = irand(200);
                j = irand(200);
                temp = candidates[id];
                candidates[id] = candidates[j];
                candidates[j] = temp;
            }
            for (i = 0; i < 200; i++) {
                if (BattleAudienceItemOn(candidates[i], 0, 1) == 1) {
                    *(s32*)(base + 0x137C0) = *(s32*)(base + 0x137C0) + 1;
                    return 0;
                }
            }
        }
    }
    return 0;
}

s32 BattleAudienceItemOn(u32 memberIdx, u16 itemType, int numberItems) {
    extern s32 swGet(s32 id);
    extern u8 audience_kind[];
    void* base;
    void* audience;
    void* kindInfo;
    void* item;
    s32 column;
    s32 i;
    u8 status;

    base = BattleAudienceBaseGetPtr();
    audience = BattleAudienceGetPtr(memberIdx);

    if ((*(u32*)base & 0x40000) != 0) {
        return 0;
    }

    kindInfo = *(void**)(audience_kind + (*(u8*)((s32)audience + 0x1B) * 0x20) + 8);
    if (*(s32*)kindInfo == 0) {
        return 0;
    }

    if (itemType >= 0x79 && itemType < 0xEC && swGet(0xEB) == 0) {
        return 0;
    }

    column = memberIdx % 20;
    if (column < 4 || column > 15) {
        return 0;
    }

    if (BattleAudience_GetExist(memberIdx) == 0) {
        return 0;
    }

    status = *(u8*)((s32)audience + 0x19);
    if (status == 7 || status == 8 || status == 0xD || status == 0xC || status == 0xB ||
        status == 0x12 || status == 0x11 || status == 0x15 || status == 0xA) {
        return 0;
    }

    if ((*(u32*)audience & 8) == 0) {
        return 0;
    }

    item = BattleAudienceItemGetPtr(0);
    for (i = 0; i < numberItems; i++, item = (void*)((s32)item + 0x48)) {
        if ((*(u32*)item & 1) == 0) {
            *(u32*)item |= 1;
            *(s32*)((s32)item + 4) = 0;
            *(u32*)((s32)item + 0xC) = memberIdx;
            *(u32*)((s32)item + 0x10) = itemType;
            BattleAudience_ChangeStatus(memberIdx, 7);
            *(u16*)((s32)audience + 0x1C) = i;
            return 1;
        }
    }

    return 0;
}

void BattleAudienceSetThrowItemMax(void) {
    void* base;
    s32 value;

    base = BattleAudienceBaseGetPtr();
    value = irand(0x64);

    if (value < 0x50) {
        *(s32*)((s32)base + 0x137F4) = 0;
    } else if (value < 0x5A) {
        *(s32*)((s32)base + 0x137F4) = 1;
    } else if (value < 0x5F) {
        *(s32*)((s32)base + 0x137F4) = 2;
    } else if (value < 0x64) {
        *(s32*)((s32)base + 0x137F4) = 3;
    }

    *(s32*)((s32)base + 0x137F0) = 0;
    *(s32*)((s32)base + 0x137F8) = 0;
}


s32 BattleAudienceDetectTargetAll(void) {
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BattleGetUnitPartsPtr(s32 unitIdx, s32 partIdx);
    extern void* BattleGetUnitPtr(void* battle, s32 unitIdx);
    s32 candidates[64];
    u8* battle;
    u8* unit;
    u8* part;
    s32 count;
    s32 kind;
    s32 partIdx;
    s32 unitIdx;

    battle = _battleWorkPointer;
    BattleAudienceBaseGetPtr();
    count = 0;
    for (unitIdx = 0; unitIdx < 64; unitIdx++) {
        unit = *(u8**)(battle + 0x20 + unitIdx * 4);
        if (unit == 0) {
            continue;
        }
        partIdx = BtlUnit_GetBodyPartsId(unit);
        part = BattleGetUnitPartsPtr(unitIdx, partIdx);
        kind = *(s32*)(unit + 8);
        if ((*(u32*)(part + 0x40) & 0x1000) == 0 &&
            *(s16*)(unit + 0x10C) > 0 && (kind < 0xD8 || kind > 0xD9)) {
            candidates[count++] = unitIdx;
        }
    }
    if (count == 0) {
        return 0;
    }
    return (s32)BattleGetUnitPtr(battle, candidates[irand(count)]);
}

s32 BattleAudienceDetectTargetPlayer(void) {
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BattleGetUnitPartsPtr(s32 unitIdx, s32 partIdx);
    extern void* BattleGetUnitPtr(void* battle, s32 unitIdx);
    s32 candidates[64];
    u8* battle;
    u8* unit;
    u8* part;
    s32 count;
    s32 partIdx;
    s32 unitIdx;

    battle = _battleWorkPointer;
    BattleAudienceBaseGetPtr();
    count = 0;
    for (unitIdx = 0; unitIdx < 64; unitIdx++) {
        unit = *(u8**)(battle + 0x20 + unitIdx * 4);
        if (unit == 0) {
            continue;
        }
        partIdx = BtlUnit_GetBodyPartsId(unit);
        part = BattleGetUnitPartsPtr(unitIdx, partIdx);
        if ((s8)unit[0xC] == 0 && *(s16*)(unit + 0x10C) > 0 &&
            (*(u32*)(part + 0x40) & 0x1000) == 0) {
            candidates[count++] = unitIdx;
        }
    }
    if (count == 0) {
        return 0;
    }
    return (s32)BattleGetUnitPtr(battle, candidates[irand(count)]);
}

u8 BattleAudienceItemCtrlProcess(void) {
    extern s32 BattleAudienceDetectTargetPlayer(void);
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partIdx);
    extern void BtlUnit_GetHitPos(void* unit, void* part, f32*, f32*, f32*);
    u8* base;
    u8* item;
    u8* owner;
    u8* other;
    void* target;
    void* part;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 frames;
    f32 dx;
    s32 ownerIdx;
    s32 partIdx;
    s32 i;
    s32 j;

    base = BattleAudienceBaseGetPtr();
    item = BattleAudienceItemGetPtr(0);
    for (i = 0; i < 100; i++, item += 0x48) {
        if ((*(u32*)item & 1) == 0) {
            continue;
        }
        ownerIdx = *(s32*)(item + 0xC);
        owner = BattleAudienceGetPtr(ownerIdx);
        if (owner[0x19] == 12 || owner[0x19] == 13 || owner[0x19] == 19 ||
            owner[0x19] == 21 || !BattleAudience_GetExist(ownerIdx)) {
            goto clear_item;
        }
        switch (*(s32*)(item + 4)) {
            case 0:
                *(u32*)item |= 2;
                *(s32*)(item + 8) = i + 180;
                *(f32*)(item + 0x20) = 1.0f;
                *(f32*)(item + 0x24) = 0.0f;
                *(f32*)(item + 0x14) = *(f32*)(owner + 0x48);
                *(f32*)(item + 0x18) = *(f32*)(owner + 0x4C) - 10.0f;
                *(f32*)(item + 0x1C) = *(f32*)(owner + 0x50) + 1.0f;
                *(s32*)(item + 4) = 1;
            case 1:
                if (*(s32*)(item + 8) < 1) {
                    item[0x44] = 1;
                    for (j = 0; j < 100; j++) {
                        if (j == i) {
                            continue;
                        }
                        other = BattleAudienceItemGetPtr(j);
                        if ((*(u32*)other & 1) != 0 && *(s32*)(other + 4) == 1) {
                            item[0x44] = 2;
                            break;
                        }
                    }
                    target = (void*)BattleAudienceDetectTargetPlayer();
                    if (target == 0) {
                        goto cancel_owner;
                    }
                    *(void**)(item + 0x40) = target;
                    partIdx = BtlUnit_GetBodyPartsId(target);
                    part = BtlUnit_GetPartsPtr(target, partIdx);
                    BtlUnit_GetHitPos(target, part, &hitX, &hitY, &hitZ);
                    dx = hitX - *(f32*)(item + 0x14);
                    frames = 30.0f + (dx < 0.0f ? -dx : dx) / 15.0f;
                    *(f32*)(item + 0x28) = dx / frames;
                    *(f32*)(item + 0x2C) = 7.0f;
                    *(f32*)(item + 0x30) =
                        (hitZ - *(f32*)(item + 0x1C)) / frames;
                    *(f32*)(item + 0x34) = 0.0f;
                    *(f32*)(item + 0x38) =
                        -2.0f * (*(f32*)(item + 0x2C) * frames -
                        (hitY - *(f32*)(item + 0x18))) / (frames * frames);
                    *(f32*)(item + 0x3C) = 0.0f;
                    *(s32*)(item + 8) = (s32)frames;
                    *(s32*)(item + 4) = 2;
                } else {
                    *(f32*)(item + 0x14) = *(f32*)(owner + 0x48);
                    *(f32*)(item + 0x18) = *(f32*)(owner + 0x4C) - 10.0f;
                    *(f32*)(item + 0x1C) = *(f32*)(owner + 0x50) + 1.0f;
                    (*(s32*)(item + 8))--;
                }
                break;
            case 2:
                if (*(s32*)(item + 8) == 20) {
                    BattleAudience_ChangeStatus(ownerIdx, 0);
                }
                if (*(s32*)(item + 8) == 0) {
                    *(s32*)(item + 4) = 3;
                } else {
                    *(f32*)(item + 0x14) += *(f32*)(item + 0x28);
                    *(f32*)(item + 0x18) += *(f32*)(item + 0x2C);
                    *(f32*)(item + 0x1C) += *(f32*)(item + 0x30);
                    *(f32*)(item + 0x28) += *(f32*)(item + 0x34);
                    *(f32*)(item + 0x2C) += *(f32*)(item + 0x38);
                    *(f32*)(item + 0x30) += *(f32*)(item + 0x3C);
                    if (*(s32*)(item + 0x10) == 0xEF) {
                        *(f32*)(item + 0x24) += 18.0f;
                    }
                    (*(s32*)(item + 8))--;
                }
                break;
            case 3:
                if (item[0x44] == 0) {
                    *(s32*)(item + 4) = 4;
                } else if (item[0x44] == 2) {
                    *(s32*)(item + 4) = 4;
                } else {
                    *(u32*)item &= ~2;
                    *(s32*)(item + 8) = 5;
                    *(s32*)(item + 4) = 7;
                }
                break;
            case 4:
                *(f32*)(item + 0x14) += *(f32*)(item + 0x28);
                *(f32*)(item + 0x18) += *(f32*)(item + 0x2C);
                *(f32*)(item + 0x1C) += *(f32*)(item + 0x30);
                *(f32*)(item + 0x28) += *(f32*)(item + 0x34);
                *(f32*)(item + 0x2C) += *(f32*)(item + 0x38);
                *(f32*)(item + 0x30) += *(f32*)(item + 0x3C);
                if (*(f32*)(item + 0x18) <= 0.0f) {
                    BattleAudienceSoundStop(10);
                    BattleAudienceSoundStop(11);
                    *(s32*)(item + 4) = 5;
                    *(f32*)(item + 0x18) = 0.1f;
                    *(f32*)(item + 0x2C) = 1.0f;
                    *(f32*)(item + 0x30) = 0.0f;
                }
                break;
            case 5:
                *(f32*)(item + 0x14) += *(f32*)(item + 0x28);
                *(f32*)(item + 0x18) += *(f32*)(item + 0x2C);
                *(f32*)(item + 0x1C) += *(f32*)(item + 0x30);
                *(f32*)(item + 0x28) += *(f32*)(item + 0x34);
                *(f32*)(item + 0x2C) += *(f32*)(item + 0x38);
                *(f32*)(item + 0x30) += *(f32*)(item + 0x3C);
                if (*(f32*)(item + 0x18) <= 0.0f) {
                    *(f32*)(item + 0x18) = 0.0f;
                    *(s32*)(item + 8) = 60;
                    *(s32*)(item + 4) = 6;
                }
                break;
            case 6:
                if (*(s32*)(item + 8) > 0) {
                    if (*(s32*)(item + 8) < 30) {
                        if ((*(u32*)(item + 8) & 3) < 2) {
                            *(u32*)item |= 2;
                        } else {
                            *(u32*)item &= ~2;
                        }
                    }
                    (*(s32*)(item + 8))--;
                    break;
                }
                goto cancel_owner;
            case 7:
                (*(s32*)(item + 8))--;
                if (*(s32*)(item + 8) < 1) {
                    goto cancel_owner;
                }
                break;
        }
        continue;
cancel_owner:
        BattleAudienceSoundStop(10);
        BattleAudienceSoundStop(11);
        if (ownerIdx != -1) {
            BattleAudience_ChangeStatus(ownerIdx, 0);
        }
clear_item:
        *(u32*)item = 0;
        *(s32*)(item + 8) = 0;
        *(s32*)(item + 4) = 0;
        if (ownerIdx != -1) {
            owner = BattleAudienceGetPtr(ownerIdx);
            *(s16*)(owner + 0x1C) = -1;
        }
    }
    return 0;
}

u8 BattleAudienceApSrcCtrlProcess(void) {
    extern s32 psndSFXChk(s32 id);
    extern void psndSFX_vol(s32 id, s32 vol);
    extern void BattleStatusWindowAPRecoveryOff(void);
    extern s32 BattleAudienceApSrcEntry(void);
    extern const char str_SFX_BTL_SYS_AP_STAR1_802fa6e4[];
    extern s32 rand(void);
    extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 current, s32 max);
    extern s32 statusGetApPos(void* pos);
    extern f64 sqrt(f64 value);
    extern f64 asin(f64 value);
    extern f64 cos(f64 value);
    extern f64 sin(f64 value);
    extern s32 irand(s32 max);
    extern const f32 float_64_80424a44;
    extern const f32 float_63_804249b4;
    extern const f32 float_50_80424a48;
    extern const f32 float_32767_804249e4;
    extern const f32 float_25_80424a4c;
    extern const f32 float_0p9_80424a50;
    extern const f32 float_0p8_80424a0c;
    extern const f32 float_0p05_80424a54;
    extern const f32 float_0p2_804249ec;
    extern const f32 float_6p2832_80424a58;
    extern const f32 float_128_80424a5c;

    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    u8* base;
    u8* star;
    s32 active;
    s32 i;
    s32 entry;
    s32 state;
    VecLocal pos;
    f32 dx;
    f32 dy;
    f32 dist;
    f32 angle;

    base = BattleAudienceBaseGetPtr();
    if ((*(u32*)base & 2) != 0) {
        active = 0;
        star = base + 0x10E7C;
        for (i = 0; i < 100; i++, star += 0x60) {
            if ((*(u32*)star & 1) != 0) {
                active++;
            }
        }

        if (psndSFXChk(*(s32*)(base + 8)) != -1) {
            psndSFX_vol(*(s32*)(base + 8), (s32)(float_64_80424a44 + (float_63_804249b4 * (f32)active) / float_100_804249bc));
        }

        if (*(s32*)(base + 0x137BC) < 1) {
            star = base + 0x10E7C;
            for (i = 0; i < 100; i++, star += 0x60) {
                if ((*(u32*)star & 1) != 0) {
                    break;
                }
            }
            if (i == 100) {
                *(u32*)base &= ~2;
                psndSFXOff(*(s32*)(base + 8));
                *(s32*)(base + 8) = 0;
                *(s32*)(base + 0x137BC) = 0;
                BattleStatusWindowAPRecoveryOff();
                return 0;
            }
        } else {
            entry = BattleAudienceApSrcEntry();
            if (entry == 1) {
                *(s32*)(base + 0x137BC) = *(s32*)(base + 0x137BC) - 1;
            }
            if (active == 0 && psndSFXChk(*(s32*)(base + 8)) == -1) {
                psndSFXOn(str_SFX_BTL_SYS_AP_STAR1_802fa6e4);
                *(s32*)(base + 8) = 0;
            }
        }

        star = base + 0x10E7C;
        for (i = 0; i < 100; i++, star += 0x60) {
            pouchGetPtr();
            if ((*(u32*)star & 1) == 0) {
                continue;
            }
            state = *(s32*)(star + 4);
            switch (state) {
                case 0:
                    *(s32*)(star + 4) = 5;
                    *(f32*)(star + 0x24) = *(f32*)(star + 0x18);
                    *(f32*)(star + 0x28) = *(f32*)(star + 0x1C);
                    *(f32*)(star + 0x2C) = *(f32*)(star + 0x20);
                    *(f32*)(star + 0x30) = *(f32*)(star + 0x24);
                    *(f32*)(star + 0x34) = *(f32*)(star + 0x28);
                    *(f32*)(star + 0x38) = *(f32*)(star + 0x2C);
                    *(f32*)(star + 0x24) += (float_50_80424a48 * (f32)rand()) / float_32767_804249e4 - float_25_80424a4c;
                    *(f32*)(star + 0x28) += float_25_80424a4c;
                    *(s32*)(star + 0x3C) = 0;
                    *(s32*)(star + 0x40) = 0x1E;
                    /* fallthrough */
                case 5:
                    *(s32*)(star + 0x3C) = *(s32*)(star + 0x3C) + 1;
                    *(f32*)(star + 0x18) = intplGetValue(*(f32*)(star + 0x30), *(f32*)(star + 0x24), 5, *(s32*)(star + 0x3C), *(s32*)(star + 0x40));
                    *(f32*)(star + 0x1C) = intplGetValue(*(f32*)(star + 0x34), *(f32*)(star + 0x28), 5, *(s32*)(star + 0x3C), *(s32*)(star + 0x40));
                    if (*(s32*)(star + 0x3C) >= *(s32*)(star + 0x40) - 0x14) {
                        *(f32*)(star + 0x10) = intplGetValue(float_0p9_80424a50, float_0p8_80424a0c, 4,
                                                             *(s32*)(star + 0x3C) - (*(s32*)(star + 0x40) - 0x14), 10);
                    }
                    if (*(s32*)(star + 0x3C) >= *(s32*)(star + 0x40)) {
                        *(s32*)(star + 4) = 10;
                        *(s32*)(star + 0x3C) = 0;
                        *(s32*)(star + 0x40) = irand(10) + 0xF;
                    }
                    break;
                case 10:
                    *(s32*)(star + 0x3C) = *(s32*)(star + 0x3C) + 1;
                    if (*(s32*)(star + 0x3C) >= *(s32*)(star + 0x40)) {
                        *(s32*)(star + 4) = 0xF;
                        *(s32*)(star + 0x3C) = 0;
                        *(s32*)(star + 0x40) = irand(0x14) + 0x14;
                        *(f32*)(star + 0x30) = *(f32*)(star + 0x18);
                        *(f32*)(star + 0x34) = *(f32*)(star + 0x1C);
                        *(f32*)(star + 0x38) = *(f32*)(star + 0x20);
                        statusGetApPos(&pos);
                        *(f32*)(star + 0x24) = pos.x;
                        *(f32*)(star + 0x28) = pos.y;
                        *(f32*)(star + 0x2C) = pos.z;
                        dx = *(f32*)(star + 0x30) - *(f32*)(star + 0x24);
                        dy = *(f32*)(star + 0x34) - *(f32*)(star + 0x28);
                        dist = (f32)sqrt(dx * dx + dy * dy);
                        *(f32*)(star + 0x44) = dist;
                        *(f32*)(star + 0x48) = dist;
                        *(f32*)(star + 0x4C) = (f32)asin(dy / dist);
                        *(f32*)(star + 0x50) = float_0p05_80424a54 + (float_0p2_804249ec * (f32)rand()) / float_32767_804249e4;
                    }
                    break;
                case 0xF:
                    *(s32*)(star + 0x3C) = *(s32*)(star + 0x3C) + 1;
                    *(f32*)(star + 0x48) = intplGetValue(*(f32*)(star + 0x44), float_0_80424988, 4, *(s32*)(star + 0x3C), *(s32*)(star + 0x40));
                    angle = *(f32*)(star + 0x4C) + *(f32*)(star + 0x50);
                    if (angle < float_0_80424988) {
                        angle += float_6p2832_80424a58;
                    }
                    *(f32*)(star + 0x4C) = angle;
                    *(f32*)(star + 0x18) = *(f32*)(star + 0x48) * (f32)cos(angle) + *(f32*)(star + 0x24);
                    *(f32*)(star + 0x1C) = *(f32*)(star + 0x48) * (f32)sin(angle) + *(f32*)(star + 0x28);
                    if (*(s32*)(star + 0x3C) >= *(s32*)(star + 0x40)) {
                        *(s32*)(star + 4) = 0x14;
                        *(s32*)(star + 0x3C) = 0;
                        *(s32*)(star + 0x40) = 0xF;
                        statusGetApPos(&pos);
                        *(f32*)(star + 0x18) = pos.x;
                        *(f32*)(star + 0x1C) = pos.y;
                        *(f32*)(star + 0x20) = pos.z;
                    }
                    break;
                case 0x14:
                    *(s32*)(star + 0x3C) = *(s32*)(star + 0x3C) + 1;
                    if (*(s32*)(star + 0x3C) >= *(s32*)(star + 0x40)) {
                        *(u32*)star &= ~1;
                    }
                    break;
            }
        }
    }
    return 0;
}

s32 BattleAudienceApSrcEntry(void) {
    extern s32 rand(void);
    extern void* camGetPtr(s32 id);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTX44MultVec(void* mtx, void* src, void* dst);
    extern const f32 vec3_802fa000[];
    extern const f32 vec3_802fa00c[];
    extern const f32 float_10_80424a04;
    extern const f32 float_20_80424a18;
    extern const f32 float_40_80424a00;
    extern const f32 float_240_80424a40;
    extern const f32 float_304_80424a3c;
    extern const f32 float_32767_804249e4;
    void* base;
    void* entry;
    void* audience;
    void* cam;
    s32 ids[200];
    s32 count;
    s32 id;
    s32 i;

    base = BattleAudienceBaseGetPtr();
    entry = (void*)((s32)base + 0x10E7C);
    for (i = 0; i < 100; i++, entry = (void*)((s32)entry + 0x60)) {
        if ((*(u32*)entry & 1) == 0) {
            break;
        }
    }

    if (i == 100) {
        return 0;
    }

    count = 0;
    for (id = 0; id < 200; id++) {
        if (BattleAudience_GetExist(id) == 1) {
            ids[count] = id;
            count++;
        }
    }

    if (count == 0) {
        return 1;
    }

    *(s32*)entry = 1;
    *(s32*)((s32)entry + 4) = 0;
    *(s32*)((s32)entry + 0x44) = 0;
    *(f32*)((s32)entry + 0x38) = vec3_802fa000[0];
    *(f32*)((s32)entry + 0x3C) = vec3_802fa000[1];
    *(f32*)((s32)entry + 0x40) = vec3_802fa000[2];
    *(u8*)((s32)entry + 0x5C) = 0x80;

    audience = BattleAudienceGetPtr(ids[irand(count)]);
    *(f32*)((s32)entry + 8) = *(f32*)((s32)audience + 0x48);
    *(f32*)((s32)entry + 0xC) = *(f32*)((s32)audience + 0x4C) + float_20_80424a18;
    *(f32*)((s32)entry + 0x10) = *(f32*)((s32)audience + 0x50) + float_1_80424990;

    cam = camGetPtr(4);
    PSMTXMultVec((void*)((s32)cam + 0x11C), (void*)((s32)entry + 8), (void*)((s32)entry + 8));
    PSMTX44MultVec((void*)((s32)cam + 0x15C), (void*)((s32)entry + 8), (void*)((s32)entry + 8));

    *(f32*)((s32)entry + 8) *= float_304_80424a3c;
    *(f32*)((s32)entry + 0xC) *= float_240_80424a40;
    *(f32*)((s32)entry + 0x10) = float_0_80424988;

    *(f32*)((s32)entry + 0x14) =
        (float_40_80424a00 * (f32)rand() / float_32767_804249e4) - float_20_80424a18;
    *(f32*)((s32)entry + 0x18) =
        float_10_80424a04 * (f32)rand() / float_32767_804249e4;
    *(f32*)((s32)entry + 0x1C) = float_0_80424988;

    *(f32*)((s32)entry + 0x20) = *(f32*)((s32)entry + 8);
    *(f32*)((s32)entry + 0x24) = *(f32*)((s32)entry + 0xC);
    *(f32*)((s32)entry + 0x28) = *(f32*)((s32)entry + 0x10);
    *(f32*)((s32)entry + 0x2C) = vec3_802fa00c[0];
    *(f32*)((s32)entry + 0x30) = vec3_802fa00c[1];
    *(f32*)((s32)entry + 0x34) = vec3_802fa00c[2];

    return 1;
}

u8 BattleAudienceWinCtrlProcess(void) {
    extern void* BattleAudienceBaseGetPtr(void);
    extern void* BattleAudienceWinGetPtr(void);
    extern f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);
    extern f32 float_0p1_804249e8;
    extern f32 float_neg0p1_80424a2c;
    extern f32 float_neg500_80424a30;
    extern f32 float_neg280_80424a34;
    extern f32 float_neg160_80424a38;
    u8* base;
    u8* win;
    f32 target;
    f32 diff;
    f32 step;
    s32 timer;

    base = BattleAudienceBaseGetPtr();
    win = BattleAudienceWinGetPtr();
    target = (f32)*(s32*)(base + 0x13784);
    diff = target - *(f32*)(win + 0x18);
    if (diff != float_0_80424988) {
        if (diff > float_0_80424988) {
            step = float_0p1_804249e8 * diff;
            if (step < float_0p1_804249e8) {
                step = float_0p1_804249e8;
            }
            *(f32*)(win + 0x18) += step;
            if (target - *(f32*)(win + 0x18) < float_0_80424988) {
                *(f32*)(win + 0x18) = target;
            }
        } else {
            step = float_0p1_804249e8 * diff;
            if (step > float_neg0p1_80424a2c) {
                step = float_0p1_804249e8;
            } else {
                step = float_neg0p1_80424a2c * diff;
            }
            *(f32*)(win + 0x18) -= step;
            if (target - *(f32*)(win + 0x18) > float_0_80424988) {
                *(f32*)(win + 0x18) = target;
            }
        }
    }

    timer = *(s32*)(win + 0x14);
    if (win[0] != 0) {
        timer++;
        *(s32*)(win + 0x14) = timer;
        if (timer > 0xF) {
            *(s32*)(win + 0x14) = 0xF;
        }
    } else {
        timer--;
        *(s32*)(win + 0x14) = timer;
        if (timer < 0) {
            *(s32*)(win + 0x14) = 0;
        }
    }

    *(f32*)(win + 8) = intplGetValue(4, *(s32*)(win + 0x14), float_neg500_80424a30, float_neg280_80424a34, 0xF);
    *(f32*)(win + 0xC) = float_neg160_80424a38;
    *(f32*)(win + 0x10) = float_0_80424988;
}

u8 BattleAudienceGXInit(void) {
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);

    BattleAudienceBaseGetPtr();
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(11, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 1, 1, 0, 2, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 10, 8, 15);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetTevOrder(0, 0, 0, 4);
    return 0;
}

u8 BattleAudienceAnimProcess(void) {
    u8* member;
    s32* command;
    s32 count;
    s32 label;
    s32 i;

    BattleAudienceBaseGetPtr();
    i = 0;
    member = BattleAudienceGetPtr(0);
    do {
        if ((*(u32*)member & 1) != 0 && (*(u32*)member & 4) != 0) {
            command = *(s32**)(member + 0x28);
            if (*(s32*)(member + 0x2C) > 0) {
                *(s32*)(member + 0x2C) -= 1;
            } else {
                switch (*command) {
                    case -2:
                        member[0x1A] = (u8)command[1];
                        *(s32*)(member + 0x2C) = command[2];
                        *(s32**)(member + 0x28) += 3;
                        break;
                    case -3:
                        *(s32**)(member + 0x28) = *(s32**)(member + 0x24);
                        break;
                    case -4:
                        *(s32**)(member + 0x28) = command + 2;
                        break;
                    case -5:
                        label = command[1];
                        *(s32**)(member + 0x28) = *(s32**)(member + 0x24);
                        while (**(s32**)(member + 0x28) != -4 ||
                               (*(s32**)(member + 0x28))[1] != label) {
                            *(s32**)(member + 0x28) += 1;
                        }
                        break;
                    case -6:
                        *(s32*)(member + 0x34 + member[0x40] * 4) = command[1];
                        member[0x40] += 1;
                        *(s32**)(member + 0x28) += 2;
                        break;
                    case -7:
                        *(s32*)(member + 0x30 + member[0x40] * 4) -= 1;
                        if (*(s32*)(member + 0x30 + member[0x40] * 4) < 1) {
                            member[0x40] -= 1;
                            *(s32**)(member + 0x28) += 1;
                        } else {
                            count = 1;
                            do {
                                *(s32**)(member + 0x28) -= 1;
                                if (**(s32**)(member + 0x28) == -6) {
                                    count -= 1;
                                }
                                if (**(s32**)(member + 0x28) == -7) {
                                    count += 1;
                                }
                            } while (count > 0);
                            *(s32**)(member + 0x28) += 2;
                        }
                        break;
                    case -8:
                        *(s32*)(member + 0x2C) = command[1];
                        *(s32**)(member + 0x28) += 2;
                        break;
                }
            }
        }
        i++;
        member += 0x134;
    } while (i < 200);
    return 0;
}

u8 BattleAudienceDispAudience(void) {
    typedef f32 Mtx[3][4];
    extern void* BattleAudienceBaseGetPtr(void);
    extern void* BattleAudienceGetPtr(s32);
    extern void* camGetPtr(s32);
    extern s32 dispGetCurWork(void);
    extern void TEXGetGXTexObjFromPalette(void*, void*, s32);
    extern void GXLoadTexObj(void*, s32);
    extern s32 GXGetTexObjWidth(void*);
    extern s32 GXGetTexObjHeight(void*);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXBegin(s32, s32, s16);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(void*, void*, void*);
    u8* base;
    u8* member;
    u8* cam;
    void* tpl;
    void* palette;
    u8 texObj[32];
    u8 color[4];
    Mtx scale;
    Mtx rotate;
    Mtx model;
    Mtx viewModel;
    f32 width;
    f32 height;
    f32 sizeX;
    f32 sizeY;
    f32 x;
    f32 y;
    f32 z;
    s32 work;
    s32 id;
    s32 guest;
    s32 vertex;

    base = BattleAudienceBaseGetPtr();
    cam = camGetPtr(4);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 1, 1, 0, 2, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 10, 8, 15);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetTevOrder(0, 0, 0, 4);

    member = BattleAudienceGetPtr(0);
    for (id = 0; id < 200; id++, member += 0xE8) {
        work = dispGetCurWork();
        if ((*(u8*)(work + 1) == 2 && *(s8*)(member + 0x44) == -1) ||
            (*(u8*)(work + 1) != 2 && *(s8*)(member + 0x44) != -1)) {
            continue;
        }
        if ((*(u32*)member & 1) == 0 || (*(u32*)member & 4) == 0) {
            continue;
        }

        tpl = 0;
        if (*(u8*)(member + 0x19) < 12) {
            tpl = *(void**)(base + 0xC);
        } else {
            for (guest = 0; guest < 2; guest++) {
                if (*(u8*)(member + 0x19) == *(u8*)(base + 0x18 + guest)) {
                    tpl = *(void**)(base + 0x10 + guest * 4);
                    break;
                }
            }
        }
        if (tpl == 0) {
            continue;
        }
        palette = **(void***)((u8*)tpl + 0xA0);
        TEXGetGXTexObjFromPalette(palette, texObj, *(u8*)(member + 0x1A));
        GXLoadTexObj(texObj, 0);
        width = (f32)GXGetTexObjWidth(texObj);
        height = (f32)GXGetTexObjHeight(texObj);
        sizeX = *(f32*)(member + 0x60) * width * 0.015625f;
        sizeY = *(f32*)(member + 0x64) * height * 0.015625f;
        *(f32*)(member + 0x54) = sizeX;
        *(f32*)(member + 0x58) = sizeY;

        *(f32*)(member + 0x5C) = *(f32*)(member + 0x68);
        PSMTXScale(scale, sizeX, sizeY, *(f32*)(member + 0x5C));
        PSMTXRotRad(rotate, *(f32*)(member + 0xA0) * 0.017453292f, 'y');
        PSMTXConcat(rotate, scale, model);
        model[0][3] += *(f32*)(member + 0xA8) * sizeX;
        model[1][3] += *(f32*)(member + 0xAC) * sizeY;
        model[2][3] += *(f32*)(member + 0xB0) * *(f32*)(member + 0x5C);
        x = *(f32*)(member + 0x48) + *(f32*)(member + 0xC0) -
            *(f32*)(member + 0xA8) * sizeX;
        y = *(f32*)(member + 0x4C) + *(f32*)(member + 0xC4) -
            *(f32*)(member + 0xAC) * sizeY;
        z = *(f32*)(member + 0x50) + *(f32*)(member + 0xC8) -
            *(f32*)(member + 0xB0) * *(f32*)(member + 0x5C);
        model[0][3] += x;
        model[1][3] += y;
        model[2][3] += z;
        PSMTXConcat(cam + 0x118, model, viewModel);
        GXLoadPosMtxImm(viewModel, 0);

        color[0] = member[0x41];
        color[1] = member[0x42];
        color[2] = member[0x43];
        color[3] = member[0x44];
        GXSetChanMatColor(4, color);
        GXBegin(0x80, 0, 4);
        for (vertex = 0; vertex < 4; vertex++) {
            *(volatile f32*)0xCC008000 = (vertex & 1) ? 20.0f : -20.0f;
            *(volatile f32*)0xCC008000 = (vertex & 2) ? 0.0f : 40.0f;
            *(volatile f32*)0xCC008000 = 0.0f;
            *(volatile f32*)0xCC008000 = (f32)(vertex & 1);
            *(volatile f32*)0xCC008000 = (f32)((vertex >> 1) & 1);
        }
    }
    return 0;
}

u8 BattleAudienceDispItem(void) {
    typedef f32 Mtx[3][4];
    extern const u8 itemDataTable[];
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXTransApply(Mtx, Mtx, f32, f32, f32);
    extern void PSMTXScaleApply(Mtx, Mtx, f32, f32, f32);
    extern void iconDispGx2(Mtx, s32, s32);
    u8* item;
    Mtx rotate;
    Mtx model;
    s32 iconId;
    s32 i;

    BattleAudienceGXInit();
    item = BattleAudienceItemGetPtr(0);
    for (i = 0; i < 100; i++, item += 0x48) {
        if ((*(u32*)item & 3) != 3) {
            continue;
        }
        iconId = *(u16*)(itemDataTable + *(s32*)(item + 0x10) * 0x28 + 0x20);
        PSMTXTrans(model, 0.0f, -20.0f, 0.0f);
        PSMTXRotRad(rotate, 0.017453292f * *(f32*)(item + 0x24), 'z');
        PSMTXConcat(rotate, model, model);
        PSMTXTransApply(model, model, 0.0f, 20.0f, 0.0f);
        PSMTXScaleApply(model, model, *(f32*)(item + 0x20),
                       *(f32*)(item + 0x20), 1.0f);
        PSMTXTransApply(model, model, *(f32*)(item + 0x14),
                       *(f32*)(item + 0x18), *(f32*)(item + 0x1C));
        iconDispGx2(model, 0, iconId);
    }
    return 0;
}


u8 BattleAudienceDispApSrc(void) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void iconDispGxCol(Mtx, s32, s32, void*);
    extern const u32 dat_80424978;
    u8* base;
    u8* star;
    Mtx scale;
    Mtx rotate;
    Mtx translate;
    Mtx model;
    u32 color;
    s32 i;

    base = BattleAudienceBaseGetPtr();
    camGetPtr(8);
    BattleAudienceGXInit();
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);
    star = base + 0x10E7C;
    for (i = 0; i < 100; i++, star += 0x60) {
        if ((*(u32*)star & 1) == 0) {
            continue;
        }
        PSMTXScale(scale, *(f32*)(star + 0x38), *(f32*)(star + 0x3C),
                   *(f32*)(star + 0x40));
        PSMTXRotRad(rotate, 0.0f, 'z');
        PSMTXTrans(translate, *(f32*)(star + 8), *(f32*)(star + 0xC), 0.0f);
        PSMTXConcat(scale, rotate, model);
        PSMTXConcat(translate, model, model);
        color = dat_80424978;
        ((u8*)&color)[3] = star[0x5C];
        iconDispGxCol(model, 0x10, 0x1BA, &color);
    }
    return 0;
}

u8 BattleAudienceDispWin(void) {
    typedef f32 Mtx[3][4];
    extern void windowDispGX_Waku_col(s32, f32, f32, f32, f32, f32, s32, void*);
    extern void iconDispGx(f32, void*, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void iconNumberDispGx(Mtx, s32, s32, void*);
    extern const u32 dat_8042497c;
    extern const u32 dat_80424980;
    u8* win;
    u32 color;
    f32 pos[3];
    Mtx matrix;
    u32 numberColor;

    BattleAudienceBaseGetPtr();
    win = BattleAudienceWinGetPtr();
    color = dat_8042497c;
    windowDispGX_Waku_col(0, *(f32*)(win + 8), *(f32*)(win + 0xC),
                         90.0f, 40.0f, 10.0f, 0, &color);
    pos[0] = *(f32*)(win + 8) + 17.0f;
    pos[1] = *(f32*)(win + 0xC) - 40.0f;
    pos[2] = 0.0f;
    iconDispGx(0.8f, pos, 0x10, 0x20D);
    PSMTXTrans(matrix, *(f32*)(win + 8) + 75.0f,
               *(f32*)(win + 0xC) - 40.0f, 0.0f);
    numberColor = dat_80424980;
    iconNumberDispGx(matrix, (s32)*(f32*)(win + 0x18), 0, &numberColor);
    return 0;
}

s32 BattleAudience_Entry(s32 memberIdx, s32 audienceKind, s32 status) {
    extern s32 rand(void);
    extern void* BattleStageGetPtr(void);
    extern u8 BattleAudience_Entry_Sub(u32, u32, u8);
    extern const u8 audience_kind[];
    u8* member;
    f32 x;

    BattleAudienceBaseGetPtr();
    BattleStageGetPtr();
    member = BattleAudienceGetPtr(memberIdx);
    if ((audience_kind[audienceKind * 0x20] & 1) == 0) {
        if ((*(u32*)member & 1) != 0 || (*(u32*)member & 0x100) != 0) {
            return 0;
        }
        *(u32*)member = 5;
        x = (f32)(((memberIdx % 20) * 25 + (rand() * 10) / 0x7FFF) - 235);
        *(f32*)(member + 0x48) = x;
        *(f32*)(member + 0xB4) = x;
        BattleAudience_Entry_Sub(memberIdx, audienceKind, (u8)status);
    } else {
        if ((memberIdx & 1) != 0 || (*(u32*)member & 1) != 0 ||
            (*(u32*)member & 0x100) != 0) {
            return 0;
        }
        *(u32*)member = 0x85;
        x = 12.5f + (f32)((memberIdx % 20) * 25 - 230) +
            (f32)((rand() * 10) / 0x7FFF) - 5.0f;
        *(f32*)(member + 0x48) = x;
        *(f32*)(member + 0xB4) = x;
        BattleAudience_Entry_Sub(memberIdx, audienceKind, (u8)status);
        *(u32*)(member + 0x134) = 0x101;
    }
    return 1;
}


u8 BattleAudience_Entry_Sub(u32 memberIdx, u32 audienceKind, u8 status) {
    extern void* BattleStageGetPtr(void);
    extern void* PTR_audience_anim_kinopio_803b4de8[];
    u8* member;
    u8* stage;
    f32 y;
    f32 z;
    s32 column;

    BattleAudienceBaseGetPtr();
    stage = BattleStageGetPtr();
    member = BattleAudienceGetPtr(memberIdx);
    *(s32*)(member + 4) = 0;
    *(s32*)(member + 8) = 0;
    member[0x19] = status;
    member[0x1B] = (u8)audienceKind;
    *(s16*)(member + 0x1C) = -1;
    *(void**)(member + 0x20) = PTR_audience_anim_kinopio_803b4de8[audienceKind * 8];
    *(s32*)(member + 0x30) = 0;
    BattleAudience_SetAnim(memberIdx, 1, 1);
    member[0x40] = 0;
    member[0x41] = 0;
    member[0x42] = 0;
    member[0x43] = 0;
    member[0x44] = 0xFF;
    y = *(f32*)((s32)stage + 0x30);
    *(f32*)(member + 0x4C) = y;
    *(f32*)(member + 0xB8) = y;
    column = memberIdx % 20;
    z = (f32)((memberIdx / 20) * 30 + 115) + (f32)column * 0.1f;
    *(f32*)(member + 0x50) = z;
    *(f32*)(member + 0xBC) = z;
    if (member[0x1B] == 8) {
        *(f32*)(member + 0x50) += 0.2f;
        *(f32*)(member + 0xBC) += 0.2f;
    }
    if (status == 11) {
        *(f32*)(member + 0x4C) = -2000.0f;
    }
    *(f32*)(member + 0x84) = 0.0f;
    *(f32*)(member + 0x88) = 0.0f;
    *(f32*)(member + 0x8C) = 0.0f;
    *(f32*)(member + 0x78) = 0.0f;
    *(f32*)(member + 0x7C) = 0.0f;
    *(f32*)(member + 0x80) = 0.0f;
    *(f32*)(member + 0x90) = 0.0f;
    *(f32*)(member + 0x94) = 0.0f;
    *(f32*)(member + 0x98) = 1.0f;
    *(f32*)(member + 0x9C) = 0.0f;
    *(f32*)(member + 0xA0) = 0.0f;
    *(f32*)(member + 0xA4) = 0.0f;
    *(f32*)(member + 0xA8) = 0.0f;
    *(f32*)(member + 0xAC) = 0.0f;
    *(f32*)(member + 0xB0) = 0.0f;
    *(f32*)(member + 0x54) = 1.0f;
    *(f32*)(member + 0x58) = 1.0f;
    *(f32*)(member + 0x5C) = 1.0f;
    *(f32*)(member + 0x60) = 1.0f;
    *(f32*)(member + 0x64) = 1.0f;
    *(f32*)(member + 0x68) = 1.0f;
    *(f32*)(member + 0x6C) = 0.0f;
    *(f32*)(member + 0x70) = 0.0f;
    *(f32*)(member + 0x74) = 0.0f;
    *(f32*)(member + 0xE8) = 270.0f;
    if ((memberIdx > 3 && memberIdx < 16) ||
        (memberIdx > 23 && memberIdx < 36) ||
        (memberIdx > 43 && memberIdx < 56)) {
        *(u32*)member |= 8;
    }
    return 0;
}

void BattleAudience_Delete(s32 id) {
    void* audience;

    BattleAudienceBaseGetPtr();

    audience = BattleAudienceGetPtr(id);

    if ((*(u32*)audience & 0x80) != 0) {
        *(s32*)((s32)audience + 0x134) = 0;
    }

    *(s32*)audience = 0;
}

void BattleAudience_SetAnim(s32 memberIdx, s32 animIdx, s32 restart) {
    u8* member;
    void* anim;
    s32 i;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(memberIdx);
    if ((*(u32*)member & 1) == 0 || (*(u32*)member & 0x100) != 0) {
        return;
    }
    anim = *(void**)(*(u8**)(member + 0x20) + animIdx * 4);
    if (*(void**)(member + 0x24) == 0 ||
        *(void**)(member + 0x24) != anim || restart != 0) {
        *(void**)(member + 0x24) = anim;
        *(void**)(member + 0x28) = anim;
        *(s32*)(member + 0x2C) = 0;
        for (i = 0; i < 3; i++) {
            *(s32*)(member + 0x34 + i * 4) = 0;
        }
        member[0x40] = 0;
    }
}

s32 BattleAudience_GetAnimEnd(s32 id) {
    void* audience;
    void* anim;

    audience = BattleAudienceGetPtr(id);
    anim = *(void**)((s32)audience + 0x28);

    if (*(s32*)anim != -1) {
        return 0;
    }

    return *(s32*)((s32)audience + 0x2C) == 0;
}

void BattleAudience_GetPosition(s32 id, f32* x, f32* y, f32* z) {
    void* audience;

    audience = BattleAudienceGetPtr(id);

    *x = *(f32*)((s32)audience + 0x48);
    *y = *(f32*)((s32)audience + 0x4C);
    *z = *(f32*)((s32)audience + 0x50);
}

void BattleAudience_GetHomePosition(s32 id, f32* x, f32* y, f32* z) {
    void* audience;

    audience = BattleAudienceGetPtr(id);

    *x = *(f32*)((s32)audience + 0xB4);
    *y = *(f32*)((s32)audience + 0xB8);
    *z = *(f32*)((s32)audience + 0xBC);
}

void BattleAudience_GetRotate(s32 id, f32* x, f32* y, f32* z) {
    void* audience;

    audience = BattleAudienceGetPtr(id);

    *x = *(f32*)((s32)audience + 0x9C);
    *y = *(f32*)((s32)audience + 0xA0);
    *z = *(f32*)((s32)audience + 0xA4);
}

void BattleAudience_SetPosition(s32 id, f32 x, f32 y, f32 z) {
    BattleAudienceWorkPartial* audience;

    audience = BattleAudienceGetPtr(id);

    audience->posX = x;
    audience->posY = y;
    audience->posZ = z;
}

void BattleAudience_SetRotate(s32 id, f32 x, f32 y, f32 z) {
    BattleAudienceWorkPartial* audience;

    audience = BattleAudienceGetPtr(id);

    audience->rotX = x;
    audience->rotY = y;
    audience->rotZ = z;
}

void BattleAudience_SetRotateOffset(s32 id, f32 x, f32 y, f32 z) {
    BattleAudienceWorkPartial* audience;

    audience = BattleAudienceGetPtr(id);

    audience->rotOffsetX = x;
    audience->rotOffsetY = y;
    audience->rotOffsetZ = z;
}

void BattleAudience_GetItemOn(s32* outMemberIdx, f32* outX, f32* outY,
                              f32* outZ, u32* outItemType) {
    u8* base;
    u8* member;
    u8* item;
    s16 itemIdx;
    s32 index;

    base = BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(0);
    for (index = 0; index < 200; index++, member += 0x134) {
        if ((*(u32*)member & 1) == 0 || (*(u32*)member & 0x100) != 0 ||
            (*(u32*)member & 2) != 0) {
            continue;
        }
        itemIdx = *(s16*)(member + 0x1C);
        if (itemIdx == -1) {
            continue;
        }
        item = BattleAudienceItemGetPtr(itemIdx);
        if (*(s32*)(item + 4) > 4 && *(s32*)(item + 4) != 7) {
            continue;
        }
        if (outMemberIdx != 0) {
            *(s32*)(base + 0x138BC) = index;
            *outMemberIdx = index;
        }
        *(f32*)(base + 0x138C0) = *(f32*)(member + 0x48);
        *(f32*)(base + 0x138C4) = *(f32*)(member + 0x4C);
        *(f32*)(base + 0x138C8) = *(f32*)(member + 0x50) + 3.0f;
        if (outX != 0) {
            *outX = *(f32*)(base + 0x138C0);
        }
        if (outY != 0) {
            *outY = *(f32*)(base + 0x138C4);
        }
        if (outZ != 0) {
            *outZ = *(f32*)(base + 0x138C8);
        }
        if (outItemType != 0) {
            *outItemType = itemIdx < 0 ? 0 : *(u32*)(base + 0xF26C + itemIdx * 0x48);
        }
        return;
    }
    *outMemberIdx = -1;
}

void BattleAudience_GetItemOn2(s32* outMemberIdx, f32* outX, f32* outY,
                               f32* outZ, u32* outItemType) {
    u8* base;
    u8* member;
    s16 itemIdx;

    base = BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(*(s32*)(base + 0x138BC));
    if (outMemberIdx != 0) {
        *outMemberIdx = *(s32*)(base + 0x138BC);
    }
    if (outX != 0) {
        *outX = *(f32*)(base + 0x138C0);
    }
    if (outY != 0) {
        *outY = *(f32*)(base + 0x138C4);
    }
    if (outZ != 0) {
        *outZ = *(f32*)(base + 0x138C8);
    }
    if (outItemType != 0) {
        itemIdx = *(s16*)(member + 0x1C);
        if (itemIdx < 0) {
            *outItemType = 0;
        } else {
            *outItemType = *(u32*)(base + 0xF26C + itemIdx * 0x48);
        }
    }
}


void BattleAudience_SetTarget(s32 memberIdx) {
    u8* member;
    u8* item;
    s32 index;

    member = BattleAudienceGetPtr(memberIdx);
    *(u32*)member |= 0x20;
    for (index = 0; index < 200; index++) {
        if (index == memberIdx || !BattleAudience_GetExist(index)) {
            continue;
        }
        member = BattleAudienceGetPtr(index);
        if (member[0x19] == 7) {
            item = BattleAudienceItemGetPtr(*(s16*)(member + 0x1C));
            *(u32*)item = 0;
            *(s16*)(member + 0x1C) = -1;
            BattleAudience_ChangeStatus(index, 0);
        }
    }
}

void BattleAudience_Attack(s32 id) {
    void* audience;

    audience = BattleAudienceGetPtr(id);
    *(u32*)audience |= 0x40;
}

u32 BattleAudience_GetFront(s32 memberIdx) {
    extern void* camGetPtr(s32 cameraId);
    u8* member;
    u8* camera;
    f32 dot;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(memberIdx);
    camera = camGetPtr(3);
    dot = (*(f32*)(camera + 0x18) - *(f32*)(camera + 0xC)) *
              *(f32*)(member + 0x90) +
          (*(f32*)(camera + 0x1C) - *(f32*)(camera + 0x10)) *
              *(f32*)(member + 0x94) +
          (*(f32*)(camera + 0x20) - *(f32*)(camera + 0x14)) *
              *(f32*)(member + 0x98);
    return dot < float_0_80424988;
}

void BattleAudience_ChangeStatus(s32 memberIdx, s32 status) {
    u8* member;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(memberIdx);
    if ((*(u32*)member & 1) == 0) {
        return;
    }
    if ((*(u32*)member & 0x100) != 0) {
        if (memberIdx > 0) {
            BattleAudience_ChangeStatus(memberIdx - 1, status);
        }
        return;
    }
    member[0x41] = 0;
    member[0x42] = 0;
    member[0x43] = 0;
    member[0x44] = 0xFF;
    *(f32*)(member + 0x9C) = 0.0f;
    *(f32*)(member + 0xA0) = 0.0f;
    *(f32*)(member + 0xA4) = 0.0f;
    *(f32*)(member + 0xA8) = 0.0f;
    *(f32*)(member + 0xAC) = 0.0f;
    *(f32*)(member + 0xB0) = 0.0f;
    *(f32*)(member + 0xB4) = 1.0f;
    *(f32*)(member + 0xB8) = 1.0f;
    *(f32*)(member + 0xBC) = 1.0f;
    *(f32*)(member + 0x54) = 0.0f;
    *(f32*)(member + 0x58) = 0.0f;
    *(f32*)(member + 0x5C) = 0.0f;
    *(s32*)(member + 0x30) = 0;
    *(s32*)(member + 4) = 0;
    *(s32*)(member + 8) = 0;
    *(f32*)(member + 0xC) = 0.0f;
    *(f32*)(member + 0x10) = 0.0f;
    *(f32*)(member + 0x14) = 0.0f;
    member[0x19] = (u8)status;
}

s32 BattleAudience_GetExist(s32 id) {
    u32 flags;
    void* audience;

    BattleAudienceBaseGetPtr();

    audience = BattleAudienceGetPtr(id);
    flags = *(u32*)audience;

    if ((flags & 1) == 0) {
        return 0;
    }

    if ((flags & 0x100) != 0) {
        return 0;
    }

    return (u32)__cntlzw(flags & 2) >> 5;
}

s32 BattleAudience_GetSysCtrl(s32 id) {
    void* audience;

    BattleAudienceBaseGetPtr();

    audience = BattleAudienceGetPtr(id);

    if ((u8)BattleAudience_GetExist(id) == 0) {
    return 0;
}

    return (u32)__cntlzw(*(u32*)audience & 0x10) >> 5;
}

s32 BattleAudience_GetWaiting(s32 id) {
    void* audience;
    u8 result;

    BattleAudienceBaseGetPtr();

    audience = BattleAudienceGetPtr(id);

    if ((u8)BattleAudience_GetSysCtrl(id) == 0) {
        return 0;
    }

    if (*(s16*)((s32)audience + 0x1C) != -1) {
        return 0;
    }

    result = *(u8*)((s32)audience + 0x19) <= 3;
    return result;
}

s32 BattleAudience_GetEscapeChangeOK(s32 memberIdx) {
    u8* base;
    u8* member;
    u32 status;

    base = BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(memberIdx);
    if ((*(u32*)base & 0x4000) != 0 ||
        !BattleAudience_GetSysCtrl(memberIdx) ||
        (*(u32*)member & 0x200) != 0) {
        return 0;
    }
    status = member[0x19];
    if (status == 12 || status == 13 || status == 14 || status == 15 ||
        status == 17 || status == 18 || status == 21 || status == 22) {
        return 0;
    }
    return status < 12;
}

s32 BattleAudience_GetAudienceNoFromOffset(s32 id, s32 offset, s32 arg) {
    s32 col;
    s32 result;

    BattleAudienceBaseGetPtr();
    BattleAudienceGetPtr(id);

    col = id - ((id / 20) * 20);
    col += offset;
    if (col < 0 || col >= 20) {
        return -1;
    }

    result = id - (arg * 20);
    result = offset + result;
    if (result < 0 || result >= 200) {
        return -1;
    }

    return result;
}


s32 BattleAudience_HaitiRandForFallObject(void) {
    s32 column;
    s32 row;

    column = irand(3);
    row = 10;
    irand(12);
    if (column == 0) {
        row = irand(12) + 4;
    } else if (column == 1) {
        row = irand(10) + 5;
    } else if (column == 2) {
        row = irand(8) + 6;
    }
    return column * 20 + row;
}

s32 BattleAudience_NoUsedHaitiRand(void) {
    s32 candidates[0xC8];
    void* pouch;
    s32* data;
    s32 count;
    s32 offset;
    s32 row;
    s32 rowBase;
    s32 rowOffset;
    s32 col;
    s32 start;
    s32 id;
    s32 flags;

    BattleAudienceBaseGetPtr();

    pouch = pouchGetPtr();
    data = &haiti_data[*(s16*)((s32)pouch + 0x88) * 10];

    count = 0;
    offset = 0;
    row = 0;
    rowOffset = 0;

    while (row < 10) {
        rowBase = data[row];
        start = (0x14 - rowBase) / 2;
        col = 0;

        while (col < rowBase) {
            id = start + rowOffset;

            flags = *(s32*)BattleAudienceGetPtr(id);

            if ((flags & 1) == 0) {
                flags = *(s32*)BattleAudienceGetPtr(id);

                if ((flags & 0x100) == 0) {
                    candidates[count] = id;
                    count++;
                    offset += 4;
                }
            }

            col++;
            start++;
        }

        row++;
        rowOffset += 0x14;
    }

    if (count >= 1) {
        return candidates[irand(count)];
    }

    return -1;
}

s32 unk_801a23e0(void) {
    extern s32 rand(void);
    extern s32 dat_802fa134[10];
    extern s32 dat_802fa15c[20];
    extern f32 float_32767_804249e4;
    s32 ids[200];
    s32 weights[200];
    s32 row;
    s32 col;
    s32 count;
    s32 start;
    s32 id;
    s32 found;
    s32 total;
    s32 pick;
    s32 offset;
    s32 weight;
    s32* haiti;
    s32 stage;
    void* audience;

    BattleAudienceBaseGetPtr();
    stage = *(s16*)((s32)pouchGetPtr() + 0x88);
    haiti = &haiti_data[stage * 10];
    found = 0;
    total = 0;
    offset = 0;

    for (row = 0; row < 10; row++) {
        count = haiti[row];
        start = (20 - count) / 2;
        for (col = 0; col < count; col++) {
            id = row * 20 + start + col;
            audience = BattleAudienceGetPtr(id);
            if ((*(u32*)audience & 1) == 0) {
                audience = BattleAudienceGetPtr(id);
                if ((*(u32*)audience & 0x100) == 0) {
                    ids[found] = id;
                    weight = dat_802fa134[row] * dat_802fa15c[start + col] + 1;
                    if (found != 0) {
                        weights[found] = weights[found - 1] + weight;
                    } else {
                        weights[found] = weight;
                    }
                    total += weight;
                    found++;
                }
            }
        }
    }

    pick = (s32)(((f32)total * (f32)rand()) / float_32767_804249e4);
    for (offset = 0; offset < found; offset++) {
        if (pick < weights[offset]) {
            return ids[offset];
        }
    }
    return -1;
}

s32 unk_801a21e0(void) {
    extern s32 rand(void);
    extern s32 dat_802fa1ac[10];
    extern s32 dat_802fa1d4[20];
    extern s32 unk_8039cc20[];
    extern f32 float_32767_804249e4;
    s32 ids[200];
    s32 weights[200];
    s32 row;
    s32 col;
    s32 count;
    s32 start;
    s32 id;
    s32 found;
    s32 total;
    s32 pick;
    s32 offset;
    s32 weight;
    s32* haiti;
    s32 stage;
    void* audience;

    BattleAudienceBaseGetPtr();
    stage = *(s16*)((s32)pouchGetPtr() + 0x88);
    haiti = &unk_8039cc20[stage * 10];
    found = 0;
    total = 0;
    offset = 0;

    for (row = 0; row < 10; row++) {
        count = haiti[row];
        start = (20 - count) / 2;
        for (col = 0; col < count; col++) {
            id = row * 20 + start + col;
            audience = BattleAudienceGetPtr(id);
            if ((*(u32*)audience & 1) == 0) {
                audience = BattleAudienceGetPtr(id);
                if ((*(u32*)audience & 0x100) == 0) {
                    ids[found] = id;
                    weight = dat_802fa1ac[row] * dat_802fa1d4[start + col] + 1;
                    if (found != 0) {
                        weights[found] = weights[found - 1] + weight;
                    } else {
                        weights[found] = weight;
                    }
                    total += weight;
                    found++;
                }
            }
        }
    }

    pick = (s32)(((f32)total * (f32)rand()) / float_32767_804249e4);
    for (offset = 0; offset < found; offset++) {
        if (pick < weights[offset]) {
            return ids[offset];
        }
    }
    return -1;
}
s32 BattleAudience_NoUsedFCHaitiRand(void) {
    s32 result;

    if (BattleAudience_GetAudienceNum() >= 0x14) {
        return unk_801a23e0();
    }

    result = unk_801a21e0();

    if (result == -1) {
        result = unk_801a23e0();
    }

    return result;
}

s32 BattleAudience_GetAudienceNum(void) {
    return *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x13784);
}

s32 BattleAudience_GetPPAudienceNum(void) {
    s32 left;
    s32 right;

    left = BattleAudience_GetPPAudienceNum_L();
    right = BattleAudience_GetPPAudienceNum_R();

    return right + left;
}

s32 BattleAudience_GetPPAudienceNumKind(s32 kind) {
    s32 i;
    s32 count;
    void* audience;

    BattleAudienceBaseGetPtr();

    count = 0;
    i = 0;

    while (i < 0xC8) {
        if (BattleAudience_GetPPAudienceNum_Sub(i) != 0) {
            audience = BattleAudienceGetPtr(i);

            if (kind == *(u8*)((s32)audience + 0x1B)) {
                count++;
            }
        }

        i++;
    }

    return count;
}

s32 BattleAudience_GetPPAudienceNum_R(void) {
    return BattleAudience_GetPPAudienceNum_RL_Sub(10);
}

s32 BattleAudience_GetPPAudienceNum_L(void) {
    return BattleAudience_GetPPAudienceNum_RL_Sub(0);
}

s32 BattleAudience_GetPPAudienceNum_RL_Sub(s32 side) {
    s32 count;
    s32 row;
    s32 baseIndex;
    s32 col;

    BattleAudienceBaseGetPtr();

    count = 0;
    row = 0;
    baseIndex = 0;

    while (row < 10) {
        col = 0;

        while (col < 10) {
            if (BattleAudience_GetPPAudienceNum_Sub(baseIndex + side + col) == 1) {
                count++;
            }

            col++;
        }

        row++;
        baseIndex += 0x14;
    }

    return count;
}

s32 BattleAudience_GetPPAudienceNum_Sub(s32 id) {
    void* audience;
    u8 status;

    audience = BattleAudienceGetPtr(id);

    if ((u8)BattleAudience_GetExist(id) == 0) {
        return 0;
    }

    status = *(u8*)((s32)audience + 0x19);

    if (status >= 0xD && status <= 0xD) {
        return 0;
    }

    if (status == 0x11) {
        return 0;
    }

    return (0xC - status) != 0;
}

void BattleAudience_ApRecoveryBuild(void* apInfo) {
    extern void* BattleAlloc(u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern f64 sqrt(f64 value);
    extern void* BattleGetMarioPtr(void* battle);
    extern void* BattleGetPartyPtr(void* battle);
    extern s32 BattleAudience_GetPPAudienceNumKind(s32 kind);
    extern s32 BattleBreakSlot_GetBreakTurn(void);
    extern void* BattleBreakSlotGetPtr(void);
    extern void BattleBreakSlot_PointInc(void);
    extern const u8 audience_kind[];
    u8* base;
    u8* mario;
    u8* party;
    u8* breakSlot;
    f32 audienceValue;
    f32 actionValue;
    f32 marioRate;
    f32 partyRate;
    f32 result;
    s32 i;

    base = BattleAudienceBaseGetPtr();
    if (*(void**)(base + 0x137C8) == 0) {
        *(void**)(base + 0x137C8) = BattleAlloc(12);
    }
    memcpy(*(void**)(base + 0x137C8), apInfo, 12);
    mario = BattleGetMarioPtr(_battleWorkPointer);
    party = BattleGetPartyPtr(_battleWorkPointer);
    audienceValue = float_0_80424988;
    for (i = 0; i < 13; i++) {
        audienceValue += *(f32*)(audience_kind + i * 0x20 + 0x14) *
            (f32)BattleAudience_GetPPAudienceNumKind(i);
    }
    audienceValue = (f32)sqrt(audienceValue);
    actionValue = *(f32*)((u8*)apInfo + 4) + (f32)*(u8*)((u8*)apInfo + 8);
    marioRate = 1.0f;
    if ((*(u32*)(mario + 0x138) & 0x20000000) != 0) {
        marioRate = 3.0f;
    } else if ((*(u32*)(mario + 0x138) & 0x10000000) != 0) {
        marioRate = 2.0f;
    }
    partyRate = 1.0f;
    if (party != 0) {
        if ((*(u32*)(party + 0x138) & 0x20000000) != 0) {
            partyRate = 2.0f;
        } else if ((*(u32*)(party + 0x138) & 0x10000000) != 0) {
            partyRate = 1.5f;
        }
    }
    result = audienceValue * actionValue * marioRate * partyRate;
    if (BattleBreakSlot_GetBreakTurn() > 0) {
        breakSlot = BattleBreakSlotGetPtr();
        result *= *(f32*)(breakSlot + 0x10);
    }
    *(s32*)(base + 0x137C4) += (s32)result;
    if (irand(100) < *(u8*)((u8*)apInfo + 9)) {
        BattleBreakSlot_PointInc();
    }
}

void BattleAudience_WinSetActive(s32 active) {
    void* win = BattleAudienceWinGetPtr();
    *(u8*)win = active;
}

void BattleAudienceNumToTarget(void) {
    void* base;
    s32 target;
    s32 current;

    base = BattleAudienceBaseGetPtr();

    if ((*(u32*)base & 0x40000) == 0) {
        target = (s32)(*(f32*)((s32)base + 0x1377C) + *(f32*)((s32)base + 0x13780));

        if (target < 0) {
            target = 0;
        }

        if (target > *(s32*)((s32)base + 0x13790)) {
            target = *(s32*)((s32)base + 0x13790);
        }

        current = BattleAudienceNumToTargetSub();
        BattleAudienceAddAudienceNum(target - current);
    }
}

s32 BattleAudienceNumToTargetSub(void) {
    s32 i;
    s32 count;
    void* audience;

    BattleAudienceBaseGetPtr();

    count = 0;
    i = 0;

    while (i < 0xC8) {
        if ((u8)BattleAudience_GetExist(i) == 1) {
            audience = BattleAudienceGetPtr(i);

            if (*(u8*)((s32)audience + 0x19) != 0xC) {
                count++;
            }
        }

        i++;
    }

    return count;
}

void BattleAudienceAddTargetNum(f32 added, f32 carry) {
    extern const f32 float_neg1000_804249d8;
    extern const f32 float_1000_804249dc;
    u8* base;
    f32 step;
    s32 maxAudience;

    base = BattleAudienceBaseGetPtr();
    *(f32*)(base + 0x13780) += carry;
    step = added;
    if (*(f32*)(base + 0x13780) > float_0_80424988 &&
        added < float_0_80424988) {
        *(f32*)(base + 0x13780) += added;
        step = float_0_80424988;
        if (*(f32*)(base + 0x13780) < float_0_80424988) {
            step = *(f32*)(base + 0x13780);
            *(f32*)(base + 0x13780) = float_0_80424988;
        }
    } else if (*(f32*)(base + 0x13780) < float_0_80424988 &&
               added > float_0_80424988) {
        *(f32*)(base + 0x13780) += added;
        step = float_0_80424988;
        if (*(f32*)(base + 0x13780) > float_0_80424988) {
            step = *(f32*)(base + 0x13780);
            *(f32*)(base + 0x13780) = float_0_80424988;
        }
    }
    ((void (*)(f32))BattleAudienceAddTargetNumSub)(step);
    if (*(f32*)(base + 0x1377C) < float_0_80424988) {
        *(f32*)(base + 0x1377C) = float_0_80424988;
    }
    maxAudience = *(s32*)(base + 0x13790);
    if (*(f32*)(base + 0x1377C) > (f32)maxAudience) {
        *(f32*)(base + 0x1377C) = (f32)maxAudience;
    }
    if (*(f32*)(base + 0x13780) < float_neg1000_804249d8) {
        *(f32*)(base + 0x13780) = float_neg1000_804249d8;
    }
    if (*(f32*)(base + 0x13780) > float_1000_804249dc) {
        *(f32*)(base + 0x13780) = float_1000_804249dc;
    }
}

u8 BattleAudienceAddTargetNumSub(void) {
    return 0;
}

void BattleAudienceAddAudienceNum(s32 amount) {
    s32 candidates[200];
    u8* base;
    u8* pouch;
    u8* member;
    s32 totalWeight;
    s32 runningWeight;
    s32 result;
    s32 count;
    s32 slot;
    s32 type;
    s32 i;

    base = BattleAudienceBaseGetPtr();
    result = 0;
    pouch = pouchGetPtr();
    if (*(u16*)(pouch + 0x8C) == 0 || amount == 0 ||
        (*(u32*)((s32)_battleWorkPointer + 0xEF4) & 0x100) != 0) {
        return;
    }
    if (amount > 0) {
        totalWeight = 0;
        for (i = 0; i < 12; i++) {
            totalWeight += *(s32*)(base + 0x13794 + i * 4);
        }
        for (i = 0; i < amount; i++) {
            slot = irand(totalWeight);
            runningWeight = 0;
            type = 0;
            while (type < 12) {
                runningWeight += *(s32*)(base + 0x13794 + type * 4);
                if (slot < runningWeight) {
                    break;
                }
                type++;
            }
            slot = BattleAudience_NoUsedFCHaitiRand();
            if (slot == -1) {
                break;
            }
            if (BattleAudience_Entry(slot, (u8)type, 11) == 1) {
                result++;
                member = BattleAudienceGetPtr(slot);
                *(s32*)(member + 8) = result;
            }
        }
    } else {
        for (i = 0; i > amount; i--) {
            count = 0;
            for (slot = 0; slot < 200; slot++) {
                if (BattleAudience_GetEscapeChangeOK(slot)) {
                    candidates[count++] = slot;
                }
            }
            if (count > 0) {
                slot = candidates[irand(count)];
                BattleAudience_ChangeStatus(slot, 12);
                result--;
                member = BattleAudienceGetPtr(slot);
                *(s32*)(member + 8) = -result;
            }
        }
    }
}

void BattleAudiencePuniAllEscape(void) {
    void* base;
    void* highBase;
    s32 i;
    s32 count;
    void* audience;

    base = BattleAudienceBaseGetPtr();
    highBase = (void*)((s32)base + 0x10000);

    count = 0;
    i = 0;

    while (i < 0xC8) {
        audience = BattleAudienceGetPtr(i);

        if ((u8)BattleAudience_GetEscapeChangeOK(i) != 0) {
            if (*(u8*)((s32)audience + 0x1B) == 7) {
                BattleAudience_ChangeStatus(i, 0xC);
                *(f32*)((s32)highBase + 0x3780) -= float_1_80424990;
                count++;
            }
        }

        i++;
    }

    if (count >= 1) {
        *(u32*)base |= 0x8000;
    }
}

void BattleAudienceAddPuni(s32 count) {
    void* pouch;
    void* base;
    void* battleWork;
    s32 i;
    s32 id;

    battleWork = _battleWorkPointer;
    base = BattleAudienceBaseGetPtr();
    pouch = pouchGetPtr();

    if (*(u16*)((s32)pouch + 0x8C) != 0) {
        if (*(s32*)((s32)base + 0x137B0) > 0) {
            if (count > 0) {
                if ((*(u32*)((s32)battleWork + 0xEF4) & 0x100) == 0) {
                    i = 0;

                    while (i < count) {
                        id = BattleAudience_NoUsedFCHaitiRand();

                        if (id == -1) {
                            return;
                        }

                        if (BattleAudience_Entry(id, 7, 0xB) == 1) {
                            *(f32*)((s32)base + 0x13780) += float_1_80424990;
                        }

                        i++;
                    }
                }
            }
        }
    }
}

void BattleAudienceAddPhaseEvtList(s32 evt) {
    void* base;
    s32 count;
    s32 i;
    s32 offset;

    base = BattleAudienceBaseGetPtr();
    count = *(s32*)((s32)base + 0x13904);
    offset = 0;

    for (i = count; i > 0; i--) {
        if (*(s32*)((s32)base + offset + 0x138CC) == evt) {
            return;
        }
        offset += 4;
    }

    *(s32*)((s32)base + (count << 2) + 0x138CC) = evt;
    *(s32*)((s32)base + 0x13904) = *(s32*)((s32)base + 0x13904) + 1;
}

void BattleAudienceCheer(s32 kind) {
    void* base;

    base = BattleAudienceBaseGetPtr();

    if (*(s32*)((s32)base + 0x137C4) > 0x320) {
        *(s32*)((s32)base + 0x137C4) = 0x320;
    }

    if (*(s32*)((s32)base + 0x137C4) > 0) {
        if (*(s16*)((s32)pouchGetPtr() + 0x7C) == 0) {
            *(s32*)((s32)base + 0x137C4) = 0;
        } else {
            *(u32*)base |= 2;
        }

        BattleAudienceJoy(kind);

        if (*(s32*)((s32)base + 0x137C4) != 0) {
            BattleStatusWindowAPRecoveryOn();
        }
    }
}


void BattleAudienceJoy(s32 kind) {
    extern void* BattleAudienceBaseGetPtr(void);
    extern void BattleAudienceJoy_Sub(s32 arg0, s32 arg1, s32 arg2, s32 kind);
    u8* base;
    s32 level;

    base = BattleAudienceBaseGetPtr();
    switch (kind) {
        case 0:
            BattleAudienceJoy_Sub(0x64, 0, 0, kind);
            break;
        case 1:
            BattleAudienceJoy_Sub(0, 0x64, 0, kind);
            break;
        case 2:
            BattleAudienceJoy_Sub(0x46, 0x1E, 0, kind);
            break;
        case 3:
            level = *(s32*)(base + 0x1390C);
            switch (level) {
                case 0:
                    BattleAudienceJoy_Sub(0x32, 0x28, 0xA, kind);
                    break;
                case 1:
                    BattleAudienceJoy_Sub(0x28, 0x28, 0x14, kind);
                    break;
                case 2:
                    BattleAudienceJoy_Sub(0x1E, 0x28, 0x1E, kind);
                    break;
                case 3:
                    BattleAudienceJoy_Sub(0x14, 0x28, 0x28, kind);
                    break;
                case 4:
                    BattleAudienceJoy_Sub(0xA, 0x28, 0x32, kind);
                    break;
                default:
                    BattleAudienceJoy_Sub(0x32, 0x28, 0xA, kind);
                    break;
            }
            *(s32*)(base + 0x1390C) = *(s32*)(base + 0x1390C) + 1;
            if (*(s32*)(base + 0x1390C) > 4) {
                *(s32*)(base + 0x1390C) = 4;
            }
            break;
    }
}

void BattleAudienceJoySACLecture(void) {
    BattleAudienceSoundCheer(0x78, 0x3C);
    BattleAudienceJoy(2);
}

void BattleAudienceJoyEnding(void) {
    BattleAudienceJoy(3);
}

void BattleAudienceJoy_Sub(s32 joy, s32 cheer, s32 boo, s32 kind) {
    s32 total;
    s32 memberIdx;
    s32 value;
    u8* member;

    BattleAudienceBaseGetPtr();
    total = joy + cheer + boo;
    member = BattleAudienceGetPtr(0);
    for (memberIdx = 0; memberIdx < 200; memberIdx++, member += 0x134) {
        if (!BattleAudience_GetWaiting(memberIdx)) {
            continue;
        }
        if (member[0x1B] == 8) {
            BattleAudience_ChangeStatus(memberIdx, 4);
            *(f32*)(member + 0xC) = kind == 3 ? 3.0f : 2.0f;
            continue;
        }
        value = irand(total);
        if (value < joy) {
            BattleAudience_ChangeStatus(memberIdx, 4);
            *(f32*)(member + 0xC) = kind == 3 ? 3.0f : 2.0f;
        } else if (value < joy + cheer) {
            BattleAudience_ChangeStatus(memberIdx, 5);
        } else if (value < total) {
            BattleAudience_ChangeStatus(memberIdx, 6);
        }
    }
}

s32 BattleAudienceDetectPakkunEatTargetSub2(s32 id, s32 offset) {
    s32 audienceNo;

    BattleAudienceBaseGetPtr();

    audienceNo = BattleAudience_GetAudienceNoFromOffset(id, offset, 0);

    if (audienceNo == -1) {
        return -1;
    }

    BattleAudienceGetPtr(audienceNo);

    if ((u8)BattleAudience_GetSysCtrl(audienceNo) == 1) {
        return audienceNo;
    }

    return -1;
}

s32 BattleAudienceDetectPakkunEatTargetSub(s32 id, s32 offsetA, s32 offsetB) {
    s32 first;
    s32 second;
    s32 result;

    first = BattleAudienceDetectPakkunEatTargetSub2(id, offsetA);
    second = BattleAudienceDetectPakkunEatTargetSub2(id, offsetB);

    if (first != -1) {
        return first;
    }

    if (second != -1) {
        result = second;
    } else {
        result = -1;
    }

    return result;
}

s32 BattleAudienceDetectPakkunEatTarget(s32 id) {
    if (irand(2) == 0) {
        return BattleAudienceDetectPakkunEatTargetSub(id, -1, 1);
    }

    return BattleAudienceDetectPakkunEatTargetSub(id, 1, -1);
}

void BattleAudienceSoundBooing(void) {
    s32 kind;
    s32 left;
    s32 right;
    s32 total;

    BattleAudienceBaseGetPtr();

    kind = 0;

    left = BattleAudience_GetPPAudienceNum_L();
    right = BattleAudience_GetPPAudienceNum_R();
    total = right + left;

    if (total >= 0 && total <= 0x31) {
        kind = 1;
    }

    if (total >= 0x32 && total <= 0x63) {
        kind = 2;
    }

    if (total >= 0x64 && total <= 0x95) {
        kind = 3;
    }

    if (total >= 0x96) {
        kind = 4;
    }

    BattleAudienceSoundBooingKind(kind);
}

void BattleAudienceSoundBooingKind(s32 kind) {
    char name[0x34];
    void* sound;
    s32 right;
    s32 left;
    const char* baseName;

    baseName = str_btl_msg_audience_pun_802f9e28;
    BattleAudienceBaseGetPtr();

    sound = BattleAudienceSoundGetPtr(0);

    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();

    BattleAudienceSoundStop(0);

    if (kind == 1) {
        strcpy(name, baseName + 0x7FC);
        *(u8*)((s32)sound + 0x8) = 1;
    }

    if (kind == 2) {
        strcpy(name, baseName + 0x814);
        *(u8*)((s32)sound + 0x8) = 2;
    }

    if (kind == 3) {
        strcpy(name, baseName + 0x82C);
        *(u8*)((s32)sound + 0x8) = 3;
    }

    if (kind == 4) {
        strcpy(name, baseName + 0x844);
        *(u8*)((s32)sound + 0x8) = 4;
    }

    if (kind == 5) {
        strcpy(name, baseName + 0x85C);
        *(u8*)((s32)sound + 0x8) = 5;
    }

    if (kind == 6) {
        strcpy(name, baseName + 0x874);
        *(u8*)((s32)sound + 0x8) = 6;
    }

    if (kind == 7) {
        strcpy(name, baseName + 0x88C);
        *(u8*)((s32)sound + 0x8) = 7;
    }

    if (kind == 8) {
        strcpy(name, baseName + 0x8A4);
        *(u8*)((s32)sound + 0x8) = 8;
    }

    *(s32*)((s32)sound + 0x4) = BattleAudienceSound1(name, right, left);

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
}

void BattleAudienceSoundClap(s32 vol, s32 frames) {
    s32 vol2;
    s32 frames2;
    char name[0x3C];
    void* sound;
    s32 right;
    s32 left;
    s32 total;
    const char* baseName;

    vol2 = vol;
    frames2 = frames;
    baseName = str_btl_msg_audience_pun_802f9e28;
    BattleAudienceBaseGetPtr();

    sound = BattleAudienceSoundGetPtr(1);

    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    total = right + left;

    BattleAudienceSoundStop(1);

    if (total >= 1 && total <= 0x31) {
        strcpy(name, baseName + 0x79C);
        *(u8*)((s32)sound + 0x8) = 1;
    }

    if (total >= 0x32 && total <= 0x63) {
        strcpy(name, baseName + 0x7B4);;
        *(u8*)((s32)sound + 0x8) = 2;
    }

    if (total >= 0x64 && total <= 0x95) {
        strcpy(name, baseName + 0x7CC);;
        *(u8*)((s32)sound + 0x8) = 3;
    }

    if (total >= 0x96) {
        strcpy(name, baseName + 0x7E4);
        *(u8*)((s32)sound + 0x8) = 4;
    }

    *(s32*)((s32)sound + 0x4) = BattleAudienceSound1(name, right, left);

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, vol2 + frames2, frames2);
    }
}

void BattleAudienceSoundNoiseAlways(void) {
    extern const char str_SFX_AUDIENCE_NOISY1_802fa574[];
    extern const char str_SFX_AUDIENCE_NOISY2_802fa588[];
    extern const char str_SFX_AUDIENCE_NOISY3_802fa59c[];
    extern const char str_SFX_AUDIENCE_NOISY4_802fa5b0[];
    char name[64];
    s32 right;
    s32 left;
    s32 total;
    void* base;
    void* sound;

    base = BattleAudienceBaseGetPtr();
    sound = BattleAudienceSoundGetPtr(2);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    total = right + left;
    if ((*(u32*)sound & 1) != 0) {
        return;
    }
    if (total == 0) {
        *(u8*)((s32)sound + 8) = 0;
    } else if (total < 50) {
        strcpy(name, str_SFX_AUDIENCE_NOISY1_802fa574);
        *(u8*)((s32)sound + 8) = 1;
    } else if (total < 100) {
        strcpy(name, str_SFX_AUDIENCE_NOISY2_802fa588);
        *(u8*)((s32)sound + 8) = 2;
    } else if (total < 150) {
        strcpy(name, str_SFX_AUDIENCE_NOISY3_802fa59c);
        *(u8*)((s32)sound + 8) = 3;
    } else {
        strcpy(name, str_SFX_AUDIENCE_NOISY4_802fa5b0);
        *(u8*)((s32)sound + 8) = 4;
    }
    *(s32*)((s32)sound + 4) = BattleAudienceSound1(name, right, left);
    if (*(s32*)((s32)sound + 4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
        *(u32*)base |= 0x2000;
    }
}

void BattleAudienceSoundCheer(s32 vol, s32 frames) {
    s32 kind;
    s32 left;
    s32 right;
    s32 total;

    BattleAudienceBaseGetPtr();

    kind = 0;

    left = BattleAudience_GetPPAudienceNum_L();
    right = BattleAudience_GetPPAudienceNum_R();
    total = right + left;

    if (total >= 1 && total <= 0x31) {
        kind = 1;
    }

    if (total >= 0x32 && total <= 0x63) {
        kind = 2;
    }

    if (total >= 0x64 && total <= 0x95) {
        kind = 3;
    }

    if (total >= 0x96) {
        kind = 4;
    }

    BattleAudienceSoundCheerKind(kind, vol, frames);
}

void BattleAudienceSoundCheerKind(s32 kind, s32 vol, s32 frames) {
    char name[0x48];
    void* sound4;
    void* sound5;
    void* sound;
    s32 right;
    s32 left;
    s32 usePitch;
    const char* baseName;
    s32 sfxVol;
    s32 active4;

    baseName = str_btl_msg_audience_pun_802f9e28;
    BattleAudienceBaseGetPtr();

    usePitch = 0;

    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();

    sound4 = BattleAudienceSoundGetPtr(4);
    sound5 = BattleAudienceSoundGetPtr(5);
    sound = sound4;

    active4 = *(u32*)sound4 & 1;
    if (active4 != 0 && (*(u32*)sound5 & 1) != 0) {
        if (*(s32*)((s32)sound4 + 0xC) <= *(s32*)((s32)sound5 + 0xC)) {
            BattleAudienceSoundStop(4);
            sound = BattleAudienceSoundGetPtr(4);
        } else {
            BattleAudienceSoundStop(5);
            sound = BattleAudienceSoundGetPtr(5);
        }
    } else {
        if (active4 != 0) {
            sound = BattleAudienceSoundGetPtr(5);
            usePitch = 1;
        } else {
            if ((*(u32*)sound5 & 1) != 0) {
                sound = BattleAudienceSoundGetPtr(4);
                usePitch = 1;
            } else {
                sound = BattleAudienceSoundGetPtr(4);
            }
        }
    }

    if (kind == 1) {
        strcpy(name, baseName + 0x6DC);
        *(u8*)((s32)sound + 0x8) = 1;
    }

    if (kind == 2) {
        strcpy(name, baseName + 0x6F8);
        *(u8*)((s32)sound + 0x8) = 2;
    }

    if (kind == 3) {
        strcpy(name, baseName + 0x714);
        *(u8*)((s32)sound + 0x8) = 3;
    }

    if (kind == 4) {
        strcpy(name, baseName + 0x730);
        *(u8*)((s32)sound + 0x8) = 4;
    }

    if (usePitch == 1) {
        *(s32*)((s32)sound + 0x4) = BattleAudienceSound1(name, right, left);
        psndSFX_pit(*(s32*)((s32)sound + 0x4), 0x7F);
    } else {
        *(s32*)((s32)sound + 0x4) = BattleAudienceSound1(name, right, left);
    }

    if (*(s32*)((s32)sound + 0x4) != -1) {
        *(u32*)sound |= 1;
        *(s32*)((s32)sound + 0xC) = vol + frames;
        *(s32*)((s32)sound + 0x10) = frames;
        sfxVol = psndSFX_get_vol(*(s32*)((s32)sound + 0x4));
        *(u8*)((s32)sound + 0x1D) = sfxVol;
        *(u8*)((s32)sound + 0x1C) = sfxVol;
        *(s32*)((s32)sound + 0x18) = 0;
        *(s32*)((s32)sound + 0x14) = 0;
        *(u8*)((s32)sound + 0x1E) = 0x64;
    }
}

void BattleAudienceSoundWhistle(void) {
    BattleAudienceSoundWhistleKind(irand(3) + 1);
}

void BattleAudienceSoundWhistleKind(s32 kind) {
    extern const char str_SFX_AUDIENCE_WHISTLE_802fa4bc[];
    extern const char str_SFX_AUDIENCE_WHISTLE_802fa4d4[];
    extern const char str_SFX_AUDIENCE_WHISTLE_802fa4ec[];
    s32 waiting[200];
    f32 pos[3];
    s32 count;
    s32 memberIdx;
    s32 soundId;
    s32 i;
    void* sound;

    count = 0;
    for (i = 0; i < 200; i++) {
        if (BattleAudience_GetWaiting(i)) {
            waiting[count++] = i;
        }
    }
    if (count == 0) {
        return;
    }
    memberIdx = waiting[irand(count)];
    BattleAudienceSoundGetInfo2(memberIdx, pos);
    soundId = pos[0] <= float_0_80424988 ? 6 : 7;
    sound = BattleAudienceSoundGetPtr(soundId);
    BattleAudienceSoundStop(soundId);
    if (kind == 1) {
        *(s32*)((s32)sound + 4) = BattleAudienceSound2(
            str_SFX_AUDIENCE_WHISTLE_802fa4bc, memberIdx);
    } else if (kind == 2) {
        *(s32*)((s32)sound + 4) = BattleAudienceSound2(
            str_SFX_AUDIENCE_WHISTLE_802fa4d4, memberIdx);
    } else if (kind == 3) {
        *(s32*)((s32)sound + 4) = BattleAudienceSound2(
            str_SFX_AUDIENCE_WHISTLE_802fa4ec, memberIdx);
    }
    if (*(s32*)((s32)sound + 4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
}

void BattleAudienceSoundCallKind(s32 kind) {
    void* sound;
    s32 right;
    s32 left;

    BattleAudienceBaseGetPtr();

    sound = BattleAudienceSoundGetPtr(8);

    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();

    BattleAudienceSoundStop(8);

    switch (kind) {
        case 1:
            *(s32*)((s32)sound + 0x4) = BattleAudienceSound1(
                str_SFX_AUDIENCE_CALL_AL_802fa48c,
                right,
                left
            );
            break;
        case 2:
            *(s32*)((s32)sound + 0x4) = BattleAudienceSound1(
                str_SFX_AUDIENCE_CALL_AL_802fa4a4,
                right,
                left
            );
            break;
    }

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
}

void BattleAudienceSoundHandBeat(void) {
    s32 right;
    void* sound;
    s32 left;
    s32 total;

    BattleAudienceBaseGetPtr();

    sound = BattleAudienceSoundGetPtr(9);

    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    total = right + left;

    BattleAudienceSoundStop(9);

    *(s32*)((s32)sound + 0x4) = BattleAudienceSound1(
        str_SFX_AUDIENCE_HANDBEA_802fa474,
        right,
        left
    );

    if (*(s32*)((s32)sound + 0x4) != -1) {
        *(u32*)sound |= 1;
        *(s32*)((s32)sound + 0xC) = -1;
        *(s32*)((s32)sound + 0x10) = 0;
        *(s32*)((s32)sound + 0x18) = 0;
        *(s32*)((s32)sound + 0x14) = 0;

        right = 0;
        if (total != 0) {
            right = (s32)(float_75_804249c8 + (((irand(0x1A) + float_26_804249c0) * total) / float_200_804249b8));
        }

        psndSFX_vol(*(s32*)((s32)sound + 0x4), right);
        *(u8*)((s32)sound + 0x1D) = right;
        *(u8*)((s32)sound + 0x1C) = right;
        *(u8*)((s32)sound + 0x1E) = 0x64;
    }
}

void BattleAudienceSoundItemThrow(s32 kind, s32 memberIdx) {
    extern const char str_SFX_AUDIENCE_THROW_O_802fa43c[];
    extern const char str_SFX_AUDIENCE_THROW_O_802fa458[];
    f32 pos[3];
    s32 soundId;
    void* sound;

    BattleAudienceSoundGetInfo2(memberIdx, pos);
    soundId = pos[0] <= float_0_80424988 ? 10 : 11;
    sound = BattleAudienceSoundGetPtr(soundId);
    BattleAudienceSoundStop(soundId);
    if (kind == 0) {
        *(s32*)((s32)sound + 4) = BattleAudienceSound2(
            str_SFX_AUDIENCE_THROW_O_802fa43c, memberIdx);
    } else {
        *(s32*)((s32)sound + 4) = BattleAudienceSound2(
            str_SFX_AUDIENCE_THROW_O_802fa458, memberIdx);
    }
    if (*(s32*)((s32)sound + 4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
}

u8 BattleAudienceSoundRun(int kind) {
    extern const char str_SFX_AUDIENCE_RUN1_802fa414[];
    extern const char str_SFX_AUDIENCE_RUN2_802fa428[];
    void* sound;

    sound = BattleAudienceSoundGetPtr(0xD);
    BattleAudienceSoundStop(0xD);

    if (kind == 0) {
        *(s32*)((s32)sound + 4) = psndSFXOn_3D(
            str_SFX_AUDIENCE_RUN1_802fa414, 0);
    } else if (kind == 1) {
        *(s32*)((s32)sound + 4) = psndSFXOn_3D(
            str_SFX_AUDIENCE_RUN2_802fa428, 0);
    }

    if (*(s32*)((s32)sound + 4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
    return 0;
}

u8 BattleAudienceSoundSleep(int kind) {
    extern const char str_SFX_AUDIENCE_SLEEP1_802fa3d8[];
    extern const char str_SFX_AUDIENCE_SLEEP2_802fa3ec[];
    extern const char str_SFX_AUDIENCE_SLEEP3_802fa400[];
    f32 pos[3];
    void* sound;
    s32 soundKind;
    s32 choice;

    BattleAudienceSoundGetInfo2(kind, pos);
    soundKind = pos[0] > float_0_80424988 ? 0xF : 0xE;
    sound = BattleAudienceSoundGetPtr(soundKind);
    BattleAudienceSoundStop(soundKind);

    choice = irand(3);
    switch (choice) {
        case 0:
            *(s32*)((s32)sound + 4) = BattleAudienceSound2(
                str_SFX_AUDIENCE_SLEEP1_802fa3d8, kind);
            break;
        case 1:
            *(s32*)((s32)sound + 4) = BattleAudienceSound2(
                str_SFX_AUDIENCE_SLEEP2_802fa3ec, kind);
            break;
        default:
            *(s32*)((s32)sound + 4) = BattleAudienceSound2(
                str_SFX_AUDIENCE_SLEEP3_802fa400, kind);
            break;
    }

    if (*(s32*)((s32)sound + 4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
    return 0;
}

void BattleAudienceSoundZZZ(s32 kind) {
    void* sound;

    sound = BattleAudienceSoundGetPtr(0x10);
    BattleAudienceSoundStop(0x10);

    BattleAudienceSound2(str_SFX_AUDIENCE_ZZZ1_802fa3c4, kind);

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
}

void BattleAudienceSoundSing(s32 kind) {
    void* sound;

    sound = BattleAudienceSoundGetPtr(0x11);
    BattleAudienceSoundStop(0x11);

    *(s32*)((s32)sound + 0x4) = BattleAudienceSound2(
        str_SFX_AUDIENCE_PANSY_S_802fa3a8,
        kind
    );

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
}

void BattleAudienceSoundShell(s32 kind) {
    f32 pos[3];
    s32 soundId;
    void* sound;

    BattleAudienceSoundGetInfo2(kind, pos);

    if (pos[0] <= float_0_80424988) {
        soundId = 0x12;
    } else {
        soundId = 0x13;
    }

    sound = BattleAudienceSoundGetPtr(soundId);
    BattleAudienceSoundStop(soundId);

    *(s32*)((s32)sound + 0x4) = BattleAudienceSound2(
        str_SFX_AUDIENCE_SHELL_T_802fa388,
        kind
    );

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
}

void BattleAudienceSoundBombIgnite(s32 kind, s32 start, s32 frames) {
    void* sound;

    sound = BattleAudienceSoundGetPtr(0x14);
    BattleAudienceSoundStop(0x14);

    *(s32*)((s32)sound + 0x4) = BattleAudienceSound2(
        str_SFX_AUDIENCE_BOMB_RE_802fa36c,
        kind
    );

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, start + frames, frames);
    }
}

void BattleAudienceSoundBombFire(s32 kind) {
    f32 pos[3];
    s32 soundId;
    void* sound;

    BattleAudienceSoundGetInfo2(kind, pos);

    if (pos[0] <= float_0_80424988) {
        soundId = 0x15;
    } else {
        soundId = 0x16;
    }

    sound = BattleAudienceSoundGetPtr(soundId);
    BattleAudienceSoundStop(soundId);

    *(s32*)((s32)sound + 0x4) = BattleAudienceSound2(
        str_SFX_AUDIENCE_BOMB_BU_802fa350,
        kind
    );

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
}

    void BattleAudienceSoundPakkunEat(s32 kind) {
    void* sound;

    sound = BattleAudienceSoundGetPtr(0x17);
    BattleAudienceSoundStop(0x17);

    *(s32*)((s32)sound + 0x4) = BattleAudienceSound2(
        str_SFX_AUDIENCE_PAKU_EA_802fa338,
        kind
    );

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
}

void BattleAudienceSoundSetVol(s32 id, s32 vol, s32 frames) {
    void* sound;
    s32 zero;

    sound = BattleAudienceSoundGetPtr(id);
    zero = 0;

    *(u8*)((s32)sound + 0x1F) = *(u8*)((s32)sound + 0x1E);
    *(u8*)((s32)sound + 0x20) = vol;
    *(s32*)((s32)sound + 0x14) = zero;
    *(s32*)((s32)sound + 0x18) = frames;
    *(s32*)((s32)sound + 0x10) = zero;
}

void BattleAudienceSoundSetVolAll(s32 vol, s32 frames) {
    s32 i;

    for (i = 0; i < 0x18; i++) {
        BattleAudienceSoundSetVol(i, vol, frames);
    }
}

s32 BattleAudienceSoundCheck(s32 id) {
    void* sound;

    sound = BattleAudienceSoundGetPtr(id);
    return *(u32*)sound & 1;
}

void BattleAudienceSoundStop(s32 id) {
    void* sound;

    sound = BattleAudienceSoundGetPtr(id);

    if ((*(u32*)sound & 1) != 0) {
        psndSFXOff(*(s32*)((s32)sound + 0x4));
        *(u32*)sound &= ~1;
    }
}
void BattleAudienceSound0(const char* name) {
    psndSFXOn(name);
}

s32 BattleAudienceSound1(const char* name, s32 kind, s32 arg) {
    u8 vol;
    f32 pos[3];
    s32 id;

    BattleAudienceSoundGetInfo1(kind, arg, &vol, pos);

    id = psndSFXOn_3D(name, pos);
    psndSFX_vol(id, vol);

    return id;
}

void BattleAudienceSoundGetInfo1(s32 right, s32 left, u8* vol, void* pos) {
    s32 total;
    s32 temp;
    f32* out;

    out = (f32*)pos;
    total = right + left;

    if (total == 0) {
        *vol = 0;
    } else {
        *vol = (s32)(float_100_804249bc + ((float_26_804249c0 * total) / float_200_804249b8));
    }

    if (left == 0 || total == 0) {
        out[0] = float_250_804249c4;
    } else {
        temp = (s32)((float_250_804249c4 * (right - left)) / total);
        out[0] = (u8)temp;
    }

    out[1] = float_0_80424988;
    out[2] = float_0_80424988;
}

s32 BattleAudienceSound2(const char* name, s32 kind) {
    f32 pos[3];

    BattleAudienceSoundGetInfo2(kind, pos);
    return psndSFXOn_3D(name, pos);
}

void BattleAudienceSoundGetInfo2(s32 id, void* pos) {
    void* audience;
    f32* out;

    out = (f32*)pos;
    audience = BattleAudienceGetPtr(id);

    out[0] = *(f32*)((s32)audience + 0x48);
    out[1] = *(f32*)((s32)audience + 0x4C);
    out[2] = *(f32*)((s32)audience + 0x50);
}

u8 BattleAudienceSoundMain(void) {
    extern void* BattleAudienceBaseGetPtr(void);
    extern void* BattleAudienceSoundGetPtr(s32 id);
    extern void BattleAudienceNoiseMain(void);
    extern s32 psndSFXChk(s32 sfxId);
    extern f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);
    u8* sound;
    s32 i;
    s32 value;

    BattleAudienceBaseGetPtr();
    BattleAudienceNoiseMain();

    for (i = 0; i < 0x18; i++) {
        sound = BattleAudienceSoundGetPtr(i);
        if (psndSFXChk(*(s32*)(sound + 4)) == -1) {
            *(u32*)sound &= ~1;
        } else if (*(s32*)(sound + 0xC) == 0) {
            psndSFXOff(*(s32*)(sound + 4));
            *(u32*)sound &= ~1;
        } else {
            if (*(s32*)(sound + 0x18) > 0) {
                *(s32*)(sound + 0x14) = *(s32*)(sound + 0x14) + 1;
                value = (s32)intplGetValue(0, *(s32*)(sound + 0x14), (f32)sound[0x1F], (f32)sound[0x20], *(s32*)(sound + 0x18));
                sound[0x1E] = value;
                psndSFX_vol(*(s32*)(sound + 4), (u8)((sound[0x1D] * sound[0x1E]) / 100));
                if (*(s32*)(sound + 0x14) >= *(s32*)(sound + 0x18)) {
                    *(s32*)(sound + 0x18) = 0;
                }
            }

            if (*(s32*)(sound + 0xC) > 0) {
                if (*(s32*)(sound + 0xC) == *(s32*)(sound + 0x10)) {
                    sound[0x1F] = sound[0x1E];
                    sound[0x20] = 0;
                }
                if (*(s32*)(sound + 0xC) <= *(s32*)(sound + 0x10)) {
                    value = (s32)intplGetValue(0, *(s32*)(sound + 0x10) - *(s32*)(sound + 0xC), (f32)sound[0x1F], (f32)sound[0x20], *(s32*)(sound + 0x10));
                    sound[0x1E] = value;
                    psndSFX_vol(*(s32*)(sound + 4), (u8)((sound[0x1D] * sound[0x1E]) / 100));
                }
                *(s32*)(sound + 0xC) = *(s32*)(sound + 0xC) - 1;
            }
        }
    }
}

u8 BattleAudienceNoiseMain(void) {
    extern void* BattleAudienceBaseGetPtr(void);
    extern void* BattleAudienceSoundGetPtr(s32 id);
    extern s32 BattleAudience_GetPPAudienceNum(void);
    extern void BattleAudienceSoundSetVol(s32 id, s32 vol, s32 frames);
    extern void BattleAudienceSoundNoiseAlways(void);
    extern f32 float_63_804249b4;
    void* battleWork;
    u8* base;
    u8* sound;
    s32 num;
    s32 vol;

    battleWork = _battleWorkPointer;
    base = BattleAudienceBaseGetPtr();
    sound = BattleAudienceSoundGetPtr(2);
    num = BattleAudience_GetPPAudienceNum();

    if ((*(u32*)base & 0x10000) == 0 && (*(u32*)((s32)battleWork + 0xEF4) & 0x80) != 0) {
        BattleAudienceSoundSetVol(2, 0x3C, 0x50);
    }
    if ((*(u32*)base & 0x10000) != 0 && (*(u32*)((s32)battleWork + 0xEF4) & 0x80) == 0) {
        BattleAudienceSoundSetVol(2, 0, 0x50);
    }

    if (num == 0 && sound[8] != 0) {
        psndSFXOff(*(s32*)(sound + 4));
        sound[8] = 0;
    }
    if (num >= 1 && num <= 0x31 && sound[8] != 1) {
        BattleAudienceSoundNoiseAlways();
        sound[8] = 1;
    }
    if (num >= 0x32 && num <= 0x63 && sound[8] != 2) {
        BattleAudienceSoundNoiseAlways();
        sound[8] = 2;
    }
    if (num >= 0x64 && num <= 0x95 && sound[8] != 3) {
        BattleAudienceSoundNoiseAlways();
        sound[8] = 3;
    }
    if (num >= 0x96 && num <= 0xC8 && sound[8] != 4) {
        BattleAudienceSoundNoiseAlways();
        sound[8] = 4;
    }

    if ((*(u32*)base & 0x2000) == 0) {
        sound[0x1D] = 0;
    } else if (num == 0) {
        sound[0x1D] = 0;
    } else {
        vol = (s32)(float_63_804249b4 + (float_63_804249b4 * (f32)num) / float_200_804249b8);
        sound[0x1D] = vol;
    }
}

void BattleAudience_Case_KillEnemy(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= 0x1000;
}

void BattleAudience_Case_ActionCommandGood(void) {
    void* base;
    s32 i;
    s32 count;

    base = BattleAudienceBaseGetPtr();

    count = 0;
    i = 0;

    while (i < 0xC8) {
        if ((u8)BattleAudience_GetWaiting(i) == 1) {
            count++;
        }

        i++;
    }

    if (count != 0) {
        BattleAudienceSoundCheer(0xB4, 0x78);
        BattleAudienceJoy(0);
    }

    *(u32*)base |= 0x10;
}

void BattleAudience_Case_ActionCommandBad(void* unit) {
    void* base;
    s32 count;
    s32 i;

    base = BattleAudienceBaseGetPtr();

    count = 0;
    i = 0;

    while (i < 0xC8) {
        if ((u8)BattleAudience_GetWaiting(i) == 1) {
            count++;
        }

        i++;
    }

    if (count != 0) {
        BattleAudienceSoundBooing();
        BattleAudienceJoy(0);

        if (*(void**)((s32)unit + 0x1C) == weaponGetPowerFromMarioHammerLv) {
            BattleAudienceAddPhaseEvtList(7);
        }
    }

    *(u32*)base |= 0x20;
}

void BattleAudience_Case_GuardGood(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= 0x40;
}

void BattleAudience_Case_GuardBad(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= 0x80;
}

void BattleAudience_Case_AcrobatGood(void) {
    void* base;
    void* work;
    s32 count;
    s32 i;

    base = BattleAudienceBaseGetPtr();

    count = 0;
    i = 0;

    while (i < 0xC8) {
        if ((u8)BattleAudience_GetWaiting(i) == 1) {
            count++;
        }

        i++;
    }

    if (count >= 1) {
        BattleAudienceSoundCheer(0xB4, 0x78);
        BattleAudienceSoundClap(0xB4, 0x78);
        BattleAudienceJoy(1);
    }

    work = (void*)((s32)base + 0x10000);

    *(s32*)((s32)work + 0x37D4) += 1;

    if (*(s32*)((s32)work + 0x37D4) > 5) {
        *(s32*)((s32)work + 0x37D4) = 5;
    }

    *(u32*)base |= 0x100;
}

void BattleAudience_Case_AcrobatBad(void) {
    void* base;
    s32 count;
    s32 i;

    base = BattleAudienceBaseGetPtr();

    count = 0;
    i = 0;

    while (i < 0xC8) {
        if ((u8)BattleAudience_GetWaiting(i) == 1) {
            count++;
        }

        i++;
    }

    if (count >= 1) {
        BattleAudienceSoundBooing();
        BattleAudienceJoy(0);
    }

    *(u32*)base |= 0x200;
}
void BattleAudience_Case_AcrobatNoTry(void) {
    BattleAudienceBaseGetPtr();
}

void BattleAudience_Case_MarioBigDamage(void) {
    BattleAudienceBaseGetPtr();
}

void BattleAudience_Case_MarioPinch(void) {
    void* base;
    f32 scale;
    f32 value;

    base = BattleAudienceBaseGetPtr();
    scale = float_0p05_80424994;
    value = *(f32*)((s32)base + 0x13778);
    value = scale * value;
    BattleAudienceAddTargetNum(value, float_0_80424988);
}

void BattleAudience_Case_MarioDanger(void) {
    void* base;
    f32 scale;
    f32 value;

    base = BattleAudienceBaseGetPtr();
    scale = float_0p15_804249a0;
    value = *(f32*)((s32)base + 0x13778);
    value = scale * value;
    BattleAudienceAddTargetNum(value, float_0_80424988);
}

void BattleAudience_Case_PartyDown(void) {
    void* base;
    f32 scale;
    f32 value;

    base = BattleAudienceBaseGetPtr();
    scale = float_neg0p25_804249b0;
    value = *(f32*)((s32)base + 0x13778);
    value = scale * value;
    BattleAudienceAddTargetNum(value, float_0_80424988);
}

void BattleAudience_Case_JumpNewRecord(void) {
    ;
}

void BattleAudience_Case_FinalAttack(void) {
    ;
}

void BattleAudience_Case_TurnEnd(void) {
    extern void BattleAudienceNumToTarget(void);
    extern void* BattleGetMarioPtr(void* battleWork);
    void* battle;
    void* mario;
    void* base;
    s32 threshold;
    s32 chance;

    battle = _battleWorkPointer;
    mario = BattleGetMarioPtr(battle);
    base = BattleAudienceBaseGetPtr();
    pouchGetPtr();

    if ((*(u32*)base & 0x40000) == 0) {
        chance = *(s32*)((s32)base + 0x137D8) + 3;
        if (chance > 100) {
            chance = 100;
        }
        *(s32*)((s32)base + 0x137D8) = chance;

        threshold = 5;
        if (*(u8*)((s32)mario + 0xD) != 0) {
            threshold = 10 / *(u8*)((s32)mario + 0xD);
            if (threshold < 5) {
                threshold = 5;
            }
        }

        if (threshold <= *(s16*)battle) {
            BattleAudienceAddTargetNum(
                float_neg0p05_80424984 * *(f32*)((s32)base + 0x13778),
                float_0_80424988);
        }
        if (*(s16*)battle == threshold) {
            BattleAudienceAddPhaseEvtList(6);
        }
        if (irand(100) < chance) {
            if (threshold <= *(s16*)battle) {
                BattleAudienceAddPhaseEvtList(1);
            }
            BattleAudienceAddPhaseEvtList(4);
            BattleAudienceAddPhaseEvtList(9);
            BattleAudienceAddPhaseEvtList(10);
            BattleAudienceAddPhaseEvtList(11);
            BattleAudienceAddPhaseEvtList(12);
            BattleAudienceAddPhaseEvtList(13);
            *(s32*)((s32)base + 0x137D8) = 0;
        }
        BattleAudienceNumToTarget();
    }
}

void BattleAudience_Case_Appeal(void* unit) {
    extern const f32 float_0p25_804249ac;
    void* base;
    s32 audienceCount;
    u8 superAppeal;

    base = BattleAudienceBaseGetPtr();
    audienceCount = BattleAudience_GetPPAudienceNum();
    if (audienceCount > 0) {
        *(s32*)((s32)base + 0x137C4) +=
            (s32)(audienceCount * float_0p25_804249ac) + 0x19;

        superAppeal = *(u8*)((s32)unit + 0x2F3);
        if (superAppeal != 0) {
            *(s32*)((s32)base + 0x137C4) += superAppeal * 0x19;
        }

        BattleAudienceSoundCheer(0xB4, 0x3C);
        BattleAudienceSoundClap(0xB4, 0x3C);
        BattleAudienceJoy(0);
    }

    base = BattleAudienceBaseGetPtr();
    BattleAudienceAddTargetNum(
        float_0p05_80424994 * *(f32*)((s32)base + 0x13778),
        float_0_80424988);
}

void BattleAudience_Case_Escape(void) {
    ;
}

void BattleAudience_Case_Escape_Good(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    BattleAudienceAddTargetNum(
        float_neg0p1_804249a4 * *(f32*)((s32)base + 0x13778),
        float_0_80424988
    );

    BattleAudienceSoundSetVol(2, 0x64, 0x1E);
    BattleAudienceSoundBooing();
}

void BattleAudience_Case_Escape_Bad(void) {
    void* base;
    f32 scale;
    f32 value;

    base = BattleAudienceBaseGetPtr();
    scale = float_neg0p2_804249a8;
    value = *(f32*)((s32)base + 0x13778);
    value = scale * value;
    BattleAudienceAddTargetNum(value, float_0_80424988);

    BattleAudienceSoundBooing();
}

void BattleAudience_Case_EnemyDamage(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= 0x400;
}
void BattleAudience_Case_EnemyNoDamage(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= 0x800;
}

void BattleAudience_Case_Countered(void) {
    void* base;
    f32 scale;
    f32 value;

    base = BattleAudienceBaseGetPtr();
    scale = float_neg0p1_804249a4;
    value = *(f32*)((s32)base + 0x13778);
    value = scale * value;
    BattleAudienceAddTargetNum(value, float_0_80424988);

    BattleAudienceAddPhaseEvtList(5);
}

void BattleAudience_Case_HaikeiSet(void) {
    void* base;
    f32 scale;
    f32 value;
    f32 zero;

    base = BattleAudienceBaseGetPtr();

    scale = float_0p05_80424994;
    value = *(f32*)((s32)base + 0x13778);
    zero = float_0_80424988;

    value = scale * value;

    BattleAudienceAddTargetNum(value, zero);
}

void BattleAudience_Case_GreatVictory(void) {
    void* base;
    f32 scale;
    f32 value;

    base = BattleAudienceBaseGetPtr();
    scale = float_0p15_804249a0;
    value = *(f32*)((s32)base + 0x13778);
    value = scale * value;
    BattleAudienceAddTargetNum(value, float_0_80424988);
}

void BattleAudience_Case_FastVictory(f32 value) {
    void* base;
    f32 temp;

    base = BattleAudienceBaseGetPtr();

    temp = float_5_8042499c * value;
    temp = float_0p01_80424998 * temp;

    BattleAudienceAddTargetNum(
        temp * *(f32*)((s32)base + 0x13778),
        float_0_80424988
    );
}

void BattleAudience_Case_FallObject_Stage(void) {
    void* base;
    f32 scale;
    f32 value;

    base = BattleAudienceBaseGetPtr();
    scale = float_0p05_80424994;
    value = *(f32*)((s32)base + 0x13778);
    value = scale * value;
    BattleAudienceAddTargetNum(value, float_0_80424988);
}

void BattleAudience_Case_FallObject_Aud(int memberIdx, u8 objectType) {
    static const s32 nearOffsets[8][2] = {
        {-1, -1}, {0, -1}, {1, -1}, {-1, 0},
        {1, 0}, {-1, 1}, {0, 1}, {1, 1}
    };
    static const s32 wideOffsets[24][2] = {
        {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2},
        {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1},
        {-2, 0}, {-1, 0}, {1, 0}, {2, 0},
        {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1},
        {-2, 2}, {-1, 2}, {0, 2}, {1, 2}, {2, 2}
    };
    void* base;
    void* member;
    const s32 (*offsets)[2];
    s32 count;
    s32 other;
    s32 i;

    base = BattleAudienceBaseGetPtr();

    if (BattleAudience_GetSysCtrl(memberIdx) == 1) {
        member = BattleAudienceGetPtr(memberIdx);
        if (*(u8*)((s32)member + 0x19) != 0xF) {
            BattleAudience_ChangeStatus(memberIdx, 10);
        }
    }

    if (objectType == 6) {
        for (i = 0; i < 200; i++) {
            if (BattleAudience_GetEscapeChangeOK(i) != 0) {
                BattleAudience_ChangeStatus(i, 0xC);
                *(f32*)((s32)base + 0x1377C) -= float_1_80424990;
            }
        }
    } else if (objectType == 2) {
        *(f32*)((s32)base + 0x1377C) -= float_1_80424990;
    } else {
        if (objectType == 7) {
            offsets = wideOffsets;
            count = 24;
        } else {
            offsets = nearOffsets;
            count = 8;
        }

        for (i = 0; i < count; i++) {
            other = BattleAudience_GetAudienceNoFromOffset(
                memberIdx, offsets[i][0], offsets[i][1]);
            if (other != -1 && BattleAudience_GetEscapeChangeOK(other) != 0) {
                BattleAudience_ChangeStatus(other, 0xC);
                *(f32*)((s32)base + 0x1377C) -= float_1_80424990;
            }
        }
    }

    BattleAudiencePuniAllEscape();
}

void BattleAudience_Case_FirstAttack_Good(void) {
    void* base;
    f32 scale;
    f32 value;

    base = BattleAudienceBaseGetPtr();
    scale = float_0p02_8042498c;
    value = *(f32*)((s32)base + 0x13778);
    value = scale * value;
    BattleAudienceAddTargetNum(value, float_0_80424988);
}
void BattleAudience_Case_FirstAttack_Bad(void) {
    void* base;
    f32 scale;
    f32 value;

    base = BattleAudienceBaseGetPtr();
    scale = float_neg0p05_80424984;
    value = *(f32*)((s32)base + 0x13778);
    value = scale * value;
    BattleAudienceAddTargetNum(value, float_0_80424988);
}

void BattleAudience_SetPresentItemNo(s32 itemNo) {
    *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137E4) = itemNo;
}
s32 BattleAudience_GetPresentItemNo(void) {
    return *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137E4);
}

void BattleAudience_SetPresentTargetUnitId(s32 unitId) {
    *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137EC) = unitId;
}

s32 BattleAudience_GetPresentTargetUnitId(void) {
    return *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137EC);
}

void BattleAudience_SetPresentItemType(s32 itemType) {
    *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137E8) = itemType;
}

s32 BattleAudience_GetPresentItemType(void) {
    return *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137E8);
}
