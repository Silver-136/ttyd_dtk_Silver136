#include "driver/offscreendrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "sdk/DEMOInit.h"
#include "mario/mariost.h"
#include "memory.h"
#include <string.h>

//.sbss
static OffscreenWork work[2]; //field, battle

#define offscreenGetWork(inBattle) ((inBattle) ? &work[1] : &work[0])
#define offscreenGetWorkGp() (gp->inBattle ? &work[1] : &work[0])

//local prototypes
void offscreenDisp(CameraId camId, void* param);

void offscreenInit(void) {
    OffscreenWork* wp;

    work[0].count = 10;
    work[0].entries = __memAlloc(HEAP_DEFAULT, work[0].count * sizeof(OffscreenEntry));
    memset(work[0].entries, 0, work[0].count * sizeof(OffscreenEntry));
    wp = &work[1];
    wp->count = 10;
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(OffscreenEntry));
    memset(wp->entries, 0, wp->count * sizeof(OffscreenEntry));
}

void offscreenReset(BOOL inBattle) { //1:1
    OffscreenWork* wp = offscreenGetWork(inBattle);
    memset(wp->entries, 0, wp->count * sizeof(OffscreenEntry));
}

s32 offscreenEntry(const char* name) { //1:1, needs variable names
    OffscreenWork* wp = offscreenGetWorkGp();
    int i, count;
    OffscreenEntry* entry;

    //check if already created
    for (i = 0, count = wp->count, entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1 && !strcmp(entry->name, name)) {
            break;
        }
    }

    //find new slot
    for (i = 0, entry = wp->entries; i < count; i++, entry++) {
        if (!(entry->flags & 1)) break;
    }

    entry->flags = 1;
    strcpy(entry->name, name);
    entry->currLeft = 1020;
    entry->currTop = 1020;
    entry->currRight = 2;
    entry->currBottom = 2;
    entry->prevLeft = 1020;
    entry->prevTop = 2;
    entry->prevRight = 2;
    entry->prevBottom = 2;
    entry->copyFmt = 5;
    entry->texFmt = 5;
    return i;
}

void offscreenDisp(CameraId camId, void* param) {
    OffscreenWork* wp = offscreenGetWorkGp();
    int i, count;
    OffscreenEntry* entry;
    u16 width, height;

    for (i = 0, count = wp->count, entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1 && entry->flags & 2) {
            width = entry->currRight - entry->currLeft;
            height = entry->currBottom - entry->currTop;
            GXSetTexCopySrc(entry->currLeft, entry->currTop, width, height);
            GXSetTexCopyDst(width, height, (GXTexFmt)entry->copyFmt, GX_FALSE);
            entry->texBuf = smartAlloc(GXGetTexBufferSize(width, height, (u32)entry->copyFmt, GX_FALSE, 0), 3);
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
            GXCopyTex(entry->texBuf->address, GX_TRUE);
            GXPixModeSync();
            GXInitTexObj(&entry->texObj, entry->texBuf->address, width, height, (GXTexFmt)entry->texFmt,
                GX_CLAMP, GX_CLAMP, GX_FALSE);
            entry->flags |= 4;
        }
    }
}

void offscreenMain(void) { //1:1, need variable names
    OffscreenWork* wp = offscreenGetWorkGp();
    int i, count;
    OffscreenEntry* entry;

    for (i = 0, count = wp->count, entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1) {
            entry->flags &= ~2;
            entry->flags &= ~4;
            entry->prevLeft = entry->currLeft;
            entry->prevTop = entry->currTop;
            entry->prevRight = entry->currRight;
            entry->prevBottom = entry->currBottom;
            entry->currLeft = 1020;
            entry->currTop = 1020;
            entry->currRight = 2;
            entry->currBottom = 2;
        }
    }
    dispEntry(CAMERA_OFFSCREEN2, 0.0f, 7, offscreenDisp, NULL);
}

void offscreenAddBoundingBox(s32 id, u16 left, u16 top, u16 right, u16 bottom) { //1:1
    OffscreenEntry* entry;

    if (right > left) {
        entry = &offscreenGetWorkGp()->entries[id];
        if (entry->flags & 1) {
            if (left >= 2) {
                left -= 2;
            }
            if (top >= 2) {
                top -= 2;
            }
            if (right <= DEMOGetRenderModeObj()->fbWidth - 2) {
                right += 2;
            }
            if (bottom <= DEMOGetRenderModeObj()->efbHeight - 2) {
                bottom += 2;
            }
            left -= (left & 1);
            top -= (top & 1);
            right += (right & 1);
            bottom += (bottom & 1);
            if (left < entry->currLeft) {
                entry->currLeft = left;
                entry->flags |= 2;
            }
            if (top < entry->currTop) {
                entry->currTop = top;
                entry->flags |= 2;
            }
            if (right > entry->currRight) {
                entry->currRight = right;
                entry->flags |= 2;
            }
            if (bottom > entry->currBottom) {
                entry->currBottom = bottom;
                entry->flags |= 2;
            }
        }
    }
}

BOOL offscreenGetBoundingBox(s32 id, u16* left, u16* top, u16* right, u16* bottom) {
    OffscreenEntry* entry = &offscreenGetWorkGp()->entries[id];

    if (!(entry->flags & 1)) {
        return FALSE;
    }
    if (!(entry->flags & 2)) {
        return FALSE;
    }
    *left = entry->currLeft;
    *top = entry->currTop;
    *right = entry->currRight;
    *bottom = entry->currBottom;
    return TRUE;
}

void offscreenGetTexObj(s32 id, GXTexObj** texObj, smartEntry** texBuf) {
    OffscreenEntry* entry = &offscreenGetWorkGp()->entries[id];

    if (!(entry->flags & 1)) {
        *texObj = NULL;
        *texBuf = NULL;
        return;
    }
    if (!(entry->flags & 2)) {
        *texObj = NULL;
        *texBuf = NULL;
        return;
    }
    if (!(entry->flags & 4)) {
        *texObj = NULL;
        *texBuf = NULL;
        return;
    }
    *texObj = &entry->texObj;
    *texBuf = entry->texBuf;
}

s32 offscreenNameToId(const char* name) {
    OffscreenWork* wp = offscreenGetWorkGp();
    int i, count;
    OffscreenEntry* entry;

    for (i = 0, count = wp->count, entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1 && !strcmp(entry->name, name)) {
            break;
        }
    }
    return i;
}

OffscreenEntry* offscreenNameToPtr(const char* name) {
    OffscreenWork* wp = offscreenGetWorkGp();
    int i, count;
    OffscreenEntry* entry;

    for (i = 0, count = wp->count, entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1 && !strcmp(entry->name, name)) {
            break;
        }
    }
    return entry;
}

void offscreenDelete(const char* name) {
    OffscreenWork* wp = offscreenGetWorkGp();
    int i, count;
    OffscreenEntry* entry;

    for (i = 0, count = wp->count, entry = wp->entries; i < count; i++, entry++) {
        if (entry->flags & 1 && !strcmp(entry->name, name)) {
            break;
        }
    }
    entry->flags = 0;
}

BOOL offscreenGetBoundingBox_BeforeFrame(s32 id, u16* left, u16* top, u16* right, u16* bottom) {
    OffscreenEntry* entry = &offscreenGetWorkGp()->entries[id];

    if (!(entry->flags & 1)) {
        return FALSE;
    }
    if (entry->prevLeft > entry->prevRight) {
        return FALSE;
    }
    *left = entry->prevLeft;
    *top = entry->prevTop;
    *right = entry->prevRight;
    *bottom = entry->prevBottom;
    return TRUE;
}
