#include "effect/eff_mahojin.h"
void callback(void* data) {
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetTevOrder(s32 tev, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 tev, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 tev, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 tev, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 tev, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevSwapMode(s32 tev, s32 rasSel, s32 texSel);
    extern u32 dat_80427750;
    extern s32 mahojin_rate;

    u32 colorCopy;
    u32 color = dat_80427750;
    s32 tev = *(s32*)((s32)data + 0xC);
    s32 v10 = *(s32*)((s32)data + 0x10);
    s32 v14 = *(s32*)((s32)data + 0x14);
    s32 v18 = *(s32*)((s32)data + 0x18);

    ((u8*)&color)[3] = mahojin_rate;
    colorCopy = color;
    GXSetTevColor(1, &colorCopy);
    GXSetTevOrder(tev, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(tev, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(tev, 0, 0, 0, 1, 0);
    GXSetTevColorIn(tev, 0, 2, 3, 0xF);
    GXSetTevAlphaIn(tev, 7, 7, 7, 0);
    GXSetTevSwapMode(tev, 0, 0);
    *(s32*)((s32)data + 0xC) = tev + 1;
    *(s32*)((s32)data + 0x14) = v14;
    *(s32*)((s32)data + 0x18) = v18;
    *(s32*)((s32)data + 0x10) = v10;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMahojinEntry(s32 kind, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effMahojinMain(void);
    extern const char str_Mahojin_802fefb4[];

    void* entry = effEntry();
    s32 countKind = kind;
    s32 count;
    void* work;
    s32 i;
    void* ptr;

    countKind = 7;
    if (kind < 8) {
        countKind = kind;
    }
    count = countKind + 2;
    *(const char**)((s32)entry + 0x14) = str_Mahojin_802fefb4;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x30);
    *(void**)((s32)entry + 0xC) = work;
    *(s32*)((s32)entry + 0x10) = (s32)effMahojinMain;
    *(u32*)entry |= 2;

    *(s32*)work = kind;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x10) = 1000;
    *(s32*)((s32)work + 0x14) = 0;
    *(s32*)((s32)work + 0x18) = -1;
    *(s32*)((s32)work + 0x1C) = -1;
    *(s32*)((s32)work + 0x20) = 0xFF;
    *(s32*)((s32)work + 0x24) = 0;
    *(s32*)((s32)work + 0x28) = 0;
    *(s32*)((s32)work + 0x2C) = 0;

    *(s32*)((s32)work + 0x30) = kind;
    *(f32*)((s32)work + 0x34) = x;
    *(f32*)((s32)work + 0x38) = y;
    *(f32*)((s32)work + 0x3C) = z;
    *(s32*)((s32)work + 0x40) = 1000;
    *(s32*)((s32)work + 0x44) = 0;
    *(s32*)((s32)work + 0x48) = -1;
    *(s32*)((s32)work + 0x4C) = -1;
    *(s32*)((s32)work + 0x50) = 0xFF;
    *(s32*)((s32)work + 0x54) = 0;
    *(s32*)((s32)work + 0x58) = 0;
    *(s32*)((s32)work + 0x5C) = 0;

    ptr = (void*)((s32)work + 0x60);
    for (i = 2; i < *(s32*)((s32)entry + 8); i++, ptr = (void*)((s32)ptr + 0x30)) {
        *(f32*)((s32)ptr + 4) = x;
        *(f32*)((s32)ptr + 8) = y;
        *(f32*)((s32)ptr + 0xC) = z;
        *(s32*)((s32)ptr + 0x18) = -1;
        *(s32*)((s32)ptr + 0x1C) = -1;
        *(s32*)((s32)ptr + 0x20) = 0;
        *(s32*)((s32)ptr + 0x24) = 0;
        *(s32*)((s32)ptr + 0x28) = 0;
        *(s32*)((s32)ptr + 0x2C) = 0;
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void effMahojinMain(void* entry) {
    typedef struct Vec_s {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct MahojinWork_s {
        s32 kind;
        f32 x;
        f32 y;
        f32 z;
        s32 timer;
        s32 unk14;
        s32 pose1;
        s32 pose2;
        s32 alpha;
        s32 state;
        s32 unk28;
        s32 unk2c;
    } MahojinWork;

    extern s32 animGroupBaseAsync(void* name, s32 mode, s32 flags);
    extern s32 animPoseEntry(void* name, s32 mode);
    extern void animPoseSetAnim(s32 poseId, void* name, s32 force);
    extern void animPoseRelease(s32 poseId);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern void effDelete(void* entry);
    extern void mapSetTevCallback(s32 idx, void* callback);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 priority);
    extern void callback(void* data);
    extern void effMahojinDisp(void);
    extern void* gp;
    extern char vec3_802fef78[];
    extern void* name_tbl[];
    extern s32 mahojin_rate;
    extern char str_A_1_8042776c[];
    extern char str_A_4_80427770[];
    extern char str_S_1_80427778[];
    extern char str_S_4_8042777c[];
    extern f32 float_0_80427764;
    extern f32 float_1_80427774;
    extern f32 float_0p6_80427780;
    extern f32 float_0p8_80427784;

    MahojinWork* work = *(MahojinWork**)((s32)entry + 0xC);
    MahojinWork* cur;
    Vec zpos;
    Vec pos;
    s32 gpBattle;
    s32 inBattle;
    s32 kind;
    s32 count;
    s32 i;
    s32 state;
    s32 value;
    f32 loopTimes;
    void** row;

    pos = *(Vec*)vec3_802fef78;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    zpos = pos;

    gpBattle = *(s32*)((s32)gp + 0x14);
    kind = work->kind;
    inBattle = ((u32)(-gpBattle) | (u32)gpBattle) >> 31;
    loopTimes = float_0_80427764;

    if (work->pose1 == -1) {
        if (animGroupBaseAsync((void*)(vec3_802fef78 + 0xC), inBattle, 0) == 0) {
            return;
        }
        if (animGroupBaseAsync((void*)(vec3_802fef78 + 0x24), inBattle, 0) == 0) {
            return;
        }
        work->pose1 = animPoseEntry((void*)(vec3_802fef78 + 0xC), inBattle);
        work->pose2 = animPoseEntry((void*)(vec3_802fef78 + 0x24), inBattle);
        animPoseSetAnim(work->pose1, str_A_1_8042776c, 1);
        animPoseSetAnim(work->pose2, str_A_1_8042776c, 1);
        (work + 1)->pose1 = animPoseEntry((void*)(vec3_802fef78 + 0xC), inBattle);
        (work + 1)->pose2 = animPoseEntry((void*)(vec3_802fef78 + 0x24), inBattle);
        animPoseSetAnim((work + 1)->pose1, str_A_4_80427770, 1);
        animPoseSetAnim((work + 1)->pose2, str_A_4_80427770, 1);
        work = *(MahojinWork**)((s32)entry + 0xC);
    }

    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        work->timer = 0x10;
    }

    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 0x10) {
        value = work->timer << 4;
        if (work->alpha > value) {
            work->alpha = value;
        }
    }

    if (work->timer < 0) {
        cur = *(MahojinWork**)((s32)entry + 0xC);
        i = 0;
        while (i < *(s32*)((s32)entry + 8)) {
            if (cur->pose1 != -1) {
                animPoseRelease(cur->pose1);
            }
            if (cur->pose2 != -1) {
                animPoseRelease(cur->pose2);
            }
            i++;
            cur++;
        }
        effDelete(entry);
        return;
    }

    work = *(MahojinWork**)((s32)entry + 0xC);
    state = work->state;
    switch (state) {
        case 0:
            loopTimes = animPoseGetLoopTimes(work->pose1);
            if (loopTimes > float_1_80427774) {
                animPoseSetAnim(work->pose1, str_S_1_80427778, 1);
                animPoseSetAnim(work->pose2, str_S_1_80427778, 1);
                animPoseSetAnim((work + 1)->pose1, str_S_4_8042777c, 1);
                animPoseSetAnim((work + 1)->pose2, str_S_4_8042777c, 1);
                work->state++;
            }
            break;
        case 1:
            if (kind == 7) {
                count = *(s32*)((s32)entry + 8);
                if (animPoseGetLoopTimes(*(s32*)((s32)work + count * 0x30 - 0x18)) > float_1_80427774) {
                    mapSetTevCallback(10, callback);
                    cur = work;
                    i = 0;
                    while (i < *(s32*)((s32)entry + 8)) {
                        animPoseSetMaterialFlagOn(cur->pose1, 0x0A000000);
                        animPoseSetMaterialFlagOn(cur->pose2, 0x0A000000);
                        cur++;
                        i++;
                    }
                    mahojin_rate = 0;
                    work->state++;
                }
            }
            break;
        case 2:
            mahojin_rate += 2;
            if (mahojin_rate > 0xFF) {
                mahojin_rate = 0xFF;
                work->state++;
            }
            break;
    }

    cur = (MahojinWork*)((s32)*(MahojinWork**)((s32)entry + 0xC) + 0x60);
    i = 2;
    while (i < *(s32*)((s32)entry + 8)) {
        state = cur->state;
        if (state == 0) {
            if (i - 1 < kind) {
                if (loopTimes > float_0p6_80427780) {
                    cur->pose1 = animPoseEntry((void*)(vec3_802fef78 + 0xC), inBattle);
                    cur->pose2 = animPoseEntry((void*)(vec3_802fef78 + 0x24), inBattle);
                    row = &name_tbl[(i - 2) * 3];
                    if (kind == 7 || kind == 8) {
                        animPoseSetAnim(cur->pose1, row[1], 1);
                        animPoseSetAnim(cur->pose2, row[1], 1);
                    } else {
                        animPoseSetAnim(cur->pose1, row[2], 1);
                        animPoseSetAnim(cur->pose2, row[2], 1);
                    }
                    cur->alpha = 0;
                    cur->state = 1;
                }
            } else {
                if (loopTimes > float_0p8_80427784) {
                    cur->pose1 = animPoseEntry((void*)(vec3_802fef78 + 0xC), inBattle);
                    cur->pose2 = animPoseEntry((void*)(vec3_802fef78 + 0x24), inBattle);
                    row = &name_tbl[(i - 2) * 3];
                    animPoseSetAnim(cur->pose1, row[0], 1);
                    animPoseSetAnim(cur->pose2, row[0], 1);
                    cur->alpha = 0xFF;
                    cur->state = 10;
                }
            }
        } else if (state == 1) {
            cur->alpha += 10;
            if (cur->alpha > 0xFF) {
                cur->alpha = 0xFF;
                cur->state = 2;
            }
        } else if (state == 10) {
            if (animPoseGetLoopTimes(cur->pose1) > float_1_80427774) {
                row = &name_tbl[(i - 2) * 3];
                animPoseSetAnim(cur->pose1, row[1], 1);
                animPoseSetAnim(cur->pose2, row[1], 1);
                cur->state = 11;
            }
        }
        i++;
        cur++;
    }

    dispEntry(4, 2, effMahojinDisp, entry, dispCalcZ(&zpos));
}



/* CHATGPT STUB FILL: main/effect/eff_mahojin 20260624_184823 */

/* stub-fill: effMahojinDisp | prototype_only | source_prototype */
void effMahojinDisp(s32 cameraId, void* effect) {
    typedef f32 MtxLocal[3][4];
    typedef struct MapEntryLocal {
        u16 dataFlags;
    } MapEntryLocal;
    typedef struct MapWorkLocal {
        MapEntryLocal entries[2];
    } MapWorkLocal;
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern void camGetPtr(s32 cameraId);
    extern MapWorkLocal* mapGetWork(void);
    extern f32 dispCalcZ(VecLocal* pos);
    extern void PSMTXTrans(MtxLocal mtx, f32 x, f32 y, f32 z);
    extern void animPoseMain(s32 poseId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseDrawMtx(s32 poseId, MtxLocal mtx, s32 mode, f32 rot, f32 scale);
    extern VecLocal pos_tbl[];
    extern u32 dat_80427754;
    extern f32 float_neg9999_80427758;
    extern f32 float_neg10000_8042775c;
    extern f32 float_10000_80427760;
    extern f32 float_0_80427764;
    extern f32 float_10_80427768;

    void* base = *(void**)((s32)effect + 0xC);
    s32 rate = *(s32*)((s32)base + 0x20);
    MapWorkLocal* map = mapGetWork();
    u16 oldFlags = map->entries[0].dataFlags;
    f32 zs[16];
    s32 order[16];
    MtxLocal mtx;
    s32 pass;
    s32 i;
    s32 j;

    camGetPtr(cameraId);
    for (pass = 0; pass < 2; pass++) {
        if (pass == 0) {
            zs[0] = float_neg9999_80427758;
            zs[1] = float_neg10000_8042775c;
        } else {
            zs[0] = float_neg10000_8042775c;
            zs[1] = float_10000_80427760;
        }
        order[0] = 0;
        order[1] = 1;
        for (i = 2; i < *(s32*)((s32)effect + 8); i++) {
            zs[i] = dispCalcZ(&pos_tbl[i - 2]);
            order[i] = i;
        }
        for (i = 0; i < *(s32*)((s32)effect + 8) - 1; i++) {
            for (j = i + 1; j < *(s32*)((s32)effect + 8); j++) {
                if (zs[j] < zs[i]) {
                    f32 tmpZ = zs[i];
                    s32 tmpIdx = order[i];
                    zs[i] = zs[j];
                    order[i] = order[j];
                    zs[j] = tmpZ;
                    order[j] = tmpIdx;
                }
            }
        }
        map->entries[0].dataFlags &= 0xFFFE;
        PSMTXTrans(mtx, *(f32*)((s32)base + 4), *(f32*)((s32)base + 8), *(f32*)((s32)base + 0xC));
        for (i = 0; i < *(s32*)((s32)effect + 8); i++) {
            void* work = (void*)((s32)base + order[i] * 0x30);
            s32 poseId = *(s32*)((s32)work + 0x18 + pass * 4);
            if (pass == 1 && *(s32*)((s32)work + 0x28) != 0) {
                ((void (*)(s32))(*(void**)((s32)work + 0x2C)))(cameraId);
            }
            if (poseId != -1) {
                u32 color = dat_80427754;
                u32 colorCopy;
                s32 alpha = *(s32*)((s32)work + 0x20) * rate;
                alpha = alpha / 0xFF + (alpha >> 31);
                animPoseMain(poseId);
                animPoseSetMaterialFlagOn(poseId, 0x40);
                ((u8*)&color)[3] = (u8)(alpha - (alpha >> 31));
                colorCopy = color;
                animPoseSetMaterialEvtColor(poseId, &colorCopy);
                animPoseDrawMtx(poseId, mtx, 3, float_0_80427764, float_10_80427768);
            }
        }
        map->entries[0].dataFlags = oldFlags;
    }
}

