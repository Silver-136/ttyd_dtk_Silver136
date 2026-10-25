#include "battle/battle.h"
#include "data/item_data.h"
#include "driver/dispdrv.h"
#include "driver/msgdrv.h"
#include "driver/windowdrv.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include <stdio.h>
#include "evt/evt_msg.h"
#ifdef VERSION_G8ME01
#include <string.h>
#include "mario/mariost.h"
#endif

extern ItemData itemDataTable[];

void _disp(CameraId cameraId, void* param);

// s32 type, s32 x, s32 y, const char* msg, s32 frames
USER_FUNC(btlevtcmd_AnnounceMessage) {
    BattleAnnounce* announce = &_battleWorkPointer->announce;
#ifdef VERSION_G8ME01
    const char* text;
#endif
    EvtWord* args = event->args;
    s32 type, x, y, frames;
    const char* msg;
#ifndef VERSION_G8ME01
    const char* text;
#endif

    type = evtGetValue(event, args[0]);
    x = evtGetValue(event, args[1]);
    y = evtGetValue(event, args[2]);
    msg = (const char*)evtGetValue(event, args[3]);
    frames = evtGetValue(event, args[4]);
    if (!msg) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        event->userdata[0] = frames;
        announce->x = x;
        announce->y = y;
        text = msgSearch(msg);
        switch (type) {
            case 1:
#ifdef VERSION_G8ME01
                if ((u32)gp->language == 0) {
                    sprintf(announce->text, text, msgSearch(itemDataTable[announce->params[0]].name));
                } else {
                    strcpy(announce->text, text);
                    evtMsgFmtStr(announce->text, msgSearch(itemDataTable[announce->params[0]].name));
                }
#else
                sprintf(announce->text, text, itemDataTable[announce->params[0]].id);
#endif
                break;
            default:
                sprintf(announce->text, text);
                break;
        }
    }
    dispEntry(CAMERA_2D, 900.0f, 1, _disp, NULL);
#ifdef VERSION_G8ME01
    if (type == 2) {
        if (BattlePadCheckTrigger(PAD_BUTTON_A) || BattlePadCheckTrigger(PAD_BUTTON_B)) {
            event->userdata[0] = 0;
        }
    }
#endif
    event->userdata[0]--;
    return event->userdata[0] <= 0;
}

void _disp(CameraId cameraId, void* param) {
    BattleAnnounce* announce = &_battleWorkPointer->announce;
    GXColor* col;
    u16 lines;
    u16 width;
    f32 x, y, w, h;

    x = announce->x;
    y = announce->y + 120;
    width = FontGetMessageWidthLine(announce->text, &lines);
    lines++;
    col = &(GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    w = (s32)width + 20;
    x -= (s32)width / 2;
    h = (s32)(lines * 29) + 3;
    ((void (*)(u16, GXColor*, f32, f32, f32, f32, f32))windowDispGX_Waku_col)(0, col, x - 10.0f, -29.0f + (3.0f + y), w, h, 10.0f);
    FontDrawStart();
    FontDrawMessage(x, (s32)y - 29, announce->text);
}

// s32 index, s32 value
USER_FUNC(btlevtcmd_AnnounceSetParam) {
    EvtWord* args = event->args;
    BattleAnnounce* announce = &_battleWorkPointer->announce;
    s32 index;

    index = evtGetValue(event, args[0]);
    announce->params[index] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}
