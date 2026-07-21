#include "event/evt_cloud.h"

extern u8 cloud[];
extern void* cloud_p;
extern u8 cloud_once_flag;
extern char str_i_name_PCT1d_802f73d8[];
extern char str_o_name_PCT1d_802f73e4[];
s32 sprintf(char* str, const char* fmt, ...);

s32 evt_cloud_main(void* pEvt, s32 firstCall) {
    extern void* gp;
    extern void* marioGetPtr(void);
    extern void* partyGetPtr(s32 id);
    extern void* cloudGetHitBreathout(s32 index);
    extern void* hitNameToPtr(char* name);
    extern void mapGrpFlagOff(char* name, s32 flags);
    extern void mapGrpFlagOn(char* name, s32 flags);
    extern s32 animGroupBaseAsync(char* name, s32 heap, s32 flags);
    extern void animPaperPoseEntry(char* name, s32 heap);
    extern void animPaperPoseRelease(s32 poseId);
    extern void offscreenEntry(char* name);
    extern void offscreenDelete(char* name);
    extern void mapGrpSetOffScreen(char* objectName, char* offscreenName);
    extern void mapGrpClearOffScreen(char* objectName);
    extern char* strcpy(char* dest, const char* src);
    extern void imgEntry(char* name, s32 heap);
    extern void* imgNameToPtr(char* name, s32 heap);
    extern s32 animPaperPoseGetId(char* name, s32 heap);
    extern f64 animTimeGetTime(s32 heap);
    extern void imgClearVirtualPoint(void* image);
    extern void imgSetShadow(void* image, s32 type);
    extern void imgFreeCapture(s32 image, s32 heap);
    extern void imgRelease(void* image);
    extern s32 evtEntry(void* script, s32 priority, s32 flags);
    extern void hitObjFlagOn(char* name, s32 flags);
    extern void psndSFXOn_3D(char* name, void* position);
    extern char str_P_oof_cloud_1_802f73c8[];
    extern char str_P_oof_cloud_2_802f73f0[];
    extern char str_SFX_OFF_CLAUD_WIND2_802f7400[];
    extern char str_SFX_OFF_CLAUD_WIND3_802f7414[];
    extern char str_A_1_80424050[];
    extern char str_A_2_80424058[];
    extern u8 cloud_once_flag;

    u8* cloud = *(u8**)((s32)pEvt + 0x9C);
    void* player = marioGetPtr();
    void* party = partyGetPtr(0);
    s32 heap = *(s32*)((s32)gp + 0x14) != 0;
    s32 state;
    s32 i;
    void* hit;
    void* image;
    char** group;

    if (firstCall != 0 || party == 0) {
        return 0;
    }

    state = *(s32*)(cloud + 4);
    if (state == 0) {
        if (cloud_once_flag != 1) {
            hit = hitNameToPtr((char*)cloud + 0x68);
            for (i = 0; i < 4; i++) {
                if (cloudGetHitBreathout(i) == hit) {
                    *(s32*)(cloud + 4) += 1;
                    break;
                }
            }
            *(s32*)(cloud + 0xAC) = 0;
        }
    } else if (state == 1) {
        if (*(f32*)((s32)player + 0x19C) <= 90.0f ||
            *(f32*)((s32)player + 0x19C) > 270.0f) {
            strcpy((char*)cloud + 8, str_P_oof_cloud_1_802f73c8);
        } else {
            strcpy((char*)cloud + 8, str_P_oof_cloud_2_802f73f0);
        }
        if (animGroupBaseAsync((char*)cloud + 8, heap, 0) != 0) {
            cloud_once_flag = 1;
            animPaperPoseEntry((char*)cloud + 8, heap);
            offscreenEntry((char*)cloud + 0x88);
            if ((*(u16*)cloud & 2) == 0) {
                mapGrpSetOffScreen((char*)cloud + 0x68, (char*)cloud + 0x88);
            } else {
                for (group = *(char***)(cloud + 0xBC); *group != 0; group++) {
                    mapGrpSetOffScreen(*group, (char*)cloud + 0x88);
                }
            }
            *(s32*)(cloud + 4) += 1;
        }
    } else if (state == 2) {
        imgEntry((char*)cloud + 0x28, heap);
        image = imgNameToPtr((char*)cloud + 0x28, heap);
        *(u32*)image |= 2;
        *(s32*)((s32)image + 0x10C) = animPaperPoseGetId((char*)cloud + 8, heap);
        *(f64*)((s32)image + 0x110) = animTimeGetTime(heap);
        imgClearVirtualPoint(image);
        imgSetShadow(image, 2);
        *(s32*)(cloud + 4) += 1;
    } else if (state == 3) {
        image = imgNameToPtr((char*)cloud + 0x28, heap);
        *(u32*)image &= ~2;
        *(char**)((s32)image + 0x108) = str_A_1_80424050;
        *(f64*)((s32)image + 0x110) = animTimeGetTime(heap);
        mapGrpFlagOn((char*)cloud + 0x68, 1);
        *(s32*)(cloud + 0xB8) = 30;
        *(s32*)(cloud + 4) += 1;
        *(f32*)(cloud + 0xC4) = 0.0f;
    } else if (state == 4) {
        image = imgNameToPtr((char*)cloud + 0x28, heap);
        if (*(f32*)(cloud + 0xC4) == 0.0f ||
            *(f32*)(cloud + 0xC4) + 1.0f < *(f32*)((s32)image + 0x118)) {
            psndSFXOn_3D(str_SFX_OFF_CLAUD_WIND2_802f7400,
                         (void*)((s32)player + 0x4C));
            *(f32*)(cloud + 0xC4) = *(f32*)((s32)image + 0x118);
        }
    } else if (state == 10) {
        mapGrpFlagOff((char*)cloud + 0x68, 1);
        *(s32*)(cloud + 4) = 99;
    } else if (state == 20) {
        psndSFXOn_3D(str_SFX_OFF_CLAUD_WIND3_802f7414,
                     (void*)((s32)player + 0x4C));
        if (*(void**)(cloud + 0xB4) != 0) {
            evtEntry(*(void**)(cloud + 0xB4), 0, 0);
        }
        image = imgNameToPtr((char*)cloud + 0x28, heap);
        *(char**)((s32)image + 0x108) = str_A_2_80424058;
        *(f64*)((s32)image + 0x110) = animTimeGetTime(heap);
        hitObjFlagOn((char*)cloud + 0x68, 1);
        *(s32*)(cloud + 0xB8) = 60;
        *(s32*)(cloud + 4) += 1;
    } else if (state == 21) {
        if (*(s32*)(cloud + 0xB8) == 0) {
            *(s32*)(cloud + 0xB8) = 30;
            *(s32*)(cloud + 0xAC) += 2;
            *(s32*)(cloud + 4) = 99;
        } else {
            *(s32*)(cloud + 0xB8) -= 1;
        }
    } else if (state == 99) {
        image = imgNameToPtr((char*)cloud + 0x28, heap);
        if ((*(u16*)cloud & 2) == 0) {
            mapGrpClearOffScreen((char*)cloud + 0x68);
        } else {
            for (group = *(char***)(cloud + 0xBC); *group != 0; group++) {
                mapGrpClearOffScreen(*group);
            }
        }
        animPaperPoseRelease(*(s32*)((s32)image + 0x10C));
        imgFreeCapture((s32)image, 0);
        imgSetShadow(image, 0);
        imgRelease(image);
        offscreenDelete((char*)cloud + 0x88);
        cloud_once_flag = 0;
        *(s32*)(cloud + 4) = 0;
    }
    return 0;
}

s32 evt_cloud_init(void) {
    char* iName;
    s32 zero;
    char* oName;
    u8* entry;
    s32 i;

    entry = cloud;
    iName = str_i_name_PCT1d_802f73d8;
    oName = str_o_name_PCT1d_802f73e4;
    i = 0;
    zero = 0;
    while (i < 0x10) {
        *(u16*)entry = zero;
        sprintf((char*)entry + 0x28, iName, i);
        sprintf((char*)entry + 0x48, oName, i);
        i++;
        entry += 0xC8;
    }
    cloud_p = NULL;
    cloud_once_flag = 0;
    return 2;
}

s32 evt_cloud_ent(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtEntry(void* script, s32 priority, s32 flags);
    extern void* evt_cloud_main_evt;
    extern char str_P_oof_cloud_1_802f73c8[];
    extern char* strncpy(char* dest, const char* src, unsigned long n);
    s32* args;
    s32 indirectName;
    s32 name;
    s32 outName;
    s32 mode;
    s32 i;
    u8* entry;

    args = *(s32**)((s32)pEvt + 0x18);
    indirectName = evtGetValue(pEvt, args[0]);
    name = evtGetValue(pEvt, args[1]);
    outName = evtGetValue(pEvt, args[2]);
    mode = evtGetValue(pEvt, args[3]);

    entry = cloud;
    for (i = 0; i < 0x10; i++, entry += 0xC8) {
        if ((*(u16*)entry & 1) == 0) {
            break;
        }
    }
    if (i >= 0x10) {
        return 2;
    }

    *(u16*)entry |= 1;
    strncpy((char*)entry + 8, str_P_oof_cloud_1_802f73c8, 0x20);
    if (indirectName == 0) {
        strncpy((char*)entry + 0x68, (char*)name, 0x20);
    } else {
        strncpy((char*)entry + 0x68, *(char**)name, 0x20);
    }
    strncpy((char*)entry + 0x88, (char*)outName, 0x20);
    *(s32*)(entry + 0xB4) = mode;
    *(s32*)(entry + 0xA8) = 0xB4;
    *(s32*)(entry + 0xAC) = 0;
    *(s32*)(entry + 4) = 0;
    *(s32*)(entry + 0xB8) = 0;
    *(u16*)(entry + 0xC0) = 0;
    *(u16*)(entry + 0xC2) = 0;
    *(s32*)(entry + 0xB0) = evtEntry(evt_cloud_main_evt, 0, 0);
    *(s32*)(*(s32*)(entry + 0xB0) + 0x9C) = (s32)entry;
    cloud_p = entry;
    if (indirectName != 0) {
        *(s32*)(entry + 0xBC) = name;
        *(u16*)entry |= 2;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_cloud_set_cap_size(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* cloud_p;
    s32* args = *(s32**)((s32)event + 0x18);
    u16 width = evtGetValue(event, args[0]);
    s32 height = evtGetValue(event, args[1]);

    *(u16*)((s32)cloud_p + 0xC0) = width;
    *(u16*)((s32)cloud_p + 0xC2) = height;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_cloud_get_mode(void* event, s32 isFirstCall) {
    extern void evtSetValue(void* event, s32 target, s32 value);
    extern void* cloud_p;
    s32* args = *(s32**)((s32)event + 0x18);
    void* cloud = cloud_p;

    if (cloud == 0) {
        evtSetValue(event, args[0], -1);
        return 2;
    }
    evtSetValue(event, args[0], *(s32*)((s32)cloud + 4));
    return 2;
}
