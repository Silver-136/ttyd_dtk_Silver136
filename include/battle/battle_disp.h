#pragma once

#include <dolphin/types.h>

#include "battle/battle_unit.h"

void btlDispInit(void);
void btlDispMain(void);

void btlDispGXInit2DRasta(void);
void btlDispGXInit2D(void);
void btlDispGXQuads2DRasta(f32 x1, f32 y1, f32 x2, f32 y2, u8 r, u8 g, u8 b, u8 a);
void btlDispGXPoint2D(u8 r, u8 g, u8 b, u8 a, f32 x, f32 y, f32 s, f32 t);
void btlDispGXQuads2D(f32 x1, f32 y1, f32 x2, f32 y2, u8 r, u8 g, u8 b, u8 a);
void btlDispTexPlane(s32 tpl, void* color, s32 flags, f32 x, f32 y, f32 z, f32 scaleX, f32 scaleY);
void btlDispTexPlane2(void* mtx, s32 texId, void* color);
void btlDispTexPlane3(void* mtx, s32 texId, void* color0, void* color1, void* color2, void* color3);
void btlUnitPartsDisp(s32 cameraId, void* part);
void btlDispEntAnime(void* unit);
void btlDispChangeAnime(BattleWorkUnitPart *part, const char *name, BOOL a3);
void btlDispChangePeraAnime(void* part);
void btlDispPoseAnime(void* part);
void btlGetScreenPoint(void* inPos, void* outScreenSpacePos);
void btlDispTex4(s32 texId, f32* trans, f32* scale, f32* rot, u32* color);
