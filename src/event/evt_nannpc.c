#include "event/evt_nannpc.h"

extern void extLoadShadowMtx(void*);
extern u8 extLoadShadowRenderMode(void);
extern u8 extLoadShadowVertex(void);
extern u8 extLoadShadowTev(void);
extern u8 extDrawShadow(void);
extern void* nanNPCWork;
s32 evtGetValue(void* event, s32 arg);

typedef struct NanNpcSortEntry {
    f32 unk0;
    f32 z;
} NanNpcSortEntry;


u8 nanNPCOption(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* nanNPCWork;
    extern f32 PSVECDistance(Vec* a, Vec* b);
    extern void PSVECSubtract(Vec* a, Vec* b, Vec* out);
    extern f32 PSVECMag(Vec* v);
    extern void PSVECNormalize(Vec* in, Vec* out);
    extern void PSVECScale(Vec* in, f32 scale, Vec* out);
    extern void PSVECAdd(Vec* a, Vec* b, Vec* out);
    extern s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 flags,
                              s32* outA, s32* outB, s32* outC, f32* length,
                              Vec* hitPos, Vec* hitNormal, Vec* hitOther);
    extern void C_VECReflect(Vec* in, Vec* normal, Vec* out);
    extern f32 PSVECDotProduct(Vec* a, Vec* b);

    s32* work;
    char* entry;
    char* other;
    char* end;
    Vec delta;
    Vec oldPos;
    Vec moveDir;
    Vec normal;
    Vec reflect;
    Vec hitPos;
    Vec hitNormal;
    Vec hitOther;
    s32 outA;
    s32 outB;
    s32 outC;
    f32 dist;
    f32 mag;
    s32 didMove;

    work = (s32*)nanNPCWork;
    end = (char*)(work[0] + work[3] * 0xC0);

    for (entry = (char*)work[0]; entry < end; entry += 0xC0) {
        if ((*(u16*)(entry + 0x14) & 4) != 0) {
            for (other = (char*)work[0]; other < end; other += 0xC0) {
                if (entry != other && (*(u16*)(other + 0x14) & 4) != 0) {
                    dist = PSVECDistance((Vec*)(entry + 0x18), (Vec*)(other + 0x18));
                    PSVECSubtract((Vec*)(entry + 0x18), (Vec*)(other + 0x18), &delta);
                    if (dist <= *(f32*)(entry + 0x4C)) {
                        if (PSVECMag(&delta) == 0.0f) {
                            delta.x = 1.0f;
                        }
                        PSVECNormalize(&delta, &delta);
                        delta.y = 0.0f;
                        PSVECScale(&delta, 0.5f, &delta);
                        PSVECAdd((Vec*)(entry + 0x3C), &delta, (Vec*)(entry + 0x3C));
                        PSVECSubtract((Vec*)(other + 0x3C), &delta, (Vec*)(other + 0x3C));
                    }
                }
            }
        }
    }

    for (entry = (char*)work[0]; entry < end; entry += 0xC0) {
        if ((*(u16*)(entry + 0x14) & 4) != 0) {
            PSVECSubtract((Vec*)(entry + 0x30), (Vec*)(entry + 0x18), &delta);
            if (PSVECMag(&delta) > 0.8f * *(f32*)(entry + 0x4C)) {
                PSVECNormalize(&delta, &delta);
                delta.y = 0.0f;
                PSVECScale(&delta, 0.5f, &delta);
                PSVECAdd((Vec*)(entry + 0x3C), &delta, (Vec*)(entry + 0x3C));
            }
        }
    }

    for (entry = (char*)work[0]; entry < end; entry += 0xC0) {
        if ((*(u16*)(entry + 0x14) & 4) != 0) {
            mag = PSVECMag((Vec*)(entry + 0x3C));
            if (mag != 0.0f) {
                if (mag > 10.0f) {
                    PSVECScale((Vec*)(entry + 0x3C), 5.0f / mag, (Vec*)(entry + 0x3C));
                    mag = 5.0f;
                }

                oldPos = *(Vec*)(entry + 0x18);
                moveDir = *(Vec*)(entry + 0x3C);
                moveDir.y = 0.0f;
                PSVECNormalize((Vec*)(entry + 0x3C), &normal);

                if ((*(u16*)(entry + 0x14) & 8) != 0) {
                    PSVECAdd((Vec*)(entry + 0x18), (Vec*)(entry + 0x3C), (Vec*)(entry + 0x18));
                    PSVECScale((Vec*)(entry + 0x3C), 0.6f, (Vec*)(entry + 0x3C));
                } else {
                    oldPos.y += 1.0f;
                    *(s32*)(entry + 0xB8) = 0;
                    *(s32*)(entry + 0xB4) = 0;
                    dist = mag;
                    didMove = 1;
                    *(s32*)(entry + 0xB4) = hitCheckFilter(oldPos.x, oldPos.y, oldPos.z,
                                                           normal.x, normal.y, normal.z, 0,
                                                           &outA, &outB, &outC, &dist,
                                                           &hitPos, &hitNormal, &hitOther);
                    if (*(s32*)(entry + 0xB4) == 0) {
                        PSVECAdd((Vec*)(entry + 0x18), (Vec*)(entry + 0x3C), (Vec*)(entry + 0x18));
                    } else {
                        oldPos = *(Vec*)(entry + 0x18);
                        PSVECAdd((Vec*)(entry + 0x18), (Vec*)(entry + 0x3C), (Vec*)(entry + 0x18));
                        if (*(f32*)(entry + 0x40) == 0.0f) {
                            hitNormal.y = 0.0f;
                        }
                        PSVECNormalize(&hitNormal, &hitNormal);
                        C_VECReflect(&normal, &hitNormal, &reflect);
                        PSVECScale(&reflect, mag, (Vec*)(entry + 0x3C));
                        didMove = 0;
                        PSVECScale(&hitNormal,
                                   PSVECDotProduct(&normal, &hitNormal) * -(mag - dist) + 0.1f,
                                   &hitNormal);
                        PSVECAdd((Vec*)(entry + 0x18), &hitNormal, (Vec*)(entry + 0x18));
                        PSVECSubtract((Vec*)(entry + 0x18), &oldPos, &delta);
                        dist = PSVECMag(&delta);
                        if (dist > 0.0f) {
                            PSVECNormalize(&delta, &delta);
                            oldPos.y += 1.0f;
                            *(s32*)(entry + 0xB8) = hitCheckFilter(oldPos.x, oldPos.y, oldPos.z,
                                                                   delta.x, delta.y, delta.z, 0,
                                                                   &outA, &outB, &outC, &dist,
                                                                   &hitPos, &hitNormal, &hitOther);
                            if (*(s32*)(entry + 0xB8) != 0) {
                                *(Vec*)(entry + 0x18) = oldPos;
                                ((Vec*)(entry + 0x18))->y -= 1.0f;
                            }
                        }
                    }
                    if (didMove != 0) {
                        PSVECScale((Vec*)(entry + 0x3C), 0.6f, (Vec*)(entry + 0x3C));
                    }
                }
            }
        }
    }

    return 0;
}

u8 evt_nannpc_jump_position(void* event, s32 isFirstCall) {
    extern s32 sprintf(char* buf, char* fmt, ...);
    extern s32 strcmp(char* a, char* b);
    extern s32 evtGetValue(void* event, s32 arg);
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 curStep, s32 maxStep);
    extern void* nanNPCWork;
    extern void* gp;
    extern char makestring[];
    extern char str_PCT06x_8042285c[];

    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;
    f32 x;
    f32 y;
    f32 z;
    s32 steps;
    f32 peak;
    s32 curStep;
    f32 ratio;

    args = *(s32**)((s32)event + 0x18);
    name = (char*)evtGetValue(event, args[0]);
    work = (s32*)nanNPCWork;
    entry = NULL;
    offset = 0;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    steps = evtGetValue(event, args[4]);
    peak = (f32)evtGetValue(event, args[5]);

    if (isFirstCall != 0) {
        *(s32*)(entry + 0x54) = *(s32*)((s32)gp + 0x3C);
        *(s32*)(entry + 0x50) = *(s32*)((s32)gp + 0x38);
        *(s32*)(entry + 0x68) = (s32)*(f32*)(entry + 0x18);
        *(s32*)(entry + 0x6C) = (s32)*(f32*)(entry + 0x1C);
        *(s32*)(entry + 0x70) = (s32)*(f32*)(entry + 0x20);
        *(f32*)((s32)event + 0x7C) = -4.0f * peak;
        *(f32*)((s32)event + 0x80) = (y - (f32)*(s32*)(entry + 0x6C)) - *(f32*)((s32)event + 0x7C);
    }

    curStep = (*(s32*)((s32)gp + 0x3C) - *(s32*)(entry + 0x54)) / ((*(u32*)0x800000F8 >> 2) / 4000);
    if (curStep < steps) {
        ratio = (f32)curStep / (f32)steps;
        *(f32*)(entry + 0x18) = intplGetValue((f32)*(s32*)(entry + 0x68), x, 0, curStep, steps);
        *(f32*)(entry + 0x20) = intplGetValue((f32)*(s32*)(entry + 0x70), z, 0, curStep, steps);
        *(f32*)(entry + 0x1C) = ratio * (*(f32*)((s32)event + 0x7C) * ratio + *(f32*)((s32)event + 0x80)) +
                                (f32)*(s32*)(entry + 0x6C);
        return 0;
    }

    *(f32*)(entry + 0x18) = x;
    *(f32*)(entry + 0x1C) = y;
    *(f32*)(entry + 0x20) = z;
    return 2;
}

s32 evt_nannpc_init(void* event, s32 firstCall) {
    extern s32 evtGetValue(void*, s32);
    extern void* mapalloc_base_ptr;
    extern void* _mapAlloc(void*, u32);
    extern void* memset(void*, s32, u32);
    extern void extEntry(s32, void*, void*, s32, s32);
    extern void nannpc_ext_init(void);
    extern void nannpc_ext_main(void);
    extern void nannpc_ext_main_sub(void);
    extern void nannpc_ext_dispent(void);
    s32* args = *(s32**)((u8*)event + 0x18);
    void* poseData = (void*)evtGetValue(event, args[0]);
    s16* table = (s16*)evtGetValue(event, args[1]);
    void* initFunc = (void*)evtGetValue(event, args[2]);
    void* mainFunc = (void*)evtGetValue(event, args[3]);
    s32* work;
    char* entry;
    s32 count;
    s32 i;

    if (!firstCall) {
        work = _mapAlloc(mapalloc_base_ptr, 0x20);
        nanNPCWork = work;
        memset(work, 0, 0x20);
        count = evtGetValue(event, args[4]);
        work[3] = count;
        work[0] = (s32)_mapAlloc(mapalloc_base_ptr, count * 0xC0);
        memset((void*)work[0], 0, count * 0xC0);
        work[1] = (s32)_mapAlloc(mapalloc_base_ptr, count * 8);
        memset((void*)work[1], 0, count * 8);
        work[2] = (s32)_mapAlloc(mapalloc_base_ptr, 0x14);
        memset((void*)work[2], 0, 0x14);
        work[5] = 0;
        work[6] = (s32)nannpc_ext_main_sub;
        work[7] = 0;
        if (initFunc == 0) initFunc = nannpc_ext_init;
        if (mainFunc == 0) mainFunc = nannpc_ext_main;
        extEntry(count, poseData, initFunc, (s32)mainFunc, (s32)nannpc_ext_dispent);
        for (i = 0; i < count; i++) {
            entry = (char*)work[0] + i * 0xC0;
            entry[0] = 0;
            *(s32*)(entry + 0x10) = 0;
            *(u32*)(entry + 0x14) = 0;
            *(f32*)(entry + 0x18) = 0.0f;
            *(f32*)(entry + 0x1C) = 0.0f;
            *(f32*)(entry + 0x20) = 0.0f;
            *(f32*)(entry + 0x24) = 1.0f;
            *(f32*)(entry + 0x28) = 1.0f;
            *(f32*)(entry + 0x2C) = 1.0f;
            *(f32*)(entry + 0x30) = 0.0f;
            *(f32*)(entry + 0x34) = 0.0f;
            *(f32*)(entry + 0x38) = 0.0f;
            *(f32*)(entry + 0x48) = 32.0f;
            *(f32*)(entry + 0x4C) = 32.0f;
            *(u32*)(entry + 0x50) = 0;
            *(u32*)(entry + 0x54) = 0;
            *(u32*)(entry + 0x58) = 0;
            *(u32*)(entry + 0x5C) = 0;
            *(f32*)(entry + 0x60) = 0.0f;
            *(u32*)(entry + 0x64) = 0;
        }
        if (table != 0) {
            for (i = 0; i < 5 && table[i * 2] != -1; i++) {
                *(s16*)(work[2] + i * 4) = table[i * 2];
                *(s16*)(work[2] + i * 4 + 2) = table[i * 2 + 1];
            }
        }
        return 2;
    }
    return 0;
}

void nannpc_ext_disp(void) {
    extern void extLoadRenderMode(void);
    extern void extLoadVertex(void);
    extern void extLoadTexture(void);
    extern void extLoadTev(void);
    extern void extDraw(void);
    extern void extLoadTextureExit(void);
    extern s32 extGetPoseNum(void);
    extern void extPoseDraw(s32 pose);
    extern void qqsort(void* base, s32 count, s32 size, void* compare);
    extern s32 nannpc_zcompare(void*, void*);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevColor(s32, void*);
    extern u32 dat_80422850;
    s32* work;
    s32* sortEntry;
    char* npc;
    u32 color;
    s32 count;
    s32 i;
    s32 pose;

    work = (s32*)nanNPCWork;
    if ((work[4] & 1) == 0) {
        extLoadRenderMode();
        extLoadVertex();
        extLoadTexture();
        extLoadTev();
        extDraw();
        extLoadTextureExit();
        return;
    }

    extLoadVertex();
    extLoadTexture();
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 8, 2, 15);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 4, 1, 7);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    color = dat_80422850;
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &color);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);

    count = extGetPoseNum();
    for (i = count - 1; i >= 0; i--) {
        sortEntry = (s32*)work[1] + i * 2;
        npc = (char*)work[0] + i * 0xC0;
        sortEntry[0] = i;
        sortEntry[1] = *(s32*)(npc + 0x20);
    }
    qqsort((void*)work[1], work[3], 8, nannpc_zcompare);
    for (i = count - 1; i >= 0; i--) {
        pose = *(s32*)(work[1] + i * 8);
        color = *(u32*)(work[0] + pose * 0xC0 + 0xBC);
        GXSetTevColor(1, &color);
        extPoseDraw(pose);
    }
    extLoadTextureExit();
}

s32 evt_nannpc_move_position2(void* event, s32 firstCall) {
    typedef struct Vec3f { f32 x, y, z; } Vec3f;
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern s32 sprintf(char*, const char*, ...);
    extern s32 strcmp(const char*, const char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    extern u8* gpGlobals;
    extern void PSVECSubtract(Vec3f*, Vec3f*, Vec3f*);
    extern f32 PSVECMag(Vec3f*);
    extern void PSVECScale(Vec3f*, f32, Vec3f*);
    extern void PSVECAdd(Vec3f*, Vec3f*, Vec3f*);
    s32* args = *(s32**)((u8*)event + 0x18);
    s32* work = (s32*)nanNPCWork;
    char* name = (char*)evtGetValue(event, args[0]);
    char* entry = 0;
    char* lookup;
    Vec3f target;
    Vec3f delta;
    Vec3f movement;
    f32 speed;
    f32 distance;
    u32 now;
    u32 elapsed;
    u32 duration;
    s32 i;

    for (i = 0; i < work[3]; i++) {
        entry = (char*)work[0] + i * 0xC0;
        lookup = name;
        if (((u32)name & 0x80000000) == 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            lookup = makestring;
        }
        if (strcmp(entry, lookup) == 0) break;
        entry = 0;
    }
    if (entry == 0) return 2;
    now = *(u32*)(gpGlobals + (*(u8*)(gpGlobals + 0x10) ? 0x28 : 0x20));
    target.x = (f32)evtGetValue(event, args[1]);
    target.y = (f32)evtGetValue(event, args[2]);
    target.z = (f32)evtGetValue(event, args[3]);
    speed = evtGetFloat(event, args[4]);
    if (firstCall) {
        *(u32*)((u8*)event + 0xC0) = now;
        *(Vec3f*)(entry + 0x88) = *(Vec3f*)(entry + 0x18);
    }
    PSVECSubtract(&target, (Vec3f*)(entry + 0x88), &delta);
    distance = PSVECMag(&delta);
    duration = (u32)(1000.0f * distance / speed);
    elapsed = (now - *(u32*)((u8*)event + 0xC0)) / 1000;
    if (elapsed < duration) {
        PSVECScale(&delta, (f32)elapsed / (f32)duration, &movement);
        PSVECAdd((Vec3f*)(entry + 0x88), &movement, (Vec3f*)(entry + 0x18));
        return 0;
    }
    *(Vec3f*)(entry + 0x18) = target;
    return 2;
}

s32 evt_nannpc_entry(void* event) {
    extern s32 evtGetValue(void*, s32);
    extern s32 extGetPosePtr(void);
    extern s32 sprintf(char*, const char*, ...);
    extern s32 strcmp(const char*, const char*);
    extern char* strcpy(char*, const char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args = *(s32**)((u8*)event + 0x18);
    s32* work = (s32*)nanNPCWork;
    char* name = (char*)evtGetValue(event, args[0]);
    char* lookup;
    char* entry;
    s32 poseBase;
    s32 i;

    poseBase = extGetPosePtr();
    for (i = 0; i < work[3]; i++) {
        entry = (char*)work[0] + i * 0xC0;
        lookup = name;
        if (((u32)name & 0x80000000) == 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            lookup = makestring;
        }
        strcmp(entry, lookup);
        if (*(s32*)(entry + 0x10) == 0) {
            strcpy(entry, lookup);
            *(s32*)(entry + 0x10) = poseBase + i * 0x20;
            *(u32*)(entry + 0x14) = 0;
            *(f32*)(entry + 0x18) = 0.0f;
            *(f32*)(entry + 0x1C) = 0.0f;
            *(f32*)(entry + 0x20) = 0.0f;
            *(f32*)(entry + 0x24) = 1.0f;
            *(f32*)(entry + 0x28) = 1.0f;
            *(f32*)(entry + 0x2C) = 1.0f;
            *(f32*)(entry + 0x30) = 0.0f;
            *(f32*)(entry + 0x34) = 0.0f;
            *(f32*)(entry + 0x38) = 0.0f;
            *(f32*)(entry + 0x48) = 32.0f;
            *(f32*)(entry + 0x4C) = 32.0f;
            *(u32*)(entry + 0x50) = 0;
            *(u32*)(entry + 0x54) = 0;
            *(u32*)(entry + 0x58) = 0;
            *(u16*)(entry + 0x5C) = 0;
            *(f32*)(entry + 0x60) = 0.0f;
            *(f32*)(*(s32*)(entry + 0x10) + 4) = -*(f32*)(entry + 0x60);
            return 2;
        }
    }
    return 2;
}

void nannpc_ext_main_sub_fast(void) {
    extern s32 extGetPoseNum(void);
    extern u8* gpGlobals;
    s32* work = (s32*)nanNPCWork;
    char* entry;
    s32 pose;
    s32 frame;
    s32 count;
    s32 i;

    count = extGetPoseNum();
    for (i = count - 1; i >= 0; i--) {
        entry = (char*)work[0] + i * 0xC0;
        if (entry[0] == 0) continue;
        if (*(void**)(entry + 0x64) != 0) {
            ((void (*)(void*))*(void**)(entry + 0x64))(entry);
        }
        if (*(s32*)(entry + 0x58) != 0) {
            frame = *(u32*)(gpGlobals + 0x34) + i * i;
            frame %= *(s16*)(entry + 0x5C);
            *(s16*)(entry + 0x5E) = *(s16*)(*(s32*)(entry + 0x58) + frame * 2);
        }
        pose = *(s32*)(entry + 0x10);
        *(s32*)(pose + 0x10) = *(s16*)(entry + 0x5E);
        *(f32*)(pose + 4) = -*(f32*)(entry + 0x60);
        pose = *(s32*)(pose + 0xC);
        if ((*(u16*)(entry + 0x14) & 2) == 0) {
            *(f32*)(pose + 0x0C) = *(f32*)(entry + 0x18);
            *(f32*)(pose + 0x1C) = *(f32*)(entry + 0x1C);
            *(f32*)(pose + 0x2C) = *(f32*)(entry + 0x20);
        } else {
            *(f32*)(pose + 0x0C) = 0.0f;
            *(f32*)(pose + 0x1C) = -1000.0f;
            *(f32*)(pose + 0x2C) = 0.0f;
        }
    }
}

s32 evt_nannpc_ppflag_onoff(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    u32 mask;
    s32 onoff;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    onoff = evtGetValue(pEvt, args[1]);
    mask = (u16)evtGetValue(pEvt, args[2]);
    if (onoff == 0) {
        **(u32**)(entry + 0x10) |= mask;
    } else {
        **(u32**)(entry + 0x10) &= ~mask;
    }
    return 2;
}

s32 evt_nannpc_flag_onoff(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    u16 mask;
    s32 onoff;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    onoff = evtGetValue(pEvt, args[1]);
    mask = (u16)evtGetValue(pEvt, args[2]);
    if (onoff == 0) {
        *(u16*)(entry + 0x14) |= mask;
    } else {
        *(u16*)(entry + 0x14) &= ~mask;
    }
    return 2;
}

s32 evt_nannpc_set_animtbl(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s16* tbl;
    s32 count;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    tbl = (s16*)evtGetValue(pEvt, args[1]);
    *(s16**)(entry + 0x58) = tbl;
    count = 0;
    while (*tbl != -1) {
        tbl++;
        count++;
    }
    *(s16*)(entry + 0x5C) = count;
    if (*(s16*)(entry + 0x5C) == 0) {
        *(s32*)(entry + 0x58) = 0;
    }
    return 2;
}

s32 evt_nannpc_set_color(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    entry[0xBC] = evtGetValue(pEvt, args[1]);
    entry[0xBD] = evtGetValue(pEvt, args[2]);
    entry[0xBE] = evtGetValue(pEvt, args[3]);
    entry[0xBF] = evtGetValue(pEvt, args[4]);
    return 2;
}

s32 evt_nannpc_set_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern f32 evtGetFloat(void*, s32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    *(f32*)(entry + 0x18) = x;
    *(f32*)(entry + 0x30) = x;
    *(f32*)(entry + 0x1C) = y;
    *(f32*)(entry + 0x34) = y;
    *(f32*)(entry + 0x20) = z;
    *(f32*)(entry + 0x38) = z;
    return 2;
}

s32 evt_nannpc_set_dir(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    *(f32*)(entry + 0x60) = (f32)evtGetValue(pEvt, args[1]);
    return 2;
}

s32 evt_nannpc_set_hosei_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern f32 evtGetFloat(void*, s32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    *(f32*)(entry + 0x24) = evtGetFloat(pEvt, args[1]);
    *(f32*)(entry + 0x28) = evtGetFloat(pEvt, args[2]);
    *(f32*)(entry + 0x2C) = evtGetFloat(pEvt, args[3]);
    return 2;
}

s32 evt_nannpc_get_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern void evtSetFloat(void*, s32, f32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    evtSetFloat(pEvt, args[1], *(f32*)(entry + 0x18));
    evtSetFloat(pEvt, args[2], *(f32*)(entry + 0x1C));
    evtSetFloat(pEvt, args[3], *(f32*)(entry + 0x20));
    return 2;
}

s32 evt_nannpc_set_work(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 idx;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    idx = evtGetValue(pEvt, args[1]);
    *(s32*)(entry + idx * 4 + 0xA4) = evtGetValue(pEvt, args[2]);
    return 2;
}

s32 evt_nannpc_set_shadow_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern f32 evtGetFloat(void*, s32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    void* pp;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    pp = *(void**)(entry + 0x10);
    *(u32*)pp |= 0x10;
    *(f32*)((s32)pp + 8) = evtGetFloat(pEvt, args[1]);
    return 2;
}

s32 evt_nannpc_get_work(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern void evtSetValue(void*, s32, s32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 idx;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    idx = evtGetValue(pEvt, args[1]);
    evtSetValue(pEvt, args[2], *(s32*)(entry + idx * 4 + 0xA4));
    return 2;
}

s32 evt_nannpc_set_size(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern f32 evtGetFloat(void*, s32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    *(f32*)(entry + 0x48) = evtGetFloat(pEvt, args[1]);
    *(f32*)(entry + 0x4C) = evtGetFloat(pEvt, args[2]);
    return 2;
}

s32 evt_nannpc_set_func(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    *(u32*)(entry + 0x64) = evtGetValue(pEvt, args[1]);
    return 2;
}

void nannpc_ext_init(void) {
    extern void* extGetPosePtr(void);
    extern s32 extGetPoseNum(void);
    extern void PSMTXIdentity(void* matrix);
    extern f32 float_0_80422870;
    extern f32 float_neg1000_80422874;
    char* pose;
    void* matrix;
    s32 count;

    pose = (char*)extGetPosePtr();
    count = extGetPoseNum();
    while (--count >= 0) {
        *(s32*)(pose + 0x00) = 3;
        *(f32*)(pose + 0x04) = float_0_80422870;
        matrix = *(void**)(pose + 0x0C);
        PSMTXIdentity(matrix);
        *(f32*)((char*)matrix + 0x0C) = float_0_80422870;
        *(f32*)((char*)matrix + 0x1C) = float_neg1000_80422874;
        *(f32*)((char*)matrix + 0x2C) = float_0_80422870;
        pose += 0x20;
    }
}

s32 evt_nannpc_set_subfunc(void* pEvt) {
    s32* args;
    s32 type;

    args = *(s32**)((s32)pEvt + 0x18);
    type = evtGetValue(pEvt, args[0]);
    if (type == 0) {
        *(u32*)((s32)nanNPCWork + 0x14) = evtGetValue(pEvt, args[1]);
    } else if (type == 1) {
        *(u32*)((s32)nanNPCWork + 0x18) = evtGetValue(pEvt, args[1]);
    } else if (type == 2) {
        *(u32*)((s32)nanNPCWork + 0x1C) = evtGetValue(pEvt, args[1]);
    }
    return 2;
}

void nannpc_ext_dispent(void) {
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void nannpc_ext_disp(void);
    extern void nannpc_ext_shadow_disp(void);
    s32 layer;

    layer = (*(u32*)((u8*)nanNPCWork + 0x10) & 1) == 0 ? 1 : 2;
    dispEntry(4, layer, nannpc_ext_disp, 0, 0.0f);
    dispEntry(4, 2, nannpc_ext_shadow_disp, 0, 0.0f);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_nannpcwork_flag_onoff(void* pEvt) {
    s32* args;
    void* work;
    s32 onoff;
    u16 mask;

    args = *(s32**)((s32)pEvt + 0x18);
    onoff = evtGetValue(pEvt, args[0]);
    work = nanNPCWork;
    mask = evtGetValue(pEvt, args[1]);

    if (onoff != 0) {
        *(u32*)((s32)work + 0x10) = *(u32*)((s32)work + 0x10) & ~mask;
    } else {
        *(u32*)((s32)work + 0x10) = *(u32*)((s32)work + 0x10) | mask;
    }

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void nannpc_ext_main_sub(void) {
    extern s32 extGetPoseNum(void);
    s32* work;
    char* entry;
    void (*callback)(void);
    s32 i;

    work = (s32*)nanNPCWork;
    for (i = extGetPoseNum() - 1; i >= 0; i--) {
        entry = (char*)work[0] + i * 0xC0;
        if (*(s8*)entry != 0) {
            callback = *(void (**)(void))(entry + 0x64);
            if (callback != 0) {
                callback();
            }
        }
    }
}

void nannpc_ext_main(void) {
    void (**work)(void) = (void (**)(void))nanNPCWork;
    if (work[5] != 0) work[5]();
    if (work[6] != 0) work[6]();
    if (work[7] != 0) work[7]();
}

u8 evt_nannpc_set_animtbl_sub(int param_1, short* param_2) {
    s32 count = 0;

    *(short**)(param_1 + 0x58) = param_2;
loop:
    if (*param_2 == -1) {
        goto done;
    }
    param_2++;
    count++;
    goto loop;
done:
    *(s16*)(param_1 + 0x5C) = count;
    if (*(s16*)(param_1 + 0x5C) == 0) {
        *(s32*)(param_1 + 0x58) = 0;
    }
}


u8 nannpc_ext_shadow_disp(void) {
    static f32 mt[3][4];

    extLoadShadowMtx(mt);
    extLoadShadowRenderMode();
    extLoadShadowVertex();
    extLoadShadowTev();
    return extDrawShadow();
}


s32 nannpc_zcompare(NanNpcSortEntry* param_1, NanNpcSortEntry* param_2) {
    if (param_1->z < param_2->z) {
        return 1;
    }
    if (param_1->z > param_2->z) {
        return -1;
    }
    return 0;
}
