#pragma once

#include <dolphin/types.h>

typedef s32 (*QSortCompareFunc)(const void* a, const void* b);

char* getMarioStDvdRoot(void);
f32 reviseAngle(f32 angle);
f32 distABf(f32 ax, f32 ay, f32 bx, f32 by);
f32 compAngle(f32 angleA, f32 angleB);
f32 angleABf(f32 ax, f32 ay, f32 bx, f32 by);
f32 intplGetValue(s32 mode, s32 currStep, s32 steps, f32 start, f32 end);
void qqsort(void* base, u32 count, u32 size, QSortCompareFunc compare);
void makeKey(void);
u32 keyGetDir(s32 controller);
u32 keyGetButton(s32 controller);
u32 keyGetDirRep(s32 controller);
u32 keyGetButtonRep(s32 controller);
u32 keyGetDirTrg(s32 controller);
u32 keyGetButtonTrg(s32 controller);
u8 keyGetStickX(s32 controller);
u8 keyGetStickY(s32 controller);
void padRumbleOn(s32 controller);
void padRumbleOff(s32 controller);
void padRumbleHardOff(s32 controller);
u8 padGetRumbleStatus(s32 controller);
void sincosf(f32 angle, f32* outSin, f32* outCos);
void movePos(f32* x, f32* z, f32 distance, f32 angle);
s32 irand(s32 scalar);
u16 sysGetToken(void);
void sysWaitDrawSync(void);
void sysDummyDraw(void* mtx);
f32 getV60FPS(f32 scale, u64 start, u64 end);
f32 sysFrame2SecFloat(f32 frame);
s32 sysMsec2Frame(s32 msec);
f32 sysMsec2FrameFloat(f32 msec);
void memcpy_as4(void* dst, void* src, u32 size);
