#include "action/star/sac_zubastar.h"

extern void* _battleWorkPointer;

void* memset(void* dst, int value, u32 size);
void GXSetTexCopySrc(s32 left, s32 top, s32 wd, s32 ht);
void GXSetTexCopyDst(s32 wd, s32 ht, s32 fmt, s32 mipmap);
void GXCopyTex(void* dest, s32 clear);
void GXPixModeSync(void);
void* GetZubaStarPtr(void);

s32 star_stone_appear(void) {
    s32* ptr = GetZubaStarPtr();

    ptr[0x163] = 1;
    return 2;
}

s32 wait_star_stone_up(void) {
    s32 value = *(s32*)((s32)GetZubaStarPtr() + 0x58C);
    return (value == 4) ? 2 : 0;
}

s32 main_star(void) {
    typedef struct Vec { f32 x, y, z; } Vec;

    extern void* GetZubaStarPtr(void);
    extern void* _battleWorkPointer;
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void* effStarStoneEntry(f32 x, f32 y, f32 z, f32 scale, s32 type);
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 cur, s32 steps);
    extern void dispEntry(s32 camera, s32 mode, void* func, s32 param, f32 order);
    extern void zubaStarDispStar(void);

    extern f32 float_0_80427d40;
    extern f32 float_neg1000_80427da0;
    extern f32 float_1_80427d30;
    extern f32 float_37_80427dc4;
    extern f32 float_50_80427da4;
    extern f32 float_neg1_80427d9c;
    extern f32 float_2_80427d70;
    extern f32 float_2160_80427dc8;
    extern f32 float_300_80427d94;
    extern f32 float_1p5_80427dcc;
    extern Vec vec3_802fffa0;
    extern Vec vec3_802fffac;

    void* work;
    void* mario;
    void* eff;
    void* alloc;
    s32 timer;

    work = GetZubaStarPtr();
    mario = BattleGetMarioPtr(_battleWorkPointer);

    switch (*(s32*)((s32)work + 0x58C)) {
        case 1:
            *(s32*)((s32)work + 0x58C) = 2;
            *(s32*)((s32)work + 0x590) = 0;
            eff = effStarStoneEntry(float_0_80427d40, float_neg1000_80427da0,
                                    float_0_80427d40, float_1_80427d30, 6);
            *(void**)((s32)work + 0x5DC) = eff;

            BtlUnit_GetPos(mario,
                           (f32*)((s32)work + 0x594),
                           (f32*)((s32)work + 0x598),
                           (f32*)((s32)work + 0x59C));

            *(f32*)((s32)work + 0x598) =
                *(f32*)((s32)work + 0x598) +
                (*(f32*)((s32)mario + 0x114) * (f32)*(s16*)((s32)mario + 0xCE)) +
                float_37_80427dc4;

            *(Vec*)((s32)work + 0x5A0) = *(Vec*)((s32)work + 0x594);
            *(Vec*)((s32)work + 0x5AC) = *(Vec*)((s32)work + 0x594);

            *(f32*)((s32)work + 0x5B0) = *(f32*)((s32)work + 0x5B0) + float_50_80427da4;
            *(f32*)((s32)work + 0x5B4) = *(f32*)((s32)work + 0x5B4) + float_neg1_80427d9c;

            *(Vec*)((s32)work + 0x5C4) = vec3_802fffa0;
            *(Vec*)((s32)work + 0x5D0) = vec3_802fffac;
            /* fallthrough */

        case 2:
            *(s32*)((s32)work + 0x590) = *(s32*)((s32)work + 0x590) + 1;
            timer = *(s32*)((s32)work + 0x590);
            if (timer <= 100) {
                *(f32*)((s32)work + 0x594) = intplGetValue(
                    *(f32*)((s32)work + 0x5A0),
                    *(f32*)((s32)work + 0x5AC),
                    0,
                    timer,
                    100);

                *(f32*)((s32)work + 0x598) = intplGetValue(
                    *(f32*)((s32)work + 0x5A4),
                    *(f32*)((s32)work + 0x5B0),
                    0,
                    *(s32*)((s32)work + 0x590),
                    100);

                *(f32*)((s32)work + 0x59C) = intplGetValue(
                    *(f32*)((s32)work + 0x5A8),
                    *(f32*)((s32)work + 0x5B4),
                    0,
                    *(s32*)((s32)work + 0x590),
                    100);

                *(f32*)((s32)work + 0x5C4) = intplGetValue(
                    float_0_80427d40,
                    float_2_80427d70,
                    0,
                    *(s32*)((s32)work + 0x590),
                    100);
                *(f32*)((s32)work + 0x5C8) = *(f32*)((s32)work + 0x5C4);
                *(f32*)((s32)work + 0x5CC) = *(f32*)((s32)work + 0x5C4);
            } else {
                *(Vec*)((s32)work + 0x594) = *(Vec*)((s32)work + 0x5AC);
                *(f32*)((s32)work + 0x5C4) = float_2_80427d70;
                *(f32*)((s32)work + 0x5C8) = float_2_80427d70;
                *(f32*)((s32)work + 0x5CC) = float_2_80427d70;
            }

            *(f32*)((s32)work + 0x5D4) = intplGetValue(
                float_0_80427d40,
                float_2160_80427dc8,
                4,
                *(s32*)((s32)work + 0x590),
                120);

            if (*(s32*)((s32)work + 0x590) >= 120) {
                *(s32*)((s32)work + 0x58C) = 3;
                *(s32*)((s32)work + 0x590) = 0;
                *(Vec*)((s32)work + 0x5A0) = *(Vec*)((s32)work + 0x594);
                *(f32*)((s32)work + 0x5B0) = float_300_80427d94;
            }
            break;

        case 3:
            timer = *(s32*)((s32)work + 0x590);
            *(s32*)((s32)work + 0x590) = timer + 1;
            *(f32*)((s32)work + 0x598) = intplGetValue(
                *(f32*)((s32)work + 0x5A4),
                *(f32*)((s32)work + 0x5B0),
                1,
                *(s32*)((s32)work + 0x590),
                60);
            if (*(s32*)((s32)work + 0x590) >= 60) {
                *(s32*)((s32)work + 0x58C) = 4;
                *(s32*)((s32)work + 0x590) = 0;
            }
            break;

        case 0:
        case 4:
        default:
            break;
    }

    eff = *(void**)((s32)work + 0x5DC);
    if (eff != 0) {
        alloc = *(void**)((s32)eff + 0x0C);
        *(f32*)((s32)alloc + 0x08) = *(f32*)((s32)work + 0x594);
        *(f32*)((s32)alloc + 0x0C) = *(f32*)((s32)work + 0x598);
        *(f32*)((s32)alloc + 0x10) = *(f32*)((s32)work + 0x59C);
        *(f32*)((s32)alloc + 0x18) = *(f32*)((s32)work + 0x5D0);
        *(f32*)((s32)alloc + 0x1C) = *(f32*)((s32)work + 0x5D4);
        *(f32*)((s32)alloc + 0x20) = *(f32*)((s32)work + 0x5D8);
        *(f32*)((s32)alloc + 0x14) = float_1p5_80427dcc * *(f32*)((s32)work + 0x5C4);
    }

    dispEntry(4, 2, zubaStarDispStar, 0, float_0_80427d40);
    return 0;
}

void zubaStarDispStar(void) {
    ;
}

void* GetZubaStarPtr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

s32 weaponGetPower_ZubaStar(void* unit, s32* weapon) {
    return weapon[8 + *(s32*)((s32)GetZubaStarPtr() + 0x10)];
}

void zubastar_create_takaku(s32 count, void* points) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* GetZubaStarPtr(void);
    extern void* BattleAlloc(u32 size);
    extern const Vec vec3_802fffb8;

    void* work;
    void* obj;
    void* copy;
    s32 i;
    s32 offset;

    work = GetZubaStarPtr();
    *(void**)((s32)work + (*(s32*)((s32)work + 0x14) << 2) + 0xA8) = BattleAlloc(0x50);
    obj = *(void**)((s32)work + (*(s32*)((s32)work + 0x14) << 2) + 0xA8);
    copy = BattleAlloc(count * 0xC);
    *(void**)((s32)obj + 8) = copy;
    *(s32*)((s32)obj + 0) = 0;
    *(s32*)((s32)obj + 4) = count;
    *(Vec*)((s32)obj + 0xC) = vec3_802fffb8;
    *(s32*)((s32)obj + 0x48) = 0;
    *(s32*)((s32)obj + 0x4C) = 1;
    offset = 0;
    for (i = 0; i < count; i++) {
        *(Vec*)((s32)*(void**)((s32)obj + 8) + offset) = *(Vec*)((s32)points + offset);
        offset += 0xC;
    }
    *(s32*)((s32)work + 0x14) = *(s32*)((s32)work + 0x14) + 1;
}

void zubastar_bunkatu(void* lineStartRaw, void* lineEndRaw) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct Takaku { s32 state; s32 vertexCount; Vec* vertices; Vec cur; Vec center; Vec start; Vec target; f32 zRot; f32 zRotStep; f32 yStep; s32 frame; s32 duration; } Takaku;
    extern void* GetZubaStarPtr(void);
    extern void* BattleAlloc(s32 size);
    extern void BattleFree(void* ptr);
    extern s32 irand(s32 max);
    extern s32 zubastar_get_kouten(f32* outX, f32* outY, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4);
    extern f32 float_0_80427d40;
    extern f32 vec3_802fffc4, FLOAT_802fffc8, FLOAT_802fffcc;
    extern f32 vec3_802fffd0, FLOAT_802fffd4, FLOAT_802fffd8;

    void* work = GetZubaStarPtr();
    Vec* lineStart = (Vec*)lineStartRaw;
    Vec* lineEnd = (Vec*)lineEndRaw;
    s32 fragmentCount = *(s32*)((s32)work + 0x14);
    s32 curIx;
    s32 vertIx;
    s32 found;
    s32 i;
    s32 j;
    s32 indices[2];
    s32 flags[2];
    Vec hit[2];
    f32 hitX;
    f32 hitY;
    Takaku* cur;
    Takaku* rem;
    Takaku* neu;
    Vec* v0;
    Vec* v1;

    for (curIx = 0; curIx < fragmentCount; curIx++) {
        cur = *(Takaku**)((s32)work + 0xA8 + curIx * 4);
        found = 0;
        flags[0] = 0;
        flags[1] = 0;
        for (vertIx = 0; vertIx < cur->vertexCount && found != 2; vertIx++) {
            v0 = &cur->vertices[vertIx];
            v1 = &cur->vertices[(vertIx + 1) % cur->vertexCount];
            if (zubastar_get_kouten(&hitX, &hitY, lineStart->x, lineStart->y, lineEnd->x, lineEnd->y, v0->x, v0->y, v1->x, v1->y) != 0) {
                for (i = 0; i < found; i++) {
                    if (flags[i] == 1 && (s32)hitX == (s32)hit[i].x && (s32)hitY == (s32)hit[i].y) {
                        break;
                    }
                }
                if (i == found) {
                    hit[i].x = hitX;
                    hit[i].y = hitY;
                    hit[i].z = float_0_80427d40;
                    indices[i] = vertIx;
                    flags[i] = 1;
                    found++;
                }
            }
        }
        if (found == 2) {
            rem = (Takaku*)BattleAlloc(0x50);
            neu = (Takaku*)BattleAlloc(0x50);
            rem->vertexCount = (cur->vertexCount - indices[1]) + indices[0] + 2;
            neu->vertexCount = (indices[1] - indices[0]) + 2;
            rem->vertices = (Vec*)BattleAlloc(rem->vertexCount * 0xC);
            neu->vertices = (Vec*)BattleAlloc(neu->vertexCount * 0xC);
            j = 0;
            for (i = 0; i <= indices[0]; i++) {
                rem->vertices[j++] = cur->vertices[i];
            }
            rem->vertices[j++] = hit[0];
            rem->vertices[j++] = hit[1];
            for (i = indices[1] + 1; i < cur->vertexCount; i++) {
                rem->vertices[j++] = cur->vertices[i];
            }
            neu->vertices[0] = hit[0];
            for (i = 1; i <= neu->vertexCount - 2; i++) {
                neu->vertices[i] = cur->vertices[indices[0] + i];
            }
            neu->vertices[neu->vertexCount - 1] = hit[1];

            rem->state = 0;
            rem->cur.x = vec3_802fffc4;
            rem->cur.y = FLOAT_802fffc8;
            rem->cur.z = FLOAT_802fffcc;
            rem->frame = irand(0x80);
            rem->duration = irand(5) + 1;
            neu->state = 0;
            neu->cur.x = vec3_802fffd0;
            neu->cur.y = FLOAT_802fffd4;
            neu->cur.z = FLOAT_802fffd8;
            neu->frame = irand(0x80);
            neu->duration = irand(5) + 1;
            BattleFree(cur->vertices);
            BattleFree(cur);
            *(Takaku**)((s32)work + 0xA8 + curIx * 4) = rem;
            *(Takaku**)((s32)work + 0xA8 + fragmentCount * 4) = neu;
            *(s32*)((s32)work + 0x14) = fragmentCount + 1;
            fragmentCount++;
        }
    }
}

s32 zubastar_get_kouten(f32* outX, f32* outY, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4) {
    extern f32 float_0_80427d40;
    extern f32 float_1Eneg05_80427dc0;
    extern f32 float_1_80427d30;

    f32 dx1;
    f32 dx2;
    f32 slope1;
    f32 slope2;
    f32 denom;
    f32 zero;
    f32 t;

    dx1 = x2 - x1;
    zero = float_0_80427d40;
    if (zero == dx1 && zero == (x4 - x3)) {
        return 0;
    }
    if (zero != dx1) {
        dx1 = dx1;
    } else {
        dx1 = float_1Eneg05_80427dc0;
    }
    dx2 = x4 - x3;
    if (zero != dx2) {
        dx2 = dx2;
    } else {
        dx2 = float_1Eneg05_80427dc0;
    }
    slope1 = (y2 - y1) / dx1;
    slope2 = (y4 - y3) / dx2;
    denom = slope1 - slope2;
    if (zero == denom) {
        return 0;
    }
    *outX = ((y3 - y1) + (slope2 * x3 - slope1 * x1)) / denom;
    if (zero != (x2 - x1)) {
        *outY = slope1 * (*outX - x1) + y1;
    } else {
        *outY = slope2 * (*outX - x3) + y3;
    }
    t = (*outX - x3) / dx2;
    if (t >= zero && t <= float_1_80427d30) {
        return 1;
    }
    return 0;
}



/* CHATGPT STUB FILL: main/action/star/sac_zubastar 20260624_184929 */

/* stub-fill: zubastar_main | missing_definition | ghidra_signature */
s32 zubastar_main(void* event, s32 isFirstCall) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern u8* GetZubaStarPtr(void);
    extern s32 BattleAudience_GetAudienceNum(void);
    extern u32 GXGetTexBufferSize(s32, s32, s32, s32, s32);
    extern s32 keyGetButtonTrg(s32);
    extern void psndSFXOff(s32);
    extern void BattleAudienceSoundCallKind(s32);
    extern void BattleAudienceSoundWhistleKind(s32);
    extern s32 irand(s32);
    extern void* smartAlloc(s32, s32);
    extern void zubastar_create_takaku(s32, void*);
    extern void btl_camera_shake_h(f32, f32, s32, s32, s32);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void zubastar_disp2D(void);
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    u8* work = GetZubaStarPtr();
    s32 audience = BattleAudience_GetAudienceNum();
    s32* state = (s32*)work;

    if (isFirstCall != 0) {
        Vec fragments[4];
        u32 size = GXGetTexBufferSize(400, 0x1E0, 4, 0, 0);
        *(void**)(work + 0x5D8) = smartAlloc(size, 1);
        fragments[0].x = -200.0f; fragments[0].y = 240.0f; fragments[0].z = 0.0f;
        fragments[1].x = 200.0f; fragments[1].y = 240.0f; fragments[1].z = 0.0f;
        fragments[2].x = -200.0f; fragments[2].y = -240.0f; fragments[2].z = 0.0f;
        fragments[3].x = 200.0f; fragments[3].y = -240.0f; fragments[3].z = 0.0f;
        zubastar_create_takaku(4, fragments);
        *(f32*)(work + 0xA0) = 0.0f;
        *(f32*)(work + 0xA4) = 300.0f;
        *(f32*)(work + 0xA8) = 0.0f;
    }

    switch (*state) {
        case 0:
            *state = 5;
            *(s32*)(work + 0x94) = 5;
            break;
        case 5:
            *state = 10;
            *(s32*)(work + 4) = 0;
            if (*(s32*)(work + 0x518) == 0) {
                *(s32*)(work + 0x518) = 1;
            }
            if (*(s32*)(work + 0x518) == 5) {
                *(s32*)(work + 0x518) = 6;
            }
        case 10:
            if (*(s32*)(work + 0x94) == 10 && *(s32*)(work + 0x518) == 2) {
                *state = 11;
            }
            break;
        case 11:
            *state = 12;
            *(s32*)(work + 0xC) = 300;
        case 12:
            if (*(f32*)(work + 0x520) < 100.0f) {
                if (--*(s32*)(work + 0xC) < 1) {
                    *state = 13;
                    psndSFXOff(*(s32*)(work + 0x530));
                } else if (keyGetButtonTrg(0) & 0x100) {
                    *(f32*)(work + 0x524) = 2.8f;
                }
            } else {
                *state = 14;
                (*(s32*)(work + 8))++;
                psndSFXOff(*(s32*)(work + 0x530));
            }
            break;
        case 13:
            *(s32*)(work + 0x518) = 5;
            psndSFXOff(*(s32*)(work + 0x530));
            *state = 0x1E;
            break;
        case 14:
            *(s32*)(work + 0x518) = 4;
            *(s32*)(work + 0x51C) = 0;
            psndSFXOff(*(s32*)(work + 0x530));
            if (audience > 0 && audience < 50) {
                BattleAudienceSoundCallKind(1);
            } else if (audience < 100) {
                BattleAudienceSoundCallKind(1);
                BattleAudienceSoundWhistleKind(1);
            } else {
                BattleAudienceSoundCallKind(2);
                BattleAudienceSoundWhistleKind(audience > 150 ? 3 : 2);
            }
            *state = 15;
            *(s32*)(work + 0x94) = 15;
            break;
        case 15:
            if (*(s32*)(work + 0x94) == 20) {
                *state = 16;
                *(s32*)(work + 4) = 0;
            }
            break;
        case 16:
            *state = 17;
            *(s32*)(work + 0x94) = 25;
            if (irand(2) == 0) {
                *(f32*)(work + 0xA0) = -96.0f;
            } else {
                *(f32*)(work + 0xA0) = 96.0f;
            }
            break;
        case 17:
            if (*(s32*)(work + 0x94) == 30) {
                (*(s32*)(work + 4))++;
                *state = *(s32*)(work + 4) < 3 ? 16 : 32;
            }
            break;
        case 32:
            *state = *(s32*)(work + 8) < 5 ? 0 : 33;
            break;
        case 33:
            *state = 0x24;
            *(s32*)(work + 0x94) = 25;
            break;
        case 0x24:
            if (*(s32*)(work + 0x94) == 30) {
                *state = 0x26;
                *(s32*)(work + 4) = 60;
                btl_camera_shake_h(5.0f, 5.0f, 0, 60, 0);
            }
            break;
        case 0x26:
            if (--*(s32*)(work + 4) < 1) {
                *state = 0x28;
            }
            break;
        case 0x28:
            {
                s32 i;
                for (i = 0; i < 16; i++) {
                    *(s32*)(work + 0xC0 + i * 0x1C) = 0;
                }
            }
            return 2;
    }
    switch (*(s32*)(work + 0x94)) {
    case 5:
        *(s32*)(work + 0x94) = 6;
        *(s32*)(work + 0x98) = 0;
    case 6:
        (*(s32*)(work + 0x98))++;
        *(f32*)(work + 0xA0) = 0.0f;
        *(f32*)(work + 0xA4) = (f32)intplGetValue(
            300.0, 300.0, 4, *(s32*)(work + 0x98), 15);
        *(f32*)(work + 0xA8) = 0.0f;
        if (*(s32*)(work + 0x98) > 14) {
            *(s32*)(work + 0x94) = 10;
        }
        break;
    case 10:
        *(f32*)(work + 0xA0) = 0.0f;
        *(f32*)(work + 0xA4) = 300.0f;
        *(f32*)(work + 0xA8) = 0.0f;
        break;
    }
    switch (*(s32*)(work + 0x518)) {
    case 1:
        *(s32*)(work + 0x518) = 2;
        *(s32*)(work + 0x51C) = 0;
        *(f32*)(work + 0x520) = 0.0f;
        *(f32*)(work + 0x524) = 0.0f;
        *(f32*)(work + 0x528) = -1.0f;
        *(s32*)(work + 0x52C) = 0xFF;
        break;
    case 2:
        *(f32*)(work + 0x520) += *(f32*)(work + 0x524);
        *(f32*)(work + 0x520) += *(f32*)(work + 0x528);
        if (*(f32*)(work + 0x520) > 100.0f) {
            *(f32*)(work + 0x520) = 100.0f;
        }
        if (*(f32*)(work + 0x520) < 0.0f) {
            *(f32*)(work + 0x520) = 0.0f;
        }
        *(f32*)(work + 0x524) -= 0.25f;
        if (*(f32*)(work + 0x524) < 0.0f) {
            *(f32*)(work + 0x524) = 0.0f;
        }
        (*(s32*)(work + 0x52C))++;
        if (*(s32*)(work + 0x52C) > 7) {
            *(s32*)(work + 0x52C) = 0;
        }
        break;
    }
    {
        Vec* ghosts = (Vec*)(work + 0xAC);
        ghosts[3] = ghosts[2];
        ghosts[2] = ghosts[1];
        ghosts[1] = ghosts[0];
        ghosts[0].x = *(f32*)(work + 0xA0);
        ghosts[0].y = *(f32*)(work + 0xA4);
        ghosts[0].z = *(f32*)(work + 0xA8);
    }
    {
        s32 i;
        for (i = 0; i < 16; i++) {
            u8* line = work + 0xC0 + i * 0x1C;
            if (*(s32*)line == 5) {
                (*(s32*)(line + 4))++;
                if (*(s32*)(line + 4) > 60) {
                    *(s32*)(line + 4) = 0;
                }
            }
        }
    }
    dispEntry(1, 1, zubastar_disp2D, 0, 900.0f);
    return 0;
}

s32 zubastar_bomb(void* evt, s32 isFirstCall) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct Takaku {
        s32 state;
        s32 vertexCount;
        Vec* vertices;
        Vec cur;
        Vec center;
        Vec start;
        Vec target;
        f32 zRot;
        f32 zRotStep;
        f32 yStep;
        s32 frame;
        s32 duration;
    } Takaku;

    extern void* GetZubaStarPtr(void);
    extern void dispEntry(s32 camera, s32 mode, void* func, s32 param, f32 order);
    extern s32 irand(s32 max);
    extern s32 rand(void);
    extern void psndSFXOn(char* name);
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 cur, s32 steps);
    extern void zubastar_capture(void);
    extern void zubastar_disp_bomb(void);
    extern char str_btl_wn_sac_zubastar_802fff70[];
    extern f32 float_0_80427d40;
    extern f32 float_104_80427d60;
    extern f32 float_200_80427d28;
    extern f32 float_600_80427d5c;
    extern f32 float_70_80427d64;
    extern f32 float_32767_80427d68;
    extern f32 float_35_80427d6c;
    extern f32 float_2_80427d70;
    extern f32 float_neg1p25_80427d74;

    char* rodata = str_btl_wn_sac_zubastar_802fff70;
    void* work = GetZubaStarPtr();
    s32 offset;
    s32 i;
    char* sfx;
    Takaku* frag;
    s32 finished;
    s32 count;
    Vec* v;
    f32 sumX;
    f32 sumY;
    f32 sumZ;
    s32 vertexCount;
    s32 durationFrames;
    s32 r;
    f32 duration;
    f32 zero;

    if (isFirstCall != 0) {
        dispEntry(8, 0, zubastar_capture, 0, float_0_80427d40);
        *(s32*)((s32)work + 4) = 0;
        return 0;
    }

    if (*(s32*)((s32)work + 4) == 0) {
        count = *(s32*)((s32)work + 0x14);
        finished = 0;
        offset = 0;
        for (i = 0; i < count; i++) {
            frag = *(Takaku**)((s32)work + offset + 0xA8);
            if (frag->state != 10) {
                break;
            }
            finished++;
            offset += 4;
        }
        if (finished == count) {
            return 2;
        }
    }

    sfx = rodata + 0x1A4;
    offset = 0;
    for (i = 0; i < *(s32*)((s32)work + 0x14); i++) {
        frag = *(Takaku**)((s32)work + offset + 0xA8);

        switch (frag->state) {
            case 0:
                durationFrames = irand(0x1E) + 0x1E;
                zero = float_0_80427d40;
                frag->duration = durationFrames;
                frag->start = *(Vec*)(rodata + 0x144);
                frag->target = *(Vec*)(rodata + 0x150);

                sumX = zero;
                sumY = zero;
                sumZ = zero;
                vertexCount = frag->vertexCount;
                {
                    s32 j;
                    s32 vertexOffset = 0;
                    for (j = 0; j < vertexCount; j++) {
                        v = (Vec*)((s32)frag->vertices + vertexOffset);
                        vertexOffset += 0xC;
                        sumX += v->x;
                        sumY += v->y;
                        sumZ += v->z;
                    }
                }

                frag->center.x = sumX / (f32)vertexCount;
                frag->center.y = sumY / (f32)frag->vertexCount;
                frag->center.z = sumZ / (f32)frag->vertexCount;
                frag->target.x += (float_600_80427d5c * (frag->center.x - float_104_80427d60)) / float_200_80427d28;
                frag->zRot = zero;

                r = rand();
                frag->zRotStep = (float_70_80427d64 * (f32)r) / float_32767_80427d68 - float_35_80427d6c;

                duration = (f32)durationFrames;
                frag->yStep = ((float_2_80427d70 * (frag->target.y - frag->start.y)) -
                               (duration * float_neg1p25_80427d74 * duration)) /
                              (float_2_80427d70 * duration);
                frag->frame = 0;
                frag->state = 5;
                psndSFXOn(sfx);
                break;

            case 5:
                frag->cur.x = intplGetValue(frag->start.x, frag->target.x, 0, frag->frame, frag->duration);
                frag->cur.z = intplGetValue(frag->start.z, frag->target.z, 0, frag->frame, frag->duration);
                frag->cur.y = frag->cur.y + frag->yStep;
                frag->zRot = frag->zRot + frag->zRotStep;
                frag->yStep = frag->yStep + float_neg1p25_80427d74;
                frag->frame++;
                if (frag->frame > frag->duration) {
                    frag->state = 10;
                }
                break;

            case 10:
            default:
                break;
        }

        offset += 4;
    }

    dispEntry(8, 1, zubastar_disp_bomb, 0, float_0_80427d40);
    return 0;
}

s32 zubastar_init(void) {
    memset(GetZubaStarPtr(), 0, 0x5E0);
    return 2;
}

s32 zubastar_end(void) {
    extern void* GetZubaStarPtr(void);
    extern void effDelete(void* effect);
    extern void BattleFree(void* ptr);
    extern void smartFree(void* ptr);

    void* work;
    s32 i;
    s32 offset;

    work = GetZubaStarPtr();
    effDelete(*(void**)((s32)*(void**)((s32)*(void**)((s32)work + 0x5DC) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x5DC));
    offset = 0;
    for (i = 0; i < 0xC8; i++) {
        if (*(void**)((s32)work + offset + 0xA8) != 0) {
            if (*(void**)((s32)*(void**)((s32)work + offset + 0xA8) + 8) != 0) {
                BattleFree(*(void**)((s32)*(void**)((s32)work + offset + 0xA8) + 8));
                *(void**)((s32)*(void**)((s32)work + offset + 0xA8) + 8) = 0;
            }
            BattleFree(*(void**)((s32)work + offset + 0xA8));
            *(void**)((s32)work + offset + 0xA8) = 0;
        }
        offset += 4;
    }
    smartFree(*(void**)((s32)work + 0x588));
    return 2;
}

void zubastar_capture(void) {
    void* work = GetZubaStarPtr();

    GXSetTexCopySrc(0xD0, 0, 0x190, 0x1E0);
    GXSetTexCopyDst(0x190, 0x1E0, 4, 0);
    GXCopyTex(*(void**)*(s32*)((s32)work + 0x588), 0);
    GXPixModeSync();
}

void zubastar_disp_bomb(void) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct Takaku { s32 state; s32 vertexCount; Vec* vertices; Vec cur; Vec center; Vec start; Vec target; f32 zRot; f32 zRotStep; f32 yStep; s32 frame; s32 duration; } Takaku;
    extern void* GetZubaStarPtr(void);
    extern void* camGetPtr(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetZCompLoc(s32 loc);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 type, s32 frac, s32 shift);
    extern void GXSetNumChans(s32 n);
    extern void GXSetNumTexGens(s32 n);
    extern void GXSetTexCoordGen2(s32 texgen, s32 type, s32 src, s32 mtx, s32 normalize, s32 postmtx);
    extern void GXSetNumTevStages(s32 n);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outreg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outreg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXInitTexObj(void* obj, void* image, s32 w, s32 h, s32 fmt, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXInitTexObjLOD(void* obj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 edgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* obj, s32 mapid);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 count);
    extern f32 float_0_80427d40, float_96_80427d4c, float_400_80427d50, float_240_80427d54, float_480_80427d58, float_deg2rad_80427d48;

    void* work = GetZubaStarPtr();
    void* cam = camGetPtr(1);
    s32 count = *(s32*)((s32)work + 0x14);
    s32 i, j;
    Takaku* frag;
    Vec* v;
    u8 texObj[0x20];
    u8 trans[0x30];
    u8 back[0x30];
    u8 rot[0x30];
    u8 move[0x30];
    u8 mtx[0x30];
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 texXOff;
    f32 texW;
    f32 texYBase;
    f32 texH;
    f32 deg;

    GXSetCullMode(0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(7, 0, 1, 0, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 0, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXInitTexObj(texObj, **(void***)((s32)work + 0x588), 0x190, 0x1E0, 4, 0, 0, 0);
    GXInitTexObjLOD(texObj, 1, 1, float_0_80427d40, float_0_80427d40, float_0_80427d40, 0, 0, 0);
    GXLoadTexObj(texObj, 0);

    texXOff = float_96_80427d4c;
    texW = float_400_80427d50;
    texYBase = float_240_80427d54;
    texH = float_480_80427d58;
    deg = float_deg2rad_80427d48;
    for (i = 0; i < count; i++) {
        frag = *(Takaku**)((s32)work + 0xA8 + i * 4);
        PSMTXTrans(trans, frag->center.x, frag->center.y, frag->center.z);
        PSMTXTrans(back, -frag->center.x, -frag->center.y, -frag->center.z);
        PSMTXTrans(move, frag->cur.x, frag->cur.y, frag->cur.z);
        PSMTXRotRad(rot, 'z', deg * frag->zRot);
        PSMTXConcat(rot, back, mtx);
        PSMTXConcat(trans, mtx, mtx);
        PSMTXConcat(move, mtx, mtx);
        PSMTXConcat((void*)((s32)cam + 0x11C), mtx, mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXBegin(0xA0, 0, (s16)frag->vertexCount);
        for (j = 0; j < frag->vertexCount; j++) {
            v = &frag->vertices[j];
            fifo[0] = v->x;
            fifo[0] = v->y;
            fifo[0] = float_0_80427d40;
            fifo[0] = (texXOff + v->x) / texW;
            fifo[0] = (texYBase - v->y) / texH;
        }
    }
}

void zubastar_disp2D(s32 cameraId) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct Vec2 { f32 x, y; } Vec2;
    typedef struct GXColor { u8 r, g, b, a; } GXColor;
    typedef struct SplitLine { s32 state; s32 unk4; s32 frame; Vec2 start; Vec2 end; } SplitLine;
    typedef struct LineTracer { s32 state; s32 frame2; Vec curPos; Vec ghosts[4]; Vec startPos; Vec targetPos; s32 frame; s16 curLine; s16 unk62; } LineTracer;
    typedef struct AcGauge { s32 state; s32 moveFrame; f32 percent; f32 inc; f32 fightback; f32 unk14; Vec drawPos; u8 pad24; u8 btnFrame; u8 pad26; u8 pad27; } AcGauge;

    extern void* GetZubaStarPtr(void);
    extern void iconDispGx(f32 scale, Vec* pos, s32 flags, s32 iconId);
    extern void iconDispGxCol(void* mtx, s32 flags, s32 iconId, void* color);
    extern void BattleAcDrawGauge(f32 ratio, s32 x, s32 y, s32 innerBarWidth, s32 param5, s32 b1, s32 b2, s32 b3, s32 flags);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 type, s32 frac, s32 shift);
    extern void GXSetNumChans(s32 n);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambsrc, s32 matsrc, s32 lightmask, s32 diff, s32 attn);
    extern void GXSetNumTexGens(s32 n);
    extern void GXSetNumTevStages(s32 n);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outreg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outreg);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetCurrentMtx(s32 id);
    extern void* camGetPtr(s32 id);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetLineWidth(s32 width, s32 fmt);
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 cur, s32 steps);
    extern void GXSetChanMatColor(s32 chan, GXColor* color);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 count);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern f32 float_200_80427d28, float_60_80427d2c, float_1_80427d30, float_0p01_80427d34;
    extern f32 float_128_80427d38, float_255_80427d3c, float_0_80427d40, float_20_80427d44;
    extern u32 dat_80427d18, dat_80427d1c;

    void* work = GetZubaStarPtr();
    LineTracer* tracer = (LineTracer*)((s32)work + 0x18);
    AcGauge* gauge = (AcGauge*)((s32)work + 0x7C);
    SplitLine* line;
    Vec pos;
    GXColor color;
    GXColor color2;
    u8 bright;
    s32 frame;
    s32 i;
    f32 value;
    u8 mtx[0x30];
    volatile f32* fifo = (volatile f32*)0xCC008000;

    if (gauge->state > 4) {
        pos.x = gauge->drawPos.x - float_200_80427d28;
        pos.y = gauge->drawPos.y + float_60_80427d2c;
        pos.z = float_0_80427d40;
        iconDispGx(float_1_80427d30, &pos, 0x10, (gauge->btnFrame < 4) ? 0x6C : 0x6D);
        BattleAcDrawGauge(float_0p01_80427d34 * gauge->percent, (s32)gauge->drawPos.x, (s32)gauge->drawPos.y, 0xB1, 4, 0x1A, 0x34, 0x4E, 0);
    }

    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 4);
    GXSetCurrentMtx(0);
    GXLoadPosMtxImm((void*)((s32)camGetPtr(cameraId) + 0x11C), 0);
    GXSetLineWidth(0x18, 0);

    for (i = 0; i < 0x10; i++) {
        line = (SplitLine*)((s32)work + 0x3C8 + i * 0x1C);
        frame = line->frame;
        if (frame < 0x1F) {
            value = intplGetValue(float_128_80427d38, float_255_80427d3c, 0, frame, 0x1E);
        } else {
            value = intplGetValue(float_255_80427d3c, float_128_80427d38, 0, frame - 0x1E, 0x1E);
        }
        bright = (u8)(s32)value;
        if (line->state == 1) {
            color.r = bright;
            color.g = bright;
            color.b = bright;
            color.a = (u8)dat_80427d18;
            color2 = color;
            GXSetChanMatColor(4, &color2);
            GXBegin(0xA8, 0, 2);
            fifo[0] = line->start.x;
            fifo[0] = line->start.y;
            fifo[0] = float_0_80427d40;
            fifo[0] = line->end.x;
            fifo[0] = line->end.y;
            fifo[0] = float_0_80427d40;
        }
    }

    iconDispGx(float_1_80427d30, &tracer->curPos, 0x10, 0x1A5);
    if (tracer->state > 0x18 && tracer->state < 0x1E) {
        for (i = 0; i < 4; i++) {
            PSMTXTrans(mtx, tracer->ghosts[i].x, tracer->ghosts[i].y - float_20_80427d44, float_0_80427d40);
            color.r = (u8)(((i + 1) * -0x32) - 1);
            color.g = (u8)(dat_80427d1c >> 16);
            color.b = (u8)(dat_80427d1c >> 8);
            color.a = (u8)dat_80427d1c;
            color2 = color;
            iconDispGxCol(mtx, 0x10, 0x1A5, &color2);
        }
    }
}
