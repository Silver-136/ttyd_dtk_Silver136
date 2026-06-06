#include "battle/battle_ac_help.h"

extern void* _battleWorkPointer;
extern f32 float_498_804272b8;

void BattleAudience_WinSetActive(s32 active);
void _ac_help_disp(void);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 priority);

void BattleAcHelpInit(void) {
}

void BattleAcHelpSetDispType(s32 type) {
    *(s32*)((s32)_battleWorkPointer + 0x1900C) = type;
    switch (type) {
        case 0:
            BattleAudience_WinSetActive(1);
            break;
        default:
            BattleAudience_WinSetActive(0);
            break;
    }
}

void BattleAcHelpSetHelp(s32 help) {
    *(s32*)((s32)_battleWorkPointer + 0x19008) = help;
}

void BattleAcHelpMain(void) {
    void* battleWork = _battleWorkPointer;
    void* acHelp = (void*)((s32)battleWork + 0x19008);

    if (*(u32*)acHelp != 0 && *(s32*)((s32)acHelp + 4) != 0 && !(*(u32*)((s32)battleWork + 0xEF4) & 2)) {
        dispEntry(8, 1, _ac_help_disp, 0, float_498_804272b8);
    }
}

void _ac_help_disp(void) {
    const char* msg;
    void* battleWork;
    u16 lines;
    u32 color;
    s32 width;
    s32 lineCount;
    s32 alpha;
    f32 x;

    extern u32 dat_804272a0;
    extern f32 float_0_804272a4;
    extern f32 float_20_804272a8;
    extern f32 float_neg145_804272ac;
    extern f32 float_10_804272b0;
    extern f32 float_neg148_804272b4;
    extern const char* msgSearch(const char* msg);
    extern u32 FontGetMessageWidthLine(const char* msg, u16* lines);
    extern void FontDrawStart_alpha(s32 alpha);
    extern void FontDrawMessage(s32 x, s32 y, const char* msg);
    extern void windowDispGX_Waku_col(s32, void*, f32, f32, f32, f32, f32);

    battleWork = _battleWorkPointer;
    alpha = 0xFF;
    if (*(s32*)((s32)battleWork + 0x1900C) == 1) {
        alpha = 0x80;
    }

    msg = msgSearch(*(const char**)((s32)battleWork + 0x19008));
    width = FontGetMessageWidthLine(msg, &lines);
    lineCount = (u16)(lines + 1);
    color = dat_804272a0;
    ((u8*)&color)[3] = alpha;
    x = float_0_804272a4 - (f32)((s32)(u16)width / 2);
    lines = lineCount;
    windowDispGX_Waku_col(
        0,
        &color,
        x - float_20_804272a8,
        float_neg145_804272ac + (f32)((lineCount - 1) * 0x1D),
        (f32)((u16)width + 0x28),
        (f32)(lineCount * 0x1D + 1),
        float_10_804272b0);
    FontDrawStart_alpha(alpha);
    FontDrawMessage((s32)x, (s32)float_neg148_804272b4 + ((s32)lines - 1) * 0x1D, msg);
}
