#include "event/evt_urouro.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 urouro_init_func(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void* mapalloc_base_ptr;
    extern void* _mapAlloc(void* heap, u32 size);
    extern s32 sysMsec2Frame(s32 msec);

    s32* args;
    void* npc;
    s32 value;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;

    args = *(s32**)((s32)event + 0x18);
    name = evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    scale = evtGetFloat(event, args[4]);
    value = evtGetValue(event, args[5]);
    *(void**)((s32)event + 0xC4) = _mapAlloc(mapalloc_base_ptr, 0x2C);
    args = *(s32**)((s32)event + 0xC4);
    args[0] = value;
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    args[4] = sysMsec2Frame(0x1770);
    args[5] = sysMsec2Frame(0x2328);
    args[6] = sysMsec2Frame(0x5DC);
    *(f32*)&args[7] = x;
    *(f32*)&args[8] = *(f32*)((s32)npc + 0x90);
    *(f32*)&args[9] = y;
    *(f32*)&args[10] = z;
    *(f32*)((s32)npc + 0x1B0) = scale;
    *(u32*)npc |= 0x400000;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/event/evt_urouro 20260624_184929 */

/* stub-fill: urouro_main_func | missing_definition | ghidra_signature */
s32 urouro_main_func(void* event) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct NpcWork {
        u8 pad_000[0x8C];
        Vec position;
        u8 pad_098[0x6C];
        s32 poseId;
        u8 pad_108[0x3C];
        f32 rotationY;
        u8 pad_148[0x5C];
        f32 walkingSpeed;
        f32 field_1A8;
        f32 jumpAngle;
        f32 jumpScale;
        f32 moveX;
        f32 field_1B8;
        f32 moveZ;
        u8 pad_1C0[0x18];
        u16 soundSide;
        u16 pad_1DA;
        f32 soundDistance;
        char* leftSfx;
        char* rightSfx;
        u16 soundId;
        u16 pad_1EA;
        u8 pad_1EC[0x110];
        u32 wallStop;
    } NpcWork;
    typedef struct PlayerWork {
        u8 pad_000[0x8C];
        Vec position;
        u8 pad_098[0x10C];
        f32 direction;
    } PlayerWork;

    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void* marioGetPtr(void);
    extern s32 irand(s32);
    extern f64 reviseAngle(f64);
    extern f64 distABf(f64, f64, f64, f64);
    extern u32 animPoseGetPeraEnd(s32);
    extern f64 angleABf(f64, f64, f64, f64);
    extern void* marioGetTouchNpcPtr(void);
    extern f64 compAngle(f64, f64);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern u32 psndGetFlag(void);
    extern u32 animPoseGetMaterialFlag(s32);
    extern char* searchPSSFXList(char*);
    extern void psndSFXOnEx_3D(char*, Vec*, s8, s8, u32, u16);

    s32* args = *(s32**)((u8*)event + 0x18);
    NpcWork* npc = evtNpcNameToPtr(event, evtGetValue(event, args[0]));
    PlayerWork* player = marioGetPtr();
    u32* work = *(u32**)((u8*)event + 0xC4);
    u32 state = work[1];
    s32 random;
    f64 distance;
    f64 angle;

    if (state == 2) {
        random = irand((s32)work[5] / 2 + 1);
        work[2] = (s32)work[5] / 2 + random;
        random = irand(60);
        npc->rotationY = (f32)reviseAngle(npc->rotationY + (f32)random - 30.0f);
        npc->jumpAngle = npc->rotationY;
        work[3] = 0;
        if (work[0] & 0x10000) {
            work[3] = irand(1000) % 3 + 2;
        }
        work[1] = 3;
    } else if ((s32)state < 2) {
        if (state == 0) {
            random = irand((s32)work[4] / 2 + 1);
            work[2] = (s32)work[4] / 2 + random;
            work[2] = ((s32)work[2] / 20) * 20;
            work[3] = 0;
            random = irand(60);
            npc->rotationY = (f32)reviseAngle(npc->rotationY + (f32)random - 30.0f);
            npc->jumpAngle = npc->rotationY;
            work[1] = 1;
        } else if ((s32)state >= 0) {
            distance = distABf(npc->position.x, npc->position.z,
                               (f32)work[7], (f32)work[9]);
            if (animPoseGetPeraEnd(npc->poseId) != 0) {
                if (distance <= (f32)work[10]) {
                    if (work[0] & 1) {
                        work[0] &= ~3;
                    }
                } else {
                    if ((work[0] & 1) == 0) {
                        work[0] |= 3;
                    }
                    if (work[0] & 2) {
                        angle = reviseAngle(angleABf(npc->position.x, npc->position.z,
                                                     (f32)work[7], (f32)work[9]));
                        npc->rotationY = (f32)angle;
                        npc->jumpAngle = npc->rotationY;
                        work[0] &= ~2;
                    }
                }
            }

            if (animPoseGetPeraEnd(npc->poseId) != 0 &&
                npc == marioGetTouchNpcPtr() &&
                __fabsf((f32)compAngle(npc->jumpAngle, player->direction)) < 90.0f) {
                angle = compAngle(angleABf(npc->position.x, npc->position.z,
                                           player->position.x, player->position.z),
                                  player->direction);
                if (__fabsf((f32)angle) < 90.0f && distance < (f32)work[10]) {
                    npc->rotationY = (f32)reviseAngle(npc->jumpAngle - 180.0f);
                    npc->jumpAngle = npc->rotationY;
                    work[0] &= ~3;
                }
            }

            if (animPoseGetPeraEnd(npc->poseId) != 0 && npc->wallStop != 0) {
                npc->rotationY = (f32)reviseAngle(angleABf(npc->position.x, npc->position.z,
                                                           (f32)work[7], (f32)work[9]));
                npc->jumpAngle = npc->rotationY;
                work[0] &= ~3;
            }

            work[2]--;
            if (work[2] == 0) {
                work[1] = 2;
            } else if (animPoseGetPeraEnd(npc->poseId) != 0) {
                f32 speed = npc->jumpScale * npc->walkingSpeed;
                f32 maxSpeed = (20.0f * npc->jumpScale) / 1000.0f;
                u32 soundFlags = 0;

                if (speed > maxSpeed) {
                    speed = maxSpeed;
                }
                npc->moveX = speed * (f32)sin((6.283185f * npc->jumpAngle) / 360.0f);
                npc->moveZ = speed * -(f32)cos((6.283185f * npc->jumpAngle) / 360.0f);

                if ((psndGetFlag() & 0x100) != 0 &&
                    (animPoseGetMaterialFlag(npc->poseId) & 0x1800) == 0) {
                    soundFlags = 0x10000;
                }
                if ((f32)npc->soundId / npc->jumpScale < npc->soundDistance) {
                    char* sfx = npc->soundSide == 0 ? npc->rightSfx : npc->leftSfx;
                    if (sfx != 0) {
                        sfx = searchPSSFXList(sfx);
                        psndSFXOnEx_3D((char*)(soundFlags | (u32)sfx),
                                      &npc->position, -1, -1, 0, 8);
                    }
                    npc->soundDistance = 0.0f;
                    npc->soundSide = (u16)(s8)(1 - (s8)npc->soundSide);
                }
                if (npc->rightSfx != 0 || npc->soundSide != 0) {
                    npc->soundDistance += 1000.0f * npc->walkingSpeed;
                }
            }
        }
    } else if ((s32)state < 4) {
        work[2]--;
        if (work[2] == 0) {
            random = irand((s32)work[6] / 2 + 1);
            work[2] = (s32)work[6] / 2 + random;
            if (work[3] == 0) {
                work[1] = 0;
            } else {
                work[3]--;
                npc->rotationY = (f32)reviseAngle(180.0f + npc->rotationY);
            }
        }
    }
    return 0;
}

