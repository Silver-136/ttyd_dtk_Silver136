#pragma once

#include <dolphin/types.h>

void windowInit(void);
void windowTexSetup(void);
void windowReInit(void);
s32 windowEntry(s16 type);
s32 windowDelete(void* window);
s32 windowDeleteID(s32 id);
void windowMain(void);
void windowDispGX_Kanban(f32 x, f32 y, f32 width, f32 height, s32 type, u8 alpha);
void windowDispGX_System(f32 x, f32 y, f32 width, f32 height, s32 type, u8 alpha);
void windowDispGX_Waku_col(double x, double y, double width, double height, double curve, u16 gxTexMapID, u32* color);
void windowDispGX2_Waku_col(f32 x, f32 y, f32 width, f32 height, f32 curve, void* mtx, u16 gxTexMapID, u32* color);
s32 windowCheckID(s32 id);
void* windowGetPointer(s32 id);
