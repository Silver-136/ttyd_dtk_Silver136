#include "event/evt_cloud.h"

extern u8 cloud[];
extern void* cloud_p;
extern u8 cloud_once_flag;
extern s32 evtEntry(void* script, s32 priority, s32 flags);
extern s32 evtGetValue(void* event, s32 value);
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
    extern char str_P_oof_cloud_1_802f73c8[];
    extern void imgEntry(char* name, s32 heap);
    extern void* imgNameToPtr(char* name, s32 heap);
    extern s32 animPaperPoseGetId(char* name, s32 heap);
    extern u64 animTimeGetTime(s32 heap);
    extern void imgClearVirtualPoint(void* image);
    extern void imgSetShadow(void* image, s32 type);
    extern void imgFreeCapture(s32 image, s32 heap);
    extern void imgRelease(void* image, s32 heap);
    extern void hitObjFlagOn(char* name, s32 flags);
    extern void psndSFXOn_3D(char* name, void* position);
    extern char str_P_oof_cloud_2_802f73f0[];
    extern char str_SFX_OFF_CLAUD_WIND2_802f7400[];
    extern char str_SFX_OFF_CLAUD_WIND3_802f7414[];
    extern const char str_A_1_80424050[4];
    extern const char str_A_2_80424058[4];
    extern const f64 double_to_int_mask_802f7428;
    extern const f64 double_to_int_802f7430;
    extern f32 float_90_80424040;
    extern f32 float_270_80424044;
    extern f32 float_1_80424048;
    extern f32 float_0p5_8042404c;
    extern f32 float_0_80424054;
    extern void mapObjGetPos(char* name, void* position);
    extern void* camGetPtr(s32 id);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTX44MultVec(void* mtx, void* src, void* dst);
    extern s32 evt_img_alloc_capture(void* event);

    u8* cloud = *(u8**)((s32)pEvt + 0x9C);
    void* player = marioGetPtr();
    void* party = partyGetPtr(0);
    s32 heap = *(s32*)((s32)gp + 0x14) != 0;
    s32 state;
    s32 i;
    void* hit;
    void* image;
    char** group;
    void* hit0;
    void* hit1;
    void* hit2;
    void* hit3;

    if (firstCall != 0 || party == 0) {
        return 0;
    }

    hit0 = cloudGetHitBreathout(0);
    hit1 = cloudGetHitBreathout(1);
    hit2 = cloudGetHitBreathout(2);
    hit3 = cloudGetHitBreathout(3);

    state = *(s32*)(cloud + 4);
    switch (state) {
    case 0: {
        if (cloud_once_flag != 1) {
            hit = hitNameToPtr((char*)cloud + 0x88);
            if (hit == hit0 || hit == hit1 || hit == hit2 || hit == hit3) {
                *(s32*)(cloud + 4) += 1;
            }
            *(s32*)(cloud + 0xAC) = 0;
        }
        break;
    }
    case 1: {
        if (*(f32*)((s32)player + 0x1B0) <= float_90_80424040 ||
            *(f32*)((s32)player + 0x1B0) > float_270_80424044) {
            strcpy((char*)cloud + 8, str_P_oof_cloud_1_802f73c8);
        } else {
            strcpy((char*)cloud + 8, str_P_oof_cloud_2_802f73f0);
        }
        if (animGroupBaseAsync((char*)cloud + 8, heap, 0) != 0) {
            cloud_once_flag = 1;
            animPaperPoseEntry((char*)cloud + 8, heap);
            offscreenEntry((char*)cloud + 0x48);
            if ((*(u16*)cloud & 2) == 0) {
                mapGrpSetOffScreen((char*)cloud + 0x68, (char*)cloud + 0x48);
            } else {
                for (group = *(char***)(cloud + 0xBC); *group != 0; group++) {
                    mapGrpSetOffScreen(*group, (char*)cloud + 0x48);
                }
            }
            *(s32*)(cloud + 4) += 1;
        }
        break;
    }
    case 2: {
        u8 captureEvent[0x1E0];
        s32 captureArgs[8];
        f32 position[3];
        s32 left;
        s32 top;
        s32 width;
        s32 height;
        union {
            f64 value;
            struct { u32 hi, lo; } words;
        } cvt;
        f32 screenWidth;
        f32 screenHeight;
        f32 halfWidth;
        f32 halfHeight;

        if (*(u16*)(cloud + 0xC0) != 0 && *(u16*)(cloud + 0xC2) != 0) {
            void* camera;
            mapObjGetPos((char*)cloud + 0x68, position);
            camera = camGetPtr(4);
            PSMTXMultVec((u8*)camera + 0x11C, position, position);
            camera = camGetPtr(4);
            PSMTX44MultVec((u8*)camera + 0x15C, position, position);
            width = *(u16*)(cloud + 0xC0);
            height = *(u16*)(cloud + 0xC2);
            cvt.words.hi = 0x43300000;
            cvt.words.lo = *(u16*)((u8*)gp + 0x170);
            screenWidth = (f32)(cvt.value - double_to_int_mask_802f7428);
            cvt.words.lo = *(u16*)((u8*)gp + 0x172);
            screenHeight = (f32)(cvt.value - double_to_int_mask_802f7428);
            position[0] = (float_1_80424048 + position[0]) * screenWidth * float_0p5_8042404c;
            position[1] = (float_1_80424048 - position[1]) * screenHeight * float_0p5_8042404c;
            cvt.words.lo = (width >> 1) ^ 0x80000000;
            halfWidth = (f32)(cvt.value - double_to_int_802f7430);
            cvt.words.lo = (height >> 1) ^ 0x80000000;
            halfHeight = (f32)(cvt.value - double_to_int_802f7430);
            left = (s32)(position[0] - halfWidth);
            top = (s32)(position[1] - halfHeight);
            if (left < 0) left = 0;
            if (top < 0) top = 0;
            if (width > *(u16*)((u8*)gp + 0x170) - left) {
                width = *(u16*)((u8*)gp + 0x170) - left;
            }
            if (height > *(u16*)((u8*)gp + 0x172) - top) {
                height = *(u16*)((u8*)gp + 0x172) - top;
            }
        } else {
            left = 0;
            top = 0;
            width = 0;
            height = 0;
        }
        imgEntry((char*)cloud + 0x28, heap);
        image = imgNameToPtr((char*)cloud + 0x28, heap);
        *(u32*)((s32)image + 0xCC) |= 2;
        *(s32*)((s32)image + 0x104) = animPaperPoseGetId((char*)cloud + 8, heap);
        *(u64*)((s32)image + 0x110) = animTimeGetTime(heap);
        captureArgs[0] = (s32)cloud + 0x28;
        captureArgs[1] = 0;
        captureArgs[2] = (s32)cloud + 0x48;
        captureArgs[3] = 1;
        captureArgs[4] = left;
        captureArgs[5] = top;
        captureArgs[6] = width;
        captureArgs[7] = height;
        *(s32**)(captureEvent + 0x18) = captureArgs;
        evt_img_alloc_capture(captureEvent);
        imgClearVirtualPoint(image);
        imgSetShadow(image, 2);
        *(s32*)(cloud + 4) += 1;
        break;
    }
    case 3: {
        image = imgNameToPtr((char*)cloud + 0x28, heap);
        *(u32*)((s32)image + 0xCC) &= ~2;
        *(char**)((s32)image + 0x108) = (char*)str_A_1_80424050;
        *(u64*)((s32)image + 0x110) = animTimeGetTime(heap);
        mapGrpFlagOn((char*)cloud + 0x68, 1);
        *(s32*)(cloud + 0xB8) = 30;
        *(s32*)(cloud + 4) += 1;
        *(f32*)(cloud + 0xC4) = float_0_80424054;
        break;
    }
    case 4: {
        image = imgNameToPtr((char*)cloud + 0x28, heap);
        if (*(f32*)(cloud + 0xC4) == float_0_80424054 ||
            *(f32*)(cloud + 0xC4) + float_1_80424048 < *(f32*)((s32)image + 0x118)) {
            psndSFXOn_3D(str_SFX_OFF_CLAUD_WIND2_802f7400,
                         (void*)((s32)player + 0x8C));
            *(f32*)(cloud + 0xC4) = *(f32*)((s32)image + 0x118);
        }
        if (*(u16*)((s32)player + 0x2E) == 0x1B) {
            hit = hitNameToPtr((char*)cloud + 0x88);
            if (hit == hit0 || hit == hit1 || hit == hit2 || hit == hit3) {
                *(s32*)(cloud + 0xB8) = 30;
                *(s32*)(cloud + 0xAC) += 2;
                if (*(s32*)(cloud + 0xAC) > *(s32*)(cloud + 0xA8)) {
                    *(s32*)(cloud + 4) = 20;
                }
            } else if (*(s32*)(cloud + 0xB8) == 0) {
                *(s32*)(cloud + 4) = 10;
            } else {
                *(s32*)(cloud + 0xB8) -= 1;
                *(s32*)(cloud + 0xAC) += 2;
            }
        } else {
            *(s32*)(cloud + 4) = 10;
        }
        break;
    }
    case 10: {
        mapGrpFlagOff((char*)cloud + 0x68, 1);
        *(s32*)(cloud + 4) = 99;
        break;
    }
    case 20: {
        psndSFXOn_3D(str_SFX_OFF_CLAUD_WIND3_802f7414,
                     (void*)((s32)player + 0x8C));
        if (*(void**)(cloud + 0xB4) != 0) {
            evtEntry(*(void**)(cloud + 0xB4), 0, 0);
        }
        image = imgNameToPtr((char*)cloud + 0x28, heap);
        *(char**)((s32)image + 0x108) = (char*)str_A_2_80424058;
        *(u64*)((s32)image + 0x110) = animTimeGetTime(heap);
        hitObjFlagOn((char*)cloud + 0x68, 1);
        *(s32*)(cloud + 0xB8) = 60;
        *(s32*)(cloud + 4) += 1;
        break;
    }
    case 21: {
        if (*(s32*)(cloud + 0xB8) == 0) {
            *(s32*)(cloud + 0xB8) = 30;
            *(s32*)(cloud + 0xAC) += 2;
            *(s32*)(cloud + 4) = 99;
        } else {
            *(s32*)(cloud + 0xB8) -= 1;
        }
        break;
    }
    case 99: {
        image = imgNameToPtr((char*)cloud + 0x28, heap);
        if ((*(u16*)cloud & 2) == 0) {
            mapGrpClearOffScreen((char*)cloud + 0x68);
        } else {
            for (group = *(char***)(cloud + 0xBC); *group != 0; group++) {
                mapGrpClearOffScreen(*group);
            }
        }
        animPaperPoseRelease(*(s32*)((s32)image + 0x104));
        imgFreeCapture((s32)image, 0);
        imgSetShadow(image, 0);
        imgRelease(image, heap);
        offscreenDelete((char*)cloud + 0x48);
        cloud_once_flag = 0;
        *(s32*)(cloud + 4) = 0;
        break;
    }
    }
    return 0;
}

s32 evt_cloud_init(void) {
    extern char str_i_name_PCT1d_802f73d8[];
    extern char str_o_name_PCT1d_802f73e4[];
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
    extern void* evt_cloud_main_evt;
    extern char str_P_oof_cloud_1_802f73c8[];
    extern char* strncpy(char* dest, const char* src, unsigned long n);
    s32* args;
    s32 indirectName;
    s32 name;
    s32 outName;
    s32 mode;
    s32 i;
    s32 count;
    u8* entry;
    u8* scan;

    args = *(s32**)((s32)pEvt + 0x18);
    indirectName = evtGetValue(pEvt, args[0]);
    name = evtGetValue(pEvt, args[1]);
    outName = evtGetValue(pEvt, args[2]);
    mode = evtGetValue(pEvt, args[3]);

    scan = cloud;
    count = 0;
    for (i = 0; i < 2; i++) {
        if ((*(u16*)scan & 1) == 0) {
            goto cloud_entry_found;
        }
        count++;
        scan += 0xC8;
        if ((*(u16*)scan & 1) == 0) goto cloud_entry_found;
        count++;
        scan += 0xC8;
        if ((*(u16*)scan & 1) == 0) goto cloud_entry_found;
        count++;
        scan += 0xC8;
        if ((*(u16*)scan & 1) == 0) goto cloud_entry_found;
        count++;
        scan += 0xC8;
        if ((*(u16*)scan & 1) == 0) goto cloud_entry_found;
        count++;
        scan += 0xC8;
        if ((*(u16*)scan & 1) == 0) goto cloud_entry_found;
        count++;
        scan += 0xC8;
        if ((*(u16*)scan & 1) == 0) goto cloud_entry_found;
        count++;
        scan += 0xC8;
        if ((*(u16*)scan & 1) == 0) goto cloud_entry_found;
        count++;
        scan += 0xC8;
    }
cloud_entry_found:
    if (count >= 0x10) {
        return 2;
    }
    entry = cloud + count * 0xC8;

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
    s32* args = *(s32**)((s32)event + 0x18);
    void* cloud = cloud_p;

    if (cloud == 0) {
        evtSetValue(event, args[0], -1);
        return 2;
    }
    evtSetValue(event, args[0], *(s32*)((s32)cloud + 4));
    return 2;
}

u8 cloud_once_flag;
void* cloud_p;
u8 cloud[0xC80];

const char str_P_oof_cloud_1_802f73c8[] = "P_oof_cloud_1";
const char str_i_name_PCT1d_802f73d8[] = "i_name_%1d";
const char str_o_name_PCT1d_802f73e4[] = "o_name_%1d";
const char str_P_oof_cloud_2_802f73f0[] = "P_oof_cloud_2";
const char str_SFX_OFF_CLAUD_WIND2_802f7400[] = "SFX_OFF_CLAUD_WIND2";
const char str_SFX_OFF_CLAUD_WIND3_802f7414[] = "SFX_OFF_CLAUD_WIND3";
const f64 double_to_int_mask_802f7428 = 4503599627370496.0;
const f64 double_to_int_802f7430 = 4503601774854144.0;

const f32 float_90_80424040 = 90.0f;
const f32 float_270_80424044 = 270.0f;
const f32 float_1_80424048 = 1.0f;
const f32 float_0p5_8042404c = 0.5f;
const char str_A_1_80424050[] = "A_1";
const f32 float_0_80424054 = 0.0f;
const char str_A_2_80424058[] = "A_2";

u32 evt_cloud_main_evt[4] = {
    0x0001005B,
    (u32)evt_cloud_main,
    2,
    1,
};
