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

void BattleAudience_Case_Escape(void) {
}

void BattleAudience_Case_FinalAttack(void) {
}

void BattleAudience_Case_JumpNewRecord(void) {
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

void* BattleAudienceSoundGetPtr(s32 id) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    return (void*)((s32)base + id * 0x24 + 0x133FC);
}

void* BattleAudienceWinGetPtr(void) {
    return (void*)((s32)BattleAudienceBaseGetPtr() + 0x1375C);
}
void BattleAudienceSound0(const char* name) {
    psndSFXOn(name);
}

s32 BattleAudienceSoundCheck(s32 id) {
    void* sound;

    sound = BattleAudienceSoundGetPtr(id);
    return *(u32*)sound & 1;
}

void BattleAudienceSoundWhistle(void) {
    BattleAudienceSoundWhistleKind(irand(3) + 1);
}
void BattleAudience_Case_AcrobatNoTry(void) {
    BattleAudienceBaseGetPtr();
}

void BattleAudience_Case_MarioBigDamage(void) {
    BattleAudienceBaseGetPtr();
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

s32 BattleAudience_GetPPAudienceNum_L(void) {
    return BattleAudience_GetPPAudienceNum_RL_Sub(0);
}

s32 BattleAudience_GetPPAudienceNum_R(void) {
    return BattleAudience_GetPPAudienceNum_RL_Sub(10);
}

s32 BattleAudience_GetPPAudienceNum(void) {
    s32 left;
    s32 right;

    left = BattleAudience_GetPPAudienceNum_L();
    right = BattleAudience_GetPPAudienceNum_R();

    return right + left;
}

s32 BattleAudience_GetAudienceNum(void) {
    return *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x13784);
}
s32 BattleAudience_GetPresentItemNo(void) {
    return *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137E4);
}

s32 BattleAudience_GetPresentItemType(void) {
    return *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137E8);
}

s32 BattleAudience_GetPresentTargetUnitId(void) {
    return *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137EC);
}

void BattleAudience_SetPresentItemNo(s32 itemNo) {
    *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137E4) = itemNo;
}

void BattleAudience_SetPresentItemType(s32 itemType) {
    *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137E8) = itemType;
}

void BattleAudience_SetPresentTargetUnitId(s32 unitId) {
    *(s32*)((s32)BattleAudienceBaseGetPtr() + 0x137EC) = unitId;
}

void BattleAudience_WinSetActive(s32 active) {
    void* win = BattleAudienceWinGetPtr();
    *(u8*)win = active;
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

s32 BattleAudienceSound2(const char* name, s32 kind) {
    f32 pos[3];

    BattleAudienceSoundGetInfo2(kind, pos);
    return psndSFXOn_3D(name, pos);
}

void BattleAudienceSoundStop(s32 id) {
    void* sound;

    sound = BattleAudienceSoundGetPtr(id);

    if ((*(u32*)sound & 1) != 0) {
        psndSFXOff(*(s32*)((s32)sound + 0x4));
        *(u32*)sound &= ~1;
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
void BattleAudience_Case_EnemyNoDamage(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= 0x800;
}

void BattleAudience_Case_EnemyDamage(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= 0x400;
}

void BattleAudience_Case_GuardBad(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= 0x80;
}

void BattleAudience_Case_GuardGood(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= 0x40;
}

void BattleAudience_Case_KillEnemy(void) {
    void* base;

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= 0x1000;
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

void BattleAudienceJoyEnding(void) {
    BattleAudienceJoy(3);
}

void BattleAudience_Attack(s32 id) {
    void* audience;

    audience = BattleAudienceGetPtr(id);
    *(u32*)audience |= 0x40;
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

void BattleAudience_SetRotateOffset(s32 id, f32 x, f32 y, f32 z) {
    BattleAudienceWorkPartial* audience;

    audience = BattleAudienceGetPtr(id);

    audience->rotOffsetX = x;
    audience->rotOffsetY = y;
    audience->rotOffsetZ = z;
}

void BattleAudience_SetRotate(s32 id, f32 x, f32 y, f32 z) {
    BattleAudienceWorkPartial* audience;

    audience = BattleAudienceGetPtr(id);

    audience->rotX = x;
    audience->rotY = y;
    audience->rotZ = z;
}

void BattleAudience_SetPosition(s32 id, f32 x, f32 y, f32 z) {
    BattleAudienceWorkPartial* audience;

    audience = BattleAudienceGetPtr(id);

    audience->posX = x;
    audience->posY = y;
    audience->posZ = z;
}

void BattleAudience_GetRotate(s32 id, f32* x, f32* y, f32* z) {
    void* audience;

    audience = BattleAudienceGetPtr(id);

    *x = *(f32*)((s32)audience + 0x9C);
    *y = *(f32*)((s32)audience + 0xA0);
    *z = *(f32*)((s32)audience + 0xA4);
}

void BattleAudience_GetHomePosition(s32 id, f32* x, f32* y, f32* z) {
    void* audience;

    audience = BattleAudienceGetPtr(id);

    *x = *(f32*)((s32)audience + 0xB4);
    *y = *(f32*)((s32)audience + 0xB8);
    *z = *(f32*)((s32)audience + 0xBC);
}

void BattleAudience_GetPosition(s32 id, f32* x, f32* y, f32* z) {
    void* audience;

    audience = BattleAudienceGetPtr(id);

    *x = *(f32*)((s32)audience + 0x48);
    *y = *(f32*)((s32)audience + 0x4C);
    *z = *(f32*)((s32)audience + 0x50);
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

void BattleAudience_Delete(s32 id) {
    void* audience;

    BattleAudienceBaseGetPtr();

    audience = BattleAudienceGetPtr(id);

    if ((*(u32*)audience & 0x80) != 0) {
        *(s32*)((s32)audience + 0x134) = 0;
    }

    *(s32*)audience = 0;
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

void BattleAudienceSoundZZZ(s32 kind) {
    void* sound;

    sound = BattleAudienceSoundGetPtr(0x10);
    BattleAudienceSoundStop(0x10);

    BattleAudienceSound2(str_SFX_AUDIENCE_ZZZ1_802fa3c4, kind);

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, -1, 0);
    }
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

void BattleAudienceSoundGetInfo2(s32 id, void* pos) {
    void* audience;
    f32* out;

    out = (f32*)pos;
    audience = BattleAudienceGetPtr(id);

    out[0] = *(f32*)((s32)audience + 0x48);
    out[1] = *(f32*)((s32)audience + 0x4C);
    out[2] = *(f32*)((s32)audience + 0x50);
}

void BattleAudienceSoundGetInfo1(s32 right, s32 left, u8* vol, void* pos) {
    s32 total;
    s32 temp;
    f32* out;

    out = (f32*)pos;
    total = right + left;

    if (total != 0) {
        *vol = (s32)(float_100_804249bc + ((float_26_804249c0 * total) / float_200_804249b8));
    } else {
        *vol = 0;
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

void BattleAudienceSoundCheerKind(s32 kind, s32 vol, s32 frames) {
    char name[0x48];
    void* sound4;
    void* sound5;
    void* sound;
    s32 usePitch;
    s32 right;
    s32 left;
    const char* baseName;

    baseName = str_btl_msg_audience_pun_802f9e28;
    BattleAudienceBaseGetPtr();

    usePitch = 0;

    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();

    sound4 = BattleAudienceSoundGetPtr(4);
    sound5 = BattleAudienceSoundGetPtr(5);
    sound = sound4;

    if ((*(u32*)sound4 & 1) != 0) {
    if ((*(u32*)sound5 & 1) != 0) {
        if (*(s32*)((s32)sound4 + 0xC) <= *(s32*)((s32)sound5 + 0xC)) {
            BattleAudienceSoundStop(4);
            sound = BattleAudienceSoundGetPtr(4);
        } else {
            BattleAudienceSoundStop(5);
            sound = BattleAudienceSoundGetPtr(5);
        }
    } else {
        sound = BattleAudienceSoundGetPtr(5);
        usePitch = 1;
    }
    } else {
    if ((*(u32*)sound5 & 1) != 0) {
        sound = BattleAudienceSoundGetPtr(4);
        usePitch = 1;
    } else {
        sound = BattleAudienceSoundGetPtr(4);
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

    *(s32*)((s32)sound + 0x4) = BattleAudienceSound1(name, right, left);

    if (usePitch == 1) {
    *(s32*)((s32)sound + 0x4) = BattleAudienceSound1(name, right, left);
    psndSFX_pit(*(s32*)((s32)sound + 0x4), 0x7F);
        } else {
    *(s32*)((s32)sound + 0x4) = BattleAudienceSound1(name, right, left);
    }

    if (*(s32*)((s32)sound + 0x4) != -1) {
        BA_SOUND_INIT(sound, vol + frames, frames);
    }
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


s32 BattleAudience_CheckReactionPerPhase(void) {
    return 0;
}


u8 BattleAudienceSettingAudience(void) {
    return 0;
}


u8 BattleAudienceItemCtrlProcess(void) {
    return 0;
}


u8 BattleAudienceDispAudience(void) {
    return 0;
}


u8 BattleAudienceApSrcCtrlProcess(void) {
    return 0;
}


u8 BattleAudience_PerAct(void) {
    return 0;
}


u8 BattleAudienceCtrlProcess(void) {
    return 0;
}


void BattleAudience_Case_FallObject_Aud(int memberIdx, u8 objectType) {
}


void BattleAudience_ApRecoveryBuild(void* apInfo) {
}


s32 BattleAudienceApSrcEntry(void) {
    return 0;
}


s32 unk_801a21e0(void) {
    return 0;
}


s32 unk_801a23e0(void) {
    return 0;
}


u8 BattleAudienceSoundMain(void) {
    return 0;
}


u8 BattleAudienceAnimProcess(void) {
    return 0;
}


u8 BattleAudienceWinCtrlProcess(void) {
    return 0;
}


s32 BattleAudience_Entry(s32 id, s32 type, s32 arg) {
    return 0;
}


u8 BattleAudienceNoiseMain(void) {
    return 0;
}


void BattleAudienceAddAudienceNum(s32 num) {
}


void BattleAudience_Init(void) {
}


u8 BattleAudienceAddTargetNumSub(void) {
    return 0;
}


void BattleAudienceJoy(s32 kind) {
}


void BattleAudienceAddTargetNum(f32 value, f32 unused) {
}


void BattleAudienceSoundWhistleKind(s32 kind) {
}


void BattleAudience_Case_TurnEnd(void) {
}


u8 BattleAudienceGXInit(void) {
    return 0;
}


u8 BattleAudienceSoundNoiseAlways(void) {
    return 0;
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


u8 BattleAudienceDispApSrc(void) {
    return 0;
}


u8 BattleAudienceSoundSleep(int param_1) {
    return 0;
}


u8 BattleAudienceJoy_Sub(int param_1, int param_2, int param_3, int param_4) {
    return 0;
}


void BattleAudience_PerPhase(int phaseId) {
}


u8 BattleAudienceDispItem(void) {
    return 0;
}


void BattleAudience_End(void) {
}


void BattleAudience_ChangeStatus(s32 id, s32 status) {
}


s32 BattleAudience_GetEscapeChangeOK(s32 id) {
    return 0;
}


u8 BattleAudienceDispWin(void) {
    return 0;
}


u8 BattleAudienceSoundItemThrow(int param_1, int param_2) {
    return 0;
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


s32 BattleAudienceDetectTargetAll(void) {
    return 0;
}


s32 BattleAudienceDetectTargetPlayer(void) {
    return 0;
}


u8 BattleAudience_Disp(void) {
    return 0;
}


u8 BattleAudienceSoundRun(int param_1) {
    return 0;
}


u32 check_exe_phase_evt_status(int param_1, u32 param_2) {
    return 0;
}


void BattleAudience_SetAnim(s32 id, s32 anim, s32 pose) {
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


u8 BattleAudience_Entry_Sub(u32 memberIdx, u32 audienceKind, u8 param_3) {
    return 0;
}


s32 BattleAudienceItemOn(u32 memberIdx, u16 itemType, int numberItems) {
    return 0;
}


void BattleAudience_GetItemOn(int* outMemberIdx, float* outXPos, float* outYPos, float* outZPos, u32* outItemType) {
}


void BattleAudience_Case_Appeal(void* unit) {
}


void BattleAudience_GetItemOn2(int* outMemberIdx, float* outPosX, float* outPosY, float* outPosZ, u32* outItemType) {
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


u8 BattleAudience_SetTarget(int param_1) {
    return 0;
}


u32 BattleAudience_GetFront(int param_1) {
    return 0;
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


int BattleAudience_HaitiRandForFallObject(void) {
    return 0;
}


void BattleAudience_Main(void) {
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


void BattleAudienceGuestTPLRead(int index, u32 memberKind, char* tplName) {
}


void tplRead(char* tplName) {
}


void BattleAudienceJoySACLecture(void) {
    BattleAudienceSoundCheer(0x78, 0x3C);
    BattleAudienceJoy(2);
}
