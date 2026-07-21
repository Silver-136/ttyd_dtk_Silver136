#include "battle/battle_message.h"

extern void* _battleWorkPointer;
extern u32 dat_80424bb8;
extern f32 float_10_80424bbc;
extern f32 float_neg29_80424bc0;
extern f32 float_3_80424bc4;

s32 FontGetMessageWidthLine(char* msg, u16* lines);
void windowDispGX_Waku_col(s32 texture, void* color, f32 x, f32 y, f32 width, f32 height, f32 curve);
void FontDrawStart(void);
void FontDrawMessage(s32 x, s32 y, char* msg);


u32 btlevtcmd_AnnounceMessage(int param_1, int param_2) {
    extern s32 evtGetValue(void* evt, s32 arg);
    extern char* msgSearch(char* msg);
    extern int sprintf(char* str, const char* format, ...);
    extern char* strcpy(char* dst, const char* src);
    extern void unk_800d1364(char* dst, const char* src);
    extern s32 dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, s32 param);
    extern s32 BattlePadCheckTrigger(s32 buttons);
    extern void _disp(void);
    extern void* gp;
    extern u8 itemDataTable[];
    extern f32 float_900_80424bc8;
    s32* args;
    void* battleWork;
    void* work;
    s32 mode;
    s32 x;
    s32 y;
    s32 msgId;
    s32 timer;
    char* msg;
    char* itemName;

    args = *(s32**)(param_1 + 0x18);
    battleWork = _battleWorkPointer;
    work = (void*)((s32)battleWork + 0x20000);

    mode = evtGetValue((void*)param_1, args[0]);
    x = evtGetValue((void*)param_1, args[1]);
    y = evtGetValue((void*)param_1, args[2]);
    msgId = evtGetValue((void*)param_1, args[3]);
    timer = evtGetValue((void*)param_1, args[4]);

    if (msgId == 0) {
        return 2;
    }

    if (param_2 != 0) {
        *(s32*)(param_1 + 0x78) = timer;
        *(u16*)((s32)work - 0x7374) = x;
        *(u16*)((s32)work - 0x7372) = y;
        msg = msgSearch((char*)msgId);
        if (mode == 1) {
            if (*(u32*)((s32)gp + 0x16C) == 0) {
                itemName = msgSearch(*(char**)(itemDataTable + *(s32*)((s32)work - 0x7370) * 0x28 + 4));
                sprintf((char*)((s32)work - 0x7360), msg, itemName);
            } else {
                strcpy((char*)((s32)work - 0x7360), msg);
                itemName = msgSearch(*(char**)(itemDataTable + *(s32*)((s32)work - 0x7370) * 0x28 + 4));
                unk_800d1364((char*)((s32)work - 0x7360), itemName);
            }
        } else {
            sprintf((char*)((s32)work - 0x7360), msg);
        }
    }

    dispEntry(8, 1, _disp, float_900_80424bc8, 0);

    if (mode == 2) {
        if (BattlePadCheckTrigger(0x100) != 0 || BattlePadCheckTrigger(0x200) != 0) {
            *(s32*)(param_1 + 0x78) = 0;
        }
    }

    *(s32*)(param_1 + 0x78) = *(s32*)(param_1 + 0x78) - 1;
    return *(s32*)(param_1 + 0x78) == 0;
}

void _disp(void) {
    register void* work = (void*)((s32)_battleWorkPointer + 0x20000);
    u16 lines[2];
    u32 color;
    char* msg;
    s32 metrics;
    u16 lineCount;
    u16 width;
    f32 x;
    f32 y;

    msg = (char*)((s32)work - 0x7360);
    y = (f32)(*(s16*)((s32)work - 0x7372) + 0x78);
    x = (f32)*(s16*)((s32)work - 0x7374);
    metrics = FontGetMessageWidthLine(msg, lines);
    width = (u16)metrics;
    lineCount = lines[0] + 1;
    x -= (f32)(width / 2);
    lines[0] = lineCount;
    color = dat_80424bb8;
    windowDispGX_Waku_col(0, &color, x - float_10_80424bbc, float_neg29_80424bc0 + (float_3_80424bc4 + y),
                          width + 0x14, lineCount * 0x1D + 3, float_10_80424bbc);
    FontDrawStart();
    FontDrawMessage((s32)x, (s32)y - 0x1D, msg);
}

s32 btlevtcmd_AnnounceSetParam(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32* params = (s32*)((s32)battleWork + 0x18C8C);
    s32 index = evtGetValue(evt, args[0]);

    params[index + 1] = evtGetValue(evt, args[1]);
    return 2;
}
