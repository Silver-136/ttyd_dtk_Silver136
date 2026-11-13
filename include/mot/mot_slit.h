// mot_slit.c: Mario's paper slit (side-step) motion, with its ability, button, floor and crevice checks.

#include <dolphin/mtx.h>
#include "dolphin/types.h"

BOOL marioSlitAbilityChk(void);
s32 marioSlitButton(void);
void motSlitContinue(void);

BOOL marioChkSlitEnd(void);
BOOL marioChkSlitThrouh(void);
BOOL marioGetMotSlitCancel3(void);
void marioSlitSetAfterLand(void);
BOOL marioChkSlitThrouhEnd(void);
void marioClearSlitFloor(void);
void marioForceSlitAnime(void);
void marioReInit_slit(void);
void marioSlitForceCancel(void);
BOOL marioSlitKeyDisable(void);
void marioSlitReenter(void);
void motSlitCancel(void);
void motSlitCancel2(void);
void motSlitCancel3(void);
void mot_slit(void);
void mot_slit_post(void);
BOOL marioSlitContinueChk(void);
BOOL marioCreviceWallChk(Vec p);

BOOL marioGetSlitFloorPos(Vec* pos);
void init_slit(void);
