#pragma once

#include <dolphin/types.h>

extern void* nanNPCWork;
extern char makestring[16];
extern char str_PCT06x_8042285c[];

static inline char* nannpcLookupName(char* name) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    s32* work = (s32*)nanNPCWork;
    s32 i, offset;
    char* entry;
    for (i = 0, offset = 0; i < work[3]; offset += 0xC0, i++) {
        entry = (char*)(work[0] + offset);
        if (((u32)name & 0x80000000) == 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) return entry;
    }
    return NULL;
}


typedef struct NannpcQueryVec { f32 x, y, z; } NannpcQueryVec;

static inline void nannpcResolveMotion(char* entry, NannpcQueryVec oldPos,
                                      NannpcQueryVec normal, f32 mag) {
    typedef NannpcQueryVec Vec;
    extern void* hitCheckFilter(void*, f32*, f32*, f32*, f32*, f32*, f32*, f32*,
                                f32, f32, f32, f32, f32, f32);
    extern void PSVECAdd(Vec*, Vec*, Vec*);
    extern void PSVECSubtract(Vec*, Vec*, Vec*);
    extern void PSVECScale(Vec*, f32, Vec*);
    extern void PSVECNormalize(Vec*, Vec*);
    extern f32 PSVECMag(Vec*);
    extern f32 PSVECDotProduct(Vec*, Vec*);
    extern void C_VECReflect(Vec*, Vec*, Vec*);
                    Vec previous;
                    Vec reflect;
                    Vec hitNormal;
                    Vec hitOther;
                    Vec firstHitPosition;
                    f32 dist;
                    s32 didMove;
                    Vec collisionDelta;
                    *(s32*)(entry + 0xB8) = 0;
                    *(s32*)(entry + 0xB4) = 0;
                    dist = mag;
                    didMove = 1;
                    *(u32*)(entry + 0xB4) = (u32)hitCheckFilter(NULL, &firstHitPosition.x, &firstHitPosition.y, &firstHitPosition.z, &dist, &hitNormal.x, &hitNormal.y, &hitNormal.z, oldPos.x, oldPos.y, oldPos.z, normal.x, normal.y, normal.z);
                    if (*(u32*)(entry + 0xB4) == 0) {
                        PSVECAdd((Vec*)(entry + 0x18), (Vec*)(entry + 0x3C), (Vec*)(entry + 0x18));
                    } else {
                        previous = *(Vec*)(entry + 0x18);
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
                        PSVECSubtract((Vec*)(entry + 0x18), &previous, &collisionDelta);
                        dist = PSVECMag(&collisionDelta);
                        if (dist > 0.0f) {
                            Vec secondHitPosition;
                            PSVECNormalize(&collisionDelta, &collisionDelta);
                            *(u32*)(entry + 0xB8) = (u32)hitCheckFilter(NULL, &secondHitPosition.x, &secondHitPosition.y, &secondHitPosition.z, &dist, &hitOther.x, &hitOther.y, &hitOther.z, previous.x, previous.y + 1.0f, previous.z, collisionDelta.x, collisionDelta.y, collisionDelta.z);
                            if (*(u32*)(entry + 0xB8) != 0) {
                                *(Vec*)(entry + 0x18) = previous;
                            }
                        }
                    }
                    if (didMove != 0) {
                        PSVECScale((Vec*)(entry + 0x3C), 0.6f, (Vec*)(entry + 0x3C));
                    }
}
