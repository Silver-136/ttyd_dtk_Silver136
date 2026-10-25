#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"

void SelectedItemCoordinateColorUpDate(void);
BOOL BattleMenuKeyOKInACT(BattleWork* wp);
void battleMenuDispInit(void);

void battleMenuDispEnd(void);

void BattleMenuDisp_ActSelect_Disp(BattleWork* wp);
void BattleMenuDisp_ActSelect_End(BattleWork* wp);
void BattleMenuDisp_ActSelect_Main(BattleWork* wp);
void BattleMenuDisp_ChangePartySelect_Disp(BattleWork* wp);
void BattleMenuDisp_ChangePartySelect_End(BattleWork* wp);
void BattleMenuDisp_ChangePartySelect_Main(BattleWork* wp);
void BattleMenuDisp_MultiItem_Disp(BattleWork* wp);
void BattleMenuDisp_MultiItem_End(BattleWork* wp);
void BattleMenuDisp_MultiItem_Main(BattleWork* wp);
void BattleMenuDisp_Operation_Disp(BattleWork* wp);
void BattleMenuDisp_Operation_End(BattleWork* wp);
void BattleMenuDisp_Operation_Main(BattleWork* wp);
void BattleMenuDisp_WeaponSelect_Disp(BattleWork* wp);
void BattleMenuDisp_WeaponSelect_End(BattleWork* wp);
void BattleMenuDisp_WeaponSelect_Main(BattleWork* wp);
void BattleMenuDisp_MultiItem_Init(BattleWork* wp, BOOL flag);
void BattleMenuDisp_Operation_Init(BattleWork* wp, BOOL flag);
