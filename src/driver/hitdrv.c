#include "driver/hitdrv.h"

extern void* mapGetWork(void);
extern s32 mapGetJoints(void* mapObj);
extern void hitEntrySub(void* mapObj, s32 arg1, void* arg2, s32 arg3, s32 arg4);

extern void hitReCalcMatrix2(void* hit, void* arg, s32 flag);
extern void PSMTXMultVec(void* mtx, void* src, void* dst);
extern f32 PSVECDistance(void* a, void* b);
extern s32 strcmp(const char* str1, const char* str2);

s32 chkFilterAttr(void* a, void* b) {
    return ((*(u32*)((s32)b + 0x4) & *(u32*)((s32)a + 0x4)) == 0);
}

u32 hitGetAttr(void* hit) {
    if (hit != NULL) {
        return *(u32*)((s32)hit + 0x4);
    }

    return 0;
}

void* hitGetDamageReturnPos(void* hit) {
    return (void*)(*(s32*)((s32)hit + 0xB0) + 0x4);
}

void hitInit(void) {
    ;
}

void hitEntryMOBJ(void* mapObj, void* arg) {
    void* mapWork;

    mapWork = mapGetWork();

    *(s32*)((s32)mapWork + 0x158) += mapGetJoints(mapObj);

    hitEntrySub(mapObj, 0, arg, 1, 0);
}

void hitReCalcMatrix(void* hit, void* arg) {
    f32 pos[3];

    *(u16*)hit |= 0x40;

    hitReCalcMatrix2(hit, arg, 0);

    PSMTXMultVec(
        (void*)((s32)hit + 0xC),
        (void*)(*(s32*)((s32)hit + 0x8) + 0x48),
        pos
    );

    *(f32*)((s32)hit + 0xCC) = PSVECDistance(
        pos,
        (void*)((s32)hit + 0xC0)
    );
}


u8 hitCheckVecFilter(void) {
    return 0;
}


void* hitCheckSphereFilter(void* filter, f32 x, f32 y, f32 z, f32 radius) {
    extern void* mapGetWork(void);
    extern f32 PSVECDistance(void* a, void* b);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern void PSVECNormalize(void* src, void* dst);
    extern f32 PSVECDotProduct(void* a, void* b);
    extern void PSVECCrossProduct(void* a, void* b, void* out);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSVECAdd(void* a, void* b, void* out);
    extern f32 float_neg1_8041f848;
    extern f32 float_0_8041f838;
    extern f32 float_0p5_8041f84c;

    typedef s32 (*HitSphereFilterFunc)(void* work, void* hit);

    void* map;
    void* group;
    void* hit;
    void* selected;
    void* tri;
    f32 pos[3];
    f32 scanPos[3];
    f32 work[16];
    f32 vec0[3];
    f32 vec1[3];
    f32 vec2[3];
    f32 cross0[3];
    f32 cross1[3];
    f32 cross2[3];
    f32 scaled[3];
    f32 hitPos[3];
    f32 hitNormal[3];
    f32 bestDist;
    f32 bestRadius;
    f32 dist;
    f32 hitRadius;
    f32 dot;
    f32 denom;
    f32 side;
    f32 t;
    f32 bestT;
    s32 bestGroup;
    s32 bestIndex;
    s32 groupIndex;
    s32 hitIndex;
    s32 triIndex;
    s32 triHit;
    s32 oneSided;

    map = mapGetWork();
    bestDist = float_neg1_8041f848;
    bestIndex = -1;
    bestGroup = 0;

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    scanPos[0] = x;
    scanPos[1] = y;
    scanPos[2] = z;

    group = map;
    groupIndex = 0;
    while (groupIndex < *(s32*)map) {
        hit = *(void**)((s32)group + 0x15C);
        hitIndex = 0;
        while (hitIndex < *(s32*)((s32)group + 0x158)) {
            if ((*(u32*)((s32)hit + 0x04) & 0x80000000) != 0 &&
                *(s32*)(*(s32*)((s32)hit + 0x08) + 0x5C) != 0 &&
                (*(u16*)hit & 1) == 0 &&
                (*(u16*)hit & 0x80) == 0) {
                dist = PSVECDistance(scanPos, (void*)((s32)hit + 0xC0));
                hitRadius = *(f32*)((s32)hit + 0xCC);
                if (dist <= radius + hitRadius) {
                    if (filter == 0 || ((HitSphereFilterFunc)filter)(work, hit) != 0) {
                        if (bestDist < float_0_8041f838 || dist < bestDist) {
                            bestDist = dist;
                            bestRadius = hitRadius;
                            bestIndex = hitIndex;
                            bestGroup = groupIndex;
                        }
                    }
                }
            }
            hitIndex++;
            hit = (void*)((s32)hit + 0xE4);
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    if (bestIndex < 0) {
        return 0;
    }

    group = (void*)((s32)map + bestGroup * 0x178);
    selected = (void*)(*(s32*)((s32)group + 0x15C) + bestIndex * 0xE4);

    if (bestDist < float_0p5_8041f84c * bestRadius) {
        return selected;
    }

    PSVECSubtract((void*)((s32)selected + 0xC0), scanPos, (void*)((s32)work + 0x18));
    PSVECNormalize((void*)((s32)work + 0x18), (void*)((s32)work + 0x18));

    work[3] = scanPos[0];
    work[4] = scanPos[1];
    work[5] = scanPos[2];
    work[15] = radius;

    oneSided = (*(u8*)(*(s32*)(*(s32*)((s32)selected + 0x08) + 0x58) + 1) == 1);
    *(s32*)work = oneSided;

    tri = *(void**)((s32)selected + 0xAC);
    bestT = float_neg1_8041f848;
    triHit = -1;
    triIndex = 0;
    while (triIndex < *(s16*)((s32)selected + 0xA8)) {
        if (oneSided != 0) {
            if (PSVECDotProduct((void*)((s32)tri + 0x48), (void*)((s32)work + 0x18)) >= float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECSubtract((void*)((s32)work + 0x0C), tri, vec0);
            dot = PSVECDotProduct((void*)((s32)tri + 0x48), vec0);
            if (dot < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x24), cross0);
            side = PSVECDotProduct(vec0, cross0);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECSubtract((void*)((s32)work + 0x0C), (void*)((s32)tri + 0x0C), vec1);
            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x30), cross1);
            side = PSVECDotProduct(vec1, cross1);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECSubtract((void*)((s32)work + 0x0C), (void*)((s32)tri + 0x18), vec2);
            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x3C), cross2);
            side = PSVECDotProduct(vec2, cross2);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            denom = PSVECDotProduct((void*)((s32)tri + 0x48), (void*)((s32)work + 0x18));
        } else {
            PSVECSubtract((void*)((s32)work + 0x0C), tri, vec0);
            dot = PSVECDotProduct((void*)((s32)tri + 0x48), vec0);
            denom = PSVECDotProduct((void*)((s32)tri + 0x48), (void*)((s32)work + 0x18));
            if (denom * dot >= float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x24), cross0);
            side = dot * PSVECDotProduct(vec0, cross0);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECSubtract((void*)((s32)work + 0x0C), (void*)((s32)tri + 0x0C), vec1);
            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x30), cross1);
            side = dot * PSVECDotProduct(vec1, cross1);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECSubtract((void*)((s32)work + 0x0C), (void*)((s32)tri + 0x18), vec2);
            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x3C), cross2);
            side = dot * PSVECDotProduct(vec2, cross2);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }
        }

        t = -dot / denom;
        if (work[15] >= float_0_8041f838 && t >= work[15]) {
            triIndex++;
            tri = (void*)((s32)tri + 0x54);
            continue;
        }

        work[15] = t;
        PSVECScale((void*)((s32)work + 0x18), scaled, t);
        PSVECAdd((void*)((s32)work + 0x0C), scaled, (void*)((s32)work + 0x24));

        if (dot <= float_0_8041f838) {
            *(s32*)((s32)work + 0x30) = *(s32*)((s32)tri + 0x48);
            *(s32*)((s32)work + 0x34) = *(s32*)((s32)tri + 0x4C);
            *(s32*)((s32)work + 0x38) = *(s32*)((s32)tri + 0x50);
        } else {
            PSVECScale((void*)((s32)tri + 0x48), (void*)((s32)work + 0x30), float_neg1_8041f848);
        }

        if (bestT < float_0_8041f838 || work[15] < bestT) {
            bestT = work[15];
            triHit = 1;
        }

        triIndex++;
        tri = (void*)((s32)tri + 0x54);
    }

    if (bestT > radius) {
        return 0;
    }
    if (triHit < 0) {
        return 0;
    }

    return selected;
}

u8 hitObjGetPosSub(int param_1, s32 param_2, int* param_3, int param_4) {
  typedef struct { f32 x; f32 y; f32 z; } HitVec;
  extern void PSMTXMultVec(void* mtx, void* src, void* dst);
  extern void PSVECAdd(void* a, void* b, void* out);
int iVar1;
  int iVar2;
  int iVar3;
  HitVec VStack_d8;
  HitVec VStack_cc;
  HitVec VStack_c0;
  HitVec VStack_b4;
  HitVec VStack_a8;
  HitVec VStack_9c;
  HitVec VStack_90;
  HitVec VStack_84;
  HitVec VStack_78;
  HitVec VStack_6c;
  HitVec VStack_60;
  HitVec VStack_54;
  HitVec VStack_48;
  HitVec VStack_3c;
  HitVec VStack_30;
  
  iVar3 = *(int *)(param_1 + 0xd8);
  if (iVar3 == 0) {
    PSMTXMultVec((void*)(param_1 + 0xc), (void*)(param_1 + 0x9c), &VStack_30);
    PSVECAdd((void*)param_2, &VStack_30, (void*)param_2);
    *param_3 = *param_3 + 1;
  }
  else {
    iVar2 = *(int *)(iVar3 + 0xd8);
    if (iVar2 == 0) {
      PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_3c);
      PSVECAdd((void*)param_2, &VStack_3c, (void*)param_2);
      *param_3 = *param_3 + 1;
    }
    else {
      iVar1 = *(int *)(iVar2 + 0xd8);
      if (iVar1 == 0) {
        PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_54);
        PSVECAdd((void*)param_2, &VStack_54, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      else {
        if (*(int *)(iVar1 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar1 + 0xc), (void*)(iVar1 + 0x9c), &VStack_84);
          PSVECAdd((void*)param_2, &VStack_84, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar1 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar1 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar1 + 0xdc), param_2, param_3, 1);
        }
      }
      iVar2 = *(int *)(iVar2 + 0xdc);
      if (iVar2 != 0) {
        if (*(int *)(iVar2 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_90);
          PSVECAdd((void*)param_2, &VStack_90, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
    }
    iVar3 = *(int *)(iVar3 + 0xdc);
    if (iVar3 != 0) {
      iVar2 = *(int *)(iVar3 + 0xd8);
      if (iVar2 == 0) {
        PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_60);
        PSVECAdd((void*)param_2, &VStack_60, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      else {
        if (*(int *)(iVar2 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_9c);
          PSVECAdd((void*)param_2, &VStack_9c, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
      iVar3 = *(int *)(iVar3 + 0xdc);
      if (iVar3 != 0) {
        if (*(int *)(iVar3 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_a8);
          PSVECAdd((void*)param_2, &VStack_a8, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar3 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar3 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar3 + 0xdc), param_2, param_3, 1);
        }
      }
    }
  }
  if ((param_4 != 0) && (iVar3 = *(int *)(param_1 + 0xdc), iVar3 != 0)) {
    iVar2 = *(int *)(iVar3 + 0xd8);
    if (iVar2 == 0) {
      PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_48);
      PSVECAdd((void*)param_2, &VStack_48, (void*)param_2);
      *param_3 = *param_3 + 1;
    }
    else {
      iVar1 = *(int *)(iVar2 + 0xd8);
      if (iVar1 == 0) {
        PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_6c);
        PSVECAdd((void*)param_2, &VStack_6c, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      else {
        if (*(int *)(iVar1 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar1 + 0xc), (void*)(iVar1 + 0x9c), &VStack_b4);
          PSVECAdd((void*)param_2, &VStack_b4, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar1 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar1 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar1 + 0xdc), param_2, param_3, 1);
        }
      }
      iVar2 = *(int *)(iVar2 + 0xdc);
      if (iVar2 != 0) {
        if (*(int *)(iVar2 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_c0);
          PSVECAdd((void*)param_2, &VStack_c0, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
    }
    iVar3 = *(int *)(iVar3 + 0xdc);
    if (iVar3 != 0) {
      iVar2 = *(int *)(iVar3 + 0xd8);
      if (iVar2 == 0) {
        PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_78);
        PSVECAdd((void*)param_2, &VStack_78, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      else {
        if (*(int *)(iVar2 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_cc);
          PSVECAdd((void*)param_2, &VStack_cc, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
      iVar3 = *(int *)(iVar3 + 0xdc);
      if (iVar3 != 0) {
        if (*(int *)(iVar3 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_d8);
          PSVECAdd((void*)param_2, &VStack_d8, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar3 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar3 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar3 + 0xdc), param_2, param_3, 1);
        }
      }
    }
  }
  return 0;
}

s32 hitCalcVtxPosition(void* hit) {
    extern void* mapGetWork(void);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern void PSVECCrossProduct(void* a, void* b, void* out);

    void* model;
    void* joint;
    void* shape;
    void* poly;
    void* triOut;
    void* vertexBase;
    void* map;
    s32 count;
    s32 jointIndex;
    s32 jointOffset;
    s32 shapeIndex;
    s32 triIndex;
    s32 triOffset;
    s32 vertexCount;
    s32 firstIndex;
    s32 secondIndex;
    s32 thirdIndex;
    s32 tmpIndex;
    s32 stride;
    s32 baseOffset;
    s32 vcdFlags;
    s32 bitCount;
    s32 i;
    s32 v0;
    s32 v1;
    s32 v2;
    s32 src0;
    s32 src1;
    s32 src2;
    u32 vtxDesc;
    f32 local0[3];
    f32 local1[3];
    f32 local2[3];

    count = 0;

    if (*(s32*)((s32)hit + 0xAC) == 0) {
        return 0;
    }

    model = *(void**)((s32)hit + 0x08);
    jointOffset = 0;
    jointIndex = 0;

    while (jointIndex < *(s32*)((s32)model + 0x5C)) {
        joint = *(void**)((s32)model + jointOffset + 0x64);
        if (joint != 0) {
            if (*(u8*)((s32)joint + 0x03) == 0) {
                shape = joint;
                shapeIndex = 0;
                while (shapeIndex < *(s32*)((s32)joint + 0x04)) {
                    poly = *(void**)((s32)shape + 0x10);
                    triOffset = count * 0x54;
                    triIndex = 0;
                    vertexCount = *(s32*)poly;

                    while (triIndex < vertexCount - 2) {
                        triOut = (void*)(*(s32*)((s32)hit + 0xAC) + triOffset);
                        firstIndex = *(s16*)((s32)poly + triIndex * 0x18 + 0x04);
                        secondIndex = *(s16*)((s32)poly + (triIndex + 1) * 0x18 + 0x04);
                        thirdIndex = *(s16*)((s32)poly + (triIndex + 2) * 0x18 + 0x04);
                        vertexBase = (void*)(**(s32**)((s32)joint + 0x0C) + 0x04);

                        count++;
                        triOffset += 0x54;

                        PSMTXMultVec(
                            (void*)((s32)hit + 0x0C),
                            (void*)((s32)vertexBase + firstIndex * 0x0C),
                            triOut
                        );

                        if ((triIndex & 1) == 0) {
                            tmpIndex = secondIndex;
                            secondIndex = thirdIndex;
                            thirdIndex = tmpIndex;
                        }

                        PSMTXMultVec(
                            (void*)((s32)hit + 0x0C),
                            (void*)((s32)vertexBase + secondIndex * 0x0C),
                            (void*)((s32)triOut + 0x0C)
                        );

                        PSMTXMultVec(
                            (void*)((s32)hit + 0x0C),
                            (void*)((s32)vertexBase + thirdIndex * 0x0C),
                            (void*)((s32)triOut + 0x18)
                        );

                        PSVECSubtract((void*)((s32)triOut + 0x18), triOut, (void*)((s32)triOut + 0x24));
                        PSVECSubtract(triOut, (void*)((s32)triOut + 0x0C), (void*)((s32)triOut + 0x30));
                        PSVECSubtract((void*)((s32)triOut + 0x0C), (void*)((s32)triOut + 0x18), (void*)((s32)triOut + 0x3C));
                        PSVECCrossProduct((void*)((s32)triOut + 0x24), (void*)((s32)triOut + 0x30), (void*)((s32)triOut + 0x48));

                        triIndex++;
                    }

                    shape = (void*)((s32)shape + 0x04);
                    shapeIndex++;
                }
            } else {
                map = mapGetWork();
                vcdFlags = *(s32*)((s32)joint + 0x08);
                bitCount = 0;
                i = 0;
                while (i < 12) {
                    bitCount += (vcdFlags >> i) & 1;
                    i++;
                }
                stride = bitCount * 2;
                if (stride == 0) {
                    stride = 6;
                }

                shape = joint;
                baseOffset = count * 0x54;
                shapeIndex = 0;
                while (shapeIndex < *(s32*)((s32)joint + 0x04)) {
                    poly = *(void**)((s32)shape + 0x10);
                    vertexCount = *(u16*)((s32)poly + 0x01);
                    triIndex = 0;
                    triOffset = baseOffset;

                    while (triIndex < vertexCount - 2) {
                        firstIndex = *(u16*)((s32)poly + 0x03);
                        secondIndex = *(u16*)((s32)poly + stride * (triIndex + 1) + 0x03);
                        thirdIndex = *(u16*)((s32)poly + stride * (triIndex + 2) + 0x03);
                        triOut = (void*)(*(s32*)((s32)hit + 0xAC) + triOffset);
                        vertexBase = (void*)(**(s32**)((s32)joint + 0x0C) + 0x04);

                        if ((triIndex & 1) == 0) {
                            tmpIndex = secondIndex;
                            secondIndex = thirdIndex;
                            thirdIndex = tmpIndex;
                        }

                        src0 = (s32)vertexBase + firstIndex * 6;
                        src1 = (s32)vertexBase + secondIndex * 6;
                        src2 = (s32)vertexBase + thirdIndex * 6;

                        local0[0] = (f32)*(s16*)(src0 + 0);
                        local0[1] = (f32)*(s16*)(src0 + 2);
                        local0[2] = (f32)*(s16*)(src0 + 4);
                        local1[0] = (f32)*(s16*)(src1 + 0);
                        local1[1] = (f32)*(s16*)(src1 + 2);
                        local1[2] = (f32)*(s16*)(src1 + 4);
                        local2[0] = (f32)*(s16*)(src2 + 0);
                        local2[1] = (f32)*(s16*)(src2 + 2);
                        local2[2] = (f32)*(s16*)(src2 + 4);

                        PSMTXMultVec((void*)((s32)hit + 0x0C), local0, triOut);
                        PSMTXMultVec((void*)((s32)hit + 0x0C), local1, (void*)((s32)triOut + 0x0C));
                        PSMTXMultVec((void*)((s32)hit + 0x0C), local2, (void*)((s32)triOut + 0x18));

                        PSVECSubtract((void*)((s32)triOut + 0x18), triOut, (void*)((s32)triOut + 0x24));
                        PSVECSubtract(triOut, (void*)((s32)triOut + 0x0C), (void*)((s32)triOut + 0x30));
                        PSVECSubtract((void*)((s32)triOut + 0x0C), (void*)((s32)triOut + 0x18), (void*)((s32)triOut + 0x3C));
                        PSVECCrossProduct((void*)((s32)triOut + 0x24), (void*)((s32)triOut + 0x30), (void*)((s32)triOut + 0x48));

                        if (*(f32*)((s32)triOut + 0x48) != 0.0f ||
                            *(f32*)((s32)triOut + 0x4C) != 0.0f ||
                            *(f32*)((s32)triOut + 0x50) != 0.0f) {
                            triOffset += 0x54;
                            baseOffset += 0x54;
                            count++;
                        }

                        triIndex++;
                    }

                    shape = (void*)((s32)shape + 0x08);
                    shapeIndex++;
                }
            }
        }

        jointOffset += 0x08;
        jointIndex++;
    }

    return count;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void hitReCalcMatrix2(void* hit, void* arg, s32 flag) {
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern s32 hitCalcVtxPosition(void* hit);

    void* child;
    void* sub;
    u16 flags;
    s32 doRecalc;

#define DO_RECALC_NODE(node_, parent_)                                                     \
    do {                                                                                    \
        if ((*(u16*)(node_) & 0x10) != 0) {                                                 \
            PSMTXConcat((parent_), (void*)((s32)(node_) + 0x6C), (void*)((s32)(node_) + 0x0C)); \
        } else {                                                                            \
            PSMTXConcat((parent_), (void*)((s32)(node_) + 0x3C), (void*)((s32)(node_) + 0x0C)); \
        }                                                                                   \
        if ((*(u16*)(node_) & 0x20) != 0) {                                                 \
            PSMTXConcat((void*)((s32)(node_) + 0x0C),                                      \
                        (void*)(*(s32*)((s32)(node_) + 0xD0) + 0xAC),                       \
                        (void*)((s32)(node_) + 0x0C));                                     \
        }                                                                                   \
        PSMTXMultVec((void*)((s32)(node_) + 0x0C),                                         \
                     (void*)((s32)(node_) + 0x9C),                                         \
                     (void*)((s32)(node_) + 0xC0));                                        \
        hitCalcVtxPosition((node_));                                                        \
        *(u16*)(node_) = *(u16*)(node_) & ~0x40;                                            \
    } while (0)

    flags = *(u16*)hit;
    doRecalc = 0;
    if ((flags & 0x40) != 0) {
        doRecalc = 1;
    }
    if (flag != 0) {
        doRecalc = 1;
    }

    if (doRecalc != 0) {
        DO_RECALC_NODE(hit, arg);

        child = *(void**)((s32)hit + 0xD8);
        if (child != 0) {
            DO_RECALC_NODE(child, (void*)((s32)hit + 0x0C));

            sub = *(void**)((s32)child + 0xD8);
            if (sub != 0) {
                hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), 1);
            }
            sub = *(void**)((s32)child + 0xDC);
            if (sub != 0) {
                hitReCalcMatrix2(sub, (void*)((s32)hit + 0x0C), 1);
            }
        }

        child = *(void**)((s32)hit + 0xDC);
        if (child != 0) {
            flags = *(u16*)child;
            doRecalc = 0;
            if ((flags & 0x40) != 0) {
                doRecalc = 1;
            }
            if (flag != 0) {
                doRecalc = 1;
            }

            if (doRecalc != 0) {
                DO_RECALC_NODE(child, arg);

                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), 1);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, arg, flag);
                }
            } else {
                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), flag);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, arg, flag);
                }
            }
        }
    } else {
        child = *(void**)((s32)hit + 0xD8);
        if (child != 0) {
            flags = *(u16*)child;
            doRecalc = 0;
            if ((flags & 0x40) != 0) {
                doRecalc = 1;
            }
            if (flag != 0) {
                doRecalc = 1;
            }

            if (doRecalc != 0) {
                DO_RECALC_NODE(child, (void*)((s32)hit + 0x0C));

                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), 1);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)hit + 0x0C), flag);
                }
            } else {
                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), flag);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)hit + 0x0C), flag);
                }
            }
        }

        child = *(void**)((s32)hit + 0xDC);
        if (child != 0) {
            flags = *(u16*)child;
            doRecalc = 0;
            if ((flags & 0x40) != 0) {
                doRecalc = 1;
            }
            if (flag != 0) {
                doRecalc = 1;
            }

            if (doRecalc != 0) {
                DO_RECALC_NODE(child, arg);

                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), 1);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, arg, flag);
                }
            } else {
                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), flag);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, arg, flag);
                }
            }
        }
    }

#undef DO_RECALC_NODE
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

u8 checkTriVec_xz(void* ray, void* tri) {
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 PSVECDotProduct(void* a, void* b);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern f32 float_0_8041f838;
    extern f32 float_neg1_8041f848;

    f32 vec0[3];
    f32 vec1[3];
    f32 vec2[3];
    f32 dot;
    f32 side;
    f32 denom;
    f32 t;

    PSVECSubtract((void*)((s32)ray + 0x0C), tri, vec0);
    dot = PSVECDotProduct((void*)((s32)tri + 0x48), vec0);

    if (*(s32*)ray != 0) {
        if (dot < float_0_8041f838) {
            return 0;
        }

        side = vec0[1] * (*(f32*)((s32)tri + 0x24) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x2C) * *(f32*)((s32)ray + 0x18))
             + vec0[0] * (-*(f32*)((s32)tri + 0x28) * *(f32*)((s32)ray + 0x20))
             + vec0[2] * (*(f32*)((s32)tri + 0x28) * *(f32*)((s32)ray + 0x18));
        if (side < float_0_8041f838) {
            return 0;
        }

        PSVECSubtract((void*)((s32)ray + 0x0C), (void*)((s32)tri + 0x0C), vec1);
        side = vec1[1] * (*(f32*)((s32)tri + 0x30) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x38) * *(f32*)((s32)ray + 0x18))
             + vec1[0] * (-*(f32*)((s32)tri + 0x34) * *(f32*)((s32)ray + 0x20))
             + vec1[2] * (*(f32*)((s32)tri + 0x34) * *(f32*)((s32)ray + 0x18));
        if (side < float_0_8041f838) {
            return 0;
        }

        PSVECSubtract((void*)((s32)ray + 0x0C), (void*)((s32)tri + 0x18), vec2);
        side = vec2[1] * (*(f32*)((s32)tri + 0x3C) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x44) * *(f32*)((s32)ray + 0x18))
             + vec2[0] * (-*(f32*)((s32)tri + 0x40) * *(f32*)((s32)ray + 0x20))
             + vec2[2] * (*(f32*)((s32)tri + 0x40) * *(f32*)((s32)ray + 0x18));
        if (side < float_0_8041f838) {
            return 0;
        }
    } else {
        denom = *(f32*)((s32)tri + 0x48) * *(f32*)((s32)ray + 0x18)
              + *(f32*)((s32)tri + 0x50) * *(f32*)((s32)ray + 0x20);
        if (dot * denom >= float_0_8041f838) {
            return 0;
        }

        side = dot * (vec0[1] * (*(f32*)((s32)tri + 0x24) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x2C) * *(f32*)((s32)ray + 0x18))
                    + vec0[0] * (-*(f32*)((s32)tri + 0x28) * *(f32*)((s32)ray + 0x20))
                    + vec0[2] * (*(f32*)((s32)tri + 0x28) * *(f32*)((s32)ray + 0x18)));
        if (side < float_0_8041f838) {
            return 0;
        }

        PSVECSubtract((void*)((s32)ray + 0x0C), (void*)((s32)tri + 0x0C), vec1);
        side = dot * (vec1[1] * (*(f32*)((s32)tri + 0x30) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x38) * *(f32*)((s32)ray + 0x18))
                    + vec1[0] * (-*(f32*)((s32)tri + 0x34) * *(f32*)((s32)ray + 0x20))
                    + vec1[2] * (*(f32*)((s32)tri + 0x34) * *(f32*)((s32)ray + 0x18)));
        if (side < float_0_8041f838) {
            return 0;
        }

        PSVECSubtract((void*)((s32)ray + 0x0C), (void*)((s32)tri + 0x18), vec2);
        side = dot * (vec2[1] * (*(f32*)((s32)tri + 0x3C) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x44) * *(f32*)((s32)ray + 0x18))
                    + vec2[0] * (-*(f32*)((s32)tri + 0x40) * *(f32*)((s32)ray + 0x20))
                    + vec2[2] * (*(f32*)((s32)tri + 0x40) * *(f32*)((s32)ray + 0x18)));
        if (side < float_0_8041f838) {
            return 0;
        }
    }

    denom = *(f32*)((s32)tri + 0x48) * *(f32*)((s32)ray + 0x18)
          + *(f32*)((s32)tri + 0x50) * *(f32*)((s32)ray + 0x20);
    t = -dot / denom;

    if (*(f32*)((s32)ray + 0x3C) < float_0_8041f838 || t < *(f32*)((s32)ray + 0x3C)) {
        *(f32*)((s32)ray + 0x3C) = t;
        *(f32*)((s32)ray + 0x24) = *(f32*)((s32)ray + 0x18) * t + *(f32*)((s32)ray + 0x0C);
        *(f32*)((s32)ray + 0x28) = *(f32*)((s32)ray + 0x10);
        *(f32*)((s32)ray + 0x2C) = *(f32*)((s32)ray + 0x20) * t + *(f32*)((s32)ray + 0x14);

        if (dot < float_0_8041f838) {
            *(s32*)((s32)ray + 0x30) = *(s32*)((s32)tri + 0x48);
            *(s32*)((s32)ray + 0x34) = *(s32*)((s32)tri + 0x4C);
            *(s32*)((s32)ray + 0x38) = *(s32*)((s32)tri + 0x50);
        } else {
            PSVECScale((void*)((s32)tri + 0x48), (void*)((s32)ray + 0x30), float_neg1_8041f848);
        }
        return 1;
    }

    return 0;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

u8 hitDamageReturnSet(int param_1, s32 param_2, int param_3) {
  int iVar1;
  int iVar2;
  int iVar3;
  
  *(u32 *)(param_1 + 4) = *(u32 *)(param_1 + 4) | 0x40000000;
  *(s32 *)(param_1 + 0xb0) = param_2;
  iVar3 = *(int *)(param_1 + 0xd8);
  if (iVar3 != 0) {
    *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
    *(s32 *)(iVar3 + 0xb0) = param_2;
    iVar2 = *(int *)(iVar3 + 0xd8);
    if (iVar2 != 0) {
      *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
      *(s32 *)(iVar2 + 0xb0) = param_2;
      iVar1 = *(int *)(iVar2 + 0xd8);
      if (iVar1 != 0) {
        *(u32 *)(iVar1 + 4) = *(u32 *)(iVar1 + 4) | 0x40000000;
        *(s32 *)(iVar1 + 0xb0) = param_2;
        if (*(int *)(iVar1 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar1 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar1 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar1 + 0xdc), param_2, 1);
        }
      }
      iVar2 = *(int *)(iVar2 + 0xdc);
      if (iVar2 != 0) {
        *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
        *(s32 *)(iVar2 + 0xb0) = param_2;
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xdc), param_2, 1);
        }
      }
    }
    iVar3 = *(int *)(iVar3 + 0xdc);
    if (iVar3 != 0) {
      *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
      *(s32 *)(iVar3 + 0xb0) = param_2;
      iVar2 = *(int *)(iVar3 + 0xd8);
      if (iVar2 != 0) {
        *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
        *(s32 *)(iVar2 + 0xb0) = param_2;
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xdc), param_2, 1);
        }
      }
      iVar3 = *(int *)(iVar3 + 0xdc);
      if (iVar3 != 0) {
        *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
        *(s32 *)(iVar3 + 0xb0) = param_2;
        if (*(int *)(iVar3 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar3 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar3 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar3 + 0xdc), param_2, 1);
        }
      }
    }
  }
  if ((param_3 != 0) && (iVar3 = *(int *)(param_1 + 0xdc), iVar3 != 0)) {
    *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
    *(s32 *)(iVar3 + 0xb0) = param_2;
    iVar2 = *(int *)(iVar3 + 0xd8);
    if (iVar2 != 0) {
      *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
      *(s32 *)(iVar2 + 0xb0) = param_2;
      iVar1 = *(int *)(iVar2 + 0xd8);
      if (iVar1 != 0) {
        *(u32 *)(iVar1 + 4) = *(u32 *)(iVar1 + 4) | 0x40000000;
        *(s32 *)(iVar1 + 0xb0) = param_2;
        if (*(int *)(iVar1 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar1 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar1 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar1 + 0xdc), param_2, 1);
        }
      }
      iVar2 = *(int *)(iVar2 + 0xdc);
      if (iVar2 != 0) {
        *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
        *(s32 *)(iVar2 + 0xb0) = param_2;
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xdc), param_2, 1);
        }
      }
    }
    iVar3 = *(int *)(iVar3 + 0xdc);
    if (iVar3 != 0) {
      *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
      *(s32 *)(iVar3 + 0xb0) = param_2;
      iVar2 = *(int *)(iVar3 + 0xd8);
      if (iVar2 != 0) {
        *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
        *(s32 *)(iVar2 + 0xb0) = param_2;
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xdc), param_2, 1);
        }
      }
      iVar3 = *(int *)(iVar3 + 0xdc);
      if (iVar3 != 0) {
        *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
        *(s32 *)(iVar3 + 0xb0) = param_2;
        if (*(int *)(iVar3 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar3 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar3 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar3 + 0xdc), param_2, 1);
        }
      }
    }
  }
  return;
  return 0;
}

u8 hitAtrOff(int param_1, u32 param_2, int param_3) {
    void* iVar1;
    void* iVar2;
    void* iVar3;
    u32 value = ~param_2;

    *(u32*)(param_1 + 0x04) = *(u32*)(param_1 + 0x04) & value;

    iVar3 = *(void**)(param_1 + 0xD8);
    if (iVar3 != 0) {
        *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;

        iVar2 = *(void**)((s32)iVar3 + 0xD8);
        if (iVar2 != 0) {
            *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;

            iVar1 = *(void**)((s32)iVar2 + 0xD8);
            if (iVar1 != 0) {
                *(u32*)((s32)iVar1 + 0x04) = *(u32*)((s32)iVar1 + 0x04) & value;
                if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                }
            }

            iVar2 = *(void**)((s32)iVar2 + 0xDC);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }
        }

        iVar3 = *(void**)((s32)iVar3 + 0xDC);
        if (iVar3 != 0) {
            *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;
                if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                }
            }
        }
    }

    if (param_3 != 0) {
        iVar3 = *(void**)(param_1 + 0xDC);
        if (iVar3 != 0) {
            *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;

                iVar1 = *(void**)((s32)iVar2 + 0xD8);
                if (iVar1 != 0) {
                    *(u32*)((s32)iVar1 + 0x04) = *(u32*)((s32)iVar1 + 0x04) & value;
                    if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                    }
                }

                iVar2 = *(void**)((s32)iVar2 + 0xDC);
                if (iVar2 != 0) {
                    *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;

                iVar2 = *(void**)((s32)iVar3 + 0xD8);
                if (iVar2 != 0) {
                    *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }

                iVar3 = *(void**)((s32)iVar3 + 0xDC);
                if (iVar3 != 0) {
                    *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;
                    if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                    }
                }
            }
        }
    }

    return 0;
}

u8 hitAtrOn(int param_1, u32 param_2, int param_3) {
    void* iVar1;
    void* iVar2;
    void* iVar3;
    u32 value = param_2;

    *(u32*)(param_1 + 0x04) = *(u32*)(param_1 + 0x04) | value;

    iVar3 = *(void**)(param_1 + 0xD8);
    if (iVar3 != 0) {
        *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;

        iVar2 = *(void**)((s32)iVar3 + 0xD8);
        if (iVar2 != 0) {
            *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;

            iVar1 = *(void**)((s32)iVar2 + 0xD8);
            if (iVar1 != 0) {
                *(u32*)((s32)iVar1 + 0x04) = *(u32*)((s32)iVar1 + 0x04) | value;
                if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                }
            }

            iVar2 = *(void**)((s32)iVar2 + 0xDC);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }
        }

        iVar3 = *(void**)((s32)iVar3 + 0xDC);
        if (iVar3 != 0) {
            *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;
                if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                }
            }
        }
    }

    if (param_3 != 0) {
        iVar3 = *(void**)(param_1 + 0xDC);
        if (iVar3 != 0) {
            *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;

                iVar1 = *(void**)((s32)iVar2 + 0xD8);
                if (iVar1 != 0) {
                    *(u32*)((s32)iVar1 + 0x04) = *(u32*)((s32)iVar1 + 0x04) | value;
                    if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                    }
                }

                iVar2 = *(void**)((s32)iVar2 + 0xDC);
                if (iVar2 != 0) {
                    *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;

                iVar2 = *(void**)((s32)iVar3 + 0xD8);
                if (iVar2 != 0) {
                    *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }

                iVar3 = *(void**)((s32)iVar3 + 0xDC);
                if (iVar3 != 0) {
                    *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;
                    if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                    }
                }
            }
        }
    }

    return 0;
}

void hitEntrySub(void* mapObj, s32 arg1, void* arg2, s32 arg3, s32 arg4) {
    ;
}


u8 hitMain(void) {
    return 0;
}


void hitObjGetPos(s32 name, void* pos) {
    ;
}


u8 hitDelete(char* param_1) {
    return 0;
}


int hitCheckAttr(double param_1, double param_2, double param_3, double param_4, double param_5, double param_6, s32 param_7, void* param_8, void* param_9, void* param_10, void* param_11, void* param_12, void* param_13, void* param_14) {
    return 0;
}


s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, s32 flags, f32* outZ, f32* outY, f32* outX, f32* outDist, void* outA, void* outB, void* outC) {
    return 0;
}


void hitBindUpdate(s32 name) {
    ;
}


u8 hitBindMapObj(char* param_1, char* param_2) {
    return 0;
}


u8 hitCheckVecHitObjXZ(void) {
    return 0;
}


u8 hitObjGetNormal(char* param_1, s32 param_2) {
    return 0;
}


u8 hitGrpAttrOff(char* param_1, u32 param_2) {
    return 0;
}


u8 hitGrpAttrOn(char* param_1, u32 param_2) {
    return 0;
}


u8 hitGrpFlagOff(char* param_1, s32 param_2) {
    return 0;
}


u8 hitGrpFlagOn(char* param_1, s32 param_2) {
    return 0;
}


u8 hitObjAttrOff(char* param_1, u32 param_2) {
    return 0;
}


u8 hitObjAttrOn(char* param_1, u32 param_2) {
    return 0;
}


u8 hitObjFlagOff(char* param_1, u16 param_2) {
    return 0;
}


u8 hitObjFlagOn(char* param_1, u16 param_2) {
    return 0;
}


void hitGrpDamageReturnSet(char* name, s32 value) {
    void* map;
    void* group;
    void* hit;
    s32 groupIndex;
    s32 hitIndex;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        hit = NULL;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) == 0 &&
                    strcmp(**(char***)((s32)hit + 0x8), name) == 0) {
                    goto found;
                }
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit != NULL) {
        hitDamageReturnSet((s32)hit, value, 0);
    }
}


u8 hitReInit(void) {
    return 0;
}


s32 hitGetName(void* pHit) {
    void* map;
    s32 groupCount;
    s32 groupIndex;

    if (pHit == NULL) {
        return 0;
    }

    map = mapGetWork();
    groupCount = *(s32*)map;
    groupIndex = 0;
    if (groupCount > 0) {
        do {
            void* hitBase = *(void**)((s32)map + 0x15C);
            u32 size = (*(s32*)((s32)map + 0x158) + 0x80) * 0xE4;
            size = (size + 0x1F) & ~0x1F;
            if ((u32)pHit >= (u32)hitBase && (u32)pHit < (u32)hitBase + size) {
                break;
            }
            map = (void*)((s32)map + 0x178);
            groupIndex++;
        } while (groupIndex < groupCount);
    }

    if (groupIndex >= groupCount) {
        return 0;
    }
    return **(s32**)((s32)pHit + 0x8);
}


void* hitEntry(void* param_1, void* param_2, int idx) {
    return 0;
}


void* _hitEnt(void* param_1, s32 param_2, s32 param_3, int param_4) {
    extern void* mapGetWork(void);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSVECAdd(void* a, void* b, void* out);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern f32 PSVECDistance(void* a, void* b);
    extern void* _mapAlloc(void* heap, s32 size);
    extern void* mapalloc_base_ptr;
    extern f32 float_deg2rad_8041f850;
    extern f32 float_0p5_8041f84c;
    extern f32 float_0p1_8041f83c;

    void* map;
    void* group;
    void* hit;
    void* part;
    void* poly;
    void* alloc;
    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    f32 rotXMtx[3][4];
    f32 rotYMtx[3][4];
    f32 rotZMtx[3][4];
    f32 tempVec[3];
    f32 pad;
    s32 groupOffset;
    s32 hitCount;
    s32 partCount;
    s32 triCount;
    s32 partIndex;
    s32 polyIndex;
    s32 polyCount;
    s32 unroll;
    s32 remaining;

    map = mapGetWork();
    groupOffset = param_4 * 0x178;
    group = (void*)((s32)map + groupOffset);
    hitCount = *(s32*)((s32)group + 0x158);
    hit = *(void**)((s32)group + 0x15C);

    if (hitCount > 0) {
        do {
            if (*(void**)((s32)hit + 0x08) == 0) {
                break;
            }
            hit = (void*)((s32)hit + 0xE4);
            hitCount--;
        } while (hitCount != 0);
    }

    *(u16*)hit = 0;
    *(void**)((s32)hit + 0x08) = param_1;
    *(s16*)((s32)hit + 0xAA) = (s16)param_4;

    PSMTXTrans(
        transMtx,
        *(f32*)((s32)param_1 + 0x30),
        *(f32*)((s32)param_1 + 0x34),
        *(f32*)((s32)param_1 + 0x38)
    );
    PSMTXScale(
        scaleMtx,
        *(f32*)((s32)param_1 + 0x18),
        *(f32*)((s32)param_1 + 0x1C),
        *(f32*)((s32)param_1 + 0x20)
    );
    PSMTXRotRad(rotXMtx, 'x', float_deg2rad_8041f850 * *(f32*)((s32)param_1 + 0x24));
    PSMTXRotRad(rotYMtx, 'y', float_deg2rad_8041f850 * *(f32*)((s32)param_1 + 0x28));
    PSMTXRotRad(rotZMtx, 'z', float_deg2rad_8041f850 * *(f32*)((s32)param_1 + 0x2C));

    PSMTXConcat(transMtx, rotZMtx, transMtx);
    PSMTXConcat(transMtx, rotYMtx, transMtx);
    PSMTXConcat(transMtx, rotXMtx, transMtx);
    PSMTXConcat(transMtx, scaleMtx, (void*)((s32)hit + 0x3C));
    PSMTXConcat((void*)param_3, (void*)((s32)hit + 0x3C), (void*)((s32)hit + 0x0C));

    *(s32*)((s32)hit + 0xD4) = param_2;
    *(s32*)((s32)hit + 0xDC) = 0;
    *(s32*)((s32)hit + 0xD8) = 0;
    *(s32*)((s32)hit + 0xAC) = 0;

    if (*(void**)((s32)param_1 + 0x58) != 0) {
        *(u32*)((s32)hit + 0x04) = *(u32*)(*(s32*)((s32)param_1 + 0x58) + 0x08);
    } else {
        *(u32*)((s32)hit + 0x04) = 0;
    }

    if (*(s32*)((s32)param_1 + 0x5C) != 0) {
        PSVECAdd((void*)((s32)param_1 + 0x3C), (void*)((s32)param_1 + 0x48), (void*)((s32)hit + 0x9C));
        PSVECScale((void*)((s32)hit + 0x9C), (void*)((s32)hit + 0x9C), float_0p5_8041f84c);
    } else {
        *(u32*)((s32)hit + 0x9C) = *(u32*)((s32)param_1 + 0x30);
        *(u32*)((s32)hit + 0xA0) = *(u32*)((s32)param_1 + 0x34);
        *(u32*)((s32)hit + 0xA4) = *(u32*)((s32)param_1 + 0x38);
    }

    PSMTXMultVec((void*)((s32)hit + 0x0C), (void*)((s32)hit + 0x9C), (void*)((s32)hit + 0xC0));

    *(f32*)((s32)param_1 + 0x3C) -= float_0p1_8041f83c;
    *(f32*)((s32)param_1 + 0x40) -= float_0p1_8041f83c;
    *(f32*)((s32)param_1 + 0x44) -= float_0p1_8041f83c;
    *(f32*)((s32)param_1 + 0x48) += float_0p1_8041f83c;
    *(f32*)((s32)param_1 + 0x4C) += float_0p1_8041f83c;
    *(f32*)((s32)param_1 + 0x50) += float_0p1_8041f83c;

    PSMTXMultVec((void*)((s32)hit + 0x0C), (void*)((s32)param_1 + 0x48), tempVec);
    *(f32*)((s32)hit + 0xCC) = PSVECDistance(tempVec, (void*)((s32)hit + 0xC0));

    triCount = 0;
    partIndex = 0;
    partCount = *(s32*)((s32)param_1 + 0x5C);
    while (partIndex < partCount) {
        part = *(void**)((s32)param_1 + 0x64 + partIndex * 8);
        if (part != 0) {
            polyCount = *(s32*)((s32)part + 0x04);
            polyIndex = 0;
            if (*(u8*)((s32)part + 0x03) == 0) {
                while (polyIndex < polyCount) {
                    poly = *(void**)((s32)part + 0x10 + polyIndex * 4);
                    triCount += *(s32*)poly - 2;
                    polyIndex++;
                }
            } else {
                while (polyIndex < polyCount) {
                    poly = *(void**)((s32)part + 0x10 + polyIndex * 8);
                    triCount += *(u16*)((s32)poly + 1) - 2;
                    polyIndex++;
                }
            }
        }
        partIndex++;
    }

    if (triCount > 0) {
        alloc = _mapAlloc(mapalloc_base_ptr, triCount * 0x54);
        *(void**)((s32)hit + 0xAC) = alloc;
        *(s16*)((s32)hit + 0xA8) = (s16)hitCalcVtxPosition(hit);
    }

    return hit;
}

u8 hitFlgOff(void* param_1, s32 param_2, int param_3) {
    void* iVar1;
    void* iVar2;
    void* iVar3;
    u32 value = ~((u32)(u16)param_2);

    *(u16*)param_1 = *(u16*)param_1 & value;

    iVar3 = *(void**)((s32)param_1 + 0xD8);
    if (iVar3 != 0) {
        *(u16*)iVar3 = *(u16*)iVar3 & value;

        iVar2 = *(void**)((s32)iVar3 + 0xD8);
        if (iVar2 != 0) {
            *(u16*)iVar2 = *(u16*)iVar2 & value;

            iVar1 = *(void**)((s32)iVar2 + 0xD8);
            if (iVar1 != 0) {
                *(u16*)iVar1 = *(u16*)iVar1 & value;
                if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                    hitFlgOff(*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                    hitFlgOff(*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                }
            }

            iVar2 = *(void**)((s32)iVar2 + 0xDC);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 & value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitFlgOff(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitFlgOff(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }
        }

        iVar3 = *(void**)((s32)iVar3 + 0xDC);
        if (iVar3 != 0) {
            *(u16*)iVar3 = *(u16*)iVar3 & value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 & value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitFlgOff(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitFlgOff(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u16*)iVar3 = *(u16*)iVar3 & value;
                if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                    hitFlgOff(*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                    hitFlgOff(*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                }
            }
        }
    }

    if (param_3 != 0) {
        iVar3 = *(void**)((s32)param_1 + 0xDC);
        if (iVar3 != 0) {
            *(u16*)iVar3 = *(u16*)iVar3 & value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 & value;

                iVar1 = *(void**)((s32)iVar2 + 0xD8);
                if (iVar1 != 0) {
                    *(u16*)iVar1 = *(u16*)iVar1 & value;
                    if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                        hitFlgOff(*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                        hitFlgOff(*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                    }
                }

                iVar2 = *(void**)((s32)iVar2 + 0xDC);
                if (iVar2 != 0) {
                    *(u16*)iVar2 = *(u16*)iVar2 & value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitFlgOff(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitFlgOff(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u16*)iVar3 = *(u16*)iVar3 & value;

                iVar2 = *(void**)((s32)iVar3 + 0xD8);
                if (iVar2 != 0) {
                    *(u16*)iVar2 = *(u16*)iVar2 & value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitFlgOff(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitFlgOff(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }

                iVar3 = *(void**)((s32)iVar3 + 0xDC);
                if (iVar3 != 0) {
                    *(u16*)iVar3 = *(u16*)iVar3 & value;
                    if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                        hitFlgOff(*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                        hitFlgOff(*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                    }
                }
            }
        }
    }

    return 0;
}

u8 hitFlgOn(void* param_1, s32 param_2, int param_3) {
    void* iVar1;
    void* iVar2;
    void* iVar3;
    u32 value = param_2;

    *(u16*)param_1 = *(u16*)param_1 | value;

    iVar3 = *(void**)((s32)param_1 + 0xD8);
    if (iVar3 != 0) {
        *(u16*)iVar3 = *(u16*)iVar3 | value;

        iVar2 = *(void**)((s32)iVar3 + 0xD8);
        if (iVar2 != 0) {
            *(u16*)iVar2 = *(u16*)iVar2 | value;

            iVar1 = *(void**)((s32)iVar2 + 0xD8);
            if (iVar1 != 0) {
                *(u16*)iVar1 = *(u16*)iVar1 | value;
                if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                    hitFlgOn(*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                    hitFlgOn(*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                }
            }

            iVar2 = *(void**)((s32)iVar2 + 0xDC);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 | value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitFlgOn(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitFlgOn(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }
        }

        iVar3 = *(void**)((s32)iVar3 + 0xDC);
        if (iVar3 != 0) {
            *(u16*)iVar3 = *(u16*)iVar3 | value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 | value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitFlgOn(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitFlgOn(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u16*)iVar3 = *(u16*)iVar3 | value;
                if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                    hitFlgOn(*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                    hitFlgOn(*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                }
            }
        }
    }

    if (param_3 != 0) {
        iVar3 = *(void**)((s32)param_1 + 0xDC);
        if (iVar3 != 0) {
            *(u16*)iVar3 = *(u16*)iVar3 | value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 | value;

                iVar1 = *(void**)((s32)iVar2 + 0xD8);
                if (iVar1 != 0) {
                    *(u16*)iVar1 = *(u16*)iVar1 | value;
                    if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                        hitFlgOn(*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                        hitFlgOn(*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                    }
                }

                iVar2 = *(void**)((s32)iVar2 + 0xDC);
                if (iVar2 != 0) {
                    *(u16*)iVar2 = *(u16*)iVar2 | value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitFlgOn(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitFlgOn(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u16*)iVar3 = *(u16*)iVar3 | value;

                iVar2 = *(void**)((s32)iVar3 + 0xD8);
                if (iVar2 != 0) {
                    *(u16*)iVar2 = *(u16*)iVar2 | value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitFlgOn(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitFlgOn(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }

                iVar3 = *(void**)((s32)iVar3 + 0xDC);
                if (iVar3 != 0) {
                    *(u16*)iVar3 = *(u16*)iVar3 | value;
                    if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                        hitFlgOn(*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                        hitFlgOn(*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                    }
                }
            }
        }
    }

    return 0;
}

void* hitNameToPtr(char* hitName) {
    void* map;
    void* group;
    void* hit;
    s32 groupIndex;
    s32 hitIndex;

    map = mapGetWork();
    if (hitName == NULL) {
        return NULL;
    }

    group = map;
    groupIndex = 0;
    while (groupIndex < *(s32*)map) {
        hit = *(void**)((s32)group + 0x15C);
        hitIndex = 0;
        while (hitIndex < *(s32*)((s32)group + 0x158)) {
            if ((*(u16*)hit & 0x80) == 0 &&
                strcmp(**(char***)((s32)hit + 0x8), hitName) == 0) {
                return hit;
            }
            hitIndex++;
            hit = (void*)((s32)hit + 0xE4);
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }
    return NULL;
}
