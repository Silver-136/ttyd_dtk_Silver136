// E3 sequence: the trade-show chapter-select menu, still present in the retail builds.

#include "version.h"
#include "seq/seq_e3.h"
#include "demo/DEMOPad.h"
#include "dolphin/gx/GXEnum.h"
#include "dolphin/gx/GXStruct.h"
#include "dolphin/gx/GXTransform.h"
#include "dolphin/gx/GXVert.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/fadedrv.h"
#include "driver/offscreendrv.h"
#include "driver/seqdrv.h"
#include "driver/swdrv.h"
#include "event.h"
#include "mario/mario.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "mario/mario_motion.h"
#include "statuswindow.h"
#include "win/win_main.h"
#include "manager/fontmgr.h"
#include "driver/msgdrv.h"
#include "driver/windowdrv.h"
#include "sfx_name_word.h"
#include "mario/mario_party.h"

typedef enum winTexMode {
#if VERSION == VERSION_EU
    // PAL's texture file has one texture fewer before the tile
    MODE_5 = 4,
    MODE_6 = 5,
    MODE_7 = 6,
    MODE_8 = 7,
#else
    MODE_5 = 5,
    MODE_6 = 6,
    MODE_7 = 7,
    MODE_8 = 8,
#endif
    MODE_11 = 11,
} winTexMode;

typedef struct seq_e3_entry {
    s32 id; // 0x0
    f32 x;  // 0x4
    f32 y;  // 0x8
} seq_e3_entry;

//.bss
static seq_e3_work work;

//.sdata
static seq_e3_work* wp = &work;
#if VERSION >= VERSION_US
/**
 * The four scenarios of the trade-show menu: the story progress each one starts at and where its box sits
 * on screen, one box in each corner.
 */
seq_e3_entry e3_tbl[4] = {
    { 3, -140.0f, 100.0f },
    { 23, 140.0f, 100.0f },
    { 35, -140.0f, -100.0f },
    { 96, 140.0f, -100.0f },
};
#else
/**
 * The four scenarios of the trade-show menu: the story progress each one starts at and where its box sits
 * on screen, one box in each corner.
 */
seq_e3_entry e3_tbl[3] = {
    { 23, -140.0f, 100.0f },
    { 35, 140.0f, 100.0f },
    { 96, 0.0f, -100.0f },
};
#endif
#if VERSION < VERSION_US
char* anim_tbl[] = { "b_mario", "PKR_W_1", "PKR_S_1", "PKR_A_2A", "PNK_W_1", "PNK_S_1",
                     "PNK_O_1", "c_windy", "PWD_W_1", "PWD_S_1",  "PWD_A_7" };
#endif

#if VERSION >= VERSION_US
// US: anim_tbl is gone (dead-stripped) but its strings stayed at the start of .rodata
const char e3_anim_str0[] = "b_mario";
const char e3_anim_str1[] = "PKR_W_1";
const char e3_anim_str2[] = "PKR_S_1";
const char e3_anim_str3[] = "PKR_A_2A";
const char e3_anim_str4[] = "PNK_W_1";
const char e3_anim_str5[] = "PNK_S_1";
const char e3_anim_str6[] = "PNK_O_1";
const char e3_anim_str7[] = "c_windy";
const char e3_anim_str8[] = "PWD_W_1";
const char e3_anim_str9[] = "PWD_S_1";
const char e3_anim_str10[] = "PWD_A_7";
#endif

// Forward Declarations
void e3Disp(CameraId cameraId, void* param);
void e3DispOffscreen(CameraId cameraId, void* param);
#if VERSION >= VERSION_US
static
#endif
void alarm_handler(OSAlarm* alarm, OSContext* context) {
    gp->flags |= GW_FLAG_E3_TIMEOUT;
}

void seq_e3Init(SequenceWork* seq) {
    wp->handle = fileAllocf(FILE_TYPE_TPL, "%s/monthry.tpl", getMarioStDvdRoot());
    wp->curScreen = 0;
    wp->nextScreen = 0;
    wp->scenario = 0;
    wp->poseRotY = 270.0f;
    OSCreateAlarm(&wp->alarm);
#if VERSION >= VERSION_US
#if VERSION == VERSION_EU
    OSSetAlarm(&wp->alarm, (OSTime)OS_TIMER_CLOCK * 600, alarm_handler);
#else
    OSSetAlarm(&wp->alarm, (OSTime)OS_TIMER_CLOCK * 300, alarm_handler);
#endif
#else
    OSSetAlarm(&wp->alarm, (OSTime)OS_TIMER_CLOCK * 600, alarm_handler);
#endif
    wp->initTime = OSGetTime();
    gp->flags &= ~GW_FLAG_NO_ENTRANCE;
    gp->flags |= GW_FLAG_NO_SAVE_BLOCK;
    gp->flags |= GW_FLAG_E3;
    gp->flags &= ~GW_FLAG_E3_TIMEOUT;
    fadeEntry(FADE_IN_BLACK, 200, (GXColor){ 0x00, 0x00, 0x00, 0xFF });
    fadeReset(1);
    psndStopAllFadeOut();
    psndBGMOn(PSND_FADEOUT, "BGM_FILE_MENU1");
    seq->state = 0;
    animGroupBaseAsync("OFF_d_meku", 0, NULL);
}

void seq_e3Exit(SequenceWork* seq) {
    fileFree(wp->handle);
    psndStopAllFadeOut();
}

/**
 * Kiosk menu state machine: press A/Start page-flips from the title to the scenario boxes;
 * the stick picks a scenario (B or 2 min idle flips back);
 * A sets story progress to just before the scenario's e3_tbl entry, runs the story init events up to it,
 * sets partners, a 20 HP/FP/BP loadout and items, and starts its map.
 */
void seq_e3Main(SequenceWork* seq) {
    CameraEntry* camera;
    s32 prevId;
    BOOL matched;
    s32 targetId;
    s32 prev;
    s32 stickX;
    s32 stickY;
    s32 i;
    s32 stageId;
    s32 idx;
    s32 foundIdx;
    EventData* found;

    camera = camGetPtr(CAMERA_3D);
    camera->cameraAt = (Vec){ 0.0f, 0.0f, 0.0f };
    camera->cameraPos = (Vec){ 0.0f, 0.0f, (240.0f / (f32)tan(0.21816612780094147)) / 2 };

    switch (seq->state) {
        case 0:
            seq->state++;
            break;

        case 1:
            if (fadeIsFinish() && keyGetButtonTrg(0) & (PAD_BUTTON_START | PAD_BUTTON_A)) {
                psndSFXOn("SFX_PRESS_START1");
                fadeEntry(MARIO_HEAD_FADE_IN, 0, (GXColor){ 0x00, 0x00, 0x00, 0xFF });
                fadeSetOffscreenCallback(e3DispOffscreen, wp);
                wp->nextScreen = 1;
                seq->state++;
            }
            break;

        case 2:
            if (fadeIsFinish()) {
                fadeEntry(MARIO_HEAD_FADE_OUT, 0, (GXColor){ 0x00, 0x00, 0x00, 0xFF });
                wp->curX = wp->dstX = e3_tbl[wp->scenario].x;
                wp->curY = wp->dstY = e3_tbl[wp->scenario].y;
                seq->state++;
            }
            break;

        case 3:
            if (fadeIsFinish()) {
                seq->state++;
            }
            break;

        case 4: {
#if VERSION >= VERSION_US
            prev = wp->scenario;
            keyGetStickX(0);
            keyGetStickY(0);
            if (keyGetDirTrg(0) & (DEMO_STICK_LEFT | DEMO_STICK_RIGHT)) {
                wp->scenario ^= 1;
            } else if (keyGetDirTrg(0) & (DEMO_STICK_UP | DEMO_STICK_DOWN)) {
                wp->scenario ^= 2;
            }
#else
            prev = wp->scenario;
            stickX = keyGetStickX(0);
            stickY = keyGetStickY(0);
            switch (wp->scenario) {
                case 0:
                    if (keyGetDirTrg(0) & DEMO_STICK_RIGHT) {
                        wp->scenario = 1;
                    } else if (keyGetDirTrg(0) & DEMO_STICK_DOWN) {
                        wp->scenario = 2;
                    }
                    break;

                case 1:
                    if (keyGetDirTrg(0) & DEMO_STICK_LEFT) {
                        wp->scenario = 0;
                    } else if (keyGetDirTrg(0) & DEMO_STICK_DOWN) {
                        wp->scenario = 2;
                    }
                    break;

                case 2:
                    if (stickY > 20) {
                        wp->scenario = 0;
                        if (stickX > 0) {
                            wp->scenario = 1;
                        }
                    }
                    break;
            }

#endif
            if (prev != wp->scenario) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
            } else if (keyGetButtonTrg(0) & PAD_BUTTON_B || OSTicksToSeconds(OSGetTime() - none_key) > 120) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1));
                fadeEntry(MARIO_HEAD_FADE_IN, 0, (GXColor){ 0x00, 0x00, 0x00, 0xFF });
                fadeSetOffscreenCallback(e3DispOffscreen, seq);
                wp->nextScreen = 0;
                seq->state++;
            } else if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1));
                seq->state = 100;
            }
        } break;

        case 5:
            if (fadeIsFinish()) {
                fadeEntry(MARIO_HEAD_FADE_OUT, 0, (GXColor){ 0x00, 0x00, 0x00, 0xFF });
                seq->state = 0;
            }
            break;

        case 100:
            fadeEntry(FADE_OUT_BLACK, 200, (GXColor){ 0x00, 0x00, 0x00, 0xFF });
            seq->state++;
            break;

        case 101:
            if (fadeIsFinish()) {
                seq->state++;
            }
            break;

        case 102: {
            prevId = 0;
            matched = FALSE;
            targetId = e3_tbl[wp->scenario].id;
            swInit();

            for (stageId = 0; stageId < eventStgNum(); stageId++) {
                EventStage* stage = eventStgDtPtr(stageId);
                EventData* data = stage->data;
                for (idx = 0; idx < stage->count; idx++, data++) {
                    if (targetId == data->id) {
                        matched = TRUE;
                        break;
                    }
                    prevId = data->id;
                }
                if (matched) {
                    break;
                }
            }
            swByteSet(0, prevId);
            foundIdx = idx;

            for (i = 0; i < stageId; i++) {
                EventStage* stage = eventStgDtPtr(i);
                for (idx = 0; idx < stage->count; idx++) {
                    if (stage->data[idx].initEvent) {
                        stage->data[idx].initEvent();
                    }
                }
            }

            found = eventStgDtPtr(stageId)->data;
            {
                // FAKE: a second loop counter; reusing the function's i changes the object
                s32 j;
                for (j = 0; j <= foundIdx; j++) {
                    if (found[j].initEvent) {
                        found[j].initEvent();
                    }
                }
            }

            switch (found[foundIdx].motionType) {
                case 0:
                    marioChgStayMotion();
                    break;

                case 1:
                    marioChgShipMotion();
                    break;
            }

            marioPartyKill();

            {
                MarioWork* mario = marioGetPtr();
                mario->prevPartyMemberId[0] = 0;
                mario->prevPartyMemberId[1] = 0;
            }

            if (found[foundIdx].partners[0]) {
                marioPartyEntry(found[foundIdx].partners[0]);
            }
            if (found[foundIdx].partners[1]) {
                marioPartyEntry(found[foundIdx].partners[1]);
            }

            if (wp->scenario == 0) {
#if VERSION >= VERSION_US
                seqSetSeq(SEQ_MAP_CHANGE, "gor_00", NULL);
            } else if (wp->scenario == 1) {
#endif
                seqSetSeq(SEQ_MAP_CHANGE, "hei_00", "dokan_2");
            } else {
                seqSetSeq(SEQ_MAP_CHANGE, found[foundIdx].mapName, found[foundIdx].beroName);
            }

            memset(pouchGetPtr()->heldItems, 0, sizeof(s16) * MAX_HELD_ITEMS);
            memset(pouchGetPtr()->storedItems, 0, sizeof(s16) * MAX_STORED_ITEMS);
            memset(pouchGetPtr()->badges, 0, sizeof(s16) * MAX_BADGES);
            memset(pouchGetPtr()->equippedBadges, 0, sizeof(s16) * MAX_BADGES);
            pouchSetMaxHP(20);
            pouchSetHP(20);
            pouchSetMaxFP(20);
            pouchSetFP(20);

            for (i = 0; i < PARTNER_COUNT; i++) {
                pouchGetPtr()->partyData[i].currentHP = 20;
                pouchGetPtr()->partyData[i].HPLevel = 1;
            }

#if VERSION == VERSION_EU
            if (wp->scenario != 0) {
#endif
            for (i = 0; i < 2; i++) {
                pouchGetItem(ITEM_THUNDER_RAGE);
                pouchGetItem(ITEM_SHOOTING_STAR);
                pouchGetItem(ITEM_FIRE_FLOWER);
                pouchGetItem(ITEM_POW_BLOCK);
            }

#if VERSION == VERSION_EU
            }
#endif
            pouchGetPtr()->availableBP = 20;
            pouchGetPtr()->maximumBP = 20;
#if VERSION == VERSION_EU
            if (wp->scenario != 0) {
#endif
            pouchGetItem(ITEM_POWER_BOUNCE);
            pouchGetItem(ITEM_POWER_SMASH);
            pouchEquipBadgeID(ITEM_POWER_BOUNCE);
            pouchEquipBadgeID(ITEM_POWER_SMASH);
#if VERSION == VERSION_EU
            }
#endif
            pouchReviseMarioParam();
            pouchRevisePartyParam();
            statusWinForceUpdate();
        } break;
    }

    wp->dstX = e3_tbl[wp->scenario].x;
    wp->dstY = e3_tbl[wp->scenario].y;
    wp->curX += (wp->dstX - wp->curX) / 4;
    wp->curY += (wp->dstY - wp->curY) / 4;
    dispEntry(CAMERA_3D, 0.0f, 0, e3Disp, seq);
}

void e3Disp(CameraId cameraId, void* param) {
    CameraEntry* camera = camGetPtr(CAMERA_2D);
    Mtx mtx;
    Mtx mtxScale;
    f32 projBackup[GX_PROJECTION_SZ];
    f32 viewportBackup[GX_VIEWPORT_SZ];
    CameraEntry camBackup;
    s32 active;
    s32 alpha;
    s32 i;
    s32 dy, x;
    s32 col, row;
    s32 inset, pass, step;

    active = wp->curScreen;
    if (cameraId == CAMERA_OFFSCREEN2) {
        active = 1 - wp->curScreen;
    }

    // Draw with the 2D camera: save the current camera and the GX projection/viewport (restored at the end)
    camBackup = *camGetPtr(cameraId);
    *camGetPtr(cameraId) = *camera;

    GXGetProjectionv(projBackup);
    GXGetViewportv(viewportBackup);
    GXSetProjection(camera->projection, camera->projType);
    GXSetViewport(camera->viewport[0], camera->viewport[1], camera->viewport[2], camera->viewport[3],
                  camera->viewport[4], camera->viewport[5]);

    if (active == 0) {
        // 'Press Start' overlay with alpha pulsing
        alpha = (s32)((f32)sin((f64)((f32)gp->retraceCount / 30.0f)) * 459.0f);
#if VERSION == VERSION_EU
        {
            // PAL: per-language title texture
            s32 tbl[6] = {11, 11, 12, 13, 14, 15};
#endif
        if (alpha < 0) {
            alpha = 0;
        }
        if (alpha > 255) {
            alpha = 255;
        }

        winTexInit(*wp->handle->data);
#if VERSION >= VERSION_US
#if VERSION == VERSION_EU
            winTexSet(tbl[gp->language], (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#else
        winTexSet(12, (Vec){ 0.0f, 0.0f, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
#endif
#else
        winTexSet(MODE_11, (Vec){ 0.0f, 0.0f, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
#endif
        pressStartGX(alpha, 0.0f, -110.0f);
#if VERSION == VERSION_EU
        }
#endif
    } else {
        // Tiled background: 40 columns x 32 rows of 16-unit tiles, from x = -304 and down from y = 240
        winTexInit(*wp->handle->data);
        for (col = 0, x = -304; col < 40; col++, x += 16) {
            for (row = 0, dy = 0; row < 32; row++, dy += 16) {
                winTexSet(MODE_5, (Vec){ (f32)x, (f32)(240 - dy), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f },
                          (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
        }

#if VERSION >= VERSION_US
        for (i = 0; i < 4; i++) {
#else
        for (i = 0; i < 3; i++) {
#endif
            winTexInit(*wp->handle->data);

            if (i == wp->scenario) {
                // FAKE: the (u32) cast on the 8; plain i + 8 changes the object
                // Shadow Box
                winTexSet(i + (u32)MODE_8, (Vec){ 10.0f + e3_tbl[i].x, e3_tbl[i].y - 10.0f, 0.0f },
                          (Vec){ 0.35f, 0.35f, 0.35f }, (GXColor){ 0x00, 0x00, 0x00, 0x40 });
                // Content Box
                winTexSet(i + MODE_8, (Vec){ e3_tbl[i].x, e3_tbl[i].y, 0.0f }, (Vec){ 0.35f, 0.35f, 0.35f },
                          (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            } else {
                winTexSet(i + MODE_8, (Vec){ 10.0f + e3_tbl[i].x, e3_tbl[i].y - 10.0f, 0.0f }, (Vec){ 0.35f, 0.35f, 0.35f },
                          (GXColor){ 0x80, 0x80, 0x80, 0xFF });
            }

            {
                char* keys[] = {
#if VERSION >= VERSION_US
                    "menu_monosiri_gor",
#endif
                    "menu_monosiri_hei", "menu_monosiri_gon", "menu_monosiri_win"
                };
                step = (i == wp->scenario) ? 0 : 10;

                windowDispGX_Waku_col(0, (GXColor){ 0x4A, 0x56, 0xB5, 0xFF }, (((f32)step + e3_tbl[i].x) - 100.0f),
                                      ((e3_tbl[i].y - (f32)step) - 60.0f), 200.0f, 40.0f, 20.0f);

                // Centered Label
                FontDrawStart();
                FontDrawColor((GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                MTXTrans(mtx, ((f32)step + e3_tbl[i].x) - 0.7f * (f32)(s32)((FontGetMessageWidth(msgSearch(keys[i])) >> 1) & 0x7FFF),
                         (((e3_tbl[i].y - (f32)step) - 60.0f) - 8.0f), 0.0f);
                MTXScale(mtxScale, 0.7f, 1.0f, 1.0f);
                MTXConcat(mtx, mtxScale, mtx);
                FontDrawStringMtx(mtx, msgSearch(keys[i]));
            }
        }

        // Two passes of four mirrored window corners (shadow and content)
        winTexInit(*wp->handle->data);
        for (pass = 1, inset = 5; pass >= 0; pass--, inset -= 5) {
            // Top-Left
            winTexSet(((gp->retraceCount >> 4) & 1) ? MODE_6 : MODE_7,
                      (Vec){ (110.0f + wp->curX) + (f32)inset, (90.0f + wp->curY) - (f32)inset, 0.0f },
                      (Vec){ 1.0f, 1.0f, 1.0f },
                      (pass != 0) ? (GXColor){ 0x00, 0x00, 0x00, 0x40 } : (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            // Top-Right
            winTexSet(((gp->retraceCount >> 4) & 1) ? MODE_6 : MODE_7,
                      (Vec){ (wp->curX - 110.0f) + (f32)inset, (90.0f + wp->curY) - (f32)inset, 0.0f },
                      (Vec){ -1.0f, 1.0f, 1.0f },
                      (pass != 0) ? (GXColor){ 0x00, 0x00, 0x00, 0x40 } : (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            // Bottom-Left
            winTexSet(((gp->retraceCount >> 4) & 1) ? MODE_6 : MODE_7,
                      (Vec){ (110.0f + wp->curX) + (f32)inset, (wp->curY - 90.0f) - (f32)inset, 0.0f },
                      (Vec){ 1.0f, -1.0f, 1.0f },
                      (pass != 0) ? (GXColor){ 0x00, 0x00, 0x00, 0x40 } : (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            // Bottom-Right
            winTexSet(((gp->retraceCount >> 4) & 1) ? MODE_6 : MODE_7,
                      (Vec){ (wp->curX - 110.0f) + (f32)inset, (wp->curY - 90.0f) - (f32)inset, 0.0f },
                      (Vec){ -1.0f, -1.0f, 1.0f },
                      (pass != 0) ? (GXColor){ 0x00, 0x00, 0x00, 0x40 } : (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
        }
    }

    // Restore all the GX and camera data we backed up
    *camGetPtr(cameraId) = camBackup;
    GXSetProjectionv(projBackup);
    GXSetViewport(viewportBackup[0], viewportBackup[1], viewportBackup[2], viewportBackup[3], viewportBackup[4],
                  viewportBackup[5]);
}

// FAKE: stand-in that nothing calls (the linker strips it); it only places 30.0f in the constant pool (JP and US); removing it changes the object
f32 forceSdata(void) {
    return 30.0f;
}

/**
 * Page-flip offscreen callback: switches to the next page, draws it,
 * and registers its drawn bounding box with the fOff_peron offscreen.
 */
void e3DispOffscreen(CameraId cameraId, void* param) {
    u16 left, top, right, bottom;

    wp->curScreen = wp->nextScreen;
    sysWaitDrawSync();
    GXClearBoundingBox();
    e3Disp(cameraId, param);
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(offscreenNameToId("fOff_peron"), left, top, right, bottom);
}

#if VERSION >= VERSION_US // US: local here, seq_title has the global copy
static
#endif
void pressStartGX(s32 alpha, f32 x, f32 y) {
    CameraEntry* camera = camGetCurPtr();
    GXTexObj startText, gradient;
    Mtx mtx, scale;

#if VERSION == VERSION_EU
    s32 tbl[6] = {16, 16, 17, 18, 19, 20};
    TEXGetGXTexObjFromPalette(*wp->handle->data, &startText, tbl[gp->language]);
#else
    TEXGetGXTexObjFromPalette(*wp->handle->data, &startText, 3);
#endif
    GXLoadTexObj(&startText, GX_TEXMAP0);
#if VERSION == VERSION_EU
    TEXGetGXTexObjFromPalette(*wp->handle->data, &gradient, 3);
#else
    TEXGetGXTexObjFromPalette(*wp->handle->data, &gradient, 4);
#endif
    GXLoadTexObj(&gradient, GX_TEXMAP1);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0x00, 0x00, 0x00, alpha });
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    MTXScale(scale, 1.0f, 1.0f, 1.0f);
    MTXTrans(mtx, (f32)gp->retraceCount / -200.0f, 0.0f, 0.0f);
    MTXConcat(scale, mtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    MTXTrans(mtx, x, y, 0.0f);
    MTXScale(scale, (f32)GXGetTexObjWidth(&startText), (f32)GXGetTexObjHeight(&startText), 1.0f);
    MTXConcat(mtx, scale, mtx);
    MTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(0.5f, 0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(-0.5f, 0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}
