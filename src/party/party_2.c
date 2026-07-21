#include "party/party_2.h"

/* Moved from src/party/party.c: correct objdiff unit main/party/party_2. */
/* CHATGPT STUB FILL: main/party/party 20260624_183901 */

u8 partyGetAppearPos(void* party, f32* outPos);
u8 partyGetAppearPos2(void* party, f32* outPos);
u8 partyGetAppearPos3(void* party, f32* outPos);
u8 partyGetAppearPos4(void* party, f32* outPos);
u8 partyGetAppearPos5(void* party, void* hitObj, f32* outPos);

/* stub-fill: partyGetAppearPos | missing_definition | ghidra_signature */
u8 partyGetAppearPos(void* party, f32* outPos) {
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
        f32 dist;
    } HitWork;
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern s32 marioPlaneGetFlyingDir(void);
    extern f64 revise360(f64 angle);
    extern f64 toMovedir(f64 angle);
    extern f64 toMovedir2(f64 angle, f64 offset);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern void movePos(f64 distance, f64 angle, f32* x, f32* z);
    extern s32 hitCheckVecFilter(void* work, void* filter);
    extern u32 hitGetAttr(void* hit);
    extern MobjName* mobjHitObjPtrToPtr(void* hit);
    extern s32 strcmp(char* a, char* b);
    extern char* dat_ptrarr_80314fe8[];
    extern void* npcNearDistCheck(f32 x, f32 y, f32 z, f32 radius);
    extern f32 float_0_804215b8;
    extern f32 float_30_804215c0;
    extern f32 float_37_804215b0;
    extern f32 float_100_804215dc;
    extern f32 float_150_80421594;
    extern f32 float_180_804215ac;
    void* player;
    HitWork work;
    f32 baseDirection;
    f32 oppositeDirection;
    f32 angle;
    f32 sine;
    f32 cosine;
    f32 width;
    u32 attr;
    u32 nameIndex;
    MobjName* mobj;
    void* hit;

#define TRY_APPEAR_OFFSET(offsetValue, moveDistance) \
    do { \
        angle = (f32)toMovedir2(baseDirection, (offsetValue)); \
        sincosf(angle, &sine, &cosine); \
        work.start[0] = *(f32*)((s32)player + 0x8C); \
        work.start[1] = *(f32*)((s32)player + 0x90) + float_37_804215b0; \
        work.start[2] = *(f32*)((s32)player + 0x94); \
        work.dir[0] = sine; \
        work.dir[1] = 0.0f; \
        work.dir[2] = cosine; \
        work.dist = width; \
        if (hitCheckVecFilter(&work, 0) != 0) break; \
        outPos[0] = *(f32*)((s32)player + 0x8C); \
        outPos[1] = *(f32*)((s32)player + 0x90); \
        outPos[2] = *(f32*)((s32)player + 0x94); \
        movePos((moveDistance), angle, outPos, outPos + 2); \
        work.start[0] = outPos[0]; \
        work.start[1] = outPos[1] + float_37_804215b0; \
        work.start[2] = outPos[2]; \
        work.dir[0] = 0.0f; \
        work.dir[1] = -1.0f; \
        work.dir[2] = 0.0f; \
        work.dist = float_150_80421594; \
        hit = (void*)hitCheckVecFilter(&work, 0); \
        if (hit == 0) break; \
        attr = hitGetAttr(hit); \
        if ((attr & 0xA00) != 0) break; \
        if ((attr & 0x80000000) != 0) { \
            mobj = mobjHitObjPtrToPtr(hit); \
            for (nameIndex = 0; dat_ptrarr_80314fe8[nameIndex] != 0; nameIndex++) { \
                if (strcmp(mobj->name, dat_ptrarr_80314fe8[nameIndex]) == 0) break; \
            } \
            if (dat_ptrarr_80314fe8[nameIndex] != 0) break; \
        } \
        if (work.dist <= float_100_804215dc && \
            npcNearDistCheck(outPos[0], work.hitPos[1], outPos[2], width) == 0) { \
            outPos[1] = work.hitPos[1]; \
            return 1; \
        } \
    } while (0)

    player = *(void**)((s32)party + 0x160);
    width = float_30_804215c0 + *(f32*)((s32)party + 0xF4);
    if (*(u16*)((s32)player + 0x2E) == 0x18) {
        if (marioPlaneGetFlyingDir() == 1) {
            oppositeDirection = float_0_804215b8;
            baseDirection = float_180_804215ac;
        } else {
            oppositeDirection = float_180_804215ac;
            baseDirection = float_0_804215b8;
        }
    } else {
        oppositeDirection = (f32)revise360(float_180_804215ac + *(f32*)((s32)player + 0x1AC));
        baseDirection = *(f32*)((s32)player + 0x1AC);
    }
    outPos[0] = *(f32*)((s32)player + 0x8C);
    outPos[1] = *(f32*)((s32)player + 0x90);
    outPos[2] = *(f32*)((s32)player + 0x94);

    TRY_APPEAR_OFFSET(0.0f, -float_30_804215c0);
    TRY_APPEAR_OFFSET(-90.0f, -float_30_804215c0);
    TRY_APPEAR_OFFSET(90.0f, -float_30_804215c0);
    TRY_APPEAR_OFFSET(180.0f, -float_30_804215c0);
    TRY_APPEAR_OFFSET(-45.0f, float_30_804215c0);
    TRY_APPEAR_OFFSET(45.0f, float_30_804215c0);
    TRY_APPEAR_OFFSET(-135.0f, float_30_804215c0);
    TRY_APPEAR_OFFSET(135.0f, float_30_804215c0);
#undef TRY_APPEAR_OFFSET
    return 0;
}

/* stub-fill: partyGetAppearPos2 | prototype_only | source_prototype */
u8 partyGetAppearPos2(void* party, f32* outPos) {
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
        f32 dist;
    } HitWork;
    extern f64 revise360(f64 angle);
    extern f64 toMovedir(f64 angle);
    extern f64 toMovedir2(f64 angle, f64 offset);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern void movePos(f64 distance, f64 angle, f32* x, f32* z);
    extern s32 hitCheckVecFilter(void* work, void* filter);
    extern f32 float_30_804215c0;
    extern f32 float_180_804215ac;
    extern f32 float_37_804215b0;
    extern f32 float_38_804215d0;
    extern f32 float_neg90_804215d4;
    extern f32 float_90_80421598;
    extern f32 float_neg20_804215d8;
    extern f32 float_20_8042159c;
    void* player;
    HitWork work;
    f32 sine;
    f32 cosine;
    f32 width;
    f32 direction;
    f32 angle;
    f32 side;

    player = *(void**)((s32)party + 0x160);
    width = float_30_804215c0 + *(f32*)((s32)party + 0xF4);
    revise360(float_180_804215ac + *(f32*)((s32)player + 0x1AC));
    direction = *(f32*)((s32)player + 0x1AC);
    outPos[0] = *(f32*)((s32)player + 0x8C);
    outPos[1] = *(f32*)((s32)player + 0x90);
    outPos[2] = *(f32*)((s32)player + 0x94);

    if (*(s8*)((s32)player + 0x246) < 0) {
        angle = (f32)toMovedir(float_180_804215ac + direction);
        sincosf(angle, &sine, &cosine);
        work.start[0] = outPos[0];
        work.start[1] = outPos[1] + float_37_804215b0;
        work.start[2] = outPos[2];
        work.dir[0] = sine;
        work.dir[1] = 0.0f;
        work.dir[2] = cosine;
        work.dist = width;
        if (hitCheckVecFilter(&work, 0) == 0) {
            movePos(-float_30_804215c0, toMovedir(direction), outPos, outPos + 2);
            work.start[0] = outPos[0];
            work.start[1] = outPos[1] + float_37_804215b0;
            work.start[2] = outPos[2];
            work.dir[0] = 0.0f;
            work.dir[1] = -1.0f;
            work.dir[2] = 0.0f;
            work.dist = float_38_804215d0;
            if (hitCheckVecFilter(&work, 0) != 0) {
                outPos[1] = work.hitPos[1];
                return 1;
            }
        }
    } else {
        angle = (f32)toMovedir2(direction, float_neg90_804215d4);
        sincosf(angle, &sine, &cosine);
        work.start[0] = outPos[0];
        work.start[1] = outPos[1] + float_37_804215b0;
        work.start[2] = outPos[2];
        work.dir[0] = sine;
        work.dir[1] = 0.0f;
        work.dir[2] = cosine;
        work.dist = width;
        if (hitCheckVecFilter(&work, 0) == 0) {
            side = *(s8*)((s32)party + 0x2F) == 0 ? float_20_8042159c : float_neg20_804215d8;
            movePos(-float_30_804215c0, toMovedir2(direction, side), outPos, outPos + 2);
            work.start[0] = outPos[0];
            work.start[1] = outPos[1] + float_37_804215b0;
            work.start[2] = outPos[2];
            work.dir[0] = 0.0f;
            work.dir[1] = -1.0f;
            work.dir[2] = 0.0f;
            work.dist = float_38_804215d0;
            if (hitCheckVecFilter(&work, 0) != 0) {
                outPos[1] = work.hitPos[1];
                return 1;
            }
        }

        angle = (f32)toMovedir2(direction, float_90_80421598);
        sincosf(angle, &sine, &cosine);
        work.start[0] = *(f32*)((s32)player + 0x8C);
        work.start[1] = *(f32*)((s32)player + 0x90) + float_37_804215b0;
        work.start[2] = *(f32*)((s32)player + 0x94);
        work.dir[0] = sine;
        work.dir[1] = 0.0f;
        work.dir[2] = cosine;
        work.dist = width;
        if (hitCheckVecFilter(&work, 0) == 0) {
            outPos[0] = *(f32*)((s32)player + 0x8C);
            outPos[1] = *(f32*)((s32)player + 0x90);
            outPos[2] = *(f32*)((s32)player + 0x94);
            side = *(s8*)((s32)party + 0x2F) == 0 ? float_neg20_804215d8 : float_20_8042159c;
            movePos(-float_30_804215c0, toMovedir2(direction, side), outPos, outPos + 2);
            work.start[0] = outPos[0];
            work.start[1] = outPos[1] + float_37_804215b0;
            work.start[2] = outPos[2];
            work.dir[0] = 0.0f;
            work.dir[1] = -1.0f;
            work.dir[2] = 0.0f;
            work.dist = float_38_804215d0;
            if (hitCheckVecFilter(&work, 0) != 0) {
                outPos[1] = work.hitPos[1];
                return 1;
            }
        }
    }
    return 0;
}

/* stub-fill: partyGetAppearPos3 | missing_definition | ghidra_signature */
u8 partyGetAppearPos3(void* party, f32* outPos) {
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
        f32 dist;
    } HitWork;
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern f64 toMovedir2(f64 angle, f64 offset);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern void movePos(f64 distance, f64 angle, f32* x, f32* z);
    extern s32 hitCheckVecFilter(void* work, void* filter);
    extern u32 hitGetAttr(void* hit);
    extern MobjName* mobjHitObjPtrToPtr(void* hit);
    extern s32 strcmp(char* a, char* b);
    extern char* dat_ptrarr_80314fe8[];
    extern void* npcNearDistCheck(f32 x, f32 y, f32 z, f32 radius);
    extern f32 float_30_804215c0;
    extern f32 float_37_804215b0;
    extern f32 float_87_804215b4;
    extern f32 float_2_804215cc;
    static const s32 angles[9] = { 45, 135, -135, 315, -315, 180, -180, 90, -90 };
    void* player;
    HitWork work;
    MobjName* mobj;
    f32 sine;
    f32 cosine;
    f32 width;
    f32 angle;
    u32 i;
    u32 j;
    u32 attr;
    void* hit;

    player = *(void**)((s32)party + 0x160);
    width = float_30_804215c0 + *(f32*)((s32)party + 0xF4);
    outPos[0] = *(f32*)((s32)player + 0x8C);
    outPos[1] = *(f32*)((s32)player + 0x90);
    outPos[2] = *(f32*)((s32)player + 0x94);
    for (i = 0; i < 9; i++) {
        angle = (f32)toMovedir2(*(f32*)((s32)player + 0x1B0), (f32)angles[i]);
        sincosf(angle, &sine, &cosine);
        work.start[0] = *(f32*)((s32)player + 0x8C);
        work.start[1] = *(f32*)((s32)player + 0x90) + float_37_804215b0;
        work.start[2] = *(f32*)((s32)player + 0x94);
        work.dir[0] = sine;
        work.dir[1] = 0.0f;
        work.dir[2] = cosine;
        work.dist = width;
        if (hitCheckVecFilter(&work, 0) != 0) {
            continue;
        }
        outPos[0] = *(f32*)((s32)player + 0x8C);
        outPos[1] = *(f32*)((s32)player + 0x90);
        outPos[2] = *(f32*)((s32)player + 0x94);
        movePos(float_30_804215c0, angle, outPos, outPos + 2);
        work.start[0] = outPos[0];
        work.start[1] = outPos[1] + float_37_804215b0;
        work.start[2] = outPos[2];
        work.dir[0] = 0.0f;
        work.dir[1] = -1.0f;
        work.dir[2] = 0.0f;
        work.dist = float_87_804215b4;
        hit = (void*)hitCheckVecFilter(&work, 0);
        if (hit == 0) {
            continue;
        }
        attr = hitGetAttr(hit);
        if ((attr & 0x80000000) != 0) {
            mobj = mobjHitObjPtrToPtr(hit);
            for (j = 0; dat_ptrarr_80314fe8[j] != 0; j++) {
                if (strcmp(mobj->name, dat_ptrarr_80314fe8[j]) == 0) {
                    break;
                }
            }
            if (dat_ptrarr_80314fe8[j] != 0) {
                continue;
            }
        }
        if ((attr & 0xA00) != 0) {
            continue;
        }
        outPos[1] = work.hitPos[1];
        if (npcNearDistCheck(outPos[0], outPos[1], outPos[2], float_2_804215cc) == 0) {
            return 1;
        }
    }
    return 0;
}

/* stub-fill: partyGetAppearPos4 | missing_definition | ghidra_signature */
u8 partyGetAppearPos4(void* party, f32* outPos) {
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
        f32 dist;
    } HitWork;
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern void* anotherPartyGetPtr(s32 slot);
    extern f64 revise360(f64 angle);
    extern f64 toMovedir2(f64 angle, f64 offset);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern void movePos(f64 distance, f64 angle, f32* x, f32* z);
    extern s32 hitCheckVecFilter(void* work, void* filter);
    extern u32 hitGetAttr(void* hit);
    extern MobjName* mobjHitObjPtrToPtr(void* hit);
    extern s32 strcmp(char* a, char* b);
    extern char* dat_ptrarr_80314fe8[];
    extern void* npcNearDistCheck(f32 x, f32 y, f32 z, f32 radius);
    extern u8 partyGetAppearPos(void* party, f32* pos);
    extern f32 float_26_804215a8;
    extern f32 float_28_804215a4;
    extern f32 float_32_804215a0;
    extern f32 float_37_804215b0;
    extern f32 float_87_804215b4;
    extern f32 float_180_804215ac;
    extern f32 float_5_804215bc;
    static const s32 normalAngles[11] = { 180, 150, -150, 120, -120, 90, -90, 60, -60, 30, -30 };
    static const s32 partnerAngles[10] = { 150, -150, 120, -120, 90, -90, 60, -60, 30, -30 };
    static const s32 alternateAngles[11] = { -150, 150, -120, 120, -90, 90, -60, 60, -30, 30, -180 };
    void* player;
    void* other;
    HitWork work;
    const s32* angles;
    f32 radius;
    f32 direction;
    f32 angle;
    f32 sine;
    f32 cosine;
    u32 count;
    u32 i;
    u32 j;
    u32 attr;
    MobjName* mobj;
    void* hit;

    player = *(void**)((s32)party + 0x160);
    other = 0;
    if (*(s8*)((s32)party + 0x31) == 5) {
        radius = float_32_804215a0;
    } else if ((*(u32*)((s32)party + 8) & 0x100000) == 0) {
        radius = float_26_804215a8;
    } else {
        radius = float_28_804215a4;
    }
    radius += *(f32*)((s32)party + 0xF4);
    direction = (f32)revise360(float_180_804215ac + *(f32*)((s32)party + 0x54));
    if ((*(u32*)party & 0x100) != 0) {
        other = anotherPartyGetPtr(*(s8*)((s32)party + 0x2F));
    }
    if (other == 0) {
        outPos[0] = *(f32*)((s32)player + 0x8C);
        outPos[1] = *(f32*)((s32)player + 0x90);
        outPos[2] = *(f32*)((s32)player + 0x94);
    } else {
        outPos[0] = *(f32*)((s32)other + 0x58);
        outPos[1] = *(f32*)((s32)other + 0x5C);
        outPos[2] = *(f32*)((s32)other + 0x60);
    }
    if (other == 0) {
        angles = normalAngles;
        count = 11;
    } else if ((*(u32*)party & 0x100) == 0) {
        angles = partnerAngles;
        count = 10;
    } else {
        angles = alternateAngles;
        count = 11;
    }
    for (i = 0; i < count; i++) {
        angle = i == 0 ? *(f32*)((s32)party + 0x54) : (f32)toMovedir2(direction, (f32)angles[i]);
        sincosf(angle, &sine, &cosine);
        work.start[0] = outPos[0];
        work.start[1] = outPos[1] + float_37_804215b0;
        work.start[2] = outPos[2];
        work.dir[0] = sine;
        work.dir[1] = 0.0f;
        work.dir[2] = cosine;
        work.dist = radius;
        if (hitCheckVecFilter(&work, 0) != 0) {
            continue;
        }
        movePos(radius, angle, outPos, outPos + 2);
        work.start[0] = outPos[0];
        work.start[1] = outPos[1] + float_37_804215b0;
        work.start[2] = outPos[2];
        work.dir[0] = 0.0f;
        work.dir[1] = -1.0f;
        work.dir[2] = 0.0f;
        work.dist = float_87_804215b4;
        hit = (void*)hitCheckVecFilter(&work, 0);
        if (hit == 0) {
            continue;
        }
        attr = hitGetAttr(hit);
        if ((attr & 0xA00) != 0) {
            continue;
        }
        if ((attr & 0x80000000) != 0) {
            mobj = mobjHitObjPtrToPtr(hit);
            for (j = 0; dat_ptrarr_80314fe8[j] != 0; j++) {
                if (strcmp(mobj->name, dat_ptrarr_80314fe8[j]) == 0) {
                    break;
                }
            }
            if (dat_ptrarr_80314fe8[j] != 0) {
                continue;
            }
        }
        if (npcNearDistCheck(outPos[0], outPos[1], outPos[2], radius) == 0) {
            outPos[1] = work.hitPos[1];
            if (*(u8*)((s32)party + 0x32) != 0 && outPos[1] < *(f32*)((s32)player + 0x90)) {
                outPos[1] = *(f32*)((s32)player + 0x90) + float_5_804215bc;
            }
            return 1;
        }
    }
    partyGetAppearPos(party, outPos);
    return 0;
}

/* stub-fill: partyGetAppearPos5 | missing_definition | ghidra_signature */
u8 partyGetAppearPos5(void* pParty, void* hitObj, f32* outPos) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct MobjLocal {
        u8 pad[0x38];
        VecLocal position;
    } MobjLocal;
    extern u32 hitGetAttr(void* hit);
    extern MobjLocal* mobjHitObjPtrToPtr(void* hit);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern s32 partyHitCheck(void* party, void* pos, void* dir, f32* hitPos, void* normal, f32* distance);
    extern f32 float_50_80421590;
    extern f32 float_150_80421594;
    static const s32 angles[12] = { 0, 30, -30, 60, -60, 90, -90, 120, -120, 150, -150, 180 };
    MobjLocal* mobj;
    VecLocal pos;
    VecLocal dir;
    VecLocal hitPos;
    VecLocal normal;
    f32 distance;
    f32 sine;
    f32 cosine;
    u32 i;
    void* hit;

    if (hitObj == 0 || (hitGetAttr(hitObj) & 0x80000000) == 0) {
        return 0;
    }
    mobj = mobjHitObjPtrToPtr(hitObj);
    pos.x = mobj->position.x;
    pos.y = mobj->position.y + float_50_80421590;
    pos.z = mobj->position.z;
    for (i = 0; i < 12; i++) {
        sincosf(*(f32*)((s32)pParty + 0x100) + (f32)angles[i], &sine, &cosine);
        dir.x = sine;
        dir.y = 0.0f;
        dir.z = cosine;
        distance = float_50_80421590;
        if (partyHitCheck(pParty, &pos, &dir, (f32*)&hitPos, &normal, &distance) == 0) {
            pos.x = mobj->position.x + sine * float_50_80421590;
            pos.z = mobj->position.z + cosine * float_50_80421590;
            dir.x = 0.0f;
            dir.y = -1.0f;
            dir.z = 0.0f;
            distance = float_150_80421594;
            hit = (void*)partyHitCheck(pParty, &pos, &dir, (f32*)&hitPos, &normal, &distance);
            if (hit == 0 || (hitGetAttr(hit) & 0x200) == 0) {
                outPos[0] = pos.x;
                outPos[1] = pos.y;
                outPos[2] = pos.z;
                return 1;
            }
        }
    }
    return 0;
}
