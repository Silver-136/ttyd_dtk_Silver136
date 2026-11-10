// Declares the action-command help text functions.

#pragma once

#include <dolphin/types.h>

void BattleAcHelpInit(void);
void BattleAcHelpMain(void);

// BattleWork.acHelpDispType: the action command help text. Off also lets the audience window show.
enum {
    AC_HELP_DISP_OFF = 0,
    AC_HELP_DISP_XLU = 1,  // half transparent
    AC_HELP_DISP_ON = 2,
};

void BattleAcHelpSetDispType(s32 type);
void BattleAcHelpSetHelp(const char* msg);
