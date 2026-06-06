#include "action/star/sac_common.h"

extern s32 cheerEnable;
extern s32 handBeatEnable;
extern void* _battleWorkPointer;
extern s32 enemy_slide_return_evt[];
extern s32 enemy_slide_go_evt[];

void BattleAcHelpSetDispType(s32 type);
void* BattleGetUnitPtr(void* battleWork, s32 unitId);
void* evtEntry(void* script, s32 type, s32 flags);
s32 BattleAudience_GetAudienceNum(void);
void BattleAudienceSoundClap(s32 left, s32 right);
void BattleAudienceSoundCheerKind(s32 kind, s32 left, s32 right);
void BattleAudienceSoundWhistleKind(s32 kind);

s32 sac_cheer_end(void) {
    cheerEnable = 0;
    return 2;
}

s32 sac_handbeat_end(void) {
    handBeatEnable = 0;
    return 2;
}

s32 sac_ac_help_off(void) {
    BattleAcHelpSetDispType(0);
    return 2;
}

s32 sac_ac_help_on(void) {
    BattleAcHelpSetDispType(2);
    return 2;
}

s32 sac_enemy_slide_return(void) {
    s32 i;
    void* battleWork;
    void* script;

    script = enemy_slide_return_evt;
    i = 0;
    battleWork = _battleWorkPointer;
    for (; i < 0x40; i++) {
        void* unit = BattleGetUnitPtr(battleWork, i);

        if (unit != NULL && *(s8*)((s32)unit + 0xC) == 1) {
            void* evt = evtEntry(script, 0, 0x20);

            *(s32*)((s32)evt + 0x160) = i;
        }
    }
    return 2;
}

s32 sac_enemy_slide_go(void) {
    s32 i;
    void* battleWork;
    void* script;

    script = enemy_slide_go_evt;
    i = 0;
    battleWork = _battleWorkPointer;
    for (; i < 0x40; i++) {
        void* unit = BattleGetUnitPtr(battleWork, i);

        if (unit != NULL && *(s8*)((s32)unit + 0xC) == 1) {
            void* evt = evtEntry(script, 0, 0x20);

            *(s32*)((s32)evt + 0x160) = i;
        }
    }
    return 2;
}

s32 sac_wao(void) {
    s32 audience = BattleAudience_GetAudienceNum();

    BattleAudienceSoundClap(0x78, 0x78);
    if (audience >= 1 && audience <= 0x95) {
        BattleAudienceSoundCheerKind(1, 0x78, 0x78);
    }
    if (audience >= 0x96) {
        BattleAudienceSoundCheerKind(4, 0x78, 0x78);
        BattleAudienceSoundWhistleKind(1);
    }
    return 2;
}

s32 sac_cheer(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 target);
    extern s32 BattleAudienceSoundCheck(s32 id);
    extern void BattleAudienceSoundStop(s32 id);
    extern void BattleAudienceSoundCheer(s32 kind, s32 flags);
    extern void BattleAudienceSoundSetVol(s32 id, s32 volume, s32 flags);
    extern void* BattleAudienceSoundGetPtr(s32 id);
    extern s32 irand(s32 max);
    extern s32 count;
    extern s32 count2;
    extern u8 volPer;

    s32 value;
    s32 newVol;
    void* sound;

    value = evtGetValue(event, *(s32*)*(void**)((s32)event + 0x18));
    if (isFirstCall != 0) {
        count = 0;
        count2 = 0x3C;
        cheerEnable = 1;
        volPer = 0;
        if (BattleAudienceSoundCheck(4) == 1) {
            BattleAudienceSoundStop(4);
        }
        if (BattleAudienceSoundCheck(5) == 1) {
            BattleAudienceSoundStop(5);
        }
        BattleAudienceSoundCheer(-1, 0);
        BattleAudienceSoundSetVol(4, 0, 1);
    }
    if (value == 0) {
        BattleAudienceSoundSetVol(4, volPer, 1);
    }
    if (cheerEnable == 0) {
        sound = BattleAudienceSoundGetPtr(4);
        *(s32*)((s32)sound + 0xC) = 0xB4;
        *(s32*)((s32)sound + 0x10) = 0xB4;
        return 2;
    }
    if (count < 0) {
        count = count2;
        count += irand(2);
        count2 -= 4;
        if (count2 < 0x14) {
            count2 = 0x14;
        }
        newVol = volPer + 5;
        volPer = newVol;
        if ((u8)newVol > 0x64) {
            volPer = 0x64;
        }
    }
    count--;
    return 0;
}

s32 sac_handbeat(void* event, s32 isFirstCall) {
    extern s32 BattleAudienceSoundCheck(s32 id);
    extern void BattleAudienceSoundStop(s32 id);
    extern void BattleAudienceSoundCheer(s32 kind, s32 flags);
    extern void BattleAudienceSoundSetVol(s32 id, s32 volume, s32 flags);
    extern void* BattleAudienceSoundGetPtr(s32 id);
    extern void BattleAudienceSoundHandBeat(void);
    extern s32 irand(s32 max);
    extern s32 count;
    extern s32 count2;
    extern u8 volPer;

    s32 newVol;
    void* sound;

    if (isFirstCall != 0) {
        count = 0;
        count2 = 0x3C;
        handBeatEnable = 1;
        volPer = 0;
        if (BattleAudienceSoundCheck(4) == 1) {
            BattleAudienceSoundStop(4);
        }
        if (BattleAudienceSoundCheck(5) == 1) {
            BattleAudienceSoundStop(5);
        }
        BattleAudienceSoundCheer(-1, 0);
        BattleAudienceSoundSetVol(4, 0, 1);
    }
    BattleAudienceSoundSetVol(4, volPer, 1);
    if (handBeatEnable == 0) {
        sound = BattleAudienceSoundGetPtr(4);
        *(s32*)((s32)sound + 0xC) = 0xB4;
        *(s32*)((s32)sound + 0x10) = 0xB4;
        return 2;
    }
    if (count < 0) {
        BattleAudienceSoundHandBeat();
        count = count2;
        count += irand(2);
        count2 -= 4;
        if (count2 < 0x14) {
            count2 = 0x14;
        }
        newVol = volPer + 5;
        volPer = newVol;
        if ((u8)newVol > 0x64) {
            volPer = 0x64;
        }
    }
    count--;
    return 0;
}
