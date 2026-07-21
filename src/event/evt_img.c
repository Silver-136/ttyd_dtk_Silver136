#include "event/evt_img.h"

s32 evt_img_entry(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void imgEntry(s32 name, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    imgEntry(name, ((u32)(-flag) | (u32)flag) >> 31);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_img_set_position(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    s32 flag = *(s32*)((s32)gp + 0x14);
    void* img = imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31);

    *(volatile f32*)((s32)img + 0xE8) = x;
    *(volatile f32*)((s32)img + 0xEC) = y;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_img_set_paper(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern s32 animPaperPoseGetId(s32 paper, s32 flag);
    extern s64 animTimeGetTime(s32 flag);
    extern void animPoseSetMaterialFlagOn(void* pose, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 paper = evtGetValue(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);
    s32 localFlag = ((u32)(-flag) | (u32)flag) >> 31;
    void* img = imgNameToPtr(name, localFlag);
    s64 time;

    *(s32*)((s32)img + 0x104) = animPaperPoseGetId(paper, localFlag);
    time = animTimeGetTime(localFlag);
    *(s32*)((s32)img + 0x114) = (s32)time;
    *(s32*)((s32)img + 0x110) = (s32)(time >> 32);
    animPoseSetMaterialFlagOn(*(void**)((s32)img + 0x104), 0x1800);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_img_set_paper_anim(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern s64 animTimeGetTime(s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 anim = evtGetValue(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);
    s32 localFlag = ((u32)(-flag) | (u32)flag) >> 31;
    void* img = imgNameToPtr(name, localFlag);
    s64 time;

    *(s32*)((s32)img + 0x108) = anim;
    time = animTimeGetTime(localFlag);
    *(s32*)((s32)img + 0x114) = (s32)time;
    *(s32*)((s32)img + 0x110) = (s32)(time >> 32);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_img_alloc_capture(void* evt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern void* imgNameToPtr(s32 name, s32 inBattle);
    extern s32 offscreenNameToId(s32 name);
    extern void* DEMOGetRenderModeObj(void);
    extern void* gp;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    s32 slot = evtGetValue(evt, args[1]);
    s32 offscreenName = evtGetValue(evt, args[2]);
    s32 flags = evtGetValue(evt, args[3]);
    s32 left = evtGetValue(evt, args[4]);
    s32 top = evtGetValue(evt, args[5]);
    s32 width = evtGetValue(evt, args[6]);
    s32 height = evtGetValue(evt, args[7]);
    s32 inBattle = *(s32*)((s32)gp + 0x14);
    void* image;
    u32* capture;
    s32 offscreenId;
    void* renderMode;

    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    image = imgNameToPtr(name, inBattle);
    capture = (u32*)((s32)image + slot * 0x44);
    offscreenId = offscreenName == 0 ? -1 : offscreenNameToId(offscreenName);
    if ((*capture & 4) != 0) {
        return 2;
    }

    width += width & 1;
    height += height & 1;
    left -= left & 1;
    top -= top & 1;
    if ((slot == 0 || (*(u32*)image & 4) == 0 ||
         (*(u16*)((s32)image + 0xE4) == (u16)width && *(u16*)((s32)image + 0xE6) == (u16)height)) &&
        (slot == 1 || (*(u32*)((s32)image + 0x44) & 4) == 0 ||
         (*(u16*)((s32)image + 0xE4) == (u16)width && *(u16*)((s32)image + 0xE6) == (u16)height)) &&
        (slot == 2 || (*(u32*)((s32)image + 0x88) & 4) == 0 ||
         (*(u16*)((s32)image + 0xE4) == (u16)width && *(u16*)((s32)image + 0xE6) == (u16)height))) {
        if (left == 0 && top == 0 && width == 0 && height == 0 && offscreenId != -1) {
            *(u16*)((s32)image + 0xE4) = 0;
            *(u16*)((s32)image + 0xE6) = 0;
            *(u16*)(capture + 1) = 0;
            *(u16*)((s32)capture + 6) = 0;
            if (slot == 2) {
                capture[5] = 0x27;
                capture[6] = 1;
            } else {
                capture[5] = 6;
                capture[6] = 6;
            }
            *(u32*)image |= 4;
        } else {
            if (left < 0 || top < 0 || width < 1 || height < 1) {
                return 2;
            }
            renderMode = DEMOGetRenderModeObj();
            if ((s32)*(u16*)((s32)renderMode + 4) < left + width) {
                return 2;
            }
            renderMode = DEMOGetRenderModeObj();
            if ((s32)*(u16*)((s32)renderMode + 6) < top + height) {
                return 2;
            }
            *(u16*)((s32)image + 0xE4) = (u16)width;
            *(u16*)((s32)image + 0xE6) = (u16)height;
            *(u16*)(capture + 1) = (u16)left;
            *(u16*)((s32)capture + 6) = (u16)top;
            if (slot == 2) {
                capture[5] = 0x27;
                capture[6] = 1;
            } else {
                capture[5] = 6;
                capture[6] = 6;
            }
        }
        capture[4] = offscreenId;
        *capture = 0x14;
        capture[0x10] = flags;
        if (flags != 0) {
            *capture |= 0x100;
        }
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_img_free_capture(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void imgFreeCapture(void* img, s32 capture);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 capture = evtGetValue(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    imgFreeCapture(imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31), capture);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_img_onoff(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 onoff = evtGetValue(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);
    void* img = imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31);

    if (onoff != 0) {
        *(u32*)((s32)img + 0xCC) &= ~2;
    } else {
        *(u32*)((s32)img + 0xCC) |= 2;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_img_set_virtual_point(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void imgSetVirtualPoint(void* img, void* vec);
    extern s32 vec3_802c2a88[3];
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    s32 flag = *(s32*)((s32)gp + 0x14);
    void* img = imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31);
    s32 vec[3];
    s32 out[3];

    vec[0] = vec3_802c2a88[0];
    vec[1] = vec3_802c2a88[1];
    vec[2] = vec3_802c2a88[2];
    *(f32*)&vec[0] = x;
    *(f32*)&vec[1] = y;
    *(f32*)&vec[2] = z;
    out[0] = vec[0];
    out[1] = vec[1];
    out[2] = vec[2];
    imgSetVirtualPoint(img, out);
    return 2;
}

s32 evt_img_clear_virtual_point(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void imgClearVirtualPoint(void* img);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);

    imgClearVirtualPoint(imgNameToPtr(evtGetValue(event, args[0]), ((u32)(-*(s32*)((s32)gp + 0x14)) | (u32)*(s32*)((s32)gp + 0x14)) >> 31));
    return 2;
}
s32 evt_img_release(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void imgRelease(void* img, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    void* img = imgNameToPtr(evtGetValue(event, args[0]), ((u32)(-*(s32*)((s32)gp + 0x14)) | (u32)*(s32*)((s32)gp + 0x14)) >> 31);
    imgRelease(img, ((u32)(-*(s32*)((s32)gp + 0x14)) | (u32)*(s32*)((s32)gp + 0x14)) >> 31);
    return 2;
}
s32 evt_img_wait_animend(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void* gp;
    extern f32 float_1_80420360;
    s32* args = *(s32**)((s32)event + 0x18);
    void* img = imgNameToPtr(evtGetValue(event, args[0]), ((u32)(-*(s32*)((s32)gp + 0x14)) | (u32)*(s32*)((s32)gp + 0x14)) >> 31);

    if (isFirstCall != 0) {
        return 0;
    }
    if (*(f32*)((s32)img + 0x118) < float_1_80420360) {
        return 0;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_img_set_shadow(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void imgSetShadow(void* img, s32 shadow);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 shadow = evtGetValue(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    imgSetShadow(imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31), shadow);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_img_set_paper_timerate(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void animPoseSetLocalTimeRate(void* pose, f32 rate);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    f32 rate = evtGetFloat(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    animPoseSetLocalTimeRate(*(void**)((s32)imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31) + 0x104), rate);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_img_set_color(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void animPoseSetMaterialFlagOn(void* pose, s32 flag);
    extern void animPoseSetMaterialEvtColor(void* pose, void* color);
    extern u32 unk_80429570;
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    u32 r = evtGetValue(event, args[1]);
    u32 g = evtGetValue(event, args[2]);
    u32 b = evtGetValue(event, args[3]);
    u32 a = evtGetValue(event, args[4]);
    s32 flag = *(s32*)((s32)gp + 0x14);
    void* img = imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31);
    u32 color = unk_80429570;
    u32 out;

    animPoseSetMaterialFlagOn(*(void**)((s32)img + 0x104), 0x40);
    ((unsigned char*)&color)[0] = r;
    ((unsigned char*)&color)[1] = g;
    ((unsigned char*)&color)[2] = b;
    ((unsigned char*)&color)[3] = a;
    out = color;
    animPoseSetMaterialEvtColor(*(void**)((s32)img + 0x104), &out);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 N_evt_img_set_z(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    f32 z = evtGetFloat(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);
    void* img = imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31);

    *(f32*)((s32)img + 0xFC) = z;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
