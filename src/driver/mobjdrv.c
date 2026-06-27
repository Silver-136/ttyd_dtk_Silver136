#include "driver/mobjdrv.h"

typedef struct MobjSet {
    s32 count;
    void* entries;
    u8 pad8[0x14];
} MobjSet;

typedef struct MobjWork {
    MobjSet normal;
    MobjSet alt;
    MobjSet koopa;
} MobjWork;

extern MobjWork work;
extern void* gp;
extern s32 koopaRunFlag;
extern void* mapalloc_base_ptr;

void* memset(void* dst, int value, u32 size);
void* _mapAlloc(void* heap, u32 size);

s32 mobjGetHint(void* mobj) {
    if (mobj == 0) {
        return 0;
    }
    return (*(u32*)mobj >> 5) & 1;
}

void* mobjHitObjPtrToPtr(void* hitObj) {
    if (*(u32*)((s32)hitObj + 4) & 0x80000000) {
        return *(void**)((s32)hitObj + 0xD0);
    }
    return 0;
}

void mobjReset(s32 alt) {
    MobjSet* set = &work.normal;

    if (alt != 0) {
        set = &work.alt;
    }
    memset(set->entries, 0, set->count * 0x23C);
    koopaRunFlag = 0;
}

void mobjKoopaOn(void) {
    if (koopaRunFlag == 0) {
        MobjSet* set = &work.koopa;

        koopaRunFlag = 1;
        set->count = 0x80;
        set->entries = _mapAlloc(mapalloc_base_ptr, set->count * 0x23C);
        memset(set->entries, 0, set->count * 0x23C);
    }
}

s32 mobjCheckExec(void) {
    MobjSet* set;
    s32 i;
    void* entry;

    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        set = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            set = &work.alt;
        }
    }

    entry = set->entries;
    for (i = 0; i < set->count; i++, entry = (void*)((s32)entry + 0x23C)) {
        if (*(u32*)entry & 0x200) {
            return 1;
        }
    }
    return 0;
}


u8 mobjMain(void) {
    return 0;
}


u32* mobjNearDistCheck2(double param_1, double param_2, double param_3, double param_4, char** param_5) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern MobjWork work;
    extern void* gp;
    extern s32 koopaRunFlag;
    extern s32 strcmp(const char* a, const char* b);
    extern void* itemNameToPtr(char* name);
    extern u32 evtGetValue(void* event, s32 value);
    extern void PSVECSubtract(Vec3* a, Vec3* b, Vec3* out);
    extern f32 PSVECMag(Vec3* vec);
    extern const char str_MOBJ_TreasureBox_802c1fe4[];
    extern const char str_MOBJ_BigTreasureBox_802c1ff8[];
    extern const char str_MOBJ_GrayTreasureBox_802c200c[];
    extern const char str_MOBJ_BlackTreasureBo_802c2024[];
    extern const char str_MOBJ_KururinFloor_802c1fd0[];
    MobjSet* set;
    s32 i;
    s32 count;
    u32* entry;
    u32* best;
    char** list;
    Vec3 pos;
    Vec3 diff;
    f32 dist;
    s32 check;
    void* item;
    u32 value;

    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        set = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            set = &work.alt;
        }
    }

    best = 0;
    i = 0;
    count = set->count;
    entry = (u32*)set->entries;
    pos.x = (f32)param_1;
    pos.y = (f32)param_2;
    pos.z = (f32)param_3;

    while (i < count) {
        if ((*entry & 1) != 0) {
            list = param_5;
            while (*list != 0) {
                if (strcmp((char*)((s32)entry + 0x15), *list) == 0) {
                    if (strcmp((char*)((s32)entry + 0x15), str_MOBJ_TreasureBox_802c1fe4) == 0 ||
                        strcmp((char*)((s32)entry + 0x15), str_MOBJ_BigTreasureBox_802c1ff8) == 0 ||
                        strcmp((char*)((s32)entry + 0x15), str_MOBJ_GrayTreasureBox_802c200c) == 0 ||
                        strcmp((char*)((s32)entry + 0x15), str_MOBJ_BlackTreasureBo_802c2024) == 0) {
                        if (*(u32*)((s32)entry + 0x1DC) == 99) {
                            check = 2;
                        } else {
                            check = 1;
                        }
                        if (check != 1) {
                            list++;
                            continue;
                        }
                    } else if (strcmp((char*)((s32)entry + 0x15), str_MOBJ_KururinFloor_802c1fd0) == 0) {
                        check = 0;
                        if (strcmp((char*)((s32)entry + 0x15), str_MOBJ_KururinFloor_802c1fd0) == 0) {
                            item = itemNameToPtr((char*)((s32)entry + 5));
                            if (item == 0 || ((*(u16*)item & 1) == 0) || evtGetValue(0, *(s32*)((s32)entry + 0x1E4)) != 0) {
                                value = evtGetValue(0, *(s32*)((s32)entry + 0x1E4));
                                if (value == 0) {
                                    check = 2;
                                } else {
                                    check = 1;
                                }
                            } else {
                                check = 1;
                            }
                        }
                        if (check != 2) {
                            list++;
                            continue;
                        }
                    }

                    PSVECSubtract((Vec3*)((s32)entry + 0x38), &pos, &diff);
                    dist = PSVECMag(&diff);
                    if ((double)dist < param_4) {
                        best = entry;
                        param_4 = (double)dist;
                    }
                    break;
                }
                list++;
            }
        }
        i++;
        entry = (u32*)((s32)entry + 0x23C);
    }
    return best;
}

int mobjEntry(char* name, char* pAnimPoseName) {
    typedef struct MobjEntryLocal {
        u32 flags;
        u8 camId;
        char name[16];
        char animPoseName[16];
    } MobjEntryLocal;
    extern MobjWork work;
    extern void* gp;
    extern s32 koopaRunFlag;
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    extern int animPoseEntry(char* name, u32 inBattle);
    extern void animPosePeraOff(s32 poseId);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void animPoseSetMaterialLightFlagOn(s32 poseId, u32 flags);
    extern void mobjHitEntry(void* pMobj, s32 param_2);
    extern char vec3_802c1f58[];
    extern f32 float_1_804201c0;
    extern const char str_S_1_804201ec;
    MobjSet* set;
    MobjEntryLocal* entry;
    s32 count;
    s32 index;
    s32 poseId;
    s32 battle;

    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        set = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            set = &work.alt;
        }
    }

    count = set->count;
    index = 0;
    entry = (MobjEntryLocal*)set->entries;
    while (index < count) {
        if ((entry->flags & 1) && strcmp(entry->name, name) == 0) {
            break;
        }
        index++;
        entry = (MobjEntryLocal*)((s32)entry + 0x23C);
    }

    entry = (MobjEntryLocal*)set->entries;
    index = 0;
    if (count > 0) {
        do {
            if ((entry->flags & 1) == 0) {
                break;
            }
            index++;
            entry = (MobjEntryLocal*)((s32)entry + 0x23C);
            count--;
        } while (count != 0);
    }

    memset(entry, 0, 0x23C);
    entry->flags |= 1;
    strcpy(entry->name, name);
    *(u32*)((s32)entry + 0x38) = *(u32*)((s32)vec3_802c1f58 + 0x18);
    *(u32*)((s32)entry + 0x3C) = *(u32*)((s32)vec3_802c1f58 + 0x1C);
    *(u32*)((s32)entry + 0x40) = *(u32*)((s32)vec3_802c1f58 + 0x20);
    *(u32*)((s32)entry + 0x44) = *(u32*)((s32)vec3_802c1f58 + 0x24);
    *(u32*)((s32)entry + 0x48) = *(u32*)((s32)vec3_802c1f58 + 0x28);
    *(u32*)((s32)entry + 0x4C) = *(u32*)((s32)vec3_802c1f58 + 0x2C);
    *(u32*)((s32)entry + 0x50) = *(u32*)((s32)vec3_802c1f58 + 0x30);
    *(u32*)((s32)entry + 0x54) = *(u32*)((s32)vec3_802c1f58 + 0x34);
    *(u32*)((s32)entry + 0x58) = *(u32*)((s32)vec3_802c1f58 + 0x38);
    *(u32*)((s32)entry + 0x5C) = *(u32*)((s32)vec3_802c1f58 + 0x3C);
    *(u32*)((s32)entry + 0x60) = *(u32*)((s32)vec3_802c1f58 + 0x40);
    *(u32*)((s32)entry + 0x64) = *(u32*)((s32)vec3_802c1f58 + 0x44);
    *(f32*)((s32)entry + 0x68) = float_1_804201c0;
    *(f32*)((s32)entry + 0x6C) = float_1_804201c0;
    *(u8*)((s32)entry + 4) = 4;
    battle = *(s32*)((s32)gp + 0x14);
    poseId = animPoseEntry(pAnimPoseName, ((u32)(-battle) | (u32)battle) >> 31);
    *(s32*)((s32)entry + 0x70) = poseId;
    animPosePeraOff(*(s32*)((s32)entry + 0x70));
    strcpy(entry->animPoseName, pAnimPoseName);
    animPoseSetAnim(*(s32*)((s32)entry + 0x70), &str_S_1_804201ec, 1);
    animPoseSetMaterialLightFlagOn(*(s32*)((s32)entry + 0x70), 2);
    mobjHitEntry(entry, 0);
    *(s32*)((s32)entry + 0x19C) = -1;
    return index;
}

u8 mobjDisp_OffscreenXLU(s32 param_1, u32* param_2) {
    return 0;
}


u8 mobjDisp(s32 param_1, u32* param_2) {
    return 0;
}


u8 mobjDispXLU(s32 param_1, u32* param_2) {
    return 0;
}


u8 mobjCalcMtx2(int param_1) {
    return 0;
}


u8 mobjSetPosition(float posX, float posY, float posZ, s32 mobjName) {
    return 0;
}


void mobjDelete(s32 name) {
    ;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* mobjNameToPtrNoAssert(char* name) {
    extern MobjWork work;
    extern void* gp;
    extern s32 koopaRunFlag;
    extern s32 strcmp(const char* a, const char* b);
    MobjSet* set;
    s32 i;
    s32 count;
    void* entry;
    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        set = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            set = &work.alt;
        }
    }
    count = set->count;
    i = 0;
    entry = set->entries;
    while (i < count) {
        if ((*(u32*)entry & 1) && strcmp((char*)((s32)entry + 5), name) == 0) {
            break;
        }
        i++;
        entry = (void*)((s32)entry + 0x23C);
    }
    if (i >= count) {
        return 0;
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* mobjNameToPtr(char* name) {
    extern MobjWork work;
    extern void* gp;
    extern s32 koopaRunFlag;
    extern s32 strcmp(const char* a, const char* b);
    MobjSet* set;
    s32 count;
    s32 i;
    void* entry;
    if (koopaRunFlag != 0) {
        set = &work.koopa;
    } else {
        set = &work.normal;
        if (*(s32*)((s32)gp + 0x14) != 0) {
            set = &work.alt;
        }
    }
    count = set->count;
    i = 0;
    entry = set->entries;
    while (i < count) {
        if ((*(u32*)entry & 1) && strcmp((char*)((s32)entry + 5), name) == 0) {
            break;
        }
        i++;
        entry = (void*)((s32)entry + 0x23C);
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

int mobjCheckItemboxOpen(int param_1) {
    return 0;
}


u8 mobjInit(void) {
    return 0;
}


void mobjCalcMtx(void* pMobj) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef f32 Mtx34[3][4];
    typedef struct AnimBaseLocal {
        s32 size;
        char fileName[64];
        char texFileName[64];
        char buildTime[64];
        s32 dispModeMask;
        s32 radius;
        s32 height;
        Vec3 bboxMin;
        Vec3 bboxMax;
    } AnimBaseLocal;
    extern void PSVECAdd(Vec3* a, Vec3* b, Vec3* out);
    extern void* animPoseGetAnimBaseDataPtr(s32 poseId);
    extern u32 strlen(const char* s);
    extern s32 strncmp(const char* a, const char* b, u32 n);
    extern void PSMTXTrans(Mtx34 m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx34 m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx34 m, char axis, f32 rad);
    extern void PSMTXConcat(Mtx34 a, Mtx34 b, Mtx34 out);
    extern void hitReCalcMatrix(void* hitEntry, Mtx34 mtx);
    extern f32 __fabsf(f32 value);
    extern f32 float_10_804201bc;
    extern f32 float_0_804201b8;
    extern f32 float_2p5_804201d8;
    extern f32 float_deg2rad_804201cc;
    extern const char str_HOBJ_804201d0[];
    Vec3 trans;
    Mtx34 transMtx;
    Mtx34 scaleMtx;
    Mtx34 rotXMtx;
    Mtx34 rotYMtx;
    Mtx34 rotZMtx;
    Mtx34 outMtx;
    AnimBaseLocal* base;
    s32 i;

    PSVECAdd((Vec3*)((s32)pMobj + 0x38), (Vec3*)((s32)pMobj + 0x190), &trans);
    base = animPoseGetAnimBaseDataPtr(*(s32*)((s32)pMobj + 0x70));
    *(f32*)((s32)pMobj + 0x184) = float_10_804201bc * *(f32*)((s32)pMobj + 0x44) * __fabsf(base->bboxMax.x - base->bboxMin.x);
    *(f32*)((s32)pMobj + 0x188) = float_10_804201bc * *(f32*)((s32)pMobj + 0x48) * __fabsf(base->bboxMax.y);
    *(f32*)((s32)pMobj + 0x18C) = float_10_804201bc * *(f32*)((s32)pMobj + 0x4C) * __fabsf(base->bboxMax.z - base->bboxMin.z);

    if (strncmp(base->fileName, str_HOBJ_804201d0, strlen(str_HOBJ_804201d0)) == 0) {
        *(f32*)((s32)pMobj + 0x198) = float_0_804201b8;
        *(f32*)((s32)pMobj + 0x18C) = float_10_804201bc * float_2p5_804201d8 * *(f32*)((s32)pMobj + 0x4C);
    }

    PSMTXTrans(transMtx, trans.x, trans.y, trans.z);
    PSMTXScale(scaleMtx, *(f32*)((s32)pMobj + 0x184), *(f32*)((s32)pMobj + 0x188), *(f32*)((s32)pMobj + 0x18C));
    PSMTXRotRad(rotXMtx, 'x', float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x50));
    PSMTXRotRad(rotYMtx, 'y', float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x54));
    PSMTXRotRad(rotZMtx, 'z', float_deg2rad_804201cc * *(f32*)((s32)pMobj + 0x58));
    PSMTXConcat(transMtx, rotZMtx, transMtx);
    PSMTXConcat(transMtx, rotYMtx, transMtx);
    PSMTXConcat(transMtx, rotXMtx, transMtx);
    PSMTXConcat(transMtx, scaleMtx, outMtx);

    for (i = 0; i < 2; i++) {
        if (*(void**)((s32)pMobj + 0x74) != 0) {
            hitReCalcMatrix(*(void**)((s32)pMobj + 0x74), outMtx);
        }
        pMobj = (void*)((s32)pMobj + 0x88);
    }
}

u8 mobjHitEntry(void* pMobj, int param_2) {
    return 0;
}


void* mobjRunEvent(void* pMobj, void* evtCode) {
    extern s32 evtCheckID(s32 id);
    extern void* evtEntryType(void* evtCode, s32 priority, s32 flags, s32 type);
    void* event;

    if (evtCode == 0) {
        return 0;
    }
    if (evtCheckID(*(s32*)((s32)pMobj + 0x1CC)) != 0) {
        return 0;
    }
    *(s32*)((s32)pMobj + 0x1CC) = 0;
    event = evtEntryType(evtCode, 0x1E, 0, 0x1A);
    *(void**)((s32)event + 0x174) = pMobj;
    *(s32*)((s32)pMobj + 0x1CC) = *(s32*)((s32)event + 0x15C);
    return event;
}

