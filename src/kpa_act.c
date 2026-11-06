// Bowser interlude score display: coins and score counters.

#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "driver/dispdrv.h"
#include "mario/mario.h"
#include "mario/mario_pouch.h"
#include "memory.h"
#include <string.h>
#include "kpa_act.h"

typedef struct KpaScore {
    char str[8];
    u8 timer;
    u8 pad_9[3];
    Vec pos;
} KpaScore;

#define KPA_SCORE_SLOT_COUNT 50
#define KPA_COIN_ROLLOVER_BONUS 1000
#define KPA_SCORE_PER_COIN 500
#define KPA_SCORE_PICKUP_CAP 10000

KpaScore* ksdp;
USER_FUNC(kpa_score_disp_main);
/** Endless loop that runs kpa_score_disp_main once a frame, keeping the Super Bowser score popups moving. */
EVT_BEGIN(kpa_score_disp_evt)
    LOOP(0)
        CALL(kpa_score_disp_main)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static void kpaScoreDisp(CameraId cameraId, void* param) {
    Mtx m;
    Mtx scale;
    s32 i;
    PSMTXScale(scale, 0.5f, 0.5f, 0.5f);
    FontDrawStart();
    FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    for (i = 0; i < KPA_SCORE_SLOT_COUNT; i++) {
        if (ksdp[i].timer != 0) {
            PSMTXTrans(m, ksdp[i].pos.x, ksdp[i].pos.y, 300.0f);
            PSMTXConcat(m, scale, m);
            FontDrawStringMtx(m, ksdp[i].str);
        }
    }
}

/**
 * Script helper run every frame by kpa_score_disp_evt:
 * floats each active score popup up one unit, counts down its 30-frame timer and queues kpaScoreDisp.
 */
USER_FUNC(kpa_score_disp_main) {
    s32 i;
    for (i = 0; i < KPA_SCORE_SLOT_COUNT; i++) {
        if (ksdp[i].timer != 0) {
            ksdp[i].pos.y += 1.0f;
            ksdp[i].timer--;
        }
    }
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_XLU, kpaScoreDisp, 0);
    return EVT_RETURN_DONE;
}

/**
 * Script helper for the Super Bowser stages: starts kpa_score_disp_evt as a child that copies the caller's LW/LF
 * variables and owner, and allocates and clears the 50 score popup slots on the map heap. No arguments.
 */
USER_FUNC(kpa_score_disp_init) {
    EventEntry* e;
    u32 i;
    e = evtEntryType(kpa_score_disp_evt, event->priority, 0, event->type);
    e->unitId = event->unitId;
    e->ownerNPC = event->ownerNPC;
    for (i = 0; i < 16; i++) {
        e->lwData[i] = event->lwData[i];
    }
    for (i = 0; i < 3; i++) {
        e->lfData[i] = event->lfData[i];
    }
    e->uwBase = event->uwBase;
    e->ufBase = event->ufBase;
    ksdp = _mapAlloc(KPA_SCORE_SLOT_COUNT * sizeof(KpaScore));

    // FAKE: (s32) cast on the u32 counter (signed compare); the plain `i < 50` changes the object
    for (i = 0; (s32)i < KPA_SCORE_SLOT_COUNT; i++) {
        ksdp[i].str[0] = 0;
        ksdp[i].timer = 0;
        ksdp[i].pos = (Vec){0.0f, 0.0f, 0.0f};
    }
    return EVT_RETURN_DONE;
}

/** Adds coins to Bowser's coin count; when it rolls over, also adds the rollover bonus to his score. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void _kpaAddCoin(s32 coins) {
    if (pouchAddKpaCoin(coins)) {
        pouchAddKpaScore(KPA_COIN_ROLLOVER_BONUS);
    }
}

/**
 * Adds Super Bowser coins: every 100 coins also gives 1000 points, and each pickup scores 500 per coin,
 * capped at
 * 10000. Called when a Bowser-stage ? block gives a coin (evt_mobj).
 */
void kpaAddCoin(s32 coins) {
    s32 score;
    _kpaAddCoin(coins);
    score = coins * KPA_SCORE_PER_COIN;
    if (score > KPA_SCORE_PICKUP_CAP) {
        score = KPA_SCORE_PICKUP_CAP;
    }
    pouchAddKpaScore(score);
}

/**
 * Takes the first free popup slot and shows score (without leading zeros) centred 20 units above pos for 30
 * frames; does nothing when all 50 are in use.
 */
static inline void kpaSetScoreStr(s32 score, Vec* pos) {
    char* p;
    char* str;
    s32 t;
    s8 d[5];
    s32 j;
    s32 n;
    s32 i;
    for (i = 0; i < KPA_SCORE_SLOT_COUNT; i++) {
        if (ksdp[i].timer == 0) {
            d[0] = score / 10000;
            t = score % 10000;
            d[1] = t / 1000;
            t %= 1000;
            d[2] = t / 100;
            t %= 100;
            d[3] = t / 10;
            d[4] = t % 10;
            p = ksdp[i].str;

            // FAKE: str is a spare second pointer to ksdp[i].str (only writes the terminator); ksdp[i].str[n] changes the object
            str = ksdp[i].str;
            n = 0;
            for (j = 0; j < 5; j++) {
                if (n != 0 || d[j] != 0) {
                    *p++ = d[j] + '0';
                    n++;
                }
            }
            str[n] = 0;
            ksdp[i].timer = 30;
            ksdp[i].pos.x = pos->x - 0.5f * (10.0f * (f32)strlen(ksdp[i].str));
            ksdp[i].pos.y = 20.0f + pos->y;
            ksdp[i].pos.z = 0.0f;
            break;
        }
    }
}

/**
 * Same as kpaAddCoin, and also shows the points scored as a popup above pos.
 * kpa.rel's kpa_add_coin calls it for each stage coin Bowser picks up (kpa_coin_evt).
 */
void kpaAddCoinPos(s32 coins, Vec* pos) {
    s32 score;
    _kpaAddCoin(coins);
    score = coins * KPA_SCORE_PER_COIN;
    if (score > KPA_SCORE_PICKUP_CAP) {
        score = KPA_SCORE_PICKUP_CAP;
    }
    pouchAddKpaScore(score);
    kpaSetScoreStr(score, pos);
}

/** Adds score points, capped at 10000 per call, to the Super Bowser total and returns the points added. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
s32 _kpaAddScore(s32 score) {
    if (score > KPA_SCORE_PICKUP_CAP) {
        score = KPA_SCORE_PICKUP_CAP;
    }
    pouchAddKpaScore(score);
    return score;
}

/** Adds score points (capped at 10000 per call) to the Super Bowser total. */
void kpaAddScore(s32 score) {
    _kpaAddScore(score);
}

/**
 * Adds score points (capped at 10000 per call) to the Super Bowser total and shows them as a popup above pos.
 * Used for stomped enemies (npcdrv) and power-ups (evt_mobj).
 */
void kpaAddScorePos(s32 score, Vec* pos) {
    s32 sc = score;
    if (score > KPA_SCORE_PICKUP_CAP) {
        sc = KPA_SCORE_PICKUP_CAP;
    }
    pouchAddKpaScore(sc);
    kpaSetScoreStr(sc, pos);
}

/** Script helper: switches the player character's (Bowser's) animation to the pose named by args[0]. */
USER_FUNC(kpa_chg_pose) {
    marioChgPose((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}
