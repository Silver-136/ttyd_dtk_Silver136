#pragma once

#include "version.h"
#include <dolphin/os.h>
#include "driver/seqdrv.h"
#include "manager/filemgr.h"

typedef struct seq_e3_work {
    FileEntry* handle; // 0x00
    u32 curScreen;     // 0x04, offscreen page being drawn (copied from nextScreen)
    u32 nextScreen;    // 0x08, 1 after START
    s32 scenario;      // 0x0C
    f32 curX;          // 0x10
    f32 curY;          // 0x14
    f32 dstX;          // 0x18
    f32 dstY;          // 0x1C
    OSAlarm alarm;     // 0x20
    OSTime initTime;   // 0x48
    s32 poseIds[4];    // 0x50, early demos only: the four characters on the title screen
    struct EventEntry* animEvt; // 0x60, early demos only: event that animates them
    f32 poseRotY;   // 0x64, set to 270.0f in seq_e3Init; only the June US demo reads it (e3PoseSetRotY turns the characters drawn with it)
} seq_e3_work;

void seq_e3Init(SequenceWork* work);
void seq_e3Main(SequenceWork* work);
void seq_e3Exit(SequenceWork* work);
