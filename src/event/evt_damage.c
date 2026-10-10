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
extern const f32 float_180_80422728;
extern const f32 float_1024_8042272c;
extern const f32 float_90_80422730;
extern const f32 float_270_80422734;

/* Data-pass A1: declarations for the complete Gazigazi Event family. */
extern s32 evt_npc_entry(void* event, s32 isFirstCall);
extern s32 evt_npc_set_position(void* event, s32 isFirstCall);
extern s32 evt_npc_pera_onoff(void* event, s32 isFirstCall);
extern s32 evt_npc_set_ry(void* event, s32 isFirstCall);
extern s32 evt_npc_flag_onoff(void* event, s32 isFirstCall);
extern s32 evt_snd_sfxon_3d(void* event, s32 isFirstCall);
extern s32 evt_npc_set_anim(void* event, s32 isFirstCall);
extern s32 evt_npc_wait_anim(void* event, s32 isFirstCall);
extern s32 evt_npc_set_rotate_offset(void* event, s32 isFirstCall);
extern s32 evt_npc_set_rotate(void* event, s32 isFirstCall);
extern s32 evt_npc_set_scale(void* event, s32 isFirstCall);
u8 camdir(void* event);
u8 evt_gazigazi_move_position(void* event, s32 isFirstCall);

/* Data-pass A1: complete target-backed Event family; string arguments remain natural literals. */
u32 evt_gazigazi_entry[25] = {
    0x0003005B, (u32)evt_npc_entry, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57", (u32)(char*)"c_gazigazi",
    0x0005005B, (u32)evt_npc_set_position, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57", 0x00000000,
    0xFFFFFC18, 0x00000000, 0x0003005B, (u32)evt_npc_pera_onoff,
    (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57", 0x00000000, 0x0003005B, (u32)evt_npc_set_ry,
    (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57", 0xF24A7A80, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57", 0x02000000, 0x00000002,
    0x00000001,
};

u32 evt_gazigazi_anim_loop[20] = {
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)"SFX_MARIO_GAJIGAJI1", 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00010005,
    0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57",
    (u32)(char*)"GAZ_A_1", 0x0003005B, (u32)evt_npc_wait_anim, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57",
    0xF24A7E80, 0x00000006, 0x00000002, 0x00000001,
};

u32 evt_gazigazi_appear[104] = {
    0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032,
    0xFE363C8B, 0xFE363C81, 0x00020032, 0xFE363C8C,
    0xFE363C82, 0x00020032, 0xFE363C8D, 0xFE363C80,
    0x00020032, 0xFE363C8E, 0xFE363C81, 0x00020032,
    0xFE363C8F, 0xFE363C82, 0x00020035, 0xFE363C8A,
    0x00000000, 0x00020035, 0xFE363C8B, 0xFFFFFFCE,
    0x00020035, 0xFE363C8C, 0x00000000, 0x00020035,
    0xFE363C8D, 0x00000000, 0x00020035, 0xFE363C8E,
    0xFFFFFFF6, 0x00020035, 0xFE363C8F, 0x00000000,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57",
    0x40000020, 0x0005005B, (u32)evt_npc_set_rotate_offset, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57",
    0x00000000, 0x00000014, 0x00000000, 0x0001005B,
    (u32)camdir, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57",
    0xF24A7A80, 0xFE363C83, 0xF24EB280, 0x0005005B,
    (u32)evt_npc_set_scale, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57", 0xF24A7E80, 0xF24A7E80,
    0xFE363C84, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57",
    (u32)(char*)"GAZ_S_1", 0x0002005D, (u32)evt_gazigazi_anim_loop, 0xFE363C89,
    0x0000006B, 0x000A005B, (u32)evt_gazigazi_move_position, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57",
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0xFE363C8D,
    0xFE363C8E, 0xFE363C8F, 0x0000012C, 0x00000001,
    0x0000006D, 0x0001000A, 0x000003E8, 0x000A005B,
    (u32)evt_gazigazi_move_position, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57", 0xFE363C8D, 0xFE363C8E,
    0xFE363C8F, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000258, 0x00000000, 0x0001005F, 0xFE363C89,
    0x0005005B, (u32)evt_npc_set_position, (u32)(char*)"\x83\x4B\x83\x57\x83\x4B\x83\x57", 0x00000000,
    0xFFFFFC18, 0x00000000, 0x00000002, 0x00000001,
};

/* Data-pass A1: target-backed zero vectors that precede the natural conversion double. */
const Vec3 vec3_802f021c = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_802f0228 = { 0.0f, 0.0f, 0.0f };

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


#pragma no_register_save_helpers on
#pragma optimize_for_size off
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
        Vec3 position;
        startX = intplGetValue(startX, endX, mode, curStep, stepCount);
        startY = intplGetValue(startY, endY, mode, curStep, stepCount);
        startZ = intplGetValue(startZ, endZ, mode, curStep, stepCount);
        position = vec3_802f021c;
        result = 0;
        position.x = startX; position.y = startY; position.z = startZ;
        *(Vec3*)((s32)npc + 0x98) = position;
        *(Vec3*)((s32)npc + 0x8C) = *(Vec3*)((s32)npc + 0x98);
    } else {
        Vec3 position = vec3_802f0228;
        result = 2;
        position.x = endX; position.y = endY; position.z = endZ;
        *(Vec3*)((s32)npc + 0x98) = position;
        *(Vec3*)((s32)npc + 0x8C) = *(Vec3*)((s32)npc + 0x98);
    }

    return result;
}
#pragma no_register_save_helpers reset
#pragma optimize_for_size reset

/* Data-pass A2: complete target-backed .sdata2 scalar family, defined after all consumers. */
const f32 float_180_80422728 = 180.0f;
const f32 float_1024_8042272c = 1024.0f;
const f32 float_90_80422730 = 90.0f;
const f32 float_270_80422734 = 270.0f;
