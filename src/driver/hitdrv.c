#include "driver/hitdrv.h"
#include "addr_word.h"
#include "hwaccess.h"
#include <dolphin/os/OSFastCast.h>
#include "dolphin/os.h"
#include "driver/mapdrv.h"
#include "memory.h"
#include "party.h"
#include <string.h>

// forward declarations: these functions are defined further down
void hitDamageReturnSet(HitEntry* hit, HitDamageReturn* damage, BOOL recursion);
void hitObjGetPosSub(HitEntry* hit, Vec* position, s32* counter, BOOL recursion);
BOOL chkFilterAttr(HitCheckQuery* query, HitEntry* entry);
BOOL checkTriVec_xz(HitCheckQuery* query, HitVector* vector);
s32 hitCalcVtxPosition(HitEntry* hit);
void hitReCalcMatrix2(HitEntry* hit, Mtx mtx, BOOL recursion);
void hitAtrOff(HitEntry* hit, u32 mask, s32 r);
void hitAtrOn(HitEntry* hit, u32 mask, s32 r);
void hitFlgOff(HitEntry* hit, u16 mask, s32 r);
void hitFlgOn(HitEntry* hit, u16 mask, s32 r);
HitEntry* hitEntrySub(MapFileJoint* joint, HitEntry* parent, Mtx parentMtx, BOOL ignoreSiblings, s32 mapIndex);
HitEntry* _hitEnt(MapFileJoint* joint, HitEntry* parent, Mtx parentMtx, s32 mapIndex);

s32 unk_8041840C;
HitEntry* hitActiveListTop; // head of the active hit list (descriptive name, see docs/NAME_PROVENANCE.md)

// local prototypes

inline BOOL tempfunc(HitCheckQuery* query, HitVector* vector);
inline BOOL tempfunc2(HitCheckQuery* query, HitVector* vector);
inline BOOL tempfunc3(HitCheckQuery* query, HitVector* vector);

void hitInit(void) {
    ; // stubbed in retail
}

void hitReInit(void) {
    MapEntry* entry;
    MapWork* wp;
    s32 j;
    s32 i;
    HitEntry* hit;

    wp = mapGetWork();
    entry = wp->entries;
    for (i = 0; i < wp->entries->count; entry++, i++) {
        hit = entry->hitEntries;
        if (hit != NULL) {
            for (j = 0; j < entry->hitNumJoints; j++, hit++) {
                if (hit->vectors != NULL) {
                    _mapFree(hit->vectors);
                }
            }
            _mapFree(entry->hitEntries);
            entry->hitEntries = NULL;
            entry->hitNumJoints = 0;
        }
    }
}

HitEntry* _hitEnt(MapFileJoint* joint, HitEntry* parent, Mtx parentMtx, s32 mapIndex) {
    MapWork* wp = mapGetWork();
    HitEntry* hit = wp->entries[mapIndex].hitEntries;
    Mtx trans, scale, rotX, rotY, rotZ;
    Vec vtmp;
    s32 i;

    for (i = 0; i < wp->entries[mapIndex].hitNumJoints; hit++, i++) {
        if (!hit->joint) {
            break;
        }
    }

    hit->flags = 0;
    hit->joint = joint;
    hit->mapIndex = mapIndex;

    MTXTrans(trans, joint->translation.x, joint->translation.y, joint->translation.z);
    MTXScale(scale, joint->scale.x, joint->scale.y, joint->scale.z);
    MTXRotRad(rotX, 'x', MTXDegToRad(joint->rotation.x));
    MTXRotRad(rotY, 'y', MTXDegToRad(joint->rotation.y));
    MTXRotRad(rotZ, 'z', MTXDegToRad(joint->rotation.z));
    MTXConcat(trans, rotZ, trans);
    MTXConcat(trans, rotY, trans);
    MTXConcat(trans, rotX, trans);
    MTXConcat(trans, scale, hit->localMtx);
    MTXConcat(parentMtx, hit->localMtx, hit->worldMtx);

    hit->parent = parent;
    hit->sibling = NULL;
    hit->child = NULL;
    hit->vectors = NULL;

    if (joint->drawMode != NULL) {
        hit->attributes = joint->drawMode->hitAttributes;
    } else {
        hit->attributes = 0;
    }

    if (joint->partCount) {
        VECAdd(&joint->bboxMin, &joint->bboxMax, &hit->centerLocal);
        VECScale(&hit->centerLocal, &hit->centerLocal, 0.5f);
    } else {
        hit->centerLocal = joint->translation;
    }
    MTXMultVec(hit->worldMtx, &hit->centerLocal, &hit->centerWorld);

    joint->bboxMin.x -= 0.1f;
    joint->bboxMin.y -= 0.1f;
    joint->bboxMin.z -= 0.1f;
    joint->bboxMax.x += 0.1f;
    joint->bboxMax.y += 0.1f;
    joint->bboxMax.z += 0.1f;

    MTXMultVec(hit->worldMtx, &hit->joint->bboxMax, &vtmp);
    hit->radius = PSVECDistance(&vtmp, &hit->centerWorld);

    {
        s32 numVectors, j;
        s32 partIdx;
        for (numVectors = 0, partIdx = 0; partIdx < hit->joint->partCount; partIdx++) {
            MapFileMesh* mesh = hit->joint->parts[partIdx].mesh;
            if (mesh == NULL) {
                continue;
            }

            if (!mesh->isPackedDisplay) {
                for (j = 0; j < mesh->displayListCount; j++) {
                    numVectors += mesh->displayLists.normalLists[j]->count - 2;
                }
            } else {
                // FAKE: i (dead after the joint search) as the packed counter and j holding the list offset: their later webs become the @ temps that give the target colouring
                for (i = 0; i < mesh->displayListCount; i++) {
                    j = mesh->displayLists.packedLists[i].offset;
                    numVectors += *(u16*)((AddrWord)j + 1) - 2;
                }
            }
        }

        if (numVectors > 0) {
            hit->vectors = _mapAlloc(sizeof(HitVector) * numVectors);
            hit->vectorCount = hitCalcVtxPosition(hit);
        }
    }
    return hit;
}

HitEntry* hitEntrySub(MapFileJoint* joint, HitEntry* parent, Mtx parentMtx, BOOL ignoreSiblings, s32 mapIndex) {
    HitEntry* entry = _hitEnt(joint, parent, parentMtx, mapIndex);
    if (joint->child != NULL) {
        entry->child = hitEntrySub(joint->child, entry, entry->worldMtx, FALSE, mapIndex);
    }
    if (!ignoreSiblings && joint->next != NULL) {
        entry->sibling = hitEntrySub(joint->next, parent, parentMtx, FALSE, mapIndex);
    }
    return entry;
}

HitEntry* hitEntry(MapFileJoint* root, Mtx rootMtx, s32 mapIndex) {
    MapWork* wp = mapGetWork();
    u32 size;

    wp->entries[mapIndex].hitNumJoints = mapGetJoints(root);
    size = OSRoundUp32B(((wp->entries[mapIndex].hitNumJoints + 128) * sizeof(HitEntry)));
    wp->entries[mapIndex].hitEntries = _mapAlloc(size);
    memset(wp->entries[mapIndex].hitEntries, 0, size);
    return hitEntrySub(root, NULL, rootMtx, TRUE, mapIndex);
}

HitEntry* hitEntryMOBJ(MapFileJoint* joint, Mtx mtx) {
    MapWork* wp = mapGetWork();

    wp->entries[0].hitNumJoints += mapGetJoints(joint);
    return hitEntrySub(joint, NULL, mtx, TRUE, 0);
}

void mapCalcAnimMatrix(Mtx out, Mtx in, void* anim, void* joint);

typedef struct HitAnimJointList {
    s32 count;
    void* joints[1];
} HitAnimJointList;

inline void hitPreCheck(void) {
    MapEntry* entry;
    s32 i;
    s32 j;
    MapWork* wp = mapGetWork();
    HitEntry* hit;
    HitEntry* prev;

    prev = hitActiveListTop = NULL;
    i = 0;
    entry = wp->entries;
    for (; i < wp->entries->count; entry++, i++) {
        hit = entry->hitEntries;
        for (j = 0; j < entry->hitNumJoints; j++, hit++) {
            if (hit->joint->partCount != 0 && !(hit->flags & (HIT_FLAG_OFF | HIT_FLAG_DELETED))) {
                if (prev == NULL) {
                    hitActiveListTop = hit;
                    prev = hit;
                } else {
                    prev->siblingActive = hit;
                    prev = hit;
                }
            }
        }
    }
    if (prev != NULL) {
        prev->siblingActive = NULL;
    }
}
inline HitEntry* hitNameToPtrNoChk(const char* name) {
    MapWork* wp = mapGetWork();
    MapEntry* map;
    HitEntry* hit;
    s32 i, j;

    map = wp->entries;
    for (i = 0; i < wp->entries->count; map++, i++) {
        hit = map->hitEntries;
        for (j = 0; j < map->hitNumJoints; j++, hit++) {
            if (!(hit->flags & HIT_FLAG_DELETED) && !strcmp(hit->joint->name, name)) {
                return hit;
            }
        }
    }
    return NULL;
}

void hitDelete(const char* name) {
    MapWork* wp = mapGetWork();
    s32 j;
    s32 i;

    for (i = 0; i < wp->entries->count; i++) {
        HitEntry* hit;

        for (j = 0, hit = wp->entries[i].hitEntries; j < wp->entries[i].hitNumJoints; j++, hit++) {
            if (hit->flags & HIT_FLAG_DELETED) {
                continue;
            }
            if (strcmp(name, hit->joint->name) == 0) {
                hit->flags |= HIT_FLAG_DELETED;
                hitPreCheck();
                return;
            }
        }
    }
}

void hitMain(void) {
    void** joints;
    MapEntry* entry;
    MapWork* wp = mapGetWork();
    s32 j;
    s32 k;
    s32 i;
    MapEntryAnimData* anim;
    BOOL recalc;
    HitAnimJointList* list;
    HitEntry* hit;
    Mtx mtx;

    hitPreCheck();
    entry = wp->entries;
    for (i = 0; i < wp->entries->count; entry++, i++) {
        anim = entry->animData;
        for (recalc = FALSE, j = 0; j < entry->numAnims; j++, anim++) {
            list = (HitAnimJointList*)anim->anim->jointAnims;
            if (!(anim->flags & 1)) {
                continue;
            }
            if ((anim->flags & 2) && (anim->flags & 0x1000)) {
                continue;
            }
            if (list == NULL) {
                continue;
            }
            joints = (void**)list;
            for (k = 0; k < list->count; joints++, k++) {
                hit = hitNameToPtr(*(const char**)joints[1]);
                if (hit != NULL) {
                    mapCalcAnimMatrix(hit->animMtx, hit->localMtx, anim, joints[1]);
                    recalc = TRUE;
                    hit->flags |= HIT_FLAG_ANIM_MTX;
                    hit->flags |= HIT_FLAG_MTX_DIRTY;
                    anim->flags |= 0x1000;
                }
            }
        }
        if (recalc) {
            PSMTXScale(mtx, 10.0f, 10.0f, 10.0f);
            hitReCalcMatrix2(entry->rootHitObj, mtx, FALSE);
        }
    }
}


void hitObjFlagOn(const char* name, u16 mask) {
    HitEntry* hit = hitNameToPtr(name);
    if (hit == NULL) {
        mapErrorEntry(1, name);
        return;
    }
    hit->flags |= mask;
}

void hitObjFlagOff(const char* name, u16 mask) {
    HitEntry* hit = hitNameToPtr(name);
    if (hit == NULL) {
        mapErrorEntry(1, name);
        return;
    }
    hit->flags &= ~mask;
}

void hitFlgOn(HitEntry* hit, u16 mask, s32 r) {
    hit->flags |= mask;
    if (hit->child != NULL) {
        hitFlgOn(hit->child, mask, 1);
    }
    if (r != 0 && hit->sibling != NULL) {
        hitFlgOn(hit->sibling, mask, 1);
    }
}

void hitFlgOff(HitEntry* hit, u16 mask, s32 r) {
    hit->flags &= ~mask;
    if (hit->child != NULL) {
        hitFlgOff(hit->child, mask, 1);
    }
    if (r != 0 && hit->sibling != NULL) {
        hitFlgOff(hit->sibling, mask, 1);
    }
}

void hitGrpFlagOn(const char* name, u16 mask) {
    HitEntry* hit = hitNameToPtr(name);
    if (hit == NULL) {
        mapErrorEntry(1, name);
        return;
    }
    hitFlgOn(hit, mask, 0);
}

void hitGrpFlagOff(const char* name, u16 mask) {
    HitEntry* hit = hitNameToPtr(name);
    if (hit == NULL) {
        mapErrorEntry(1, name);
        return;
    }
    hitFlgOff(hit, mask, 0);
}

void hitObjAttrOn(const char* name, u32 mask) {
    HitEntry* hit = hitNameToPtr(name);
    if (hit == NULL) {
        mapErrorEntry(1, name);
        return;
    }
    hit->attributes |= mask;
}

void hitObjAttrOff(const char* name, u32 mask) {
    HitEntry* hit = hitNameToPtr(name);
    if (hit == NULL) {
        mapErrorEntry(1, name);
        return;
    }
    hit->attributes &= ~mask;
}

void hitAtrOn(HitEntry* hit, u32 mask, s32 r) {
    hit->attributes |= mask;
    if (hit->child != NULL) {
        hitAtrOn(hit->child, mask, 1);
    }
    if (r != 0 && hit->sibling != NULL) {
        hitAtrOn(hit->sibling, mask, 1);
    }
}

void hitAtrOff(HitEntry* hit, u32 mask, s32 r) {
    hit->attributes &= ~mask;
    if (hit->child != NULL) {
        hitAtrOff(hit->child, mask, 1);
    }
    if (r != 0 && hit->sibling != NULL) {
        hitAtrOff(hit->sibling, mask, 1);
    }
}

void hitGrpAttrOn(const char* name, u32 mask) {
    HitEntry* hit = hitNameToPtr(name);
    if (hit == NULL) {
        mapErrorEntry(1, name);
        return;
    }
    hitAtrOn(hit, mask, 0);
}

void hitGrpAttrOff(const char* name, u32 mask) {
    HitEntry* hit = hitNameToPtr(name);
    if (hit == NULL) {
        mapErrorEntry(1, name);
        return;
    }
    hitAtrOff(hit, mask, 0);
}

void hitReCalcMatrix(HitEntry* hit, Mtx mtx) {
    Vec world;

    hit->flags |= HIT_FLAG_MTX_DIRTY;
    hitReCalcMatrix2(hit, mtx, FALSE);
    MTXMultVec(hit->worldMtx, &hit->joint->bboxMax, &world);
    hit->radius = PSVECDistance(&world, &hit->centerWorld);
}

void hitReCalcMatrix2(HitEntry* hit, Mtx mtx, BOOL recursion) {
    int doCalc;

    doCalc = 0;
    if (hit->flags & HIT_FLAG_MTX_DIRTY) {
        doCalc = 1;
    }
    if (recursion != 0) {
        doCalc = 1;
    }
    if (doCalc != 0) {
        if (hit->flags & HIT_FLAG_ANIM_MTX) {
            PSMTXConcat(mtx, hit->animMtx, hit->worldMtx);
        } else {
            PSMTXConcat(mtx, hit->localMtx, hit->worldMtx);
        }
        if (hit->flags & HIT_FLAG_MAPOBJ_BIND) {
            PSMTXConcat(hit->worldMtx, hit->mapObj->transformMtx, hit->worldMtx);
        }
        PSMTXMultVec(hit->worldMtx, &hit->centerLocal, &hit->centerWorld);
        hitCalcVtxPosition(hit);
        hit->flags &= ~HIT_FLAG_MTX_DIRTY;
        if (hit->child != NULL) {
            hitReCalcMatrix2(hit->child, hit->worldMtx, TRUE);
        }
        if (hit->sibling != NULL) {
            hitReCalcMatrix2(hit->sibling, mtx, recursion);
        }
    } else {
        if (hit->child != NULL) {
            hitReCalcMatrix2(hit->child, hit->worldMtx, recursion);
        }
        if (hit->sibling != NULL) {
            hitReCalcMatrix2(hit->sibling, mtx, recursion);
        }
    }
}

static inline f32 hitDequant(s16* base, s32 idx) {
    return __OSs16tof32(base + idx);
}

s32 hitCalcVtxPosition(HitEntry* hit) {
    s32 i;
    Vec a;
    MapWork* wp;
    Vec b;
    Vec c;
    s32 j;
    s32 k;
    s32 stride;
    s32 count = 0;
    MapFileMesh* mesh;

    // FAKE: NULL check through stride gives stride an early web (the packed stride becomes a split @ temp, target r19);
    // the dead int guard (COLOURING M) keeps stride as the compared operand so the pointer compare stays cmplwi
    stride = (s32)(AddrWord)hit->vectors;
    i = 0;
    if (i != 0) {
        stride = stride;
    }
    if ((void*)(AddrWord)stride == NULL) {
        return 0;
    }
    for (i = 0; i < hit->joint->partCount; i++) {
        mesh = hit->joint->parts[i].mesh;
        if (mesh == NULL) {
            continue;
        }
        if (!mesh->isPackedDisplay) {
            MapRawDL* list;
            s16 i1;
            s16 i2;
            HitVector* vec;
            Vec* pos;
            k = 0; // FAKE: dead store, references k before j's first use so the packed k/counter split temps get the target r18/r17
            for (j = 0; j < mesh->displayListCount; j++) {
                list = mesh->displayLists.normalLists[j];
                for (k = 0; k < list->count - 2; k++) {
                    s16* idx;
                    idx = &list->vtx[k].pos;
                    i1 = *(s16*)((u8*)list + (k + 1) * sizeof(MapVtxRec) + offsetof(MapRawDL, vtx)); // FAKE: the next two strip vertices by byte offset ((k + n) * size + 4), which keeps the target's mulli/strength reduction; list->vtx[k + n].pos schedules differently
                    i2 = *(s16*)((u8*)list + (k + 2) * sizeof(MapVtxRec) + offsetof(MapRawDL, vtx));
                    vec = &hit->vectors[count++];
                    pos = (Vec*)mesh->vcdTable->positions->data;
                    MTXMultVec(hit->worldMtx, &pos[*idx], &vec->v0);
                    MTXMultVec(hit->worldMtx, &pos[!(k & 1) ? i2 : i1], &vec->v1);
                    MTXMultVec(hit->worldMtx, &pos[!(k & 1) ? i1 : i2], &vec->v2);
                    PSVECSubtract(&vec->v2, &vec->v0, &vec->edge0);
                    PSVECSubtract(&vec->v0, &vec->v1, &vec->edge1);
                    PSVECSubtract(&vec->v1, &vec->v2, &vec->edge2);
                    PSVECCrossProduct(&vec->edge0, &vec->edge1, &vec->normal);
                }
            }
        } else {
            u32 bit;
            AddrWord m;
            wp = mapGetWork();
            m = 0; // FAKE: popcount accumulated in m (later reused as the i0 pointer) so stride is a single-def web
            for (bit = 0; bit < 12; bit++) {
                m += ((u32)mesh->elementMask >> bit) & 1;
            }
            stride = m * 2;
            for (k = 0; k < mesh->displayListCount; k++) {
                u8* list = (u8*)(AddrWord)mesh->displayLists.packedLists[k].offset;
                bit = *(u16*)(list + 1) - 2; // FAKE: loop bound in the dead popcount counter (its later web becomes an @ temp); (s32) keeps the signed compare
                for (j = 0; j < (s32)bit; j++) {
                    u16 i0;
                    s16* base;
                    BOOL even;
                    u16 i2;
                    s32 o0;
                    s32 o1;
                    s32 o2;
                    u16 i1;
                    register u32 gqr;
                    HitVector* v2;
                    base = (s16*)mesh->vcdTable->positions->data;
                    v2 = &hit->vectors[count];
                    m = (AddrWord)list; // FAKE: m (the popcount counter) reused as the i1/i2/i0 address temp gives the target add list,X; lhz 3 and leaves o1 a single web (o0/o1/base colour r4/r5/r3)
                    m += stride * (j + 1);
                    i1 = *(u16*)(m + 3);
                    m = (AddrWord)list;
                    m += stride * (j + 2);
                    i2 = *(u16*)(m + 3);
                    even = !(j & 1);
                    m = (AddrWord)list;
                    m += j * stride;
                    i0 = *(u16*)(m + 3);
                    o0 = i0 * 3;
                    o1 = (u16)(even ? i2 : i1) * 3;
                    o2 = (u16)(even ? i1 : i2) * 3;
                    gqr = (wp->entries[hit->mapIndex].vcdTable->posFrac << 8) | 7;
                    gqr |= gqr << 16;
                    HW_SET_GQR5(gqr);
                    a.x = hitDequant(base, o0);
                    a.y = hitDequant(base, o0 + 1);
                    a.z = hitDequant(base, o0 + 2);
                    b.x = hitDequant(base, o1);
                    b.y = hitDequant(base, o1 + 1);
                    b.z = hitDequant(base, o1 + 2);
                    c.x = hitDequant(base, o2);
                    c.y = hitDequant(base, o2 + 1);
                    c.z = hitDequant(base, o2 + 2);
                    MTXMultVec(hit->worldMtx, &a, &v2->v0);
                    MTXMultVec(hit->worldMtx, &b, &v2->v1);
                    MTXMultVec(hit->worldMtx, &c, &v2->v2);
                    PSVECSubtract(&v2->v2, &v2->v0, &v2->edge0);
                    PSVECSubtract(&v2->v0, &v2->v1, &v2->edge1);
                    PSVECSubtract(&v2->v1, &v2->v2, &v2->edge2);
                    PSVECCrossProduct(&v2->edge0, &v2->edge1, &v2->normal);
                    if (v2->normal.x || v2->normal.y || v2->normal.z) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

inline BOOL tempfunc(HitCheckQuery* query, HitVector* vector) {
    Vec toV0;
    Vec toV1;
    Vec toV2;
    f32 dotProduct;
    f32 denom;

    PSVECSubtract(&query->targetPos, &vector->v0, &toV0);
    dotProduct = PSVECDotProduct(&vector->normal, &toV0);
    if (query->singleSided) {
        if (dotProduct < 0.0f) {
            return 0;
        }
        if (((toV0.z * vector->edge0.x) - (toV0.x * vector->edge0.z)) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &toV1);
        if (((toV1.z * vector->edge1.x) - (toV1.x * vector->edge1.z)) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &toV2);
        if (((toV2.z * vector->edge2.x) - (toV2.x * vector->edge2.z)) < 0.0f) {
            return 0;
        }
    } else {
        if ((vector->normal.y * dotProduct) <= 0.0f) {
            return 0;
        }
        if ((dotProduct * ((toV0.z * vector->edge0.x) - (toV0.x * vector->edge0.z))) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &toV1);
        if ((dotProduct * ((toV1.z * vector->edge1.x) - (toV1.x * vector->edge1.z))) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &toV2);
        if ((dotProduct * ((toV2.z * vector->edge2.x) - (toV2.x * vector->edge2.z))) < 0.0f) {
            return 0;
        }
    }
    denom = -vector->normal.y;
    if ((query->targetDistance >= 0.0f) && ((-dotProduct / denom) >= query->targetDistance)) {
        return 0;
    }
    query->targetDistance = -dotProduct / denom;
    query->hitPos.x = query->targetPos.x;
    query->hitPos.y = query->targetPos.y - query->targetDistance;
    query->hitPos.z = query->targetPos.z;
    if (dotProduct >= 0.0f) {
        query->hitNormal = vector->normal;
    } else {
        PSVECScale(&vector->normal, &query->hitNormal, -1.0f);
    }
    return 1;
}

inline BOOL tempfunc2(HitCheckQuery* query, HitVector* vector) {
    Vec toV0;
    Vec toV1;
    Vec toV2;
    f32 dotProduct;
    f32 denom;

    PSVECSubtract(&query->targetPos, &vector->v0, &toV0);
    dotProduct = PSVECDotProduct(&vector->normal, &toV0);
    if (query->singleSided) {
        if (dotProduct < 0.0f) {
            return 0;
        }
        if (((toV0.z * vector->edge0.x) - (toV0.x * vector->edge0.z)) > 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &toV1);
        if (((toV1.z * vector->edge1.x) - (toV1.x * vector->edge1.z)) > 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &toV2);
        if (((toV2.z * vector->edge2.x) - (toV2.x * vector->edge2.z)) > 0.0f) {
            return 0;
        }
    } else {
        if ((vector->normal.y * dotProduct) >= 0.0f) {
            return 0;
        }
        if ((dotProduct * ((toV0.z * vector->edge0.x) - (toV0.x * vector->edge0.z))) > 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &toV1);
        if ((dotProduct * ((toV1.z * vector->edge1.x) - (toV1.x * vector->edge1.z))) > 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &toV2);
        if ((dotProduct * ((toV2.z * vector->edge2.x) - (toV2.x * vector->edge2.z))) > 0.0f) {
            return 0;
        }
    }
    denom = vector->normal.y;
    if ((query->targetDistance >= 0.0f) && ((-dotProduct / denom) >= query->targetDistance)) {
        return 0;
    }
    query->targetDistance = -dotProduct / denom;
    query->hitPos.x = query->targetPos.x;
    query->hitPos.y = query->targetPos.y + query->targetDistance;
    query->hitPos.z = query->targetPos.z;
    if (dotProduct >= 0.0f) {
        query->hitNormal = vector->normal;
    } else {
        PSVECScale(&vector->normal, &query->hitNormal, -1.0f);
    }
    return 1;
}

inline BOOL tempfunc3(HitCheckQuery* query, HitVector* vector) {
    Vec toV0;
    Vec toV1;
    Vec toV2;
    Vec cross0;
    Vec cross1;
    Vec cross2;
    Vec scaledDir;
    f32 dotProduct;
    f32 dirDotNormal;

    PSVECSubtract(&query->targetPos, &vector->v0, &toV0);
    dotProduct = PSVECDotProduct(&vector->normal, &toV0);
    if (query->singleSided) {
        if (dotProduct < 0.0f) {
            return 0;
        }
        PSVECCrossProduct(&query->targetDir, &vector->edge0, &cross0);
        if (PSVECDotProduct(&toV0, &cross0) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &toV1);
        PSVECCrossProduct(&query->targetDir, &vector->edge1, &cross1);
        if (PSVECDotProduct(&toV1, &cross1) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &toV2);
        PSVECCrossProduct(&query->targetDir, &vector->edge2, &cross2);
        if (PSVECDotProduct(&toV2, &cross2) < 0.0f) {
            return 0;
        }
        dirDotNormal = PSVECDotProduct(&vector->normal, &query->targetDir);
    } else {
        dirDotNormal = PSVECDotProduct(&vector->normal, &query->targetDir);
        if ((dirDotNormal * dotProduct) >= 0.0f) {
            return 0;
        }
        PSVECCrossProduct(&query->targetDir, &vector->edge0, &cross0);
        if ((dotProduct * PSVECDotProduct(&toV0, &cross0)) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &toV1);
        PSVECCrossProduct(&query->targetDir, &vector->edge1, &cross1);
        if ((dotProduct * PSVECDotProduct(&toV1, &cross1)) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &toV2);
        PSVECCrossProduct(&query->targetDir, &vector->edge2, &cross2);
        if ((dotProduct * PSVECDotProduct(&toV2, &cross2)) < 0.0f) {
            return 0;
        }
    }

    if ((query->targetDistance >= 0.0f) && ((-dotProduct / dirDotNormal) >= query->targetDistance)) {
        return 0;
    }
    query->targetDistance = -dotProduct / dirDotNormal;
    PSVECScale(&query->targetDir, &scaledDir, query->targetDistance);
    PSVECAdd(&query->targetPos, &scaledDir, &query->hitPos);
    if (dotProduct >= 0.0f) {
        query->hitNormal = vector->normal;
    } else {
        PSVECScale(&vector->normal, &query->hitNormal, -1.0f);
    }
    return 1;
}

HitEntry* hitCheckVecFilter(HitCheckQuery* query, HitFilterCallback callback) {
    f32 temp;
    HitVector* vector;
    Vec position;
    Vec hitNormalTmp;
    Vec normal;
    Vec endPos;
    HitEntry* entry;
    HitEntry* closest;
    int i;
    f32 distance;
    f32 minDist;
    HitVector* vec;
    f32 reach;

    mapGetWork();
    closest = NULL;
    if (!PSVECSquareMag(&query->targetDir)) {
        return NULL;
    }
    minDist = -1.0f;
    temp = 0.5f;
    reach = query->targetDistance * temp;
    PSVECScale(&query->targetDir, &endPos, reach);
    PSVECAdd(&query->targetPos, &endPos, &endPos);
    entry = hitActiveListTop;
    if (query->targetDir.y == 0.0f) {
        while (entry != NULL) {
            if (callback == NULL || callback(query, entry)) {
                if (!(PSVECDistance(&endPos, &entry->centerWorld) > reach + entry->radius)) {
                    if (entry->joint->drawMode->cullMode == 1) {
                        query->singleSided = 1;
                    } else {
                        query->singleSided = 0;
                    }
                    vec = entry->vectors;
                    for (i = 0; i < entry->vectorCount; i++, vec++) {
                        if ((query->singleSided == 0) ||
                            !(PSVECDotProduct(&vec->normal, &query->targetDir) >= 0.0f)) {
                            if (checkTriVec_xz(query, vec) && (minDist < 0.0f || minDist > query->targetDistance)) {
                                minDist = query->targetDistance;
                                closest = entry;
                                distance = minDist;
                                position = query->hitPos;
                                hitNormalTmp = query->hitNormal;
                            }
                        }
                    }
                }
            }
            entry = entry->siblingActive;
        }
    } else if (query->targetDir.x == 0.0f && query->targetDir.z == 0.0f) {
        if (query->targetDir.y == -1.0f) {
            while (entry != NULL) {
                if (callback == NULL || callback(query, entry)) {
                    if (!(PSVECDistance(&endPos, &entry->centerWorld) > reach + entry->radius)) {
                        if (entry->joint->drawMode->cullMode == 1) {
                            query->singleSided = 1;
                        } else {
                            query->singleSided = 0;
                        }
                        vector = entry->vectors;
                        for (i = 0; i < entry->vectorCount; i++, vector++) {
                            if (query->singleSided == 0 || !(vector->normal.y <= 0.0f)) {
                                if (tempfunc(query, vector) && (minDist < 0.0f || minDist > query->targetDistance)) {
                                    minDist = query->targetDistance;
                                    closest = entry;
                                    distance = minDist;
                                    position = query->hitPos;
                                    hitNormalTmp = query->hitNormal;
                                }
                            }
                        }
                    }
                }
                entry = entry->siblingActive;
            }
        } else if (query->targetDir.y == 1.0f) {
            while (entry != NULL) {
                if (callback == NULL || callback(query, entry)) {
                    if (!(PSVECDistance(&endPos, &entry->centerWorld) > reach + entry->radius)) {
                        if (entry->joint->drawMode->cullMode == 1) {
                            query->singleSided = 1;
                        } else {
                            query->singleSided = 0;
                        }
                        vector = entry->vectors;
                        for (i = 0; i < entry->vectorCount; i++, vector++) {
                            if (query->singleSided == 0 || !(vector->normal.y >= 0.0f)) {
                                if (tempfunc2(query, vector) && (minDist < 0.0f || minDist > query->targetDistance)) {
                                    minDist = query->targetDistance;
                                    closest = entry;
                                    distance = minDist;
                                    position = query->hitPos;
                                    hitNormalTmp = query->hitNormal;
                                }
                            }
                        }
                    }
                }
                entry = entry->siblingActive;
            }
        } else {
            goto label_235;
        }
    } else {
    label_235:
        while (entry != NULL) {
            if (callback == NULL || callback(query, entry)) {
                if (!(PSVECDistance(&endPos, &entry->centerWorld) > (reach + entry->radius))) {
                    if (entry->joint->drawMode->cullMode == 1) {
                        query->singleSided = 1;
                    } else {
                        query->singleSided = 0;
                    }
                    vector = entry->vectors;
                    for (i = 0; i < entry->vectorCount; i++, vector++) {
                        if (query->singleSided == 0 || !(PSVECDotProduct(&vector->normal, &query->targetDir) >= 0.0f)) {
                            if (tempfunc3(query, vector) && (minDist < 0.0f || minDist > query->targetDistance)) {
                                minDist = query->targetDistance;
                                closest = entry;
                                distance = minDist;
                                position = query->hitPos;
                                hitNormalTmp = query->hitNormal;
                            }
                        }
                    }
                }
            }
            entry = entry->siblingActive;
        }
    }
    if (closest == NULL) {
        return NULL;
    }
    PSVECNormalize(&hitNormalTmp, &normal);
    query->targetDistance = distance;
    query->hitPos = position;
    query->hitNormal = normal;
    return closest;
}

HitEntry* hitCheckFilter(HitFilterCallback callback, f32* hitX, f32* hitY, f32* hitZ, f32* distance, f32* hitNX,
                         f32* hitNY, f32* hitNZ, f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz) {
    HitCheckQuery temp;
    HitCheckQuery* query = &temp;
    HitEntry* entry;

    query->targetPos = (Vec){ x, y, z };
    query->targetDir = (Vec){ nx, ny, nz };
    query->targetDistance = *distance;
    entry = hitCheckVecFilter(query, callback);
    if (entry == NULL) {
        return NULL;
    }
    *distance = query->targetDistance;
    *hitX = query->hitPos.x;
    *hitY = query->hitPos.y;
    *hitZ = query->hitPos.z;
    *hitNX = query->hitNormal.x;
    *hitNY = query->hitNormal.y;
    *hitNZ = query->hitNormal.z;
    return entry;
}

BOOL checkTriVec_xz(HitCheckQuery* query, HitVector* vector) {
    Vec v1, v2, v3;
    f32 cosAngle;
    f32 distToTrianglePlane;

    PSVECSubtract(&query->targetPos, &vector->v0, &v1);
    distToTrianglePlane = PSVECDotProduct(&vector->normal, &v1);
    if (query->singleSided != 0) {
        if (distToTrianglePlane < 0.0f) {
            return FALSE;
        }
        if ((v1.x * (-vector->edge0.y * query->targetDir.z)) +
                (v1.y * ((vector->edge0.x * query->targetDir.z) - (vector->edge0.z * query->targetDir.x))) +
                (v1.z * (vector->edge0.y * query->targetDir.x)) <
            0.0f) {
            return FALSE;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &v2);
        if ((v2.x * (-vector->edge1.y * query->targetDir.z)) +
                (v2.y * ((vector->edge1.x * query->targetDir.z) - (vector->edge1.z * query->targetDir.x))) +
                (v2.z * (vector->edge1.y * query->targetDir.x)) <
            0.0f) {
            return FALSE;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &v3);
        if ((v3.x * (-vector->edge2.y * query->targetDir.z)) +
                (v3.y * ((vector->edge2.x * query->targetDir.z) - (vector->edge2.z * query->targetDir.x))) +
                (v3.z * (vector->edge2.y * query->targetDir.x)) <
            0.0f) {
            return FALSE;
        }
    } else {
        if (((vector->normal.x * query->targetDir.x) + (vector->normal.z * query->targetDir.z)) * distToTrianglePlane >=
            0.0f) {
            return FALSE;
        }
        if (((v1.x * (-vector->edge0.y * query->targetDir.z)) +
             (v1.y * ((vector->edge0.x * query->targetDir.z) - (vector->edge0.z * query->targetDir.x))) +
             (v1.z * (vector->edge0.y * query->targetDir.x))) *
                distToTrianglePlane <
            0.0f) {
            return FALSE;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &v2);
        if (((v2.x * (-vector->edge1.y * query->targetDir.z)) +
             (v2.y * ((vector->edge1.x * query->targetDir.z) - (vector->edge1.z * query->targetDir.x))) +
             (v2.z * (vector->edge1.y * query->targetDir.x))) *
                distToTrianglePlane <
            0.0f) {
            return FALSE;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &v3);
        if (((v3.x * (-vector->edge2.y * query->targetDir.z)) +
             (v3.y * ((vector->edge2.x * query->targetDir.z) - (vector->edge2.z * query->targetDir.x))) +
             (v3.z * (vector->edge2.y * query->targetDir.x))) *
                distToTrianglePlane <
            0.0f) {
            return FALSE;
        }
    }
    cosAngle = (vector->normal.x * query->targetDir.x) + (vector->normal.z * query->targetDir.z);
    if (query->targetDistance >= 0.0f && -distToTrianglePlane / cosAngle >= query->targetDistance) {
        return FALSE;
    }
    query->targetDistance = -distToTrianglePlane / cosAngle;
    query->hitPos.x = query->targetPos.x + (query->targetDir.x * query->targetDistance);
    query->hitPos.y = query->targetPos.y;
    query->hitPos.z = query->targetPos.z + (query->targetDir.z * query->targetDistance);
    if (distToTrianglePlane >= 0.0f) {
        query->hitNormal = vector->normal;
    } else {
        PSVECScale(&vector->normal, &query->hitNormal, -1.0f);
    }
    return TRUE;
}

BOOL hitCheckVecHitObjXZ(HitCheckQuery* query, HitEntry* entry) {
    int i;
    HitVector* vector;

    if (entry->joint->drawMode->cullMode == 1) {
        query->singleSided = 1;
    } else {
        query->singleSided = 0;
    }
    vector = entry->vectors;
    for (i = 0; i < entry->vectorCount; i++, vector++) {
        if ((query->singleSided == 0 || !(PSVECDotProduct(&vector->normal, &query->targetDir) >= 0.0f)) &&
            checkTriVec_xz(query, vector)) {
            return 1;
        }
    }
    return 0;
}

BOOL chkFilterAttr(HitCheckQuery* query, HitEntry* entry) {
    return (entry->attributes & query->user0) == 0;
}

HitEntry* hitCheckAttr(s32 user0, f32* hitX, f32* hitY, f32* hitZ, f32* distance, f32* hitNX, f32* hitNY, f32* hitNZ,
                       f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz) {
    HitCheckQuery temp;
    HitCheckQuery* query = &temp;
    HitEntry* entry;

    query->targetPos = (Vec){ x, y, z };
    query->targetDir = (Vec){ nx, ny, nz };
    query->targetDistance = *distance;
    query->user0 = user0;
    entry = hitCheckVecFilter(query, chkFilterAttr);
    if (entry == NULL) {
        return NULL;
    }
    *distance = query->targetDistance;
    *hitX = query->hitPos.x;
    *hitY = query->hitPos.y;
    *hitZ = query->hitPos.z;
    *hitNX = query->hitNormal.x;
    *hitNY = query->hitNormal.y;
    *hitNZ = query->hitNormal.z;
    return entry;
}

HitEntry* hitCheckSphereFilter(HitFilterCallback callback, f32 x, f32 y, f32 z,
                               f32 distance) {
    MapEntry* map = mapGetWork()->entries;
    MapEntry* mapEntry = map;
    int i;
    Vec center;
    int j;
    HitEntry* hitObj;
    HitVector* vector;
    f32 minDist;
    s32 objectId;
    s32 fl;
    f32 dist;
    f32 minVecDist;
    f32 radius;
    s32 entryId;
    s32 entryFlags;
    HitCheckQuery temp;
    HitCheckQuery* query;
    HitEntry* bestEntry;

    minDist = -1.0f;
    objectId = -1;
    query = &temp;
    center = (Vec){ x, y, z };
    for (i = 0; i < map->count; mapEntry++, i++) {
        hitObj = mapEntry->hitEntries;
        for (j = 0; j < mapEntry->hitNumJoints; j++, hitObj++) {
            if ((hitObj->attributes & HIT_ATTR_MOBJ) && (hitObj->joint->partCount != 0)) {
                entryFlags = hitObj->flags;
                if (!(entryFlags & 1) && !(entryFlags & 0x80)) {
                    dist = PSVECDistance(&center, &hitObj->centerWorld);
                    if (!(dist > (distance + hitObj->radius))) {
                        if (callback == NULL || callback(query, hitObj)) {
                            if (minDist < 0.0f || minDist > dist) {
                                minDist = dist;
                                radius = hitObj->radius;
                                objectId = j;
                                entryId = i;
                            }
                        }
                    }
                }
            }
        }
    }
    if (objectId < 0) {
        return 0;
    }
    if (minDist < (0.5f * radius)) {
        return &map[entryId].hitEntries[objectId];
    }
    map = &map[entryId];
    bestEntry = &map->hitEntries[objectId];
    PSVECSubtract(&bestEntry->centerWorld, &center, &query->targetDir);
    PSVECNormalize(&query->targetDir, &query->targetDir);
    minVecDist = -1.0f;
    fl = -1;
    query->targetPos = center;
    query->targetDistance = distance;
    if (bestEntry->joint->drawMode->cullMode == 1) {
        query->singleSided = 1;
    } else {
        query->singleSided = 0;
    }
    vector = bestEntry->vectors;
    for (i = 0; i < bestEntry->vectorCount; i++, vector++) {
        if ((query->singleSided == 0) || !(PSVECDotProduct(&vector->normal, &query->targetDir) >= 0.0f)) {
            if (tempfunc3(query, vector) && ((minVecDist < 0.0f) || (minVecDist > query->targetDistance))) {
                minVecDist = query->targetDistance;
                fl = 1;
            }
        }
    }
    if (minVecDist > distance) {
        return 0;
    }
    if (fl < 0) {
        return 0;
    }
    return &map->hitEntries[objectId];
}

HitEntry* hitNameToPtr(const char* name) {
    MapEntry* map;
    MapWork* wp = mapGetWork();
    int j;
    int i;
    HitEntry* hit;

    if (name == NULL) {
        return NULL;
    }
    for (i = 0; i < wp->entries->count; i++) {
        map = &wp->entries[i];
        hit = map->hitEntries;
        for (j = 0; j < map->hitNumJoints; j++, hit++) {
            if (!(hit->flags & HIT_FLAG_DELETED) && !strcmp(hit->joint->name, name)) {
                return hit;
            }
        }
    }

    return NULL;
}

void hitObjGetPosSub(HitEntry* hit, Vec* position, s32* counter, BOOL recursion) {
    Vec center;

    if (hit->child != NULL) {
        hitObjGetPosSub(hit->child, position, counter, 1);
    } else {
        PSMTXMultVec(hit->worldMtx, &hit->centerLocal, &center);
        PSVECAdd(position, &center, position);
        (*counter)++;
    }
    if (recursion) {
        if (hit->sibling != NULL) {
            hitObjGetPosSub(hit->sibling, position, counter, 1);
        }
    }
}

void hitObjGetPos(const char* name, Vec* position) {
    Vec sum;
    s32 counter;
    HitEntry* hit;

    counter = 0;
    sum = (Vec){ 0.0f, 0.0f, 0.0f };
    hit = hitNameToPtr(name);
    if (hit == NULL) {
        *position = (Vec){ 0.0f, 0.0f, 0.0f };
        return;
    }
    hitObjGetPosSub(hit, &sum, &counter, 0);
    PSVECScale(&sum, position, 1.0f / (f32)counter);
}

void hitObjGetNormal(const char* name, Vec* normal) {
    HitEntry* hit;

    hit = hitNameToPtr(name);
    if (hit != NULL && hit->joint->partCount > 0) {
        PSVECNormalize(&hit->vectors->normal, normal);
    }
}

const char* hitGetName(HitEntry* hit) {
    MapEntry* map;
    s32 count;
    int i;
    u8* start;
    u32 size;

    if (hit == NULL) {
        return NULL;
    }
    map = mapGetWork()->entries;
    count = map->count;
    for (i = 0; i < count; map++, i++) {
        start = (u8*)map->hitEntries;
        size = OSRoundUp32B((map->hitNumJoints + 0x80) * sizeof(HitEntry));
        if ((u8*)hit >= start && (u8*)hit < start + size) {
            break;
        }
    }
    if (i >= count) {
        return NULL;
    }
    return hit->joint->name;
}

s32 hitGetAttr(HitEntry* hit) {
    return hit != NULL ? hit->attributes : 0;
}

void hitDamageReturnSet(HitEntry* hit, HitDamageReturn* damage, BOOL recursion) {
    hit->attributes |= HIT_ATTR_DAMAGE_RETURN;
    hit->damage = damage;
    if (hit->child != NULL) {
        hitDamageReturnSet(hit->child, damage, TRUE);
    }
    if (recursion != 0) {
        if (hit->sibling != NULL) {
            hitDamageReturnSet(hit->sibling, damage, TRUE);
        }
    }
}

void hitGrpDamageReturnSet(const char* name, HitDamageReturn* damage) {
    HitEntry* hit;

    hit = hitNameToPtr(name);
    if (hit != NULL) {
        hitDamageReturnSet(hit, damage, FALSE);
    }
}

Vec* hitGetDamageReturnPos(HitEntry* hit) {
    return &hit->damage->position;
}

void hitBindMapObj(const char* hitName, const char* mapobjName) {
    HitEntry* hit;
    MapObject* mapObj;

    hit = hitNameToPtr(hitName);
    mapObj = mapGetMapObj(mapobjName);
    if (mapObj == NULL) {
        mapErrorEntry(0, mapobjName);
        return;
    }
    if (hit == NULL) {
        mapErrorEntry(1, hitName);
        return;
    }
    hit->mapObj = mapObj;
    hit->flags |= HIT_FLAG_MAPOBJ_BIND;
}

void hitBindUpdate(const char* name) {
    HitEntry* parent;
    HitEntry* hit;
    Mtx mtx;

    hit = hitNameToPtr(name);
    if (hit == NULL) {
        mapErrorEntry(1, name);
        return;
    }
    if (hit->flags & HIT_FLAG_MAPOBJ_BIND) {
        parent = hit->parent;
        if (parent != NULL) {
            hitReCalcMatrix(hit, parent->worldMtx);
            return;
        }
        PSMTXScale(mtx, 10.0f, 10.0f, 10.0f);
        hitReCalcMatrix(hit, mtx);
    }
}

// FAKE: stand-in for code the linker stripped from the original object; its 0.0f and 0.1f are the first
// literals in the target .sdata2 pool (codegen runs bottom-up, so it must sit last in the file). The JP demo map
// lists three stripped hitdrv functions, hitNumberToPtr (0x4C bytes), hitGetEntNum (0x38) and bounding_box
// (0x528); no build contains their code, so the original cannot be restored.
void hitStrippedPool(f32* f) {
    f[0] = 0.0f;
    f[1] = 0.1f;
}

