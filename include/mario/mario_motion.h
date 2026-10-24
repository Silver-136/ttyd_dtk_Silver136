#include "dolphin/types.h"
#include "mario/mario.h"

void marioChgMot(MarioMotion);
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
