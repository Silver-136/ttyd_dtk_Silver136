#include "event/evt_cam.h"
#include "event/evt_cmd.h"
#include "cam_road.h"

void* marioGetPtr(void);
void* camGetPtr(s32 id);
void camShiftReset(void* cam);
void camCtrlOn(s32 id);
void camCtrlOff(s32 id);
void psndSetFlag(s32 flag);
void psndClearFlag(s32 flag);
extern void* gp;
extern const f32 float_0p01_80421040;
extern const f32 float_0_80421044;
extern const f32 float_100_80421048;
typedef struct EvtCamVec3 {
    f32 x;
    f32 y;
    f32 z;
} EvtCamVec3;
extern const EvtCamVec3 vec3_802c4438;
extern const EvtCamVec3 vec3_802c4420;
extern const EvtCamVec3 vec3_802c442c;
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_cam_ctrl_onoff) {
    s32* args = event->args;
    s32 camId = evtGetValue(event, args[0]);

    if (evtGetValue(event, args[1]) != 0) {
        camCtrlOn(camId);
    } else {
        camCtrlOff(camId);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_cam_get_pos) {
    s32* args = event->args;
    void* cam = camGetPtr(evtGetValue(event, args[0]));

    evtSetFloat(event, args[1], *(f32*)((s32)cam + 0xC));
    evtSetFloat(event, args[2], *(f32*)((s32)cam + 0x10));
    evtSetFloat(event, args[3], *(f32*)((s32)cam + 0x14));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_cam_get_at) {
    s32* args = event->args;
    void* cam = camGetPtr(evtGetValue(event, args[0]));

    evtSetFloat(event, args[1], *(f32*)((s32)cam + 0x18));
    evtSetFloat(event, args[2], *(f32*)((s32)cam + 0x1C));
    evtSetFloat(event, args[3], *(f32*)((s32)cam + 0x20));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 evt_cam_shake(s32 pEvt, s32 param_2) {
    typedef struct Pool { u8 pad[0xD8]; f64 half, three, zero; } Pool;
    extern Pool vec3_802c43a8;
    extern f32 evtGetFloat(void*, s32);
    extern void PSMTX44Trans(void*, f32, f32, f32);
    extern void padRumbleOn(s32);
    extern void padRumbleOff(s32);
    extern f32 __float_nan[];
    extern f32 float_13000_80421050, float_50_80421054, float_1_80421058;
    Pool* pool = &vec3_802c43a8;
    s32* args = *(s32**)(pEvt + 0x18);
    s32 camId = evtGetValue((void*)pEvt, args[0]);
    f32 x = evtGetFloat((void*)pEvt, args[1]);
    f32 y = evtGetFloat((void*)pEvt, args[2]);
    s32 duration = evtGetValue((void*)pEvt, args[3]);
    u64 now = *(u64*)pEvt;
    void* cam;
    f32 len;
    s32 onTime, offTime;
    u64 elapsed;
    if (param_2 != 0) {
        *(s32*)(pEvt + 0x78) = 0;
        *(u64*)(pEvt + 0x7C) = ((u64)(u32)(now >> 32) << 32) | (u32)now;
        *(s32*)(pEvt + 0x84) = 0;
        *(u64*)(pEvt + 0x88) = ((u64)(u32)(now >> 32) << 32) | (u32)now;
    }
    *(s32*)(pEvt + 0x78) += 1;
    *(s32*)(pEvt + 0x78) %= 4;
    if (*(s32*)(pEvt + 0x78) < 2) {
        cam = camGetPtr(camId);
        PSMTX44Trans((void*)((s32)cam + 0x1A0), x, y, float_0_80421044);
    } else {
        cam = camGetPtr(camId);
        PSMTX44Trans((void*)((s32)cam + 0x1A0), -x, -y, float_0_80421044);
    }
    cam = camGetPtr(camId);
    *(u16*)cam |= 8;
    if (duration != 0 &&
        (now - *(u64*)(pEvt + 0x7C)) /
        (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6) > duration) {
        cam = camGetPtr(camId);
        *(u16*)cam &= ~8;
        padRumbleOff(0);
        return 2;
    }
    {

        f64 square, inv;
        s32 exponent, type;
        len = x * x + y * y;
        square = len;
        if (square > (f64)float_0_80421044) {
            inv = __frsqrte(square);
            inv = (pool->half * inv) * (pool->three - square * (inv * inv));
            inv = (pool->half * inv) * (pool->three - square * (inv * inv));
            inv = (pool->half * inv) * (pool->three - square * (inv * inv));
            len = (f32)(square * inv);
        } else if (square < pool->zero) {
            len = __float_nan[0];
        } else {
            exponent = *(u32*)&len & 0x7F800000;
            switch (exponent) {
            case 0x7F800000: if ((*(u32*)&len & 0x007FFFFF) != 0) type = 1; else type = 2; break;
            case 0: if ((*(u32*)&len & 0x007FFFFF) != 0) type = 5; else type = 3; break;
            default: type = 4; break;
            }
            if (type == 1) len = __float_nan[0];
        }
    }
    onTime = (s32)(float_13000_80421050 * len);
    offTime = (s32)(float_50_80421054 * (float_1_80421058 - len));
    elapsed = *(u64*)(pEvt + 0x88);
    if (onTime != 0 && offTime != 0) {
        if (*(s32*)(pEvt + 0x84) == 0) {
            padRumbleOn(0);
            if ((now - elapsed) / (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6) > onTime) {
                *(s32*)(pEvt + 0x84) = 1 - *(s32*)(pEvt + 0x84);
                *(u64*)(pEvt + 0x88) = ((u64)(u32)(now >> 32) << 32) | (u32)now;
            }
        } else {
            padRumbleOff(0);
            if ((now - elapsed) / (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6) > offTime) {
                *(s32*)(pEvt + 0x84) = 1 - *(s32*)(pEvt + 0x84);
                *(u64*)(pEvt + 0x88) = ((u64)(u32)(now >> 32) << 32) | (u32)now;
            }
        }
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on

s32 evt_cam3d_evt_zoom_in(void* pEvt) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct Pair { u32 x, y; } Pair;
    extern void PSVECSubtract(void*, void*, void*);
    extern void PSVECScale(void*, void*, f32);
    extern void PSVECAdd(void*, void*, void*);
    extern double cos(double);
    extern double sin(double);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 percent = evtGetValue(pEvt, args[0]);
    s32 time = evtGetValue(pEvt, args[1]);
    s32 dist = evtGetValue(pEvt, args[2]);
    s32 type = evtGetValue(pEvt, args[3]);
    void* cam = camGetPtr(4);
    Vec3 pos;
    PSVECSubtract((void*)((s32)cam + 0x18), (void*)((s32)cam + 0x0C), &pos);
    PSVECScale(&pos, &pos, (f32)percent / float_100_80421048);
    PSVECAdd(&pos, (void*)((s32)cam + 0x0C), &pos);
    *(EvtCamVec3*)((s32)cam + 0x58) = *(EvtCamVec3*)((s32)cam + 0x0C);
    *(EvtCamVec3*)((s32)cam + 0x64) = *(EvtCamVec3*)((s32)cam + 0x18);
    *(Vec3*)((s32)cam + 0x40) = pos;
    *(EvtCamVec3*)((s32)cam + 0x4C) = *(EvtCamVec3*)((s32)cam + 0x18);
    *(f32*)((s32)cam + 0x40) += (f32)dist * (f32)cos(*(f32*)((s32)cam + 0x114));
    *(f32*)((s32)cam + 0x48) += (f32)dist * (f32)sin(*(f32*)((s32)cam + 0x114));
    *(f32*)((s32)cam + 0x4C) += (f32)dist * (f32)cos(*(f32*)((s32)cam + 0x114));
    *(f32*)((s32)cam + 0x54) += (f32)dist * (f32)sin(*(f32*)((s32)cam + 0x114));
    *(Pair*)((s32)cam + 0x70) = *(Pair*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0x7C) = time * (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6);
    *(u32*)((s32)cam + 0x78) = 0;
    *(u16*)((s32)cam + 0x04) = 3;
    *(u8*)((s32)cam + 0x80) = type;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_cam3d_evt_set_at(void* pEvt) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct Pair {
        u32 x;
        u32 y;
    } Pair;

    extern void PSVECSubtract(void* a, void* b, void* out);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSVECAdd(void* a, void* b, void* out);

    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 x = evtGetValue(pEvt, args[0]);
    s32 y = evtGetValue(pEvt, args[1]);
    s32 z = evtGetValue(pEvt, args[2]);
    s32 percent = evtGetValue(pEvt, args[3]);
    s32 time = evtGetValue(pEvt, args[4]);
    s32 type = evtGetValue(pEvt, args[5]);
    void* cam = camGetPtr(4);
    Vec3 target;
    Vec3 pos;
    Vec3 targetWork = { 0.0f, 0.0f, 0.0f };

    targetWork.x = (f32)x;
    targetWork.y = (f32)y;
    targetWork.z = (f32)z;
    target = targetWork;

    PSVECSubtract(&target, (void*)((s32)cam + 0x0C), &pos);
    PSVECScale(&pos, &pos, (f32)percent / float_100_80421048);
    PSVECAdd(&pos, (void*)((s32)cam + 0x0C), &pos);

    *(EvtCamVec3*)((s32)cam + 0x58) = *(EvtCamVec3*)((s32)cam + 0x0C);
    *(EvtCamVec3*)((s32)cam + 0x64) = *(EvtCamVec3*)((s32)cam + 0x18);
    *(Vec3*)((s32)cam + 0x40) = pos;
    *(Vec3*)((s32)cam + 0x4C) = target;
    *(Pair*)((s32)cam + 0x70) = *(Pair*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0x7C) =
        time * (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6);
    *(u32*)((s32)cam + 0x78) = 0;
    *(u16*)((s32)cam + 0x04) = 3;
    *(u8*)((s32)cam + 0x80) = type;

    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 evt_cam3d_evt_set(s32 pEvt) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct Pair {
        u32 x;
        u32 y;
    } Pair;
    extern const EvtCamVec3 vec3_802c43c0;
    extern const EvtCamVec3 vec3_802c43cc;

    s32* args = *(s32**)(pEvt + 0x18);
    f32 x = (f32)evtGetValue((void*)pEvt, args[0]);
    f32 y = (f32)evtGetValue((void*)pEvt, args[1]);
    f32 z = (f32)evtGetValue((void*)pEvt, args[2]);
    f32 targetX = (f32)evtGetValue((void*)pEvt, args[3]);
    f32 targetY = (f32)evtGetValue((void*)pEvt, args[4]);
    f32 targetZ = (f32)evtGetValue((void*)pEvt, args[5]);
    s32 time = evtGetValue((void*)pEvt, args[6]);
    s32 type = evtGetValue((void*)pEvt, args[7]);
    void* cam = camGetPtr(4);
    Vec3 pos = *(Vec3*)&vec3_802c43c0;
    Vec3 target = *(Vec3*)&vec3_802c43cc;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    target.x = targetX;
    target.y = targetY;
    target.z = targetZ;

    *(EvtCamVec3*)((s32)cam + 0x58) = *(EvtCamVec3*)((s32)cam + 0x0C);
    *(EvtCamVec3*)((s32)cam + 0x64) = *(EvtCamVec3*)((s32)cam + 0x18);
    *(Vec3*)((s32)cam + 0x40) = pos;
    *(Vec3*)((s32)cam + 0x4C) = target;
    *(Pair*)((s32)cam + 0x70) = *(Pair*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0x7C) =
        time * (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6);
    *(u32*)((s32)cam + 0x78) = 0;
    *(u16*)((s32)cam + 0x04) = 3;
    *(u8*)((s32)cam + 0x80) = type;

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 evt_cam3d_evt_set_rel(s32 pEvt) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct Pair {
        u32 x;
        u32 y;
    } Pair;

    s32* args = *(s32**)(pEvt + 0x18);
    f32 xOff = (f32)evtGetValue((void*)pEvt, args[0]);
    f32 yOff = (f32)evtGetValue((void*)pEvt, args[1]);
    f32 zOff = (f32)evtGetValue((void*)pEvt, args[2]);
    f32 targetXOff = (f32)evtGetValue((void*)pEvt, args[3]);
    f32 targetYOff = (f32)evtGetValue((void*)pEvt, args[4]);
    f32 targetZOff = (f32)evtGetValue((void*)pEvt, args[5]);
    s32 time = evtGetValue((void*)pEvt, args[6]);
    s32 type = evtGetValue((void*)pEvt, args[7]);
    void* cam = camGetPtr(4);
    void* mario = marioGetPtr();
    Vec3 target;
    Vec3 pos;

    target.x = *(f32*)((s32)mario + 0x8C) + targetXOff;
    target.y = *(f32*)((s32)mario + 0x90) + targetYOff;
    target.z = *(f32*)((s32)mario + 0x94) + targetZOff;
    pos.x = target.x + xOff;
    pos.y = target.y + yOff;
    pos.z = target.z + zOff;

    *(EvtCamVec3*)((s32)cam + 0x58) = *(EvtCamVec3*)((s32)cam + 0x0C);
    *(EvtCamVec3*)((s32)cam + 0x64) = *(EvtCamVec3*)((s32)cam + 0x18);
    *(Vec3*)((s32)cam + 0x40) = pos;
    *(Vec3*)((s32)cam + 0x4C) = target;
    *(Pair*)((s32)cam + 0x70) = *(Pair*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0x7C) =
        time * (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6);
    *(u32*)((s32)cam + 0x78) = 0;
    *(u16*)((s32)cam + 0x04) = 3;
    *(u8*)((s32)cam + 0x80) = type;

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 evt_cam3d_evt_set_npc_rel(s32 pEvt) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct Pair {
        u32 x;
        u32 y;
    } Pair;

    extern void* evtNpcNameToPtr(void* evt, char* name);
    extern void PSVECAdd(Vec3* a, Vec3* b, Vec3* out);
    extern const EvtCamVec3 vec3_802c43f0;
    extern const EvtCamVec3 vec3_802c43fc;

    s32* args = *(s32**)(pEvt + 0x18);
    char* npcName = (char*)evtGetValue((void*)pEvt, args[0]);
    f32 posX = (f32)evtGetValue((void*)pEvt, args[1]);
    f32 posY = (f32)evtGetValue((void*)pEvt, args[2]);
    f32 posZ = (f32)evtGetValue((void*)pEvt, args[3]);
    f32 targetX = (f32)evtGetValue((void*)pEvt, args[4]);
    f32 targetY = (f32)evtGetValue((void*)pEvt, args[5]);
    f32 targetZ = (f32)evtGetValue((void*)pEvt, args[6]);
    s32 time = evtGetValue((void*)pEvt, args[7]);
    s32 type = evtGetValue((void*)pEvt, args[8]);
    void* cam = camGetPtr(4);
    void* npc = evtNpcNameToPtr((void*)pEvt, npcName);
    Vec3 inputTarget = *(Vec3*)&vec3_802c43f0;
    Vec3 inputPos = *(Vec3*)&vec3_802c43fc;
    Vec3 target;
    Vec3 pos;
    Vec3 finalTarget;
    Vec3 finalPos;

    inputTarget.x = targetX;
    inputTarget.y = targetY;
    inputTarget.z = targetZ;
    inputPos.x = posX;
    inputPos.y = posY;
    inputPos.z = posZ;
    target = inputTarget;
    pos = inputPos;

    PSVECAdd((Vec3*)((s32)npc + 0x8C), &target, &finalTarget);
    PSVECAdd(&finalTarget, &pos, &finalPos);

    *(EvtCamVec3*)((s32)cam + 0x58) = *(EvtCamVec3*)((s32)cam + 0x0C);
    *(EvtCamVec3*)((s32)cam + 0x64) = *(EvtCamVec3*)((s32)cam + 0x18);
    *(Vec3*)((s32)cam + 0x40) = finalPos;
    *(Vec3*)((s32)cam + 0x4C) = finalTarget;
    *(Pair*)((s32)cam + 0x70) = *(Pair*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0x7C) =
        time * (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6);
    *(u32*)((s32)cam + 0x78) = 0;
    *(u16*)((s32)cam + 0x04) = 3;
    *(u8*)((s32)cam + 0x80) = type;

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 evt_cam3d_evt_set_rel_dir(s32 pEvt) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef f32 Mtx[3][4];
    typedef struct Pair {
        u32 x;
        u32 y;
    } Pair;

    extern f32 float_deg2rad_8042104c;
    extern void PSMTXRotRad(Mtx m, double angle, char axis);
    extern void PSMTXMultVec(Mtx m, Vec3* src, Vec3* dst);
    extern void PSVECAdd(Vec3* a, Vec3* b, Vec3* out);

    s32* args = *(s32**)(pEvt + 0x18);
    f32 posX = (f32)evtGetValue((void*)pEvt, args[0]);
    f32 posY = (f32)evtGetValue((void*)pEvt, args[1]);
    f32 posZ = (f32)evtGetValue((void*)pEvt, args[2]);
    f32 targetX = (f32)evtGetValue((void*)pEvt, args[3]);
    f32 targetY = (f32)evtGetValue((void*)pEvt, args[4]);
    f32 targetZ = (f32)evtGetValue((void*)pEvt, args[5]);
    s32 time = evtGetValue((void*)pEvt, args[6]);
    s32 type = evtGetValue((void*)pEvt, args[7]);
    f32 angle = -*(f32*)((s32)camGetPtr(4) + 0x114);
    void* cam = camGetPtr(4);
    void* mario = marioGetPtr();
    Vec3 inputTarget = { 0.0f, 0.0f, 0.0f };
    Vec3 inputPos = { 0.0f, 0.0f, 0.0f };
    Vec3 target;
    Vec3 pos;
    Vec3 finalTarget;
    Vec3 finalPos;
    Mtx rot;

    inputTarget.x = targetX;
    inputTarget.y = targetY;
    inputTarget.z = targetZ;
    inputPos.x = posX;
    inputPos.y = posY;
    inputPos.z = posZ;
    target = inputTarget;
    pos = inputPos;

    PSMTXRotRad(rot, (double)(float_deg2rad_8042104c * angle), 'Y');
    PSMTXMultVec(rot, &pos, &pos);
    PSMTXMultVec(rot, &target, &target);
    PSVECAdd((Vec3*)((s32)mario + 0x8C), &target, &finalTarget);
    PSVECAdd(&finalTarget, &pos, &finalPos);

    *(EvtCamVec3*)((s32)cam + 0x58) = *(EvtCamVec3*)((s32)cam + 0x0C);
    *(EvtCamVec3*)((s32)cam + 0x64) = *(EvtCamVec3*)((s32)cam + 0x18);
    *(Vec3*)((s32)cam + 0x40) = finalPos;
    *(Vec3*)((s32)cam + 0x4C) = finalTarget;
    *(Pair*)((s32)cam + 0x70) = *(Pair*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0x7C) =
        time * (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6);
    *(u32*)((s32)cam + 0x78) = 0;
    *(u16*)((s32)cam + 0x04) = 3;
    *(u8*)((s32)cam + 0x80) = type;

    return 2;
}

#pragma use_lmw_stmw off

s32 evt_cam3d_evt_off(void* pEvt) {
    typedef struct Pair {
        u32 x;
        u32 y;
    } Pair;

    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 time = evtGetValue(pEvt, args[0]);
    s32 type = evtGetValue(pEvt, args[1]);
    void* cam = camGetPtr(4);

    *(EvtCamVec3*)((s32)cam + 0x58) = *(EvtCamVec3*)((s32)cam + 0x0C);
    *(EvtCamVec3*)((s32)cam + 0x64) = *(EvtCamVec3*)((s32)cam + 0x18);
    *(Pair*)((s32)cam + 0x70) = *(Pair*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0x7C) =
        time * (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6);
    *(u32*)((s32)cam + 0x78) = 0;
    *(u16*)((s32)cam + 0x04) = 1;
    *(u8*)((s32)cam + 0x80) = type;
    return 2;
}

#pragma use_lmw_stmw on

USER_FUNC(evt_cam3d_road_shift_onoff) {
    s32 onoff = evtGetValue(event, event->args[0]);
    void* cam = camGetPtr(4);

    if (onoff != 0) {
        *(u16*)cam &= ~0x20;
    } else {
        *(u16*)cam |= 0x20;
    }
    return 2;
}

s32 evt_cam3d_evt_set_xyz(void* pEvt) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct Pair {
        u32 x;
        u32 y;
    } Pair;


    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 x = evtGetValue(pEvt, args[0]);
    s32 y = evtGetValue(pEvt, args[1]);
    s32 z = evtGetValue(pEvt, args[2]);
    s32 dist = evtGetValue(pEvt, args[3]);
    s32 time = evtGetValue(pEvt, args[4]);
    s32 type = evtGetValue(pEvt, args[5]);
    void* cam = camGetPtr(4);
    EvtCamVec3 old = vec3_802c4420;
    EvtCamVec3 next = vec3_802c442c;

    old.x = *(f32*)((s32)cam + 0x88);
    old.y = *(f32*)((s32)cam + 0x8C);
    old.z = *(f32*)((s32)cam + 0x90);
    *(EvtCamVec3*)((s32)cam + 0xC4) = old;

    next.x = (f32)x;
    next.y = (f32)y;
    next.z = (f32)z;
    *(EvtCamVec3*)((s32)cam + 0xB8) = next;

    *(f32*)((s32)cam + 0xF0) = *(f32*)((s32)cam + 0xE4);
    *(f32*)((s32)cam + 0xEC) = (f32)dist;
    *(Pair*)((s32)cam + 0xD0) = *(Pair*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0xDC) =
        time * (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6);
    *(u32*)((s32)cam + 0xD8) = 0;
    *(u16*)((s32)cam + 0x82) = 3;
    *(u8*)((s32)cam + 0xE0) = type;

    return 2;
}

#pragma use_lmw_stmw off

s32 evt_cam3d_evt_xyz_off(void* pEvt) {
    typedef struct Pair {
        u32 x;
        u32 y;
    } Pair;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 time = evtGetValue(pEvt, args[0]);
    s32 type = evtGetValue(pEvt, args[1]);
    void* cam = camGetPtr(4);
    EvtCamVec3 old = vec3_802c4438;

    old.x = *(f32*)((s32)cam + 0x88);
    old.y = *(f32*)((s32)cam + 0x8C);
    old.z = *(f32*)((s32)cam + 0x90);
    *(EvtCamVec3*)((s32)cam + 0xC4) = old;
    *(f32*)((s32)cam + 0xF0) = *(f32*)((s32)cam + 0xE4);
    *(Pair*)((s32)cam + 0xD0) = *(Pair*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0xDC) =
        time * (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6);
    *(u32*)((s32)cam + 0xD8) = 0;
    *(u16*)((s32)cam + 0x82) = 1;
    *(u8*)((s32)cam + 0xE0) = type;
    return 2;
}

#pragma use_lmw_stmw on

#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
s32 evt_cam3d_get_shift(void* pEvt) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;

    extern void evtSetFloat(void* event, s32 arg, f32 value);
    extern void camShiftMain(void* cam, void* mario, void* out);

    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* mario = marioGetPtr();
    void* cam = camGetPtr(4);
    f32 x = evtGetFloat(pEvt, args[0]);
    f32 y = evtGetFloat(pEvt, args[1]);
    f32 z = evtGetFloat(pEvt, args[2]);
    f32 oldX = *(f32*)((s32)mario + 0x8C);
    f32 oldY = *(f32*)((s32)mario + 0x90);
    f32 oldZ = *(f32*)((s32)mario + 0x94);
    Vec3 out;

    *(f32*)((s32)mario + 0x8C) = x;
    *(f32*)((s32)mario + 0x90) = y;
    *(f32*)((s32)mario + 0x94) = z;
    camShiftMain(cam, mario, &out);
    *(f32*)((s32)mario + 0x8C) = oldX;
    *(f32*)((s32)mario + 0x90) = oldY;
    *(f32*)((s32)mario + 0x94) = oldZ;
    evtSetFloat(pEvt, args[3], out.x);
    return 2;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
USER_FUNC(evt_cam3d_evt_set_now) {
    typedef struct Pair {
        u32 x;
        u32 y;
    } Pair;
    void* cam = camGetPtr(4);
    *(EvtCamVec3*)((s32)cam + 0x40) = *(EvtCamVec3*)((s32)cam + 0xC);
    *(EvtCamVec3*)((s32)cam + 0x58) = *(EvtCamVec3*)((s32)cam + 0x40);
    *(EvtCamVec3*)((s32)cam + 0x4C) = *(EvtCamVec3*)((s32)cam + 0x18);
    *(EvtCamVec3*)((s32)cam + 0x64) = *(EvtCamVec3*)((s32)cam + 0x4C);
    *(Pair*)((s32)cam + 0x70) = *(Pair*)((s32)gp + 0x38);
    *(s32*)((s32)cam + 0x7C) = 0;
    *(s32*)((s32)cam + 0x78) = 0;
    *(u16*)((s32)cam + 0x4) = 3;
    *(u8*)((s32)cam + 0x80) = 0xB;
    return 2;
}

u8 evt_cam3d_event_from_road(s32 pEvt) {
    typedef struct RoadResult {
        f32 unused[3];
        f32 position[3];
        f32 target[3];
    } RoadResult;
    extern f32 evtGetFloat(s32, s32);
    extern void evtSetFloat(s32, s32, f32);
    extern void camRoadMain(f64, f64, f64, f64, f64, f64, f64, void*);
    s32* args = *(s32**)(pEvt + 0x18);
    f32 x = evtGetFloat(pEvt, args[0]);
    f32 y = evtGetFloat(pEvt, args[1]);
    f32 z = evtGetFloat(pEvt, args[2]);
    f32 distance = evtGetFloat(pEvt, args[3]);
    RoadResult road;

    camRoadMain(x, y, z, x, y, z, distance, &road);
    evtSetFloat(pEvt, args[4], road.position[0]);
    evtSetFloat(pEvt, args[5], road.position[1]);
    evtSetFloat(pEvt, args[6], road.position[2]);
    evtSetFloat(pEvt, args[7], road.target[0]);
    evtSetFloat(pEvt, args[8], road.target[1]);
    evtSetFloat(pEvt, args[9], road.target[2]);
    return 2;
}

USER_FUNC(evt_cam_road_reset) {
    void* mario = marioGetPtr();
    void* cam = camGetPtr(4);
    *(f32*)((s32)cam + 0x94) = *(f32*)((s32)mario + 0x8C);
    *(f32*)((s32)cam + 0x98) = *(f32*)((s32)mario + 0x90) + float_0p01_80421040;
    *(f32*)((s32)cam + 0x9C) = *(f32*)((s32)mario + 0x94);
    *(f32*)((s32)cam + 0xAC) = *(f32*)((s32)cam + 0x94);
    *(f32*)((s32)cam + 0xB0) = *(f32*)((s32)cam + 0x98);
    *(f32*)((s32)cam + 0xB4) = *(f32*)((s32)cam + 0x9C);
    *(EvtCamVec3*)((s32)mario + 0xEC) = *(EvtCamVec3*)((s32)mario + 0x8C);
    *(EvtCamVec3*)((s32)mario + 0xF8) = *(EvtCamVec3*)((s32)mario + 0x8C);
    camRoadReset();
    ((void (*)(void))camShiftReset)();
    return 2;
}

#pragma use_lmw_stmw off

s32 evt_cam_road_reset2(void* pEvt) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;

    extern void camShiftReset(void* cam);
    extern const Vec3 vec3_802c4444;
    extern const Vec3 vec3_802c4450;

    s32* args = *(s32**)((s32)pEvt + 0x18);
    f32 x = evtGetFloat(pEvt, args[0]);
    f32 y = evtGetFloat(pEvt, args[1]);
    f32 z = evtGetFloat(pEvt, args[2]);
    void* mario = marioGetPtr();
    void* cam = camGetPtr(4);
    Vec3 a;
    Vec3 b;

    *(f32*)((s32)cam + 0x94) = x;
    *(f32*)((s32)cam + 0x98) = float_0p01_80421040 + y;
    *(f32*)((s32)cam + 0x9C) = z;
    *(f32*)((s32)cam + 0xE8) = float_0_80421044;
    *(f32*)((s32)cam + 0xAC) = *(f32*)((s32)cam + 0x94);
    *(f32*)((s32)cam + 0xB0) = *(f32*)((s32)cam + 0x98);
    *(f32*)((s32)cam + 0xB4) = *(f32*)((s32)cam + 0x9C);

    a = vec3_802c4444;
    a.x = x;
    a.y = y;
    a.z = z;
    *(u32*)((s32)mario + 0xEC) = *(u32*)&a.x;
    *(u32*)((s32)mario + 0xF0) = *(u32*)&a.y;
    *(u32*)((s32)mario + 0xF4) = *(u32*)&a.z;

    b = vec3_802c4450;
    b.x = x;
    b.y = y;
    b.z = z;
    *(u32*)((s32)mario + 0xF8) = *(u32*)&b.x;
    *(u32*)((s32)mario + 0xFC) = *(u32*)&b.y;
    *(u32*)((s32)mario + 0x100) = *(u32*)&b.z;

    camRoadReset();
    ((void (*)(void))camShiftReset)();
    return 2;
}

#pragma use_lmw_stmw on

USER_FUNC(evt_cam_shift_reset) {
    marioGetPtr();
    camShiftReset(camGetPtr(4));
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_cam_type_change(void* pEvt) {
    extern void camSetTypePersp(s32 camId);
    extern void camSetTypeOrtho(s32 camId);
    void* event = pEvt;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 type = evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    if (type == 0) {
        camSetTypePersp(camId);
    } else {
        camSetTypeOrtho(camId);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_cam_letter_box_onoff) {
    s32* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 hide = evtGetValue(event, args[1]);
    void* cam = camGetPtr(8);

    if (on != 0) {
        *(u16*)cam |= 0x100;
        psndSetFlag(0x20);
        *(u16*)((s32)cam + 0x1E0) = 0xFF;
        *(u16*)((s32)cam + 0x1E2) = 0;
        *(u16*)((s32)cam + 0x1E4) = 0;
        *(s32*)((s32)cam + 0x1E8) = 8;
    } else {
        *(u16*)cam &= ~0x100;
        psndClearFlag(0x20);
    }

    if (hide != 0) {
        *(u16*)cam |= 0x400;
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_cam_letter_box_disable) {
    s32 disable = evtGetValue(event, event->args[0]);
    void* cam = camGetPtr(8);

    if (disable != 0) {
        *(u16*)cam |= 0x200;
        psndSetFlag(0x40);
    } else {
        *(u16*)cam &= ~0x200;
        psndClearFlag(0x40);
    }
    return 2;
}
s32 evt_cam_letter_box_camid(void* pEvt) {
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 id = evtGetValue(pEvt, args[0]);

    *(s32*)((s32)camGetPtr(8) + 0x1E8) = id;
    return 2;
}

const EvtCamVec3 vec3_802c4420 = { 0.0f, 0.0f, 0.0f };
const EvtCamVec3 vec3_802c442c = { 0.0f, 0.0f, 0.0f };
const EvtCamVec3 vec3_802c4438 = { 0.0f, 0.0f, 0.0f };
const f64 double_0p5_802c4480 = 0.5;
const f64 double_3_802c4488 = 3.0;
const f64 double_0_802c4490 = 0.0;

const f32 float_0p01_80421040 = 0.01f;
const f32 float_0_80421044 = 0.0f;
const f32 float_100_80421048 = 100.0f;
const f32 float_deg2rad_8042104c = 0.017453292f;
const f32 float_13000_80421050 = 13000.0f;
const f32 float_50_80421054 = 50.0f;
const f32 float_1_80421058 = 1.0f;
