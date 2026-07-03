#include "driver/mapdrv.h"

void* getMapDataDvdRoot(void) {
    extern char str__8041f928[1];
    return str__8041f928;
}

s32 mapGetActiveGroup(void) {
    extern s32 activeGroup;
    return activeGroup;
}

s32 mapPreLoad(void) {
    return 1;
}

void N_mapDispOff(void) {
    extern s32 unk_8041e678;
    unk_8041e678 = 1;
}

void N_mapDispOn(void) {
    extern s32 unk_8041e678;
    unk_8041e678 = 0;
}

void mapResetPaperAmbColor(void) {
    extern s32 dat_8041f90c;
    extern s32 paper_ambient;
    paper_ambient = dat_8041f90c;
}

void mapSetPaperAmbColor(void* color) {
    extern s32 paper_ambient;
    paper_ambient = *(s32*)color;
}

void* mapGetWork(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    return (void*)(mapWork + activeGroup * 0x2F4);
}

void mapDispMapGrp(void) {
    extern void _mapDispMapGrp(void);
    _mapDispMapGrp();
}

void mapDispMapObj(void) {
    extern void _mapDispMapObj(void);
    _mapDispMapObj();
}

void mapSetTevCallback(s32 index, void* callback) {
    extern s32 activeGroup;
    extern s32 mapWork;
    *(void**)(mapWork + activeGroup * 0x2F4 + 0x110 + index * 4) = callback;
}

void mapGrpFlushOff(s32 group) {
    extern void mapGrpFlagOff(s32, u32);
    mapGrpFlagOff(group, 0x100000);
}

void mapGrpFlushOn(s32 group) {
    extern void mapGrpFlagOn(s32, u32);
    mapGrpFlagOn(group, 0x100000);
}

void mapSetProjMtx(void* mtx) {
    extern void PSMTXCopy(void*, void*);
    extern s32 projMtx[12];
    PSMTXCopy(mtx, projMtx);
}

void mapSetBlend(void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    s32 work = mapWork + activeGroup * 0x2F4;
    *(s32*)(work + 0x16) = *(s32*)color;
    *(u16*)(work + 4) |= 2;
}

void mapSetBlend2(void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    s32 work = mapWork + activeGroup * 0x2F4;
    *(s32*)(work + 0x1A) = *(s32*)color;
    *(u16*)(work + 4) |= 4;
}

void mapGetBlend(void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 dat_8041f920;
    s32 work = mapWork + activeGroup * 0x2F4;
    if (*(u16*)(work + 4) & 2) {
        *(s32*)color = *(s32*)(work + 0x16);
    } else {
        *(s32*)color = dat_8041f920;
    }
}

void mapGetBlend2(void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 dat_8041f924;
    s32 work = mapWork + activeGroup * 0x2F4;
    if (*(u16*)(work + 4) & 4) {
        *(s32*)color = *(s32*)(work + 0x1A);
    } else {
        *(s32*)color = dat_8041f924;
    }
}

void mapGetFog(void* type, f32* start, f32* end, void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    s32 work = mapWork + activeGroup * 0x2F4;
    *(s32*)type = *(s32*)(work + 0x28);
    *start = *(f32*)(work + 0x2C);
    *end = *(f32*)(work + 0x30);
    *(s32*)color = *(s32*)(work + 0x34);
}

void mapInit(void) {
    extern void* __memAlloc(s32, u32);
    extern void* memset(void*, s32, u32);
    extern void* mapWork;
    extern s32 activeGroup;
    extern s32 mapClipOffFlag;
    mapWork = __memAlloc(0, 0x5E8);
    memset(mapWork, 0, 0x5E8);
    activeGroup = 0;
    mapClipOffFlag = 0;
}

s32 mapGetJoints(void* data) {
    extern void mapGetJointsSub(void*, s32*);
    s32 count = 0;
    if (data != 0) {
        count = 1;
        data = *(void**)((s32)data + 0xC);
        if (data != 0) {
            mapGetJointsSub(data, &count);
        }
    }
    return count;
}

void mapDispMapGrp_bbox(s32 group, void* obj) {
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void _mapDispMapGrp_NoMaterial(s32, void*);
    extern void GXReadBoundingBox(void*, void*, void*, void*);
    sysWaitDrawSync();
    GXClearBoundingBox();
    _mapDispMapGrp_NoMaterial(group, obj);
    sysWaitDrawSync();
    GXReadBoundingBox((void*)((s32)obj + 0xEC), (void*)((s32)obj + 0xEE),
                      (void*)((s32)obj + 0xF0), (void*)((s32)obj + 0xF2));
}

void* mapSearchDmdJoint(void* data, char* name) {
    extern void* mapSearchDmdJointSub(void*, char*);
    extern void* mapSearchDmdJointSub2(void*, char*, char*);
    extern char str__8041f94c[1];
    if ((s32)*(u8*)name != '|') {
        return mapSearchDmdJointSub(data, name);
    }
    return mapSearchDmdJointSub2(data, name, str__8041f94c);
}

void mapDispMapGrp_off(s32 group, void* obj) {
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void _mapDispMapGrp(s32, void*);
    extern void GXReadBoundingBox(void*, void*, void*, void*);
    extern void offscreenAddBoundingBox(s32, u16, u16, u16, u16);
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
    sysWaitDrawSync();
    GXClearBoundingBox();
    _mapDispMapGrp(group, obj);
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(*(s16*)((s32)obj + 0xDC), left, top, right, bottom);
}

void mapDispMapObj_off(s32 group, void* obj) {
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void _mapDispMapObj(s32, void*);
    extern void GXReadBoundingBox(void*, void*, void*, void*);
    extern void offscreenAddBoundingBox(s32, u16, u16, u16, u16);
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
    sysWaitDrawSync();
    GXClearBoundingBox();
    _mapDispMapObj(group, obj);
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(*(s16*)((s32)obj + 0xDC), left, top, right, bottom);
}

void mapSetProjTexObj(void* texObj, void* imgRef) {
    typedef struct TexObjCopy {
        s32 word[8];
    } TexObjCopy;
    extern TexObjCopy projTexObj;
    extern void* projTexImgRef;
    projTexObj = *(TexObjCopy*)texObj;
    projTexImgRef = imgRef;
}

void mapGrpSetFlushColor(s32 group, u8 r, u8 g, u8 b, u8 a) {
    extern s32 unk_80429530;
    extern void mapGrpSetColor(s32, void*);
    extern void mapGrpFlagOff(s32, u32);
    s32 copy;
    s32 color;
    color = unk_80429530;
    ((u8*)&color)[0] = r;
    ((u8*)&color)[1] = g;
    ((u8*)&color)[2] = b;
    ((u8*)&color)[3] = a;
    copy = color;
    mapGrpSetColor(group, &copy);
    mapGrpFlagOff(group, 0x40);
}


void mapSetMaterialTev(u32 texCount, int drawMode, u32 materialFlag, s32 pMtx) {
    ;
}


u8 mapMain(void) {
    return 0;
}


void _mapDispMapObj(void) {
    ;
}


u8 mapDisp(void) {
    return 0;
}


u8 spline_maketable(int param_1, float* param_2, float* param_3, float* param_4) {
    return 0;
}


void* mapSearchDmdJointSub(void* param_1, char* param_2) {
    extern s32 strcmp(const char* a, const char* b);

    void* pMVar5;
    char* target;
    void* root;
    void* pMVar4;
    void* pMVar3;
    void* pMVar2;

#define JOINT_NAME(j_) (*(char**)((s32)(j_) + 0x00))
#define JOINT_CHILD(j_) (*(void**)((s32)(j_) + 0x0C))
#define JOINT_SIB(j_) (*(void**)((s32)(j_) + 0x10))

    root = param_1;
    target = param_2;

    if (strcmp(JOINT_NAME(root), target) == 0) {
        return root;
    }

    pMVar5 = JOINT_CHILD(root);
    if (pMVar5 != 0) {
        if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
            pMVar4 = JOINT_CHILD(pMVar5);
            if (pMVar4 != 0) {
                if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                    pMVar3 = JOINT_CHILD(pMVar4);
                    if (pMVar3 != 0) {
                        if (strcmp(JOINT_NAME(pMVar3), target) != 0) {
                            if (JOINT_CHILD(pMVar3) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar3), target);
                                if (pMVar2 != 0) {
                                    pMVar3 = pMVar2;
                                    goto first_grand_done;
                                }
                            }
                            if (JOINT_SIB(pMVar3) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar3), target);
                                if (pMVar2 != 0) {
                                    pMVar3 = pMVar2;
                                    goto first_grand_done;
                                }
                            }
                            pMVar3 = 0;
                        }

first_grand_done:
                        if (pMVar3 != 0) {
                            pMVar4 = pMVar3;
                            goto first_child_done;
                        }
                    }

                    pMVar4 = JOINT_SIB(pMVar4);
                    if (pMVar4 != 0) {
                        if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                            if (JOINT_CHILD(pMVar4) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar4), target);
                                if (pMVar2 != 0) {
                                    pMVar4 = pMVar2;
                                    goto first_child_sib_done;
                                }
                            }
                            if (JOINT_SIB(pMVar4) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar4), target);
                                if (pMVar2 != 0) {
                                    pMVar4 = pMVar2;
                                    goto first_child_sib_done;
                                }
                            }
                            pMVar4 = 0;
                        }

first_child_sib_done:
                        if (pMVar4 != 0) {
                            goto first_child_done;
                        }
                    }
                    pMVar4 = 0;
                }

first_child_done:
                if (pMVar4 != 0) {
                    pMVar5 = pMVar4;
                    goto first_top_done;
                }
            }

            pMVar5 = JOINT_SIB(pMVar5);
            if (pMVar5 != 0) {
                if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
                    pMVar4 = JOINT_CHILD(pMVar5);
                    if (pMVar4 != 0) {
                        if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                            if (JOINT_CHILD(pMVar4) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar4), target);
                                if (pMVar2 != 0) {
                                    pMVar4 = pMVar2;
                                    goto first_top_sib_child_done;
                                }
                            }
                            if (JOINT_SIB(pMVar4) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar4), target);
                                if (pMVar2 != 0) {
                                    pMVar4 = pMVar2;
                                    goto first_top_sib_child_done;
                                }
                            }
                            pMVar4 = 0;
                        }

first_top_sib_child_done:
                        if (pMVar4 != 0) {
                            pMVar5 = pMVar4;
                            goto first_top_sib_done;
                        }
                    }

                    pMVar5 = JOINT_SIB(pMVar5);
                    if (pMVar5 != 0) {
                        if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
                            if (JOINT_CHILD(pMVar5) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar5), target);
                                if (pMVar2 != 0) {
                                    pMVar5 = pMVar2;
                                    goto first_top_sib_sib_done;
                                }
                            }
                            if (JOINT_SIB(pMVar5) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar5), target);
                                if (pMVar2 != 0) {
                                    pMVar5 = pMVar2;
                                    goto first_top_sib_sib_done;
                                }
                            }
                            pMVar5 = 0;
                        }

first_top_sib_sib_done:
                        if (pMVar5 != 0) {
                            goto first_top_sib_done;
                        }
                    }
                    pMVar5 = 0;
                }

first_top_sib_done:
                if (pMVar5 != 0) {
                    goto first_top_done;
                }
            }
            pMVar5 = 0;
        }

first_top_done:
        if (pMVar5 != 0) {
            return pMVar5;
        }
    }

    pMVar5 = JOINT_SIB(root);
    if (pMVar5 == 0) {
        return 0;
    }

    if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
        pMVar4 = JOINT_CHILD(pMVar5);
        if (pMVar4 != 0) {
            if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                pMVar3 = JOINT_CHILD(pMVar4);
                if (pMVar3 != 0) {
                    if (strcmp(JOINT_NAME(pMVar3), target) != 0) {
                        if (JOINT_CHILD(pMVar3) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar3), target);
                            if (pMVar2 != 0) {
                                pMVar3 = pMVar2;
                                goto sib_grand_done;
                            }
                        }
                        if (JOINT_SIB(pMVar3) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar3), target);
                            if (pMVar2 != 0) {
                                pMVar3 = pMVar2;
                                goto sib_grand_done;
                            }
                        }
                        pMVar3 = 0;
                    }

sib_grand_done:
                    if (pMVar3 != 0) {
                        pMVar4 = pMVar3;
                        goto sib_child_done;
                    }
                }

                pMVar4 = JOINT_SIB(pMVar4);
                if (pMVar4 != 0) {
                    if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                        if (JOINT_CHILD(pMVar4) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar4), target);
                            if (pMVar2 != 0) {
                                pMVar4 = pMVar2;
                                goto sib_child_sib_done;
                            }
                        }
                        if (JOINT_SIB(pMVar4) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar4), target);
                            if (pMVar2 != 0) {
                                pMVar4 = pMVar2;
                                goto sib_child_sib_done;
                            }
                        }
                        pMVar4 = 0;
                    }

sib_child_sib_done:
                    if (pMVar4 != 0) {
                        goto sib_child_done;
                    }
                }
                pMVar4 = 0;
            }

sib_child_done:
            if (pMVar4 != 0) {
                pMVar5 = pMVar4;
                goto sib_done;
            }
        }

        pMVar5 = JOINT_SIB(pMVar5);
        if (pMVar5 != 0) {
            if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
                pMVar4 = JOINT_CHILD(pMVar5);
                if (pMVar4 != 0) {
                    if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                        if (JOINT_CHILD(pMVar4) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar4), target);
                            if (pMVar2 != 0) {
                                pMVar4 = pMVar2;
                                goto sib_sib_child_done;
                            }
                        }
                        if (JOINT_SIB(pMVar4) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar4), target);
                            if (pMVar2 != 0) {
                                pMVar4 = pMVar2;
                                goto sib_sib_child_done;
                            }
                        }
                        pMVar4 = 0;
                    }

sib_sib_child_done:
                    if (pMVar4 != 0) {
                        pMVar5 = pMVar4;
                        goto sib_sib_done;
                    }
                }

                pMVar5 = JOINT_SIB(pMVar5);
                if (pMVar5 != 0) {
                    if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
                        if (JOINT_CHILD(pMVar5) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar5), target);
                            if (pMVar2 != 0) {
                                pMVar5 = pMVar2;
                                goto sib_sib_sib_done;
                            }
                        }
                        if (JOINT_SIB(pMVar5) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar5), target);
                            if (pMVar2 != 0) {
                                pMVar5 = pMVar2;
                                goto sib_sib_sib_done;
                            }
                        }
                        pMVar5 = 0;
                    }

sib_sib_sib_done:
                    if (pMVar5 != 0) {
                        goto sib_sib_done;
                    }
                }
                pMVar5 = 0;
            }

sib_sib_done:
            if (pMVar5 != 0) {
                goto sib_done;
            }
        }
        pMVar5 = 0;
    }

sib_done:
    if (pMVar5 == 0) {
        return 0;
    }
    return pMVar5;

#undef JOINT_NAME
#undef JOINT_CHILD
#undef JOINT_SIB
}

u8 mapSetMaterialLight(u32 materialLightFlag, void* pPos) {
    extern void* lightGetPaper(void);
    extern void* lightGetPaperCraft(void);
    extern s32 lightCheckCharaLight(void);
    extern s32 lightGetNearObj(void* pPos, void** pDst, s32 count, s32 flags);
    extern void* lightGetCharaAmbient(void);
    extern void* camGetCurPtr(void);

    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTXMultVecSR(void* mtx, void* src, void* dst);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSVECAdd(void* a, void* b, void* out);

    extern void GXInitLightPos(void* light, f32 x, f32 y, f32 z);
    extern void GXInitLightDir(void* light, f32 x, f32 y, f32 z);
    extern void GXInitLightSpot(void* light, f32 angle, s32 fn);
    extern void GXInitLightDistAttn(void* light, f32 refDistance, f32 refBrightness, s32 fn);
    extern void GXInitLightColor(void* light, void* color);
    extern void GXInitLightAttn(void* light, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2);
    extern void GXLoadLightObjImm(void* light, u32 id);
    extern void GXSetFog(s32 type, f32 start, f32 end, f32 nearZ, f32 farZ, void* color);
    extern void GXSetNumChans(u32 nChans);
    extern void GXSetChanAmbColor(s32 chan, void* color);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, u32 lightMask, u32 diffFn, s32 attnFn);

    extern u32 dat_8041f914;
    extern u32 dat_8041f918;
    extern u32 dat_8041f91c;
    extern u32 paper_ambient;
    extern u32 paperCraft_ambient;
    extern u32 lightid_tbl[];
    extern u32 vec3_802bfaa8[];
    extern f32 float_0_8041f930;
    extern f32 float_1_8041f940;
    extern f32 float_deg2rad_8041f948;
    extern f32 float_neg1p0486E06_8041f950;
    extern f32 float_0p5_8041f934;
    extern f32 float_1000_8041f954;

    u32 matColor;
    u32 ambColor;
    u32 charAmbColor;
    u32 fogColor;
    u32 tmpColor;
    f32 dir[3];
    f32 pos[3];
    void* lights[8];
    f32 mtxZ[3][4];
    f32 mtxY[3][4];
    f32 mtxX[3][4];
    u32 lightObj[16];

    u32 paperFlag;
    u32 craftFlag;
    u32 lightMask;
    u32* lightId;
    void** lightEntryPtr;
    void* light;
    void* cam;
    u16 flags;
    s32 lightNum;
    s32 i;
    s32 attnFn;
    s32 enable;

#define F32_AT(p_, o_) (*(f32*)((s32)(p_) + (o_)))
#define U16_AT(p_, o_) (*(u16*)((s32)(p_) + (o_)))
#define U32_AT(p_, o_) (*(u32*)((s32)(p_) + (o_)))
#define BUILD_DIR(light_)                                                        \
    do {                                                                        \
        PSMTXRotRad(mtxX, 'x', float_deg2rad_8041f948 * F32_AT((light_), 0x30)); \
        PSMTXRotRad(mtxY, 'y', float_deg2rad_8041f948 * F32_AT((light_), 0x34)); \
        PSMTXRotRad(mtxZ, 'z', float_deg2rad_8041f948 * F32_AT((light_), 0x38)); \
        PSMTXConcat(mtxZ, mtxY, mtxY);                                           \
        PSMTXConcat(mtxY, mtxX, mtxX);                                           \
        PSMTXMultVec(mtxX, dir, dir);                                            \
    } while (0)

    paperFlag = materialLightFlag & 1;
    attnFn = 2;
    matColor = dat_8041f914;
    ambColor = dat_8041f918;

    if (paperFlag != 0) {
        lights[0] = lightGetPaper();
    }
    lightNum = (paperFlag != 0);

    craftFlag = materialLightFlag & 8;
    if (craftFlag != 0) {
        lights[lightNum] = lightGetPaperCraft();
        lightNum++;
    }

    if ((materialLightFlag & 0x10) != 0) {
        lights[lightNum] = lightGetPaper();
        lightNum++;
    }

    if ((materialLightFlag & 2) != 0) {
        if (lightCheckCharaLight() != 0) {
            if (lightNum < 8) {
                lightNum += lightGetNearObj(pPos, &lights[lightNum], 8 - lightNum, 0x80);
            }
        }
    }

    if ((materialLightFlag & 0x19) != 0) {
        if (lightNum == 1) {
            if (paperFlag != 0) {
                ambColor = paper_ambient;
            } else if (craftFlag != 0) {
                ambColor = paperCraft_ambient;
            } else {
                ambColor = dat_8041f91c;
            }
        } else {
            lightNum = lightGetNearObj(pPos, lights, 8, 0x80);
        }
    }

    lightId = lightid_tbl;
    lightEntryPtr = lights;
    lightMask = 0;
    i = 0;

    while (i < lightNum) {
        ((u32*)dir)[0] = vec3_802bfaa8[0];
        light = *lightEntryPtr;
        ((u32*)dir)[1] = vec3_802bfaa8[1];
        ((u32*)dir)[2] = vec3_802bfaa8[2];

        flags = U16_AT(light, 0);
        if ((flags & 0x1000) != 0) {
            cam = camGetCurPtr();
            PSMTXMultVec((void*)((s32)cam + 0x11C), (void*)((s32)light + 0x24), pos);

            GXInitLightPos(lightObj, pos[0], pos[1], pos[2]);
            tmpColor = U32_AT(light, 0x48);
            GXInitLightColor(lightObj, &tmpColor);

            if ((U16_AT(light, 0) & 0x40) != 0) {
                GXInitLightAttn(lightObj, F32_AT(light, 0x50), float_0_8041f930, float_0_8041f930,
                                float_1_8041f940, float_0_8041f930, float_0_8041f930);
            } else {
                GXInitLightAttn(lightObj, F32_AT(light, 0x50), float_0_8041f930, float_0_8041f930,
                                F32_AT(light, 0x54), F32_AT(light, 0x58), F32_AT(light, 0x5C));
                attnFn = 1;
            }

            GXLoadLightObjImm(lightObj, *lightId);
        } else if ((flags & 0x2000) != 0) {
            BUILD_DIR(light);
            PSVECScale(dir, dir, float_neg1p0486E06_8041f950);
            PSVECAdd((void*)((s32)light + 0x24), dir, pos);

            cam = camGetCurPtr();
            PSMTXMultVec((void*)((s32)cam + 0x11C), pos, pos);

            GXInitLightPos(lightObj, pos[0], pos[1], pos[2]);
            tmpColor = U32_AT(light, 0x48);
            GXInitLightColor(lightObj, &tmpColor);
            GXLoadLightObjImm(lightObj, *lightId);
        } else {
            if ((flags & 0x4000) != 0) {
                cam = camGetCurPtr();
                PSMTXMultVec((void*)((s32)cam + 0x11C), (void*)((s32)light + 0x24), pos);

                BUILD_DIR(light);

                cam = camGetCurPtr();
                PSMTXMultVecSR((void*)((s32)cam + 0x11C), dir, dir);

                GXInitLightPos(lightObj, pos[0], pos[1], pos[2]);
                GXInitLightDir(lightObj, dir[0], dir[1], dir[2]);
                GXInitLightSpot(lightObj, F32_AT(light, 0x4C) * float_0p5_8041f934, 4);
                GXInitLightDistAttn(lightObj, float_1000_8041f954, float_0p5_8041f934, 0);

                tmpColor = U32_AT(light, 0x48);
                GXInitLightColor(lightObj, &tmpColor);
                GXLoadLightObjImm(lightObj, *lightId);
                attnFn = 1;
            }
        }

        lightMask |= *lightId;
        lightEntryPtr++;
        lightId++;
        i++;
    }

    light = lightGetCharaAmbient();
    if (light == 0) {
        charAmbColor = ambColor;
    } else {
        charAmbColor = U32_AT(light, 0x48);
    }

    fogColor = matColor;
    GXSetFog(0, float_0_8041f930, float_0_8041f930, float_0_8041f930, float_0_8041f930, &fogColor);
    GXSetNumChans(1);

    tmpColor = charAmbColor;
    GXSetChanAmbColor(4, &tmpColor);

    enable = ((u32)(-lightNum) & ~((u32)lightNum)) >> 31;
    if ((materialLightFlag & 4) != 0) {
        tmpColor = matColor;
        GXSetChanMatColor(4, &tmpColor);
        GXSetChanCtrl(0, enable, 0, 0, lightMask, 2, attnFn);
        GXSetChanCtrl(2, 0, 0, 0, 0, 2, 2);
    } else {
        GXSetChanCtrl(0, enable, 0, 1, lightMask, 2, attnFn);
        GXSetChanCtrl(2, 0, 0, 1, 0, 2, 2);
    }

#undef F32_AT
#undef U16_AT
#undef U32_AT
#undef BUILD_DIR
}

u8 mapObjGetPosSub(int param_1, s32 param_2, int* param_3, int param_4) {
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSVECAdd(void* a, void* b, void* out);

    u32 iVar1;
    u32 iVar2;
    u32 iVar3;
    f32 VStack_d8[3];
    f32 VStack_cc[3];
    f32 VStack_c0[3];
    f32 VStack_b4[3];
    f32 VStack_a8[3];
    f32 VStack_9c[3];
    f32 VStack_90[3];
    f32 VStack_84[3];
    f32 VStack_78[3];
    f32 VStack_6c[3];
    f32 VStack_60[3];
    f32 VStack_54[3];
    f32 VStack_48[3];
    f32 VStack_3c[3];
    f32 VStack_30[3];

#define DO_LEAF(node_, vec_)                                      \
    do {                                                          \
        PSMTXMultVec((void*)((node_) + 0x1C),                     \
                     (void*)((node_) + 0x10),                     \
                     (vec_));                                     \
        PSVECAdd((void*)param_2, (vec_), (void*)param_2);         \
        *param_3 = *param_3 + 1;                                  \
    } while (0)

    iVar3 = *(u32 *)(param_1 + 0xE4);
    if (iVar3 != 0) {
        iVar2 = *(u32 *)(iVar3 + 0xE4);
        if (iVar2 != 0) {
            iVar1 = *(u32 *)(iVar2 + 0xE4);
            if (iVar1 != 0) {
                if (*(u32 *)(iVar1 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar1 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar1, VStack_84);
                }
                if (*(u32 *)(iVar1 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar1 + 0xE8), param_2, param_3, 1);
                }
            } else {
                DO_LEAF(iVar2, VStack_54);
            }

            iVar2 = *(u32 *)(iVar2 + 0xE8);
            if (iVar2 != 0) {
                if (*(u32 *)(iVar2 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar2, VStack_90);
                }
                if (*(u32 *)(iVar2 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE8), param_2, param_3, 1);
                }
            }
        } else {
            DO_LEAF(iVar3, VStack_3c);
        }

        iVar3 = *(u32 *)(iVar3 + 0xE8);
        if (iVar3 != 0) {
            iVar2 = *(u32 *)(iVar3 + 0xE4);
            if (iVar2 != 0) {
                if (*(u32 *)(iVar2 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar2, VStack_9c);
                }
                if (*(u32 *)(iVar2 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE8), param_2, param_3, 1);
                }
            } else {
                DO_LEAF(iVar3, VStack_60);
            }

            iVar3 = *(u32 *)(iVar3 + 0xE8);
            if (iVar3 != 0) {
                if (*(u32 *)(iVar3 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar3 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar3, VStack_a8);
                }
                if (*(u32 *)(iVar3 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar3 + 0xE8), param_2, param_3, 1);
                }
            }
        }
    } else {
        DO_LEAF(param_1, VStack_30);
    }

    if ((param_4 != 0) && ((iVar3 = *(u32 *)(param_1 + 0xE8)) != 0)) {
        iVar2 = *(u32 *)(iVar3 + 0xE4);
        if (iVar2 != 0) {
            iVar1 = *(u32 *)(iVar2 + 0xE4);
            if (iVar1 != 0) {
                if (*(u32 *)(iVar1 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar1 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar1, VStack_b4);
                }
                if (*(u32 *)(iVar1 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar1 + 0xE8), param_2, param_3, 1);
                }
            } else {
                DO_LEAF(iVar2, VStack_6c);
            }

            iVar2 = *(u32 *)(iVar2 + 0xE8);
            if (iVar2 != 0) {
                if (*(u32 *)(iVar2 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar2, VStack_c0);
                }
                if (*(u32 *)(iVar2 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE8), param_2, param_3, 1);
                }
            }
        } else {
            DO_LEAF(iVar3, VStack_48);
        }

        iVar3 = *(u32 *)(iVar3 + 0xE8);
        if (iVar3 != 0) {
            iVar2 = *(u32 *)(iVar3 + 0xE4);
            if (iVar2 != 0) {
                if (*(u32 *)(iVar2 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar2, VStack_cc);
                }
                if (*(u32 *)(iVar2 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE8), param_2, param_3, 1);
                }
            } else {
                DO_LEAF(iVar3, VStack_78);
            }

            iVar3 = *(u32 *)(iVar3 + 0xE8);
            if (iVar3 != 0) {
                if (*(u32 *)(iVar3 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar3 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar3, VStack_d8);
                }
                if (*(u32 *)(iVar3 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar3 + 0xE8), param_2, param_3, 1);
                }
            }
        }
    }

#undef DO_LEAF
}

void _mapDispMapGrp_NoMaterial(s32 camId, void* grp) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXInvXpose(void* src, void* dst);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXLoadNrmMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern u8 mapSetPolygon(int grp, int polygon);
    extern void _mapDispMapObj_NoMaterial(s32 camId, void* obj);
    extern u32 unk_80429524;

    void* cam;
    void* child;
    void* next;
    void* obj;
    s32 i;
    s32 offset;
    u32 color0;
    u32 color1;
    u32 color2;
    f32 nrm0[3][4];
    f32 mtx0[3][4];
    f32 nrm1[3][4];
    f32 mtx1[3][4];
    f32 nrm2[3][4];
    f32 mtx2[3][4];

#define SETUP_NOMAT(node_, mtx_, nrm_, color_)                                      \
    do {                                                                            \
        cam = camGetPtr(camId);                                                     \
        PSMTXConcat((void*)((s32)cam + 0x11C), (void*)((s32)(node_) + 0x1C), (mtx_)); \
        GXLoadPosMtxImm((mtx_), 0);                                                 \
        PSMTXInvXpose((mtx_), (nrm_));                                              \
        GXLoadNrmMtxImm((nrm_), 0);                                                 \
        GXSetCurrentMtx(0);                                                         \
        GXSetCullMode(3);                                                           \
        GXSetNumChans(1);                                                           \
        (color_) = unk_80429524;                                                    \
        GXSetChanMatColor(4, &(color_));                                            \
        GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);                                         \
        GXSetNumTexGens(0);                                                         \
        GXSetNumTevStages(1);                                                       \
        GXSetTevOrder(0, 0xFF, 0xFF, 4);                                            \
        GXSetTevOp(0, 4);                                                           \
        offset = 0;                                                                 \
        i = 0;                                                                      \
        while (i < *(s32*)(*(s32*)((s32)(node_) + 0x08) + 0x5C)) {                  \
            mapSetPolygon((int)(node_), *(int*)(*(s32*)((s32)(node_) + 0x08) + offset + 0x64)); \
            offset += 8;                                                            \
            i++;                                                                    \
        }                                                                           \
    } while (0)

#define DRAW_OBJ_AND_LINKS(obj_)                                                    \
    do {                                                                            \
        _mapDispMapObj_NoMaterial(camId, (obj_));                                   \
        next = *(void**)((s32)(obj_) + 0xE4);                                       \
        if (next != 0) {                                                            \
            _mapDispMapGrp_NoMaterial(camId, next);                                 \
        }                                                                           \
        if ((*(u32*)(obj_) & 0x20) != 0) {                                          \
            next = *(void**)((s32)(obj_) + 0xE8);                                   \
            if (next != 0) {                                                        \
                _mapDispMapGrp_NoMaterial(camId, next);                             \
            }                                                                       \
        }                                                                           \
    } while (0)

    SETUP_NOMAT(grp, mtx0, nrm0, color0);

    child = *(void**)((s32)grp + 0xE4);
    if (child != 0) {
        SETUP_NOMAT(child, mtx1, nrm1, color1);

        obj = *(void**)((s32)child + 0xE4);
        if (obj != 0) {
            DRAW_OBJ_AND_LINKS(obj);
        }

        if ((*(u32*)child & 0x20) != 0) {
            obj = *(void**)((s32)child + 0xE8);
            if (obj != 0) {
                DRAW_OBJ_AND_LINKS(obj);
            }
        }
    }

    if ((*(u32*)grp & 0x20) != 0) {
        child = *(void**)((s32)grp + 0xE8);
        if (child != 0) {
            SETUP_NOMAT(child, mtx2, nrm2, color2);

            obj = *(void**)((s32)child + 0xE4);
            if (obj != 0) {
                DRAW_OBJ_AND_LINKS(obj);
            }

            if ((*(u32*)child & 0x20) != 0) {
                obj = *(void**)((s32)child + 0xE8);
                if (obj != 0) {
                    DRAW_OBJ_AND_LINKS(obj);
                }
            }
        }
    }

#undef DRAW_OBJ_AND_LINKS
#undef SETUP_NOMAT
}

u8 mapSetPolygon(int param_1, int param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 culling[];
    extern s32 attr_fmt[];
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxAttrFmtv(s32 vtxfmt, void* fmtv);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 cnt, s32 type, s32 frac);
    extern void GXCallDisplayList(void* list, u32 nbytes);

    s32 obj;
    s32 poly;
    s32 groupBase;
    s32 texCount;
    s32 posArray;
    s32 nrmArray;
    s32 flags;
    s32 i;
    s32 offset;
    s32 listCursor;
    s32 prim;
    s32 vtx;
    s32 j;
    s32 k;
    s32 q;
    s32 idx;
    s32 mask;
    s32 texPtr;
    volatile u16* fifo;

#define S16_AT(p_, o_) (*(s16*)((s32)(p_) + (o_)))
#define S32_AT(p_, o_) (*(s32*)((s32)(p_) + (o_)))
#define U32_AT(p_, o_) (*(u32*)((s32)(p_) + (o_)))
#define ARRAYS() S32_AT(poly, 0x0C)
#define ENTRY() (groupBase + S16_AT(obj, 0xDE) * 0x178)
#define ARRAY_PTR(off_) ((void*)(S32_AT(ARRAYS(), (off_)) + 4))
#define SAFE_IDX(v_) ((u16)(v_) & (s32)(((v_) + 1) | (-(v_) - 1)) >> 31)

    obj = param_1;
    poly = param_2;
    groupBase = mapWork + activeGroup * 0x2F4;
    posArray = S32_AT(ARRAYS(), 0x00) + 4;
    nrmArray = S32_AT(ARRAYS(), 0x04) + 4;
    texCount = S32_AT(ARRAYS(), 0x14);

    GXSetCullMode(culling[*(u8*)(S32_AT(S32_AT(obj, 0x08), 0x58) + 1)]);

    if ((*(u16*)(ENTRY() + 4) & 0x8000) == 0) {
        GXClearVtxDesc();
        GXSetVtxAttrFmtv(0, attr_fmt);

        GXSetVtxDesc(9, 3);
        GXSetArray(9, (void*)posArray, 0xC);

        GXSetVtxDesc(0xA, 3);
        GXSetArray(0xA, (void*)nrmArray, 0xC);

        GXSetVtxDesc(0xB, 3);
        GXSetArray(0xB, ARRAY_PTR(0x0C), 4);

        offset = 0;
        for (i = 0; i < texCount; i++, offset += 4) {
            GXSetVtxDesc(i + 0xD, 3);
            GXSetArray(i + 0xD, ARRAY_PTR(offset + 0x18), 8);
        }

        fifo = (volatile u16*)0xCC008000;
        listCursor = poly;
        for (i = 0; i < S32_AT(poly, 4); i++) {
            prim = S32_AT(listCursor, 0x10);
            GXBegin(0x98, 0, (u16)S32_AT(prim, 0));

            vtx = prim;
            j = 0;
            while (j < S32_AT(prim, 0)) {
                *fifo = S16_AT(vtx, 4);
                *fifo = S16_AT(vtx, 6);
                *fifo = S16_AT(vtx, 8);

                if (texCount > 0) {
                    k = texCount;
                    texPtr = vtx + 4;

                    q = (u32)k >> 2;
                    if (q != 0) {
                        do {
                            idx = S16_AT(texPtr, 8);
                            *fifo = SAFE_IDX(idx);

                            idx = S16_AT(texPtr, 0xA);
                            *fifo = SAFE_IDX(idx);

                            idx = S16_AT(texPtr, 0xC);
                            *fifo = SAFE_IDX(idx);

                            idx = S16_AT(texPtr, 0xE);
                            *fifo = SAFE_IDX(idx);

                            texPtr += 8;
                            q--;
                        } while (q != 0);
                    }

                    k &= 3;
                    if (k != 0) {
                        do {
                            idx = S16_AT(texPtr, 8);
                            texPtr += 2;
                            *fifo = SAFE_IDX(idx);
                            k--;
                        } while (k != 0);
                    }
                }

                vtx += 0x18;
                j++;
            }

            listCursor += 4;
        }
    } else {
        flags = S32_AT(poly, 8);

        GXClearVtxDesc();

        GXSetVtxDesc(9, 3);
        GXSetVtxAttrFmt(0, 9, 1, 3, U32_AT(S32_AT(ENTRY(), 0xA4), 0x44) & 0xFF);
        GXSetArray(9, (void*)posArray, 6);

        GXSetVtxDesc(0xA, 3);
        GXSetVtxAttrFmt(0, 0xA, 0, 1, 6);
        GXSetArray(0xA, (void*)nrmArray, 3);

        offset = 0;
        i = 0;
        while (i < 2) {
            mask = 1 << (i + 2);
            if ((flags & mask) == 0) {
                break;
            }

            GXSetVtxDesc(0xB, 3);
            GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
            GXSetArray(0xB, ARRAY_PTR(offset + 0x0C), 4);

            i++;
            offset += 4;
        }

        offset = 0;
        i = 0;
        while (i < 8) {
            mask = 1 << (i + 4);
            if ((flags & mask) == 0) {
                break;
            }

            GXSetVtxDesc(i + 0xD, 3);
            GXSetVtxAttrFmt(0, i + 0xD, 1, 3, U32_AT(S32_AT(ENTRY(), 0xA4), offset + 0x48) & 0xFF);
            GXSetArray(i + 0xD, ARRAY_PTR(offset + 0x18), 4);

            i++;
            offset += 4;
        }

        listCursor = poly;
        for (i = 0; i < S32_AT(poly, 4); i++) {
            GXCallDisplayList((void*)S32_AT(listCursor, 0x10), U32_AT(listCursor, 0x14));
            listCursor += 8;
        }
    }

#undef S16_AT
#undef S32_AT
#undef U32_AT
#undef ARRAYS
#undef ENTRY
#undef ARRAY_PTR
#undef SAFE_IDX
}

void* mapSearchDmdJointSub2(void* param_1, char* param_2, char* param_3) {
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    extern s32 strcmp(const char* a, const char* b);
    extern char str__8041f94c[1];

    void* node;
    char* prefix;
    char* target;
    void* cur;
    void* sub;
    char childBuf[256];
    char topSibBuf[256];
    char grandChildBuf[256];
    char childSibBuf[256];
    char topSibChildBuf[256];
    char topSibSibBuf[256];
    char rootBuf[260];

#define JOINT_NAME(j_) (*(char**)((s32)(j_) + 0x00))
#define JOINT_CHILD(j_) (*(void**)((s32)(j_) + 0x0C))
#define JOINT_SIB(j_) (*(void**)((s32)(j_) + 0x10))

    node = param_1;
    prefix = param_3;
    target = param_2;

    strcpy(rootBuf, prefix);
    strcat(rootBuf, JOINT_NAME(node));
    if (strcmp(rootBuf, target) == 0) {
        return node;
    }

    if (JOINT_CHILD(node) != NULL) {
        strcat(rootBuf, str__8041f94c);

        cur = JOINT_CHILD(node);
        strcpy(childBuf, rootBuf);
        strcat(childBuf, JOINT_NAME(cur));

        if (strcmp(childBuf, target) != 0) {
            if (JOINT_CHILD(cur) != NULL) {
                strcat(childBuf, str__8041f94c);

                sub = JOINT_CHILD(cur);
                strcpy(grandChildBuf, childBuf);
                strcat(grandChildBuf, JOINT_NAME(sub));

                if (strcmp(grandChildBuf, target) != 0) {
                    if (JOINT_CHILD(sub) != NULL) {
                        strcat(grandChildBuf, str__8041f94c);
                        sub = mapSearchDmdJointSub2(JOINT_CHILD(sub), target, grandChildBuf);
                        if (sub != NULL) {
                            goto first_grand_done;
                        }
                    }

                    if (JOINT_SIB(sub) != NULL) {
                        sub = mapSearchDmdJointSub2(JOINT_SIB(sub), target, childBuf);
                        if (sub != NULL) {
                            goto first_grand_done;
                        }
                    }

                    sub = NULL;
                }

first_grand_done:
                if (sub != NULL) {
                    cur = sub;
                    goto first_child_done;
                }
            }

            cur = JOINT_SIB(cur);
            if (cur != NULL) {
                strcpy(childSibBuf, rootBuf);
                strcat(childSibBuf, JOINT_NAME(cur));

                if (strcmp(childSibBuf, target) != 0) {
                    if (JOINT_CHILD(cur) != NULL) {
                        strcat(childSibBuf, str__8041f94c);
                        sub = mapSearchDmdJointSub2(JOINT_CHILD(cur), target, childSibBuf);
                        if (sub != NULL) {
                            cur = sub;
                            goto first_child_done;
                        }
                    }

                    if (JOINT_SIB(cur) != NULL) {
                        sub = mapSearchDmdJointSub2(JOINT_SIB(cur), target, rootBuf);
                        if (sub != NULL) {
                            cur = sub;
                            goto first_child_done;
                        }
                    }

                    cur = NULL;
                }
            } else {
                cur = NULL;
            }
        }

first_child_done:
        if (cur != NULL) {
            return cur;
        }
    }

    cur = JOINT_SIB(node);
    if (cur == NULL) {
        return NULL;
    }

    strcpy(topSibBuf, prefix);
    strcat(topSibBuf, JOINT_NAME(cur));

    if (strcmp(topSibBuf, target) != 0) {
        if (JOINT_CHILD(cur) != NULL) {
            strcat(topSibBuf, str__8041f94c);

            sub = JOINT_CHILD(cur);
            strcpy(topSibChildBuf, topSibBuf);
            strcat(topSibChildBuf, JOINT_NAME(sub));

            if (strcmp(topSibChildBuf, target) != 0) {
                if (JOINT_CHILD(sub) != NULL) {
                    strcat(topSibChildBuf, str__8041f94c);
                    sub = mapSearchDmdJointSub2(JOINT_CHILD(sub), target, topSibChildBuf);
                    if (sub != NULL) {
                        goto top_sib_child_done;
                    }
                }

                if (JOINT_SIB(sub) != NULL) {
                    sub = mapSearchDmdJointSub2(JOINT_SIB(sub), target, topSibBuf);
                    if (sub != NULL) {
                        goto top_sib_child_done;
                    }
                }

                sub = NULL;
            }

top_sib_child_done:
            if (sub != NULL) {
                cur = sub;
                goto top_sib_done;
            }
        }

        cur = JOINT_SIB(cur);
        if (cur != NULL) {
            strcpy(topSibSibBuf, prefix);
            strcat(topSibSibBuf, JOINT_NAME(cur));

            if (strcmp(topSibSibBuf, target) != 0) {
                if (JOINT_CHILD(cur) != NULL) {
                    strcat(topSibSibBuf, str__8041f94c);
                    sub = mapSearchDmdJointSub2(JOINT_CHILD(cur), target, topSibSibBuf);
                    if (sub != NULL) {
                        cur = sub;
                        goto top_sib_done;
                    }
                }

                if (JOINT_SIB(cur) != NULL) {
                    sub = mapSearchDmdJointSub2(JOINT_SIB(cur), target, prefix);
                    if (sub != NULL) {
                        cur = sub;
                        goto top_sib_done;
                    }
                }

                cur = NULL;
            }
        } else {
            cur = NULL;
        }
    }

top_sib_done:
    if (cur == NULL) {
        return NULL;
    }
    return cur;

#undef JOINT_NAME
#undef JOINT_CHILD
#undef JOINT_SIB
}

void _mapDispMapGrp(s32 cam, void* obj) {
    extern void _mapDispMapObj(s32 cam, void* obj);

    void* p1;
    void* p2;
    void* p3;

    _mapDispMapObj(cam, obj);

    p1 = *(void**)((s32)obj + 0xE4);
    if (p1 != 0) {
        _mapDispMapObj(cam, p1);

        p2 = *(void**)((s32)p1 + 0xE4);
        if (p2 != 0) {
            _mapDispMapObj(cam, p2);

            p3 = *(void**)((s32)p2 + 0xE4);
            if (p3 != 0) {
                _mapDispMapObj(cam, p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE4));
                }

                if ((*(u32*)p3 & 0x20) != 0) {
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE8));
                    }
                }
            }

            if ((*(u32*)p2 & 0x20) != 0) {
                p3 = *(void**)((s32)p2 + 0xE8);
                if (p3 != 0) {
                    _mapDispMapObj(cam, p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE4));
                    }

                    if ((*(u32*)p3 & 0x20) != 0) {
                        if (*(void**)((s32)p3 + 0xE8) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE8));
                        }
                    }
                }
            }
        }

        if ((*(u32*)p1 & 0x20) != 0) {
            p1 = *(void**)((s32)p1 + 0xE8);
            if (p1 != 0) {
                _mapDispMapObj(cam, p1);

                p2 = *(void**)((s32)p1 + 0xE4);
                if (p2 != 0) {
                    _mapDispMapObj(cam, p2);

                    if (*(void**)((s32)p2 + 0xE4) != 0) {
                        _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE4));
                    }

                    if ((*(u32*)p2 & 0x20) != 0) {
                        if (*(void**)((s32)p2 + 0xE8) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE8));
                        }
                    }
                }

                if ((*(u32*)p1 & 0x20) != 0) {
                    p1 = *(void**)((s32)p1 + 0xE8);
                    if (p1 != 0) {
                        _mapDispMapObj(cam, p1);

                        if (*(void**)((s32)p1 + 0xE4) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p1 + 0xE4));
                        }

                        if ((*(u32*)p1 & 0x20) != 0) {
                            if (*(void**)((s32)p1 + 0xE8) != 0) {
                                _mapDispMapGrp(cam, *(void**)((s32)p1 + 0xE8));
                            }
                        }
                    }
                }
            }
        }
    }

    if ((*(u32*)obj & 0x20) != 0) {
        p1 = *(void**)((s32)obj + 0xE8);
        if (p1 != 0) {
            _mapDispMapObj(cam, p1);

            p2 = *(void**)((s32)p1 + 0xE4);
            if (p2 != 0) {
                _mapDispMapObj(cam, p2);

                p3 = *(void**)((s32)p2 + 0xE4);
                if (p3 != 0) {
                    _mapDispMapObj(cam, p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE4));
                    }

                    if ((*(u32*)p3 & 0x20) != 0) {
                        if (*(void**)((s32)p3 + 0xE8) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE8));
                        }
                    }
                }

                if ((*(u32*)p2 & 0x20) != 0) {
                    p2 = *(void**)((s32)p2 + 0xE8);
                    if (p2 != 0) {
                        _mapDispMapObj(cam, p2);

                        if (*(void**)((s32)p2 + 0xE4) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE4));
                        }

                        if ((*(u32*)p2 & 0x20) != 0) {
                            if (*(void**)((s32)p2 + 0xE8) != 0) {
                                _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE8));
                            }
                        }
                    }
                }
            }

            if ((*(u32*)p1 & 0x20) != 0) {
                p1 = *(void**)((s32)p1 + 0xE8);
                if (p1 != 0) {
                    _mapDispMapObj(cam, p1);

                    p2 = *(void**)((s32)p1 + 0xE4);
                    if (p2 != 0) {
                        _mapDispMapObj(cam, p2);

                        if (*(void**)((s32)p2 + 0xE4) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE4));
                        }

                        if ((*(u32*)p2 & 0x20) != 0) {
                            if (*(void**)((s32)p2 + 0xE8) != 0) {
                                _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE8));
                            }
                        }
                    }

                    if ((*(u32*)p1 & 0x20) != 0) {
                        p1 = *(void**)((s32)p1 + 0xE8);
                        if (p1 != 0) {
                            _mapDispMapObj(cam, p1);

                            if (*(void**)((s32)p1 + 0xE4) != 0) {
                                _mapDispMapGrp(cam, *(void**)((s32)p1 + 0xE4));
                            }

                            if ((*(u32*)p1 & 0x20) != 0) {
                                if (*(void**)((s32)p1 + 0xE8) != 0) {
                                    _mapDispMapGrp(cam, *(void**)((s32)p1 + 0xE8));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

u8 mapSetTextureMatrix(void* param_1) {
    extern s32 texmtx_tbl[];
    extern f32 float_1_8041f940;
    extern f32 float_0_8041f930;
    extern f32 float_0p5_8041f934;
    extern f32 float_deg2rad_8041f948;

    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXCopy(void* src, void* dst);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetTexCoordGen2(s32 coord, s32 func, s32 source, u32 mtx, u32 normalize, s32 postmtx);

    f32 outMtx[3][4];
    f32 negCenterMtx[3][4];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];

    f32 one;
    f32 scaleX;
    f32 scaleY;
    f32 transX;
    f32 negTransY;
    f32 negRot;
    f32 centerX;
    f32 centerY;
    f32 zero;
    void* work;
    s32 coord;
    s32 index;
    s32 countIndex;
    s32* texMtx;
    s32 texCount;

    one = float_1_8041f940;
    work = param_1;
    texCount = *(u8*)((s32)work + 0x0B);
    coord = 0;
    index = texCount - 1;
    countIndex = 0;
    work = (void*)((s32)work + index * 0x1C);
    texMtx = &texmtx_tbl[index];

    while (index >= 0) {
        scaleX = *(f32*)((s32)work + 0x34);
        negRot = -*(f32*)((s32)work + 0x3C);
        scaleY = *(f32*)((s32)work + 0x38);
        transX = *(f32*)((s32)work + 0x2C);
        negTransY = -*(f32*)((s32)work + 0x30);

        if (one == scaleX &&
            one == scaleY &&
            float_0_8041f930 == transX &&
            float_0_8041f930 == negTransY &&
            float_0_8041f930 == negRot) {
            GXSetTexCoordGen2(coord, 1, (*(u8*)((s32)param_1 + 0x0B) + 3) - countIndex, 0x3C, 0, 0x7D);
        } else {
            zero = float_0_8041f930;
            if (negRot != zero) {
                centerX = (float_0p5_8041f934 * *(f32*)((s32)work + 0x40)) * scaleX;
                centerY = (float_1_8041f940 - (float_0p5_8041f934 * *(f32*)((s32)work + 0x44))) * scaleY;

                PSMTXTrans(outMtx, centerX, centerY, zero);
                PSMTXRotRad(rotMtx, 'z', float_deg2rad_8041f948 * negRot);
                PSMTXTrans(negCenterMtx, -centerX, -centerY, float_0_8041f930);
                PSMTXConcat(outMtx, rotMtx, rotMtx);
                PSMTXConcat(rotMtx, negCenterMtx, rotMtx);
            }

            if (scaleX != float_0_8041f930 || scaleY != float_0_8041f930) {
                PSMTXScale(scaleMtx, scaleX, scaleY, float_1_8041f940);
            }

            if (transX != float_0_8041f930 || negTransY != float_0_8041f930) {
                PSMTXTrans(transMtx, transX, negTransY, float_0_8041f930);
            }

            zero = float_0_8041f930;
            if (negRot != zero) {
                if (scaleX != zero || scaleY != zero) {
                    PSMTXConcat(rotMtx, scaleMtx, outMtx);
                    if (transX != float_0_8041f930 || negTransY != float_0_8041f930) {
                        PSMTXConcat(outMtx, transMtx, outMtx);
                    }
                } else if (transX != zero || negTransY != zero) {
                    PSMTXConcat(rotMtx, transMtx, outMtx);
                } else {
                    PSMTXCopy(rotMtx, outMtx);
                }
            } else if (scaleX != zero || scaleY != zero) {
                if (transX != float_0_8041f930 || negTransY != float_0_8041f930) {
                    PSMTXConcat(scaleMtx, transMtx, outMtx);
                } else {
                    PSMTXCopy(scaleMtx, outMtx);
                }
            } else if (transX != zero || negTransY != zero) {
                PSMTXCopy(transMtx, outMtx);
            }

            GXLoadTexMtxImm(outMtx, *texMtx, 1);
            GXSetTexCoordGen2(coord, 1, (*(u8*)((s32)param_1 + 0x0B) + 3) - countIndex, *texMtx, 0, 0x7D);
        }

        coord++;
        countIndex++;
        work = (void*)((s32)work - 0x1C);
        texMtx--;
        index--;
    }
}

u8 mapFlgOff(u32* param_1, u32 param_2, int param_3) {
    u32* root;
    u32 flags;
    u32 mask;
    int recurse;
    u32* p1;
    u32* p2;
    u32* p3;

    root = param_1;
    flags = param_2;
    recurse = param_3;
    mask = ~flags;

#define FLG_OFF(node_)              \
    do {                            \
        *(node_) = *(node_) & mask; \
    } while (0)

    FLG_OFF(root);

    p1 = (u32*)root[0x39];
    if (p1 != 0) {
        FLG_OFF(p1);

        p2 = (u32*)p1[0x39];
        if (p2 != 0) {
            FLG_OFF(p2);

            p3 = (u32*)p2[0x39];
            if (p3 != 0) {
                FLG_OFF(p3);

                if ((u32*)p3[0x39] != 0) {
                    mapFlgOff((u32*)p3[0x39], flags, 1);
                }
                if ((u32*)p3[0x3A] != 0) {
                    mapFlgOff((u32*)p3[0x3A], flags, 1);
                }
            }

            p3 = (u32*)p2[0x3A];
            if (p3 != 0) {
                FLG_OFF(p3);

                if ((u32*)p3[0x39] != 0) {
                    mapFlgOff((u32*)p3[0x39], flags, 1);
                }
                if ((u32*)p3[0x3A] != 0) {
                    mapFlgOff((u32*)p3[0x3A], flags, 1);
                }
            }
        }

        p2 = (u32*)p1[0x3A];
        if (p2 != 0) {
            FLG_OFF(p2);

            p3 = (u32*)p2[0x39];
            if (p3 != 0) {
                FLG_OFF(p3);

                if ((u32*)p3[0x39] != 0) {
                    mapFlgOff((u32*)p3[0x39], flags, 1);
                }
                if ((u32*)p3[0x3A] != 0) {
                    mapFlgOff((u32*)p3[0x3A], flags, 1);
                }
            }

            p2 = (u32*)p2[0x3A];
            if (p2 != 0) {
                FLG_OFF(p2);

                if ((u32*)p2[0x39] != 0) {
                    mapFlgOff((u32*)p2[0x39], flags, 1);
                }
                if ((u32*)p2[0x3A] != 0) {
                    mapFlgOff((u32*)p2[0x3A], flags, 1);
                }
            }
        }
    }

    if (recurse != 0) {
        p1 = (u32*)root[0x3A];
        if (p1 != 0) {
            FLG_OFF(p1);

            p2 = (u32*)p1[0x39];
            if (p2 != 0) {
                FLG_OFF(p2);

                p3 = (u32*)p2[0x39];
                if (p3 != 0) {
                    FLG_OFF(p3);

                    if ((u32*)p3[0x39] != 0) {
                        mapFlgOff((u32*)p3[0x39], flags, 1);
                    }
                    if ((u32*)p3[0x3A] != 0) {
                        mapFlgOff((u32*)p3[0x3A], flags, 1);
                    }
                }

                p3 = (u32*)p2[0x3A];
                if (p3 != 0) {
                    FLG_OFF(p3);

                    if ((u32*)p3[0x39] != 0) {
                        mapFlgOff((u32*)p3[0x39], flags, 1);
                    }
                    if ((u32*)p3[0x3A] != 0) {
                        mapFlgOff((u32*)p3[0x3A], flags, 1);
                    }
                }
            }

            p2 = (u32*)p1[0x3A];
            if (p2 != 0) {
                FLG_OFF(p2);

                p3 = (u32*)p2[0x39];
                if (p3 != 0) {
                    FLG_OFF(p3);

                    if ((u32*)p3[0x39] != 0) {
                        mapFlgOff((u32*)p3[0x39], flags, 1);
                    }
                    if ((u32*)p3[0x3A] != 0) {
                        mapFlgOff((u32*)p3[0x3A], flags, 1);
                    }
                }

                p2 = (u32*)p2[0x3A];
                if (p2 != 0) {
                    FLG_OFF(p2);

                    if ((u32*)p2[0x39] != 0) {
                        mapFlgOff((u32*)p2[0x39], flags, 1);
                    }
                    if ((u32*)p2[0x3A] != 0) {
                        mapFlgOff((u32*)p2[0x3A], flags, 1);
                    }
                }
            }
        }
    }

#undef FLG_OFF
}

void mapGetJointsSub(void*, s32*) {
    ;
}


u8 mapPlayAnimationLv(char* param_1, int param_2, int param_3) {
    return 0;
}


void bmapUnLoad(void) {
    extern void sysWaitDrawSync(void);
    extern void _mapFree(void* heap, void* ptr);
    extern void* _mapAllocTail(void* heap, u32 size);
    extern void hitReInit(void);
    extern void lightReInit(void);
    extern void envReInit(void);
    extern s32 arcDataCheck(u32 ptr);
    extern void strncpy(char* dst, const char* src, u32 n);
    extern void arcDelete(s32 type);
    extern void arcEntry(s32 type, void* data, u32 size);
    extern void aramMgrToMram(void* aram, void* mram);
    extern void makeDisplayList(s32 index);
    extern void* mapWork;
    extern s32 activeGroup;
    extern s32 unk_8041e678;
    extern void* mapalloc_base_ptr;
    extern char float_0_8041f9ac[];
    void* group;
    void* work;
    void* entry;
    s32 i;

    sysWaitDrawSync();
    work = mapWork;
    activeGroup = 1;
    group = (void*)((s32)work + 0x2F4);

    entry = group;
    for (i = 0; i < *(s32*)group; i++, entry = (void*)((s32)entry + 0x178)) {
        if (*(void**)((s32)entry + 0x154) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x154));
            *(s32*)((s32)entry + 0x150) = 0;
            *(void**)((s32)entry + 0x154) = NULL;
        }
        if (*(void**)((s32)entry + 0x164) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x164));
            *(s32*)((s32)entry + 0x160) = 0;
            *(void**)((s32)entry + 0x164) = NULL;
        }
        if (*(void**)((s32)entry + 0x16C) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x16C));
            *(void**)((s32)entry + 0x16C) = NULL;
            *(s32*)((s32)entry + 0x168) = 0;
        }
    }

    unk_8041e678 = 0;
    hitReInit();
    lightReInit();
    envReInit();

    entry = group;
    for (i = 0; i < *(s32*)group; i++, entry = (void*)((s32)entry + 0x178)) {
        if (*(void**)((s32)entry + 0x7C) != NULL) {
            if (arcDataCheck((u32)*(void**)((s32)entry + 0x7C)) == 0) {
                _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x7C));
            }
            *(void**)((s32)entry + 0x7C) = NULL;
        }
        if (*(void**)((s32)entry + 0x84) != NULL) {
            if (arcDataCheck((u32)*(void**)((s32)entry + 0x84)) == 0) {
                _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x84));
            }
            *(void**)((s32)entry + 0x84) = NULL;
        }
        if (*(void**)((s32)entry + 0x8C) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x8C));
            *(void**)((s32)entry + 0x8C) = NULL;
        }
        strncpy((char*)((s32)entry + 0x6), float_0_8041f9ac, 0x10);
    }

    arcDelete(1);
    arcDelete(2);
    memset(group, 0, 0x2F4);

    activeGroup = 0;
    entry = work;
    for (i = 0; i < *(s32*)work; i++, entry = (void*)((s32)entry + 0x178)) {
        if (*(void**)((s32)entry + 0x170) != NULL) {
            *(void**)((s32)entry + 0x7C) = _mapAllocTail(mapalloc_base_ptr, *(u32*)((s32)entry + 0x80));
            *(void**)((s32)entry + 0x84) = _mapAllocTail(mapalloc_base_ptr, *(u32*)((s32)entry + 0x88));
            aramMgrToMram(*(void**)((s32)entry + 0x170), *(void**)((s32)entry + 0x7C));
            aramMgrToMram(*(void**)((s32)entry + 0x178), *(void**)((s32)entry + 0x84));
            *(void**)((s32)entry + 0x170) = NULL;
            *(void**)((s32)entry + 0x178) = NULL;
        } else {
            *(void**)((s32)entry + 0x8C) = _mapAllocTail(mapalloc_base_ptr, *(u32*)((s32)entry + 0x90));
            aramMgrToMram(*(void**)((s32)entry + 0x174), *(void**)((s32)entry + 0x8C));
            *(void**)((s32)entry + 0x174) = NULL;
        }
        makeDisplayList(i);
    }

    if (*(void**)((s32)work + 0x8C) != NULL) {
        arcEntry(1, *(void**)((s32)work + 0x8C), *(u32*)((s32)work + 0x90));
    }
}

u8 makeDisplayList(int param_1) {
    return 0;
}


void mapObjSetFlushColor(char* name, u8 r, u8 g, u8 b, u8 a) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;
    extern u32 unk_8042952c;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    u32 color;
    s32 errorOffset;
    char* entry;

    color = unk_8042952c;
    *((u8*)&color + 0) = r;
    *((u8*)&color + 1) = g;
    *((u8*)&color + 2) = b;
    *((u8*)&color + 3) = a;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (name == NULL) {
        obj = NULL;
        goto first_done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), name) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto first_done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

first_done:
    if (obj != NULL) {
        *(u32*)((s32)obj + 0x0C) = color;
        *(u32*)obj = *(u32*)obj | 0x40;
    }

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (name == NULL) {
        obj = NULL;
        goto second_done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), name) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto second_done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

second_done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, name, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        *(u32*)obj = *(u32*)obj & ~0x40;
    }
}

void mapSetPolygonVtxDesc(int obj, int poly) {
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 cnt, s32 type, s32 frac);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern s32 activeGroup;
    extern void* mapWork;
    void* group;
    void* vcd;
    void* arrays;
    u32 flags;
    s32 i;
    s32 offset;

    arrays = *(void**)((s32)poly + 0xC);
    flags = *(u32*)((s32)poly + 0x8);
    group = (void*)((s32)mapWork + activeGroup * 0x2F4);

    GXClearVtxDesc();
    GXSetVtxDesc(9, 3);
    vcd = *(void**)((s32)group + (*(s16*)((s32)obj + 0xDE) * 0x178) + 0xA4);
    GXSetVtxAttrFmt(0, 9, 1, 3, *(u32*)((s32)vcd + 0x44) & 0xFF);
    GXSetArray(9, (void*)(*(s32*)arrays + 4), 6);

    GXSetVtxDesc(0xA, 3);
    GXSetVtxAttrFmt(0, 0xA, 0, 1, 6);
    GXSetArray(0xA, (void*)(*(s32*)((s32)arrays + 4) + 4), 3);

    offset = 0;
    for (i = 0; i < 2; i++, offset += 4) {
        if ((flags & (1 << (i + 2))) == 0) {
            break;
        }
        GXSetVtxDesc(0xB, 3);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetArray(0xB, (void*)(*(s32*)((s32)arrays + offset + 0xC) + 4), 4);
    }

    offset = 0;
    for (i = 0; i < 8; i++, offset += 4) {
        if ((flags & (1 << (i + 4))) == 0) {
            break;
        }
        GXSetVtxDesc(i + 0xD, 3);
        vcd = *(void**)((s32)group + (*(s16*)((s32)obj + 0xDE) * 0x178) + 0xA4);
        GXSetVtxAttrFmt(0, i + 0xD, 1, 3, *(u32*)((s32)vcd + offset + 0x48) & 0xFF);
        GXSetArray(i + 0xD, (void*)(*(s32*)((s32)arrays + offset + 0x18) + 4), 4);
    }
}

void mapGetBoundingBox(void* pMin, void* pMax) {
    ;
}


void mapUnLoad(void) {
    extern void sysWaitDrawSync(void);
    extern void _mapFree(void* heap, void* ptr);
    extern void hitReInit(void);
    extern void lightReInit(void);
    extern void envReInit(void);
    extern void camUnLoadRoad(s32 cameraId);
    extern void camSetTypePersp(s32 cameraId);
    extern s32 arcDataCheck(u32 ptr);
    extern void strncpy(char* dst, const char* src, u32 n);
    extern void arcDelete(s32 type);
    extern void* mapWork;
    extern s32 activeGroup;
    extern s32 unk_8041e678;
    extern void* mapalloc_base_ptr;
    extern char float_0_8041f9ac[];
    void* work;
    void* entry;
    s32 i;

    sysWaitDrawSync();
    work = mapWork;
    activeGroup = 0;

    entry = work;
    for (i = 0; i < *(s32*)work; i++, entry = (void*)((s32)entry + 0x178)) {
        if (*(void**)((s32)entry + 0x154) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x154));
            *(s32*)((s32)entry + 0x150) = 0;
            *(void**)((s32)entry + 0x154) = NULL;
        }
        if (*(void**)((s32)entry + 0x164) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x164));
            *(s32*)((s32)entry + 0x160) = 0;
            *(void**)((s32)entry + 0x164) = NULL;
        }
        if (*(void**)((s32)entry + 0x16C) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x16C));
            *(void**)((s32)entry + 0x16C) = NULL;
            *(s32*)((s32)entry + 0x168) = 0;
        }
    }

    unk_8041e678 = 0;
    hitReInit();
    lightReInit();
    envReInit();
    camUnLoadRoad(4);
    camSetTypePersp(4);

    entry = work;
    for (i = 0; i < *(s32*)work; i++, entry = (void*)((s32)entry + 0x178)) {
        if (*(void**)((s32)entry + 0x7C) != NULL) {
            if (arcDataCheck((u32)*(void**)((s32)entry + 0x7C)) == 0) {
                _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x7C));
            }
            *(void**)((s32)entry + 0x7C) = NULL;
        }
        if (*(void**)((s32)entry + 0x84) != NULL) {
            if (arcDataCheck((u32)*(void**)((s32)entry + 0x84)) == 0) {
                _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x84));
            }
            *(void**)((s32)entry + 0x84) = NULL;
        }
        if (*(void**)((s32)entry + 0x8C) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x8C));
            *(void**)((s32)entry + 0x8C) = NULL;
        }
        strncpy((char*)((s32)entry + 0x6), float_0_8041f9ac, 0x10);
    }

    arcDelete(1);
    memset(work, 0, 0x2F4);
}

u8 mapObjRotate(char* name, f32 x, f32 y, f32 z) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern void PSMTXIdentity(void*);
    extern void PSMTXRotRad(void*, s8, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern f32 float_deg2rad_8041f948;
    void* work;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    f32 mtxX[3][4];
    f32 mtxY[3][4];
    f32 mtxZ[3][4];

    work = (void*)(mapWork + activeGroup * 0x2F4);
    if (name == 0) {
        obj = 0;
    } else {
        group = work;
        groupIndex = 0;
        while (groupIndex < *(s32*)work) {
            obj = *(void**)((s32)group + 0x154);
            objIndex = 0;
            while (objIndex < *(s32*)((s32)group + 0x150)) {
                if (strcmp(*(const char**)*(s32*)((s32)obj + 8), name) == 0) {
                    goto found;
                }
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        obj = 0;
    }

found:
    if (obj != 0) {
        if ((*(u32*)obj & 0x200) == 0) {
            PSMTXIdentity((void*)((s32)obj + 0xAC));
        }
        PSMTXRotRad(mtxX, 'x', float_deg2rad_8041f948 * x);
        PSMTXRotRad(mtxY, 'y', float_deg2rad_8041f948 * y);
        PSMTXRotRad(mtxZ, 'z', float_deg2rad_8041f948 * z);
        PSMTXConcat((void*)((s32)obj + 0xAC), mtxZ, (void*)((s32)obj + 0xAC));
        PSMTXConcat((void*)((s32)obj + 0xAC), mtxY, (void*)((s32)obj + 0xAC));
        PSMTXConcat((void*)((s32)obj + 0xAC), mtxX, (void*)((s32)obj + 0xAC));
        *(u32*)obj |= 0x2000200;
    }
}

u8 bmapLoad(char* rankStageName, char* battleStageName) {
    return 0;
}


u8 spline_getvalue(double param_1, float* param_2, int param_3, float* param_4, int param_5, int param_6) {
    return 0;
}


u8 mapDispMapObj_bbox(int param_1, int param_2) {
    return 0;
}


u8 mapObjGetPos(char* param_1, void* param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern void mapObjGetPosSub(void*, void*, s32*, s32);
    extern void PSVECScale(void*, void*, f32);
    extern f32 float_1_8041f940;
    extern f32 vec3_802bfb74[3];
    extern f32 vec3_802bfb80[3];
    void* work;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 count;
    f32 pos[3];

    count = 0;
    pos[0] = vec3_802bfb74[0];
    pos[1] = vec3_802bfb74[1];
    pos[2] = vec3_802bfb74[2];
    work = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == 0) {
        obj = 0;
    } else {
        group = work;
        groupIndex = 0;
        while (groupIndex < *(s32*)work) {
            obj = *(void**)((s32)group + 0x154);
            objIndex = 0;
            while (objIndex < *(s32*)((s32)group + 0x150)) {
                if (strcmp(*(const char**)*(s32*)((s32)obj + 8), param_1) == 0) {
                    goto found;
                }
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        obj = 0;
    }

found:
    if (obj == 0) {
        ((f32*)param_2)[0] = vec3_802bfb80[0];
        ((f32*)param_2)[1] = vec3_802bfb80[1];
        ((f32*)param_2)[2] = vec3_802bfb80[2];
    } else {
        mapObjGetPosSub(obj, pos, &count, 0);
        PSVECScale(pos, param_2, float_1_8041f940 / (f32)count);
    }
}

u8 mapObjTranslate(char* name, f32 x, f32 y, f32 z) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern void PSMTXIdentity(void*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern f32 float_10_8041f944;
    void* work;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    f32 mtx[3][4];

    work = (void*)(mapWork + activeGroup * 0x2F4);
    if (name == 0) {
        obj = 0;
    } else {
        group = work;
        groupIndex = 0;
        while (groupIndex < *(s32*)work) {
            obj = *(void**)((s32)group + 0x154);
            objIndex = 0;
            while (objIndex < *(s32*)((s32)group + 0x150)) {
                if (strcmp(*(const char**)*(s32*)((s32)obj + 8), name) == 0) {
                    goto found;
                }
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        obj = 0;
    }

found:
    if (obj != 0) {
        if ((*(u32*)obj & 0x200) == 0) {
            PSMTXIdentity((void*)((s32)obj + 0xAC));
        }
        PSMTXTrans(mtx, x / float_10_8041f944, y / float_10_8041f944, z / float_10_8041f944);
        PSMTXConcat((void*)((s32)obj + 0xAC), mtx, (void*)((s32)obj + 0xAC));
        *(u32*)obj |= 0x2000200;
    }
}

void mapObjGetFlushColor(char* pName, void* param_2, void* param_3, void* param_4, void* param_5) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (pName == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), pName) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, pName, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        *(u8*)param_2 = *(u8*)((s32)obj + 0x0C);
        *(u8*)param_3 = *(u8*)((s32)obj + 0x0D);
        *(u8*)param_4 = *(u8*)((s32)obj + 0x0E);
        *(u8*)param_5 = *(u8*)((s32)obj + 0x0F);
    }
}

void mapCheckAnimation(s32 id, s32* outDone, f32* outFrame) {
    ;
}


u8 mapObjFlagOn(char* param_1, u32 param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        if ((param_2 & 0x100) == 0 || *(void**)((s32)obj + 0x114) != NULL) {
            *(u32*)obj = *(u32*)obj | param_2;
        }
    }
}

u8 mapObjScale(char* name, f32 x, f32 y, f32 z) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern void PSMTXIdentity(void*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    void* work;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    f32 mtx[3][4];

    work = (void*)(mapWork + activeGroup * 0x2F4);
    if (name == 0) {
        obj = 0;
    } else {
        group = work;
        groupIndex = 0;
        while (groupIndex < *(s32*)work) {
            obj = *(void**)((s32)group + 0x154);
            objIndex = 0;
            while (objIndex < *(s32*)((s32)group + 0x150)) {
                if (strcmp(*(const char**)*(s32*)((s32)obj + 8), name) == 0) {
                    goto found;
                }
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        obj = 0;
    }

found:
    if (obj != 0) {
        if ((*(u32*)obj & 0x200) == 0) {
            PSMTXIdentity((void*)((s32)obj + 0xAC));
        }
        PSMTXScale(mtx, x, y, z);
        PSMTXConcat((void*)((s32)obj + 0xAC), mtx, (void*)((s32)obj + 0xAC));
        *(u32*)obj |= 0x2000200;
    }
}

void mapGrpFlagOff(s32 param_1, u32 param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern u8 mapFlgOff(u32* param_1, u32 param_2, int param_3);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == 0) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), (const char*)param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, (const char*)param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        mapFlgOff((u32*)obj, param_2, 0);
    }
}

void mapGrpFlagOn(s32 param_1, u32 param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern u8 mapFlgOn(u32* param_1, u32 param_2, int param_3);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == 0) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), (const char*)param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, (const char*)param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        mapFlgOn((u32*)obj, param_2, 0);
    }
}

u8 mapObjFlagOff(char* param_1, u32 param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        *(u32*)obj = *(u32*)obj & ~param_2;
    }
}

u8 mapObjFlushOff(char* param_1) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        *(u32*)obj = *(u32*)obj & ~0x100000;
    }
}

u8 mapObjFlushOn(char* param_1) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        *(u32*)obj = *(u32*)obj | 0x100000;
    }
}

u8 mapGrpSetOffScreen(char* param_1, char* param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern s32 offscreenNameToId(char* name);
    extern u8 _setOffScrnId(void* param_1, int param_2, int param_3);

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 offId;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    offId = offscreenNameToId(param_2);
    _setOffScrnId(obj, offId, 0);
}

u8 mapObjSetOffScreen(char* param_1, char* param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern s32 offscreenNameToId(char* name);

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 offId;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    offId = offscreenNameToId(param_2);
    *(u16*)((s32)obj + 0xDC) = (u16)offId;
    *(u32*)obj = *(u32*)obj | 0x80;
}

void mapGrpSetColor(s32 param_1, void* param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern u8 setColor(void* param_1, u32* param_2, int param_3);

    void* obj;
    void* group;
    void* groupBase;
    s32 objIndex;
    s32 groupIndex;
    u32 color;

    color = *(u32*)param_2;
    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == 0) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), (const char*)param_1) == 0) {
                goto done;
            }
            objIndex++;
            obj = (void*)((s32)obj + 0x134);
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    setColor(obj, &color, 0);
}

void mapSetPlayRate(char* name, f32 rate) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    void* work;
    void* group;
    void* anim;
    s32 i;
    s32 j;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            if ((*(u16*)anim & 1) != 0) {
                if (strcmp(*(const char**)*(s32*)((s32)anim + 0x18), name) == 0) {
                    goto found;
                }
            }
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }
    anim = 0;

found:
    if (anim != 0) {
        *(f32*)((s32)anim + 0x14) = rate;
    }
}


void mapObjClearOffScreen(s32 id) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (id == 0) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), (const char*)id) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    *(s16*)((s32)obj + 0xDC) = -1;
    *(u32*)obj = *(u32*)obj & ~0x80;
}

void mapReStartAnimation(char* name) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    void* work;
    void* group;
    void* anim;
    s32 i;
    s32 j;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            if ((*(u16*)anim & 1) != 0) {
                if (strcmp(*(const char**)*(s32*)((s32)anim + 0x18), name) == 0) {
                    goto found;
                }
            }
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }
    anim = 0;

found:
    if (anim != 0) {
        *(u16*)anim |= 0x20;
    }
}


void mapPauseAnimation(char* name) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    void* work;
    void* group;
    void* anim;
    s32 i;
    s32 j;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            if ((*(u16*)anim & 1) != 0) {
                if (strcmp(*(const char**)*(s32*)((s32)anim + 0x18), name) == 0) {
                    goto found;
                }
            }
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }
    anim = 0;

found:
    if (anim != 0) {
        *(u16*)anim |= 0x10;
    }
}


void mapGrpClearOffScreen(s32 id) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern u8 _setOffScrnId(void* param_1, int param_2, int param_3);

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (id == 0) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), (const char*)id) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    _setOffScrnId(obj, -1, 0);
}

void* mapGetMapObj(s32 name) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    void* work;
    void* group;
    void* obj;
    s32 i;
    s32 j;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    if (name == 0) {
        return 0;
    }

    group = work;
    i = 0;
    while (i < *(s32*)work) {
        obj = *(void**)((s32)group + 0x154);
        j = 0;
        while (j < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x8), (const char*)name) == 0) {
                return obj;
            }
            j++;
            obj = (void*)((s32)obj + 0x134);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }

    return 0;
}


u8 test_kururing_mapdisp(s32 cam) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern void _mapDispMapObj(s32 cam, void* obj);

    void* work;
    void* group;
    void* obj;
    s32 groupCount;
    s32 objCount;
    s32 i;
    s32 j;
    u32 flags;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    groupCount = *(s32*)work;

    for (i = 0; i < groupCount; i++) {
        obj = *(void**)((s32)group + 0x154);
        objCount = *(s32*)((s32)group + 0x150);

        for (j = 0; j < objCount; j++) {
            flags = *(u32*)obj;
            if ((flags & 0x400000) != 0) {
                *(u32*)obj = flags & 0xFFFFFFFD;
                _mapDispMapObj(cam, obj);
                *(u32*)obj = flags;
            }

            obj = (void*)((s32)obj + 0x134);
        }

        group = (void*)((s32)group + 0x178);
    }
}

void mapSetMaterialFog(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 fog_type;
    extern u32 dat_8041f900;
    extern f32 float_0_8041f930;
    extern void* camGetCurPtr(void);
    extern void GXSetFog(s32 type, f32 start, f32 end, f32 nearz, f32 farz, void* color);
    void* cam;
    void* work;
    u32 color;
    u32 zeroColor;

    cam = camGetCurPtr();
    work = (void*)(mapWork + activeGroup * 0x2F4);
    if ((*(u16*)((s32)work + 4) & 1) != 0) {
        color = *(u32*)((s32)work + 0x34);
        GXSetFog(
            *(s32*)((s32)&fog_type + (*(s32*)((s32)work + 0x28) * 4)),
            *(f32*)((s32)work + 0x2C),
            *(f32*)((s32)work + 0x30),
            *(f32*)((s32)cam + 0x30),
            *(f32*)((s32)cam + 0x34),
            &color
        );
    } else {
        zeroColor = dat_8041f900;
        GXSetFog(
            0,
            float_0_8041f930,
            float_0_8041f930,
            float_0_8041f930,
            float_0_8041f930,
            &zeroColor
        );
    }
}


void mapErrorEntry(s32 type, char* message) {
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;
    extern char* strncpy(char* dst, const char* src, u32 n);
    char* entry;

    entry = error_data;
    entry += error_count * 0x24;
    *(s32*)(entry + 0x20) = type;
    strncpy(entry, message, 0x1F);
    error_flag = 1;
    error_count++;
    error_count %= 32;
}

void mapLoad(char* name) {
    extern void sysWaitDrawSync(void);
    extern void* memset(void*, s32, u32);
    extern void _mapLoad(void*, int, char*);
    extern s32 activeGroup;
    extern s32 mapWork;
    void* work;

    sysWaitDrawSync();
    work = (void*)mapWork;
    activeGroup = 0;
    memset(work, 0, 0x2F4);
    _mapLoad(work, 0, name);
    *(s32*)work = 1;
}

void mapReStartAnimationAll(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    void* work;
    void* group;
    s32 j;
    s32 i;
    void* anim;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            *(u16*)anim |= 0x20;
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }
}


void mapPauseAnimationAll(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    void* work;
    void* group;
    s32 j;
    s32 i;
    void* anim;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            *(u16*)anim |= 0x10;
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }
}


void mapSetFog(s32 type, f32 start, f32 end, void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    s32 rgba;
    s32 work;

    rgba = *(s32*)color;
    work = mapWork + activeGroup * 0x2F4;
    *(s32*)(work + 0x28) = type;
    *(f32*)(work + 0x2C) = start;
    *(f32*)(work + 0x30) = end;
    *(s32*)(work + 0x34) = rgba;
}


void mapBlendOff2(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] &= ~4;
}

void mapBlendOff(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] &= ~2;
}

void mapFogOff(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] &= ~1;
}

void mapFogOn(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] |= 1;
}

void mapSetMaterial(void* param_1, void* param_2) {
    extern void mapSetMaterialTev(u32 texCount, int drawMode, u32 materialFlag, s32 pMtx);
    extern void* lightGetAmbient(void);
    extern u32 dat_8041f908;
    extern void* camGetCurPtr(void);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void GXInitLightPos(void* lightObj, f32 x, f32 y, f32 z);
    extern void GXInitLightColor(void* lightObj, void* color);
    extern void GXInitLightAttn(void* lightObj, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2);
    extern void GXLoadLightObjImm(void* lightObj, u32 lightId);
    extern s32 activeGroup;
    extern s32 mapWork;
    extern u32 texwrap_tbl[];
    extern s32 mapSetLight(void* mapObj, void* lightList);
    extern u32 lightid_tbl[];
    extern s32 strcmp(char* s1, char* s2);
    extern void* TEXGet(s32 table, s32 id);
    extern f32 float_deg2rad_8041f948;
    extern f32 float_neg1p0486E06_8041f950;
    extern f32 float_0_8041f930;
    extern f32 float_1_8041f940;
    extern u8 vec3_802bf958[];
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSVECAdd(void* a, void* b, void* out);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanAmbColor(s32 chan, void* color);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern f32 float_0p5_8041f934;
    extern f32 float_1000_8041f954;
    extern void PSMTXMultVecSR(void* mtx, void* src, void* dst);
    extern void GXInitLightDir(void* lightObj, f32 x, f32 y, f32 z);
    extern void GXInitLightSpot(void* lightObj, f32 cutoff, s32 spotFn);
    extern void GXInitLightDistAttn(void* lightObj, f32 refDist, f32 refBrightness, s32 distFn);
    extern void GXSetChanCtrl(
        s32 chan,
        s32 enable,
        s32 ambSrc,
        s32 matSrc,
        u32 lightMask,
        s32 diffFn,
        s32 attnFn
    );

    extern void GXInitTexObj(
        void* obj,
        void* data,
        u16 width,
        u16 height,
        s32 format,
        u32 wrapS,
        u32 wrapT,
        s32 mipmap
    );
    extern void GXInitTexObjLOD(
        void* obj,
        s32 minFilt,
        s32 magFilt,
        f32 minLOD,
        f32 maxLOD,
        f32 lodBias,
        s32 biasClamp,
        s32 edgeLOD,
        s32 maxAniso
    );
    extern void GXLoadTexObj(void* obj, s32 mapId);

    void* ambientPtr;
    void* drawModePtr;
    u32 ambientColor;
    u32 materialColor;
    u32 texCount;
    int drawMode;
    u32 materialFlag;
    s32 pMtx;

        /* light-loop locals */
    s32 lightCount;
    u32 lightMask;
    s32 lightAttn;
    s32 lightEnable;
    u8 chanMode;
    void* lightList[16];
    s32 lightIndex;
    void* light;
    u16 lightFlags;
    u8 lightObj[0x40];
    f32 lightPos[3];
    u32 lightColor;
    void* cam;
    u8 mtxX[0x30];
    u8 mtxY[0x30];
    u8 mtxZ[0x30];
    f32 baseDir[3];

    /* texture-loop locals */
    u8 texObj[0x20];
    s32 texIndex;
    s32 loadedTexCount;
    s32 groupBase;
    s32 entryIndex;
    s32 entryBase;
    s32 texTableAddr;
    s32 texSearchIndex;
    s32 texSearchCount;
    s32 texId;
    s32 texTableEntry;
    void* texSlot;
    char* texName;
    void* tplDesc;
    void* texHeader;
    s32 mipmap;

        for (lightIndex = 0; lightIndex < lightCount; lightIndex++) {
        light = lightList[lightIndex];
        lightFlags = *(u16*)light;

        if ((lightFlags & 0x1000) != 0) {
            cam = camGetCurPtr();

            PSMTXMultVec(
                (void*)((s32)cam + 0x11C),
                (void*)((s32)light + 0x24),
                lightPos
            );

            GXInitLightPos(lightObj, lightPos[0], lightPos[1], lightPos[2]);

            lightColor = *(u32*)((s32)light + 0x48);
            GXInitLightColor(lightObj, &lightColor);

            GXInitLightAttn(
                lightObj,
                *(f32*)((s32)light + 0x50),
                0.0f,
                0.0f,
                *(f32*)((s32)light + 0x54),
                *(f32*)((s32)light + 0x58),
                *(f32*)((s32)light + 0x5C)
            );

            GXLoadLightObjImm(lightObj, lightid_tbl[lightIndex]);
            lightAttn = 1;
        } else if ((lightFlags & 0x2000) != 0) {
            baseDir[0] = *(f32*)((s32)vec3_802bf958 + 0x144);
            baseDir[1] = *(f32*)((s32)vec3_802bf958 + 0x148);
            baseDir[2] = *(f32*)((s32)vec3_802bf958 + 0x14C);

            PSMTXRotRad(
                mtxX,
                0x78,
                float_deg2rad_8041f948 * *(f32*)((s32)light + 0x30)
            );
            PSMTXRotRad(
                mtxY,
                0x79,
                float_deg2rad_8041f948 * *(f32*)((s32)light + 0x34)
            );
            PSMTXRotRad(
                mtxZ,
                0x7A,
                float_deg2rad_8041f948 * *(f32*)((s32)light + 0x38)
            );

            PSMTXConcat(mtxZ, mtxY, mtxY);
            PSMTXConcat(mtxY, mtxX, mtxX);
            PSMTXMultVec(mtxX, baseDir, baseDir);

            PSVECScale(baseDir, baseDir, float_neg1p0486E06_8041f950);
            PSVECAdd((void*)((s32)light + 0x24), baseDir, lightPos);

            cam = camGetCurPtr();
            PSMTXMultVec(
                (void*)((s32)cam + 0x11C),
                lightPos,
                lightPos
            );

            GXInitLightPos(lightObj, lightPos[0], lightPos[1], lightPos[2]);

            lightColor = *(u32*)((s32)light + 0x48);
            GXInitLightColor(lightObj, &lightColor);

            GXInitLightAttn(
                lightObj,
                *(f32*)((s32)light + 0x50),
                0.0f,
                0.0f,
                0.0f,
                0.0f,
                1.0f
            );

            GXLoadLightObjImm(lightObj, lightid_tbl[lightIndex]);
        } else if ((lightFlags & 0x4000) != 0) {
    cam = camGetCurPtr();

    PSMTXMultVec(
        (void*)((s32)cam + 0x11C),
        (void*)((s32)light + 0x24),
        lightPos
    );

    baseDir[0] = *(f32*)((s32)vec3_802bf958 + 0x144);
    baseDir[1] = *(f32*)((s32)vec3_802bf958 + 0x148);
    baseDir[2] = *(f32*)((s32)vec3_802bf958 + 0x14C);

    PSMTXRotRad(
        mtxX,
        0x78,
        float_deg2rad_8041f948 * *(f32*)((s32)light + 0x30)
    );
    PSMTXRotRad(
        mtxY,
        0x79,
        float_deg2rad_8041f948 * *(f32*)((s32)light + 0x34)
    );
    PSMTXRotRad(
        mtxZ,
        0x7A,
        float_deg2rad_8041f948 * *(f32*)((s32)light + 0x38)
    );

    PSMTXConcat(mtxZ, mtxY, mtxY);
    PSMTXConcat(mtxY, mtxX, mtxX);
    PSMTXMultVec(mtxX, baseDir, baseDir);

    cam = camGetCurPtr();
    PSMTXMultVecSR(
        (void*)((s32)cam + 0x11C),
        baseDir,
        baseDir
    );

    GXInitLightPos(lightObj, lightPos[0], lightPos[1], lightPos[2]);
    GXInitLightDir(lightObj, baseDir[0], baseDir[1], baseDir[2]);

    GXInitLightSpot(
        lightObj,
        *(f32*)((s32)light + 0x4C) * float_0p5_8041f934,
        4
    );

    GXInitLightDistAttn(
        lightObj,
        float_1000_8041f954,
        float_0p5_8041f934,
        0
    );

    lightColor = *(u32*)((s32)light + 0x48);
    GXInitLightColor(lightObj, &lightColor);

    GXLoadLightObjImm(lightObj, lightid_tbl[lightIndex]);
    lightAttn = 1;
}

        lightMask |= lightid_tbl[lightIndex];
    }

    ambientPtr = lightGetAmbient();
    if (ambientPtr != 0) {
        ambientColor = *(u32*)((s32)ambientPtr + 0x48);
    } else {
        ambientColor = dat_8041f908;
    }

    GXSetNumChans(1);

    GXSetChanAmbColor(4, &ambientColor);

    materialColor = *(u32*)((s32)param_2 + 4);
    GXSetChanMatColor(4, &materialColor);

    lightEnable = lightCount != 0;
    chanMode = *(u8*)((s32)param_2 + 8);

    switch (chanMode) {
        case 0:
            GXSetChanCtrl(0, lightEnable, 0, 0, lightMask, 2, lightAttn);
            GXSetChanCtrl(2, 0, 0, 0, 0, 0, 2);
            break;

        case 1:
            GXSetChanCtrl(0, lightEnable, 0, 1, lightMask, 2, lightAttn);
            GXSetChanCtrl(2, 0, 0, 1, 0, 0, 2);
            break;

        default:
            break;
    }

    loadedTexCount = 0;
    texIndex = *(u8*)((s32)param_2 + 0xB) - 1;

    while (texIndex >= 0) {
        texSlot = *(void**)((s32)param_2 + 0xC + texIndex * 4);
        texName = *(char**)(*(s32*)texSlot);

        entryIndex = *(s16*)((s32)param_1 + 0xDE);
        groupBase = mapWork + activeGroup * 0x2F4;
        entryBase = groupBase + entryIndex * 0x178;

        texTableAddr = *(s32*)(entryBase + 0x94);
        texSearchCount = *(s32*)texTableAddr;
        texSearchIndex = 0;

        while (texSearchIndex < texSearchCount) {
            texTableEntry = *(s32*)(texTableAddr + 4 + texSearchIndex * 4);
            if (strcmp((char*)texTableEntry, texName) == 0) {
                break;
            }
            texSearchIndex++;
        }

        if (texSearchIndex >= texSearchCount) {
            texId = -1;
        } else {
            texId = texSearchIndex;
        }

        tplDesc = TEXGet(*(s32*)(entryBase + 0x84), texId);
        texHeader = *(void**)tplDesc;

        mipmap = (*(u8*)((s32)texHeader + 0x21) != *(u8*)((s32)texHeader + 0x22));

        GXInitTexObj(
            texObj,
            *(void**)((s32)texHeader + 0x8),
            *(u16*)((s32)texHeader + 0x2),
            *(u16*)((s32)texHeader + 0x0),
            *(s32*)((s32)texHeader + 0x4),
            texwrap_tbl[*(u8*)((s32)texSlot + 0x8)],
            texwrap_tbl[*(u8*)((s32)texSlot + 0x9)],
            mipmap
        );

        GXInitTexObjLOD(
            texObj,
            1,
            1,
            (f32)*(u8*)((s32)texHeader + 0x21),
            (f32)*(u8*)((s32)texHeader + 0x22),
            *(f32*)((s32)texHeader + 0x1C),
            0,
            *(u8*)((s32)texHeader + 0x20),
            0
        );

        GXLoadTexObj(texObj, loadedTexCount);

        loadedTexCount++;
        texIndex--;
    }

    texCount = *(u8*)((s32)param_2 + 0xB);

    drawModePtr = *(void**)((s32)param_2 + 0x110);
    if (drawModePtr != 0) {
        drawMode = *(u8*)drawModePtr;
    } else {
        drawMode = 0;
    }

    materialFlag = *(u32*)((s32)param_1 + 0x0);
    pMtx = (s32)param_1 + 0x1C;

    mapSetMaterialTev(texCount, drawMode, materialFlag, pMtx);
}


void _mapLoad(void* pWork, int idx, char* pMapName) {
    ;
}


void* _mapEnt(void* pJoint, void* pParent, void* pMtx, int entryIdx) {
    return 0;
}


u8 _setOffScrnId(void* param_1, int param_2, int param_3) {
    int offId;
    int recurse;
    void* p1;
    void* p2;
    void* p3;

    offId = param_2;
    recurse = param_3;

#define SET_OFFSCRN(node_)                                      \
    do {                                                        \
        *(u16*)((s32)(node_) + 0xDC) = (u16)offId;              \
        if (offId != -1) {                                      \
            *(u32*)(node_) = *(u32*)(node_) | 0x80;             \
        } else {                                                \
            *(u32*)(node_) = *(u32*)(node_) & ~0x80;            \
        }                                                       \
    } while (0)

    SET_OFFSCRN(param_1);

    p1 = *(void**)((s32)param_1 + 0xE4);
    if (p1 != 0) {
        SET_OFFSCRN(p1);

        p2 = *(void**)((s32)p1 + 0xE4);
        if (p2 != 0) {
            SET_OFFSCRN(p2);

            p3 = *(void**)((s32)p2 + 0xE4);
            if (p3 != 0) {
                SET_OFFSCRN(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                }
            }

            p3 = *(void**)((s32)p2 + 0xE8);
            if (p3 != 0) {
                SET_OFFSCRN(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                }
            }
        }

        p2 = *(void**)((s32)p1 + 0xE8);
        if (p2 != 0) {
            SET_OFFSCRN(p2);

            p3 = *(void**)((s32)p2 + 0xE4);
            if (p3 != 0) {
                SET_OFFSCRN(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                }
            }

            p2 = *(void**)((s32)p2 + 0xE8);
            if (p2 != 0) {
                SET_OFFSCRN(p2);

                if (*(void**)((s32)p2 + 0xE4) != 0) {
                    _setOffScrnId(*(void**)((s32)p2 + 0xE4), offId, 1);
                }
                if (*(void**)((s32)p2 + 0xE8) != 0) {
                    _setOffScrnId(*(void**)((s32)p2 + 0xE8), offId, 1);
                }
            }
        }
    }

    if (recurse != 0) {
        p1 = *(void**)((s32)param_1 + 0xE8);
        if (p1 != 0) {
            SET_OFFSCRN(p1);

            p2 = *(void**)((s32)p1 + 0xE4);
            if (p2 != 0) {
                SET_OFFSCRN(p2);

                p3 = *(void**)((s32)p2 + 0xE4);
                if (p3 != 0) {
                    SET_OFFSCRN(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                    }
                }

                p3 = *(void**)((s32)p2 + 0xE8);
                if (p3 != 0) {
                    SET_OFFSCRN(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                    }
                }
            }

            p2 = *(void**)((s32)p1 + 0xE8);
            if (p2 != 0) {
                SET_OFFSCRN(p2);

                p3 = *(void**)((s32)p2 + 0xE4);
                if (p3 != 0) {
                    SET_OFFSCRN(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                    }
                }

                p2 = *(void**)((s32)p2 + 0xE8);
                if (p2 != 0) {
                    SET_OFFSCRN(p2);

                    if (*(void**)((s32)p2 + 0xE4) != 0) {
                        _setOffScrnId(*(void**)((s32)p2 + 0xE4), offId, 1);
                    }
                    if (*(void**)((s32)p2 + 0xE8) != 0) {
                        _setOffScrnId(*(void**)((s32)p2 + 0xE8), offId, 1);
                    }
                }
            }
        }
    }

#undef SET_OFFSCRN
}

u8 setColor(void* param_1, u32* param_2, int param_3) {
    void* p2;
    void* p3;
    void* p4;
    u32 color;
    u32 local_68;
    u32 local_64;
    u32 local_60;
    u32 local_5c;
    u32 local_58;
    u32 local_54;
    u32 local_50;
    u32 local_4c;
    u32 local_48;
    u32 local_44;
    u32 local_40;
    u32 local_3c;
    u32 local_38;
    u32 local_34;
    u32 local_30;
    u32 local_2c;
    u32 local_28;
    u32 local_24;

    if (param_1 != 0) {
        color = *param_2;
        *(u32*)((s32)param_1 + 0x0C) = color;
        *(u32*)param_1 = *(u32*)param_1 | 0x40;

        p4 = *(void**)((s32)param_1 + 0xE4);
        if (p4 != 0) {
            *(u32*)((s32)p4 + 0x0C) = color;
            *(u32*)p4 = *(u32*)p4 | 0x40;

            p3 = *(void**)((s32)p4 + 0xE4);
            local_24 = color;
            if (p3 != 0) {
                *(u32*)((s32)p3 + 0x0C) = color;
                *(u32*)p3 = *(u32*)p3 | 0x40;

                p2 = *(void**)((s32)p3 + 0xE4);
                if (p2 != 0) {
                    *(u32*)((s32)p2 + 0x0C) = color;
                    *(u32*)p2 = *(u32*)p2 | 0x40;

                    if (*(void**)((s32)p2 + 0xE4) != 0) {
                        local_30 = color;
                        setColor(*(void**)((s32)p2 + 0xE4), &local_30, 1);
                    }
                    if (*(void**)((s32)p2 + 0xE8) != 0) {
                        local_2c = local_24;
                        setColor(*(void**)((s32)p2 + 0xE8), &local_2c, 1);
                    }
                }

                p3 = *(void**)((s32)p3 + 0xE8);
                if (p3 != 0) {
                    *(u32*)((s32)p3 + 0x0C) = local_24;
                    *(u32*)p3 = *(u32*)p3 | 0x40;

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        local_38 = local_24;
                        setColor(*(void**)((s32)p3 + 0xE4), &local_38, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        local_34 = local_24;
                        setColor(*(void**)((s32)p3 + 0xE8), &local_34, 1);
                    }
                }
            }

            p4 = *(void**)((s32)p4 + 0xE8);
            if (p4 != 0) {
                *(u32*)((s32)p4 + 0x0C) = local_24;
                *(u32*)p4 = *(u32*)p4 | 0x40;

                p3 = *(void**)((s32)p4 + 0xE4);
                if (p3 != 0) {
                    *(u32*)((s32)p3 + 0x0C) = local_24;
                    *(u32*)p3 = *(u32*)p3 | 0x40;

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        local_40 = local_24;
                        setColor(*(void**)((s32)p3 + 0xE4), &local_40, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        local_3c = local_24;
                        setColor(*(void**)((s32)p3 + 0xE8), &local_3c, 1);
                    }
                }

                p4 = *(void**)((s32)p4 + 0xE8);
                if (p4 != 0) {
                    *(u32*)((s32)p4 + 0x0C) = local_24;
                    *(u32*)p4 = *(u32*)p4 | 0x40;

                    if (*(void**)((s32)p4 + 0xE4) != 0) {
                        local_48 = local_24;
                        setColor(*(void**)((s32)p4 + 0xE4), &local_48, 1);
                    }
                    if (*(void**)((s32)p4 + 0xE8) != 0) {
                        local_44 = local_24;
                        setColor(*(void**)((s32)p4 + 0xE8), &local_44, 1);
                    }
                }
            }
        }

        if (param_3 != 0) {
            p4 = *(void**)((s32)param_1 + 0xE8);
            if (p4 != 0) {
                local_28 = *param_2;
                *(u32*)((s32)p4 + 0x0C) = local_28;
                *(u32*)p4 = *(u32*)p4 | 0x40;

                p3 = *(void**)((s32)p4 + 0xE4);
                if (p3 != 0) {
                    *(u32*)((s32)p3 + 0x0C) = local_28;
                    *(u32*)p3 = *(u32*)p3 | 0x40;

                    p2 = *(void**)((s32)p3 + 0xE4);
                    if (p2 != 0) {
                        *(u32*)((s32)p2 + 0x0C) = local_28;
                        *(u32*)p2 = *(u32*)p2 | 0x40;

                        if (*(void**)((s32)p2 + 0xE4) != 0) {
                            local_50 = local_28;
                            setColor(*(void**)((s32)p2 + 0xE4), &local_50, 1);
                        }
                        if (*(void**)((s32)p2 + 0xE8) != 0) {
                            local_4c = local_28;
                            setColor(*(void**)((s32)p2 + 0xE8), &local_4c, 1);
                        }
                    }

                    p3 = *(void**)((s32)p3 + 0xE8);
                    if (p3 != 0) {
                        *(u32*)((s32)p3 + 0x0C) = local_28;
                        *(u32*)p3 = *(u32*)p3 | 0x40;

                        if (*(void**)((s32)p3 + 0xE4) != 0) {
                            local_58 = local_28;
                            setColor(*(void**)((s32)p3 + 0xE4), &local_58, 1);
                        }
                        if (*(void**)((s32)p3 + 0xE8) != 0) {
                            local_54 = local_28;
                            setColor(*(void**)((s32)p3 + 0xE8), &local_54, 1);
                        }
                    }
                }

                p4 = *(void**)((s32)p4 + 0xE8);
                if (p4 != 0) {
                    *(u32*)((s32)p4 + 0x0C) = local_28;
                    *(u32*)p4 = *(u32*)p4 | 0x40;

                    p3 = *(void**)((s32)p4 + 0xE4);
                    if (p3 != 0) {
                        *(u32*)((s32)p3 + 0x0C) = local_28;
                        *(u32*)p3 = *(u32*)p3 | 0x40;

                        if (*(void**)((s32)p3 + 0xE4) != 0) {
                            local_60 = local_28;
                            setColor(*(void**)((s32)p3 + 0xE4), &local_60, 1);
                        }
                        if (*(void**)((s32)p3 + 0xE8) != 0) {
                            local_5c = local_28;
                            setColor(*(void**)((s32)p3 + 0xE8), &local_5c, 1);
                        }
                    }

                    p4 = *(void**)((s32)p4 + 0xE8);
                    if (p4 != 0) {
                        *(u32*)((s32)p4 + 0x0C) = local_28;
                        *(u32*)p4 = *(u32*)p4 | 0x40;

                        if (*(void**)((s32)p4 + 0xE4) != 0) {
                            local_68 = local_28;
                            setColor(*(void**)((s32)p4 + 0xE4), &local_68, 1);
                        }
                        if (*(void**)((s32)p4 + 0xE8) != 0) {
                            local_64 = local_28;
                            setColor(*(void**)((s32)p4 + 0xE8), &local_64, 1);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

u8 mapFlgOn(void* param_1, u32 param_2, int param_3) {
    u32 bit;
    void* p1;
    void* p2;
    void* p3;

    bit = param_2 & 0x100;

#define FLG_ON(node_)                                                   \
    do {                                                                \
        if (bit == 0 || *(void**)((s32)(node_) + 0x114) != NULL) {      \
            *(u32*)(node_) = *(u32*)(node_) | param_2;                  \
        }                                                               \
    } while (0)

    FLG_ON(param_1);

    p1 = *(void**)((s32)param_1 + 0xE4);
    if (p1 != 0) {
        FLG_ON(p1);

        p2 = *(void**)((s32)p1 + 0xE4);
        if (p2 != 0) {
            FLG_ON(p2);

            p3 = *(void**)((s32)p2 + 0xE4);
            if (p3 != 0) {
                FLG_ON(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                }
            }

            p3 = *(void**)((s32)p2 + 0xE8);
            if (p3 != 0) {
                FLG_ON(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                }
            }
        }

        p2 = *(void**)((s32)p1 + 0xE8);
        if (p2 != 0) {
            FLG_ON(p2);

            p3 = *(void**)((s32)p2 + 0xE4);
            if (p3 != 0) {
                FLG_ON(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                }
            }

            p2 = *(void**)((s32)p2 + 0xE8);
            if (p2 != 0) {
                FLG_ON(p2);

                if (*(void**)((s32)p2 + 0xE4) != 0) {
                    mapFlgOn(*(void**)((s32)p2 + 0xE4), param_2, 1);
                }
                if (*(void**)((s32)p2 + 0xE8) != 0) {
                    mapFlgOn(*(void**)((s32)p2 + 0xE8), param_2, 1);
                }
            }
        }
    }

    if (param_3 != 0) {
        p1 = *(void**)((s32)param_1 + 0xE8);
        if (p1 != 0) {
            FLG_ON(p1);

            p2 = *(void**)((s32)p1 + 0xE4);
            if (p2 != 0) {
                FLG_ON(p2);

                p3 = *(void**)((s32)p2 + 0xE4);
                if (p3 != 0) {
                    FLG_ON(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                    }
                }

                p3 = *(void**)((s32)p2 + 0xE8);
                if (p3 != 0) {
                    FLG_ON(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                    }
                }
            }

            p2 = *(void**)((s32)p1 + 0xE8);
            if (p2 != 0) {
                FLG_ON(p2);

                p3 = *(void**)((s32)p2 + 0xE4);
                if (p3 != 0) {
                    FLG_ON(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                    }
                }

                p2 = *(void**)((s32)p2 + 0xE8);
                if (p2 != 0) {
                    FLG_ON(p2);

                    if (*(void**)((s32)p2 + 0xE4) != 0) {
                        mapFlgOn(*(void**)((s32)p2 + 0xE4), param_2, 1);
                    }
                    if (*(void**)((s32)p2 + 0xE8) != 0) {
                        mapFlgOn(*(void**)((s32)p2 + 0xE8), param_2, 1);
                    }
                }
            }
        }
    }

#undef FLG_ON
}

u8 mapReCalcMatrix(void* this, s32 param_2, int param_3) {
    return 0;
}


u8 mapCalcAnimMatrix(void* pDstMtx, void* pParentMtx, void* pEntry, void* pTrack) {
    return 0;
}


u8 mapSetMaterialLastStageBlend(u32 flags, void* pEvtColor, void* param_3) {
    return 0;
}


void* mapEntrySub(void* pJoint, void* pParent, void* pMtx, int isRoot, int entryIdx) {
    return 0;
}


u8 mapBuildTexture(void* param_1, int param_2, int* param_3) {
    return 0;
}


int mapSetLight(void* param_1, void** param_2) {
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern int lightGetNearObj(void* pos, void** dst, int count, int flags);
    extern u16* lightNameToPtr(char* name);
    extern s32 activeGroup;
    extern s32 mapWork;
    f32 pos[3];
    void* drawMode;
    u16* light;
    register int count;
    register int i;
    register void** out;
    register int offset;

    count = 0;
    PSMTXMultVec((void*)((s32)param_1 + 0x1C), (void*)((s32)param_1 + 0x10), pos);
    drawMode = *(void**)(*(s32*)((s32)param_1 + 8) + 0x58);
    if ((*(u8*)((s32)drawMode + 2) & 8) != 0) {
        if (*(u32*)((s32)drawMode + 4) == 0) {
            count = lightGetNearObj(pos, param_2, 8, 0);
        } else {
            out = param_2;
            i = 0;
            offset = 0;
            do {
                if ((*(u32*)(*(s32*)(*(s32*)((s32)param_1 + 8) + 0x58) + 4) & (1 << i)) != 0) {
                    void* table = *(void**)(mapWork + activeGroup * 0x2F4 + *(s16*)((s32)param_1 + 0xDE) * 0x178 + 0x98);
                    light = lightNameToPtr(*(char**)*(s32*)((s32)table + offset + 4));
                    if ((*light & 0x8000) == 0) {
                        count++;
                        *out = light;
                        out++;
                        if (count >= 8) {
                            break;
                        }
                    }
                }
                i++;
                offset += 4;
            } while (i < 0x20);
            if (count < 8) {
                count += lightGetNearObj(pos, param_2 + count, 8 - count, 0);
            }
        }
    } else {
        if (*(u32*)((s32)drawMode + 4) != 0) {
            i = 0;
            out = param_2;
            offset = 0;
            do {
                if ((*(u32*)(*(s32*)(*(s32*)((s32)param_1 + 8) + 0x58) + 4) & (1 << i)) != 0) {
                    void* table = *(void**)(mapWork + activeGroup * 0x2F4 + *(s16*)((s32)param_1 + 0xDE) * 0x178 + 0x98);
                    light = lightNameToPtr(*(char**)*(s32*)((s32)table + offset + 4));
                    if ((*light & 0x8000) == 0) {
                        count++;
                        *out = light;
                        out++;
                        if (count >= 8) {
                            break;
                        }
                    }
                }
                i++;
                offset += 4;
            } while (i < 0x20);
        }
    }
    return count;
}

void _mapDispMapObj_NoMaterial(s32 param_1, void* param_2) {
    ;
}


u8 mapObjSetColor(char* param_1, void* param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) == 0) {
                goto done;
            }
            objIndex++;
            obj = (void*)((s32)obj + 0x134);
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj != NULL) {
        *(u32*)((s32)obj + 0x0C) = *(u32*)param_2;
        *(u32*)obj = *(u32*)obj | 0x40;
    }
}

s32 mapTestXLU(u32 materialFlag, void* param_2, void* param_3) {
    extern s32 activeGroup;
    extern s32 mapWork;
    void* work;

    if ((materialFlag & 0x40) && (*(u8*)((s32)param_2 + 3) != 0xFF)) {
        return 1;
    }

    if (*(u8*)((s32)param_3 + 3) != 0xFF) {
        return 1;
    }

    if ((materialFlag & 0x800) == 0) {
        work = (void*)(mapWork + activeGroup * 0x2F4);
        if ((*(u16*)((s32)work + 4) & 2) && (*(u8*)((s32)work + 0x19) != 0xFF)) {
            return 1;
        }
    }

    if ((materialFlag & 0x1000) == 0) {
        work = (void*)(mapWork + activeGroup * 0x2F4);
        if ((*(u16*)((s32)work + 4) & 4) && (*(u8*)((s32)work + 0x1D) != 0xFF)) {
            return 1;
        }
    }

    return 0;
}

void* mapSearchAnmObj(char* name) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    void* work;
    void* group;
    void* anim;
    s32 i;
    s32 j;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            if ((*(u16*)anim & 1) != 0) {
                if (strcmp(*(const char**)*(s32*)((s32)anim + 0x18), name) == 0) {
                    return anim;
                }
            }
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }

    return 0;
}


void* mapEntry(void* pJoint, void* pMtx, int entryIdx) {
    return 0;
}
