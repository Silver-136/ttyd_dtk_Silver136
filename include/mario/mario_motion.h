#include "dolphin/types.h"
#include "mario/mario.h"

void marioChgMot(MarioMotion id);
BOOL marioChkItemMotion(void);
BOOL marioChkJump(void);
BOOL marioChkTransform(void);

void marioChgJumpStandMotion(f32 v);
void marioChgSmallJumpMotion(void);
f32 marioGetFallSpd(void);
void marioSetFallPara(void);

void marioChgMotSub(MarioMotion id, s32 callPost);
void marioChgShipMotion(void);
void marioChgStayMotion(void);
f32 marioMakeJumpPara(void);
void marioBoots(void);
void marioChgGetItemMotion(void);
void marioChgRollMotion(void);
void marioChgTalkMotion(void);
BOOL marioChkItemGetMotion(void);
void marioClearJumpPara(void);
void marioFall(void);
void marioJump(void);
void marioLandOn(void);
void marioMotion(void);
void marioSetJumpPara(void);
s32 L_marioChkRub(void);
void marioChgMot2(MarioMotion id);
s32 marioRollChgChk(void);
