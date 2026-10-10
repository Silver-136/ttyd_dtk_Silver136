#include "event/evt_npc.h"

#include "battle/battle_information.h"
#include "driver/seqdrv.h"
#include "driver/animdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "party/party.h"
#include "pmario_sound.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"

void _npc_check_coin_group(void* group, s32* coin);

/* Recovered target zero-Vec3 bases.  Keeping these as byte-addressable
 * initialized objects preserves the existing common-base + offset consumers. */
__declspec(section ".rodata") u8 vec3_802c1470[12] __attribute__((aligned(4))) = {0};
__declspec(section ".rodata") u8 vec3_802c147c[12] __attribute__((aligned(4))) = {0};

extern void* fbatGetPointer(void);
extern f32 sqrtf(f32 x);
extern s32 irand(s32 max);
extern void npcDeleteGroup(void* npc);
extern void fbatChangeMode(s16 mode);
extern char* strcpy(char* dst, const char* src);
extern s32 strcmp(const char* a, const char* b);
extern f64 sin(f64 x);
extern f64 cos(f64 x);
extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
extern void PSVECSubtract(void* a, void* b, void* out);
extern void _kamek_houki_kemuri(s32 npc, s32 mode);
extern f32 reviseAngle(f32 angle);

s32 evt_npc_entry(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 animGroupBaseAsync(s32 name, s32 flag, s32 value);
    extern s32 npcEntry(s32 name, s32 modelName);
    extern void* gp;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    s32 modelName = evtGetValue(pEvt, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    if (animGroupBaseAsync(modelName, flag != 0, 0) == 0) {
        return 0;
    }

    npcEntry(name, modelName);
    return 2;
}

s32 evt_npc_slave_entry(void* pEvt) {
    typedef struct BattleInfoView {
        u8 data[0xC8];
    } BattleInfoView;
    typedef struct NpcSlaveView {
        u32 flags;
        u8 pad_004[4];
        char description[0x20];
        u8 pad_028[0x12C - 0x28];
        void* deadEvent;
        u8 pad_130[0x230 - 0x130];
        BattleInfoView battleInfo;
    } NpcSlaveView;
    extern s32 evtGetValue(void*, s32);
    extern NpcSlaveView* evtNpcNameToPtr(void*, s32);
    extern NpcSlaveView* npcNameToPtr(char*);
    extern void* memset(void*, s32, u32);
    extern s32 sprintf(char*, const char*, ...);
    extern char* strcpy(char*, const char*);
    extern char* strcat(char*, const char*);
    extern s32 npcEntry(char*, s32);
    extern void npcSetSlave(NpcSlaveView*, NpcSlaveView*, s32);
    extern void evtSetValue(void*, s32, s32);
    extern const char str_sPCTd_8041fe08[5];
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name;
    NpcSlaveView* npc;
    s32 id;
    s32 animName;
    s32 index;
    u8 sp2C[0xB8];
    char slaveName[0x20];
    char ident[4];
    NpcSlaveView* slave;
    void* deadEvent;

    name = evtGetValue(pEvt, *args++);
    npc = evtNpcNameToPtr(pEvt, name);
    id = evtGetValue(pEvt, *args++);
    animName = evtGetValue(pEvt, *args++);
    index = *args++;
    deadEvent = (void*)evtGetValue(pEvt, *args++);
    memset(&sp2C, 0, sizeof(sp2C));
    sprintf(ident, str_sPCTd_8041fe08, id);
    strcpy(slaveName, npc->description);
    strcat(slaveName, ident);
    npcEntry(slaveName, animName);
    slave = npcNameToPtr(slaveName);
    slave->flags |= 0x120018;
    slave->deadEvent = deadEvent;
    slave->battleInfo = npc->battleInfo;
    npcSetSlave(npc, slave, id);
    evtSetValue(pEvt, index, (s32)&slave->description);
    return 2;
}

USER_FUNC(evt_npc_delete) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32 value = evtGetValue(event, event->args[0]);
    npcDeleteGroup(evtNpcNameToPtr(event, value));
    return 2;
}

USER_FUNC(evt_npc_check_delete) {
    extern void* evtNpcNameToPtr_NoAssert(EventEntry* event, s32 name);
    s32 name;
    void* npc;
    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr_NoAssert(event, name);
    if (npc != 0) {
        npcDeleteGroup(npc);
    }
    return 2;
}

USER_FUNC(evt_npc_get_ReactionOfLivingBody) {
    extern s32 npcGetReactionOfLivingBody(s32 name);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 dst = args[1];
    s32 reaction = npcGetReactionOfLivingBody(name);
    evtSetValue(evt, dst, reaction);
    return 2;
}

s32 evt_npc_setup(EventEntry* event, s32 isFirstCall) {
    extern void* npcNameToPtr_NoAssert(void* name);
    extern void* npcNameToPtr(void* name);
    extern void* evtEntry(void* code, s32 order, u32 flags);
    extern void npcSetBattleInfo(void* npc, s32 battleInfoId);
    extern char* strcpy(char* dst, char* src);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 force);
    extern s32 evtCheckID(s32 id);
    void* setup;
    void* iter;
    void* tribe;
    void* npc;
    void* evt;

    setup = (void*)evtGetValue(event, event->args[0]);
    fbatGetPointer();

    if (isFirstCall != 0) {
        iter = setup;
        while (*(void**)iter != 0) {
            npc = npcNameToPtr_NoAssert(*(void**)iter);
            if (npc != 0) {
                *(u32*)npc |= *(u32*)((s32)iter + 4);
                *(u32*)((s32)npc + 4) |= *(u32*)((s32)iter + 8);
                tribe = *(void**)((s32)npc + 0x28);
                if (*(void**)((s32)iter + 0xC) != 0) {
                    if (*(s32*)((s32)npc + 0x118) != 0) {
                        evtDeleteID(*(s32*)((s32)npc + 0x118));
                    }
                    evt = evtEntry(*(void**)((s32)iter + 0xC), 0, 0);
                    *(u8*)((s32)evt + 0x10) = 0;
                    *(void**)((s32)evt + 0x170) = npc;
                    *(s32*)((s32)npc + 0x118) = *(s32*)((s32)evt + 0x15C);
                } else {
                    *(s32*)((s32)npc + 0x118) = 0;
                }

                *(void**)((s32)npc + 0x128) = *(void**)((s32)iter + 0x14);
                *(void**)((s32)npc + 0x12C) = *(void**)((s32)iter + 0x18);
                *(void**)((s32)npc + 0x124) = *(void**)((s32)iter + 0x10);
                *(void**)((s32)npc + 0x130) = *(void**)((s32)iter + 0x1C);
                *(void**)((s32)npc + 0x134) = *(void**)((s32)iter + 0x20);
                *(void**)((s32)npc + 0x138) = *(void**)((s32)iter + 0x24);
                *(void**)((s32)npc + 0x13C) = *(void**)((s32)iter + 0x28);
                *(s32*)((s32)npc + 0x1F8) = *(s32*)((s32)iter + 0x2C);

                *(s32*)((s32)npc + 0x1FC) = *(s32*)((s32)iter + 0x30);
                *(s32*)((s32)npc + 0x200) = *(s32*)((s32)iter + 0x34);
                *(s32*)((s32)npc + 0x204) = *(s32*)((s32)iter + 0x38);
                *(s32*)((s32)npc + 0x208) = *(s32*)((s32)iter + 0x3C);
                *(s32*)((s32)npc + 0x20C) = *(s32*)((s32)iter + 0x40);
                *(s32*)((s32)npc + 0x210) = *(s32*)((s32)iter + 0x44);
                *(s32*)((s32)npc + 0x214) = *(s32*)((s32)iter + 0x3C);
                *(s32*)((s32)npc + 0x218) = *(s32*)((s32)iter + 0x40);
                *(s32*)((s32)npc + 0x21C) = *(s32*)((s32)iter + 0x44);
                *(f32*)((s32)npc + 0x220) = *(f32*)((s32)iter + 0x48);
                *(f32*)((s32)npc + 0x224) = *(f32*)((s32)iter + 0x4C);
                *(f32*)((s32)npc + 0x228) = *(f32*)((s32)iter + 0x50);
                *(f32*)((s32)npc + 0x22C) = *(f32*)((s32)iter + 0x54);
                npcSetBattleInfo(npc, *(s32*)((s32)iter + 0x58));

                if (*(void**)((s32)tribe + 0x10) != 0) {
                    strcpy((char*)((s32)npc + 0x4C), *(char**)((s32)tribe + 0x10));
                }
                if (*(void**)((s32)tribe + 0x14) != 0) {
                    strcpy((char*)((s32)npc + 0x6C), *(char**)((s32)tribe + 0x14));
                }
                if (*(void**)((s32)tribe + 8) != 0) {
                    strcpy((char*)((s32)npc + 0x2C), *(char**)((s32)tribe + 8));
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), (char*)((s32)npc + 0x2C), 1);
                }
            }
            iter = (void*)((s32)iter + 0x5C);
        }
        fbatChangeMode(1);
        return 0;
    }

    iter = setup;
    while (*(void**)iter != 0) {
        npc = npcNameToPtr(*(void**)iter);
        if (*(s32*)((s32)npc + 0x118) != 0 && evtCheckID(*(s32*)((s32)npc + 0x118)) != 0) {
            return 0;
        }
        iter = (void*)((s32)iter + 0x5C);
    }

    iter = setup;
    while (*(void**)iter != 0) {
        npc = npcNameToPtr(*(void**)iter);
        if (*(void**)((s32)npc + 0x124) != 0) {
            if (*(s32*)((s32)npc + 0x11C) != 0) {
                evtDeleteID(*(s32*)((s32)npc + 0x11C));
            }
            evt = evtEntry(*(void**)((s32)npc + 0x124), 0, 0x20);
            *(u8*)((s32)evt + 0x10) = 1;
            *(void**)((s32)evt + 0x170) = npc;
            *(s32*)((s32)npc + 0x11C) = *(s32*)((s32)evt + 0x15C);
        }
        iter = (void*)((s32)iter + 0x5C);
    }
    return 2;
}

s32 evt_npc_set_position(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    f32 pos[3];
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    npc = evtNpcNameToPtr(pEvt, name);
    *(s32*)&pos[0] = *(s32*)(vec3_802c1470 + 0);
    *(s32*)&pos[1] = *(s32*)(vec3_802c1470 + 4);
    *(s32*)&pos[2] = *(s32*)(vec3_802c1470 + 8);
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    *(s32*)((s32)npc + 0x98) = *(s32*)&pos[0];
    *(s32*)((s32)npc + 0x9C) = *(s32*)&pos[1];
    *(s32*)((s32)npc + 0xA0) = *(s32*)&pos[2];
    *(s32*)((s32)npc + 0x8C) = *(s32*)((s32)npc + 0x98);
    *(s32*)((s32)npc + 0x90) = *(s32*)((s32)npc + 0x9C);
    *(s32*)((s32)npc + 0x94) = *(s32*)((s32)npc + 0xA0);
    return 2;
}

USER_FUNC(evt_npc_set_width) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    f32 value = evtGetFloat(event, args[1]);
    *(f32*)((s32)evtNpcNameToPtr(event, name) + 0x14C) = value;
    return 2;
}

USER_FUNC(evt_npc_set_height) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    f32 value = evtGetFloat(event, args[1]);
    *(f32*)((s32)evtNpcNameToPtr(event, name) + 0x150) = value;
    return 2;
}


USER_FUNC(evt_npc_get_height) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern f32 evtSetFloat(EventEntry* event, s32 target, f32 value);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 dst = args[1];
    void* npc = evtNpcNameToPtr(event, name);
    evtSetFloat(event, dst, *(f32*)((s32)npc + 0x150));
    return 2;
}

s32 evt_npc_set_scale(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    npc = evtNpcNameToPtr(pEvt, name);
    *(f32*)((s32)npc + 0xE0) = x;
    *(f32*)((s32)npc + 0xE4) = y;
    *(f32*)((s32)npc + 0xE8) = z;
    return 2;
}

s32 evt_npc_get_scale(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name;
    void* npc;
    s32 x;
    s32 y;
    s32 z;

    name = evtGetValue(pEvt, args[0]);
    x = args[1];
    y = args[2];
    z = args[3];
    npc = evtNpcNameToPtr(pEvt, name);
    evtSetFloat(pEvt, x, *(f32*)((s32)npc + 0xE0));
    evtSetFloat(pEvt, y, *(f32*)((s32)npc + 0xE4));
    evtSetFloat(pEvt, z, *(f32*)((s32)npc + 0xE8));
    return 2;
}

s32 evt_npc_get_position(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name;
    void* npc;
    s32 x;
    s32 y;
    s32 z;

    name = evtGetValue(pEvt, args[0]);
    x = args[1];
    y = args[2];
    z = args[3];
    npc = evtNpcNameToPtr(pEvt, name);
    evtSetFloat(pEvt, x, *(f32*)((s32)npc + 0x8C));
    evtSetFloat(pEvt, y, *(f32*)((s32)npc + 0x90));
    evtSetFloat(pEvt, z, *(f32*)((s32)npc + 0x94));
    return 2;
}

s32 evt_npc_get_home_position(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name;
    void* npc;
    s32 x;
    s32 y;
    s32 z;

    name = evtGetValue(pEvt, args[0]);
    x = args[1];
    y = args[2];
    z = args[3];
    npc = evtNpcNameToPtr(pEvt, name);
    evtSetFloat(pEvt, x, *(f32*)((s32)npc + 0x1FC));
    evtSetFloat(pEvt, y, *(f32*)((s32)npc + 0x200));
    evtSetFloat(pEvt, z, *(f32*)((s32)npc + 0x204));
    return 2;
}

s32 evt_npc_set_home_position(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 name;
    s32 x;
    s32 y;
    s32 z;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    x = (s32)evtGetFloat(pEvt, args[1]);
    y = (s32)evtGetFloat(pEvt, args[2]);
    z = (s32)evtGetFloat(pEvt, args[3]);
    npc = evtNpcNameToPtr(pEvt, name);
    *(f32*)((s32)npc + 0x1FC) = x;
    *(f32*)((s32)npc + 0x200) = y;
    *(f32*)((s32)npc + 0x204) = z;
    return 2;
}

s32 evt_npc_get_rotate(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name;
    void* npc;
    s32 x;
    s32 y;
    s32 z;

    name = evtGetValue(pEvt, args[0]);
    x = args[1];
    y = args[2];
    z = args[3];
    npc = evtNpcNameToPtr(pEvt, name);
    evtSetFloat(pEvt, x, *(f32*)((s32)npc + 0xEC));
    evtSetFloat(pEvt, y, *(f32*)((s32)npc + 0xF0));
    evtSetFloat(pEvt, z, *(f32*)((s32)npc + 0xF4));
    return 2;
}

s32 evt_npc_set_rotate(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    f32 rot[3];
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    x = evtGetValue(pEvt, args[1]);
    y = evtGetValue(pEvt, args[2]);
    z = evtGetValue(pEvt, args[3]);
    npc = evtNpcNameToPtr(pEvt, name);
    *(s32*)&rot[0] = *(s32*)(vec3_802c147c + 0);
    *(s32*)&rot[1] = *(s32*)(vec3_802c147c + 4);
    *(s32*)&rot[2] = *(s32*)(vec3_802c147c + 8);
    rot[0] = x;
    rot[1] = y;
    rot[2] = z;
    *(s32*)((s32)npc + 0xEC) = *(s32*)&rot[0];
    *(s32*)((s32)npc + 0xF0) = *(s32*)&rot[1];
    *(s32*)((s32)npc + 0xF4) = *(s32*)&rot[2];
    return 2;
}

s32 evt_npc_add_rotate(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    x = evtGetValue(pEvt, args[1]);
    y = evtGetValue(pEvt, args[2]);
    z = evtGetValue(pEvt, args[3]);
    npc = evtNpcNameToPtr(pEvt, name);
    *(f32*)((s32)npc + 0xEC) += x;
    *(f32*)((s32)npc + 0xF0) += y;
    *(f32*)((s32)npc + 0xF4) += z;
    return 2;
}

s32 evt_npc_set_rotate_offset(void* pEvt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern void* evtNpcNameToPtr(void* evt, s32 name);
    s32* args = *(s32**)((u8*)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    f32 x = evtGetValue(pEvt, args[1]);
    f32 y = evtGetValue(pEvt, args[2]);
    f32 z = evtGetValue(pEvt, args[3]);
    u8* npc = evtNpcNameToPtr(pEvt, name);
    *(Vec*)(npc + 0xF8) = (Vec){ x, y, z };
    return 2;
}

s32 evt_npc_move_position(EventEntry* event, s32 isFirstCall) {
    typedef struct NpcMoveView {
        u32 flags;
        u8 pad_004[0x8C - 4];
        Vec position;
        u8 pad_098[0x104 - 0x98];
        s32 poseId;
        s32 field_108;
        u8 pad_10C[0x158 - 0x10C];
        Vec jumpStartPos;
        Vec jumpTargetPos;
        u8 pad_170[0x188 - 0x170];
        s64 field_188;
        u8 pad_190[0x1A0 - 0x190];
        f32 field_1A0;
        f32 moveDt;
        u8 pad_1A8[0x1AC - 0x1A8];
        f32 moveDir;
        f32 moveSpeed;
        Vec moveOffset;
        f32 moveTime;
        f32 jumpVelY;
        f32 jumpYScale;
        f32 jumpGravity;
        u8 pad_1D0[0x1D4 - 0x1D0];
        u32 jumpFlags;
        s16 footstepToggle;
        u8 pad_1DA[2];
        f32 footstepAccum;
        const char* footstepSfxA;
        const char* footstepSfxB;
        u16 footstepStride;
        u8 pad_1EA[0x2FC - 0x1EA];
        s32 field_2FC;
    } NpcMoveView;
    typedef struct FbatView {
        s16 mode;
        u8 pad_002[6];
        void* target;
    } FbatView;
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern s32 searchPSSFXList(const char* name);
    extern int __float_nan[];
    extern const f32 float_0_8041fd84;
    extern const f32 float_1_8041fd88;
    extern const f32 float_20_8041fd9c;
    extern const f32 float_1000_8041fd94;
    extern const f32 float_0p1_8041fd98;
    extern const f32 float_6p2832_8041fda0;
    extern const f32 float_360_8041fda4;
    extern const f32 float_0p0005_8041fe04;
    s32* args = event->args;
    const u8* constBase = vec3_802c1470;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    f32 targetX = evtGetFloat(event, *args++);
    f32 targetZ = evtGetFloat(event, *args++);
    f32 moveTime = (f32)evtGetValue(event, *args++) / float_1000_8041fd94;
    f32 speed = evtGetFloat(event, *args++);
    s32 moveFlags = args[0];
    NpcMoveView* npc = evtNpcNameToPtr(event, (s32)npcName);
    Vec diff;
    f32 dist;
    f32 distSq;
    f32 step;
    s32 sfxFlag;
    f64 root;
    f64 guess;
    s32 bits;
    s32 cls;
    union { f32 f; s32 i; } bitsView;

    npc->jumpFlags |= 0x10;
    if (moveFlags & 2) {
        npc->jumpFlags |= 0x1000;
    }
    if (isFirstCall) {
        npc->field_188 = 0;
        npc->field_108 = 0;
        npc->jumpTargetPos.x = targetX;
        npc->jumpTargetPos.y = npc->position.y;
        npc->jumpTargetPos.z = targetZ;
        npc->jumpStartPos = npc->position;
        PSVECSubtract(&npc->jumpTargetPos, &npc->jumpStartPos, &diff);
        distSq = diff.x * diff.x + diff.z * diff.z;
        root = (f64)distSq;
        if (root > (f64)float_0_8041fd84) {
            guess = __frsqrte(root);
            guess = *(const f64*)(constBase + 0x48) * guess *
                    (*(const f64*)(constBase + 0x50) - guess * guess * root);
            guess = *(const f64*)(constBase + 0x48) * guess *
                    (*(const f64*)(constBase + 0x50) - guess * guess * root);
            dist = (f32)(root * *(const f64*)(constBase + 0x48) * guess *
                         (*(const f64*)(constBase + 0x50) - guess * guess * root));
        } else if (root < *(const f64*)(constBase + 0x58)) {
            dist = *(f32*)__float_nan;
        } else {
            bitsView.f = distSq;
            bits = bitsView.i & 0x7F800000;
            switch (bits) {
            case 0x7F800000:
                cls = (bitsView.i & 0x7FFFFF) ? 1 : 2;
                break;
            case 0:
                cls = (bitsView.i & 0x7FFFFF) ? 5 : 3;
                break;
            default:
                cls = 4;
                break;
            }
            if (cls == 1) {
                dist = *(f32*)__float_nan;
            } else {
                dist = distSq;
            }
        }
        npc->moveDir = angleABf(float_0_8041fd84, float_0_8041fd84, diff.x, diff.z);
        npc->moveSpeed = speed;
        npc->moveTime = moveTime;
        npc->moveOffset.y = float_0_8041fd84;
        if (npc->moveSpeed <= float_0p1_8041fd98 && npc->moveTime == float_0_8041fd84) {
            npc->moveSpeed = float_1_8041fd88;
        }
        if (npc->moveSpeed <= float_0p1_8041fd98) {
            npc->moveSpeed = dist / npc->moveTime;
            npc->moveTime = float_0_8041fd84;
        }
        npc->footstepAccum = float_0_8041fd84;
        npc->footstepToggle = 1;
        *(s32*)((u8*)event + 0x78) = 0;
    }

    (*(s32*)((u8*)event + 0x78))++;
    if ((moveFlags & 4) && npc->field_2FC && *(s32*)((u8*)event + 0x78) > 5) {
        return 2;
    }
    PSVECSubtract(&npc->jumpTargetPos, &npc->position, &diff);
    distSq = diff.x * diff.x + diff.z * diff.z;
    root = (f64)distSq;
    if (root > (f64)float_0_8041fd84) {
        guess = __frsqrte(root);
        guess = *(const f64*)(constBase + 0x48) * guess *
                (*(const f64*)(constBase + 0x50) - guess * guess * root);
        guess = *(const f64*)(constBase + 0x48) * guess *
                (*(const f64*)(constBase + 0x50) - guess * guess * root);
        dist = (f32)(root * *(const f64*)(constBase + 0x48) * guess *
                     (*(const f64*)(constBase + 0x50) - guess * guess * root));
    } else if (root < *(const f64*)(constBase + 0x58)) {
        dist = *(f32*)__float_nan;
    } else {
        bitsView.f = distSq;
        bits = bitsView.i & 0x7F800000;
        switch (bits) {
        case 0x7F800000:
            cls = (bitsView.i & 0x7FFFFF) ? 1 : 2;
            break;
        case 0:
            cls = (bitsView.i & 0x7FFFFF) ? 5 : 3;
            break;
        default:
            cls = 4;
            break;
        }
        if (cls == 1) {
            dist = *(f32*)__float_nan;
        } else {
            dist = distSq;
        }
    }
    step = (float_20_8041fd9c * npc->moveSpeed) / float_1000_8041fd94;
    if (npc->moveTime != float_0_8041fd84) {
        if (npc->moveTime <= npc->field_1A0) {
            npc->moveSpeed = float_0_8041fd84;
            return 2;
        }
    } else {
        if (npc->moveTime == float_0_8041fd84 && dist <= step) {
            npc->moveSpeed = float_0_8041fd84;
            return 2;
        }
        if (dist < float_0p0005_8041fe04) {
            npc->moveSpeed = float_0_8041fd84;
            return 2;
        }
    }
    if ((npc->flags & 1) && !(npc->jumpFlags & 1)) {
        npc->moveDir = angleABf(float_0_8041fd84, float_0_8041fd84, diff.x, diff.z);
    }
    if ((npc->jumpFlags & 2) && npc->moveSpeed > float_0p1_8041fd98) {
        if (npc != ((FbatView*)fbatGetPointer())->target ||
            (((FbatView*)fbatGetPointer())->mode != 5 && ((FbatView*)fbatGetPointer())->mode != 6)) {
            npcTuningRy(npc, npc->moveDir);
        }
    }
    if (!(npc->jumpFlags & 0x10000)) {
        f32 temp = (f32)sin(npc->moveDir * float_6p2832_8041fda0 / float_360_8041fda4);
        npc->moveOffset.x += npc->moveDt * (npc->moveSpeed * temp);
        temp = (f32)cos(npc->moveDir * float_6p2832_8041fda0 / float_360_8041fda4);
        npc->moveOffset.z += npc->moveDt * (npc->moveSpeed * -temp);
    }
    sfxFlag = 0;
    if (psndGetFlag() & 0x100) {
        if (!(animPoseGetMaterialFlag(npc->poseId) & 0x1800)) {
            sfxFlag = 0x10000;
        }
    }
    if (npc->footstepAccum > (f32)npc->footstepStride / npc->moveSpeed) {
        if (npc->footstepToggle != 0) {
            if (npc->footstepSfxA != 0) {
                psndSFXOnEx_3D(sfxFlag | searchPSSFXList(npc->footstepSfxA), &npc->position, 0xff, 0xff, 0, 8);
            }
        } else if (npc->footstepSfxB != 0) {
            psndSFXOnEx_3D(sfxFlag | searchPSSFXList(npc->footstepSfxB), &npc->position, 0xff, 0xff, 0, 8);
        }
        npc->footstepAccum = float_0_8041fd84;
        npc->footstepToggle = (s8)(1 - npc->footstepToggle);
    }
    if (npc->footstepSfxB != 0 || npc->footstepToggle != 0) {
        npc->footstepAccum += float_1000_8041fd94 * npc->moveDt;
    }
    return 0;
}

s32 evt_npc_jump_position(EventEntry* event, s32 isFirstCall) {
    typedef struct NpcJumpView {
        u32 flags;
        u8 pad_004[0x8C - 4];
        Vec position;
        u8 pad_098[0x104 - 0x98];
        s32 poseId;
        s32 field_108;
        u8 pad_10C[0x158 - 0x10C];
        Vec jumpStartPos;
        Vec jumpTargetPos;
        u8 pad_170[0x178 - 0x170];
        s64 field_178;
        u8 pad_180[0x188 - 0x180];
        s64 field_188;
        u8 pad_190[0x1A4 - 0x190];
        f32 moveDt;
        u8 pad_1A8[0x1AC - 0x1A8];
        f32 moveDir;
        f32 moveSpeed;
        Vec moveOffset;
        f32 moveTime;
        f32 jumpVelY;
        f32 jumpYScale;
        f32 jumpGravity;
        u8 pad_1D0[0x1D4 - 0x1D0];
        u32 jumpFlags;
        u8 pad_1D8[0x1EC - 0x1D8];
        const char* jumpSfx;
        const char* landSfx;
    } NpcJumpView;
    typedef struct FbatView {
        s16 mode;
        u8 pad_002[6];
        void* target;
    } FbatView;
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern s32 searchPSSFXList(const char* name);
    extern f32 __float_nan;
    extern const f32 float_0_8041fd84;
    extern const f32 float_1_8041fd88;
    extern const f32 float_0p5_8041fd90;
    extern const f32 float_1000_8041fd94;
    extern const f32 float_0p1_8041fd98;
    extern const f32 float_6p2832_8041fda0;
    extern const f32 float_360_8041fda4;
    extern const f32 float_neg980_8041fe00;
    const u8* constBase = vec3_802c1470;
    s32* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    f32 targetX = evtGetFloat(event, *args++);
    f32 targetY = evtGetFloat(event, *args++);
    f32 targetZ = evtGetFloat(event, *args++);
    f32 moveTime = (f32)evtGetValue(event, *args++) / float_1000_8041fd94;
    f32 speed = evtGetFloat(event, *args++);
    f32 arcHeight = evtGetFloat(event, *args++);
    s32 mode = evtGetValue(event, *args++);
    NpcJumpView* npc = evtNpcNameToPtr(event, (s32)npcName);
    f32 arcWork = arcHeight;
    Vec diff;
    f32 dist;
    f32 dy;
    f32 t;
    f32 halfT;
    f32 peak;
    s32 sfxFlag;
    f32 temp;
    f64 sqrtD;
    f64 inv;
    u32 bits;
    s32 cls;
    union { f32 f; u32 u; } bitsView;

    npc->jumpFlags |= 0x10;
    npc->jumpFlags |= 0x1000;

    if (isFirstCall) {
        npc->field_188 = 0;
        npc->field_178 = 0;
        npc->field_108 = 0;
        npc->jumpTargetPos.x = targetX;
        npc->jumpTargetPos.y = targetY;
        npc->jumpTargetPos.z = targetZ;
        npc->jumpStartPos = npc->position;

        PSVECSubtract(&npc->jumpTargetPos, &npc->jumpStartPos, &diff);
        dist = diff.x * diff.x + diff.z * diff.z;
        sqrtD = (f64)dist;
        if (sqrtD > (f64)float_0_8041fd84) {
            inv = __frsqrte(sqrtD);
            inv = *(const f64*)(constBase + 0x48) * inv *
                  -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50));
            inv = *(const f64*)(constBase + 0x48) * inv *
                  -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50));
            dist = (f32)(sqrtD * *(const f64*)(constBase + 0x48) * inv *
                         -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50)));
        } else if (sqrtD < *(const f64*)(constBase + 0x58)) {
            dist = __float_nan;
        } else {
            bitsView.f = dist;
            bits = bitsView.u & 0x7F800000;
            if (bits == 0x7F800000) {
                if ((bitsView.u & 0x7FFFFF) == 0) {
                    cls = 2;
                } else {
                    cls = 1;
                }
            } else if ((bits < 0x7F800000) && (bits == 0)) {
                if ((bitsView.u & 0x7FFFFF) == 0) {
                    cls = 3;
                } else {
                    cls = 5;
                }
            } else {
                cls = 4;
            }
            if (cls == 1) {
                dist = __float_nan;
            }
        }
        npc->moveDir = angleABf(float_0_8041fd84, float_0_8041fd84, diff.x, diff.z);
        npc->moveSpeed = speed;
        npc->moveTime = float_0_8041fd84;
        npc->moveOffset.y = float_0_8041fd84;

        sfxFlag = 0;
        if (psndGetFlag() & 0x100 && !(animPoseGetMaterialFlag(npc->poseId) & 0x1800)) {
            sfxFlag = 0x10000;
        }
        if (npc->jumpSfx != 0) {
            psndSFXOnEx_3D(sfxFlag | searchPSSFXList(npc->jumpSfx), &npc->position, 0xff, 0xff, 0, 8);
        }

        npc->jumpVelY = float_0_8041fd84;
        npc->jumpYScale = float_1_8041fd88;
        dy = npc->jumpTargetPos.y - npc->jumpStartPos.y;
        if (arcHeight != float_0_8041fd84) {
            if (dy > float_0_8041fd84) {
                arcWork += dy;
            }
            npc->jumpVelY = float_0_8041fd84;
            switch (mode) {
                case 0:
                    if (moveTime == float_0_8041fd84) {
                        npc->jumpVelY = arcHeight;
                        goto end1;
                    }
                    if (npc->moveSpeed > float_0p1_8041fd98) {
                        t = dist / npc->moveSpeed;
                        if (t == float_0_8041fd84) {
                            t = moveTime;
                        }
                    } else {
                        npc->moveSpeed = dist / moveTime;
                        t = moveTime;
                    }
                    if (t == float_0_8041fd84) {
                        t = float_1_8041fd88;
                    }
                    halfT = float_0p5_8041fd90 * t;
                    npc->jumpVelY = -(float_neg980_8041fe00 * npc->jumpGravity * t * t) / t;
                    peak = (npc->jumpVelY * halfT) + (float_neg980_8041fe00 * npc->jumpGravity * halfT * halfT);
                    if (peak != float_0_8041fd84) {
                        npc->jumpYScale = arcWork / peak;
                    }
                    goto end1;
                case 1:
                    if (moveTime == float_0_8041fd84) {
                        npc->jumpVelY = arcHeight;
                        goto end1;
                    }
                    halfT = float_0p5_8041fd90 * moveTime;
                    npc->jumpVelY = -(float_neg980_8041fe00 * npc->jumpGravity * moveTime * moveTime) / moveTime;
                    peak = (npc->jumpVelY * halfT) + (float_neg980_8041fe00 * npc->jumpGravity * halfT * halfT);
                    if (peak != float_0_8041fd84) {
                        npc->jumpYScale = arcWork / peak;
                    }
                    goto end1;
                default:
                    return 1;
            }
        } else {
            if (dy >= float_0_8041fd84) {
                return 1;
            }
            switch (mode) {
                case 0:
                    if (npc->moveSpeed > float_0p1_8041fd98) {
                        if (moveTime > float_0_8041fd84) {
                            t = moveTime;
                        } else {
                            t = dist / npc->moveSpeed;
                        }
                        if (t > float_0_8041fd84) {
                            npc->jumpYScale = dy / (float_neg980_8041fe00 * npc->jumpGravity * t * t);
                        } else {
                            npc->jumpYScale = float_1_8041fd88;
                        }
                    } else if (moveTime > float_0_8041fd84) {
                        npc->jumpGravity = (dy / (moveTime * moveTime)) / float_neg980_8041fe00;
                    }
                    goto end1;
                default:
                    return 1;
                case 1:
                end1:
                    return 0;
            }
        }
    }

    if ((npc->jumpFlags & 2) && npc->moveSpeed > float_0p1_8041fd98) {
        FbatView* fbat = fbatGetPointer();
        if (npc != fbat->target || (fbat->mode != 5 && fbat->mode != 6)) {
            npcTuningRy(npc, npc->moveDir);
        }
    }
    temp = (f32)sin(npc->moveDir * float_6p2832_8041fda0 / float_360_8041fda4);
    npc->moveOffset.x += npc->moveDt * (npc->moveSpeed * temp);
    temp = (f32)cos(npc->moveDir * float_6p2832_8041fda0 / float_360_8041fda4);
    npc->moveOffset.z += npc->moveDt * (npc->moveSpeed * -temp);
    if (npc->jumpFlags & 1) {
        return 0;
    }

    npc->moveSpeed = float_0_8041fd84;
    npc->jumpVelY = float_0_8041fd84;
    npc->jumpYScale = float_1_8041fd88;
    npc->jumpGravity = float_1_8041fd88;
    sfxFlag = 0;
    if (psndGetFlag() & 0x100) {
        if (!(animPoseGetMaterialFlag(npc->poseId) & 0x1800)) {
            sfxFlag = 0x10000;
        }
    }
    if (npc->landSfx != 0) {
        psndSFXOnEx_3D(sfxFlag | searchPSSFXList(npc->landSfx), &npc->position, 0xff, 0xff, 0, 8);
    }
    return 1;
}

s32 evt_npc_jump_position_nohit(EventEntry* event, s32 isFirstCall) {
    typedef struct NpcJumpView {
        u32 flags;
        u8 pad_004[0x8C - 0x4];
        Vec position;
        u8 pad_098[0x104 - 0x98];
        s32 poseId;
        u8 pad_108[0x158 - 0x108];
        Vec jumpStartPos;
        u8 pad_164[0x1EC - 0x164];
        const char* jumpSfx;
        const char* landSfx;
    } NpcJumpView;
    typedef struct EventJumpView {
        s64 lifetime;
        u8 pad_008[0x18 - 0x8];
        s32* args;
        u8 pad_01C[0x78 - 0x1C];
        f32 userdataf[3];
        u32 field_84;
        u8 pad_088[0x198 - 0x88];
        s64 savedTime;
    } EventJumpView;
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern s32 searchPSSFXList(const char* name);
    extern f32 intplGetValue(s32 mode, s32 currStep, s32 steps, f32 start, f32 end);
    extern f32 float_neg4_8041fdfc;
    s32* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    NpcJumpView* npc = evtNpcNameToPtr(event, (s32)npcName);
    f32 targetX = (f32)evtGetValue(event, *args++);
    f32 targetY = (f32)evtGetValue(event, *args++);
    f32 targetZ = (f32)evtGetValue(event, *args++);
    s32 duration = evtGetValue(event, *args++);
    f32 arcHeight = (f32)evtGetValue(event, *args++);
    s32 sfxFlag;
    u32 ticksPerMs;
    s32 elapsed;
    f32 t;
    Vec endPos;
    EventJumpView* evt = (EventJumpView*)event;

    if (isFirstCall) {
        sfxFlag = 0;
        evt->savedTime = evt->lifetime;
        npc->jumpStartPos = npc->position;
        evt->userdataf[1] = float_neg4_8041fdfc * arcHeight;
        evt->userdataf[2] = (targetY - npc->jumpStartPos.y) - evt->userdataf[1];
        evt->field_84 = npc->flags & 0x80000;
        npc->flags &= ~0x80000;

        if (psndGetFlag() & 0x100) {
            if (!(animPoseGetMaterialFlag(npc->poseId) & 0x1800)) {
                sfxFlag = 0x10000;
            }
        }
        if (npc->jumpSfx != 0) {
            psndSFXOnEx_3D(sfxFlag | searchPSSFXList(npc->jumpSfx), &npc->position, 0xff, 0xff, 0, 8);
        }
    }

    ticksPerMs = (*(u32*)0x800000F8 >> 2) / 1000;
    elapsed = (s32)((u32)evt->lifetime - (u32)evt->savedTime) / ticksPerMs;

    if (elapsed < duration) {
        t = (f32)elapsed / (f32)duration;
        npc->position.x = intplGetValue(0, elapsed, duration, npc->jumpStartPos.x, targetX);
        npc->position.z = intplGetValue(0, elapsed, duration, npc->jumpStartPos.z, targetZ);
        npc->position.y = npc->jumpStartPos.y + t * ((evt->userdataf[1] * t) + evt->userdataf[2]);
        return 0;
    }

    npc->position = (Vec){ targetX, targetY, targetZ };
    npc->flags |= evt->field_84;

    sfxFlag = 0;
    if (psndGetFlag() & 0x100) {
        if (!(animPoseGetMaterialFlag(npc->poseId) & 0x1800)) {
            sfxFlag = 0x10000;
        }
    }
    if (npc->landSfx != 0) {
        psndSFXOnEx_3D(sfxFlag | searchPSSFXList(npc->landSfx), &npc->position, 0xff, 0xff, 0, 8);
    }
    return 2;
}

s32 evt_npc_glide_position(EventEntry* event, s32 isFirstCall) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern f32 _intplGetFloat(f32 value, s32 kind);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern void psndSFXOnEx_3D(s32 id, void* pos, s32 a, s32 b, s32 c, s32 d);
    extern f32 __float_nan;
    extern const f32 float_0_8041fd84;
    extern const f32 float_0p1_8041fd98;
    extern const f32 float_1000_8041fd94;
    extern const f32 float_3p1416_8041fda8;
    s32* args;
    void* npc;
    void* fbat;
    s32 name;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    f32 durationMs;
    f32 speed;
    f32 arc;
    s32 mode;
    s32 flags;
    f32 delta[3];
    f32 dist;
    f32 denom;
    f32 now;
    f32 old;
    f32 dt;
    f32 dx;
    f32 dz;
    s32 soundMask;
    s32 sfx;
    s32 finished;
    const u8* constBase;
    f64 sqrtD;
    f64 inv;
    s32 bits;
    s32 cls;
    union { f32 f; u32 u; } bitsView;

    constBase = vec3_802c1470;
    args = event->args;
    name = evtGetValue(event, args[0]);
    targetX = evtGetFloat(event, args[1]);
    targetY = evtGetFloat(event, args[2]);
    targetZ = evtGetFloat(event, args[3]);
    durationMs = (f32)evtGetValue(event, args[4]) / float_1000_8041fd94;
    speed = evtGetFloat(event, args[5]);
    arc = evtGetFloat(event, args[6]);
    mode = evtGetValue(event, args[7]);
    flags = args[8];
    npc = evtNpcNameToPtr(event, name);

    *(u32*)((s32)npc + 0x1D4) |= 0x10;
    if ((flags & 2) == 0) {
        *(u32*)((s32)npc + 0x1D4) |= 0x1000;
    }
    if (isFirstCall != 0) {
        *(s32*)((s32)npc + 0x188) = 0;
        *(s32*)((s32)npc + 0x18C) = 0;
        *(s32*)((s32)npc + 0x108) = 0;
        *(f32*)((s32)npc + 0x164) = targetX;
        *(f32*)((s32)npc + 0x168) = targetY;
        *(f32*)((s32)npc + 0x16C) = targetZ;
        *(s32*)((s32)npc + 0x158) = *(s32*)((s32)npc + 0x8C);
        *(s32*)((s32)npc + 0x15C) = *(s32*)((s32)npc + 0x90);
        *(s32*)((s32)npc + 0x160) = *(s32*)((s32)npc + 0x94);
        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), delta);
        dist = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
        sqrtD = (f64)dist;
        if (sqrtD > (f64)float_0_8041fd84) {
            inv = __frsqrte(sqrtD);
            inv = *(const f64*)(constBase + 0x48) * inv *
                  -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50));
            inv = *(const f64*)(constBase + 0x48) * inv *
                  -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50));
            dist = (f32)(sqrtD * *(const f64*)(constBase + 0x48) * inv *
                         -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50)));
        } else {
            if (*(const f64*)(constBase + 0x58) <= sqrtD) {
                bitsView.f = dist;
                bits = bitsView.u & 0x7F800000;
                switch (bits) {
                case 0x7F800000:
                    cls = (bitsView.u & 0x7FFFFF) ? 1 : 2;
                    break;
                case 0:
                    cls = (bitsView.u & 0x7FFFFF) ? 5 : 3;
                    break;
                default:
                    cls = 4;
                    break;
                }
                if (cls == 1) {
                    dist = __float_nan;
                }
            } else {
                dist = __float_nan;
            }
        }
        if (durationMs <= float_0_8041fd84) {
            if (speed == float_0_8041fd84) {
                return 1;
            }
            if (dist == float_0_8041fd84) {
                return 1;
            }
            durationMs = dist / speed;
        }
        *(f32*)((s32)npc + 0x1C0) = durationMs;
        *(f32*)((s32)npc + 0x1B0) = speed;
        if (*(f32*)((s32)npc + 0x1B0) <= float_0p1_8041fd98) {
            *(f32*)((s32)npc + 0x1B0) = dist / *(f32*)((s32)npc + 0x1C0);
        }
        *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
        *(s16*)((s32)npc + 0x1D8) = 1;
        return 0;
    }
    if ((flags & 1) != 0 && *(s32*)((s32)npc + 0x2FC) != 0) {
        return 2;
    }
    now = *(f32*)((s32)npc + 0x1A0);
    denom = *(f32*)((s32)npc + 0x1C0);
    speed = *(f32*)((s32)npc + 0x1A4);
    finished = 0;
    if (denom <= now) {
        finished = 1;
        if (denom < now) {
            speed -= denom - now;
            if (speed <= float_0_8041fd84) {
                return 2;
            }
        }
    }
    old = (now - speed) / denom;
    now = _intplGetFloat(now / denom, mode);
    old = _intplGetFloat(old, mode);
    dt = now - old;
    dx = *(f32*)((s32)npc + 0x164) - *(f32*)((s32)npc + 0x158);
    dz = *(f32*)((s32)npc + 0x16C) - *(f32*)((s32)npc + 0x160);
    *(f32*)((s32)npc + 0x1B4) += dx * dt;
    *(f32*)((s32)npc + 0x1BC) += dz * dt;
    if ((*(u32*)((s32)npc + 0x1D4) & 2) != 0 && (dx != float_0_8041fd84 || dz != float_0_8041fd84)) {
        if (npc != *(void**)((s32)fbatGetPointer() + 8) ||
            (*(s16*)fbatGetPointer() != 5 && *(s16*)fbatGetPointer() != 6)) {
            npcTuningRy(npc, angleABf(float_0_8041fd84, float_0_8041fd84, dx, dx));
        }
    }
    *(f32*)((s32)npc + 0x1B8) += (*(f32*)((s32)npc + 0x168) - *(f32*)((s32)npc + 0x15C)) * dt;
    *(f32*)((s32)npc + 0x1B8) += arc * (f32)sin((double)(float_3p1416_8041fda8 * now)) - arc * (f32)sin((double)(float_3p1416_8041fda8 * old));
    soundMask = 0;
    if ((psndGetFlag() & 0x100) != 0 && (animPoseGetMaterialFlag(*(s32*)((s32)npc + 0x104)) & 0x1800) == 0) {
        soundMask = 0x10000;
    }
    if ((f32)((f32)*(u16*)((s32)npc + 0x1E8) / *(f32*)((s32)npc + 0x1B0)) < *(f32*)((s32)npc + 0x1DC)) {
        if (*(s16*)((s32)npc + 0x1D8) == 0) {
            if (*(s32*)((s32)npc + 0x1E4) != 0) {
                sfx = searchPSSFXList(*(s32*)((s32)npc + 0x1E4));
                psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
            }
        } else if (*(s32*)((s32)npc + 0x1E0) != 0) {
            sfx = searchPSSFXList(*(s32*)((s32)npc + 0x1E0));
            psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
        }
        *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
        *(s16*)((s32)npc + 0x1D8) = (s16)(1 - *(s16*)((s32)npc + 0x1D8));
    }
    if (*(s32*)((s32)npc + 0x1E4) != 0 || *(s16*)((s32)npc + 0x1D8) != 0) {
        *(f32*)((s32)npc + 0x1DC) += float_1000_8041fd94 * *(f32*)((s32)npc + 0x1A4);
    }
    if (finished != 0) {
        return 1;
    }
    return 0;
}

s32 evt_npc_return_interrupt(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    *(u32*)((s32)npc + 0x140) |= 0x10000000;
    return 1;
}

s32 evt_npc_homing_target(EventEntry* event, s32 isFirstCall) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern f32 compAngle(f32 a, f32 b);
    extern f32 reviseAngle(f32 a);
    extern f32 __float_nan;
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern void psndSFXOnEx_3D(s32 id, void* pos, s32 a, s32 b, s32 c, s32 d);
    extern const f32 float_0_8041fd84;
    extern const f32 float_1000_8041fd94;
    extern const f32 float_60_8041fdf8;
    extern const f32 float_6p2832_8041fda0;
    extern const f32 float_360_8041fda4;
    extern char str_mario_8041fdd4[];

    s32* args;
    s32 npcName;
    s32 targetName;
    f32 durationSec;
    f32 speedParam;
    f32 stopDist;
    f32 turnScale;
    u32 flags;
    void* npc;
    void* target;
    void* fbat;
    f32 delta[3];
    f64 dist;
    f32 dx;
    f32 dz;
    f32 targetAngle;
    f32 angleDiff;
    f32 turnAmount;
    s32 soundMask;
    s32 shouldStop;
    s32 sfx;
    s32 userBase;
    const u8* constBase;
    f64 sqrtD;
    f64 inv;
    u32 bits;
    s32 cls;
    union { f32 f; u32 u; } bitsView;

    constBase = vec3_802c1470;
    args = event->args;
    npcName = evtGetValue(event, args[0]);
    targetName = evtGetValue(event, args[1]);
    durationSec = (f32)evtGetValue(event, args[2]) / float_1000_8041fd94;
    speedParam = evtGetFloat(event, args[3]);
    stopDist = evtGetFloat(event, args[4]);
    turnScale = evtGetFloat(event, args[5]);
    flags = args[6];
    npc = evtNpcNameToPtr(event, npcName);
    *(u32*)((s32)npc + 0x1D4) |= 0x10;
    userBase = (s32)event + 0x78;

    if (isFirstCall != 0) {
        *(s32*)((s32)npc + 0x18C) = 0;
        *(s32*)((s32)npc + 0x188) = 0;
        *(s32*)((s32)npc + 0x108) = 0;

        if (strcmp((const char*)targetName, str_mario_8041fdd4) == 0) {
            *(s32*)userBase = 0;
            target = marioGetPtr();
            *(void**)(userBase + 4) = target;
            target = *(void**)(userBase + 4);
            dx = *(f32*)((s32)target + 0x8C) - *(f32*)((s32)npc + 0x8C);
            dz = *(f32*)((s32)target + 0x94) - *(f32*)((s32)npc + 0x94);
        } else {
            *(s32*)userBase = 1;
            target = evtNpcNameToPtr(event, targetName);
            *(void**)(userBase + 4) = target;
            target = *(void**)(userBase + 4);
            dx = *(f32*)((s32)target + 0x8C) - *(f32*)((s32)npc + 0x8C);
            dz = *(f32*)((s32)target + 0x94) - *(f32*)((s32)npc + 0x94);
        }

        targetAngle = angleABf(float_0_8041fd84, float_0_8041fd84, dx, dz);
        *(f32*)((s32)npc + 0x1AC) = targetAngle;
        *(f32*)((s32)npc + 0x1B0) = speedParam;
        if (speedParam == float_0_8041fd84) {
            return 2;
        }
        *(f32*)((s32)npc + 0x1C0) = durationSec;

        if (((*(u32*)((s32)npc + 0x1D4) & 2) != 0) && (*(f32*)((s32)npc + 0x1B0) != float_0_8041fd84)) {
            if ((npc != *(void**)((s32)fbatGetPointer() + 8)) ||
                ((*(s16*)fbatGetPointer() != 5) && (*(s16*)fbatGetPointer() != 6))) {
                npcTuningRy(npc, *(f32*)((s32)npc + 0x1AC));
            }
        }

        *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
        *(s16*)((s32)npc + 0x1D8) = 1;
        return 0;
    }

    if ((*(u32*)((s32)npc + 0x1D4) & 1) == 0) {
        if (*(s32*)userBase == 0) {
            target = *(void**)(userBase + 4);
            *(f32*)((s32)npc + 0x164) = *(f32*)((s32)target + 0x8C);
            *(f32*)((s32)npc + 0x168) = *(f32*)((s32)target + 0x90);
            *(f32*)((s32)npc + 0x16C) = *(f32*)((s32)target + 0x94);
        } else {
            target = *(void**)(userBase + 4);
            *(f32*)((s32)npc + 0x164) = *(f32*)((s32)target + 0x8C);
            *(f32*)((s32)npc + 0x168) = *(f32*)((s32)target + 0x90);
            *(f32*)((s32)npc + 0x16C) = *(f32*)((s32)target + 0x94);
        }

        *(s32*)((s32)npc + 0x158) = *(s32*)((s32)npc + 0x8C);
        *(s32*)((s32)npc + 0x15C) = *(s32*)((s32)npc + 0x90);
        *(s32*)((s32)npc + 0x160) = *(s32*)((s32)npc + 0x94);

        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), delta);
        dist = delta[0] * delta[0] + delta[2] * delta[2];
        sqrtD = (f64)dist;
        if (sqrtD > (f64)float_0_8041fd84) {
            inv = __frsqrte(sqrtD);
            inv = *(const f64*)(constBase + 0x48) * inv *
                  -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50));
            inv = *(const f64*)(constBase + 0x48) * inv *
                  -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50));
            dist = (f32)(sqrtD * *(const f64*)(constBase + 0x48) * inv *
                         -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50)));
        } else {
            if (*(const f64*)(constBase + 0x58) <= sqrtD) {
                bitsView.f = dist;
                bits = bitsView.u & 0x7F800000;
                switch (bits) {
                case 0x7F800000:
                    cls = (bitsView.u & 0x7FFFFF) ? 1 : 2;
                    break;
                case 0:
                    cls = (bitsView.u & 0x7FFFFF) ? 5 : 3;
                    break;
                default:
                    cls = 4;
                    break;
                }
                if (cls == 1) {
                    dist = __float_nan;
                }
            } else {
                dist = __float_nan;
            }
        }
        targetAngle = angleABf(float_0_8041fd84, float_0_8041fd84, delta[0], delta[2]);
        angleDiff = compAngle(*(f32*)((s32)npc + 0x1AC), targetAngle);
        turnAmount = float_60_8041fdf8 * turnScale * *(f32*)((s32)npc + 0x1A4);
        if (turnAmount < __fabsf(angleDiff)) {
            if (angleDiff <= float_0_8041fd84) {
                *(f32*)((s32)npc + 0x1AC) -= turnAmount;
            } else {
                *(f32*)((s32)npc + 0x1AC) += turnAmount;
            }
            *(f32*)((s32)npc + 0x1AC) = reviseAngle(*(f32*)((s32)npc + 0x1AC));
        } else {
            *(f32*)((s32)npc + 0x1AC) = targetAngle;
        }

        if (((*(u32*)((s32)npc + 0x1D4) & 2) != 0) && (*(f32*)((s32)npc + 0x1B0) != float_0_8041fd84)) {
            if ((npc != *(void**)((s32)fbatGetPointer() + 8)) ||
                ((*(s16*)fbatGetPointer() != 5) && (*(s16*)fbatGetPointer() != 6))) {
                npcTuningRy(npc, *(f32*)((s32)npc + 0x1AC));
            }
        }
    } else {
        dist = float_1000_8041fd94;
    }

    PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x8C), delta);
    dx = delta[0];
    dz = delta[2];

    shouldStop = 0;
    if (*(f32*)((s32)npc + 0x1C0) != float_0_8041fd84 &&
        *(f32*)((s32)npc + 0x1C0) <= *(f32*)((s32)npc + 0x1A0)) {
        shouldStop = 1;
    } else if (stopDist > float_0_8041fd84 && dist < stopDist) {
        shouldStop = 1;
    } else if ((flags & 4) != 0 && *(u32*)((s32)npc + 0x2FC) != 0) {
        shouldStop = 1;
    }
    if (shouldStop) {
        *(f32*)((s32)npc + 0x1B0) = float_0_8041fd84;
        return 1;
    }

    *(f32*)((s32)npc + 0x1B4) += *(f32*)((s32)npc + 0x1A4) * (*(f32*)((s32)npc + 0x1B0) * (f32)sin((f64)((float_6p2832_8041fda0 * *(f32*)((s32)npc + 0x1AC)) / float_360_8041fda4)));
    *(f32*)((s32)npc + 0x1BC) += *(f32*)((s32)npc + 0x1A4) * (*(f32*)((s32)npc + 0x1B0) * -(f32)cos((f64)((float_6p2832_8041fda0 * *(f32*)((s32)npc + 0x1AC)) / float_360_8041fda4)));

    soundMask = 0;
    if (((psndGetFlag() & 0x100) != 0) && ((animPoseGetMaterialFlag(*(s32*)((s32)npc + 0x104)) & 0x1800) == 0)) {
        soundMask = 0x10000;
    }
    if ((f32)((f32)*(u16*)((s32)npc + 0x1E8) / *(f32*)((s32)npc + 0x1B0)) < *(f32*)((s32)npc + 0x1DC)) {
        if (*(s16*)((s32)npc + 0x1D8) == 0) {
            if (*(s32*)((s32)npc + 0x1E4) != 0) {
                sfx = searchPSSFXList(*(s32*)((s32)npc + 0x1E4));
                psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
            }
        } else if (*(s32*)((s32)npc + 0x1E0) != 0) {
            sfx = searchPSSFXList(*(s32*)((s32)npc + 0x1E0));
            psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
        }
        *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
        *(s16*)((s32)npc + 0x1D8) = (s8)(1 - *(s16*)((s32)npc + 0x1D8));
    }
    if ((*(s32*)((s32)npc + 0x1E4) != 0) || (*(s16*)((s32)npc + 0x1D8) != 0)) {
        *(f32*)((s32)npc + 0x1DC) += float_1000_8041fd94 * *(f32*)((s32)npc + 0x1A4);
    }

    return 0;
}

f32 _intplGetFloat(f32 value, s32 kind) {
    extern f32 float_0_8041fd84;
    extern volatile f32 float_1_8041fd88;
    extern f32 float_0p5_8041fd90;
    extern f32 float_3p1416_8041fda8;
    extern f32 float_4_8041fde8;
    extern f32 float_15_8041fdec;
    extern f32 float_40_8041fdf0;
    extern f32 float_1p5708_8041fdf4;
    f32 t;
    f32 inv;
    f32 temp;
    f32 one;

    t = value;
    if (t > float_1_8041fd88) {
        t = float_1_8041fd88;
    }

    switch (kind) {
        case 0:
            return t;
        case 1:
            return t * t;
        case 2:
            return t * (t * t);
        case 3:
            return t * (t * (t * t));
        case 7:
            temp = (f32)cos(float_4_8041fde8 * (t / float_3p1416_8041fda8));
            one = float_1_8041fd88;
            inv = one - t;
            temp = inv * temp;
            temp = inv * temp;
            return one - temp;
        case 8:
            temp = (f32)cos((float_4_8041fde8 * ((t * t) / float_3p1416_8041fda8)) / float_15_8041fdec);
            one = float_1_8041fd88;
            inv = one - t;
            temp = inv * temp;
            temp = inv * temp;
            return one - temp;
        case 4:
            one = float_1_8041fd88;
            inv = one - t;
            return one - inv * inv;
        case 5:
            one = float_1_8041fd88;
            inv = one - t;
            temp = inv * inv;
            return one - inv * temp;
        case 6:
            one = float_1_8041fd88;
            inv = one - t;
            temp = inv * inv;
            temp = inv * temp;
            return one - inv * temp;
        case 10:
            temp = (f32)cos((float_4_8041fde8 * ((t * t) / float_3p1416_8041fda8)) / float_40_8041fdf0);
            one = float_1_8041fd88;
            inv = one - t;
            temp = inv * temp;
            temp = inv * temp;
            if (temp < float_0_8041fd84) {
                temp = -temp;
            }
            return float_1_8041fd88 - temp;
        case 11:
            temp = (f32)cos(float_3p1416_8041fda8 * t);
            return (float_1_8041fd88 - temp) * float_0p5_8041fd90;
        case 12:
            return (f32)sin(float_1p5708_8041fdf4 * t);
        case 13:
            temp = (f32)cos(float_1p5708_8041fdf4 * t);
            return float_1_8041fd88 - temp;
        default:
            return float_0_8041fd84;
    }
}

USER_FUNC(evt_npc_set_ry) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    f32 value = evtGetFloat(event, args[1]);
    *(f32*)((s32)evtNpcNameToPtr(event, name) + 0x144) = value;
    return 2;
}

s32 evt_npc_set_ry_lr(void* pEvt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 evtGetFloat(void* evt, s32 value);
    extern void* evtNpcNameToPtr(void* evt, s32 name);
    extern void* camGetPtr(s32 camera);
    extern f32 float_100_8041fde4;
    extern f32 float_5_8041fde0;
    s32* args = *(s32**)((u8*)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    s32 side = (s32)evtGetFloat(pEvt, args[1]);
    u8* npc = evtNpcNameToPtr(pEvt, name);
    u8* camera = camGetPtr(4);
    s32 angle = (s32)reviseAngle((float_100_8041fde4 * *(f32*)(camera + 0x114) +
                                   float_5_8041fde0) / float_100_8041fde4);
    f32 left = (angle - 90) % 360;
    f32 right = (angle + 90) % 360;

    if (side == 0) {
        *(f32*)(npc + 0x144) = left;
    } else {
        *(f32*)(npc + 0x144) = right;
    }
    return 2;
}

s32 evt_npc_reverse_ry(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern f32 float_180_8041fdb0;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    *(f32*)((s32)npc + 0x144) = reviseAngle(float_180_8041fdb0 + *(f32*)((s32)npc + 0x144));
    return 2;
}

USER_FUNC(evt_npc_get_dir) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[1], (s32)*(f32*)((s32)npc + 0x144));
    return 2;
}

s32 evt_npc_set_anim(void* evt, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void animPoseSetAnim(s32 poseId, void* name, s32 force);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    s32 anim = evtGetValue(evt, args[1]);
    void* npc = evtNpcNameToPtr(evt, name);

    if (*(u8*)((s32)evt + 0x10) == 1) {
        strcpy((char*)((s32)npc + 0x2C), (char*)anim);
    }
    animPoseSetAnim(*(s32*)((s32)npc + 0x104), (void*)anim, 1);
    return 2;
}

s32 evt_npc_set_damage_anim(EventEntry* event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x20);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_confuse_anim(EventEntry* event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x24);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_stop_anim(EventEntry* event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0xC);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_stay_anim(EventEntry* event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x10);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_talk_anim(EventEntry* event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x14);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_walk_anim(EventEntry* event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x18);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_run_anim(EventEntry* event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x1C);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

USER_FUNC(evt_npc_set_paper) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetPaperAnimGroup(void* pose, s32 anim, s32 enabled);
    extern void animPosePaperPeraOn(void* pose);

    s32* args;
    s32 name;
    s32 anim;
    void* npc;

    args = event->args;

    name = evtGetValue(event, args[0]);
    anim = evtGetValue(event, args[1]);

    npc = evtNpcNameToPtr(event, name);

    animPoseSetPaperAnimGroup(*(void**)((s32)npc + 0x104), anim, 1);
    animPosePaperPeraOn(*(void**)((s32)npc + 0x104));

    return 2;
}

USER_FUNC(evt_npc_set_paper_anim) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetPaperAnim(void* pose, s32 anim, s32 enabled);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 anim = evtGetValue(event, args[1]);
    animPoseSetPaperAnim(*(void**)((s32)evtNpcNameToPtr(event, name) + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_clear_paper(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void animPoseSetPaperAnimGroup(s32 poseId, s32 anim, s32 enabled);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    animPoseSetPaperAnimGroup(*(s32*)((s32)npc + 0x104), 0, 1);
    return 2;
}

s32 evt_npc_set_force_regl_anim(EventEntry* event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32* args;
    s32 name;
    const char* anim;
    void* npc;

    args = event->args;
    name = evtGetValue(event, args[0]);
    anim = (const char*)evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    strcpy((char*)((s32)npc + 0x2C), anim);
    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

u32 evt_npc_wait_pera(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern u32 animPoseGetPeraEnd(s32 poseId);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    return animPoseGetPeraEnd(*(s32*)((s32)npc + 0x104)) != 0 ? 2 : 0;
}

s32 evt_npc_wait_anim(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern f32 animPoseGetLoopTimes(s32);
    s32* args;
    s32 name;
    f32 wait;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    wait = evtGetFloat(pEvt, args[1]);
    npc = evtNpcNameToPtr(pEvt, name);
    if (animPoseGetLoopTimes(*(s32*)((s32)npc + 0x104)) >= wait) {
        return 2;
    }
    return 0;
}

s32 evt_npc_flag_onoff(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 onoff = evtGetValue(pEvt, args[0]);
    s32 name = evtGetValue(pEvt, args[1]);
    u32 flag = evtGetValue(pEvt, args[2]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    switch (onoff) {
        case 0:
            *(u32*)npc &= ~flag;
            break;
        case 1:
            *(u32*)npc |= flag;
            break;
    }
    return 2;
}

USER_FUNC(evt_npc_flag_check) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[2], *(s32*)npc & flag);
    return 2;
}

s32 evt_npc_reaction_flag_onoff(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 onoff;
    s32 name;
    u32 flag;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    onoff = evtGetValue(pEvt, args[0]);
    name = evtGetValue(pEvt, args[1]);
    flag = evtGetValue(pEvt, args[2]);
    npc = evtNpcNameToPtr(pEvt, name);
    switch (onoff) {
        case 0:
            *(u32*)((s32)npc + 4) &= ~flag;
            break;
        case 1:
            *(u32*)((s32)npc + 4) |= flag;
            break;
    }
    return 2;
}

s32 evt_npc_status_onoff(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 onoff = evtGetValue(pEvt, args[0]);
    s32 name = evtGetValue(pEvt, args[1]);
    u32 flag = evtGetValue(pEvt, args[2]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    switch (onoff) {
        case 0:
            *(u32*)((s32)npc + 0x1D4) &= ~flag;
            break;
        case 1:
            *(u32*)((s32)npc + 0x1D4) |= flag;
            break;
    }
    return 2;
}

USER_FUNC(evt_npc_status_check) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[2], *(s32*)((s32)npc + 0x1D4) & flag);
    return 2;
}

s32 evt_npc_pera_onoff(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void animPosePeraOff(s32 poseId);
    extern void animPosePeraOn(s32 poseId);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    s32 onoff = evtGetValue(pEvt, args[1]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    switch (onoff) {
        case 0:
            animPosePeraOff(*(s32*)((s32)npc + 0x104));
            break;
        case 1:
            animPosePeraOn(*(s32*)((s32)npc + 0x104));
            break;
    }
    return 2;
}

USER_FUNC(evt_npc_set_camid) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    *(s32*)((s32)evtNpcNameToPtr(event, name) + 0x1F4) = value;
    return 2;
}

f32 _get_target_dir(void* pEvt, void* npc, s32 target) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void* evtNpcNameToPtr(void*, s32);
    extern char str_mario_8041fdd4;
    extern f32 float_0_8041fd84;
    Vec3 targetPos;
    Vec3 npcPos;
    Vec3 diff;
    void* ptr;

    if (strcmp((char*)target, &str_mario_8041fdd4) == 0) {
        ptr = marioGetPtr();
        targetPos.x = *(f32*)((s32)ptr + 0x8C);
        targetPos.y = *(f32*)((s32)ptr + 0x90);
        targetPos.z = *(f32*)((s32)ptr + 0x94);
    } else {
        ptr = evtNpcNameToPtr(pEvt, target);
        targetPos.x = *(f32*)((s32)ptr + 0x8C);
        targetPos.y = *(f32*)((s32)ptr + 0x90);
        targetPos.z = *(f32*)((s32)ptr + 0x94);
    }
    npcPos = *(Vec3*)((s32)npc + 0x8C);
    PSVECSubtract(&targetPos, &npcPos, &diff);
    return angleABf(float_0_8041fd84, float_0_8041fd84, diff.x, diff.z);
}

USER_FUNC(evt_get_target_dir) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern f32 _get_target_dir(EventEntry* event, void* npc, s32 target);
    extern void evtSetFloat(EventEntry* event, s32 index, f32 value);

    s32* args;
    s32 name;
    s32 target;
    s32 dst;
    void* npc;

    args = event->args;

    name = evtGetValue(event, args[0]);
    target = evtGetValue(event, args[1]);
    dst = args[2];

    npc = evtNpcNameToPtr(event, name);

    evtSetFloat(event, dst, _get_target_dir(event, npc, target));
    return 2;
}

USER_FUNC(evt_set_dir_to_target) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern f32 _get_target_dir(EventEntry* event, void* npc, s32 target);

    s32* args;
    s32 name;
    s32 target;
    void* npc;

    args = event->args;

    name = evtGetValue(event, args[0]);
    target = evtGetValue(event, args[1]);

    npc = evtNpcNameToPtr(event, name);

    *(f32*)((s32)npc + 0x144) = _get_target_dir(event, npc, target);

    return 2;
}

USER_FUNC(evt_set_dir_to_home) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);

    s32* args;
    s32 name;
    void* npc;

    args = event->args;

    name = evtGetValue(event, args[0]);
    evtGetValue(event, args[1]);

    npc = evtNpcNameToPtr(event, name);

    *(f32*)((s32)npc + 0x144) = angleABf(
        *(f32*)((s32)npc + 0x8C),
        *(f32*)((s32)npc + 0x94),
        *(f32*)((s32)npc + 0x1FC),
        *(f32*)((s32)npc + 0x204)
    );

    return 2;
}

s32 evt_npc_add_dirdist(void* pEvt) {
    extern f32 evtGetFloat(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    extern f32 float_6p2832_8041fda0;
    extern f32 float_360_8041fda4;
    s32* args;
    s32 xArg;
    s32 zArg;
    f32 dir;
    f32 dist;
    f32 x;
    f32 z;
    f32 radians;
    f32 s;
    f32 c;

    args = *(s32**)((s32)pEvt + 0x18);
    xArg = args[0];
    zArg = args[1];
    dir = evtGetFloat(pEvt, args[2]);
    dist = evtGetFloat(pEvt, args[3]);
    x = evtGetFloat(pEvt, xArg);
    z = evtGetFloat(pEvt, zArg);
    radians = (float_6p2832_8041fda0 * dir) / float_360_8041fda4;
    s = (f32)sin(radians);
    c = (f32)cos(radians);
    evtSetFloat(pEvt, xArg, x + dist * s);
    evtSetFloat(pEvt, zArg, z - dist * c);
    return 2;
}

s32 evt_npc_get_loiter_dir(void* pEvt) {
    extern f32 evtGetFloat(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    extern s32 rand(void);
    extern f32 reviseAngle(f32);
    extern f32 float_0_8041fd84;
    s32* args;
    s32 dst;
    f32 min;
    f32 max;
    f32 base;
    f32 range;
    f32 result;

    args = *(s32**)((s32)pEvt + 0x18);
    dst = args[0];
    min = evtGetFloat(pEvt, args[1]);
    max = evtGetFloat(pEvt, args[2]);
    base = evtGetFloat(pEvt, dst);
    range = max - min;
    if (range <= float_0_8041fd84) {
        result = base + min;
    } else {
        result = base + (min + (f32)rand() / range);
    }
    evtSetFloat(pEvt, dst, reviseAngle(result));
    return 2;
}

s32 evt_npc_change_interrupt(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 name;
    s32 type;
    s32 value;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    type = evtGetValue(pEvt, args[1]);
    value = args[2];
    npc = evtNpcNameToPtr(pEvt, name);
    switch (type) {
        case 0:
            *(s32*)((s32)npc + 0x124) = value;
            break;
        case 1:
            *(s32*)((s32)npc + 0x130) = value;
            break;
        case 2:
            *(s32*)((s32)npc + 0x134) = value;
            break;
        case 3:
            *(s32*)((s32)npc + 0x138) = value;
            break;
        case 4:
            *(s32*)((s32)npc + 0x13C) = value;
            break;
        case 7:
            *(s32*)((s32)npc + 0x128) = value;
            break;
        case 8:
            *(s32*)((s32)npc + 0x12C) = value;
            break;
    }
    return 2;
}

USER_FUNC(evt_npc_get_reglid) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(evt, args[1], *(s32*)((s32)npc + 0x11C));
    return 1;
}

s32 evt_npc_restart_regular_event(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void npcCheckInterrupt(void*);
    s32 name;
    void* npc;

    name = evtGetValue(pEvt, *(s32*)*(void**)((s32)pEvt + 0x18));
    npc = evtNpcNameToPtr(pEvt, name);
    *(u32*)((s32)npc + 0x140) |= 0x10000000;
    npcCheckInterrupt(npc);
    return 2;
}

s32 evt_npc_facedirection_add(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern f32 npcTransRytoFaceDir(void*);
    extern void evtSetValue(void*, s32, s32);
    extern char str_mario_8041fdd4[];
    extern f32 float_180_8041fdb0;
    extern f32 float_90_8041fddc;
    extern f32 float_1_8041fd88;
    extern f32 float_neg1_8041fd8c;
    s32* args;
    s32 name;
    f32 base;
    f32 add;
    f32 dir;
    s32 value;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    base = evtGetFloat(pEvt, args[1]);
    add = evtGetFloat(pEvt, args[2]);
    if (strcmp((char*)name, str_mario_8041fdd4) == 0) {
        if (*(f32*)((s32)marioGetPtr() + 0x1AC) == float_180_8041fdb0) {
            dir = float_1_8041fd88;
        } else {
            dir = float_neg1_8041fd8c;
        }
    } else if (npcTransRytoFaceDir(evtNpcNameToPtr(pEvt, name)) == float_90_8041fddc) {
        dir = float_1_8041fd88;
    } else {
        dir = float_neg1_8041fd8c;
    }
    value = (s32)(base + add * dir);
    evtSetValue(pEvt, args[1], value);
    return 2;
}

void* evtNpcNameToPtr(void* event, s32 name) {
    extern void* npcNameToPtr(void*);
    extern char str_me_8041fd80[3];
    extern char str_party_8041fdcc[6];
    extern char str_mario_8041fdd4[6];
    char* names = (char*)vec3_802c1470;

    if (strcmp((char*)name, str_me_8041fd80) == 0) {
        return *(void**)((s32)event + 0x170);
    }
    if (strcmp((char*)name, str_party_8041fdcc) == 0) {
        return (void*)-1;
    }
    if (strcmp((char*)name, names + 0x80) == 0) {
        return (void*)-1;
    }
    if (strcmp((char*)name, names + 0x8C) == 0) {
        return (void*)-2;
    }
    if (strcmp((char*)name, str_mario_8041fdd4) == 0) {
        return (void*)-3;
    }
    if (strcmp((char*)name, names + 0x98) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x32C);
    }
    if (strcmp((char*)name, names + 0xA0) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x330);
    }
    if (strcmp((char*)name, names + 0xA8) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x334);
    }
    if (strcmp((char*)name, names + 0xB0) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x338);
    }
    if (strcmp((char*)name, names + 0xB8) != 0) {
        return npcNameToPtr((void*)name);
    }
    return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x33C);
}

void* evtNpcNameToPtr_NoAssert(void* event, s32 name) {
    extern void* npcNameToPtr_NoAssert(void*);
    extern char str_me_8041fd80[3];
    extern char str_party_8041fdcc[6];
    extern char str_mario_8041fdd4[6];
    char* names = (char*)vec3_802c1470;

    if (strcmp((char*)name, str_me_8041fd80) == 0) {
        return *(void**)((s32)event + 0x170);
    }
    if (strcmp((char*)name, str_party_8041fdcc) == 0) {
        return (void*)-1;
    }
    if (strcmp((char*)name, names + 0x80) == 0) {
        return (void*)-1;
    }
    if (strcmp((char*)name, names + 0x8C) == 0) {
        return (void*)-2;
    }
    if (strcmp((char*)name, str_mario_8041fdd4) == 0) {
        return (void*)-3;
    }
    if (strcmp((char*)name, names + 0x98) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x32C);
    }
    if (strcmp((char*)name, names + 0xA0) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x330);
    }
    if (strcmp((char*)name, names + 0xA8) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x334);
    }
    if (strcmp((char*)name, names + 0xB0) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x338);
    }
    if (strcmp((char*)name, names + 0xB8) != 0) {
        return npcNameToPtr_NoAssert((void*)name);
    }
    return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x33C);
}

USER_FUNC(evt_npc_stop_for_event) {
    extern void npcStopForEvent(void);
    npcStopForEvent();
    return 2;
}

USER_FUNC(evt_npc_start_for_event) {
    extern void npcStartForEvent(void);
    npcStartForEvent();
    return 2;
}

USER_FUNC(evt_npc_stop_for_one_event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcStopForOneEvent(void* npc);
    s32 value = evtGetValue(event, event->args[0]);
    npcStopForOneEvent(evtNpcNameToPtr(event, value));
    return 2;
}

USER_FUNC(evt_npc_start_for_one_event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcStartForOneEvent(void* npc);
    s32 value = evtGetValue(event, event->args[0]);
    npcStartForOneEvent(evtNpcNameToPtr(event, value));
    return 2;
}

USER_FUNC(evt_npc_change_fbat_mode) {
    fbatChangeMode((s16)evtGetValue(event, event->args[0]));
    return 2;
}

s32 evt_npc_set_offscreen(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern s32 offscreenNameToId(s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    u32 offscreen = evtGetValue(pEvt, args[1]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    if (offscreen == 0) {
        *(s32*)((s32)npc + 0x110) = -1;
    } else {
        *(s32*)((s32)npc + 0x110) = offscreenNameToId(offscreen);
    }

    return 2;
}

s32 evt_npc_set_color(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void npcSetColor(char* name, void* color);
    extern u32 unk_80429560;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    u8 r = evtGetValue(pEvt, args[1]);
    u8 g = evtGetValue(pEvt, args[2]);
    u8 b = evtGetValue(pEvt, args[3]);
    u8 a = evtGetValue(pEvt, args[4]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    u32 copy;
    u32 color = unk_80429560;

    npc = (void*)((s32)npc + 8);
    ((u8*)&color)[0] = r;
    ((u8*)&color)[1] = g;
    ((u8*)&color)[2] = b;
    ((u8*)&color)[3] = a;
    copy = color;
    npcSetColor(npc, &copy);
    return 2;
}

s32 evt_npc_blur_onoff(EventEntry* event) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcBlurOn(void* npc);
    extern void npcBlurOff(void* npc);
    s32* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 name = evtGetValue(event, args[1]);
    void* npc = evtNpcNameToPtr(event, name);

    npc = (void*)((s32)npc + 8);
    if (on) {
        npcBlurOn(npc);
    } else {
        npcBlurOff(npc);
    }
    return 2;
}

USER_FUNC(evt_npc_check) {
    extern void* npcNameToPtr_NoAssert(s32 name);
    s32* args = event->args;
    if (npcNameToPtr_NoAssert(evtGetValue(event, args[0])) == 0) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    return 2;
}

USER_FUNC(evt_npc_set_attack_mode) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s16 value = evtGetValue(event, args[1]);
    *(s32*)((s32)evtNpcNameToPtr(event, name) + 0x2F8) = value;
    return 2;
}

USER_FUNC(evt_npc_set_battle_info) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcSetBattleInfo(void* npc, s32 info);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 info = evtGetValue(event, args[1]);
    npcSetBattleInfo(evtNpcNameToPtr(event, name), info);
    return 2;
}

s32 evt_npc_set_battle_rule(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    s32 v0 = evtGetValue(pEvt, args[1]);
    s32 v1 = evtGetValue(pEvt, args[2]);
    s32 v2 = evtGetValue(pEvt, args[3]);

    *(u8*)((s32)npc + 0x2F4) = v0;
    *(u8*)((s32)npc + 0x2F5) = v1;
    *(u8*)((s32)npc + 0x2F6) = v2;
    return 2;
}

s32 evt_npc_battle_start(void* pEvt, s32 isFirstCall) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void marioFBattlePrepare(void);
    extern void marioCtrlOff(void);
    extern void partyCtrlOff(void*);
    extern void psndSetFlag(s32);
    extern void seqSetSeq(s32, s32, s32);
    s32* args;
    s32 name;
    void* fbat;
    void* info;
    void* npc;
    void* party;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    evtGetValue(pEvt, args[1]);
    fbat = fbatGetPointer();
    npc = evtNpcNameToPtr(pEvt, name);
    info = (void*)((s32)fbat + 0x20);
    if (isFirstCall != 0) {
        *(void**)((s32)fbat + 8) = npc;
        marioFBattlePrepare();
        marioCtrlOff();
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            partyCtrlOff(party);
        }
        BattleInformationSetResult(info, 0);
        psndSetFlag(0x10);
        seqSetSeq(4, 0, 0);
        return 0;
    }
    return 2;
}

s32 evt_npc_wait_battle_end(void) {
    extern void marioCtrlOn(void);
    extern void partyCtrlOn(void*);
    extern void L_psndBGM_stop(void);
    extern void marioFBattlePost(void);
    void* info;
    void* party;

    info = (void*)((s32)fbatGetPointer() + 0x20);
    if (seqGetSeq() != 2) {
        return 0;
    }
    if (BattleInformationGetResult(info) == 0) {
        return 0;
    }
    marioCtrlOn();
    party = partyGetPtr(marioGetPartyId());
    if (party != 0) {
        partyCtrlOn(party);
    }
    L_psndBGM_stop();
    marioFBattlePost();
    return 2;
}

USER_FUNC(evt_npc_get_battle_result) {
    s32 arg = event->args[0];
    evtSetValue(event, arg, BattleInformationGetResult((void*)((s32)fbatGetPointer() + 0x20)));
    return 2;
}

USER_FUNC(evt_npc_check_escape_battle) {
    extern void marioSetMutekiTime(s32 time);
    if (evtGetValue(event, event->args[0]) == 4) {
        marioSetMutekiTime(3000);
    }
    return 2;
}

USER_FUNC(evt_npc_get_battle_rule_keep_result) {
    EventEntry* evt = event;
    s32 arg = event->args[0];
    void* fbat = fbatGetPointer();
    evtSetValue(evt, arg, *(u8*)((s32)fbat + 0x39));
    return 2;
}

USER_FUNC(evt_npc_get_btlsetup_work) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 npcGetBtlSetupWork(void* npc, s32 index);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 idx = evtGetValue(event, args[1]);
    s32 dst = args[2];
    evtSetValue(evt, dst, npcGetBtlSetupWork(npc, idx));
    return 2;
}

USER_FUNC(evt_npc_set_btlsetup_work) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcSetBtlSetupWork(void* npc, s32 index, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 idx = evtGetValue(event, args[1]);
    s32 value = evtGetValue(evt, args[2]);
    npcSetBtlSetupWork(npc, idx, value);
    return 2;
}

s32 evt_npc_set_link(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void npcSetLink(void* npc, void* link);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    s32 linkName = evtGetValue(pEvt, args[1]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    void* link = evtNpcNameToPtr(pEvt, linkName);

    npcSetLink(npc, link);
    return 2;
}

USER_FUNC(evt_npc_get_drop_fixitem) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(evt, args[1], *(s32*)((s32)npc + 0x238));
    return 2;
}

USER_FUNC(evt_npc_get_drop_item) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 dst = args[1];

    if ((*(u32*)((s32)npc + 0x234) & 8) != 0) {
        evtSetValue(evt, dst, 0);
    } else {
        evtSetValue(evt, dst, *(s32*)((s32)npc + 0x23C));
    }
    return 2;
}

s32 evt_npc_get_drop_heart(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void evtSetValue(void* event, s32 dst, s32 value);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    s32 dst = args[1];
    s32 badge;
    s32 value;

    if (*(u32*)((s32)npc + 0x234) & 2) {
        evtSetValue(pEvt, dst, 0);
    } else {
        badge = pouchEquipCheckBadge(0x137);
        value = *(s32*)((s32)npc + 0x240);
        if (badge > 0) {
            if (badge == 1) {
                value += irand(3) + 1;
            } else {
                value += irand(badge + 3) + 1;
            }
        }
        evtSetValue(pEvt, dst, value);
    }
    return 2;
}

s32 evt_npc_get_drop_flower(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void evtSetValue(void* event, s32 dst, s32 value);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    s32 dst = args[1];
    s32 badge;
    s32 value;

    if (*(u32*)((s32)npc + 0x234) & 4) {
        evtSetValue(pEvt, dst, 0);
    } else {
        badge = pouchEquipCheckBadge(0x138);
        value = *(s32*)((s32)npc + 0x244);
        if (badge > 0) {
            if (badge == 1) {
                value += irand(3) + 1;
            } else {
                value += irand(badge + 3) + 1;
            }
        }
        evtSetValue(pEvt, dst, value);
    }
    return 2;
}

void _npc_check_coin(void* npc, s32* coin) {
    *coin += *(u8*)((s32)npc + 0x315);
}

void _npc_check_coin_group(void* group, s32* coin) {
    void* child;
    s32 offset;
    s32 i;
    s32 entryOffset;

    child = *(void**)((s32)group + 0x328);
    if (child != 0) {
        _npc_check_coin_group(child, coin);
    }

    i = 0;
    offset = 0;
    while (i < 4) {
        entryOffset = offset;
        entryOffset += 0x330;
        child = *(void**)((s32)group + entryOffset);

        if (child != 0) {
            _npc_check_coin(child, coin);
        }

        i++;
        offset += 4;
    }

    _npc_check_coin(group, coin);
}

s32 evt_npc_get_drop_coin(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void evtSetValue(void* event, s32 dst, s32 value);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    s32 dst = args[1];
    s32 coin = 0;

    if (*(u32*)((s32)npc + 0x234) & 1) {
        evtSetValue(pEvt, dst, 0);
        return 2;
    }
    _npc_check_coin_group(npc, &coin);
    evtSetValue(pEvt, dst, coin);
    return 2;
}

USER_FUNC(evt_npc_getback_item_entry) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcGetBackItemEntry(void* npc);
    s32 value = evtGetValue(event, event->args[0]);
    npcGetBackItemEntry(evtNpcNameToPtr(event, value));
    return 2;
}

USER_FUNC(evt_npc_get_unitwork) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 idx = evtGetValue(event, args[1]);
    evtSetValue(evt, args[2], *(s32*)((s32)npc + 0x304 + idx * 4));
    return 2;
}

USER_FUNC(evt_npc_set_unitwork) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 idx = evtGetValue(event, args[1]);
    s32 value = evtGetValue(evt, args[2]);
    *(s32*)((s32)npc + 0x304 + idx * 4) = value;
    return 2;
}

s32 evt_npc_wait_msec(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    s32* args = event->args;
    s32* userdata = (s32*)((u8*)event + 0x78);
    s32 npcName = evtGetValue(event, args[0]);
    u8* npc = evtNpcNameToPtr(event, npcName);
    s32 temp;
    s64 lifetime = *(s64*)event;

    *(u32*)(npc + 0x1D4) |= 0x10;
    if (isFirstCall) {
        userdata[0] = evtGetValue(event, args[1]);
        userdata[1] = ((u64)lifetime >> 32) & 0xFFFFFFFF;
        userdata[2] = lifetime & 0xFFFFFFFF;
    }

    temp = userdata[0];
    if (!temp) {
        return 2;
    }

    {
        s32 now = lifetime;
        u64 elapsed = now - userdata[2];
        s32 ms = elapsed / ((*(u32*)0x800000F8 >> 2) / 1000);
        return temp <= ms;
    }
}

USER_FUNC(evt_npc_set_balloontype) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name;
    void* npc;
    name = evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    *(u8*)((s32)npc + 0x317) = evtGetValue(event, args[1]);
    return 1;
}

s32 evt_npc_set_tribe(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void* npcGetTribe(s32);
    extern void animPoseSetAnim(s32, char*, s32);
    extern f32 float_0_8041fd84;
    s32* args;
    s32 name;
    s32 tribeName;
    void* npc;
    void* tribe;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    tribeName = evtGetValue(pEvt, args[1]);
    npc = evtNpcNameToPtr(pEvt, name);
    tribe = npcGetTribe(tribeName);
    *(void**)((s32)npc + 0x28) = tribe;
    *(f32*)((s32)npc + 0x14C) = *(f32*)((s32)tribe + 0x28);
    *(f32*)((s32)npc + 0x150) = *(f32*)((s32)tribe + 0x2C);
    if (*(void**)((s32)tribe + 0x10) != 0) {
        strcpy((char*)((s32)npc + 0x4C), *(char**)((s32)tribe + 0x10));
    }
    if (*(void**)((s32)tribe + 0x14) != 0) {
        strcpy((char*)((s32)npc + 0x6C), *(char**)((s32)tribe + 0x14));
    }
    if (*(void**)((s32)tribe + 8) != 0) {
        strcpy((char*)((s32)npc + 0x2C), *(char**)((s32)tribe + 8));
        animPoseSetAnim(*(s32*)((s32)npc + 0x104), (char*)((s32)npc + 0x2C), 1);
    }
    *(s32*)((s32)npc + 0x1E0) = *(s32*)((s32)tribe + 0x40);
    *(s32*)((s32)npc + 0x1E4) = *(s32*)((s32)tribe + 0x44);
    *(s32*)((s32)npc + 0x1EC) = *(s32*)((s32)tribe + 0x4C);
    *(s32*)((s32)npc + 0x1F0) = *(s32*)((s32)tribe + 0x50);
    *(u16*)((s32)npc + 0x1E8) = *(u16*)((s32)tribe + 0x48);
    *(u16*)((s32)npc + 0x1D8) = 1;
    *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
    return 1;
}

s32 evt_npc_set_autotalkpose(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void npcSetMarioAutoTalkPose(const char* stay, const char* talk);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    u32 stay = evtGetValue(pEvt, args[1]);
    u32 talk = evtGetValue(pEvt, args[2]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    if ((u32)((s32)npc + 0x10000) == 0xFFFD) {
        npcSetMarioAutoTalkPose((const char*)stay, (const char*)talk);
        return 1;
    }
    if (stay != 0) {
        strcpy((char*)((s32)npc + 0x4C), (char*)stay);
    }
    if (talk != 0) {
        strcpy((char*)((s32)npc + 0x6C), (char*)talk);
    }
    return 1;
}

s32 evt_npc_majo_disp_on(EventEntry* event, s32 isFirstCall) {
    extern f32 float_100_8041fde4;
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 animGroupBaseAsync(void* name, s32 battle, s32 flags);
    extern s32 psndSFXOn_3D(s32 id, void* pos);
    extern void animPoseSetPaperAnimGroup(s32 poseId, s32 anim, s32 flag);
    extern void* gp;
    extern char str_p_bibi_802c14d8[];
    extern char str_PM_B_2_802c14e8[];
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_3_8041fdbc;
    extern f32 float_10_8041fdc0;
    extern f32 float_0p66667_8041fdc8;
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct NpcLocal {
        u8 pad_000[0x8C];
        VecLocal position;
        VecLocal prevPosition;
        u8 pad_0A4[0x60];
        s32 poseId;
        u8 pad_108[0x4C];
        f32 field_154;
    } NpcLocal;
    typedef struct PoseLocal {
        u8 pad_00[0x90];
        s32 paperId;
    } PoseLocal;
    s32* args = event->args;
    s32 npcName = evtGetValue(event, *args++);
    f32 prevX = evtGetFloat(event, *args++);
    f32 prevY = evtGetFloat(event, *args++);
    f32 prevZ = evtGetFloat(event, *args++);
    s32 toggle = evtGetValue(event, *args++);
    s32 group = (*(s32*)((u8*)gp + 0x14) != 0);
    NpcLocal* npc = evtNpcNameToPtr(event, npcName);
    s32* userdata = (s32*)((u8*)event + 0x78);

    if (isFirstCall) {
        userdata[0] = -1;
        userdata[1] = 0;
    }
    if (!animGroupBaseAsync(str_p_bibi_802c14d8, group, 0)) {
        return 0;
    }
    if (toggle) {
        return 0;
    }

    switch (userdata[0]) {
        case -1:
            npc->position = npc->prevPosition = (VecLocal){ prevX, prevY, prevZ };
            animPoseSetEffect(npc->poseId, str_p_bibi_802c14d8, 1);
            animPoseSetEffectAnim(npc->poseId, str_PM_B_2_802c14e8, 1);
            animPosePaperPeraOn(npc->poseId);
            animPoseSetLocalTime(npc->poseId, float_0_8041fd84);
            userdata[0]++;
            psndSFXOn_3D(0x275, &npc->position);
            break;

        default:
            break;

        case 0:
            npc->field_154 += (float_3_8041fdbc - npc->field_154) / float_100_8041fde4;
            animPoseSetLocalTime(npc->poseId, float_0_8041fd84);
            userdata[1]++;
            if (userdata[1] > 30) {
                userdata[0]++;
                userdata[1] = 0;
                switch (animPoseGetVivianType(npc->poseId)) {
                    case 0:
                        break;
                    case 1:
                        psndSFXOn_3D(0x937, &npc->position);
                        break;
                    case 2:
                        psndSFXOn_3D(0x27B, &npc->position);
                        break;
                    case 3:
                        psndSFXOn_3D(0x278, &npc->position);
                        break;
                }
            }
            break;

        case 1:
            animPoseSetEffect(npc->poseId, str_p_bibi_802c14d8, 1);
            animPoseSetEffectAnim(npc->poseId, str_PM_B_2_802c14e8, 1);
            animPosePaperPeraOn(npc->poseId);
            userdata[0]++;
            break;

        case 2:
            if (animPoseGetLoopTimes(((PoseLocal*)animPoseGetAnimPosePtr(npc->poseId))->paperId) >= float_0p66667_8041fdc8) {
                animPoseSetPaperAnimGroup(npc->poseId, 0, 1);
                userdata[0]++;
            }
            break;

        case 3:
            npc->field_154 += (float_1_8041fd88 - npc->field_154) / float_10_8041fdc0;
            userdata[1]++;
            if (userdata[1] > 30) {
                return 2;
            }
            break;
    }
    return 0;
}

s32 evt_npc_majo_disp_off(EventEntry* event, s32 isFirstCall) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 psndSFXOn_3D(s32 id, void* pos);
    extern s32 animGroupBaseAsync(void* name, s32 mode, s32 flags);
    extern void* gp;
    extern char str_p_bibi_802c14d8[];
    extern char str_PM_B_1_802c14e0[];
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_3_8041fdbc;
    extern f32 float_10_8041fdc0;
    extern f32 float_8_8041fdc4;
    s32 name;
    s32 battle;
    void* npc;
    s32 state;
    s32 vivianType;
    s32 effectPose;
    s32 first;
    void* pose;
    EventEntry* evt;

    first = isFirstCall;
    evt = event;
    name = evtGetValue(evt, evt->args[0]);
    battle = (*(s32*)((s32)gp + 0x14) != 0);
    npc = evtNpcNameToPtr(evt, name);
    if (first != 0) {
        *(s32*)((s32)evt + 0x78) = 0;
        *(s32*)((s32)evt + 0x7C) = 0;
        psndSFXOn_3D(0x275, (void*)((s32)npc + 0x8C));
    }

    if (animGroupBaseAsync(str_p_bibi_802c14d8, battle, 0) == 0) {
        return 0;
    }

    state = *(s32*)((s32)evt + 0x78);
    switch (state) {
        case 0:
            *(f32*)((s32)npc + 0x154) += (float_3_8041fdbc - *(f32*)((s32)npc + 0x154)) / float_10_8041fdc0;
            *(s32*)((s32)evt + 0x7C) += 1;
            if (*(s32*)((s32)evt + 0x7C) > 0x1E) {
                *(s32*)((s32)evt + 0x78) += 1;
                *(s32*)((s32)evt + 0x7C) = 0;
                vivianType = animPoseGetVivianType(*(s32*)((s32)npc + 0x104));
                switch (vivianType) {
                    case 0:
                        break;
                    case 1:
                        psndSFXOn_3D(0x936, (void*)((s32)npc + 0x8C));
                        break;
                    case 2:
                        psndSFXOn_3D(0x27A, (void*)((s32)npc + 0x8C));
                        break;
                    case 3:
                        psndSFXOn_3D(0x277, (void*)((s32)npc + 0x8C));
                        break;
                }
            }
            break;
        case 1:
            animPoseSetEffect(*(s32*)((s32)npc + 0x104), str_p_bibi_802c14d8, 1);
            animPoseSetEffectAnim(*(s32*)((s32)npc + 0x104), str_PM_B_1_802c14e0, 1);
            animPosePaperPeraOn(*(s32*)((s32)npc + 0x104));
            pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
            effectPose = *(s32*)((s32)pose + 0x90);
            animPoseSetLocalTime(effectPose, float_8_8041fdc4);
            *(s32*)((s32)evt + 0x78) += 1;
            break;
        case 2:
            pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
            if (animPoseGetLoopTimes(*(s32*)((s32)pose + 0x90)) >= float_1_8041fd88) {
                *(s32*)((s32)evt + 0x78) += 1;
            }
            break;
        case 3:
            *(f32*)((s32)npc + 0x154) += -*(f32*)((s32)npc + 0x154) / float_10_8041fdc0;
            *(s32*)((s32)evt + 0x7C) += 1;
            if (*(s32*)((s32)evt + 0x7C) > 0x1E) {
                *(f32*)((s32)npc + 0x154) = float_0_8041fd84;
                return 2;
            }
            break;
    }
    return 0;
}

USER_FUNC(evt_npc_get_kpencount_type) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 dst = args[1];
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, dst, *(u16*)((s32)npc + 0x31C));
    return 2;
}

s32 evt_npc_calc_score(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern s32 npcCalcScore(void* npc);

    s32* args;
    s32 name;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    npc = evtNpcNameToPtr(pEvt, name);
    npcCalcScore(npc);

    return 2;
}

void _kamek_houki_kemuri(s32 npc, s32 mode) {
    extern void* gp;
    extern void* effKemuri10N64Entry(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 type);
    extern u8 cnt_1369;
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_neg1_8041fd8c;
    extern f32 float_180_8041fdb0;
    extern f32 float_36_8041fdb4;
    extern f32 float_18_8041fdb8;
    extern char str_kpa_01_802c14d0[];
    f32 x;
    f32 direction;
    u8 mask;
    void* effect;

    cnt_1369++;
    switch (mode) {
        case 3:
            mask = 7;
            break;
        case 2:
            mask = 0xF;
            break;
        default:
            return;
    }
    if ((cnt_1369 & mask) != 0) return;

    if (float_0_8041fd84 < *(f32*)(npc + 0x144) &&
        *(f32*)(npc + 0x144) < float_180_8041fdb0) {
        direction = float_1_8041fd88;
        x = -((float_36_8041fdb4 * *(f32*)(npc + 0xE0)) - *(f32*)(npc + 0x8C));
    } else {
        direction = float_neg1_8041fd8c;
        x = (float_36_8041fdb4 * *(f32*)(npc + 0xE0)) + *(f32*)(npc + 0x8C);
    }
    effect = effKemuri10N64Entry(x,
                                 *(f32*)(npc + 0x90) + float_18_8041fdb8 * *(f32*)(npc + 0xE0),
                                 *(f32*)(npc + 0x94), direction,
                                 float_0_8041fd84, float_0_8041fd84, 0);
    if (strcmp((char*)gp + 0x12C, str_kpa_01_802c14d0) == 0) {
        *(s32*)(*(u8**)((u8*)effect + 0xC) + 0x6C) = 7;
    }
}

s32 evt_npc_kamek_move_position(EventEntry* event, s32 isFirstCall) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern u8 _kamek_houki_kemuri(s32 npc, s32 animState);
    extern f32 __float_nan;
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_0p1_8041fd98;
    extern f32 float_20_8041fd9c;
    extern f32 float_1000_8041fd94;
    extern f32 float_6p2832_8041fda0;
    extern f32 float_360_8041fda4;
    extern f32 float_3p1416_8041fda8;
    extern f32 float_2_8041fdac;

    s32* args;
    void* npc;
    s32 name;
    f32 targetX;
    f32 arc;
    f32 targetZ;
    f32 duration;
    f32 step;
    s32 flags;
    f32 delta[3];
    f32 totalDist;
    f32 remainDist;
    f32 distSq;
    f64 sqrtD;
    f64 inv;
    u32 bits;
    s32 cls;
    union { f32 f; u32 u; } bitsView;
    f32 movedDist;
    f32 s0;
    f32 s1;
    f32 temp;
    f32 zero;
    const u8* constBase;

    constBase = vec3_802c1470;
    args = event->args;
    name = evtGetValue(event, args[0]);
    targetX = evtGetFloat(event, args[1]);
    targetZ = evtGetFloat(event, args[2]);
    arc = evtGetFloat(event, args[3]);
    duration = (f32)evtGetValue(event, args[4]) / float_1000_8041fd94;
    step = evtGetFloat(event, args[5]);
    flags = args[6];
    zero = float_0_8041fd84;

    npc = evtNpcNameToPtr(event, name);
    *(u32*)((s32)npc + 0x1D4) |= 0x10;
    if ((flags & 2) != 0) {
        *(u32*)((s32)npc + 0x1D4) |= 0x1000;
    }

    if (isFirstCall != 0) {
        *(s32*)((s32)npc + 0x18C) = 0;
        *(s32*)((s32)npc + 0x188) = 0;
        *(s32*)((s32)npc + 0x108) = 0;

        *(f32*)((s32)npc + 0x164) = targetX;
        *(f32*)((s32)npc + 0x168) = *(f32*)((s32)npc + 0x90);
        *(f32*)((s32)npc + 0x16C) = targetZ;

        *(s32*)((s32)npc + 0x158) = *(s32*)((s32)npc + 0x8C);
        *(s32*)((s32)npc + 0x15C) = *(s32*)((s32)npc + 0x90);
        *(s32*)((s32)npc + 0x160) = *(s32*)((s32)npc + 0x94);

        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), delta);
        distSq = delta[0] * delta[0] + delta[2] * delta[2];
        sqrtD = (f64)distSq;
        if (sqrtD > (f64)float_0_8041fd84) {
            inv = __frsqrte(sqrtD);
            inv = *(const f64*)(constBase + 0x48) * inv *
                  -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50));
            inv = *(const f64*)(constBase + 0x48) * inv *
                  -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50));
            totalDist = (f32)(sqrtD * *(const f64*)(constBase + 0x48) * inv *
                              -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50)));
        } else {
            if (*(const f64*)(constBase + 0x58) <= sqrtD) {
                bitsView.f = distSq;
                bits = bitsView.u & 0x7F800000;
                switch (bits) {
                case 0x7F800000:
                    cls = (bitsView.u & 0x7FFFFF) ? 1 : 2;
                    break;
                case 0:
                    cls = (bitsView.u & 0x7FFFFF) ? 5 : 3;
                    break;
                default:
                    cls = 4;
                    break;
                }
                if (cls == 1) {
                    totalDist = __float_nan;
                } else {
                    totalDist = distSq;
                }
            } else {
                totalDist = __float_nan;
            }
        }
        *(f32*)((s32)npc + 0x1AC) = angleABf(zero, zero, delta[0], delta[2]);
        *(f32*)((s32)npc + 0x1B0) = step;
        *(f32*)((s32)npc + 0x1C0) = duration;
        *(f32*)((s32)npc + 0x1B8) = zero;

        if ((*(f32*)((s32)npc + 0x1B0) <= float_0p1_8041fd98) && (zero == *(f32*)((s32)npc + 0x1C0))) {
            *(f32*)((s32)npc + 0x1B0) = float_1_8041fd88;
        }
        if (*(f32*)((s32)npc + 0x1B0) <= float_0p1_8041fd98) {
            *(f32*)((s32)npc + 0x1B0) = totalDist / *(f32*)((s32)npc + 0x1C0);
            *(f32*)((s32)npc + 0x1C0) = zero;
        }
    }

    if (((flags & 4) != 0) && (*(s32*)((s32)npc + 0x2FC) != 0)) {
        return 2;
    }
        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), delta);
        distSq = delta[0] * delta[0] + delta[2] * delta[2];
        sqrtD = (f64)distSq;
        if (sqrtD > (f64)float_0_8041fd84) {
            inv = __frsqrte(sqrtD);
            inv = *(const f64*)(constBase + 0x48) * inv *
                  -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50));
            inv = *(const f64*)(constBase + 0x48) * inv *
                  -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50));
            totalDist = (f32)(sqrtD * *(const f64*)(constBase + 0x48) * inv *
                              -((sqrtD * inv * inv) - *(const f64*)(constBase + 0x50)));
        } else {
            if (*(const f64*)(constBase + 0x58) <= sqrtD) {
                bitsView.f = distSq;
                bits = bitsView.u & 0x7F800000;
                switch (bits) {
                case 0x7F800000:
                    cls = (bitsView.u & 0x7FFFFF) ? 1 : 2;
                    break;
                case 0:
                    cls = (bitsView.u & 0x7FFFFF) ? 5 : 3;
                    break;
                default:
                    cls = 4;
                    break;
                }
                if (cls == 1) {
                    totalDist = __float_nan;
                } else {
                    totalDist = distSq;
                }
            } else {
                totalDist = __float_nan;
            }
        }

        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x8C), delta);
        distSq = delta[0] * delta[0] + delta[2] * delta[2];
        sqrtD = (f64)distSq;
        if (sqrtD > (f64)float_0_8041fd84) {
            inv = __frsqrte(sqrtD);
            inv = 0.5 * inv * -((sqrtD * inv * inv) - 3.0);
            inv = 0.5 * inv * -((sqrtD * inv * inv) - 3.0);
            remainDist = (f32)(sqrtD * 0.5 * inv * -((sqrtD * inv * inv) - 3.0));
        } else {
            if (*(const f64*)(constBase + 0x58) <= sqrtD) {
                bitsView.f = distSq;
                bits = bitsView.u & 0x7F800000;
                switch (bits) {
                case 0x7F800000:
                    cls = (bitsView.u & 0x7FFFFF) ? 1 : 2;
                    break;
                case 0:
                    cls = (bitsView.u & 0x7FFFFF) ? 5 : 3;
                    break;
                default:
                    cls = 4;
                    break;
                }
                if (cls == 1) {
                    remainDist = __float_nan;
                } else {
                    remainDist = distSq;
                }
            } else {
                remainDist = __float_nan;
            }
        }

        if (*(f32*)((s32)npc + 0x1C0) == zero) {
            if (remainDist <= (float_20_8041fd9c * *(f32*)((s32)npc + 0x1B0)) / float_1000_8041fd94) {
                *(s32*)((s32)npc + 0x98) = *(s32*)((s32)npc + 0x164);
                *(s32*)((s32)npc + 0x9C) = *(s32*)((s32)npc + 0x168);
                *(s32*)((s32)npc + 0xA0) = *(s32*)((s32)npc + 0x16C);
                *(s32*)((s32)npc + 0x8C) = *(s32*)((s32)npc + 0x98);
                *(s32*)((s32)npc + 0x90) = *(s32*)((s32)npc + 0x9C);
                *(s32*)((s32)npc + 0x94) = *(s32*)((s32)npc + 0xA0);
                *(f32*)((s32)npc + 0x1B0) = zero;
                return 2;
            }
        } else if (*(f32*)((s32)npc + 0x1C0) <= *(f32*)((s32)npc + 0x1A0)) {
            *(s32*)((s32)npc + 0x98) = *(s32*)((s32)npc + 0x164);
            *(s32*)((s32)npc + 0x9C) = *(s32*)((s32)npc + 0x168);
            *(s32*)((s32)npc + 0xA0) = *(s32*)((s32)npc + 0x16C);
            *(s32*)((s32)npc + 0x8C) = *(s32*)((s32)npc + 0x98);
            *(s32*)((s32)npc + 0x90) = *(s32*)((s32)npc + 0x9C);
            *(s32*)((s32)npc + 0x94) = *(s32*)((s32)npc + 0xA0);
            *(f32*)((s32)npc + 0x1B0) = zero;
            return 2;
        }

        if (((flags & 1) != 0) && ((*(u32*)((s32)npc + 0x1D4) & 1) == 0)) {
            *(f32*)((s32)npc + 0x1AC) = angleABf(zero, zero, delta[0], delta[2]);
        }

        if (((*(u32*)((s32)npc + 0x1D4) & 2) != 0) && (*(f32*)((s32)npc + 0x1B0) > float_0p1_8041fd98)) {
            if ((npc != *(void**)((s32)fbatGetPointer() + 8)) ||
                ((*(s16*)fbatGetPointer() != 5) &&
                 (*(s16*)fbatGetPointer() != 6))) {
                npcTuningRy(npc, *(f32*)((s32)npc + 0x1AC));
            }
        }

        temp = (f32)sin((f64)((float_6p2832_8041fda0 * *(f32*)((s32)npc + 0x1AC)) /
                              float_360_8041fda4));
        *(f32*)((s32)npc + 0x1B4) +=
            *(f32*)((s32)npc + 0x1A4) * (*(f32*)((s32)npc + 0x1B0) * temp);
        temp = (f32)cos((f64)((float_6p2832_8041fda0 * *(f32*)((s32)npc + 0x1AC)) /
                              float_360_8041fda4));
        *(f32*)((s32)npc + 0x1BC) +=
            *(f32*)((s32)npc + 0x1A4) * (*(f32*)((s32)npc + 0x1B0) * -temp);

        movedDist = totalDist - remainDist;
        s0 = (f32)sin((f64)(float_3p1416_8041fda8 *
                            (float_2_8041fdac * (movedDist / totalDist))));
        s1 = (f32)sin((f64)(float_3p1416_8041fda8 *
                            (float_2_8041fdac *
                             (((*(f32*)((s32)npc + 0x1B0) * *(f32*)((s32)npc + 0x1A4)) +
                               movedDist) /
                              totalDist))));
        *(f32*)((s32)npc + 0x1B8) += arc * s1 - arc * s0;

        if ((flags & 8) == 0) {
            _kamek_houki_kemuri((s32)npc, *(s32*)((s32)npc + 0x108));
        }
    return 0;
}

s32 evt_npc_kamek_kemuri1(void* pEvt, s32 isFirstCall) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern void* npcNameToPtr(s32 name);
    extern f32 float_0_8041fd84;
    s32* args = *(s32**)((u8*)pEvt + 0x18);
    u8* npc = npcNameToPtr(evtGetValue(pEvt, args[0]));

    if (isFirstCall) {
        return 0;
    }

    if (*(f32*)(npc + 0x1C0) != float_0_8041fd84) {
        if (*(f32*)(npc + 0x1C0) > *(f32*)(npc + 0x1A0)) {
            _kamek_houki_kemuri((s32)npc, *(s32*)(npc + 0x108));
            return 0;
        }
        goto block_7;
    }

    if (*(f32*)(npc + 0x1B0) > float_0_8041fd84) {
        _kamek_houki_kemuri((s32)npc, *(s32*)(npc + 0x108));
        return 0;
    }
block_7:
    return 2;
}

s32 evt_npc_kamek_kemuri2(void* pEvt, s32 isFirstCall) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern void* npcNameToPtr(s32 name);
    extern f32 float_0_8041fd84;
    extern f32 float_1000_8041fd94;
    s32* args = *(s32**)((u8*)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    f32 duration = (f32)evtGetValue(pEvt, args[1]) / float_1000_8041fd94;
    s32 type = evtGetValue(pEvt, args[2]);
    u8* npc = npcNameToPtr(name);

    if (isFirstCall != 0) {
        if (duration <= float_0_8041fd84) {
            return 2;
        }
        *(u32*)(npc + 0x18C) = 0;
        *(u32*)(npc + 0x188) = 0;
        *(f32*)(npc + 0x1C0) = duration;
        return 0;
    }
    if (*(f32*)(npc + 0x1C0) <= *(f32*)(npc + 0x1A0)) {
        return 2;
    }
    _kamek_houki_kemuri((s32)npc, (type != 0) + 2);
    return 0;
}

s32 evt_fbat_trans_floor_position(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern f32 evtSetFloat(void* evt, s32 value, f32 result);
    extern s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz,
                              s32 flags, void* out0, f32* outY, void* out2,
                              f32* outDist, void* out4, void* out5, void* out6);
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_neg1_8041fd8c;
    s32* args = *(s32**)((u8*)pEvt + 0x18);
    s32 xArg = args[0];
    s32 yArg = args[1];
    s32 zArg = args[2];
    f32 x = evtGetFloat(pEvt, xArg);
    f32 y = evtGetFloat(pEvt, yArg);
    f32 z = evtGetFloat(pEvt, zArg);
    f32 addY = evtGetFloat(pEvt, args[3]);
    f32 distance = evtGetFloat(pEvt, args[4]);
    f32 floorY;
    s32 out0;
    s32 out2;
    s32 out4;
    s32 out5;
    s32 out6;

    if (hitCheckFilter(x, y + float_1_8041fd88, z,
                       float_0_8041fd84, float_neg1_8041fd8c,
                       float_0_8041fd84, 0, &out0, &floorY, &out2,
                       &distance, &out4, &out5, &out6) != 0) {
        y = floorY + addY;
    }
    evtSetFloat(pEvt, xArg, x);
    evtSetFloat(pEvt, yArg, y);
    evtSetFloat(pEvt, zArg, z);
    return 2;
}

s32 unk_8004d180(EventEntry* event) {
    extern f32 evtGetFloat(EventEntry* event, s32 index);
    extern void evtSetFloat(EventEntry* event, s32 index, f32 value);
    extern u32 hitCheckFilter(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 flags, void* out0, f32* outY, void* out2, f32* out3, void* out4, void* out5, void* out6);
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_neg1_8041fd8c;
    extern f32 float_0p5_8041fd90;
    s32* args;
    s32 xVar;
    s32 yVar;
    s32 zVar;
    f32 x;
    f32 y;
    f32 z;
    f32 addY;
    f32 initialOut3;
    f32 width;
    f32 bestY;
    f32 halfWidth;
    s32 out0;
    f32 checkY;
    s32 out2;
    f32 out3;
    s32 out4;
    s32 out5;
    s32 out6;

    args = event->args;
    xVar = args[0];
    x = evtGetFloat(event, xVar);
    yVar = args[1];
    y = evtGetFloat(event, yVar);
    zVar = args[2];
    z = evtGetFloat(event, zVar);
    addY = evtGetFloat(event, args[3]);
    initialOut3 = evtGetFloat(event, args[4]);
    width = evtGetFloat(event, args[5]);

    bestY = y;
    y += float_1_8041fd88;
    out3 = initialOut3;
    if (hitCheckFilter(x, y, z, float_0_8041fd84, float_neg1_8041fd8c, float_0_8041fd84, 0, &out0, &checkY, &out2, &out3, &out4, &out5, &out6) != 0) {
        bestY = checkY + addY;
    }

    halfWidth = float_0p5_8041fd90 * width;
    if (hitCheckFilter(x + halfWidth, y, z, float_0_8041fd84, float_neg1_8041fd8c, float_0_8041fd84, 0, &out0, &checkY, &out2, &out3, &out4, &out5, &out6) != 0) {
        if (checkY + addY > bestY) {
            bestY = checkY + addY;
        }
    }

    if (hitCheckFilter(x - halfWidth, y, z, float_0_8041fd84, float_neg1_8041fd8c, float_0_8041fd84, 0, &out0, &checkY, &out2, &out3, &out4, &out5, &out6) != 0) {
        if (checkY + addY > bestY) {
            bestY = checkY + addY;
        }
    }

    evtSetFloat(event, xVar, x);
    evtSetFloat(event, yVar, bestY);
    evtSetFloat(event, zVar, z);
    return 2;
}

s32 evt_npc_sound_data_reset(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern const f32 float_0_8041fd84;
    typedef struct TribeLocal {
        u8 pad_00[0x40];
        s32 footstepSfxA;
        s32 footstepSfxB;
        u16 footstepStride;
        u8 pad_4A[2];
        s32 jumpSfx;
        s32 landSfx;
    } TribeLocal;
    typedef struct NpcLocal {
        u8 pad_00[0x28];
        TribeLocal* tribe;
        u8 pad_2C[0x1AC];
        u16 footstepToggle;
        u8 pad_1DA[2];
        f32 footstepAccum;
        s32 footstepSfxA;
        s32 footstepSfxB;
        u16 footstepStride;
        u8 pad_1EA[2];
        s32 jumpSfx;
        s32 landSfx;
    } NpcLocal;
    s32 name = evtGetValue(pEvt, *(s32*)*(void**)((s32)pEvt + 0x18));
    NpcLocal* npc = evtNpcNameToPtr(pEvt, name);
    TribeLocal* tribe = npc->tribe;

    if (tribe != 0) {
        npc->footstepSfxA = tribe->footstepSfxA;
        npc->footstepSfxB = tribe->footstepSfxB;
        npc->jumpSfx = tribe->jumpSfx;
        npc->landSfx = tribe->landSfx;
        npc->footstepStride = tribe->footstepStride;
        npc->footstepToggle = 1;
        npc->footstepAccum = float_0_8041fd84;
    }
    return 2;
}

s32 evt_npc_sound_data_set(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern f32 float_0_8041fd84;
    s32* args;
    void* npc;
    s32 arg1;
    s32 arg2;
    s32 arg3;
    s32 arg4;
    s32 arg5;
    s32 value1;
    s32 value2;
    s32 value3;
    u16 value4;
    s32 value5;

    args = *(s32**)((s32)pEvt + 0x18);
    npc = evtNpcNameToPtr(pEvt, evtGetValue(pEvt, args[0]));
    arg1 = args[1];
    value1 = evtGetValue(pEvt, arg1);
    arg2 = args[2];
    value2 = evtGetValue(pEvt, arg2);
    arg3 = args[3];
    value4 = evtGetValue(pEvt, arg3);
    arg4 = args[4];
    value3 = evtGetValue(pEvt, arg4);
    arg5 = args[5];
    value5 = evtGetValue(pEvt, arg5);

    if ((u32)arg1 != 0xF1194D80) {
        *(s32*)((s32)npc + 0x1E0) = value1;
    }
    if ((u32)arg2 != 0xF1194D80) {
        *(s32*)((s32)npc + 0x1E4) = value2;
    }
    if ((u32)arg4 != 0xF1194D80) {
        *(s32*)((s32)npc + 0x1EC) = value3;
    }
    if ((u32)arg5 != 0xF1194D80) {
        *(s32*)((s32)npc + 0x1F0) = value5;
    }
    if ((u32)arg3 != 0xF1194D80) {
        *(u16*)((s32)npc + 0x1E8) = value4;
    }
    *(u16*)((s32)npc + 0x1D8) = 1;
    *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
    return 2;
}

s32 evt_npc_release_filednpc(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* npcGetWorkPtr(void);
    s32 value;
    void* work;

    value = evtGetValue(pEvt, *(s32*)*(void**)((s32)pEvt + 0x18));
    work = npcGetWorkPtr();
    if (value != 0) {
        *(u32*)((s32)work + 8) |= 2;
    } else {
        *(u32*)((s32)work + 8) &= ~2;
    }
    return 2;
}

const char str_me_8041fd80[] = "me";
const f32 float_0_8041fd84 = 0.0f;
const f32 float_1_8041fd88 = 1.0f;
const f32 float_neg1_8041fd8c = -1.0f;
const f32 float_0p5_8041fd90 = 0.5f;
const f32 float_1000_8041fd94 = 1000.0f;
const f32 float_0p1_8041fd98 = 0.1f;
const f32 float_20_8041fd9c = 20.0f;
const f32 float_6p2832_8041fda0 = 6.2831855f;
const f32 float_360_8041fda4 = 360.0f;
const f32 float_3p1416_8041fda8 = 3.1415927f;
const f32 float_2_8041fdac = 2.0f;
const f32 float_180_8041fdb0 = 180.0f;
const f32 float_36_8041fdb4 = 36.0f;
const f32 float_18_8041fdb8 = 18.0f;
const f32 float_3_8041fdbc = 3.0f;
const f32 float_10_8041fdc0 = 10.0f;
const f32 float_8_8041fdc4 = 8.0f;
const f32 float_0p66667_8041fdc8 = 0.6666667f;
const char str_party_8041fdcc[] = "party";
const char str_mario_8041fdd4[] = "mario";
const f32 float_90_8041fddc = 90.0f;
const f32 float_5_8041fde0 = 5.0f;
const f32 float_100_8041fde4 = 100.0f;
const f32 float_4_8041fde8 = 4.0f;
const f32 float_15_8041fdec = 15.0f;
const f32 float_40_8041fdf0 = 40.0f;
const f32 float_1p5708_8041fdf4 = 1.5707964f;
const f32 float_60_8041fdf8 = 60.0f;
const f32 float_neg4_8041fdfc = -4.0f;
const f32 float_neg980_8041fe00 = -980.0f;
const f32 float_0p0005_8041fe04 = 0.0005f;
const char str_sPCTd_8041fe08[] = "_s%d";

s32 npc_init_evt[] = {
    0x0004005B,
    (s32)evt_npc_flag_onoff,
    1,
    (s32)(void*)str_me_8041fd80,
    0x600,
    2,
    1,
};


/* Target-owned conversion and sqrt constants: .rodata+0x40..+0x5f. */
__declspec(section ".rodata") const f64 double_to_int_802c14b0 __attribute__((aligned(8))) = 4503601774854144.0;
__declspec(section ".rodata") const f64 double_0p5_802c14b8 __attribute__((aligned(8))) = 0.5;
__declspec(section ".rodata") const f64 double_3_802c14c0 __attribute__((aligned(8))) = 3.0;
__declspec(section ".rodata") const f64 double_0_802c14c8 __attribute__((aligned(8))) = 0.0;

/* Recovered first evt_npc read-only string catalog; target .rodata+0x60..+0xBF. */
__declspec(section ".rodata") char str_kpa_01_802c14d0[] __attribute__((aligned(4))) = "kpa_01";
__declspec(section ".rodata") char str_p_bibi_802c14d8[] __attribute__((aligned(4))) = "p_bibi";
__declspec(section ".rodata") char str_PM_B_1_802c14e0[] __attribute__((aligned(4))) = "PM_B_1";
__declspec(section ".rodata") char str_PM_B_2_802c14e8[] __attribute__((aligned(4))) = "PM_B_2";
__declspec(section ".rodata") char str_x_party_802c14f0[] __attribute__((aligned(4))) = "(^x^)party";
__declspec(section ".rodata") char str_extparty_802c14fc[] __attribute__((aligned(4))) = "extparty";
__declspec(section ".rodata") char str_master_802c1508[] __attribute__((aligned(4))) = "master";
__declspec(section ".rodata") char str_slave_0_802c1510[] __attribute__((aligned(4))) = "slave_0";
__declspec(section ".rodata") char str_slave_1_802c1518[] __attribute__((aligned(4))) = "slave_1";
__declspec(section ".rodata") char str_slave_2_802c1520[] __attribute__((aligned(4))) = "slave_2";
__declspec(section ".rodata") char str_slave_3_802c1528[] __attribute__((aligned(4))) = "slave_3";

/* Target-owned unsigned conversion mask and double threshold: .rodata+0xc0..+0xcf. */
__declspec(section ".rodata") const f64 double_to_int_mask_802c1530 __attribute__((aligned(8))) = 4503599627370496.0;
__declspec(section ".rodata") const f64 double_0p1_802c1538 __attribute__((aligned(8))) = 0.1;
