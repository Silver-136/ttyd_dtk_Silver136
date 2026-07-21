#include "effect/eff_majinai.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effMajinaiMain(void* entry);
void* mapGetWork(void);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx m, s8 axis, f32 rad);
void PSMTXConcat(Mtx a, Mtx b, Mtx out);
void* camGetPtr(s32 cameraId);
void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
void animPoseSetMaterialEvtColor(s32 poseId, void* color);
void animPoseMain(s32 poseId);
void animPoseDrawMtx(s32 poseId, Mtx matrix, f32 arg2, s32 pass, f32 arg4);

extern u32 dat_80428b10;
extern u32 dat_80428b1c;
extern f32 float_deg2rad_80428b20;
extern f32 float_0_80428b24;
extern f32 float_10_80428b28;
extern f32 float_1_80428b3c;
extern char str_Majinai_80302fa0[];
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMajinaiEntry(f32 x, f32 y, f32 z, s32 type) {
    s32 typeCopy;
    void* entry;
    void* work;
    void* callback;
    void* mapWork;
    f32 scale;
    s32 alpha;
    s32 zero;
    s32 minusOne;

    typeCopy = type;
    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Majinai_80302fa0;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) << 5));
    callback = effMajinaiMain;
    *(void**)((s32)entry + 0xC) = work;
    scale = float_1_80428b3c;
    *(void**)((s32)entry + 0x10) = callback;
    alpha = 0xFF;
    zero = 0;
    minusOne = -1;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = typeCopy;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    *(s32*)((s32)work + 0x14) = alpha;
    *(s32*)((s32)work + 0x18) = zero;
    *(s32*)((s32)work + 0x1C) = minusOne;
    mapWork = mapGetWork();
    *(u32*)((s32)mapWork + 0x1E) = dat_80428b10;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/* CHATGPT STUB FILL: main/effect/eff_majinai 20260624_185035 */

/* stub-fill: effMajinaiMain | prototype_only | source_prototype */
void effMajinaiMain(void* effect) {
    extern void* gp;
    extern void* _battleWorkPointer;
    extern void* mapGetWork(void);
    extern u32 animGroupBaseAsync(char*, s32, s32);
    extern s32 animPoseEntry(char*, s32);
    extern void animPoseSetAnim(s32, char*, s32);
    extern f64 animPoseGetLoopTimes(s32);
    extern void animPoseSetMaterialFlagOn(s32, u32);
    extern void animPoseSetMaterialFlagOff(s32, u32);
    extern void animPoseRelease(s32);
    extern void* marioGetPtr(void);
    extern void* BattleGetMarioPtr(void*);
    extern s32 BtlUnit_GetBodyPartsId(void*);
    extern void* BtlUnit_GetPartsPtr(void*, s32);
    extern void effStardustEntry(f32, f32, f32, f32, f32, s32, s32, s32);
    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effMajinaiDisp(s32, void*);
    extern char str_MOBJ_EFF_majinai_80302f8c[];
    extern char str_A_1_80428b2c[];
    extern u32 dat_80428b14;
    extern u32 dat_80428b18;

    u8* entry = effect;
    u8* work = *(u8**)(entry + 0xC);
    f32 pos[3];
    s32 inBattle = *(s32*)((u8*)gp + 0x14) != 0;
    s32 state;
    void* mapWork;
    u32 color;
    s32 value;
    s32 i;

    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);
    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        if (*(s32*)(work + 0x1C) != -1) {
            animPoseRelease(*(s32*)(work + 0x1C));
        }
        mapWork = mapGetWork();
        *(u32*)((u8*)mapWork + 0x18) &= ~8;
        effDelete(effect);
        return;
    }

    state = *(s32*)(work + 0x18);
    if (state == 0) {
        if (animGroupBaseAsync(str_MOBJ_EFF_majinai_80302f8c, inBattle, 0) == 0) {
            return;
        }
        *(s32*)(work + 0x1C) = animPoseEntry(str_MOBJ_EFF_majinai_80302f8c, inBattle);
        animPoseSetAnim(*(s32*)(work + 0x1C), str_A_1_80428b2c, 1);
        (*(s32*)(work + 0x18))++;
    } else if (state == 1) {
        mapWork = mapGetWork();
        value = *(u8*)((u8*)mapWork + 0x1E) - 5;
        if (value < 0x40) {
            value = 0x40;
        }
        ((u8*)&color)[0] = (u8)value;
        ((u8*)&color)[1] = (u8)value;
        ((u8*)&color)[2] = (u8)value;
        ((u8*)&color)[3] = (u8)dat_80428b14;
        *(u32*)((u8*)mapWork + 0x1E) = color;
        *(u32*)((u8*)mapWork + 0x18) |= 8;
        if (!inBattle) {
            u8* player = marioGetPtr();
            for (i = 0; i < 5; i++) {
                animPoseSetMaterialFlagOn(*(s32*)(player + i * 4), 0x800);
            }
        } else {
            void* unit = BattleGetMarioPtr(_battleWorkPointer);
            void* part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
            animPoseSetMaterialFlagOn(*(s32*)((u8*)part + 0x70), 0x800);
        }
        if (animPoseGetLoopTimes(*(s32*)(work + 0x1C)) >= 0.93) {
            f32 radius = 45.0f * *(f32*)(work + 0x10);
            effStardustEntry(*(f32*)(work + 4),
                *(f32*)(work + 8) + 55.0f * *(f32*)(work + 0x10),
                *(f32*)(work + 0xC), radius, radius, 0, 0x10, 0x3C);
            (*(s32*)(work + 0x18))++;
        }
    } else if (state == 2) {
        if (animPoseGetLoopTimes(*(s32*)(work + 0x1C)) >= 1.0) {
            (*(s32*)(work + 0x18))++;
        }
    } else if (state == 3) {
        mapWork = mapGetWork();
        value = *(u8*)((u8*)mapWork + 0x1E) + 5;
        if (value > 0xFF) {
            value = 0xFF;
        }
        ((u8*)&color)[0] = (u8)value;
        ((u8*)&color)[1] = (u8)value;
        ((u8*)&color)[2] = (u8)value;
        ((u8*)&color)[3] = (u8)dat_80428b18;
        *(u32*)((u8*)mapWork + 0x1E) = color;
        *(u32*)((u8*)mapWork + 0x18) |= 8;
        if (!inBattle) {
            u8* player = marioGetPtr();
            for (i = 0; i < 5; i++) {
                animPoseSetMaterialFlagOff(*(s32*)(player + i * 4), 0x800);
            }
        } else {
            void* unit = BattleGetMarioPtr(_battleWorkPointer);
            void* part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
            animPoseSetMaterialFlagOff(*(s32*)((u8*)part + 0x70), 0x800);
        }
        *(s32*)(work + 0x14) -= 5;
        if (*(s32*)(work + 0x14) < 0) {
            if (*(s32*)(work + 0x1C) != -1) {
                animPoseRelease(*(s32*)(work + 0x1C));
            }
            mapWork = mapGetWork();
            *(u32*)((u8*)mapWork + 0x18) &= ~8;
            effDelete(effect);
            return;
        }
    }
    dispEntry(4, 2, effMajinaiDisp, effect, dispCalcZ(pos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMajinaiDisp(s32 cameraId, void* entry) {
    u32 color;
    u32 tempColor;
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    void* work = *(void**)((s32)entry + 0xC);
    s32 poseId = *(s32*)((s32)work + 0x1C);
    void* cam;
    f32 scale;
    f32 angle;

    if (poseId != -1) {
        PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        scale = *(f32*)((s32)work + 0x10);
        PSMTXScale(scaleMtx, scale, scale, scale);
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        PSMTXRotRad(rot, 0x79, float_deg2rad_80428b20 * angle);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scaleMtx, trans);

        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x1C), 0x840);
        tempColor = dat_80428b1c;
        ((u8*)&tempColor)[3] = (u8)*(s32*)((s32)work + 0x14);
        color = tempColor;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x1C), &color);
        animPoseMain(*(s32*)((s32)work + 0x1C));
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), trans, float_0_80428b24, 1, float_10_80428b28);
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), trans, float_0_80428b24, 2, float_10_80428b28);
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), trans, float_0_80428b24, 3, float_10_80428b28);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

