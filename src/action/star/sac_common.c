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
