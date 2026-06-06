#include "driver/msgdrv.h"

void* msgWork;
extern void* gp;

void* windowGetPointer(s32 id);
void smartFree(void* ptr);
void* animGetPtr(void* anim);
void GXSetZMode(s32 enable, s32 func, s32 update);
void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
void GXSetZCompLoc(s32 beforeTex);
void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);

void* L_msgGetWork(void) {
    return &msgWork;
}

s32 _ismbblead(s32 c) {
    u32 value;

    if (*(void**)((s32)gp + 0x16C) != 0) {
        return 0;
    }

    value = (u8)c;
    if ((value >= 0x81 && value <= 0x9F) || (value >= 0xE0 && value <= 0xFC)) {
        return 1;
    }
    return 0;
}

void msgWindow_Continue(s32 id) {
    void* win = windowGetPointer(id);
    void* obj;
    void* work;
    s32 x;
    s32 y;

    *(u16*)win = 1;
    *(u16*)((s32)win + 2) |= 2;
    work = gp;
    obj = *(void**)((s32)win + 0x28);
    x = *(s32*)((s32)work + 0x38);
    y = *(s32*)((s32)work + 0x3C);
    obj = *(void**)obj;
    *(s32*)((s32)obj + 0x34) = y;
    *(s32*)((s32)obj + 0x30) = x;
}

void msgWindow_Delete(void* win) {
    void* ptr = *(void**)((s32)win + 0x28);

    if (ptr != 0) {
        s32 count = *(s32*)(*(s32*)ptr + 0x38);

        while (count > 0) {
            count--;
        }
        smartFree(ptr);
        *(void**)((s32)win + 0x28) = 0;
    }
}

void msgDispKeyWait_render(void* anim) {
    void* ptr = animGetPtr(anim);

    GXSetZMode(1, 7, 0);
    GXSetBlendMode(0, 4, 5, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    *(void**)((s32)ptr + 0xE0) = anim;
}


u8 msgDisp(void) {
    return 0;
}


s32 msgMain(int* param_1) {
    return 0;
}


u8 msgWindow_Disp(void) {
    return 0;
}


int msgWindow_Entry(char* param_1, s32 param_2, short param_3) {
    return 0;
}


u8 selectWindow_Main(s32 param_1) {
    return 0;
}


void msgLoad(char* name, s32 unk) {
}


u8 selectWindow_Disp(s32 param_1, int param_2) {
    return 0;
}


void* msgSearch(void* msgId) {
    return 0;
}


void msgWindow_ForceClose(s32 id) {
    extern s32 strcmp(const char* a, const char* b);
    extern void BtlUnit_ChangeStayAnim(void* unit);
    extern void npcSetStayPose(void* npc);
    s32 i;
    void* win;
    void* obj;

    for (i = 0; i < 7; i++) {
        win = windowGetPointer(i);
        if (*(u16*)((s32)win + 2) != 0 && *(u16*)win != 0 && *(void**)((s32)win + 0x28) != NULL) {
            obj = *(void**)(*(void**)((s32)win + 0x28));
            if (*(void**)obj != NULL && strcmp(*(char**)obj, (char*)id) == 0) {
                *(u16*)win = 7;
                obj = *(void**)(*(void**)((s32)win + 0x28));
                if (*(u32*)((s32)obj + 4) & 8) {
                    BtlUnit_ChangeStayAnim(*(void**)((s32)obj + 0xF224));
                } else {
                    npcSetStayPose((void*)((s32)obj + 0xF204));
                }
            }
        }
    }
}

u8 msgInit(void) {
    extern void* msgw;
    extern s32 animPoseEntry(const char* name, s32 type);
    extern void animPoseSetAnim(s32 pose, const char* name, s32 loop);
    extern void animPoseSetMaterialFlagOn(s32 pose, s32 flags);
    extern void animPoseSetMaterialFlagOff(s32 pose, s32 flags);
    extern void animPoseSetGXFunc(s32 pose, void* func, s32 param);
    extern const char R_no_messages_JP[];
    char* base = (char*)R_no_messages_JP;
    void* work;
    s32 pose;

    pose = animPoseEntry(base + 0x390, 2);
    *(s32*)((s32)msgw + 0x20) = pose;
    animPoseSetAnim(*(s32*)((s32)msgw + 0x20), base + 0x398, 1);
    animPoseSetMaterialFlagOn(*(s32*)((s32)msgw + 0x20), 0x1800);
    animPoseSetMaterialFlagOff(*(s32*)((s32)msgw + 0x20), 0x40);
    animPoseSetGXFunc(*(s32*)((s32)msgw + 0x20), msgDispKeyWait_render, 0);
    work = msgw;
    *(s32*)((s32)work + 4) = 0;
    *(s32*)((s32)msgw + 0xC) = 0;
    *(s32*)((s32)msgw + 0x14) = 0;
    *(s32*)((s32)msgw + 0x1C) = 0;
    msgLoad(base + 0x3A0, 1);
}

char* msgGetCommand(char* param_1, char* param_2, char* param_3) {
    char c;
    s32 mode = 0;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        c = *param_1;
        switch (c) {
            case '>':
                if (mode == 0) {
                    *param_2 = 0;
                } else {
                    *param_3 = 0;
                }
                return param_1 + 1;
            case ' ':
                if (mode == 0) {
                    *param_2 = 0;
                    mode = 1;
                } else {
                    *param_3++ = c;
                }
                break;
            default:
                if (mode == 0) {
                    *param_2++ = c;
                } else {
                    *param_3++ = c;
                }
                break;
        }
        param_1++;
    }
    return NULL;
}

s32 msgIconStr2ID(char* param_1) {
    typedef struct MsgIcon {
        const char* name;
        s32 id;
    } MsgIcon;
    extern MsgIcon msgIcon[];
    extern s32 strcmp(const char* a, const char* b);
    register MsgIcon* icon = msgIcon;
    u32 i;

    for (i = 0; i < 0x12; i++, icon++) {
        if (strcmp(icon->name, param_1) == 0) {
            return msgIcon[i].id;
        }
    }
    return -1;
}

u8 msgWindow_Repeat(int param_1) {
    return 0;
}


u8 msgWindow_Add(s32 param_1, int param_2) {
    return 0;
}


int msg_compare(int* param_1, int* param_2) {
    return 0;
}


u8 msgAnalize(void* param_1, s32 param_2) {
    return 0;
}


u8 msgWindow_Main(void* pWindow) {
    return 0;
}


u8 msgWindow_Clear_Main(void* param_1) {
    return 0;
}
