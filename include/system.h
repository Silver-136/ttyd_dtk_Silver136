#pragma once

#include <dolphin/mtx.h>

const char* getMarioStDvdRoot(void);
f32 reviseAngle(f32 angle);
f32 distABf(f32 x1, f32 y1, f32 x2, f32 y2);
f32 compAngle(f32 angle1, f32 angle2);
f32 angleABf(f32 x1, f32 y1, f32 x2, f32 y2);

// intplGetValue modes (any other value is called as a function pointer). WAVE: a cosine wave around the end
// value whose size shrinks (or grows) over the steps; FASTER: its frequency rises as the steps go on; BOUNCE:
// the wave folded to one side. Paper Mario 64 has the same formulas except mode 9.
typedef enum EasingType {
    EASING_LINEAR = 0,
    EASING_QUADRATIC_IN = 1,
    EASING_CUBIC_IN = 2,
    EASING_QUARTIC_IN = 3,
    EASING_QUADRATIC_OUT = 4,
    EASING_CUBIC_OUT = 5,
    EASING_QUARTIC_OUT = 6,
    EASING_WAVE_SHRINKING = 7,
    EASING_WAVE_SHRINKING_FASTER = 8,
    EASING_WAVE_GROWING_FASTER = 9,
    EASING_BOUNCE_SHRINKING = 10,
    EASING_SINE_IN_OUT = 11,
    EASING_SINE_OUT = 12,
    EASING_SINE_IN = 13,
} EasingType;

f32 intplGetValue(s32 mode, f32 start, f32 end, s32 currStep, s32 steps);
//void qqsort(void* array, s32 num_elements, s32 element_size, s32(*compare)(const void**, const void**));
void makeKey(void);
u32 keyGetButton(u32 chan);
u32 keyGetButtonTrg(u32 chan);
u32 keyGetButtonRep(u32 chan);
u32 keyGetDir(u32 chan);
u32 keyGetDirTrg(u32 chan);
u32 keyGetDirRep(u32 chan);
s8 keyGetStickX(u32 chan);
s8 keyGetStickY(u32 chan);
s8 keyGetSubStickX(u32 chan);
s8 keyGetSubStickY(u32 chan);
void padRumbleOff(u32 chan);
void padRumbleOn(u32 chan);
void padRumbleHardOff(u32 chan);
u8 padGetRumbleStatus(u32 chan);
void sincosf(f32* ret_sin, f32* ret_cos, f32 degree);
void movePos(float* ret_sin, float* ret_cos, f32 dist, f32 degree);
s32 irand(s32 scalar);
u16 sysGetToken(void);
void sysWaitDrawSync(void);
void sysDummyDraw(Mtx mtx);
f32 getV60FPS(f32 perFrame, s64 startTick, s64 endTick);
f32 sysFrame2SecFloat(f32 frame);
s32 sysMsec2Frame(s32 msec);
f32 sysMsec2FrameFloat(f32 msec);
void mtxGetRotationElement(Mtx src, Mtx dst, s8 firstAxis, s8 secondAxis);
s32 LZ77Decode(u8* input, u8* output);
void* memcpy_as4(void* dest, const void* src, u32 count);

inline Vec initializeVec(Vec* input);

extern const char* build_date_ptr;
extern const char* build_time_ptr;
