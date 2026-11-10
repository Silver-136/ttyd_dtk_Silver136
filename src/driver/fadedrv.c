// Fade driver: screen transitions (iris, spot and colour fades, curtains, pipe and page-flip animations,
// story-book frames) and the tec/soft-focus/white-out overlays.

#include "version.h"
#include "driver/fadedrv.h"
#include "driver/animdrv.h"
#include "driver/arcdrv.h"
#include "driver/camdrv.h"
#include "driver/imgdrv.h"
#include "driver/dispdrv.h"
#include "driver/offscreendrv.h"
#include "manager/dvdmgr.h"
#include "manager/evtmgr.h"
#include "pmario_sound.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/dvd.h>
#include <string.h>
#include "libc_decls.h"
#include "evt/evt_img.h"
#include "driver/mapdrv.h"

#if VERSION >= VERSION_US
// US and later: named constant (dispdrv reads it by name), followed by a zero word
#pragma force_active on
const f32 float_1_8041f770 = 1.0f;
const f32 float_0_8041f774 = 0.0f;
#pragma force_active reset
#endif

static FadeWork* wp;

// local prototypes
static void disp_texture(CameraId camId, void* param);
static void disp_maku(CameraId camId, void* param);
static void disp_tec(CameraId camId, void* param);
static void disp_dummy_black(CameraId camId, void* param);

/**
 * DVD read callback for the asynchronous fade.tpl load:
 * unpacks the palette, closes the file and marks the fade textures ready.
 */
static void _callback(s32 error, DVDFileInfo* info) {
    DVDEntry* entry = info->cb.userData;
    UnpackTexPalette(wp->texture);
    DVDMgrClose(entry);
    wp->loaded = TRUE;
}

/** Boot setup: allocates the fade work and resets its five fade slots (no animation poses, centred spot, scale 1). */
void fadeInit(void) {
    FadeEntry* entry;
    int i;

    wp = __memAlloc(HEAP_DEFAULT, sizeof(FadeWork));
    memset(wp, 0, sizeof(FadeWork));

    for (i = 0; i < 5; i++) {
        entry = &wp->entries[i];
        entry->poseId = -1;
        entry->paperPoseId = -1;
        entry->fadeSpotX = 0.0f;
        entry->fadeSpotY = 0.0f;
        entry->animOffsetX = 0.0f;
        entry->animOffsetY = 0.0f;
        entry->scale = 1.0f;
#if VERSION >= VERSION_JP
        entry->delayFrames = 0;
#endif
    }

    wp->lastEntryId = -1;
    wp->texture = NULL;
    wp->loaded = FALSE;
}

/**
 * Loads fade.tpl, the iris, spot and overlay textures:
 * from the startup archive if it holds it, else asynchronously from the disc.
 * Called from the logo sequence at boot.
 */
void fadeTexSetup(void) {
    DVDEntry* dvd;
    u32 size;
    char path[128];

    wp->texture = NULL;
    wp->texture = arcOpen("fade.tpl", NULL, NULL);
    if (!wp->texture) {
        sprintf(path, "%s/fade.tpl", getMarioStDvdRoot());
        dvd = DVDMgrOpen(path, 2, 0);
        size = OSRoundUp32B(DVDMgrGetLength(dvd));
        wp->texture = __memAlloc(HEAP_DEFAULT, size);
        dvd->info.cb.userData = dvd;
        DVDMgrReadAsync(dvd, wp->texture, size, 0, _callback);
    } else {
        UnpackTexPalette(wp->texture);
        wp->loaded = TRUE;
    }
}

/**
 * Starts screen transition type over duration milliseconds in color.
 * The white fades become the colour fades in light grey; iris and spot fades last at least one second
 * (except JP and later's FADE_IN_SPOT2/FADE_OUT_SPOT2, which become spot fades without that minimum).
 * Slot 0 takes iris, spot and colour fades, slot 1 the curtain and stage animations (and the four-page flip),
 * slot 2 the pipe transitions, slot 3 the page flips
 * and the story-book frames (Mario, Peach, Bowser), slot 4 the two curtain-sides _TOP fades,
 * each with the animation file it plays.
 */
void fadeEntry(s32 type, s32 duration, GXColor color) {
    FadeEntry* entry;
    s32 id;

    if (!type) {
        return;
    }

    // types 16-21 are types 9, 10, 11, 12, 14 and 15 in near-white (0xF0F0F0) (JP and later numbers)
    if (type == FADE_IN_WHITE) {
        type = FADE_IN_BLACK;
        color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
    }
    if (type == FADE_OUT_WHITE) {
        type = FADE_OUT_BLACK;
        color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
    }
    if (type == FADE_IN_WHITE_2D) {
        type = FADE_IN_BLACK_2D;
        color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
    }
    if (type == FADE_OUT_WHITE_2D) {
        type = FADE_OUT_BLACK_2D;
        color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
    }
    if (type == FADE_IN_WHITE_3D) {
        type = FADE_IN_BLACK_3D;
        color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
    }
    if (type == FADE_OUT_WHITE_3D) {
        type = FADE_OUT_BLACK_3D;
        color = (GXColor){0xF0, 0xF0, 0xF0, 0xFF};
    }
    if ((type == FADE_IN_IRIS || type == FADE_OUT_IRIS) && duration < 1000) {
        duration = 1000;
    }
#if VERSION <= VERSION_DEMO_JP
    if ((type == 3 || type == 4 || type == 5 || type == 6) && duration < 1000) {
#else
    if (type == 7) {
        type = 3;
    } else if (type == 8) {
        type = 4;
    } else if ((type == 3 || type == 4 || type == 5 || type == 6) && duration < 1000) {
#endif
        duration = 1000;
    }

    // no default: a type without a case here would go on with entry and id uninitialised
    switch (type) {
#if VERSION <= VERSION_DEMO_JP
        case FADE_IN_IRIS:
        case FADE_OUT_IRIS:
        case FADE_IN_SPOT:
        case FADE_OUT_SPOT:
        case FADE_IN_SPOT_3D:
        case FADE_OUT_SPOT_3D:
        case FADE_IN_BLACK:
        case FADE_OUT_BLACK:
        case FADE_IN_BLACK_2D:
        case FADE_OUT_BLACK_2D:
        case FADE_COVER_BLACK_2D:
        case FADE_IN_BLACK_3D:
        case FADE_OUT_BLACK_3D:
#else
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF:
#endif
            id = 0;
            entry = &wp->entries[0];
            fadeReset(2);
#if VERSION >= VERSION_JP
            entry->delayFrames = 0;
            switch (type) {
                case 1:
                case 3:
                case 5:
                case 9:
                case 11:
                case 14:
                    if (duration) {
                        entry->delayFrames = 2;
                    }
                    break;
            }
            break;
        case FADE_CURTAIN_UP_DOWN_A1:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
        case 0x28:
        case 0x33:
#else
                    break;
            case FADE_CURTAIN_UP_DOWN_A1:
            case FADE_CURTAIN_VERTICAL_A2:
            case FADE_CURTAIN_SIDES_A1_3D:
            case FADE_CURTAIN_SIDES_A2_3D:
            case FADE_CURTAIN_SIDES_Z1_3D:
            case FADE_CURTAIN_SIDES_A1:
            case FADE_CURTAIN_SIDES_A2:
            case FADE_CURTAIN_CENTER_A1_3D:
            case FADE_CURTAIN_CENTER_A2_3D:
            case FADE_CURTAIN_CENTER_Z2_3D:
            case FADE_CURTAIN_CENTER_Z1_3D:
            case FADE_CURTAIN_LEAVE_3D:
            case FADE_CURTAIN_ENTER_3D:
            case FADE_CURTAIN_CENTER_A1:
            case FADE_CURTAIN_CENTER_A2:
            case FADE_CURTAIN_CENTER_Z2:
            case FADE_CURTAIN_CENTER_Z1:
            case FADE_CURTAIN_LEAVE:
            case FADE_CURTAIN_ENTER:
            case FADE_PAGE_FLIP_FOUR:
#endif
                id = 1;
                entry = &wp->entries[1];
                break;
#if VERSION <= VERSION_DEMO_JP
        case FADE_PIPE_UP_A1:
        case FADE_PIPE_UP_A2:
        case FADE_PIPE_DOWN_A1:
        case FADE_PIPE_DOWN_A2:
        case FADE_PIPE_LEFT_A1:
        case FADE_PIPE_LEFT_A2:
        case FADE_PIPE_RIGHT_A1:
        case FADE_PIPE_RIGHT_A2:
#else
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
        case 0x30:
#endif
            id = 2;
            entry = &wp->entries[2];
            fadeReset(0);
            break;
#if VERSION <= VERSION_DEMO_JP
        case FADE_PAGE_FLIP_Z1:
        case FADE_PAGE_FLIP_A1:
        case FADE_PAGE_FLIP_Z1_OFFSCREEN:
        case FADE_PAGE_FLIP_A1_OFFSCREEN:
        case FADE_STORY_MARIO_A3:
        case FADE_STORY_MARIO_A1:
        case FADE_STORY_PEACH_A3:
        case FADE_STORY_PEACH_A1:
        case FADE_STORY_BOWSER_A3:
        case FADE_STORY_BOWSER_A1:
        case FADE_STORY_MARIO_A2:
        case FADE_STORY_PEACH_A2:
        case FADE_STORY_BOWSER_A2:
#else
        case 0x31:
        case 0x32:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3E:
#endif
            id = 3;
            entry = &wp->entries[3];
            switch (type) {
#if VERSION <= VERSION_DEMO_JP
                case FADE_PAGE_FLIP_A1:
                case FADE_PAGE_FLIP_A1_OFFSCREEN:
                case FADE_STORY_MARIO_A3:
                case FADE_STORY_PEACH_A3:
                case FADE_STORY_BOWSER_A3:
#else
                case 0x32:
                case 0x35:
                case 0x36:
                case 0x38:
                case 0x3A:
#endif
                    fadeReset(0);
                    break;
            }
            break;
#if VERSION <= VERSION_DEMO_JP
        case FADE_CURTAIN_SIDES_A1_TOP:
        case FADE_CURTAIN_SIDES_A2_TOP:
#else
        case 0x3F:
        case 0x40:
#endif
            id = 4;
            entry = &wp->entries[4];
            break;
    }
    entry->flags = 0;
    entry->flags |= FADE_FLAG_ACTIVE;
    entry->type = type;
    entry->duration = duration;
    entry->color = color;
    entry->progress = 0.0f;
    entry->rotation = 0.0f;
    entry->elapsed = 0.0f;
    entry->state = 0;
    entry->animOffsetX = 0.0f;
    entry->animOffsetY = 0.0f;
    entry->scale = 1.0f;
    if (type != FADE_PAGE_FLIP_A1 && type != FADE_PAGE_FLIP_A1_OFFSCREEN) {
        entry->callback = NULL;
    }
    wp->lastEntryId = id;
    if (type == FADE_CURTAIN_ENTER_3D || type == FADE_CURTAIN_ENTER) {
        entry->scale = 3.0f;
    }
    switch (type) {
        case FADE_PIPE_DOWN_A1:
            entry->animFile = "OFF_d_dokan_down";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_PIPE_DOWN_A2:
            entry->animFile = "OFF_d_dokan_down";
            entry->animName = "A_2";
            entry->imageType = 2;
            break;
        case FADE_PIPE_UP_A1:
            entry->animFile = "OFF_d_dokan_up";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_PIPE_UP_A2:
            entry->animFile = "OFF_d_dokan_up";
            entry->animName = "A_2";
            entry->imageType = 2;
            break;
        case FADE_PIPE_LEFT_A1:
            entry->animFile = "OFF_d_dokan_left";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_PIPE_LEFT_A2:
            entry->animFile = "OFF_d_dokan_left";
            entry->animName = "A_2";
            entry->imageType = 2;
            break;
        case FADE_PIPE_RIGHT_A1:
            entry->animFile = "OFF_d_dokan_right";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_PIPE_RIGHT_A2:
            entry->animFile = "OFF_d_dokan_right";
            entry->animName = "A_2";
            entry->imageType = 2;
            break;
        case FADE_CURTAIN_UP_DOWN_A1:
            entry->animFile = "OFF_d_maku_up_down";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_CURTAIN_VERTICAL_A2:
            entry->animFile = "OFF_d_maku_up_down";
            entry->animName = "A_2";
            entry->imageType = 2;
            break;
        case FADE_CURTAIN_SIDES_A1_3D:
        case FADE_CURTAIN_SIDES_A1:
        case FADE_CURTAIN_SIDES_A1_TOP:
            entry->animFile = "OFF_d_maku_left_right";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_CURTAIN_SIDES_Z1_3D:
            entry->animFile = "OFF_d_maku_left_right";
            entry->animName = "Z_1";
            entry->imageType = 2;
            break;
        case FADE_CURTAIN_SIDES_A2_3D:
        case FADE_CURTAIN_SIDES_A2:
        case FADE_CURTAIN_SIDES_A2_TOP:
            entry->animFile = "OFF_d_maku_left_right";
            entry->animName = "A_2";
            entry->imageType = 2;
            break;
        case FADE_CURTAIN_CENTER_A1_3D:
        case FADE_CURTAIN_CENTER_A1:
            entry->animFile = "OFF_d_maku_center";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_CURTAIN_CENTER_A2_3D:
        case FADE_CURTAIN_CENTER_A2:
            entry->animFile = "OFF_d_maku_center";
            entry->animName = "A_2";
            entry->imageType = 2;
            break;
        case FADE_CURTAIN_CENTER_Z2_3D:
        case FADE_CURTAIN_ENTER_3D:
        case FADE_CURTAIN_CENTER_Z2:
        case FADE_CURTAIN_ENTER:
            entry->animFile = "OFF_d_maku_center";
            entry->animName = "Z_2";
            entry->imageType = 2;
            break;
        case FADE_CURTAIN_CENTER_Z1_3D:
        case FADE_CURTAIN_CENTER_Z1:
            entry->animFile = "OFF_d_maku_center";
            entry->animName = "Z_1";
            entry->imageType = 2;
            break;
        case FADE_PAGE_FLIP_Z1:
        case FADE_PAGE_FLIP_Z1_OFFSCREEN:
            entry->animFile = "OFF_d_meku";
            entry->animName = "Z_1";
            entry->imageType = 2;
            break;
        case FADE_PAGE_FLIP_A1:
        case FADE_PAGE_FLIP_A1_OFFSCREEN:
            entry->animFile = "OFF_d_meku";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_PAGE_FLIP_FOUR:
            entry->animFile = "OFF_d_four_meku_5sec";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_STORY_MARIO_A3:
            entry->animFile = "OFF_f_in_out_mario";
            entry->animName = "A_3";
            entry->imageType = 2;
            break;
        case FADE_STORY_MARIO_A1:
            entry->animFile = "OFF_f_in_out_mario";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_STORY_PEACH_A3:
            entry->animFile = "OFF_f_in_out_peach";
            entry->animName = "A_3";
            entry->imageType = 2;
            break;
        case FADE_STORY_PEACH_A1:
            entry->animFile = "OFF_f_in_out_peach";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_STORY_BOWSER_A3:
            entry->animFile = "OFF_f_in_out_kuppa";
            entry->animName = "A_3";
            entry->imageType = 2;
            break;
        case FADE_STORY_BOWSER_A1:
            entry->animFile = "OFF_f_in_out_kuppa";
            entry->animName = "A_1";
            entry->imageType = 2;
            break;
        case FADE_STORY_MARIO_A2:
            entry->animFile = "OFF_f_in_out_mario";
            entry->animName = "A_2";
            entry->imageType = 2;
            break;
        case FADE_STORY_PEACH_A2:
            entry->animFile = "OFF_f_in_out_peach";
            entry->animName = "A_2";
            entry->imageType = 2;
            break;
        case FADE_STORY_BOWSER_A2:
            entry->animFile = "OFF_f_in_out_kuppa";
            entry->animName = "A_2";
            entry->imageType = 2;
            break;
    }
}

/**
 * Turns on the tec screen overlay: disp_tec draws a dark blue-grey (25, 35, 42) layer over the 2D camera,
 * shaped by the alpha of fade.tpl image 2.
 * Scripts set it with evt_fade_tec_onoff in Peach's X-Naut Fortress scenes (aji_10 Grodus's room,
 * aji_16 her room, aji_17 TEC's room). Guess: named after TEC.
 */
void fadeTecOn(void) {
    wp->flags |= FADE_WORK_TEC;
}

/** Turns the tec overlay off at once. */
void fadeTecOff(void) {
    wp->flags &= ~FADE_WORK_TEC;
}

/** Turns the tec overlay on, fading it in over about 25 frames. */
void fadeTecSoftOn(void) {
    wp->flags |= (FADE_WORK_TEC_SOFT_IN | FADE_WORK_TEC);
    wp->softness = 0.0f;
}

/** Fades the tec overlay out over about 25 frames. */
void fadeTecSoftOff(void) {
    wp->flags |= (FADE_WORK_TEC_SOFT_OUT | FADE_WORK_TEC);
    wp->softness = 1.0f;
}

/** Turns on the soft-focus overlay (fade.tpl image 3 over the 2D layer), used by the evt_fade helpers. */
void fadeSoftFocusOn(void) {
    wp->flags |= FADE_WORK_SOFT_FOCUS;
}

/** Turns the soft-focus overlay off. */
void fadeSoftFocusOff(void) {
    wp->flags &= ~FADE_WORK_SOFT_FOCUS;
}

/** Covers the 3D scene with white (no fade in this driver); called from usu_00's room code. */
void fadeWhiteOutOn(void) {
    wp->flags |= FADE_WORK_WHITE_OUT;
}

/** Removes the white cover from fadeWhiteOutOn. */
void fadeWhiteOutOff(void) {
    wp->flags &= ~FADE_WORK_WHITE_OUT;
}

/**
 * Sets the screen position the spot fades close on or open from (e.g. where Mario stands);
 * used by the evt_fade helpers.
 */
void fadeSetSpotPos(f32 x, f32 y) {
    FadeEntry* entry = &wp->entries[0];
    entry->fadeSpotX = x;
    entry->fadeSpotY = y;
}

/** Sets the screen offset of the curtain and stage animations in slot 1. */
void fadeSetAnimOfsPos(f32 x, f32 y) {
    FadeEntry* entry = &wp->entries[1];
    entry->animOffsetX = x;
    entry->animOffsetY = y;
}

/** Places the curtain and stage animation of slot 1 at a world position, so it sorts in depth with the 3D scene. */
void fadeSetAnimVirtualPos(f32 x, f32 y, f32 z) {
    FadeEntry* entry = &wp->entries[1];
    entry->VirtualPos = (Vec){x, y, z};
    entry->flags |= FADE_FLAG_VIRTUAL_POS;
}

/**
 * Sets the callback slot 3's page-flip transitions call to draw into the offscreen image;
 * used by the E3 demo sequence.
 */
void fadeSetOffscreenCallback(OffscreenCallback callback, void* param) {
    FadeEntry* entry = &wp->entries[3];
    entry->callback = callback;
    entry->offparam = param;
}

/**
 * Captures the current frame (608x480) into image name through the evt_img capture helper,
 * for the pipe and page-flip transitions to animate.
 */
static inline void fadeCapture(const char* name, const char* off, s32 mode) {
    EventEntry evt;
    EvtWord args[10];

    evt.args = args;
    args[0] = (EvtWord)name;
    args[1] = 0;
    args[2] = (EvtWord)off;
    args[3] = mode;
    args[4] = 0;
    args[5] = 0;
    args[6] = 608;
    args[7] = 480;
    evt_img_alloc_capture(&evt, TRUE);
}

/**
 * Once per frame: advances every active fade slot (progress from the render clock,
 * the pipe and page-flip sounds and captures, posing the curtain and story-book animations),
 * marks fades finished, and queues their draw callbacks plus the tec,
 * soft-focus and black or white cover overlays.
 */
void fadeMain(void) {
    FadeEntry* entry;
    s32 i;

    entry = &wp->entries[0];
    for (i = 0; i < 5; i++, entry++) {
        if (!(entry->flags & FADE_FLAG_ACTIVE)) {
            continue;
        }
        switch (entry->type) {
#if VERSION <= VERSION_DEMO_JP
            case FADE_IN_IRIS:
            case FADE_IN_SPOT:
            case FADE_IN_SPOT_3D:
            case FADE_IN_BLACK:
            case FADE_IN_BLACK_2D:
            case FADE_IN_BLACK_3D:
#else
            case 1:
            case 3:
            case 5:
            case 9:
            case 11:
            case 14:
#endif
                switch (entry->state) {
                    case 0:
#if VERSION >= VERSION_JP
                        if (entry->duration != 0) {
                            if (entry->delayFrames != 0) {
                                entry->delayFrames--;
                            } else {
                                entry->elapsed += VERSION_PICK(16.666666f, 1000.0f / gp->framerate);
                            }
                            entry->progress = entry->elapsed / entry->duration;
                            entry->progress = 1.0f - entry->progress;
                            entry->rotation += 10.0f;
                            if (entry->progress < 0.0f) {
                                entry->progress = 0.0f;
                                entry->flags |= FADE_FLAG_DONE;
                                entry->state++;
                            }
                        } else {
                            entry->progress = 0.0f;
                            entry->flags |= FADE_FLAG_DONE;
                            entry->state++;
                        }
#else
                        entry->elapsed += VERSION_PICK(16.666666f, 1000.0f / gp->framerate);
                        entry->progress = entry->elapsed / entry->duration;
                        entry->progress = 1.0f - entry->progress;
                        entry->rotation += 10.0f;
                        if (entry->progress < 0.0f) {
                            entry->progress = 0.0f;
                            entry->flags |= FADE_FLAG_DONE;
                            entry->state++;
                        }
#endif
                        break;
                }
                break;
#if VERSION <= VERSION_DEMO_JP
            case FADE_OUT_IRIS:
            case FADE_OUT_SPOT:
            case FADE_OUT_SPOT_3D:
            case FADE_OUT_BLACK:
            case FADE_OUT_BLACK_2D:
            case FADE_COVER_BLACK_2D:
            case FADE_OUT_BLACK_3D:
#else
            case 2:
            case 4:
            case 6:
            case 10:
            case 12:
            case 13:
            case 15:
#endif
                switch (entry->state) {
                    case 0:
#if VERSION >= VERSION_JP
                        if (entry->duration != 0) {
                            entry->elapsed += VERSION_PICK(16.666666f, 1000.0f / gp->framerate);
                            entry->progress = entry->elapsed / entry->duration;
                            entry->rotation -= 10.0f;
                            if (entry->progress > 1.0f) {
                                entry->progress = 1.0f;
                                entry->flags |= FADE_FLAG_DONE;
                                entry->state++;
                            }
                        } else {
                            entry->progress = 1.0f;
                            entry->flags |= FADE_FLAG_DONE;
                            entry->state++;
                        }
#else
                        entry->elapsed += VERSION_PICK(16.666666f, 1000.0f / gp->framerate);
                        entry->progress = entry->elapsed / entry->duration;
                        entry->rotation -= 10.0f;
                        if (entry->progress > 1.0f) {
                            entry->progress = 1.0f;
                            entry->flags |= FADE_FLAG_DONE;
                            entry->state++;
                        }
#endif
                        break;
                }
                break;
#if VERSION <= VERSION_DEMO_JP
            case FADE_PIPE_UP_A1:
            case FADE_PIPE_UP_A2:
            case FADE_PIPE_DOWN_A1:
            case FADE_PIPE_DOWN_A2:
            case FADE_PIPE_LEFT_A1:
            case FADE_PIPE_LEFT_A2:
            case FADE_PIPE_RIGHT_A1:
            case FADE_PIPE_RIGHT_A2:
#else
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
            case 0x30:
#endif
                switch (entry->state) {
                    case 0:
                        if (animGroupBaseAsync(entry->animFile, entry->imageType, NULL) == 0) {
                            if (entry->paperPoseId < 0 && (entry->type == FADE_PIPE_UP_A2 || entry->type == FADE_PIPE_DOWN_A2 || entry->type == FADE_PIPE_LEFT_A2 || entry->type == FADE_PIPE_RIGHT_A2)) {
                                wp->flags |= FADE_WORK_LOAD_BLACK;
                            }
                            break;
                        }
                        wp->flags &= ~FADE_WORK_LOAD_BLACK;
                        if (entry->paperPoseId >= 0) {
                            imgRelease(entry->image, entry->imageType);
                            animPaperPoseRelease(entry->paperPoseId);
                            entry->paperPoseId = -1;
                            entry->image = NULL;
                        }
                        entry->paperPoseId = animPaperPoseEntry(entry->animFile, entry->imageType);
                        imgEntry("fImg_dokan", entry->imageType);
                        entry->image = imgNameToPtr("fImg_dokan", entry->imageType);
                        entry->image->poseId = animPaperPoseGetId(entry->animFile, entry->imageType);
                        entry->image->animTime = animTimeGetTime(entry->imageType);
                        entry->image->animName = entry->animName;
                        imgClearVirtualPoint(entry->image);
                        fadeCapture("fImg_dokan", NULL, 0);
                        switch (entry->type) {
                            case FADE_PIPE_LEFT_A1:
#if VERSION <= VERSION_DEMO_JP
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_CHANGE_MAP_DOKAN1"));
                                break;
                            case FADE_PIPE_RIGHT_A1:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_CHANGE_MAP_DOKAN2"));
                                break;
                            case FADE_PIPE_UP_A1:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_CHANGE_MAP_DOKAN3"));
                                break;
                            case FADE_PIPE_DOWN_A1:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_CHANGE_MAP_DOKAN4"));
                                break;
                            case FADE_PIPE_LEFT_A2:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_CHANGE_MAP_DOKAN5"));
                                break;
                            case FADE_PIPE_RIGHT_A2:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_CHANGE_MAP_DOKAN6"));
                                break;
                            case FADE_PIPE_UP_A2:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_CHANGE_MAP_DOKAN7"));
                                break;
                            case FADE_PIPE_DOWN_A2:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_CHANGE_MAP_DOKAN8"));
#else
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_CHANGE_MAP_DOKAN1 | PSND_SFX_KEEP));
                                break;
                            case FADE_PIPE_RIGHT_A1:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_CHANGE_MAP_DOKAN2 | PSND_SFX_KEEP));
                                break;
                            case FADE_PIPE_UP_A1:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_CHANGE_MAP_DOKAN3 | PSND_SFX_KEEP));
                                break;
                            case FADE_PIPE_DOWN_A1:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_CHANGE_MAP_DOKAN4 | PSND_SFX_KEEP));
                                break;
                            case FADE_PIPE_LEFT_A2:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_CHANGE_MAP_DOKAN5 | PSND_SFX_KEEP));
                                break;
                            case FADE_PIPE_RIGHT_A2:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_CHANGE_MAP_DOKAN6 | PSND_SFX_KEEP));
                                break;
                            case FADE_PIPE_UP_A2:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_CHANGE_MAP_DOKAN7 | PSND_SFX_KEEP));
                                break;
                            case FADE_PIPE_DOWN_A2:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_CHANGE_MAP_DOKAN8 | PSND_SFX_KEEP));
#endif
                                break;
                        }
                        entry->state++;
                        break;
                    case 1:
                        if (entry->image->loopTimes >= 1.0f) {
                            entry->flags |= FADE_FLAG_DONE;
                            switch (entry->type) {
#if VERSION <= VERSION_DEMO_JP
                                case FADE_PIPE_UP_A2:
                                case FADE_PIPE_DOWN_A2:
                                case FADE_PIPE_LEFT_A2:
                                case FADE_PIPE_RIGHT_A2:
#else
                                case 0x2A:
                                case 0x2C:
                                case 0x2E:
                                case 0x30:
#endif
                                    imgRelease(entry->image, entry->imageType);
                                    animPaperPoseRelease(entry->paperPoseId);
                                    entry->paperPoseId = -1;
                                    entry->image = NULL;
                                    break;
                            }
                            entry->state++;
                        }
                        break;
                }
                break;
#if VERSION <= VERSION_DEMO_JP
            case FADE_PAGE_FLIP_Z1:
            case FADE_PAGE_FLIP_Z1_OFFSCREEN:
            case FADE_STORY_MARIO_A1:
            case FADE_STORY_PEACH_A1:
            case FADE_STORY_BOWSER_A1:
            case FADE_STORY_MARIO_A2:
            case FADE_STORY_PEACH_A2:
            case FADE_STORY_BOWSER_A2:
#else
            case 0x31:
            case 0x34:
            case 0x37:
            case 0x39:
            case 0x3B:
            case 0x3C:
            case 0x3D:
            case 0x3E:
#endif
                switch (entry->state) {
                    case 0:
                        if (animGroupBaseAsync(entry->animFile, entry->imageType, NULL) == 0) {
                            break;
                        }
                        if (entry->paperPoseId >= 0) {
                            imgRelease(entry->image, entry->imageType);
                            animPaperPoseRelease(entry->paperPoseId);
                            entry->paperPoseId = -1;
                            entry->image = NULL;
                        }
                        entry->paperPoseId = animPaperPoseEntry(entry->animFile, entry->imageType);
                        animPoseSetMaterialFlagOn(entry->paperPoseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
                        imgEntry("fImg_peron", entry->imageType);
                        entry->image = imgNameToPtr("fImg_peron", entry->imageType);
                        entry->image->poseId = animPaperPoseGetId(entry->animFile, entry->imageType);
                        entry->image->animTime = animTimeGetTime(entry->imageType);
                        entry->image->animName = entry->animName;
                        imgClearVirtualPoint(entry->image);
                        switch (entry->type) {
                            case FADE_PAGE_FLIP_Z1_OFFSCREEN:
                                offscreenEntry("fOff_peron");
                                fadeCapture("fImg_peron", "fOff_peron", 0);
                                break;
#if VERSION <= VERSION_DEMO_JP
                            case FADE_STORY_MARIO_A1:
                            case FADE_STORY_PEACH_A1:
                            case FADE_STORY_BOWSER_A1:
#else
                            case 0x37:
                            case 0x39:
                            case 0x3B:
#endif
                                fadeCapture("fImg_peron", NULL, 2);
                                entry->image->camId = CAMERA_FADE2;
                                entry->image->dispCamId = CAMERA_FADE2;
#if VERSION <= VERSION_DEMO_JP
                                psndSFXOn(SFX_NAME_WORD("SFX_OFF_CHANGE_STORY1"));
#else
                                psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_CHANGE_STORY1 | PSND_SFX_KEEP));
#endif
                                break;
#if VERSION <= VERSION_DEMO_JP
                            case FADE_STORY_MARIO_A2:
                            case FADE_STORY_PEACH_A2:
                            case FADE_STORY_BOWSER_A2:
#else
                            case 0x3C:
                            case 0x3D:
                            case 0x3E:
#endif
                                fadeCapture("fImg_peron", NULL, 2);
                                entry->image->camId = CAMERA_FADE;
                                entry->image->dispCamId = CAMERA_FADE;
                                break;
                            default:
                                fadeCapture("fImg_peron", NULL, 2);
                                break;
                        }
                        entry->state++;
                        break;
                    case 1:
                        if (entry->image->loopTimes >= 1.0f) {
                            switch (entry->type) {
#if VERSION <= VERSION_DEMO_JP
                                case FADE_STORY_MARIO_A1:
                                case FADE_STORY_PEACH_A1:
                                case FADE_STORY_BOWSER_A1:
                                case FADE_STORY_MARIO_A2:
                                case FADE_STORY_PEACH_A2:
                                case FADE_STORY_BOWSER_A2:
#else
                                case 0x37:
                                case 0x39:
                                case 0x3B:
                                case 0x3C:
                                case 0x3D:
                                case 0x3E:
#endif
                                    entry->image->animTime = animTimeGetTime(entry->imageType);
                                    entry->image->animName = "A_2";
                                    break;
                                default:
                                    entry->flags |= FADE_FLAG_DONE;
                                    break;
                            }
                            entry->state++;
                        }
                        break;
                    case 2:
                        if (entry->image->loopTimes >= 1.0f) {
                            entry->flags |= FADE_FLAG_DONE;
                            entry->state++;
                        }
                        break;
                }
                break;
#if VERSION <= VERSION_DEMO_JP
            case FADE_PAGE_FLIP_A1:
            case FADE_PAGE_FLIP_A1_OFFSCREEN:
            case FADE_STORY_MARIO_A3:
            case FADE_STORY_PEACH_A3:
            case FADE_STORY_BOWSER_A3:
#else
            case 0x32:
            case 0x35:
            case 0x36:
            case 0x38:
            case 0x3A:
#endif
                switch (entry->state) {
                    case 0:
                        entry->image = imgNameToPtr("fImg_peron", entry->imageType);
                        entry->image->poseId = animPaperPoseGetId(entry->animFile, entry->imageType);
                        entry->image->animTime = animTimeGetTime(entry->imageType);
                        entry->image->animName = entry->animName;
                        imgClearVirtualPoint(entry->image);
                        switch (entry->type) {
#if VERSION <= VERSION_DEMO_JP
                            case FADE_STORY_MARIO_A3:
                            case FADE_STORY_PEACH_A3:
                            case FADE_STORY_BOWSER_A3:
                                psndSFXOn(SFX_NAME_WORD("SFX_OFF_CHANGE_STORY2"));
#else
                            case 0x36:
                            case 0x38:
                            case 0x3A:
                                psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_CHANGE_STORY2 | PSND_SFX_KEEP));
#endif
                                break;
#if VERSION <= VERSION_DEMO_JP
                            case FADE_PAGE_FLIP_A1:
                            case FADE_PAGE_FLIP_A1_OFFSCREEN:
                                psndSFXOn(SFX_NAME_WORD("SFX_OFF3_MAP_CHANGE1"));
#else
                            case 0x32:
                            case 0x35:
                                psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF3_MAP_CHANGE1 | PSND_SFX_KEEP));
#endif
                                break;
                        }
                        entry->state++;
                        break;
                    case 1:
                        if (entry->image->loopTimes >= 1.0f) {
                            entry->flags |= FADE_FLAG_DONE;
                            imgRelease(entry->image, entry->imageType);
                            animPaperPoseRelease(entry->paperPoseId);
                            entry->paperPoseId = -1;
                            entry->image = NULL;
                            if (entry->type == FADE_PAGE_FLIP_A1_OFFSCREEN) {
                                offscreenDelete("fOff_peron");
                            }
                            entry->state++;
                        }
                        break;
                }
                break;
            case FADE_CURTAIN_UP_DOWN_A1:
                switch (entry->state) {
                    case 0:
                        if (animGroupBaseAsync(entry->animFile, entry->imageType, NULL) == 0) {
                            break;
                        }
                        if (entry->poseId >= 0) {
                            animPoseRelease(entry->poseId);
                        }
                        entry->poseId = animPoseEntry(entry->animFile, entry->imageType);
                        animPoseSetMaterialFlagOn(entry->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
                        animPoseSetAnim(entry->poseId, entry->animName, TRUE);
                        entry->animColor = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
                        animPoseSetMaterialFlagOff(entry->poseId, MAP_OBJ_FLAG_COLOR);
                        entry->state++;
                    case 1:
                        animPoseMain(entry->poseId);
                        if (animPoseGetLoopTimes(entry->poseId) >= 1.0f) {
                            entry->flags |= FADE_FLAG_DONE;
                            entry->state++;
                        }
                        break;
                    case 2:
                        animPoseMain(entry->poseId);
                        break;
                }
                break;
            case FADE_CURTAIN_VERTICAL_A2:
                switch (entry->state) {
                    case 0:
                        if (animGroupBaseAsync(entry->animFile, entry->imageType, NULL) == 0) {
                            break;
                        }
                        if (entry->poseId >= 0) {
                            animPoseRelease(entry->poseId);
                        }
                        entry->poseId = animPoseEntry(entry->animFile, entry->imageType);
                        animPoseSetMaterialFlagOn(entry->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
                        animPoseSetAnim(entry->poseId, entry->animName, TRUE);
                        entry->animColor = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
                        animPoseSetMaterialFlagOff(entry->poseId, MAP_OBJ_FLAG_COLOR);
                        entry->state++;
                    case 1:
                        if (animPoseGetLoopTimes(entry->poseId) >= 0.5f) {
                            entry->animColor.r *= 0.98f;
                            entry->animColor.g *= 0.98f;
                            entry->animColor.b *= 0.98f;
                            animPoseSetMaterialEvtColor(entry->poseId, entry->animColor);
                            animPoseSetMaterialFlagOn(entry->poseId, MAP_OBJ_FLAG_COLOR);
                        }
                        animPoseMain(entry->poseId);
                        if (animPoseGetLoopTimes(entry->poseId) >= 1.0f) {
                            entry->flags |= FADE_FLAG_DONE;
                            entry->state++;
                            animPoseRelease(entry->poseId);
                            entry->poseId = -1;
                        }
                        break;
                }
                break;
#if VERSION <= VERSION_DEMO_JP
            case FADE_CURTAIN_SIDES_A1_3D:
            case FADE_CURTAIN_SIDES_A2_3D:
            case FADE_CURTAIN_SIDES_Z1_3D:
            case FADE_CURTAIN_SIDES_A1:
            case FADE_CURTAIN_SIDES_A2:
            case FADE_CURTAIN_CENTER_A1_3D:
            case FADE_CURTAIN_CENTER_A2_3D:
            case FADE_CURTAIN_CENTER_Z2_3D:
            case FADE_CURTAIN_CENTER_Z1_3D:
            case FADE_CURTAIN_ENTER_3D:
            case FADE_CURTAIN_CENTER_A1:
            case FADE_CURTAIN_CENTER_A2:
            case FADE_CURTAIN_CENTER_Z2:
            case FADE_CURTAIN_CENTER_Z1:
            case FADE_CURTAIN_ENTER:
            case FADE_PAGE_FLIP_FOUR:
            case FADE_CURTAIN_SIDES_A1_TOP:
            case FADE_CURTAIN_SIDES_A2_TOP:
#else
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
            case 0x1C:
            case 0x1D:
            case 0x1E:
            case 0x1F:
            case 0x20:
            case 0x22:
            case 0x23:
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x28:
            case 0x33:
            case 0x3F:
            case 0x40:
#endif
                switch (entry->state) {
                    case 0:
                        if (animGroupBaseAsync(entry->animFile, entry->imageType, NULL) == 0) {
                            break;
                        }
                        switch (entry->type) {
#if VERSION <= VERSION_DEMO_JP
                            case FADE_CURTAIN_SIDES_A1_3D:
                            case FADE_CURTAIN_SIDES_A1:
                            case FADE_CURTAIN_SIDES_A1_TOP:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_BTL_KORTESS_CHANGE1"));
                                    break;
                            case FADE_CURTAIN_SIDES_A2_3D:
                            case FADE_CURTAIN_SIDES_A2:
                            case FADE_CURTAIN_SIDES_A2_TOP:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_BTL_KORTESS_CHANGE2"));
                                    break;
                            case FADE_CURTAIN_CENTER_A1_3D:
                            case FADE_CURTAIN_CENTER_A1:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_STG_START_CURTAIN_UP1"));
                                    break;
                            case FADE_CURTAIN_CENTER_A2_3D:
                            case FADE_CURTAIN_CENTER_A2:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD("SFX_OFF_STG_START_CURTAIN_DOWN1"));
#else
                            case 0x18:
                            case 0x1B:
                            case 0x3F:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_BTL_KORTESS_CHANGE1 | PSND_SFX_KEEP));
                                break;
                            case 0x19:
                            case 0x1C:
                            case 0x40:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_BTL_KORTESS_CHANGE2 | PSND_SFX_KEEP));
                                break;
                            case 0x1D:
                            case 0x23:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_STG_START_CURTAIN_UP1 | PSND_SFX_KEEP));
                                break;
                            case 0x1E:
                            case 0x24:
                                entry->sfxId = psndSFXOn(SFX_NAME_WORD(PSND_SFX_OFF_STG_START_CURTAIN_DOWN1 | PSND_SFX_KEEP));
#endif
                                break;
                        }
                        if (entry->poseId >= 0) {
                            animPoseRelease(entry->poseId);
                        }
                        entry->poseId = animPoseEntry(entry->animFile, entry->imageType);
                        animPoseSetMaterialFlagOn(entry->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
                        animPoseSetAnim(entry->poseId, entry->animName, TRUE);
                        entry->state++;
                        if (entry->type == FADE_CURTAIN_ENTER_3D || entry->type == FADE_CURTAIN_ENTER) {
                            entry->scale = 3.0f;
                            entry->scaleTimer = 0;
                        }
                    case 1:
                        animPoseMain(entry->poseId);
                        if (entry->type == FADE_CURTAIN_ENTER_3D || entry->type == FADE_CURTAIN_ENTER) {
                            if (!(entry->flags & FADE_FLAG_NARROW_FAST)) {
                                if (entry->scaleTimer < FPS_FRAMES(180)) {
                                    entry->scale = intplGetValue(EASING_SINE_OUT, 3.0f, 1.0f, ++entry->scaleTimer, FPS_FRAMES(180));
                                }
                            } else {
                                if (entry->scaleTimer < FPS_FRAMES(90)) {
                                    entry->scale = intplGetValue(EASING_SINE_OUT, 1.5f, 1.0f, ++entry->scaleTimer, FPS_FRAMES(90));
                                }
                            }
                        }
                        if (animPoseGetLoopTimes(entry->poseId) >= 1.0f) {
                            if (entry->type == FADE_CURTAIN_ENTER_3D || entry->type == FADE_CURTAIN_ENTER) {
                                if (!(entry->flags & FADE_FLAG_NARROW_FAST)) {
                                    if (entry->scaleTimer == FPS_FRAMES(180)) {
                                        entry->flags |= FADE_FLAG_DONE;
                                        entry->state++;
                                    }
                                } else {
                                    if (entry->scaleTimer == FPS_FRAMES(90)) {
                                        entry->flags |= FADE_FLAG_DONE;
                                        entry->state++;
                                    }
                                }
                            } else {
                                entry->flags |= FADE_FLAG_DONE;
                                entry->state++;
                                psndSFXOff(entry->sfxId);
                                if (entry->type == FADE_CURTAIN_SIDES_A2_3D || entry->type == FADE_CURTAIN_SIDES_A2 || entry->type == FADE_CURTAIN_SIDES_A2_TOP) {
                                    animPoseRelease(entry->poseId);
                                    entry->poseId = -1;
                                }
                            }
                        }
                        break;
                    case 2:
                        if (entry->poseId >= 0) {
                            animPoseMain(entry->poseId);
                        }
                        break;
                }
                break;
#if VERSION <= VERSION_DEMO_JP
            case FADE_CURTAIN_LEAVE_3D:
            case FADE_CURTAIN_LEAVE:
#else
            case 0x21:
            case 0x27:
#endif
                switch (entry->state) {
                    case 0:
                        entry->scale = 1.0f;
                        entry->scaleTimer = 0;
                        entry->state++;
                    case 1:
                        animPoseMain(entry->poseId);
                        if (entry->scaleTimer < FPS_FRAMES(120)) {
                            entry->scale = intplGetValue(EASING_SINE_OUT, 1.0f, 3.0f, ++entry->scaleTimer, FPS_FRAMES(120));
                        } else {
                            entry->flags |= FADE_FLAG_DONE;
                            entry->state++;
                            animPoseRelease(entry->poseId);
                            entry->poseId = -1;
                        }
                        break;
                }
                break;
        }
        switch (entry->type) {
#if VERSION <= VERSION_DEMO_JP
            case FADE_CURTAIN_UP_DOWN_A1:
            case FADE_CURTAIN_VERTICAL_A2:
            case FADE_CURTAIN_CENTER_A1:
            case FADE_CURTAIN_CENTER_A2:
            case FADE_CURTAIN_CENTER_Z2:
            case FADE_CURTAIN_CENTER_Z1:
            case FADE_CURTAIN_LEAVE:
            case FADE_CURTAIN_ENTER:
                dispEntry(CAMERA_FADE, 0.0f, DISP_MODE_OPAQUE, disp_maku, entry);
                break;
            case FADE_CURTAIN_SIDES_A1:
            case FADE_CURTAIN_SIDES_A2:
            case FADE_CURTAIN_SIDES_A1_TOP:
            case FADE_CURTAIN_SIDES_A2_TOP:
                dispEntry(CAMERA_FADE2, 0.0f, DISP_MODE_OPAQUE, disp_maku, entry);
                break;
            case FADE_CURTAIN_SIDES_A1_3D:
            case FADE_CURTAIN_SIDES_A2_3D:
            case FADE_CURTAIN_SIDES_Z1_3D:
            case FADE_CURTAIN_CENTER_A1_3D:
            case FADE_CURTAIN_CENTER_A2_3D:
            case FADE_CURTAIN_CENTER_Z2_3D:
            case FADE_CURTAIN_CENTER_Z1_3D:
            case FADE_CURTAIN_LEAVE_3D:
            case FADE_CURTAIN_ENTER_3D:
            case FADE_PAGE_FLIP_FOUR:
                dispEntry(CAMERA_3D_IMAGE, 0.0f, DISP_MODE_OPAQUE, disp_maku, entry);
                break;
            case FADE_PAGE_FLIP_Z1:
            case FADE_PAGE_FLIP_A1:
            case FADE_PAGE_FLIP_Z1_OFFSCREEN:
            case FADE_PAGE_FLIP_A1_OFFSCREEN:
            case FADE_STORY_MARIO_A3:
            case FADE_STORY_MARIO_A1:
            case FADE_STORY_PEACH_A3:
            case FADE_STORY_PEACH_A1:
            case FADE_STORY_BOWSER_A3:
            case FADE_STORY_BOWSER_A1:
            case FADE_STORY_MARIO_A2:
            case FADE_STORY_PEACH_A2:
            case FADE_STORY_BOWSER_A2:
#else
            case FADE_CURTAIN_UP_DOWN_A1:
            case 0x17:
            case 0x23:
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
                dispEntry(CAMERA_FADE, 0.0f, DISP_MODE_OPAQUE, disp_maku, entry);
                break;
            case 0x1B:
            case 0x1C:
            case 0x3F:
            case 0x40:
                dispEntry(CAMERA_FADE2, 0.0f, DISP_MODE_OPAQUE, disp_maku, entry);
                break;
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1D:
            case 0x1E:
            case 0x1F:
            case 0x20:
            case 0x21:
            case 0x22:
            case 0x33:
                dispEntry(CAMERA_3D_IMAGE, 0.0f, DISP_MODE_OPAQUE, disp_maku, entry);
                break;
            case 0x31:
            case 0x32:
            case 0x34:
            case 0x35:
            case 0x36:
            case 0x37:
            case 0x38:
            case 0x39:
            case 0x3A:
            case 0x3B:
            case 0x3C:
            case 0x3D:
            case 0x3E:
#endif
                if (entry->paperPoseId >= 0 && entry->callback) {
                    dispEntry(CAMERA_OFFSCREEN2, 0.0f, DISP_MODE_OPAQUE, entry->callback, entry->offparam);
                }
                break;
#if VERSION <= VERSION_DEMO_JP
            case FADE_IN_BLACK_2D:
            case FADE_OUT_BLACK_2D:
            case FADE_COVER_BLACK_2D:
#else
            case 0xB:
            case 0xC:
            case 0xD:
#endif
                dispEntry(CAMERA_2D, 1000.0f, DISP_MODE_OPAQUE, disp_texture, wp);
                break;
#if VERSION <= VERSION_DEMO_JP
            case FADE_IN_SPOT_3D:
            case FADE_OUT_SPOT_3D:
            case FADE_IN_BLACK_3D:
            case FADE_OUT_BLACK_3D:
#else
            case 5:
            case 6:
            case 0xE:
            case 0xF:
#endif
                dispEntry(CAMERA_3D, 0.0f, DISP_MODE_CUSTOM_LAST, disp_texture, wp);
                break;
#if VERSION <= VERSION_DEMO_JP
            case FADE_PIPE_UP_A1:
            case FADE_PIPE_UP_A2:
            case FADE_PIPE_DOWN_A1:
            case FADE_PIPE_DOWN_A2:
            case FADE_PIPE_LEFT_A1:
            case FADE_PIPE_LEFT_A2:
            case FADE_PIPE_RIGHT_A1:
            case FADE_PIPE_RIGHT_A2:
#else
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
            case 0x30:
#endif
                break;
            default:
                dispEntry(CAMERA_FADE, 0.0f, DISP_MODE_OPAQUE, disp_texture, wp);
                break;
        }
    }
    if (wp->flags & (FADE_WORK_TEC | FADE_WORK_SOFT_FOCUS)) {
        dispEntry(CAMERA_2D, 999.0f, DISP_MODE_OPAQUE, disp_tec, wp);
    }
    if (wp->flags & FADE_WORK_LOAD_BLACK) {
        dispEntry(CAMERA_FADE, -100.0f, DISP_MODE_OPAQUE, disp_dummy_black, wp);
    }
    if (wp->flags & FADE_WORK_WHITE_OUT) {
        dispEntry(CAMERA_3D, 1000.0f, DISP_MODE_CUSTOM_LAST, disp_dummy_black, wp);
    }
}

/**
 * Draws slot 0's fade (with the 2D camera's projection, on the fade, 2D or 3D camera fadeMain queues it on): the iris
 * or spot texture scaled around the spot position (growing with the fifth power of progress) or a flat colour,
 * and nothing more once a fade-in has finished.
 */
static void disp_texture(CameraId camId, void* param) {
#if VERSION <= VERSION_DEMO_JP
    static u8 i4tex[1] ATTRIBUTE_ALIGN(32) = {0xFF};
#else
    static u8 i4tex[1] = {0xFF};
#endif
    FadeWork* work;
    CameraEntry* camera;
    f32 scale;
    f32 p;
    f32 size;
    Mtx mtx1;
    Mtx mtx2;
    Mtx mtx3;
    Mtx mtx4;
    GXTexObj obj;
    f32 proj[GX_PROJECTION_SZ];

    camera = camGetPtr(CAMERA_2D);
    work = wp;
    switch (work->entries[0].type) {
        case FADE_IN_IRIS:
        case FADE_IN_SPOT:
        case FADE_IN_SPOT_3D:
        case FADE_IN_BLACK:
        case FADE_IN_BLACK_2D:
        case FADE_IN_BLACK_3D:
            if (work->entries[0].flags & FADE_FLAG_DONE) {
                return;
            }
    }
    GXGetProjectionv(proj);
    GXSetProjection(camera->projection, camera->projType);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    switch (work->entries[0].type) {
        case FADE_IN_IRIS:
        case FADE_OUT_IRIS:
            GXLoadPosMtxImm(camera->view, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            TEXGetGXTexObjFromPalette(wp->texture, &obj, 0);
            GXLoadTexObj(&obj, GX_TEXMAP0);
            if (!(work->entries[0].flags & FADE_FLAG_DONE)) {
                p = work->entries[0].progress;
                scale = p * (p * (p * (100.0f * p * p)));
                PSMTXTrans(mtx1, 0.5f, 0.5f, 0.0f);
                PSMTXScale(mtx3, scale, scale, 1.0f);
                PSMTXTrans(mtx2, -0.5f, -0.5f, 0.0f);
                PSMTXConcat(mtx1, mtx3, mtx3);
                PSMTXConcat(mtx3, mtx2, work->entries[0].texMtx);
            } else if (work->entries[0].type == FADE_OUT_IRIS) {
                PSMTXTrans(work->entries[0].texMtx, 1.0f, 1.0f, 0.0f);
            }
            break;
        case FADE_IN_SPOT:
        case FADE_OUT_SPOT:
        case FADE_IN_SPOT_3D:
        case FADE_OUT_SPOT_3D:
            GXLoadPosMtxImm(camera->view, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            TEXGetGXTexObjFromPalette(wp->texture, &obj, 1);
            GXLoadTexObj(&obj, GX_TEXMAP0);
            if (!(work->entries[0].flags & FADE_FLAG_DONE)) {
                p = work->entries[0].progress;
                scale = p * (p * (p * (100.0f * p * p)));
                PSMTXTrans(mtx1, 0.5f, 0.5f, 0.0f);
                PSMTXScale(mtx3, scale, scale, 1.0f);
                PSMTXRotRad(mtx4, 'z', 0.017453292f * work->entries[0].rotation);
                PSMTXTrans(mtx2, -0.5f - work->entries[0].fadeSpotX / gp->fbWidth,
                    -0.5f + work->entries[0].fadeSpotY / gp->efbHeight, 0.0f);
                PSMTXConcat(mtx1, mtx3, mtx3);
                PSMTXConcat(mtx3, mtx4, mtx3);
                PSMTXConcat(mtx3, mtx2, work->entries[0].texMtx);
            } else if (work->entries[0].type == FADE_OUT_SPOT || work->entries[0].type == FADE_OUT_SPOT_3D) {
                PSMTXTrans(work->entries[0].texMtx, 1.0f, 1.0f, 0.0f);
            }
            break;
        case FADE_IN_BLACK:
        case FADE_OUT_BLACK:
        case FADE_IN_BLACK_2D:
        case FADE_OUT_BLACK_2D:
        case FADE_IN_BLACK_3D:
        case FADE_OUT_BLACK_3D:
            GXLoadPosMtxImm(camera->view, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXInitTexObj(&obj, i4tex, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXLoadTexObj(&obj, GX_TEXMAP0);
            PSMTXIdentity(work->entries[0].texMtx);
            work->entries[0].color.a = 255.0f * work->entries[0].progress;
            break;
        case FADE_COVER_BLACK_2D:
            GXLoadPosMtxImm(camera->view, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXInitTexObj(&obj, i4tex, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXLoadTexObj(&obj, GX_TEXMAP0);
            PSMTXIdentity(work->entries[0].texMtx);
            break;
    }
    GXLoadTexMtxImm(work->entries[0].texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    GXSetTevColor(GX_TEVREG0, work->entries[0].color);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    size = 304.0f;
    GXPosition3f32(-size, size, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(size, size, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(size, -size, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-size, -size, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXSetProjectionv(proj);
}

// kept: the target calls z (a local function in the JP demo map) from disp_maku; a function this small is auto-
// inlined otherwise, and -inline noauto for the file changes other functions
#pragma dont_inline on
/**
 * Depth scale for the curtain animation drawn at a world position:
 * how its depth on the 3D camera compares with its depth on the fade camera, so it hides behind closer scenery.
 * 1 when the 3D camera is perspective.
 */
static f32 z(void) {
    FadeWork* work = wp;
    CameraEntry* camera;
    Vec pos;
    f32 base;

    // `one` (1.0f) is only a factor in the returned depth (the multiply is in the retail code)
    f32 one = 1.0f;
    f32 pz;

    camera = camGetPtr(CAMERA_3D);
    if (camera->projType == GX_PERSPECTIVE) {
        return 1.0f;
    }
    camera = camGetPtr(CAMERA_FADE);
    PSMTXMultVec(camera->view, &work->entries[1].VirtualPos, &pos);
    pz = pos.z;
    base = camera->viewport[5] + (camera->viewport[5] - camera->viewport[4]) *
        ((1.0f / -pz) * (camera->projection[2][2] * pz + camera->projection[2][3]));
    camera = camGetPtr(CAMERA_3D);
    PSMTXMultVec(camera->view, &work->entries[1].VirtualPos, &pos);
    return (camera->viewport[5] + (camera->viewport[5] - camera->viewport[4]) *
        (one * (camera->projection[2][2] * pos.z + camera->projection[2][3]))) / base;
}

#pragma dont_inline reset

/**
 * Writes the far depth into the depth buffer over the whole camera view without touching colour,
 * so an animation drawn afterwards is never hidden by the scene.
 */
static void zFill(void) {
    static s32 depth_image[32] __attribute__((aligned(32))) = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    CameraEntry* camera;
    f32 y0, y1, x0, x1;
    f32 zpos;
    GXTexObj obj;
    Mtx mtx;

    camera = camGetCurPtr();
    y0 = (camera->near + camera->far) / 2.0f;
    y0 = y0 * (f32)tan(0.5f * ((6.2831855f * camera->fovY) / 360.0f));
    x1 = y0 * camera->aspect;
    y1 = -y0;
    x0 = -x1;
    GXInitTexObj(&obj, depth_image, 4, 4, GX_TF_Z8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetZTexture(GX_ZT_REPLACE, GX_TF_Z8, 0);
    GXSetNumChans(0);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_TRUE);
    GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    GXSetColorUpdate(GX_FALSE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    PSMTXIdentity(mtx);
    GXLoadPosMtxImm(camera->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    zpos = -10000.0f;
    GXPosition3f32(x0, y0, zpos);
    GXTexCoord2u8(0, 0);
    GXPosition3f32(x1, y0, zpos);
    GXTexCoord2u8(1, 0);
    GXPosition3f32(x1, y1, zpos);
    GXTexCoord2u8(1, 1);
    GXPosition3f32(x0, y1, zpos);
    GXTexCoord2u8(0, 1);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);
    GXSetColorUpdate(GX_TRUE);
}

/**
 * Draws slot 1's or slot 4's curtain or stage animation (maku is Japanese for curtain) on its camera,
 * scaled to the screen and offset by fadeSetAnimOfsPos or placed at the virtual world position,
 * with the depth set up by z or zFill.
 */
static void disp_maku(CameraId camId, void* param) {
    FadeEntry* entry = param;
    CameraEntry* camera;
    f32 scale, depth, sx, mag;
    Mtx mtx;
    Mtx mtx2;
    Vec dist;
    Vec dir;

    camera = camGetPtr(camId);
    switch (entry->type) {
        case FADE_CURTAIN_VERTICAL_A2:
        case FADE_CURTAIN_SIDES_A2_3D:
        case FADE_CURTAIN_SIDES_A2:
        case FADE_CURTAIN_SIDES_A2_TOP:
            if (entry->flags & FADE_FLAG_DONE) {
                return;
            }
    }
    switch (entry->type) {
        case FADE_CURTAIN_VERTICAL_A1:
        case FADE_CURTAIN_VERTICAL_A2:
        case FADE_CURTAIN_SIDES_A1_3D:
        case FADE_CURTAIN_SIDES_A2_3D:
        case FADE_CURTAIN_SIDES_Z1_3D:
        case FADE_CURTAIN_SIDES_A1:
        case FADE_CURTAIN_SIDES_A2:
        case FADE_CURTAIN_CENTER_A1_3D:
        case FADE_CURTAIN_CENTER_A2_3D:
        case FADE_CURTAIN_CENTER_Z2_3D:
        case FADE_CURTAIN_CENTER_Z1_3D:
        case FADE_CURTAIN_LEAVE_3D:
        case FADE_CURTAIN_ENTER_3D:
        case FADE_CURTAIN_CENTER_A1:
        case FADE_CURTAIN_CENTER_A2:
        case FADE_CURTAIN_CENTER_Z2:
        case FADE_CURTAIN_CENTER_Z1:
        case FADE_CURTAIN_LEAVE:
        case FADE_CURTAIN_ENTER:
        case FADE_PAGE_FLIP_FOUR:
        case FADE_CURTAIN_SIDES_A1_TOP:
        case FADE_CURTAIN_SIDES_A2_TOP:
            break;
        default:
            return;
    }
    if (entry->poseId < 0) {
        return;
    }
    if (entry->flags & FADE_FLAG_VIRTUAL_POS) {
        camera = camGetPtr(CAMERA_3D);
        mag = 3.141592f;
        depth = camera->fovY;

        depth = (depth *= 0.5f) / 180.0f;
        depth = tan(mag * depth);
        depth = 24.0f / depth;
        PSVECSubtract(&camGetPtr(CAMERA_3D)->cameraPos, &entry->VirtualPos, &dist);
        camera = camGetPtr(CAMERA_3D);
        dir = *(Vec*)camera->view[2];
        PSVECNormalize(&dir, &dir);
        dist.x *= dir.x;
        dist.y *= dir.y;
        dist.z *= dir.z;
        mag = PSVECMag(&dist);
        scale = mag / depth;
        depth = depth - mag;
        animPoseSetGXFunc(entry->poseId, NULL, FALSE);
        GXSetZScaleOffset(z(), 0.0f);
    } else {
        scale = entry->scale;
        depth = 0.0f;
        zFill();
    }
    {
        // zero offset; the retail code adds it with fmadds (a literal 0.0f would fold away)
        f32 ofs = 0.0f;

        PSMTXTrans(mtx, 64.0f * ((entry->animOffsetX * scale + ofs) / gp->fbWidth),
            48.0f * ((entry->animOffsetY * scale + ofs) / gp->efbHeight), depth);
    }
    PSMTXTrans(mtx2, -32.0f, 24.0f, 0.0f);
    PSMTXConcat(mtx, mtx2, mtx);
    sx = 608.0f / gp->fbWidth;
    PSMTXScale(mtx2, sx, 480.0f / gp->efbHeight, sx);
    PSMTXConcat(mtx, mtx2, mtx);
    PSMTXTrans(mtx2, 32.0f, -24.0f, 0.0f);
    PSMTXConcat(mtx, mtx2, mtx);
    animPoseDrawMtx(entry->poseId, mtx, 1, 0.0f, scale);
    animPoseDrawMtx(entry->poseId, mtx, 2, 0.0f, scale);
    animPoseDrawMtx(entry->poseId, mtx, 3, 0.0f, scale);
    sysWaitDrawSync();
}

// FAKE: stand-in that nothing calls; it only adds its 3.141592f literal to this object's constant pool
void fadeStrippedPool(f32* f) {
    f[0] = 3.141592f;
}

/** The colour whose alpha is a (0..1 scaled to 0..255), used for the tec overlay's fade. */
static inline GXColor fadeTecAlpha(f32 a) {
    GXColor c = (GXColor){0, 0, 0, 0};
    c.a = 255.0f * a;
    return c;
}

/**
 * Draws the tec or soft-focus overlay as a full-screen texture on the 2D camera,
 * fading the tec overlay in or out by 0.04 per frame when asked to.
 */
static void disp_tec(CameraId camId, void* param) {
    CameraEntry* camera;
    GXTexObj obj;

    camera = camGetPtr(camId);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    if (wp->flags & FADE_WORK_TEC) {
        TEXGetGXTexObjFromPalette(wp->texture, &obj, 2);
        GXLoadTexObj(&obj, GX_TEXMAP0);
    } else {
        TEXGetGXTexObjFromPalette(wp->texture, &obj, 3);
        GXLoadTexObj(&obj, GX_TEXMAP0);
    }
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_ZERO);
    if (wp->flags & FADE_WORK_TEC) {
        GXSetTevColor(GX_TEVREG0, (GXColor){0x19, 0x23, 0x2A, 0xFF});
    } else {
        GXSetTevColor(GX_TEVREG0, (GXColor){0xE6, 0xE6, 0xE6, 0xFF});
    }
    if (wp->flags & (FADE_WORK_TEC_SOFT_IN | FADE_WORK_TEC_SOFT_OUT)) {
        if (wp->flags & FADE_WORK_TEC_SOFT_IN) {
            wp->softness += 0.04f;
            if (wp->softness > 1.0f) {
                wp->softness = 1.0f;
                wp->flags &= ~FADE_WORK_TEC_SOFT_IN;
            }
        } else {
            wp->softness -= 0.04f;
            if (wp->softness < 0.0f) {
                wp->softness = 0.0f;
                wp->flags &= ~FADE_WORK_TEC_SOFT_OUT;
                wp->flags &= ~(FADE_WORK_TEC | FADE_WORK_SOFT_FOCUS);
            }
        }
        GXSetTevColor(GX_TEVREG1, fadeTecAlpha(wp->softness));
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A1, GX_CA_ZERO);
    }
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXLoadPosMtxImm(camera->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(-308, 244, 0);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3s16(308, 244, 0);
    GXTexCoord2f32(2.0f, 0.0f);
    GXPosition3s16(308, -244, 0);
    GXTexCoord2f32(2.0f, 2.0f);
    GXPosition3s16(-308, -244, 0);
    GXTexCoord2f32(0.0f, 2.0f);
}

/**
 * Draws a black (while a pipe transition holds the screen black) or white rectangle over the whole camera view,
 * for the black screen and the white-out cover.
 */
static void disp_dummy_black(CameraId camId, void* param) {
    CameraEntry* camera;
    f32 proj[GX_PROJECTION_SZ];

    camera = camGetPtr(CAMERA_FADE);
    GXGetProjectionv(proj);
    GXSetProjection(camera->projection, camera->projType);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_OR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    if (wp->flags & FADE_WORK_LOAD_BLACK) {
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){0x00, 0x00, 0x00, 0xFF});
    } else {
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    }
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXLoadPosMtxImm(camera->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(-32, 24, 0);
    GXPosition3s16(32, 24, 0);
    GXPosition3s16(32, -24, 0);
    GXPosition3s16(-32, -24, 0);
    GXSetProjectionv(proj);
}

/**
 * Tells whether the most recently started fade has finished (TRUE when none was started);
 * scripts and sequences wait on it.
 */
BOOL fadeIsFinish(void) {
    FadeEntry* e;

    if (wp->lastEntryId == -1) {
        return TRUE;
    }
    e = &wp->entries[wp->lastEntryId];
    return (e->flags >> 1) & 1;
}

/** Stops fade slot id at once: releases its animation poses and captured image and resets it to the defaults. */
void fadeReset(s32 id) {
    FadeEntry* entry;

    entry = &wp->entries[id];
    if (entry->poseId >= 0) {
        animPoseRelease(entry->poseId);
    }
    if (entry->paperPoseId >= 0) {
        animPaperPoseRelease(entry->paperPoseId);
    }
    if (entry->image) {
        imgRelease(entry->image, entry->imageType);
    }
    memset(entry, 0, sizeof(FadeEntry));
    entry->poseId = -1;
    entry->paperPoseId = -1;
    entry->fadeSpotX = 0.0f;
    entry->fadeSpotY = 0.0f;
    entry->animOffsetX = 0.0f;
    entry->animOffsetY = 0.0f;
    entry->scale = 1.0f;
}

/** Returns fade.tpl; the name entry screen's mask (nameMaskGX) draws its image 1 (the spot texture). */
TEXPalette* fadeGetTpl(void) {
    return wp->texture;
}

/**
 * Shortens the narrowing of the curtain-enter fade now in slot 1 (scale 1.5 to 1 over 90 frames, not 3 to 1 over
 * 180); called after fadeEntry, which clears the flag. Used by the title screen.
 */
void fadeSetNarrowFast(void) {
    wp->entries[1].flags |= FADE_FLAG_NARROW_FAST;
}
