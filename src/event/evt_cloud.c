#include "event/evt_cloud.h"

extern u8 cloud[];
extern void* cloud_p;
extern u8 cloud_once_flag;
extern char str_i_name_PCT1d_802f73d8[];
extern char str_o_name_PCT1d_802f73e4[];
s32 sprintf(char* str, const char* fmt, ...);

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


s32 evt_cloud_main(void* pEvt,int firstCall){
    typedef struct Vec3{f32 x,y,z;}Vec3;extern void* gp;extern void* marioGetPtr(void);extern void* partyGetPtr(s32);extern void* cloudGetHitBreathout(s32);extern void* hitNameToPtr(char*);extern void mapGrpFlagOff(char*,s32);extern void mapObjGetPos(char*,Vec3*);extern s32 animGroupBaseAsync(char*,s32,s32);extern void animPaperPoseEntry(char*,s32);extern void offscreenEntry(char*);extern void mapGrpSetOffScreen(char*,char*);extern char* strcpy(char*,const char*);extern void imgEntry(char*,s32);extern void* imgNameToPtr(char*,s32);extern s32 animPaperPoseGetId(char*,s32);extern void imgClearVirtualPoint(void*);extern void imgSetShadow(void*,s32);extern char str_P_oof_cloud_1_802f73c8[],str_P_oof_cloud_2_802f73f0[];extern u8 cloud_once_flag;
    u8* cloud=*(u8**)((s32)pEvt+0x9C);void* player=marioGetPtr();void* party=partyGetPtr(0);s32 battle=*(s32*)((s32)gp+0x14)!=0; s32 state=*(s32*)(cloud+4),i;void* hit;Vec3 pos;
    if(firstCall==0&&party!=0){
        if(state==0&&cloud_once_flag!=1){hit=hitNameToPtr((char*)cloud+0x68);for(i=0;i<4;i++)if(cloudGetHitBreathout(i)==hit){*(s32*)(cloud+4)=1;break;}*(s32*)(cloud+0xAC)=0;}
        else if(state==1){if(*(f32*)((s32)player+0x19C)<=90.0f||*(f32*)((s32)player+0x19C)>270.0f)strcpy((char*)cloud+8,str_P_oof_cloud_1_802f73c8);else strcpy((char*)cloud+8,str_P_oof_cloud_2_802f73f0);if(animGroupBaseAsync((char*)cloud+8,battle,0)!=0){cloud_once_flag=1;animPaperPoseEntry((char*)cloud+8,battle);offscreenEntry((char*)cloud+0x88);mapGrpSetOffScreen((char*)cloud+0x68,(char*)cloud+0x88);*(s32*)(cloud+4)=2;}}
        else if(state==2){s32 width=*(u16*)(cloud+0xC0),height=*(u16*)(cloud+0xC2);mapObjGetPos((char*)cloud+0x68,&pos);if(width<0)width=0;if(height<0)height=0;imgEntry((char*)cloud+0x28,battle);hit=imgNameToPtr((char*)cloud+0x28,battle);*(u32*)hit|=2;*(s32*)((s32)hit+0x10C)=animPaperPoseGetId((char*)cloud+8,battle);imgClearVirtualPoint(hit);imgSetShadow(hit,2);*(s32*)(cloud+4)=3;}
        else if(state==10){mapGrpFlagOff((char*)cloud+0x68,1);*(s32*)(cloud+4)=99;}
    }
    return 0;
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
