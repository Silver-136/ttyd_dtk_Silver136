#pragma once

#include <dolphin/types.h>

void extInit(void);
void extEntry(int id, int* args, void* initFunc, int arg3, int arg4);
void extReset(void);
void extMain(void);
void* extGetPosePtr(void);
s32 extGetPoseNum(void);
void extPoseDraw(s32 poseNo);
void extDraw(void);
void extLoadRenderMode(void);
void extLoadVertex(void);
void extLoadTexture(void);
void extLoadTextureExit(void);
void extLoadTev(void);
void extLoadShadowVertex(void);
void extLoadShadowTev(void);
void extLoadShadowRenderMode(void);
void extLoadShadowMtx(void* dst);
void extDrawShadow(void);
