#include "effect/eff_fukidashi.h"


u8 effFukidashiDisp(s32 cameraId, void* effect) {
    extern void mapSetMaterialFog(void);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXBegin(s32, s32, s32);
    volatile f32* fifo;
    s32* work;
    s32 sides;
    s32 side;
    s32 alpha;

    work = *(s32**)((s32)effect + 0xC);
    if (work == 0) return 0;
    alpha = work[0x12];
    sides = ((f32)work[9] != 0.0f || work[0] == 2) ? 2 : 1;
    fifo = (volatile f32*)0xCC008000;
    mapSetMaterialFog();
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZMode(1, 3, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    for (side = 0; side < sides; side++) {
        GXSetCullMode(side == 0 ? 2 : 1);
        GXBegin(0x80, 0, 4);
        fifo[0] = -32.0f; fifo[0] = 24.0f; fifo[0] = 0.0f;
        fifo[0] = 0.0f; fifo[0] = 0.0f;
        fifo[0] = 32.0f; fifo[0] = 24.0f; fifo[0] = 0.0f;
        fifo[0] = 1.0f; fifo[0] = 0.0f;
        fifo[0] = 32.0f; fifo[0] = -24.0f; fifo[0] = 0.0f;
        fifo[0] = 1.0f; fifo[0] = 1.0f;
        fifo[0] = -32.0f; fifo[0] = -24.0f; fifo[0] = 0.0f;
        fifo[0] = 0.0f; fifo[0] = 1.0f;
    }
    return alpha != 0;
}

u8 calc_pos(void* work, s32 flag) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* camGetPtr(s32 cameraId);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern void PSMTXRotAxisRad(f32 mtx[3][4], Vec* axis, f32 rad);
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern Vec vec3_802f3648;
    extern f32 float_deg2rad_804228c4;
    extern f32 float_20_80422950;
    extern f32 float_16_804228e0;
    extern f32 float_12_80422954;
    extern f32 float_0p5_8042291c;

    void* cam;
    void* base;
    Vec axis;
    f32 mtx[3][4];
    f32 distance;
    s32 target;
    s32 offset;

    cam = camGetPtr(4);
    axis = vec3_802f3648;
    axis.x = (f32)sin(float_deg2rad_804228c4 * *(f32*)((s32)cam + 0x114));
    axis.z = -(f32)cos(float_deg2rad_804228c4 * *(f32*)((s32)cam + 0x114));
    offset = flag * 0x14;
    PSMTXRotAxisRad(mtx, &axis,
                    float_deg2rad_804228c4 *
                        -((*(f32*)((s32)work + 0x1C) - float_20_80422950) + (f32)offset));

    target = *(s32*)((s32)work + 0x44);
    distance = float_16_804228e0 + *(f32*)((s32)work + 0x20);
    if (target == -1) {
        base = marioGetPtr();
        *(f32*)((s32)work + 4) = mtx[0][1] * distance + *(f32*)((s32)base + 0x8C) + *(f32*)((s32)work + 0x10);
        *(f32*)((s32)work + 8) = mtx[1][1] * distance + *(f32*)((s32)base + 0x90) + *(f32*)((s32)work + 0x14);
        *(f32*)((s32)work + 0xC) = mtx[2][1] * distance + *(f32*)((s32)base + 0x94) + *(f32*)((s32)work + 0x18);
    } else if (target == -2) {
        base = partyGetPtr(marioGetPartyId());
        if (base != 0) {
            *(f32*)((s32)work + 4) = mtx[0][1] * distance + *(f32*)((s32)base + 0x58) + *(f32*)((s32)work + 0x10);
            *(f32*)((s32)work + 8) = mtx[1][1] * distance + *(f32*)((s32)base + 0x5C) + *(f32*)((s32)work + 0x14);
            *(f32*)((s32)work + 0xC) = mtx[2][1] * distance + *(f32*)((s32)base + 0x60) + *(f32*)((s32)work + 0x18);
        }
    } else if (target == -3) {
        base = partyGetPtr(marioGetExtraPartyId());
        if (base != 0) {
            *(f32*)((s32)work + 4) = mtx[0][1] * distance + *(f32*)((s32)base + 0x58) + *(f32*)((s32)work + 0x10);
            *(f32*)((s32)work + 8) = mtx[1][1] * distance + *(f32*)((s32)base + 0x5C) + *(f32*)((s32)work + 0x14);
            *(f32*)((s32)work + 0xC) = mtx[2][1] * distance + *(f32*)((s32)base + 0x60) + *(f32*)((s32)work + 0x18);
        }
    } else if (target == 0) {
        *(f32*)((s32)work + 4) = mtx[0][1] * distance + *(f32*)((s32)work + 0x10);
        *(f32*)((s32)work + 8) = mtx[1][1] * distance + *(f32*)((s32)work + 0x14);
        *(f32*)((s32)work + 0xC) = mtx[2][1] * distance + *(f32*)((s32)work + 0x18);
    } else {
        *(f32*)((s32)work + 4) = mtx[0][1] * distance + *(f32*)(target + 0x8C) + *(f32*)((s32)work + 0x10);
        *(f32*)((s32)work + 8) = mtx[1][1] * distance + *(f32*)(target + 0x90) + *(f32*)((s32)work + 0x14);
        *(f32*)((s32)work + 0xC) = mtx[2][1] * distance + *(f32*)(target + 0x94) + *(f32*)((s32)work + 0x18);
    }

    *(f32*)((s32)work + 0x28) = (*(f32*)((s32)work + 0x1C) - float_20_80422950) + (f32)offset;
    *(f32*)((s32)work + 0x2C) = float_0p5_8042291c * (*(f32*)((s32)work + 0x20) / float_12_80422954) + float_0p5_8042291c;
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effFukidashiEntry(f32 x, f32 y, f32 z, f32 scale, f32 angle, s32 kind, s32 field3c, s32 target, s32 timer) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern u8 effFukidashiMain(void* effect);
    extern u8 calc_pos(void* work, s32 flag);
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern u32 psndSFXOn_3D(char* id, Vec* pos);
    extern char str_Fukidashi_802f36b0[];
    extern char str_SFX_EVT_NPC_BIKKURI__802f36bc[];
    extern char str_SFX_EVT_NPC_QUESTION_802f36d8[];
    extern f32 float_0_804228c0;
    extern f32 float_3_80422930;
    extern f32 float_neg450_8042294c;

    void* effect;
    void* work;
    void* base;
    Vec soundPos;
    f32 soundX;
    f32 soundY;
    f32 soundZ;

    effect = effEntry();
    *(char**)((s32)effect + 0x14) = str_Fukidashi_802f36b0;
    *(s32*)((s32)effect + 8) = 1;
    work = __memAlloc(3, 0x58);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effFukidashiMain;

    *(s32*)work = kind;
    *(s32*)((s32)work + 0x3C) = field3c;
    if (timer < 0) {
        timer = 10000;
    }
    *(s32*)((s32)work + 0x40) = timer;
    *(s32*)((s32)work + 0x30) = timer;
    *(s32*)((s32)work + 0x34) = 0;
    *(s32*)((s32)work + 0x44) = target;
    *(f32*)((s32)work + 0x24) = float_0_804228c0;
    *(s32*)((s32)work + 0x48) = 0xFF;
    *(s32*)((s32)work + 0x4C) = 4;
    *(f32*)((s32)work + 0x10) = x;
    *(f32*)((s32)work + 0x14) = y + float_3_80422930;
    *(f32*)((s32)work + 0x18) = z;
    *(f32*)((s32)work + 0x1C) = angle;
    *(f32*)((s32)work + 0x20) = scale;
    *(s32*)((s32)work + 0x54) = 0;

    calc_pos(work, 1);
    if (kind == 6) {
        *(f32*)((s32)work + 0x24) = float_neg450_8042294c;
    }

    soundX = x;
    soundY = y;
    soundZ = z;
    if (target == -1) {
        base = marioGetPtr();
        soundX = x + *(f32*)((s32)base + 0x8C);
        soundY = y + *(f32*)((s32)base + 0x90);
        soundZ = z + *(f32*)((s32)base + 0x94);
    } else if (target == -2) {
        base = partyGetPtr(marioGetPartyId());
        if (base != 0) {
            soundX = x + *(f32*)((s32)base + 0x58);
            soundY = y + *(f32*)((s32)base + 0x5C);
            soundZ = z + *(f32*)((s32)base + 0x60);
        }
    } else if (target == -3) {
        base = partyGetPtr(marioGetExtraPartyId());
        if (base != 0) {
            soundX = x + *(f32*)((s32)base + 0x58);
            soundY = y + *(f32*)((s32)base + 0x5C);
            soundZ = z + *(f32*)((s32)base + 0x60);
        }
    } else if (target != 0) {
        soundX = *(f32*)(target + 0x8C);
        soundY = *(f32*)(target + 0x90);
        soundZ = *(f32*)(target + 0x94);
    }

    if ((target == -2) || (target == -3)) {
        if (kind == 1) {
            soundPos.x = soundX;
            soundPos.y = soundY;
            soundPos.z = soundZ;
            psndSFXOn_3D((char*)0x5E, &soundPos);
        } else if (kind == 0) {
            soundPos.x = soundX;
            soundPos.y = soundY;
            soundPos.z = soundZ;
            psndSFXOn_3D((char*)0x5D, &soundPos);
        }
    } else if ((target != 0) && (target != -1)) {
        if (kind == 1) {
            soundPos.x = soundX;
            soundPos.y = soundY;
            soundPos.z = soundZ;
            psndSFXOn_3D(str_SFX_EVT_NPC_QUESTION_802f36d8, &soundPos);
        } else if (kind == 0) {
            soundPos.x = soundX;
            soundPos.y = soundY;
            soundPos.z = soundZ;
            psndSFXOn_3D(str_SFX_EVT_NPC_BIKKURI__802f36bc, &soundPos);
        }
    }

    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effFukidashiMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effFukidashiDisp(s32 camera, void* effect);
    extern u8 calc_pos(void* work, s32 flag);
    extern LocalVec3 vec3_802f3684;
    extern s8* anim_data_table[];
    extern s8* anim_data_table_2[];
    extern f32 float_22p5_80422940;
    extern f32 float_0_804228c0;
    extern f32 float_9_80422944;
    extern f32 float_neg90_80422948;
    void* work;
    LocalVec3 dispPos;
    LocalVec3 pos;
    s8* anim;
    s32 value;
    s32 frame;
    s32 kind;

    work = *(void**)((s32)effect + 0xC);
    pos = vec3_802f3684;
    pos.x = *(f32*)((s32)work + 0x4);
    pos.y = *(f32*)((s32)work + 0x8);
    pos.z = *(f32*)((s32)work + 0xC);
    dispPos = pos;
    kind = *(s32*)work;

    if (*(s32*)((s32)work + 0x40) == 0) {
        anim = anim_data_table[kind];
    } else {
        anim = anim_data_table_2[kind];
    }
    frame = *(s32*)((s32)work + 0x34);
    value = anim[frame / 2];
    *(s32*)((s32)work + 0x34) = frame + 1;
    *(s32*)((s32)work + 0x38) = value;

    if (value < 0) {
        if (value == -1) {
            effDelete(effect);
            return 0;
        }
        *(s32*)((s32)work + 0x34) = (-10 - value) * 2;
        if (*(s32*)((s32)work + 0x40) == 0) {
            anim = anim_data_table[kind];
        } else {
            anim = anim_data_table_2[kind];
        }
        frame = *(s32*)((s32)work + 0x34);
        value = anim[frame / 2];
        *(s32*)((s32)work + 0x34) = frame + 1;
        *(s32*)((s32)work + 0x38) = value;
    }

    if (kind == 6) {
        if (*(s32*)((s32)work + 0x54) == 0) {
            *(f32*)((s32)work + 0x24) += float_22p5_80422940;
            if (*(f32*)((s32)work + 0x24) >= float_0_804228c0) {
                *(f32*)((s32)work + 0x24) = float_0_804228c0;
                *(s32*)((s32)work + 0x54) += 1;
            }
        } else if (*(s32*)((s32)work + 0x54) == 0x78) {
            *(f32*)((s32)work + 0x24) -= float_9_80422944;
            if (*(f32*)((s32)work + 0x24) <= float_neg90_80422948) {
                effDelete(effect);
                return 0;
            }
        } else {
            *(s32*)((s32)work + 0x54) += 1;
        }
    } else if (*(s32*)((s32)work + 0x40) > 0) {
        if (*(s32*)((s32)work + 0x30) < 10000) {
            *(s32*)((s32)work + 0x30) -= 1;
        }
        if (*(s32*)((s32)work + 0x30) < 0) {
            effDelete(effect);
            return 0;
        }
    }

    if (*(void**)((s32)work + 0x44) != 0) {
        calc_pos(work, 1);
    }
    if (kind == 4) {
        dispEntry(*(s32*)((s32)work + 0x4C), 2, effFukidashiDisp, effect, dispCalcZ(&dispPos));
    } else {
        dispEntry(*(s32*)((s32)work + 0x4C), 8, effFukidashiDisp, effect, dispCalcZ(&dispPos));
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

