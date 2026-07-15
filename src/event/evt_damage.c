#include "event/evt_damage.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

void* camGetPtr(s32 cameraId);
void* marioGetPtr(void);
Vec3* hitGetDamageReturnPos(void* hitObj);
f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
f32 reviseAngle(f32 angle);
extern f32 float_180_80422728;
extern f32 float_1024_8042272c;
extern f32 float_90_80422730;
extern f32 float_270_80422734;


u8 evt_gazigazi_move_position(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, char* name);
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 curStep, s32 maxStep);
    extern void* gp;

    s32* args;
    char* npcName;
    s32 stepCount;
    s32 mode;
    s32 curStep;
    void* npc;
    f32 startX;
    f32 startY;
    f32 startZ;
    f32 endX;
    f32 endY;
    f32 endZ;
    f32 x;
    f32 y;
    f32 z;
    u8 result;

    args = *(s32**)((s32)event + 0x18);
    npcName = (char*)evtGetValue(event, args[0]);
    startX = (f32)evtGetValue(event, args[1]);
    startY = (f32)evtGetValue(event, args[2]);
    startZ = (f32)evtGetValue(event, args[3]);
    endX = (f32)evtGetValue(event, args[4]);
    endY = (f32)evtGetValue(event, args[5]);
    endZ = (f32)evtGetValue(event, args[6]);
    stepCount = evtGetValue(event, args[7]);
    mode = evtGetValue(event, args[8]);
    npc = evtNpcNameToPtr(event, npcName);

    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = *(s32*)((s32)gp + 0x3C);
    }

    curStep = (*(s32*)((s32)gp + 0x3C) - *(s32*)((s32)event + 0x78)) /
              ((*(u32*)0x800000F8 >> 2) / 4000);

    if (curStep < stepCount) {
        x = intplGetValue(startX, endX, mode, curStep, stepCount);
        y = intplGetValue(startY, endY, mode, curStep, stepCount);
        z = intplGetValue(startZ, endZ, mode, curStep, stepCount);
        result = 0;
    } else {
        x = endX;
        y = endY;
        z = endZ;
        result = 2;
    }

    *(f32*)((s32)npc + 0x98) = x;
    *(f32*)((s32)npc + 0x9C) = y;
    *(f32*)((s32)npc + 0xA0) = z;
    *(f32*)((s32)npc + 0x8C) = *(f32*)((s32)npc + 0x98);
    *(f32*)((s32)npc + 0x90) = *(f32*)((s32)npc + 0x9C);
    *(f32*)((s32)npc + 0x94) = *(f32*)((s32)npc + 0xA0);

    return result;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 camdir(void* event) {
    s32 dir;
    void* eventPtr;
    void* cam;
    void* mario;
    Vec3* pos;
    f32 angle;
    s32 inBack;
    s32 value;

    eventPtr = event;
    cam = camGetPtr(4);
    angle = *(f32*)((s32)cam + 0x114);
    mario = marioGetPtr();
    mario = *(void**)((s32)mario + 0x208);
    dir = 1;

    if (mario != 0) {
        pos = hitGetDamageReturnPos(mario);
        if (reviseAngle(angle - angleABf((f32)*(s32*)((s32)eventPtr + 0x9C),
                                         (f32)*(s32*)((s32)eventPtr + 0xA4),
                                         pos->x, pos->z)) > float_180_80422728) {
            angle = reviseAngle(float_180_80422728 + angle);
            dir = -1;
        }
    }

    value = (s32)(float_1024_8042272c * -angle);
    *(s32*)((s32)eventPtr + 0xA8) = value - 0xDB60000 + 0x7A80;

    inBack = 0;
    if (angle > float_90_80422730 && angle < float_270_80422734) {
        inBack = 1;
    }

    if (inBack != 0) {
        dir = -dir;
    }

    *(s32*)((s32)eventPtr + 0xAC) = (dir << 10) - 0xDB60000 + 0x7A80;

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
