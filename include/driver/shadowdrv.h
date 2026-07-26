#pragma once

#include <dolphin/types.h>

void shadowInit(void);
void shadowMain(void);
void shadowEntry(double x, double y, double z, double size);
s32 shadowEntryMode(double x, double y, double z, double size, u8 mode);
void shadowSetType(s32 id, u8 type);
void shadowSetSize(s32 id, f32 size);
void shadowSetCamId(s32 id, s16 camId);
void shadowSetCollision(s32 id, s32 on);
void shadowFlagOn(s32 id, u16 flag);
void* shadowGetCharShadowProjection(void);
void shadowDepthShadowDrawCountUp(void);
void shadowProjShadowDrawCountUp(void);
