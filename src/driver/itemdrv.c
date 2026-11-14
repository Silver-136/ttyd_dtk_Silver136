// Item driver: items on the field (drops, coins, pickups): spawning, bouncing, pickup and display.

#include "version.h"
#include "evt/evt_msg.h"
#include "driver/itemdrv.h"
#include "eff/eff_itemget.h"
#include "driver/swdrv.h"
#include "icon_ids.h"
#include <dolphin/pad.h>
#include "driver/icondrv.h"
#include "driver/windowdrv.h"
#include "manager/evtmgr.h"
#include "driver/camdrv.h"
#include "driver/mobjdrv.h"
#include "eff/eff_stardust.h"
#include "pmario_sound.h"
#include "driver/effdrv.h"
#include "driver/hitdrv.h"
#include "driver/msgdrv.h"
#include "driver/seqdrv.h"
#include "data/item_data.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "manager/winmgr.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "party.h"
#include "party/party_vivian.h"
#include "memory.h"
#include <math.h>
#include "system.h"
#include <string.h>
#include "evt/evt_lecture.h"
#include "libc_decls.h"
#include <math.h>
#include "sfx_name_word.h"
#include "mario/mario_motion.h"
#include "driver/npcdrv.h"
#include "mot/mot_jabara.h"
#include "eff/eff_starstone.h"
#include "eff/eff_splash.h"
#include "party/party_nokonoko.h"
#include "mario/mario_cam.h"
#include "eff/n64/eff_hagetaka_flush_n64.h"

/* unused; its zero Vec template still lands in the constant pool / .rodata (removing it changes the object) */
inline Vec unknownVec0(void) {
    return (Vec){ 0.0f, 0.0f, 0.0f };
}

static void winNameDisp(WinMgrEntry* win);
static void winNameDisp2(WinMgrEntry* win);
static void winHelpMain(WinMgrEntry* win);
static void winHelpDisp(WinMgrEntry* win);
static void winFullDisp(WinMgrEntry* win);

static HitEntry* itemHitCheckSide(ItemEntry* entry, f32* outX, f32 dx, f32* outZ, f32 dz, s32* angle);
static void itemseq_GetItem(ItemEntry* entry);

//.bss
static ItemWork work[2];

//.data
WinMgrDesc itemdrv_window_desc[4] = {
    {0, 0, 8, -175, 120, 350, 42, {0xE1, 0xD7, 0x9B, 0xFF}, NULL, winNameDisp},
    {0, 0, 8, -240, -130, 500, 80, {0xF0, 0xF0, 0xF0, 0xFF}, winHelpMain, winHelpDisp},
    {0, 0, 8, -175, 200, 350, 75, {0x2B, 0xA4, 0xFF, 0xFF}, NULL, winFullDisp},
    {0, 0, 8, -175, 120, 350, 42, {0xE1, 0xD7, 0x9B, 0xFF}, NULL, winNameDisp2},
};

// Always inlined: the symbol map has no itemGetWork function.
static inline ItemWork* itemGetWork(void) {
    return gp->inBattle ? &work[1] : &work[0];
}

/**
 * Puts a removed badge item on sale at the badge shop through pouchArriveBadge:
 * the early demos do so when ITEM_FLAG_LOST_BADGE_TO_SHOP (0x100) is clear, in or out of battle;
 * JP and later (the September demos too) do so only outside battle and when the same bit is set.
 */
inline void itemCheckBadge(ItemEntry* entry) {
#if VERSION <= VERSION_DEMO_JP
    if (BADGE_MIN <= entry->itemId && entry->itemId < BADGE_MAX && !(entry->flags & ITEM_FLAG_LOST_BADGE_TO_SHOP)) {
        pouchArriveBadge(entry->itemId);
    }
#else
    if (!gp->inBattle) {
        if (BADGE_MIN <= entry->itemId && entry->itemId < BADGE_MAX) {
            if (entry->flags & ITEM_FLAG_LOST_BADGE_TO_SHOP) {
                pouchArriveBadge(entry->itemId);
            }
        }
    }
#endif
}

static void itemseq_Bound(ItemEntry* entry);

/**
 * Ends an item get sequence: gives Mario back his controls and keys and unmutes the sound effects.
 * Later versions also undo the control lock if the get set it.
 */
static inline void itemGetEnd(ItemEntry* entry) {
    marioStSystemLevel(0);
    psndClearFlag(PSND_FLAG_MUTE_SFX);

#if VERSION >= VERSION_JP
    if (entry->flags2 & ITEM_FLAG2_CTRL_OFF) {
        entry->flags2 &= ~ITEM_FLAG2_CTRL_OFF;
        marioCtrlOn();
    }
#endif

    marioKeyOn();
    marioChgStayMotion();
    camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
}

void itemPlaySound(ItemEntry* entry);

/** Allocates the 32-entry field and battle item tables; called once at boot. */
void itemInit(void) {
    ItemWork* wp;

    wp = &work[0];
    wp->count = 32;
    wp->entries = __memAlloc(HEAP_DEFAULT, sizeof(ItemEntry) * wp->count);
    memset(wp->entries, 0, sizeof(ItemEntry) * wp->count);
    wp->currId = 0;

    wp = &work[1];
    wp->count = 32;
    wp->entries = __memAlloc(HEAP_DEFAULT, sizeof(ItemEntry) * wp->count);
    memset(wp->entries, 0, sizeof(ItemEntry) * wp->count);
    wp->currId = 0;
}

/**
 * Clears the current item table on map change; on the field it first puts eligible dropped badges
 * on sale at the badge shop, using itemCheckBadge's per-version flag test.
 */
void itemReInit(void) {
    ItemWork* wp = itemGetWork();
    ItemEntry* entry;
    int i;

    if (!gp->inBattle) {
        for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
            if (entry->flags & ITEM_FLAG_ACTIVE) { // eligible badges go back to the shop (itemCheckBadge's per-version flag test)
                itemCheckBadge(entry);
            }
        }
    }

    // clear all entries
    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        iconDelete(entry->name);
        entry->flags &= ~ITEM_FLAG_ACTIVE;
        memset(entry->name, 0, sizeof(entry->name));
    }
}

/**
 * Takes one coin from Mario and drops it as a coin item that scatters and vanishes after 8 seconds;
 * mario.c calls it when Mario loses coins.
 */
void itemCoinDrop(Vec* position) {
    ItemEntry* entry;

    if (pouchGetCoin() > 0) {
        pouchAddCoin(-1);
        entry = itemEntry(position->x, position->y, position->z, NULL, ITEM_COIN, ITEM_TYPE_DROP, -1, NULL);
        if (entry) {
            entry->flags2 |= ITEM_FLAG2_SPREAD;
        }
    }
}

/**
 * Returns whether Mario may pick up this item now:
 * not in water or lava, Mario has control (later versions skip that for Bowser when his stage flag bit 0 is
 * set), no map event or other sequence runs, Mario is not on the boat (or, JP and later, the plane panel), hiding with Vivian,
 * taking damage or holding Koops's item, no other get is in progress, and field battle state allows it.
 */
static BOOL itemGetCheck(ItemEntry* entry) {
#if VERSION <= VERSION_DEMO_JP
    s32 fbatMode = fbatGetPointer()->mode;
#endif
    MarioWork* mario = marioGetPtr();
    ItemWork* wp;
    ItemEntry* item;
    int i;

#if VERSION <= VERSION_DEMO_JP
    if (!entry) {
        return FALSE;
    }
    if (entry->flags2 & ITEM_FLAG2_SUNK) {
        return FALSE;
    }
    if (marioStGetSystemLevel() != 0) {
        return FALSE;
    }
    if (mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF)) {
        return FALSE;
    }
    if (gp->mapEventId) {
        return FALSE;
    }
    if (seqGetSeq() != SEQ_GAME) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_SHIP) {
        return FALSE;
    }
    if (vivianGetStatus()) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
        return FALSE;
    }
    if (nokonoko_holdItem()) {
        return FALSE;
    }
#else
    if (!entry) {
        return FALSE;
    }
    if (entry->flags2 & ITEM_FLAG2_SUNK) {
        return FALSE;
    }
    if (mario->characterId == CHARACTER_BOWSER) {
        if (!(mario->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING)) {
            if (marioStGetSystemLevel() != 0) {
                return FALSE;
            }
            if (mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF)) {
                return FALSE;
            }
        }
    } else {
        if (marioStGetSystemLevel() != 0) {
            return FALSE;
        }
        if (mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF)) {
            return FALSE;
        }
    }
    if (gp->mapEventId) {
        return FALSE;
    }
    if (seqGetSeq() != SEQ_GAME) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_SHIP) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_PLANE) {
        return FALSE;
    }
    if (vivianGetStatus()) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
        return FALSE;
    }
    if (nokonoko_holdItem()) {
        return FALSE;
    }
#endif
    wp = itemGetWork();
    for (item = wp->entries, i = 0; i < wp->count; i++, item++) {
        if (item->flags & ITEM_FLAG_ACTIVE && item->mode == ITEM_MODE_GET) {
            return FALSE;
        }
    }
#if VERSION <= VERSION_DEMO_JP
    if (fbatMode != FBAT_MODE_OFF && fbatMode != FBAT_MODE_FIELD) {
        return FALSE;
    }
#else
    if (fbatPreventMarioEventChk()) {
        return FALSE;
    }
#endif
    switch (entry->mode) {
        case ITEM_MODE_SCATTER:
        case ITEM_MODE_SCATTER_NO_PICKUP:
            if (!entry->subState) break;
            if (entry->flags2 & ITEM_FLAG2_NO_PICKUP_RISING) break;
            if (entry->flags2 & ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND) break;
            return TRUE;
        case ITEM_MODE_FALL:
        case ITEM_MODE_FALL_NO_PICKUP:
            if (entry->flags2 & ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND) break;
            return TRUE;
        case ITEM_MODE_GROUND:
        case ITEM_MODE_STILL:
            return TRUE;
    }
    return FALSE;
}

/** Like itemGetCheck for an item fetched by Koops's shell: whether the item may be collected for Mario now. */
static BOOL itemGetNokoCheck(ItemEntry* entry) {
#if VERSION <= VERSION_DEMO_JP
    s32 fbatMode = fbatGetPointer()->mode;
#endif

    MarioWork* mario = marioGetPtr();

#if VERSION <= VERSION_DEMO_JP
    if (!entry) {
        return FALSE;
    }
    if (entry->flags2 & ITEM_FLAG2_SUNK) {
        return FALSE;
    }
    if (marioStGetSystemLevel() != 0) {
        return FALSE;
    }
    if (mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF)) {
        return FALSE;
    }
    if (gp->mapEventId) {
        return FALSE;
    }
    if (seqGetSeq() != SEQ_GAME) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_SHIP) {
        return FALSE;
    }
    if (vivianGetStatus()) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_JUMP_SW || mario->currMotionId == MARIO_MOTION_JUMP_STAND ||
        mario->currMotionId == MARIO_MOTION_JUMP_2 || mario->currMotionId == MARIO_MOTION_JUMP_3 ||
        mario->currMotionId == MARIO_MOTION_GET_ITEM || mario->currMotionId == MARIO_MOTION_TALK) {
        return FALSE;
    }
#else
    if (!entry) {
        return FALSE;
    }
    if (entry->flags2 & ITEM_FLAG2_SUNK) {
        return FALSE;
    }
    if (marioStGetSystemLevel() != 0) {
        return FALSE;
    }
    if (mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF)) {
        return FALSE;
    }
    if (entry->flags2 & ITEM_FLAG2_NO_PARTNER) {
        return FALSE;
    }
    if (gp->mapEventId) {
        return FALSE;
    }
    if (seqGetSeq() != SEQ_GAME) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_SHIP) {
        return FALSE;
    }
    if (vivianGetStatus()) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_JUMP_SW || mario->currMotionId == MARIO_MOTION_JUMP_STAND ||
        mario->currMotionId == MARIO_MOTION_JUMP_2 || mario->currMotionId == MARIO_MOTION_JUMP_3 ||
        mario->currMotionId == MARIO_MOTION_GET_ITEM || mario->currMotionId == MARIO_MOTION_TALK) {
        return FALSE;
    }
#endif
    {
        ItemWork* wp = itemGetWork();
        ItemEntry* item;
        int i;
        for (item = wp->entries, i = 0; i < wp->count; i++, item++) {
            if (item->flags & ITEM_FLAG_ACTIVE && item->mode == ITEM_MODE_GET) {
                return FALSE;
            }
        }
    }
#if VERSION <= VERSION_DEMO_JP
    if (fbatMode != FBAT_MODE_OFF && fbatMode != FBAT_MODE_FIELD) {
        return FALSE;
    }
    return TRUE;
#else
    return !fbatPreventMarioEventChk();
#endif
}

/**
 * Returns TRUE while any item is in its get sequence (mode 2); NPC updates, shops and menus wait on it.
 * Descriptive name (not in the symbol maps).
 */
BOOL itemPickUpFromFieldCheck(void) {
    ItemWork* wp = itemGetWork();
    ItemEntry* entry;
    int i;

    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & ITEM_FLAG_ACTIVE && entry->mode == ITEM_MODE_GET) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Per-frame update of the field items: despawn timers and blinking before an item vanishes, then per mode:
 * resting on the floor (1), falling (3, 4), bouncing (5, 6, 10),
 * being collected (2) or carried by a partner (8); checks Mario touching items, queues drawing and shadows.
 */
void itemMain(void) {
    ItemWork* wp = itemGetWork();
    MarioWork* mario;
    ItemEntry* entry;
    PartyEntry* party;
    HitEntry* hit;
    BOOL active;
    u16 mode;
    int i;
    f32 newY;
    f32 y;
    s32 dx, dy;
    f32 dirY;

    mario = marioGetPtr();
    entry = itemHitCheck(mario->position.x, mario->position.y, mario->position.z, mario->wPlayerCollisionBox.x * 0.5f + 12.0f);
    if (itemGetCheck(entry)) {
        itemModeChange(entry, ITEM_MODE_GET);
    }

    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (!(entry->flags & ITEM_FLAG_ACTIVE)) {
            continue;
        }
        mode = entry->mode;
        active = TRUE;
        switch (mode) {
            case ITEM_MODE_GET:
            case ITEM_MODE_STILL:
            case ITEM_MODE_CARRIED:
            case ITEM_MODE_POP:
                break;
            default:
                if (gp->systemLevelFlags & SYSTEM_LEVEL_STOP_ALL) {
                    active = FALSE;
                }
                break;
        }
        if (!active) {
            entry->baseTime = gp->renderTime;
            continue;
        }
        if (mode != ITEM_MODE_GET) {
            if (entry->timer == 0) {
                entry->flags2 &= ~ITEM_FLAG2_HIDDEN;
            } else if (entry->flags2 & ITEM_FLAG2_TIMER_WAIT_LAND) {
                entry->flags2 &= ~ITEM_FLAG2_HIDDEN;
            } else {
                entry->timer -= (f32)OSTicksToMicroseconds(gp->renderTime - entry->baseTime) / 1000.0f;
                if (entry->timer <= 0) {
                    itemModeChange(entry, ITEM_MODE_DELETE);
                } else if (entry->timer <= 3000) {
                    if ((entry->timer >> 4) & 1) {
                        entry->flags2 ^= ITEM_FLAG2_HIDDEN;
                    }
                }
            }
        } else {
            entry->flags2 &= ~ITEM_FLAG2_HIDDEN;
        }

        switch (entry->mode) {
            case ITEM_MODE_SCATTER:
            case ITEM_MODE_SCATTER_NO_PICKUP:
            case ITEM_MODE_POP:
                itemseq_Bound(entry);
                break;
            case ITEM_MODE_FALL:
            case ITEM_MODE_FALL_NO_PICKUP:
                itemseq_Bound(entry);
                break;
            case ITEM_MODE_GROUND:
                y = entry->position.y;
                dirY = -1.0f;
                if (entry->flags2 & ITEM_FLAG2_SUNK) {
                    hit = NULL;
                    newY = y + dirY;
                } else {
                    f32 hitNZ, hitNY, hitNX, hitDist, hitZ, hitY, hitX;
                    f32 px = entry->position.x, py = entry->position.y + 20.0f, pz = entry->position.z;

                    hitDist = 20.0 + fabs(-1.0);
                    hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, px, py, pz, 0.0f, -1.0f, 0.0f);
#if VERSION <= VERSION_DEMO_JP
                    hit = hit ? hit : NULL;
#else
                    if (hit) {
                        newY = hitY;
                    } else {
                        hit = NULL;
                        newY = y + dirY;
                    }
#endif
                }
#if VERSION >= VERSION_JP
                if (!hit) {
                    itemModeChange(entry, ITEM_MODE_FALL);
                } else {
                    entry->position.y = newY;
                }
#else
                if (!hit) {
                    itemModeChange(entry, ITEM_MODE_FALL);
                }
#endif
                break;
            case ITEM_MODE_GET:
                itemseq_GetItem(entry);
                break;
            case ITEM_MODE_CARRIED:
                party = partyGetPtr(marioGetPartyId());
                if (party) {
                    f32 px = party->position.x, py = 15.0f + party->position.y, pz = party->position.z;
                    entry->position.x = px;
                    entry->position.y = py;
                    entry->position.z = pz;
                } else {
                    itemModeChange(entry, ITEM_MODE_FALL);
                    entry->flags2 &= ~ITEM_FLAG2_CARRIED;
                }
                break;
            case ITEM_MODE_DELETE:
#if VERSION <= VERSION_DEMO_JP
                itemCheckBadge(entry);
#endif
                itemDelete(entry->name);
                break;
        }

        if (entry->flags & ITEM_FLAG_ACTIVE) {
            switch (entry->itemId) {
                case ITEM_DIAMOND_STAR:
                case ITEM_EMERALD_STAR:
                case ITEM_GOLD_STAR:
                case ITEM_RUBY_STAR:
                case ITEM_SAPPHIRE_STAR:
                case ITEM_GARNET_STAR:
                case ITEM_CRYSTAL_STAR:
                    if (!(entry->flags2 & ITEM_FLAG2_HIDDEN)) {
                        ((EffStarStoneWork*)entry->effect->userdata)->position.x = entry->position.x;
                        ((EffStarStoneWork*)entry->effect->userdata)->position.y = 16.0f + entry->position.y;
                        ((EffStarStoneWork*)entry->effect->userdata)->position.z = entry->position.z;
                    } else {
                        ((EffStarStoneWork*)entry->effect->userdata)->position.x = entry->position.x;
                        ((EffStarStoneWork*)entry->effect->userdata)->position.y = -1000.0f;
                        ((EffStarStoneWork*)entry->effect->userdata)->position.z = entry->position.z;
                    }
                    break;
                default:
                    if (!(entry->flags2 & ITEM_FLAG2_HIDDEN)) {
                        iconSetPos(entry->name, entry->position.x, entry->position.y, entry->position.z);
                        iconSetScale(entry->name, entry->scale);
                        iconSetAlpha(entry->name, entry->alpha);
                    } else {
                        iconSetPos(entry->name, entry->position.x, -1000.0f, entry->position.z);
                    }
                    break;
            }
            entry->baseTime = gp->renderTime;
            if (entry->itemId == ITEM_COIN || entry->itemId == ITEM_PIANTA_TOKEN) {
                if (irand(1000) < 30) {
                    dx = 10 - irand(20);
                    dy = irand(20) + 10;
                    effHagetakaFlushN64Entry(0, 10, entry->position.x + dx, entry->position.y + dy, entry->position.z, 1.0f);
                }
            }
        }
    }

    wp = gp->inBattle ? &work[0] : &work[1];
    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & ITEM_FLAG_ACTIVE) {
            entry->baseTime = gp->renderTime;
        }
    }
}

/** Puts an item into mode (resting, collecting, falling, bouncing, carried, ...) and restarts that mode's sub-state. */
void itemModeChange(ItemEntry* entry, u16 mode) {
    entry->mode = mode;
    entry->subState = 0;
}

#if VERSION >= VERSION_US

/**
 * Returns the first replaceable dropped item (coin, heart, flower) with this item id,
 * so itemEntry can reuse its slot when the table is full; NULL if none.
 */
static inline ItemEntry* itemFindDeletable(ItemWork* wp, s32 itemId) {
    ItemEntry* entry;
    ItemEntry* found;
    int i;

    found = NULL;
    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & ITEM_FLAG_ACTIVE && entry->itemId == itemId && entry->flags & ITEM_FLAG_REPLACEABLE) {
            found = entry;
            break;
        }
    }
    return found;
}
#endif

/**
 * Spawns field item itemId at (x, y, z) named name (generated if NULL);
 * type (ItemEntryType) picks how it appears and how long it stays,
 * collectFlag is the GSWF or LSWF that marks it taken (-1 none), pickupEvt runs on collection.
 * When the table is full, US, PAL and the September demos may replace a dropped coin or flower; JP and the early demos only try
 * the first slot for a replaceable coin, flower or heart. Returns the entry or NULL.
 */
#if VERSION <= VERSION_DEMO_JP
ItemEntry* itemEntry(f32 x, f32 y, f32 z, const char* name, s32 itemId, u16 type, s32 collectFlag, EvtWord* pickupEvt) {
    char* nm;

#else
ItemEntry* itemEntry(f32 x, f32 y, f32 z, const char* name, s32 itemId, u16 type, s32 collectFlag, EvtWord* evtCode) {
#endif
    ItemWork* wp = itemGetWork();
    int i;
    ItemEntry* entry;
#if VERSION >= VERSION_US
    // FAKE: e/base and the explicit coin/flower search loops (the heart search uses the helper) steer register
    // numbering: the search pointer must be a named local and the entries base a named web.
    ItemEntry* e;
    ItemEntry* base;
#endif
    BOOL isGswf, isLswf;
    char itemName[8];

    isGswf = FALSE;
    isLswf = FALSE;
    if (collectFlag != -1) {
        if (collectFlag >= GSWF(0) && collectFlag <= EVTDAT_GSWF_MAX) {
            collectFlag += EVTDAT_GSWF_BASE;
            isGswf = TRUE;
            if (swGet(collectFlag)) {
                return NULL;
            }
        } else if (collectFlag >= LSWF(0) && collectFlag <= EVTDAT_LSWF_MAX) {
            collectFlag += EVTDAT_LSWF_BASE;
            isLswf = TRUE;
            if (_swGet(collectFlag)) {
                return NULL;
            }
        }
    }

    if (name == NULL) {
        wp->currId++;
        if (wp->currId >= 0xFFFFF) {
            wp->currId = 1;
        }
        sprintf(itemName, "_i%05x", wp->currId);
        name = itemName;
    }

    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & ITEM_FLAG_ACTIVE && !strcmp(entry->name, name)) {
            break;
        }
    }
#if VERSION >= VERSION_US
    base = wp->entries;
    for (entry = base, i = 0; i < wp->count; i++, entry++) {
#else
    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
#endif
        if (!(entry->flags & ITEM_FLAG_ACTIVE)) {
            break;
        }
    }

    if (i >= wp->count) {
#if VERSION <= VERSION_JP
        // BUG: with every slot in use, only the first slot is checked for a replaceable coin (and below, flower and
        // heart), so a replaceable item in any other slot is never freed; US and later search every slot.
        entry = wp->entries;
        if (entry->flags & ITEM_FLAG_ACTIVE && entry->itemId == ITEM_COIN && entry->flags & ITEM_FLAG_REPLACEABLE) {
            itemDelete(entry->name);
        } else {
            entry = wp->entries;
            if (entry->flags & ITEM_FLAG_ACTIVE && entry->itemId == ITEM_FLOWER && entry->flags & ITEM_FLAG_REPLACEABLE) {
                itemDelete(entry->name);
            } else {
                entry = wp->entries;
                if (entry->flags & ITEM_FLAG_ACTIVE && entry->itemId == ITEM_HEART && entry->flags & ITEM_FLAG_REPLACEABLE) {
                    itemDelete(entry->name);
                } else {
                    entry = NULL;
                }
            }
        }
#else
        entry = NULL;
        for (e = base, i = 0; i < wp->count; i++, e++) {
            if (e->flags & ITEM_FLAG_ACTIVE && e->itemId == ITEM_COIN && e->flags & ITEM_FLAG_REPLACEABLE) {
                entry = e;
                break;
            }
        }
        if (entry) {
            itemDelete(entry->name);
        } else {
            entry = NULL;
            for (e = base, i = 0; i < wp->count; i++, e++) {
                if (e->flags & ITEM_FLAG_ACTIVE && e->itemId == ITEM_FLOWER && e->flags & ITEM_FLAG_REPLACEABLE) {
                    entry = e;
                    break;
                }
            }
            if (entry) {
                itemDelete(entry->name);
            } else {
                // the heart found here is discarded: US and later never replace a heart
                entry = itemFindDeletable(wp, ITEM_HEART);
                entry = NULL;
            }
        }
#endif
        if (entry == NULL) {
            return NULL;
        }
    }

    memset(entry, 0, sizeof(ItemEntry));
    strcpy(entry->name, name);
    entry->position.x = x;
    entry->position.y = y;
    entry->position.z = z;
    entry->itemId = itemId;
    entry->type = type;
#if VERSION <= VERSION_DEMO_JP
    entry->evtCode = pickupEvt;
#else
    entry->evtCode = evtCode;
#endif
    entry->flags |= ITEM_FLAG_ACTIVE;
    entry->scale = 1.0f;
    entry->alpha = 0xFF;
    if (collectFlag != -1) {
        if (isGswf) {
            entry->collectFlag = collectFlag;
            entry->flags |= ITEM_FLAG_COLLECT_GSWF;
        } else if (isLswf) {
            entry->collectFlag = collectFlag;
            entry->flags |= ITEM_FLAG_COLLECT_LSWF;
        }
    } else {
        entry->collectFlag = -1;
    }

    switch (entry->itemId) {
        case ITEM_DIAMOND_STAR:
        case ITEM_EMERALD_STAR:
        case ITEM_GOLD_STAR:
        case ITEM_RUBY_STAR:
        case ITEM_SAPPHIRE_STAR:
        case ITEM_GARNET_STAR:
        case ITEM_CRYSTAL_STAR:
            entry->effect = effStarStoneEntry(entry->itemId - ITEM_DIAMOND_STAR, x, 16.0f + y, z, 1.0f);
            break;
        default:
#if VERSION <= VERSION_DEMO_JP
            iconEntry(entry->name, itemDataTable[itemId].icon_id);
            nm = entry->name;
            iconSetPos(nm, x, y, z);
#else
            entry->effect = NULL;
            iconEntry(entry->name, itemDataTable[itemId].icon_id);
            iconSetPos(entry->name, x, y, z);
#endif
            iconSetScale(entry->name, entry->scale);
            iconSetAlpha(entry->name, entry->alpha);
            break;
    }
    entry->bounceFactor = entry->gravityScale = 1.0f;
    entry->baseTime = gp->renderTime;
    switch (entry->itemId) {
        case ITEM_COIN:
        case ITEM_PIANTA_TOKEN:
        case ITEM_HEART:
        case ITEM_FLOWER:
            entry->flags |= ITEM_FLAG_QUICK_GET;
            break;
    }

    switch (type) {
        case ITEM_TYPE_STILL_NO_TOUCH:
            entry->flags |= ITEM_FLAG_NO_TOUCH_GET;
        case ITEM_TYPE_STILL:
            itemModeChange(entry, ITEM_MODE_STILL);
            break;
        case ITEM_TYPE_GROUND:
            itemModeChange(entry, ITEM_MODE_GROUND);
            break;
        case ITEM_TYPE_GROUND_NO_TOUCH:
            itemModeChange(entry, ITEM_MODE_GROUND);
            entry->flags |= ITEM_FLAG_NO_TOUCH_GET;
            break;
        case ITEM_TYPE_SCATTER_NO_PICKUP:
            itemModeChange(entry, ITEM_MODE_SCATTER_NO_PICKUP);
            break;
        case ITEM_TYPE_SCATTER_TIMED:
            entry->timer = 5000;
            itemModeChange(entry, ITEM_MODE_SCATTER);
            break;
        case ITEM_TYPE_DROP_LONG:
            entry->flags2 |= ITEM_FLAG2_TIMER_WAIT_LAND;
            entry->timer = 20000;
            itemModeChange(entry, ITEM_MODE_SCATTER_NO_PICKUP);
            break;
        case ITEM_TYPE_DROP_LONG_REPLACEABLE:
            entry->flags2 |= ITEM_FLAG2_TIMER_WAIT_LAND;
            entry->timer = 20000;
            itemModeChange(entry, ITEM_MODE_SCATTER_NO_PICKUP);
            entry->flags |= ITEM_FLAG_REPLACEABLE;
            break;
#if VERSION >= VERSION_JP
        case ITEM_TYPE_DROP_LOST_BADGE:
            entry->flags |= ITEM_FLAG_LOST_BADGE_TO_SHOP;
#endif
        case ITEM_TYPE_DROP:
            entry->flags2 |= ITEM_FLAG2_TIMER_WAIT_LAND;
            entry->timer = 8000;
            itemModeChange(entry, ITEM_MODE_SCATTER_NO_PICKUP);
            break;
        case ITEM_TYPE_DROP_REPLACEABLE:
            entry->flags2 |= ITEM_FLAG2_TIMER_WAIT_LAND;
            entry->timer = 8000;
            itemModeChange(entry, ITEM_MODE_SCATTER_NO_PICKUP);
            entry->flags |= ITEM_FLAG_REPLACEABLE;
            break;
        case ITEM_TYPE_SCATTER_NO_PICKUP_TIMED_LAND:
            entry->flags2 |= ITEM_FLAG2_TIMER_WAIT_LAND;
        case ITEM_TYPE_SCATTER_NO_PICKUP_TIMED:
            entry->timer = 5000;
            itemModeChange(entry, ITEM_MODE_SCATTER_NO_PICKUP);
            break;
        case ITEM_TYPE_SPREAD_TIMED:
            entry->timer = 5000;
            entry->flags2 |= ITEM_FLAG2_SPREAD;
            itemModeChange(entry, ITEM_MODE_SCATTER_NO_PICKUP);
            break;
        case ITEM_TYPE_SCATTER:
            itemModeChange(entry, ITEM_MODE_SCATTER);
            break;
        case ITEM_TYPE_FALL:
            itemModeChange(entry, ITEM_MODE_FALL);
            break;
        case ITEM_TYPE_FALL_NO_PICKUP:
            itemModeChange(entry, ITEM_MODE_FALL_NO_PICKUP);
            break;
        case ITEM_TYPE_POP:
            itemModeChange(entry, ITEM_MODE_POP);
            break;
    }
    return entry;
}

/**
 * Removes the named field item (and its name windows);
 * JP and later (the September demos too) first put a dropped badge on sale at the badge shop (itemCheckBadge).
 */
void itemDelete(const char* name) {
    ItemWork* wp = itemGetWork();
    ItemEntry* entry;
    int i;

    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (!strcmp(entry->name, name)) {
#if VERSION >= VERSION_JP
            itemCheckBadge(entry);
#endif
            switch (entry->itemId) {
                case ITEM_DIAMOND_STAR:
                case ITEM_EMERALD_STAR:
                case ITEM_GOLD_STAR:
                case ITEM_RUBY_STAR:
                case ITEM_SAPPHIRE_STAR:
                case ITEM_GARNET_STAR:
                case ITEM_CRYSTAL_STAR:
                    effSoftDelete(entry->effect);
                    break;
                default:
                    iconDelete(entry->name);
                    break;
            }
            entry->flags &= ~ITEM_FLAG_ACTIVE;
        }
    }
}

/** Returns the field item with this name, or NULL. */
ItemEntry* itemNameToPtr(char* name) {
    ItemWork* wp = itemGetWork();
    ItemEntry* entry;
    int i;

    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (!strcmp(entry->name, name)) {
            return entry;
        }
    }
    return NULL;
}

/**
 * Returns an eligible item within range of (x, y, z), or NULL;
 * for an item above y, checks an upward collision ray from (x, y, z), not a ray toward the item;
 * evt_mobj uses it to see whether an item sits on a block.
 */
ItemEntry* itemHitCheck(f32 x, f32 y, f32 z, f32 range) {
    ItemWork* wp = itemGetWork();
    ItemEntry* entry;
    int i;
    MarioWork* mario;
    ItemEntry* found;
    f32 dx, dy, dz, xzdist, dist;
    f32 hitX, hitY, hitZ, hitDist, hitNX, hitNY, hitNZ;
    HitEntry* hit;

    mario = marioGetPtr();
    found = NULL;
    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (!(entry->flags & ITEM_FLAG_ACTIVE)) {
            continue;
        }

        if (entry->flags & ITEM_FLAG_NO_TOUCH_GET) {
            continue;
        }
        if (entry->flags2 & ITEM_FLAG2_CARRIED) {
            continue;
        }
        if (entry->mode == ITEM_MODE_GET) {
            continue;
        }
        dx = x - entry->position.x;
        dy = y - entry->position.y;
        dz = z - entry->position.z;
        xzdist = sqrtf(dx * dx + dz * dz);
        dist = sqrtf(dx * dx + dy * dy + dz * dz);
        if (xzdist < range && marioItemGetOk() && marioGetJabaraState() != JABARA_STATE_RISING &&
            marioGetJabaraState() != JABARA_STATE_BUMP_CEILING && marioGetJabaraState() != JABARA_STATE_SWING) {
            if (__fabs((y + mario->wPlayerCollisionBox.y * 0.5f) - entry->position.y) < mario->wPlayerCollisionBox.y * 0.5f + 10.0f) {
                hit = NULL;
                if (y < entry->position.y) {
                    hitDist = 10.0f + (entry->position.y - y);
                    if (hitDist > 0.0f) {
                        hit = hitCheckAttr(HIT_ATTR_MOBJ, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, 1.0f, 0.0f);
                    }
                }
                if (!hit) {
                    found = entry;
                    range = dist;
                }
            }
        }
    }
    return found;
}

/**
 * Returns the nearest collectable item within range of (x, y, z), skipping items Koops's shell must ignore;
 * Ms. Mowz and evt_mobj use it.
 */
ItemEntry* itemNearDistCheck(f32 x, f32 y, f32 z, f32 range) {
    MarioWork* mario = marioGetPtr();
    ItemWork* wp = itemGetWork();
    ItemEntry* entry;
    ItemEntry* found;
    int i;
    Vec itemPos, diff, pos;
    f32 dist;

    entry = wp->entries;
    found = NULL;
    i = 0;
    pos = (Vec){ x, y, z };
    for (; i < wp->count; i++, entry++) {
#if VERSION <= VERSION_DEMO_JP
        if (!(entry->flags & ITEM_FLAG_ACTIVE)) {
            continue;
        }
        if (entry->flags & ITEM_FLAG_NO_TOUCH_GET)
        continue;
        if (entry->flags2 & ITEM_FLAG2_CARRIED) {
            continue;
        }
        if (entry->mode == ITEM_MODE_GET) {
            continue;
        }
#else
        if (!(entry->flags & ITEM_FLAG_ACTIVE)) {
            continue;
        }
        if (entry->flags2 & ITEM_FLAG2_CARRIED) {
            continue;
        }
        if (entry->flags2 & ITEM_FLAG2_SUNK) {
            continue;
        }
        if (entry->mode == ITEM_MODE_GET) {
            continue;
        }
        if (entry->flags2 & ITEM_FLAG2_NO_PARTNER) {
            continue;
        }
#endif
        itemPos = (Vec){ entry->position.x, entry->position.y, entry->position.z };
        PSVECSubtract(&itemPos, &pos, &diff);
        dist = PSVECMag(&diff);
        if (dist < range) {
            found = entry;
            range = dist;
        }
    }
    return found;
}

/**
 * Puts the item into the pouch; FALSE if the pouch is full.
 * On success sets its collect flag (GSWF or LSWF) and starts its pickup script.
 */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static BOOL itemGetItem(ItemEntry* entry) {
    EventEntry* evt;

    if (!pouchGetItem(entry->itemId)) {
        return FALSE;
    }
    if (entry->flags & ITEM_FLAG_COLLECT_GSWF) {
        swSet(entry->collectFlag);
    } else if (entry->flags & ITEM_FLAG_COLLECT_LSWF) {
        _swSet(entry->collectFlag);
    }
    if (entry->evtCode != NULL) {
        evt = evtEntry(entry->evtCode, 0, EVENT_START_IMMEDIATE);
        evt->lwData[0] = FLOAT(entry->position.x);
        evt->lwData[1] = FLOAT(entry->position.y);
        evt->lwData[2] = FLOAT(entry->position.z);
    }
    entry->flags2 |= ITEM_FLAG2_IN_POUCH;
    return TRUE;
}

/**
 * Draws the "You got X!" box during an item get, with "badge" wording for badges;
 * US, PAL and the September demos format the message per language and size the box by its line count; JP and the early demos use sprintf and fixed height.
 */
static void winNameDisp(WinMgrEntry* win) {
    ItemEntry* entry = win->param;
    const char* name;
    u16 width;
#if VERSION >= VERSION_US
    u16 lines;
#endif
    char buffer[1024];

    name = msgSearch(itemDataTable[entry->itemId].name);
#if VERSION >= VERSION_US
    if (gp->language == LANGUAGE_JP) {
        if (entry->itemId >= BADGE_MIN && entry->itemId < BADGE_MAX) {
            sprintf(buffer, msgSearch("msg_window_badge_get"), name);
        } else {
            sprintf(buffer, msgSearch("msg_window_item_get"), name);
        }
    } else {
        if (entry->itemId >= BADGE_MIN && entry->itemId < BADGE_MAX) {
            strcpy(buffer, msgSearch("msg_window_badge_get"));
        } else {
            strcpy(buffer, msgSearch("msg_window_item_get"));
        }
        evtMsgFmtStr(buffer, name);
    }
    width = FontGetMessageWidthLine(buffer, &lines);
#else
    if (entry->itemId >= BADGE_MIN && entry->itemId < BADGE_MAX) {
        sprintf(buffer, msgSearch("msg_window_badge_get"), name);
    } else {
        sprintf(buffer, msgSearch("msg_window_item_get"), name);
    }
    width = FontGetMessageWidth(buffer);
#endif
    if (width + 20 > win->width) {
        win->width = width + 20;
        win->x = -win->width / 2;
    }
#if VERSION >= VERSION_US
    win->height = win->description->height + lines * 24;
#endif
    if (!winMgrAction(entry->winId[0])) {
        FontDrawStart();
        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
        FontDrawMessage(-width / 2, win->y - 8, buffer);
    }
}

/** Draws the box that names an item Mario let go of (msg_window_item_release) when his pouch was full. */
static void winNameDisp2(WinMgrEntry* win) {
    ItemEntry* entry = win->param;
    const char* name;
    u16 width;
#if VERSION >= VERSION_US
    u16 lines;
#endif
    char buffer[1024];

    name = msgSearch(itemDataTable[entry->itemId].name);
#if VERSION >= VERSION_US
    if (gp->language == LANGUAGE_JP) {
        sprintf(buffer, msgSearch("msg_window_item_release"), name);
    } else {
        strcpy(buffer, msgSearch("msg_window_item_release"));
        evtMsgFmtStr(buffer, name);
    }
    width = FontGetMessageWidthLine(buffer, &lines);
#else
    sprintf(buffer, msgSearch("msg_window_item_release"), name);
    width = FontGetMessageWidth(buffer);
#endif
    if (width + 20 > win->width) {
        win->width = width + 20;
        win->x = -win->width / 2;
    }
#if VERSION >= VERSION_US
    win->height = win->description->height + lines * 24;
#endif
    if (!winMgrAction(entry->winId[0])) {
        FontDrawStart();
        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
        FontDrawMessage(-width / 2, win->y - 8, buffer);
    }
}

/** Per-frame update of the item description box shown under the get box. */
static void winHelpMain(WinMgrEntry* win) {
    winMgrHelpMain(win);
}

/** Draws the got item's description in the help box, once the help box has finished animating. */
static void winHelpDisp(WinMgrEntry* win) {
    ItemEntry* entry = win->param;

    if (!winMgrAction(entry->winId[1])) {
        win->msg = msgSearch(itemDataTable[entry->itemId].description);
        winMgrHelpDraw(win);
    }
}

/** Draws the full-pouch warning: JP and later choose item or badge wording; the early demos always use the item-full message. */
static void winFullDisp(WinMgrEntry* win) {
#if VERSION <= VERSION_DEMO_JP
    ItemEntry* entry;
#else
    ItemEntry* entry = win->param;
    BOOL isBadge = FALSE;
    u16 width;
#endif
    const char* msg;
#if VERSION <= VERSION_DEMO_JP
    u16 width;
#endif

#if VERSION <= VERSION_DEMO_JP
    entry = win->param;
    msg = msgSearch("msg_window_item_full");
    width = (u16)FontGetMessageWidth(msg);
#else
    if (entry->itemId >= BADGE_MIN && entry->itemId < BADGE_MAX) {
        isBadge = TRUE;
    }
    msg = isBadge ? msgSearch("msg_window_badge_full") : msgSearch("msg_window_item_full");
    width = FontGetMessageWidth(msg);
#endif
    if (width + 20 > win->width) {
        win->width = width + 20;
        win->x = -win->width / 2;
    }
    if (!winMgrAction(entry->winId[2])) {
        FontDrawStart();
        FontDrawEdge();
        FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        FontDrawMessage(-width / 2, win->y - 10, msg);
        FontDrawEdgeOff();
    }
}

/**
 * The item-get sequence. The early demos have no sub-states 2 (control off) and 8 (motion abort),
 * so later 3-7 and 9 are their 2-7; no badge flag bookkeeping, no item renaming after a swap,
 * and the icon flags are 0x40/0x100 where later versions have 0x100/0x200.
 */
static void itemseq_GetItem(ItemEntry* entry) {
    ItemWork* wp = itemGetWork();
    MarioWork* mario;
    s32 type, bgm;
    s32 select;

    mario = marioGetPtr();
    switch (entry->subState) {
        case 0:
            entry->flags2 &= ~ITEM_FLAG2_RELEASED;
#if VERSION >= VERSION_JP
            if (entry->flags & ITEM_FLAG_LOST_BADGE_TO_SHOP) {
                entry->flags2 |= ITEM_FLAG2_SAVED_LOST_BADGE;
            } else {
                entry->flags2 &= ~ITEM_FLAG2_SAVED_LOST_BADGE;
            }
            entry->flags &= ~ITEM_FLAG_LOST_BADGE_TO_SHOP;
#endif
            if (entry->flags & ITEM_FLAG_QUICK_GET) {
                if (itemGetItem(entry)) {
                    entry->timer = 0;
                    itemPlaySound(entry);
                    itemDelete(entry->name);
                } else {
                    itemModeChange(entry, ITEM_MODE_FALL_NO_PICKUP);
                }
                break;
            }
            entry->subState = 1;
            entry->timer = 0;
            camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
            marioKeyOff();
            marioStSystemLevel(2);
            psndSetFlag(PSND_FLAG_MUTE_SFX);
#if VERSION >= VERSION_JP
            mario->floorMove.x = 0.0f;
            mario->floorMove.z = 0.0f;
            mario->baseSpeed = 0.0f;
            mario->moveDist = 0.0f;
#endif
            if (entry->itemId == ITEM_SUPER_BOOTS || entry->itemId == ITEM_ULTRA_BOOTS || entry->itemId == ITEM_SUPER_HAMMER || entry->itemId == ITEM_ULTRA_HAMMER) {
                entry->flags2 |= ITEM_FLAG2_UPGRADE_GET;
            }
        case 1:
            if (!(entry->flags & ITEM_FLAG_FORCE_GET) || entry->flags & ITEM_FLAG_NOKO_GET) {
                if (!marioChkItemGetMotion()) {
#if VERSION >= VERSION_JP
                    if (mario->currMotionId == MARIO_MOTION_JUMP_SW || mario->currMotionId == MARIO_MOTION_JUMP_STAND ||
                        mario->currMotionId == MARIO_MOTION_JUMP_2 || mario->currMotionId == MARIO_MOTION_JUMP_3 ||
                        mario->currMotionId == MARIO_MOTION_GET_ITEM || mario->currMotionId == MARIO_MOTION_TALK) {
                        entry->subState = 8;
                    }
#endif
                    break;
                }
            }
            entry->subState = 2;
            marioChgGetItemMotion();
            {
                f32 y = 50.0f + mario->position.y, z = mario->position.z;
                entry->position.x = mario->position.x;
                entry->position.y = y;
                entry->position.z = z;
            }
            type = 7;
            bgm = -1;
            if (entry->itemId == ITEM_SHINE_SPRITE) {
                type = 4;
                bgm = -1;
            } else if (entry->itemId == ITEM_STAR_PIECE) {
                type = 8;
                bgm = 0;
            } else if (entry->itemId < 0xC) {
                type = 3;
                bgm = 3;
            } else if (entry->itemId < ITEM_DIAMOND_STAR) {
                type = 6;
                bgm = 1;
            } else if (entry->itemId < KEY_ITEM_MAX) {
                type = 0;
                bgm = -1;
            } else if (entry->itemId < ITEM_MAX) {
                type = 7;
                bgm = -1;
            } else if (entry->itemId < BADGE_MAX) {
                type = 5;
                bgm = 2;
            }
            wp->effect = effItemGetEntry(type, mario->position.x, 62.0f + mario->position.y, mario->position.z);
#if VERSION <= VERSION_DEMO_JP
            if (type == 0 || type == 3) {
                if (entry->flags2 & ITEM_FLAG2_UPGRADE_GET) {
                    ((ItemGetWork*)wp->effect->userdata)->camera = 5;
                    marioSetCamId(CAMERA_3D_EFFECTB);
                    iconFlagOn(entry->name, 0x100);
                } else {
                    ((ItemGetWork*)wp->effect->userdata)->camera = 5;
                    marioSetCamId(CAMERA_3D_EFFECTA);
                    iconFlagOn(entry->name, 0x40);
#else
            if (entry->flags2 & ITEM_FLAG2_UPGRADE_GET) {
                ((ItemGetWork*)wp->effect->userdata)->camera = 5;
                iconFlagOn(entry->name, ICON_FLAG_CAM_EFFECT_B);
                marioSetCamId(CAMERA_3D_EFFECTB);
            } else {
                ((ItemGetWork*)wp->effect->userdata)->camera = 5;
                iconFlagOn(entry->name, ICON_FLAG_CAM_EFFECT_A);
                if (type == 0 || type == 3) {
                    marioSetCamId(CAMERA_3D_EFFECTA);
#endif
                }
            }
            if (!(entry->flags & ITEM_FLAG_NO_GET_BGM)) {
                if (psndBGMChkSilent(0)) {
                    entry->flags |= ITEM_FLAG_BGM_SILENT;
                }
                switch (bgm) {
                    case 0:
                        if (!(entry->flags & ITEM_FLAG_BGM_SILENT)) {
                            psndBGMOff(PSND_SILENT | PSND_SILENT_LEVEL(3));
                        }
                        psndBGMOn(PSND_RESTART | PSND_FADEOUT | PSND_SLOT(1), "BGM_FF_GET_STARPIECE1");
                        break;
                    case 1:
                        if (!(entry->flags & ITEM_FLAG_BGM_SILENT)) {
                            psndBGMOff(PSND_SILENT | PSND_SILENT_LEVEL(3));
                        }
                        psndBGMOn(PSND_RESTART | PSND_FADEOUT | PSND_SLOT(1), "BGM_FF_GET_KEYITEM1");
                        break;
                    case 2:
                        if (!(entry->flags & ITEM_FLAG_BGM_SILENT)) {
                            psndBGMOff(PSND_SILENT | PSND_SILENT_LEVEL(3));
                        }
                        psndBGMOn(PSND_RESTART | PSND_FADEOUT | PSND_SLOT(1), "BGM_FF_GET_BADGE1");
                        break;
                    case 3:
                        if (!(entry->flags & ITEM_FLAG_BGM_SILENT)) {
                            psndBGMOff(PSND_SILENT | PSND_SILENT_LEVEL(3));
                        }
                        psndBGMOn(PSND_RESTART | PSND_FADEOUT | PSND_SLOT(1), "BGM_FF_GET_IMPORTANT_ITEM1");
                        break;
                    default:
                        if (!(entry->flags & ITEM_FLAG_BGM_SILENT)) {
                            psndBGMOff(PSND_SILENT | PSND_SILENT_LEVEL(3));
                        }
                        psndBGMOn(PSND_RESTART | PSND_FADEOUT | PSND_SLOT(1), "BGM_FF_GET_ITEM1");
                        break;
                }
            }
            if (type == 8) {
                evtSetValue(NULL, GSW_STAR_PIECES_FOUND_COUNT, evtGetValue(NULL, GSW_STAR_PIECES_FOUND_COUNT) + 1);
            }
            entry->flags &= ~ITEM_FLAG_GET_BGM_ENDED;
            itemPlaySound(entry);
            entry->winId[0] = winMgrEntry(&itemdrv_window_desc[0]);
            winMgrOpen(entry->winId[0]);
            winMgrSetParam(entry->winId[0], entry);
            if (!(entry->flags2 & ITEM_FLAG2_UPGRADE_GET)) {
                entry->winId[1] = winMgrEntry(&itemdrv_window_desc[1]);
                winMgrOpen(entry->winId[1]);
                winMgrSetParam(entry->winId[1], entry);
            }
            if (!itemGetItem(entry)) {
                entry->winId[2] = winMgrEntry(&itemdrv_window_desc[2]);
                winMgrOpen(entry->winId[2]);
                winMgrSetParam(entry->winId[2], entry);
            }
#if VERSION <= VERSION_DEMO_JP
            if (entry->itemId == ITEM_WEDDING_RING) {
                iconChange(entry->name, ICON_WEDDING_RING_LARGE);
            }
#else
            if (entry->itemId == ITEM_WEDDING_RING) {
                iconChange(entry->name, ICON_WEDDING_RING_LARGE);
            }
            if (!entry->effect) {
                iconNameToPtr(entry->name)->uflags |= ICON_FLAG_ITEM_NO_MAP_BLEND;
            }
#endif
            break;
        case 2:
#if VERSION >= VERSION_JP
            if (!(entry->flags2 & ITEM_FLAG2_UPGRADE_GET)) {
                marioCtrlOff();
                entry->flags2 |= ITEM_FLAG2_CTRL_OFF;
            }
            entry->subState = 3;
        case 3:
#endif
            if (!(entry->flags & ITEM_FLAG_NO_GET_BGM) && !(entry->flags & ITEM_FLAG_GET_BGM_ENDED)) {
                if (psndBGMChk(1)) {
                    entry->flags |= ITEM_FLAG_GET_BGM_ENDED;
                    psndBGMOff(PSND_FADEOUT | PSND_SLOT(1));
                    if (!(entry->flags & ITEM_FLAG_BGM_SILENT)) {
                        psndBGMOn(PSND_CONTINUE | PSND_UNSILENT, NULL);
                    }
                }
            }
            if (winMgrAction(entry->winId[0])) {
                break;
            }
            if (!(keyGetButtonTrg(0) & (PAD_BUTTON_A | PAD_BUTTON_B))) {
                break;
            }
            winMgrCloseAutoDelete(entry->winId[0]);
            if (!(entry->flags2 & ITEM_FLAG2_UPGRADE_GET)) {
                winMgrCloseAutoDelete(entry->winId[1]);
            }
            if (!(entry->flags & ITEM_FLAG_NO_GET_BGM) && !(entry->flags & ITEM_FLAG_GET_BGM_ENDED)) {
                psndBGMOff(PSND_FADEOUT | PSND_SLOT(1));
                if (!(entry->flags & ITEM_FLAG_BGM_SILENT)) {
                    psndBGMOn(PSND_CONTINUE | PSND_UNSILENT, NULL);
                }
            }
            if (entry->flags2 & ITEM_FLAG2_UPGRADE_GET) {
#if VERSION <= VERSION_DEMO_JP
                entry->subState = 5;
#else
                entry->subState = 6;
#endif
                break;
            }
            if (entry->itemId >= ITEM_MIN && entry->itemId < ITEM_MAX && entry->itemId != ITEM_STAR_PIECE && !swGet(GSWF_ID(GSWF_ITEM_TUTORIAL_SEEN)) && !(gp->flags & GW_FLAG_E3)) {
                swSet(GSWF_ID(GSWF_ITEM_TUTORIAL_SEEN));
#if VERSION <= VERSION_DEMO_JP
                entry->subState = 3;
#else
                entry->subState = 4;
#endif
                wp->msgWindowId = msgWindow_Entry(msgSearch("item_first_hit"), 0, 0);
                break;
            }
            effSoftDelete(wp->effect);
            marioSetCamId(CAMERA_3D);
#if VERSION <= VERSION_DEMO_JP
            iconFlagOff(entry->name, 0x40);
            if (entry->itemId == ITEM_WEDDING_RING) {
                iconChange(entry->name, ICON_WEDDING_RING_SMALL);
#else
            iconFlagOff(entry->name, ICON_FLAG_CAM_EFFECT_A);
            if (entry->itemId == ITEM_WEDDING_RING) {
                iconChange(entry->name, ICON_WEDDING_RING_SMALL);
            }
            if (!entry->effect) {
                iconNameToPtr(entry->name)->uflags &= ~ICON_FLAG_ITEM_NO_MAP_BLEND;
#endif
            }
            if (entry->flags2 & ITEM_FLAG2_IN_POUCH) {
                itemGetEnd(entry);
                itemDelete(entry->name);
                break;
            }
#if VERSION <= VERSION_DEMO_JP
            entry->subState = 4;
#else
            entry->subState = 5;
#endif
            winMgrCloseAutoDelete(entry->winId[2]);
            marioChgStayMotion();
            iconFlagOn(entry->name, ICON_FLAG_HIDDEN);
            {
                s32 id = entry->itemId;
                if (id >= BADGE_MIN && id < BADGE_MAX) {
                    wp->select = winMgrSelectEntry(SELECT_TYPE_BADGE, id, 1);
                } else {
                    wp->select = winMgrSelectEntry(SELECT_TYPE_ITEM, id, 1);
                }
            }
            break;
#if VERSION <= VERSION_DEMO_JP
        case 3:
#else
        case 4:
#endif
            if (windowCheckID(wp->msgWindowId)) {
                break;
            }
            effSoftDelete(wp->effect);
            marioSetCamId(CAMERA_3D);
            iconFlagOff(entry->name, ICON_FLAG_CAM_EFFECT_A);
            itemGetEnd(entry);
            itemDelete(entry->name);
            break;
#if VERSION <= VERSION_DEMO_JP
        case 4:
#else
        case 5:
#endif
            select = winMgrSelect(wp->select);
            if (!select) {
                break;
            }
            winMgrSelectDelete(wp->select);
#if VERSION <= VERSION_DEMO_JP
            if (select >= 0) {
#else
            if (select < 0) {
                if (entry->flags2 & ITEM_FLAG2_SAVED_LOST_BADGE) {
                    entry->flags |= ITEM_FLAG_LOST_BADGE_TO_SHOP;
                }
            } else {
#endif
                pouchRemoveItem(select);
                itemGetItem(entry);
                entry->flags2 &= ~ITEM_FLAG2_IN_POUCH;
                entry->itemId = select;
                entry->evtCode = NULL;
#if VERSION <= VERSION_DEMO_JP
                entry->flags &= ~(ITEM_FLAG_COLLECT_GSWF | ITEM_FLAG_COLLECT_LSWF);
                iconChange(entry->name, itemDataTable[entry->itemId].icon_id);
            }
#else
                entry->collectFlag = 0;
                entry->flags &= ~(ITEM_FLAG_COLLECT_GSWF | ITEM_FLAG_COLLECT_LSWF);
                iconChange(entry->name, itemDataTable[entry->itemId].icon_id);
                wp->currId++;
                sprintf(iconNameToPtr(entry->name)->name, "_i%05x", wp->currId);
                sprintf(entry->name, "_i%05x", wp->currId);
                entry->flags |= ITEM_FLAG_LOST_BADGE_TO_SHOP;
            }
            entry->flags &= ~ITEM_FLAG_FORCE_GET;
#endif
            entry->speed = 85.0f;
            entry->velocityY = 120.0f;
            entry->bounceFactor = 0.7f;
            entry->gravityScale = 1.0f;
            entry->winId[0] = winMgrEntry(&itemdrv_window_desc[3]);
            winMgrOpen(entry->winId[0]);
            winMgrSetParam(entry->winId[0], entry);
            marioChgGetItemMotion();
            iconFlagOff(entry->name, ICON_FLAG_HIDDEN);
            if (mario->flags & MARIO_FLAG_BG_MODE) {
                entry->flags2 |= ITEM_FLAG2_NO_Z_MOVE;
            }
#if VERSION <= VERSION_DEMO_JP
            entry->subState = 7;
            break;
        case 5:
#else
            entry->subState = 9;
            break;
        case 6:
#endif
            wp->lectureEvt = evtEntryType(all_juyoitem_lecture, 0, 0, 0);
            wp->lectureEvt->lwData[8] = (EvtWord)entry;
            wp->lectureEvt->lwData[9] = (EvtWord)wp->effect;
            psndClearFlag(PSND_FLAG_MUTE_SFX);
#if VERSION <= VERSION_DEMO_JP
            entry->subState = 6;
            break;
        case 6:
#else
            entry->subState = 7;
            break;
        case 7:
#endif
            if (evtCheckID(wp->lectureEvt->eventId)) {
                break;
            }
            itemGetEnd(entry);
            itemDelete(entry->name);
            break;
#if VERSION <= VERSION_DEMO_JP
        case 7:
#else
        case 9:
#endif
            if (!(keyGetButtonTrg(0) & PAD_BUTTON_A)) {
                break;
            }
            winMgrCloseAutoDelete(entry->winId[0]);
            entry->flags2 |= ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND;
            itemModeChange(entry, ITEM_MODE_SCATTER_NO_PICKUP);
            entry->flags &= ~ITEM_FLAG_CUSTOM_BOUND;
            if (entry->itemId >= KEY_ITEM_MIN && entry->itemId < KEY_ITEM_MAX) {
                entry->timer = 0;
            } else if (entry->itemId >= BADGE_MIN && entry->itemId < BADGE_MAX) {
                entry->timer = 0;
            } else {
                entry->timer = 20000;
            }
            entry->flags2 |= ITEM_FLAG2_RELEASED;
            entry->flags2 &= ~ITEM_FLAG2_CARRIED;
            entry->flags &= ~ITEM_FLAG_NO_TOUCH_GET;
            itemGetEnd(entry);
            break;
#if VERSION >= VERSION_JP
        case 8:
            if (mario->flags & MARIO_FLAG_BG_MODE) {
                entry->flags2 |= ITEM_FLAG2_NO_Z_MOVE;
            }
            entry->timer = 0;
            if (entry->flags2 & ITEM_FLAG2_CARRIED) {
                itemModeChange(entry, ITEM_MODE_GROUND);
                entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_RISING;
                entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND;
                entry->flags2 &= ~ITEM_FLAG2_TIMER_WAIT_LAND;
                entry->flags2 &= ~ITEM_FLAG2_CARRIED;
                entry->flags &= ~ITEM_FLAG_FORCE_GET;
            } else {
                entry->speed = 85.0f;
                entry->velocityY = 120.0f;
                entry->bounceFactor = 0.7f;
                entry->gravityScale = 1.0f;
                entry->flags2 |= ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND;
                itemModeChange(entry, ITEM_MODE_SCATTER_NO_PICKUP);
                entry->flags &= ~ITEM_FLAG_CUSTOM_BOUND;
                entry->flags2 &= ~ITEM_FLAG2_CARRIED;
                entry->flags &= ~ITEM_FLAG_NO_TOUCH_GET;
                entry->flags &= ~ITEM_FLAG_FORCE_GET;
            }
            marioStSystemLevel(0);
            psndClearFlag(PSND_FLAG_MUTE_SFX);
            if (entry->flags2 & ITEM_FLAG2_CTRL_OFF) {
                entry->flags2 &= ~ITEM_FLAG2_CTRL_OFF;
                marioCtrlOn();
            }
            marioKeyOn();
            break;
#endif
    }
}

// Unused 4-byte object, read by nothing; the original had it too (its storage is in the retail .sbss). Name invented.
static s32 _move_dir_1090;

/** sin and cos of an angle in degrees. */
static inline f32 sind(f32 deg) { return sin(6.2831855f * deg / 360.0f); }
static inline f32 cosd(f32 deg) { return cos(6.2831855f * deg / 360.0f); }

/**
 * Moves a bouncing or falling item: gravity, bouncing off floors (with fewer, lower bounces each time),
 * walls and ceilings, landing on map objects, water and lava, and despawning where needed;
 * becomes resting when it stops.
 */
static void itemseq_Bound(ItemEntry* entry) {
    static s32 _move_dir;
    MarioWork* mario;
    CameraEntry* camera;
    HitEntry* hit;
    MapObjectEntry* mobj;
    BOOL isFloor, bounce;
    s64 delta;
    f32 t2sq, dt, dy, t2, dx, t1;
    f32 dz, v, peak, height;
    f32 newX, newZ;
    s32 newDir;
    Vec pos;
    s32 itemId;

    mario = marioGetPtr();
    camera = camGetPtr(CAMERA_3D);
    switch (entry->subState) {
        case 0:
            entry->subState = 1;
            entry->airTime = 0;
            entry->baseTime = gp->renderTime;
            entry->curTime = gp->renderTime;
            entry->bounceCount = 0;
            if (entry->flags2 & ITEM_FLAG2_SKIP_BOUND_INIT) {
                break;
            }
            switch (entry->mode) {
                case ITEM_MODE_SCATTER_NO_PICKUP:
                    entry->flags2 |= ITEM_FLAG2_NO_PICKUP_RISING;
                case ITEM_MODE_SCATTER:
                    if (!(entry->flags & ITEM_FLAG_CUSTOM_BOUND)) {
                        entry->bounceFactor = 1.0f;
                        entry->gravityScale = 1.0f;
                        entry->maxBounces = 0;
                        entry->velocityY = 500.0f;
                    }
                    if (!(entry->flags2 & ITEM_FLAG2_SPREAD)) {
                        switch (entry->itemId) {
                            case ITEM_COIN:
                            case ITEM_HEART:
                            case ITEM_FLOWER:
                                entry->speed = irand(30) + 45;
                                if (irand(100) < 50) {
                                    entry->direction = 90.0f + camera->viewYaw + irand(60) - 30.0f;
                                } else {
                                    entry->direction = camera->viewYaw - 90.0f + irand(60) - 30.0f;
                                }
                                break;
                            default:
                                entry->speed = 100.0f;
                                if (irand(100) < 50) {
                                    entry->direction = 90.0f + camera->viewYaw + irand(30) - 15.0f;
                                } else {
                                    entry->direction = camera->viewYaw - 90.0f + irand(30) - 15.0f;
                                }
                                break;
                        }
#if VERSION >= VERSION_US
                        if (!strcmp(gp->currentMapName, "aji_03")) {
                            entry->direction = irand(90);
                        }
#endif
                    } else {
                        entry->direction = _move_dir;
                        _move_dir += 132;
                        if (_move_dir >= 360) {
                            _move_dir -= 360;
                        }
                        entry->speed = 0.67f * (irand(30) + 65);
                    }
                    break;
                case ITEM_MODE_FALL_NO_PICKUP:
                    entry->flags2 |= ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND;
                case ITEM_MODE_FALL:
                    if (!(entry->flags & ITEM_FLAG_CUSTOM_BOUND)) {
                        entry->bounceFactor = 1.0f;
                        entry->gravityScale = 1.0f;
                        entry->maxBounces = 0;
                    }
                    entry->speed = 0.0f;
                    entry->direction = 0;
                    entry->velocityY = 0.0f;
                    break;
                case ITEM_MODE_POP:
                    entry->bounceFactor = 1.0f;
                    entry->gravityScale = 1.0f;
                    entry->maxBounces = 0;
                    entry->velocityY = 500.0f;
                    entry->speed = 0.0f;
                    entry->direction = 0;
                    entry->flags2 |= ITEM_FLAG2_POP_VANISH;
                    entry->flags2 |= ITEM_FLAG2_NO_PICKUP_RISING;
                    break;
            }
            break;
        case 1:
            entry->curTime = gp->renderTime;
            delta = entry->curTime - entry->baseTime;
            dt = OSTicksToMicroseconds(delta) / 1000000.0f;
            t1 = OSTicksToMicroseconds(entry->airTime) / 1000000.0f;
            t2 = OSTicksToMicroseconds(entry->airTime + delta) / 1000000.0f;
            dx = dt * (entry->speed * sind(entry->direction));
            dz = dt * (entry->speed * -cosd(entry->direction));
            if (entry->flags2 & ITEM_FLAG2_NO_Z_MOVE) {
                dz = 0.0f;
            }
            t2sq = t2 * t2;
            dy = entry->velocityY * dt + (-980.0f * entry->gravityScale) * (t2sq - t1 * t1);
            if (entry->flags2 & ITEM_FLAG2_NO_PICKUP_RISING) {
                if (entry->bounceCount != 0 || dy < 0.0f) {
                    entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_RISING;
                }
            }
            if (entry->flags2 & ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND && entry->bounceCount != 0) {
                entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND;
            }
            if (entry->flags2 & ITEM_FLAG2_POP_VANISH && dy < 0.0f) {
                effStardustEntry(1, 3, 30, entry->position.x, entry->position.y, entry->position.z, 16.0f, 16.0f);
                itemDelete(entry->name);
                return;
            }
            newDir = entry->direction;
            hit = itemHitCheckSide(entry, &newX, dx, &newZ, dz, &newDir);
            entry->position.x = newX;
            entry->position.z = newZ;
            entry->direction = newDir;
            if (hit) {
                entry->speed *= 0.25f;
            }
            if (!(entry->flags2 & ITEM_FLAG2_POP_VANISH) && !(entry->flags & ITEM_FLAG_NO_CEILING_HIT)) {
                {
                    // FAKE: itemHitCheckUp written out here (inlining it leaves the Y block below in the wrong stack order);
                    // h is the inline's result copy and keeps the same register pressure. hit* declared in reverse for ascending slots.
                    HitEntry* h;
                    f32 hitNZ, hitNY, hitNX, hitDist, hitZ, hitY, hitX;
                    f32 x, y, z;

                    if (dy <= 0.0f) {
                        hit = NULL;
                    } else {
                        x = entry->position.x;
                        y = 20.0f + entry->position.y;
                        z = entry->position.z;
                        hitDist = 20.0 + fabs(dy);
                        if ((h = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, 1.0f, 0.0f)) != NULL) {
                            entry->position.y = hitY - 20.0f - 4.0f;
                            hit = h;
                        } else {
                            x = entry->position.x;
                            y = entry->position.y;
                            z = entry->position.z;
                            hitDist = 20.0 + fabs(dy);
                            if ((h = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, 1.0f, 0.0f)) != NULL) {
                                entry->position.y = hitY - 20.0f;
                                hit = h;
                            } else {
                                hit = NULL;
                            }
                        }
                    }
                }
                if (hit) {
                    entry->velocityY = -entry->velocityY / 4.0f; // FAKE: /4.0f (exact) gives the target register order
                    dy = 0.0f;
                    entry->airTime = 0;
                    entry->baseTime = entry->curTime;
                }
            }
            if (entry->flags2 & ITEM_FLAG2_SUNK && entry->position.y <= -1000.0f) {
                itemModeChange(entry, ITEM_MODE_STILL);
                entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_RISING;
                entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND;
                entry->flags2 &= ~ITEM_FLAG2_TIMER_WAIT_LAND;
                if (entry->itemId >= KEY_ITEM_MIN && entry->itemId < KEY_ITEM_MAX) {
                    entry->timer = 0;
                } else if (entry->itemId >= BADGE_MIN && entry->itemId < BADGE_MAX) {
                    entry->timer = 0;
                } else {
                    entry->timer = 20000;
                }
                entry->flags2 |= ITEM_FLAG2_RELEASED;
                entry->flags2 &= ~ITEM_FLAG2_CARRIED;
#if VERSION >= VERSION_JP
                entry->flags |= ITEM_FLAG_LOST_BADGE_TO_SHOP;
#endif
                break;
            }
            if (dy >= 0.0f) {
                hit = NULL;
                entry->position.y += dy;
            }

            // FAKE: no-op u32 cast so the earlier masked flags2 value is not reused (target reloads flags2 here)
            else if (*(u32*)&entry->flags2 & ITEM_FLAG2_SUNK) {
                hit = NULL;
                entry->position.y += dy;
            } else {
                // FAKE: itemHitCheckY written out here; the result goes through h so the caller's hit is set in each arm
                // (coalesces into r26 like the target). hit* declared in reverse for ascending stack slots.
                f32 hitNZ, hitNY, hitNX, hitDist, hitZ, hitY, hitX;
                HitEntry* h;
                f32 x = entry->position.x, y = 20.0f + entry->position.y, z = entry->position.z;
                hitDist = 20.0 + fabs(dy);
                h = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, -1.0f, 0.0f);
                if (h) {
                    entry->position.y = hitY;
                    hit = h;
                } else {
                    hit = NULL;
                    entry->position.y += dy;
                }
            }
            if (hit) {
                if (hitGetAttr(hit) & (HIT_ATTR_WATER_HAZARD | HIT_ATTR_DAMAGE_LIQUID)) {
                    entry->flags2 |= ITEM_FLAG2_SUNK;
                    {
                        Vec sndPos = (Vec){0.0f, 0.0f, 0.0f};
                        sndPos.x = entry->position.x;
                        sndPos.y = entry->position.y;
                        sndPos.z = entry->position.z;
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_FALL_WATER1), &sndPos);
                    }
                    effSplashEntry(0, entry->position.x, entry->position.y, entry->position.z, 0.7f);
                    entry->flags |= ITEM_FLAG_NO_TOUCH_GET;
                } else {
                    entry->bounceCount++;
                    if (!(hitGetAttr(hit) & HIT_ATTR_MOBJ) && entry->maxBounces != 0 && entry->bounceCount >= entry->maxBounces) {
                        itemModeChange(entry, ITEM_MODE_GROUND);
                        entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_RISING;
                        entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND;
                        entry->flags2 &= ~ITEM_FLAG2_TIMER_WAIT_LAND;
                        break;
                    }
                pos = (Vec){entry->position.x, entry->position.y, entry->position.z};
                switch (itemId = entry->itemId) {
                    case ITEM_COIN:
                        psndSFXOn_3D("SFX_COIN_LEAPED1", &pos);
                        break;
                    case ITEM_PIANTA_TOKEN:
                        psndSFXOn_3D("SFX_EVT_GAME_MONTE_LEAPED1", &pos);
                        break;
                    case ITEM_HEART:
                        psndSFXOn_3D("SFX_HEART_LEAPED1", &pos);
                        break;
                    case ITEM_FLOWER:
                        psndSFXOn_3D("SFX_FLOWER_LEAPED1", &pos);
                        break;
                    case ITEM_STAR_PIECE:
                        psndSFXOn_3D("SFX_STAR_PIECE_LEAPED1", &pos);
                        break;
                    case ITEM_DIAMOND_STAR:
                    case ITEM_EMERALD_STAR:
                    case ITEM_GOLD_STAR:
                    case ITEM_RUBY_STAR:
                    case ITEM_SAPPHIRE_STAR:
                    case ITEM_GARNET_STAR:
                    case ITEM_CRYSTAL_STAR:
                        psndSFXOn_3D("SFX_EVT_STARSTONE_JUMP1", &pos);
                        break;
                    default:
                        if (itemId >= BADGE_MIN && itemId < BADGE_MAX) {
                            psndSFXOn_3D("SFX_BADGE_LEAPED1", &pos);
                        } else {
                            psndSFXOn_3D("SFX_ITEM_LEAPED1", &pos);
                        }
                        break;
                }
                    isFloor = FALSE;
                    if (entry->flags & ITEM_FLAG_ELASTIC_UNUSED && entry->velocityY != 0.0f) {
                        bounce = TRUE;
                    } else if (hitGetAttr(hit) & HIT_ATTR_MOBJ && entry->velocityY != 0.0f) {
                        mobj = mobjHitObjPtrToPtr(hit);
                        if (mario->characterId == CHARACTER_BOWSER) {
                            bounce = FALSE;
                        } else if (mobj && strcmp(mobj->animName, "MOBJ_KururinFloor")) {
                            bounce = TRUE;
                            isFloor = TRUE;
                        } else {
                            goto no_bounce;
                        }
                    } else {
                    no_bounce:
                        bounce = FALSE;
                    }
                    if (!bounce) {
                        v = entry->velocityY * dt + (-980.0f * entry->gravityScale) * ((t2 + dt) * (t2 + dt) - t2sq);
                        entry->velocityY = 0.7f * (entry->bounceFactor * (-v / dt));
                    }
                    peak = fabs(0.5f * (entry->velocityY / (-980.0f * entry->gravityScale)));
                    height = entry->velocityY * peak + peak * ((-980.0f * entry->gravityScale) * peak);
                    if (!isFloor && height <= 8.0f) {
                        itemModeChange(entry, ITEM_MODE_GROUND);
                        entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_RISING;
                        entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND;
                        entry->flags2 &= ~ITEM_FLAG2_TIMER_WAIT_LAND;
                    }
                    if (entry->flags & ITEM_FLAG_BOUND_NEXT && entry->bounceCount <= 1) {
                        entry->direction = entry->savedDirection;
                        entry->speed = entry->savedSpeed;
                        entry->flags &= ~ITEM_FLAG_BOUND_NEXT;
                    }
                    entry->airTime = 0;
                    entry->baseTime = gp->renderTime;
                    break;
                }
            }
            entry->airTime += delta;
            entry->baseTime = entry->curTime;
            break;
    }
    entry->flags2 &= ~ITEM_FLAG2_SKIP_BOUND_INIT;
}

/**
 * Plays the pickup sound for coins, Pianta tokens, hearts and flowers, with sparkles for coins and tokens;
 * other items play no pickup sound here.
 */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void itemPlaySound(ItemEntry* entry) {
    Vec pos;

    pos = (Vec){entry->position.x, entry->position.y, entry->position.z};
    switch (entry->itemId) {
        case ITEM_COIN:
            effStardustEntry(1, 3, 30, entry->position.x, 35.0f + entry->position.y, entry->position.z, 16.0f, 16.0f);
            psndSFXOn_3D("SFX_COIN_GET1", &pos);
            break;
        case ITEM_PIANTA_TOKEN:
            effStardustEntry(1, 3, 30, entry->position.x, 35.0f + entry->position.y, entry->position.z, 16.0f, 16.0f);
            psndSFXOn_3D("SFX_EVT_GAME_MONTE_COUNT1", &pos);
            break;
        case ITEM_HEART:
            psndSFXOn_3D("SFX_HEART_GET1", &pos);
            break;
        case ITEM_FLOWER:
            psndSFXOn_3D("SFX_FLOWER_GET1", &pos);
            break;
        case ITEM_STAR_PIECE:
            break;
    }
}

/** Moves an item to (x, y, z). */
void itemSetPosition(ItemEntry* entry, f32 x, f32 y, f32 z) {
    entry->position.x = x;
    entry->position.y = y;
    entry->position.z = z;
}

/** Slides a move (vx, vz) along a wall with normal (nx, nz): the move minus its part into the wall, at half speed. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void _reflect(f32 vx, f32 nx, f32 vz, f32 nz, f32* mx, f32* mz) {
    f32 dot = vx * nx + vz * nz;
    *mx = 0.5f * (vx - dot * nx);
    *mz = 0.5f * (vz - dot * nz);
}

/**
 * Wall check for a moving item: casts a ray along its motion (dx, dz) and, on a wall,
 * returns the hit and the corrected position and reflected direction (degrees) in outX, outZ and angle.
 */
static HitEntry* itemHitCheckSide(ItemEntry* entry, f32* outX, f32 dx, f32* outZ, f32 dz, s32* angle) {
    HitEntry* hit;
    f32 a, b;
    f32 hitX, hitY, hitZ, hitDist, hitNX, hitNY, hitNZ;
    f32 dirSin, dirCos;
    f32 ox, oz;
    f32 h;

    *outX = entry->position.x;
    *outZ = entry->position.z;
    if (dx == 0.0f && dz == 0.0f) {
        return NULL;
    }
    sincosf(&dirSin, &dirCos, angleABf(0.0f, 0.0f, dx, dz));
    h = 15.0f;
    hitDist = 10.0f + sqrtf(dx * dx + dz * dz);
    hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ,
        entry->position.x, entry->position.y + h, entry->position.z, dirSin, 0.0f, dirCos);
    if (hit) {
        _reflect(dx, hitNX, dz, hitNZ, &ox, &oz);
        *outX += ox;
        *outZ += oz;
        a = reviseAngle(180.0f + *angle);
        b = reviseAngle(angleABf(0.0f, 0.0f, 100.0f * hitNX, 100.0f * hitNZ));
        *angle = reviseAngle(b + reviseAngle(b - a));
        return hit;
    }
    h = 15.0f;
    hitDist = 10.0f + sqrtf(dx * dx + dz * dz);
    hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ,
        entry->position.x, entry->position.y + h, entry->position.z, dirSin, 0.0f, dirCos);
    if (hit) {
        _reflect(dx, hitNX, dz, hitNZ, &ox, &oz);
        *outX += ox;
        *outZ += oz;
        a = reviseAngle(180.0f + *angle);
        b = reviseAngle(angleABf(0.0f, 0.0f, 100.0f * hitNX, 100.0f * hitNZ));
        *angle = reviseAngle(b + reviseAngle(b - a));
        return hit;
    }
    *outX += dx;
    *outZ += dz;
    return NULL;
}

HitEntry* itemHitCheckY(ItemEntry* entry, f32 z, f32 y, f32 x, f32 vy) {
    HitEntry* hit;
    f32 hitX, hitY, hitZ, hitDist, hitNX, hitNY, hitNZ;

    hitDist = 20.0 + fabs(vy);
    if ((hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, -1.0f, 0.0f)) != NULL) {
        entry->position.y = hitY;
    } else {
        hit = NULL;
        entry->position.y += vy;
    }
    return hit;
}

HitEntry* itemHitCheckUp(ItemEntry* entry, f32 vy) {
    HitEntry* hit;
    f32 hitX, hitY, hitZ, hitDist, hitNX, hitNY, hitNZ;
    f32 x, y, z;

    if (vy <= 0.0f) {
        return NULL;
    }
    x = entry->position.x;
    y = 20.0f + entry->position.y;
    z = entry->position.z;

    hitDist = 20.0 + fabs(vy);
    if ((hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, 1.0f, 0.0f)) != NULL) {
        entry->position.y = hitY - 20.0f - 4.0f;
        return hit;
    }
    x = entry->position.x;
    y = entry->position.y;
    z = entry->position.z;

    hitDist = 20.0 + fabs(vy);
    if ((hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, 1.0f, 0.0f)) != NULL) {
        entry->position.y = hitY - 20.0f;
        return hit;
    }
    return NULL;
}

/**
 * Starts the get sequence for an item at once (collected by a script or partner),
 * without waiting for Mario's get motion.
 */
void itemForceGet(ItemEntry* entry) {
    entry->timer = 0;
    itemModeChange(entry, ITEM_MODE_GET);
    entry->flags |= ITEM_FLAG_FORCE_GET;
    entry->flags2 &= ~ITEM_FLAG2_RELEASED;
}

/**
 * Koops's shell has fetched the item: starts the get sequence if Mario may collect now,
 * else drops the item back to rest where it is.
 */
void itemNokoForceGet(ItemEntry* entry) {
    if (itemGetNokoCheck(entry)) {
        entry->timer = 0;
        itemModeChange(entry, ITEM_MODE_GET);
        entry->flags |= (ITEM_FLAG_FORCE_GET | ITEM_FLAG_NOKO_GET);
        entry->flags2 &= ~ITEM_FLAG2_RELEASED;
    } else {
        entry->timer = 0;
        itemModeChange(entry, ITEM_MODE_GROUND);
        entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_RISING;
        entry->flags2 &= ~ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND;
        entry->flags2 &= ~ITEM_FLAG2_TIMER_WAIT_LAND;
        entry->flags2 &= ~ITEM_FLAG2_CARRIED;
    }
}

void itemFlagOn(ItemEntry* entry, u16 flags) {
    entry->flags |= flags;
}

void itemFlagOff(ItemEntry* entry, u16 flags) {
    entry->flags &= ~flags;
}

u16 itemFlag(ItemEntry* entry, u16 flags) {
    return entry->flags & flags;
}

void itemStatusOn(ItemEntry* entry, u32 flags) {
    entry->flags2 |= flags;
}

u32 itemStatus(ItemEntry* entry, u32 flags) {
    return entry->flags2 & flags;
}

/** A partner picks up the item and carries it (mode 8, following above the partner); it stops blinking. */
void itemPickUp(ItemEntry* entry) {
    entry->flags2 |= ITEM_FLAG2_CARRIED;
    itemModeChange(entry, ITEM_MODE_CARRIED);
    entry->timer = 0;
    entry->flags2 &= ~ITEM_FLAG2_HIDDEN;
}
