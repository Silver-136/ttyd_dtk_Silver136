#include "event/evt_twoddisp.h"

void* twodPtr;

s32 twoddisp_main(void) {
    extern void twoddisp_disp(void);
    extern f32 float_1000_80422f88;
    extern void dispEntry(s32, s32, void*, s32, f32);
    s32 i = 0;

    for (; i < 96; i += 8) {
    }
    for (; i < 100; i++) {
    }
    dispEntry(8, 0, twoddisp_disp, 0, float_1000_80422f88);
    return 2;
}


s32 evt_twoddisp_init(int param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void twoddisp_disp(void) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void GXSetFog(s32 type, f32 start, f32 end, f32 nearZ, f32 farZ, void* color);
    extern void iconDispGx(f32 scale, void* pos, s32 flags, u16 iconId);
    extern void FontDrawStart(void);
    extern void FontDrawScale(f32 scale);
    extern int sprintf(char* str, char* fmt, ...);
    extern void FontDrawColor(void* color);
    extern void FontDrawString(f32 x, f32 y, char* str);
    extern f32 float_0_80422f80;
    extern f32 float_1_80422f84;
    extern u32 unk_804295f0;
    extern u32 unk_804295f4;
    extern u32 dat_80422f78;
    extern u32 dat_80422f7c;
    extern void* twodPtr;
    u32 color2;
    u32 color1;
    u32 fogColor2;
    u32 fogColor1;
    VecLocal pos;
    char buf[0x100];
    s32 offset;
    s32 i;
    void* entry;

    offset = 0;
    i = 0;
    do {
        entry = (void*)((s32)twodPtr + offset);
        if (*(s8*)entry != 0 && (*(u16*)((s32)entry + 0x10) & 1) != 0) {
            if ((*(u32*)((s32)entry + 0x24) & 0x80000000) == 0) {
                pos.x = *(f32*)((s32)entry + 0x1C) + *(f32*)((s32)entry + 0x14);
                pos.y = *(f32*)((s32)entry + 0x20) + *(f32*)((s32)entry + 0x18);
                pos.z = float_0_80422f80;
                fogColor1 = unk_804295f0;
                GXSetFog(0, float_0_80422f80, float_0_80422f80, float_0_80422f80, float_0_80422f80, &fogColor1);
                iconDispGx(*(f32*)((s32)entry + 0x28), &pos, 0x10, (u16)*(u32*)((s32)entry + 0x24));
            } else {
                FontDrawStart();
                FontDrawScale(*(f32*)((s32)entry + 0x28));
                fogColor2 = unk_804295f4;
                GXSetFog(0, float_0_80422f80, float_0_80422f80, float_0_80422f80, float_0_80422f80, &fogColor2);
                sprintf(buf, *(char**)((s32)entry + 0x24));
                color1 = dat_80422f78;
                FontDrawColor(&color1);
                FontDrawString(*(f32*)((s32)entry + 0x1C) + *(f32*)((s32)entry + 0x14),
                               *(f32*)((s32)entry + 0x20) + *(f32*)((s32)entry + 0x18),
                               buf);
                color2 = dat_80422f7c;
                FontDrawColor(&color2);
                FontDrawString(float_1_80422f84 + (*(f32*)((s32)entry + 0x1C) + *(f32*)((s32)entry + 0x14)),
                               (*(f32*)((s32)entry + 0x20) + *(f32*)((s32)entry + 0x18)) - float_1_80422f84,
                               buf);
            }
        }
        i++;
        offset += 0x34;
    } while (i < 100);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_twoddisp_flag_onoff(int param_1) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 strcmp(const char* s1, const char* s2);
    void* event = (void*)param_1;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 onoff = evtGetValue(event, args[0]);
    char* name = (char*)evtGetValue(event, args[1]);
    s32 i = 0;
    s32 offset = 0;
    void* entry;

    while (i < 100) {
        entry = (void*)((s32)twodPtr + offset);
        if (strcmp((char*)entry, name) == 0) {
            break;
        }
        i++;
        offset += 0x34;
    }
    if (i >= 100) {
        entry = 0;
    }

    if (onoff != 0) {
        *(u16*)((s32)entry + 0x10) |= evtGetValue(event, args[2]);
    } else {
        *(u16*)((s32)entry + 0x10) &= ~evtGetValue(event, args[2]);
    }
    return 2;
}


s32 evt_twoddisp_entry(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 strcmp(const char* s1, const char* s2);
    extern char* strcpy(char* dst, const char* src);
    extern f32 float_0_80422f80;
    extern f32 float_1_80422f84;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name = (char*)evtGetValue(pEvt, args[0]);
    s32 value = evtGetValue(pEvt, args[1]);
    s32 i = 0;
    s32 offset = 0;
    void* entry;

    while (i < 100) {
        entry = (void*)((s32)twodPtr + offset);
        strcmp((char*)entry, name);
        if (*(s8*)entry == 0) {
            strcpy((char*)entry, name);
            *(u16*)((s32)entry + 0x10) = 1;
            *(u16*)((s32)entry + 0x12) = 0;
            *(f32*)((s32)entry + 0x14) = float_0_80422f80;
            *(f32*)((s32)entry + 0x1C) = float_0_80422f80;
            *(f32*)((s32)entry + 0x18) = float_0_80422f80;
            *(f32*)((s32)entry + 0x20) = float_0_80422f80;
            *(s32*)((s32)entry + 0x24) = value;
            *(f32*)((s32)entry + 0x28) = float_1_80422f84;
            break;
        }
        i++;
        offset += 0x34;
    }
    return 2;
}


s32 evt_twoddisp_set_base_position(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern s32 strcmp(const char* s1, const char* s2);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name = (char*)evtGetValue(pEvt, args[0]);
    s32 i = 0;
    s32 offset = 0;
    void* entry;

    while (i < 100) {
        entry = (void*)((s32)twodPtr + offset);
        if (strcmp((char*)entry, name) == 0) {
            break;
        }
        i++;
        offset += 0x34;
    }
    if (i >= 100) {
        entry = 0;
    }
    *(f32*)((s32)entry + 0x1C) = evtGetFloat(pEvt, args[1]);
    *(f32*)((s32)entry + 0x20) = evtGetFloat(pEvt, args[2]);
    return 2;
}


s32 evt_twoddisp_set_position(int param_1) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern s32 strcmp(const char* s1, const char* s2);
    void* event = (void*)param_1;
    s32* args = *(s32**)((s32)event + 0x18);
    char* name = (char*)evtGetValue(event, args[0]);
    s32 i = 0;
    s32 offset = 0;
    void* entry;

    while (i < 100) {
        entry = (void*)((s32)twodPtr + offset);
        if (strcmp((char*)entry, name) == 0) {
            break;
        }
        i++;
        offset += 0x34;
    }
    if (i >= 100) {
        entry = 0;
    }
    *(f32*)((s32)entry + 0x14) = evtGetFloat(event, args[1]);
    *(f32*)((s32)entry + 0x18) = evtGetFloat(event, args[2]);
    return 2;
}
