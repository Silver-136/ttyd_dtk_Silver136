#include "driver/itemdrv.h"
#include "eff/eff_itemget.h"
#include "driver/swdrv.h"
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

inline Vec unknownVec0(void) {
    return (Vec){ 0.0f, 0.0f, 0.0f };
}

inline Vec makeVecByValue2(f32 x, f32 y, f32 z) {
    Vec v = (Vec){ 0.0f, 0.0f, 0.0f };
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

inline Vec makeVecByValue(f32 x, f32 y, f32 z) {
    Vec v = (Vec){ 0.0f, 0.0f, 0.0f };
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

extern ItemData itemDataTable[];
s32 msgWindow_Entry(const char* msg, s32 callback, s32 priority);
EffectEntry* effHagetakaFlushN64Entry(s32 a1, s32 a2, f32 x, f32 y, f32 z, f32 scale);

BOOL itemGetNokoCheck(ItemEntry* entry);
void winNameDisp(WinMgrEntry* win);
void winNameDisp2(WinMgrEntry* win);
void winHelpMain(WinMgrEntry* win);
void winHelpDisp(WinMgrEntry* win);
void winFullDisp(WinMgrEntry* win);

HitEntry* itemHitCheckY(ItemEntry* entry, f32 z, f32 y, f32 x, f32 vy);
HitEntry* itemHitCheckUp(ItemEntry* entry, f32 vy);

HitEntry* itemHitCheckSide(ItemEntry* entry, f32* outX, f32 dx, f32* outZ, f32 dz, s32* angle);
void itemseq_GetItem(ItemEntry* entry);
ItemEntry* itemEntry(const char* name, s32 itemId, s32 type, s32 collectFlag, s32 evtCode, f32 x, f32 y, f32 z);

//.bss
static ItemWork work[2];

//.data
WinMgrDesc itemdrv_window_desc[4] = {
    {0, 0, 8, -175, 120, 350, 42, {0xE1, 0xD7, 0x9B, 0xFF}, NULL, winNameDisp},
    {0, 0, 8, -240, -130, 500, 80, {0xF0, 0xF0, 0xF0, 0xFF}, winHelpMain, winHelpDisp},
    {0, 0, 8, -175, 200, 350, 75, {0x2B, 0xA4, 0xFF, 0xFF}, NULL, winFullDisp},
    {0, 0, 8, -175, 120, 350, 42, {0xE1, 0xD7, 0x9B, 0xFF}, NULL, winNameDisp2},
};

#define itemGetWork() (gp->inBattle ? &work[1] : &work[0])

inline void itemCheckBadge(ItemEntry* entry) {
    if (!gp->inBattle) {
        if (BADGE_MIN <= entry->itemId && entry->itemId < BADGE_MAX) {
            if (entry->flags & 0x100) {
                pouchArriveBadge(entry->itemId);
            }
        }
    }
}

void itemseq_Bound(ItemEntry* entry);
static inline BOOL itemGetPouch(ItemEntry* entry) {
    EventEntry* evt;

    if (!pouchGetItem(entry->itemId)) {
        return FALSE;
    }
    if (entry->flags & ITEM_FLAG_COLLECT_GSWF) {
        swSet(entry->collectFlag);
    }
    else if (entry->flags & ITEM_FLAG_COLLECT_LSWF) {
        _swSet(entry->collectFlag);
    }
    if ((u32)entry->evtCode != 0) {
        evt = evtEntry((EvtWord*)entry->evtCode, 0, EVENT_START_IMMEDIATE);
        evt->lwData[0] = FLOAT(entry->position.x);
        evt->lwData[1] = FLOAT(entry->position.y);
        evt->lwData[2] = FLOAT(entry->position.z);
    }
    entry->flags2 |= 1;
    return TRUE;
}

static inline void itemGetEnd(ItemEntry* entry) {
    marioStSystemLevel(0);
    psndClearFlag(PSND_FLAG_MUTE_SFX);
    if (entry->flags2 & 0x1000000) {
        entry->flags2 &= ~0x1000000;
        marioCtrlOn();
    }
    marioKeyOn();
    marioChgStayMotion();
    camGetPtr(CAMERA_2D)->flags &= ~0x200;
}

void itemGetSound(ItemEntry* entry);

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

void itemReInit(void) {
    ItemWork* wp = itemGetWork();
    ItemEntry* entry;
    int i;

    if (!gp->inBattle) {
        for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
            if (entry->flags & ITEM_FLAG_ACTIVE) { //flush all current badges
                itemCheckBadge(entry);
            }
        }
    }

    //clear all existing entries
    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        iconDelete(entry->name);
        entry->flags &= ~ITEM_FLAG_ACTIVE;
        memset(entry->name, 0, sizeof(entry->name));
    }
}

void itemCoinDrop(Vec* position) {
    ItemEntry* entry;

    if (pouchGetCoin() > 0) {
        pouchAddCoin(-1);
        entry = itemEntry(NULL, ITEM_COIN, 8, -1, 0, position->x, position->y, position->z);
        if (entry) {
            entry->flags2 |= 0x40;
        }
    }
}

BOOL itemGetCheck(ItemEntry* entry) {
    MarioWork* mario = marioGetPtr();

    if (!entry) return FALSE;
    if (entry->flags2 & 0x100000) return FALSE;
    if (mario->characterId == CHARACTER_BOWSER) {
        if (!(mario->stageFlags & 1)) {
            if (marioStGetSystemLevel() != 0) return FALSE;
            if (mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF)) return FALSE;
        }
    }
    else {
        if (marioStGetSystemLevel() != 0) return FALSE;
        if (mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF)) return FALSE;
    }
    if (gp->mapEventId) return FALSE;
    if (seqGetSeq() != 2) return FALSE;
    if (mario->currMotionId == MARIO_MOTION_SHIP) return FALSE;
    if (mario->currMotionId == MARIO_MOTION_PLANE) return FALSE;
    if (vivianGetStatus()) return FALSE;
    if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) return FALSE;
    if (nokonoko_holdItem()) return FALSE;
    {
        ItemWork* wp = itemGetWork();
        ItemEntry* item;
        int i;
        for (item = wp->entries, i = 0; i < wp->count; i++, item++) {
            if (item->flags & ITEM_FLAG_ACTIVE && item->mode == 2) {
                return FALSE;
            }
        }
    }
    if (N_fbatPreventMarioEventChk()) return FALSE;
    switch (entry->mode) {
        case 5:
        case 6:
            if (!entry->subState) break;
            if (entry->flags2 & 4) break;
            if (entry->flags2 & 8) break;
            return TRUE;
        case 3:
        case 4:
            if (entry->flags2 & 8) break;
            return TRUE;
        case 1:
        case 7:
            return TRUE;
    }
    return FALSE;
}

BOOL itemGetNokoCheck(ItemEntry* entry) {
    MarioWork* mario = marioGetPtr();

    if (!entry) return FALSE;
    if (entry->flags2 & 0x100000) return FALSE;
    if (marioStGetSystemLevel() != 0) return FALSE;
    if (mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF)) return FALSE;
    if (entry->flags2 & 0x2000000) return FALSE;
    if (gp->mapEventId) return FALSE;
    if (seqGetSeq() != 2) return FALSE;
    if (mario->currMotionId == MARIO_MOTION_SHIP) return FALSE;
    if (vivianGetStatus()) return FALSE;
    if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) return FALSE;
    if ((u16)(mario->currMotionId - 5) <= 3 || mario->currMotionId == MARIO_MOTION_GET_ITEM || mario->currMotionId == MARIO_MOTION_TALK) return FALSE;
    {
        ItemWork* wp = itemGetWork();
        ItemEntry* item;
        int i;
        for (item = wp->entries, i = 0; i < wp->count; i++, item++) {
            if (item->flags & ITEM_FLAG_ACTIVE && item->mode == 2) {
                return FALSE;
            }
        }
    }
    return !N_fbatPreventMarioEventChk();
}

BOOL N_itemPickUpFromFieldCheck(void) {
    ItemWork* wp = itemGetWork();
    ItemEntry* entry;
    int i;

    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & ITEM_FLAG_ACTIVE && entry->mode == 2) {
            return TRUE;
        }
    }
    return FALSE;
}

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

    mario = marioGetPtr();
    entry = itemHitCheck(mario->position.x, mario->position.y, mario->position.z, mario->wPlayerCollisionBox.x * 0.5f + 12.0f);
    if (itemGetCheck(entry)) {
        itemModeChange(entry, 2);
    }

    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (!(entry->flags & ITEM_FLAG_ACTIVE)) continue;
        mode = entry->mode;
        active = TRUE;
        switch (mode) {
            case 2:
            case 7:
            case 8:
            case 10:
                break;
            default:
                if (gp->systemLevelFlags & 0xF) {
                    active = FALSE;
                }
                break;
        }
        if (!active) {
            entry->baseTime = gp->renderTime;
            continue;
        }
        if (mode != 2) {
            if (entry->timer == 0) {
                entry->flags2 &= ~0x100;
            }
            else if (entry->flags2 & 0x4000) {
                entry->flags2 &= ~0x100;
            }
            else {
                entry->timer -= (f32)OSTicksToMicroseconds(gp->renderTime - entry->baseTime) / 1000.0f;
                if (entry->timer <= 0) {
                    itemModeChange(entry, 9);
                }
                else if (entry->timer <= 3000) {
                    if ((entry->timer >> 4) & 1) {
                        entry->flags2 ^= 0x100;
                    }
                }
            }
        }
        else {
            entry->flags2 &= ~0x100;
        }

        switch (entry->mode) {
            case 5:
            case 6:
            case 10:
                itemseq_Bound(entry);
                break;
            case 3:
            case 4:
                itemseq_Bound(entry);
                break;
            case 1:
                y = entry->position.y; { f32 dirY = -1.0f;
                if (entry->flags2 & 0x100000) {
                    hit = NULL;
                    newY = y + dirY;
                }
                else {
                    f32 hitNZ, hitNY, hitNX, hitDist, hitZ, hitY, hitX;
                    f32 px = entry->position.x, py = entry->position.y + 20.0f, pz = entry->position.z;
                    { f64 d = __fabs(-1.0); hitDist = 20.0 + d; }
                    hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, px, py, pz, 0.0f, -1.0f, 0.0f);
                    if (hit) {
                        newY = hitY;
                    }
                    else {
                        hit = NULL;
                        newY = y + dirY;
                    }
                } }
                if (!hit) {
                    itemModeChange(entry, 3);
                }
                else {
                    entry->position.y = newY;
                }
                break;
            case 2:
                itemseq_GetItem(entry);
                break;
            case 8:
                party = partyGetPtr(marioGetPartyId());
                if (party) {
                    f32 px = party->position.x, py = 15.0f + party->position.y, pz = party->position.z;
                    entry->position.x = px;
                    entry->position.y = py;
                    entry->position.z = pz;
                }
                else {
                    itemModeChange(entry, 3);
                    entry->flags2 &= ~0x1000;
                }
                break;
            case 9:
                itemDelete(entry->name);
                break;
        }

        if (entry->flags & ITEM_FLAG_ACTIVE) {
            switch (entry->itemId) {
                case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: case 0x78:
                    if (!(entry->flags2 & 0x100)) {
                        ((EffStarStoneWork*)entry->effect->userdata)->position.x = entry->position.x;
                        ((EffStarStoneWork*)entry->effect->userdata)->position.y = 16.0f + entry->position.y;
                        ((EffStarStoneWork*)entry->effect->userdata)->position.z = entry->position.z;
                    }
                    else {
                        ((EffStarStoneWork*)entry->effect->userdata)->position.x = entry->position.x;
                        ((EffStarStoneWork*)entry->effect->userdata)->position.y = -1000.0f;
                        ((EffStarStoneWork*)entry->effect->userdata)->position.z = entry->position.z;
                    }
                    break;
                default:
                    if (!(entry->flags2 & 0x100)) {
                        iconSetPos(entry->name, entry->position.x, entry->position.y, entry->position.z);
                        iconSetScale(entry->name, entry->scale);
                        iconSetAlpha(entry->name, entry->alpha);
                    }
                    else {
                        iconSetPos(entry->name, entry->position.x, -1000.0f, entry->position.z);
                    }
                    break;
            }
            entry->baseTime = gp->renderTime;
            if (entry->itemId == 0x79 || entry->itemId == 0x7A) {
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

void itemModeChange(ItemEntry* entry, s32 mode) {
    entry->mode = mode;
    entry->subState = 0;
}

#ifdef VERSION_G8ME01
static inline ItemEntry* itemFindDeletable(ItemWork* wp, s32 itemId) {
    ItemEntry* entry;
    ItemEntry* found;
    int i;

    found = NULL;
    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & ITEM_FLAG_ACTIVE && entry->itemId == itemId && entry->flags & 0x400) {
            found = entry;
            break;
        }
    }
    return found;
}
#endif

ItemEntry* itemEntry(const char* name, s32 itemId, s32 type, s32 collectFlag, s32 evtCode, f32 x, f32 y, f32 z) {
    ItemWork* wp = itemGetWork();
    int i;
    ItemEntry* entry;
#ifdef VERSION_G8ME01
    // FAKE: e/base and the explicit 0x79/0x7C search loops (0x7B uses the helper) steer register
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
        }
        else if (collectFlag >= LSWF(0) && collectFlag <= EVTDAT_LSWF_MAX) {
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

#ifdef VERSION_G8ME01
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
#ifdef VERSION_G8ME01
        entry = NULL;
        for (e = base, i = 0; i < wp->count; i++, e++) {
            if (e->flags & ITEM_FLAG_ACTIVE && e->itemId == 0x79 && e->flags & 0x400) {
                entry = e;
                break;
            }
        }
        if (entry) {
#else
        entry = wp->entries;
        if (entry->flags & ITEM_FLAG_ACTIVE && entry->itemId == 0x79 && entry->flags & 0x400) {
#endif
            itemDelete(entry->name);
        }
        else {
#ifdef VERSION_G8ME01
            entry = NULL;
            for (e = base, i = 0; i < wp->count; i++, e++) {
                if (e->flags & ITEM_FLAG_ACTIVE && e->itemId == 0x7C && e->flags & 0x400) {
                    entry = e;
                    break;
                }
            }
            if (entry) {
#else
            entry = wp->entries;
            if (entry->flags & ITEM_FLAG_ACTIVE && entry->itemId == 0x7C && entry->flags & 0x400) {
#endif
                itemDelete(entry->name);
            }
            else {
#ifdef VERSION_G8ME01
                entry = itemFindDeletable(wp, 0x7B);
                entry = NULL;
#else
                entry = wp->entries;
                if (entry->flags & ITEM_FLAG_ACTIVE && entry->itemId == 0x7B && entry->flags & 0x400) {
                    itemDelete(entry->name);
                }
                else {
                    entry = NULL;
                }
#endif
            }
        }
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
    entry->evtCode = evtCode;
    entry->flags |= ITEM_FLAG_ACTIVE;
    entry->scale = 1.0f;
    entry->alpha = 0xFF;
    if (collectFlag != -1) {
        if (isGswf) {
            entry->collectFlag = collectFlag;
            entry->flags |= ITEM_FLAG_COLLECT_GSWF;
        }
        else if (isLswf) {
            entry->collectFlag = collectFlag;
            entry->flags |= ITEM_FLAG_COLLECT_LSWF;
        }
    }
    else {
        entry->collectFlag = -1;
    }

    switch (entry->itemId) {
    case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: case 0x78:
        entry->effect = effStarStoneEntry(entry->itemId - 0x72, x, 16.0f + y, z, 1.0f);
        break;
    default:
        entry->effect = NULL;
        iconEntry(entry->name, itemDataTable[itemId].icon_id);
        { char* nm = entry->name; iconSetPos(nm, x, y, z); }
        iconSetScale(entry->name, entry->scale);
        iconSetAlpha(entry->name, entry->alpha);
        break;
    }
    entry->bounceFactor = entry->gravityScale = 1.0f;
    entry->baseTime = gp->renderTime;
    switch (entry->itemId) {
    case 0x79: case 0x7A: case 0x7B: case 0x7C:
        entry->flags |= 4;
        break;
    }

    switch ((u16)type) {
        case 17:
            entry->flags |= 2;
        case 16:
            itemModeChange(entry, 7);
            break;
        case 0:
            itemModeChange(entry, 1);
            break;
        case 1:
            itemModeChange(entry, 1);
            entry->flags |= 2;
            break;
        case 4:
            itemModeChange(entry, 6);
            break;
        case 3:
            entry->timer = 5000;
            itemModeChange(entry, 5);
            break;
        case 11:
            entry->flags2 |= 0x4000;
            entry->timer = 20000;
            itemModeChange(entry, 6);
            break;
        case 12:
            entry->flags2 |= 0x4000;
            entry->timer = 20000;
            itemModeChange(entry, 6);
            entry->flags |= 0x400;
            break;
        case 9:
            entry->flags |= 0x100;
        case 8:
            entry->flags2 |= 0x4000;
            entry->timer = 8000;
            itemModeChange(entry, 6);
            break;
        case 10:
            entry->flags2 |= 0x4000;
            entry->timer = 8000;
            itemModeChange(entry, 6);
            entry->flags |= 0x400;
            break;
        case 7:
            entry->flags2 |= 0x4000;
        case 5:
            entry->timer = 5000;
            itemModeChange(entry, 6);
            break;
        case 6:
            entry->timer = 5000;
            entry->flags2 |= 0x40;
            itemModeChange(entry, 6);
            break;
        case 2:
            itemModeChange(entry, 5);
            break;
        case 13:
            itemModeChange(entry, 3);
            break;
        case 14:
            itemModeChange(entry, 4);
            break;
        case 15:
            itemModeChange(entry, 10);
            break;
    }
    return entry;
}

void itemDelete(const char* name) {
    ItemWork* wp = itemGetWork();
    ItemEntry* entry;
    int i;

    for (entry = wp->entries, i = 0; i < wp->count; i++, entry++) {
        if (!strcmp(entry->name, name)) {
            itemCheckBadge(entry);
            switch (entry->itemId) {
                case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: case 0x78:
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
        if (!(entry->flags & ITEM_FLAG_ACTIVE)) continue;
        // FAKE: the empty do-while adds a block before the continue; without it the flag tests are scheduled differently
        if (entry->flags & 2) { do { } while (0); continue; }
        if (entry->flags2 & 0x1000) continue;
        if (entry->mode == 2) continue;
        dx = x - entry->position.x; dy = y - entry->position.y;
        dz = z - entry->position.z;
        xzdist = sqrtf(dx * dx + dz * dz);
        dist = sqrtf(dx * dx + dy * dy + dz * dz);
        if (xzdist < range && marioItemGetOk() && marioGetJabaraState() != 1 &&
            marioGetJabaraState() != 4 && marioGetJabaraState() != 5)
        {
            if (__fabs((y + mario->wPlayerCollisionBox.y * 0.5f) - entry->position.y) < mario->wPlayerCollisionBox.y * 0.5f + 10.0f) {
                hit = NULL;
                if (y < entry->position.y) {
                    hitDist = 10.0f + (entry->position.y - y);
                    if (hitDist > 0.0f) {
                        hit = hitCheckAttr(0x80000000, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, 1.0f, 0.0f);
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
    pos = makeVecByValue2(x, y, z);
    for (; i < wp->count; i++, entry++) {
        if (!(entry->flags & ITEM_FLAG_ACTIVE)) continue;
        if (entry->flags2 & 0x1000) continue;
        if (entry->flags2 & 0x100000) continue;
        if (entry->mode == 2) continue;
        if (entry->flags2 & 0x2000000) continue;
        {
            f32 iz, iy, ix;
            ix = entry->position.x;
            iy = entry->position.y;
            iz = entry->position.z;
            itemPos = makeVecByValue(ix, iy, iz);
        }
        PSVECSubtract(&itemPos, &pos, &diff);
        dist = PSVECMag(&diff);
        if (dist < range) {
            found = entry;
            range = dist;
        }
    }
    return found;
}

#ifdef VERSION_G8ME01
void evtMsgFmtStr(char* dst, const char* src);

#endif
void winNameDisp(WinMgrEntry* win) {
    ItemEntry* entry = win->param;
    const char* name;
    u16 width;
#ifdef VERSION_G8ME01
    u16 lines;
#endif
    char buffer[1024];

    name = msgSearch(itemDataTable[entry->itemId].name);
#ifdef VERSION_G8ME01
    if ((u32)gp->language == 0) {
        if (entry->itemId >= 0xF0 && entry->itemId < 0x153) {
            sprintf(buffer, msgSearch("msg_window_badge_get"), name);
        }
        else {
            sprintf(buffer, msgSearch("msg_window_item_get"), name);
        }
    }
    else {
        if (entry->itemId >= 0xF0 && entry->itemId < 0x153) {
            strcpy(buffer, msgSearch("msg_window_badge_get"));
        }
        else {
            strcpy(buffer, msgSearch("msg_window_item_get"));
        }
        evtMsgFmtStr(buffer, name);
    }
    width = FontGetMessageWidthLine(buffer, &lines);
#else
    if (entry->itemId >= 0xF0 && entry->itemId < 0x153) {
        sprintf(buffer, msgSearch("msg_window_badge_get"), name);
    }
    else {
        sprintf(buffer, msgSearch("msg_window_item_get"), name);
    }
    width = FontGetMessageWidth(buffer);
#endif
    if (width + 20 > win->width) {
        win->width = width + 20;
        win->x = -win->width / 2;
    }
#ifdef VERSION_G8ME01
    win->height = win->description->height + lines * 24;
#endif
    if (!winMgrAction(entry->winId[0])) {
        FontDrawStart();
        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
        FontDrawMessage(-width / 2, win->y - 8, buffer);
    }
}


void winNameDisp2(WinMgrEntry* win) {
    ItemEntry* entry = win->param;
    const char* name;
    u16 width;
#ifdef VERSION_G8ME01
    u16 lines;
#endif
    char buffer[1024];

    name = msgSearch(itemDataTable[entry->itemId].name);
#ifdef VERSION_G8ME01
    if ((u32)gp->language == 0) {
        sprintf(buffer, msgSearch("msg_window_item_release"), name);
    }
    else {
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
#ifdef VERSION_G8ME01
    win->height = win->description->height + lines * 24;
#endif
    if (!winMgrAction(entry->winId[0])) {
        FontDrawStart();
        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
        FontDrawMessage(-width / 2, win->y - 8, buffer);
    }
}


void winHelpMain(WinMgrEntry* win) {
    winMgrHelpMain(win);
}

void winHelpDisp(WinMgrEntry* win) {
    ItemEntry* entry = win->param;

    if (!winMgrAction(entry->winId[1])) {
        win->msg = msgSearch(itemDataTable[entry->itemId].description);
        winMgrHelpDraw(win);
    }
}

void winFullDisp(WinMgrEntry* win) {
    ItemEntry* entry = win->param;
    BOOL isBadge = FALSE;
    u16 width;
    const char* msg;

    if (entry->itemId >= 0xF0 && entry->itemId < 0x153) {
        isBadge = TRUE;
    }
    msg = isBadge ? msgSearch("msg_window_badge_full") : msgSearch("msg_window_item_full");
    width = FontGetMessageWidth(msg);
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

void itemseq_GetItem(ItemEntry* entry) {
    ItemWork* wp = itemGetWork();
    MarioWork* mario;
    s32 type, bgm;
    s32 select;

    mario = marioGetPtr();
    switch (entry->subState) {
        case 0:
            entry->flags2 &= ~0x10000;
            if (entry->flags & 0x100) {
                entry->flags2 |= 0x4000000;
            }
            else {
                entry->flags2 &= ~0x4000000;
            }
            entry->flags &= ~0x100;
            if (entry->flags & 4) {
                if (itemGetPouch(entry)) {
                    entry->timer = 0;
                    itemGetSound(entry);
                    itemDelete(entry->name);
                }
                else {
                    itemModeChange(entry, 4);
                }
                break;
            }
            entry->subState = 1;
            entry->timer = 0;
            camGetPtr(CAMERA_2D)->flags |= 0x200;
            marioKeyOff();
            marioStSystemLevel(2);
            psndSetFlag(PSND_FLAG_MUTE_SFX);
            mario->floorMove.x = 0.0f;
            mario->floorMove.z = 0.0f;
            mario->baseSpeed = 0.0f;
            mario->moveDist = 0.0f;
            if (entry->itemId == 7 || entry->itemId == 8 || entry->itemId == 10 || entry->itemId == 11) {
                entry->flags2 |= 0x2000;
            }
        case 1:
            if (!(entry->flags & 0x1000) || entry->flags & 0x4000) {
                if (!marioChkItemGetMotion()) {
                    if (mario->currMotionId == MARIO_MOTION_JUMP_SW || (u16)(mario->currMotionId - 6) <= 2 ||
                        mario->currMotionId == MARIO_MOTION_GET_ITEM || mario->currMotionId == MARIO_MOTION_TALK)
                    {
                        entry->subState = 8;
                    }
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
            if (entry->itemId == 0x57) {
                type = 4;
                bgm = -1;
            }
            else if (entry->itemId == 0x7D) {
                type = 8;
                bgm = 0;
            }
            else if (entry->itemId < 0xC) {
                type = 3;
                bgm = 3;
            }
            else if (entry->itemId < 0x72) {
                type = 6;
                bgm = 1;
            }
            else if (entry->itemId < 0x79) {
                type = 0;
                bgm = -1;
            }
            else if (entry->itemId < 0xEC) {
                type = 7;
                bgm = -1;
            }
            else if (entry->itemId < 0x153) {
                type = 5;
                bgm = 2;
            }
            wp->effect = effItemGetEntry(type, mario->position.x, 62.0f + mario->position.y, mario->position.z);
            if (entry->flags2 & 0x2000) {
                ((ItemGetWork*)wp->effect->userdata)->camera = 5;
                iconFlagOn(entry->name, 0x200);
                marioSetCamId(CAMERA_3D_EFFECTB);
            }
            else {
                ((ItemGetWork*)wp->effect->userdata)->camera = 5;
                iconFlagOn(entry->name, 0x100);
                if (type == 0 || type == 3) {
                    marioSetCamId(CAMERA_3D_EFFECTA);
                }
            }
            if (!(entry->flags & 0x8000)) {
                if (psndBGMChkSilent(0)) {
                    entry->flags |= 8;
                }
                switch (bgm) {
                    case 0:
                        if (!(entry->flags & 8)) {
                            psndBGMOff(0x3800);
                        }
                        psndBGMOn(0x211, "BGM_FF_GET_STARPIECE1");
                        break;
                    case 1:
                        if (!(entry->flags & 8)) {
                            psndBGMOff(0x3800);
                        }
                        psndBGMOn(0x211, "BGM_FF_GET_KEYITEM1");
                        break;
                    case 2:
                        if (!(entry->flags & 8)) {
                            psndBGMOff(0x3800);
                        }
                        psndBGMOn(0x211, "BGM_FF_GET_BADGE1");
                        break;
                    case 3:
                        if (!(entry->flags & 8)) {
                            psndBGMOff(0x3800);
                        }
                        psndBGMOn(0x211, "BGM_FF_GET_IMPORTANT_ITEM1");
                        break;
                    default:
                        if (!(entry->flags & 8)) {
                            psndBGMOff(0x3800);
                        }
                        psndBGMOn(0x211, "BGM_FF_GET_ITEM1");
                        break;
                }
            }
            if (type == 8) {
                evtSetValue(NULL, 0xF5DE01A2, evtGetValue(NULL, 0xF5DE01A2) + 1);
            }
            entry->flags &= ~0x2000;
            itemGetSound(entry);
            entry->winId[0] = winMgrEntry(&itemdrv_window_desc[0]);
            winMgrOpen(entry->winId[0]);
            winMgrSetParam(entry->winId[0], entry);
            if (!(entry->flags2 & 0x2000)) {
                entry->winId[1] = winMgrEntry(&itemdrv_window_desc[1]);
                winMgrOpen(entry->winId[1]);
                winMgrSetParam(entry->winId[1], entry);
            }
            if (!itemGetPouch(entry)) {
                entry->winId[2] = winMgrEntry(&itemdrv_window_desc[2]);
                winMgrOpen(entry->winId[2]);
                winMgrSetParam(entry->winId[2], entry);
            }
            if (entry->itemId == 0x49) {
#ifdef VERSION_G8ME01
                iconChange(entry->name, 0x11B);
#else
                iconChange(entry->name, 0x114);
#endif
            }
            if (!entry->effect) {
                iconNameToPtr(entry->name)->uflags |= 0x80;
            }
            break;
        case 2:
            if (!(entry->flags2 & 0x2000)) {
                marioCtrlOff();
                entry->flags2 |= 0x1000000;
            }
            entry->subState = 3;
        case 3:
            if (!(entry->flags & 0x8000) && !(entry->flags & 0x2000)) {
                if (psndBGMChk(1)) {
                    entry->flags |= 0x2000;
                    psndBGMOff(0x201);
                    if (!(entry->flags & 8)) {
                        psndBGMOn(0xA0, NULL);
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
            if (!(entry->flags2 & 0x2000)) {
                winMgrCloseAutoDelete(entry->winId[1]);
            }
            if (!(entry->flags & 0x8000) && !(entry->flags & 0x2000)) {
                psndBGMOff(0x201);
                if (!(entry->flags & 8)) {
                    psndBGMOn(0xA0, NULL);
                }
            }
            if (entry->flags2 & 0x2000) {
                entry->subState = 6;
                break;
            }
            if (entry->itemId >= 0x79 && entry->itemId < 0xEC && entry->itemId != 0x7D && !swGet(0xEB) && !(gp->flags & 0x1000)) {
                swSet(0xEB);
                entry->subState = 4;
                wp->msgWindowId = msgWindow_Entry(msgSearch("item_first_hit"), 0, 0);
                break;
            }
            effSoftDelete(wp->effect);
            marioSetCamId(CAMERA_3D);
            iconFlagOff(entry->name, 0x100);
            if (entry->itemId == 0x49) {
#ifdef VERSION_G8ME01
                iconChange(entry->name, 0x11C);
#else
                iconChange(entry->name, 0x115);
#endif
            }
            if (!entry->effect) {
                iconNameToPtr(entry->name)->uflags &= ~0x80;
            }
            if (entry->flags2 & 1) {
                itemGetEnd(entry);
                itemDelete(entry->name);
                break;
            }
            entry->subState = 5;
            winMgrCloseAutoDelete(entry->winId[2]);
            marioChgStayMotion();
            iconFlagOn(entry->name, 2);
            {
                s32 id = entry->itemId;
                if (id >= 0xF0 && id < 0x153) {
                    wp->select = winMgrSelectEntry(2, id, 1);
                }
                else {
                    wp->select = winMgrSelectEntry(1, id, 1);
                }
            }
            break;
        case 4:
            if (windowCheckID(wp->msgWindowId)) {
                break;
            }
            effSoftDelete(wp->effect);
            marioSetCamId(CAMERA_3D);
            iconFlagOff(entry->name, 0x100);
            itemGetEnd(entry);
            itemDelete(entry->name);
            break;
        case 5:
            select = winMgrSelect(wp->select);
            if (!select) {
                break;
            }
            winMgrSelectDelete(wp->select);
            if (select < 0) {
                if (entry->flags2 & 0x4000000) {
                    entry->flags |= 0x100;
                }
            }
            else {
                pouchRemoveItem(select);
                itemGetPouch(entry);
                entry->flags2 &= ~1;
                entry->itemId = select;
                entry->evtCode = 0;
                entry->collectFlag = 0;
                entry->flags &= ~(ITEM_FLAG_COLLECT_GSWF | ITEM_FLAG_COLLECT_LSWF);
                iconChange(entry->name, itemDataTable[entry->itemId].icon_id);
                wp->currId++;
                sprintf(iconNameToPtr(entry->name)->name, "_i%05x", wp->currId);
                sprintf(entry->name, "_i%05x", wp->currId);
                entry->flags |= 0x100;
            }
            entry->flags &= ~0x1000;
            entry->speed = 85.0f;
            entry->velocityY = 120.0f;
            entry->bounceFactor = 0.7f;
            entry->gravityScale = 1.0f;
            entry->winId[0] = winMgrEntry(&itemdrv_window_desc[3]);
            winMgrOpen(entry->winId[0]);
            winMgrSetParam(entry->winId[0], entry);
            marioChgGetItemMotion();
            iconFlagOff(entry->name, 2);
            if (mario->flags & MARIO_FLAG_BG_MODE) {
                entry->flags2 |= 0x8000;
            }
            entry->subState = 9;
            break;
        case 6:
            wp->lectureEvt = evtEntryType(all_juyoitem_lecture, 0, 0, 0);
            wp->lectureEvt->lwData[8] = (EvtWord)entry;
            wp->lectureEvt->lwData[9] = (EvtWord)wp->effect;
            psndClearFlag(PSND_FLAG_MUTE_SFX);
            entry->subState = 7;
            break;
        case 7:
            if (evtCheckID(wp->lectureEvt->eventId)) {
                break;
            }
            itemGetEnd(entry);
            itemDelete(entry->name);
            break;
        case 9:
            if (!(keyGetButtonTrg(0) & PAD_BUTTON_A)) {
                break;
            }
            winMgrCloseAutoDelete(entry->winId[0]);
            entry->flags2 |= 8;
            itemModeChange(entry, 6);
            entry->flags &= ~0x10;
            if (entry->itemId >= 1 && entry->itemId < 0x79) {
                entry->timer = 0;
            }
            else if (entry->itemId >= 0xF0 && entry->itemId < 0x153) {
                entry->timer = 0;
            }
            else {
                entry->timer = 20000;
            }
            entry->flags2 |= 0x10000;
            entry->flags2 &= ~0x1000;
            entry->flags &= ~2;
            itemGetEnd(entry);
            break;
        case 8:
            if (mario->flags & MARIO_FLAG_BG_MODE) {
                entry->flags2 |= 0x8000;
            }
            entry->timer = 0;
            if (entry->flags2 & 0x1000) {
                itemModeChange(entry, 1);
                entry->flags2 &= ~4;
                entry->flags2 &= ~8;
                entry->flags2 &= ~0x4000;
                entry->flags2 &= ~0x1000;
                entry->flags &= ~0x1000;
            }
            else {
                entry->speed = 85.0f;
                entry->velocityY = 120.0f;
                entry->bounceFactor = 0.7f;
                entry->gravityScale = 1.0f;
                entry->flags2 |= 8;
                itemModeChange(entry, 6);
                entry->flags &= ~0x10;
                entry->flags2 &= ~0x1000;
                entry->flags &= ~2;
                entry->flags &= ~0x1000;
            }
            marioStSystemLevel(0);
            psndClearFlag(PSND_FLAG_MUTE_SFX);
            if (entry->flags2 & 0x1000000) {
                entry->flags2 &= ~0x1000000;
                marioCtrlOn();
            }
            marioKeyOn();
            break;
    }
}

static s32 _move_dir_1090;
void itemseq_Bound(ItemEntry* entry) {
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
            if (entry->flags2 & 0x10) {
                break;
            }
            switch (entry->mode) {
                case 6:
                    entry->flags2 |= 4;
                case 5:
                    if (!(entry->flags & 0x10)) {
                        entry->bounceFactor = 1.0f;
                        entry->gravityScale = 1.0f;
                        entry->maxBounces = 0;
                        entry->velocityY = 500.0f;
                    }
                    if (!(entry->flags2 & 0x40)) {
                        switch (entry->itemId) {
                            case 0x79:
                            case 0x7B:
                            case 0x7C:
                                entry->speed = irand(30) + 45;
                                if (irand(100) < 50) {
                                    entry->direction = 90.0f + camera->viewYaw + irand(60) - 30.0f;
                                }
                                else {
                                    entry->direction = camera->viewYaw - 90.0f + irand(60) - 30.0f;
                                }
                                break;
                            default:
                                entry->speed = 100.0f;
                                if (irand(100) < 50) {
                                    entry->direction = 90.0f + camera->viewYaw + irand(30) - 15.0f;
                                }
                                else {
                                    entry->direction = camera->viewYaw - 90.0f + irand(30) - 15.0f;
                                }
                                break;
                        }
#ifdef VERSION_G8ME01
                        if (!strcmp(gp->currentMapName, "aji_03")) {
                            entry->direction = irand(90);
                        }
#endif
                    }
                    else {
                        entry->direction = _move_dir;
                        _move_dir += 132;
                        if (_move_dir >= 360) {
                            _move_dir -= 360;
                        }
                        entry->speed = 0.67f * (irand(30) + 65);
                    }
                    break;
                case 4:
                    entry->flags2 |= 8;
                case 3:
                    if (!(entry->flags & 0x10)) {
                        entry->bounceFactor = 1.0f;
                        entry->gravityScale = 1.0f;
                        entry->maxBounces = 0;
                    }
                    entry->speed = 0.0f;
                    entry->direction = 0;
                    entry->velocityY = 0.0f;
                    break;
                case 10:
                    entry->bounceFactor = 1.0f;
                    entry->gravityScale = 1.0f;
                    entry->maxBounces = 0;
                    entry->velocityY = 500.0f;
                    entry->speed = 0.0f;
                    entry->direction = 0;
                    entry->flags2 |= 0x20;
                    entry->flags2 |= 4;
                    break;
            }
            break;
        case 1:
            entry->curTime = gp->renderTime;
            delta = entry->curTime - entry->baseTime;
            dt = OSTicksToMicroseconds(delta) / 1000000.0f;
            t1 = OSTicksToMicroseconds(entry->airTime) / 1000000.0f;
            t2 = OSTicksToMicroseconds(entry->airTime + delta) / 1000000.0f;
            dx = dt * (entry->speed * (f32)sin(6.2831855f * entry->direction / 360.0f));
            { f32 c = (f32)cos(6.2831855f * entry->direction / 360.0f); dz = dt * (entry->speed * -c); } // FAKE: cos result in a local
            if (entry->flags2 & 0x8000) {
                dz = 0.0f;
            }
            t2sq = t2 * t2;
            dy = entry->velocityY * dt + (-980.0f * entry->gravityScale) * (t2sq - t1 * t1);
            if (entry->flags2 & 4) {
                if (entry->bounceCount != 0 || dy < 0.0f) {
                    entry->flags2 &= ~4;
                }
            }
            if (entry->flags2 & 8 && entry->bounceCount != 0) {
                entry->flags2 &= ~8;
            }
            if (entry->flags2 & 0x20 && dy < 0.0f) {
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
            if (!(entry->flags2 & 0x20) && !(entry->flags & 0x800)) {
                {
                    // FAKE: itemHitCheckUp written out here (inlining it leaves the Y block below in the wrong stack order);
                    // h/r are the inline's result copies and keep the same register pressure. hit* declared in reverse for ascending slots.
                    HitEntry* h;
                    HitEntry* r;
                    f32 hitNZ, hitNY, hitNX, hitDist, hitZ, hitY, hitX;
                    f32 x, y, z;

                    if (dy <= 0.0f) {
                        r = NULL;
                    }
                    else {
                        x = entry->position.x;
                        y = 20.0f + entry->position.y;
                        z = entry->position.z;
                        { f64 d = __fabs(dy); hitDist = 20.0 + d; }
                        if (h = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, 1.0f, 0.0f)) {
                            entry->position.y = hitY - 20.0f - 4.0f;
                            r = h;
                        }
                        else {
                            x = entry->position.x;
                            y = entry->position.y;
                            z = entry->position.z;
                            { f64 d = __fabs(dy); hitDist = 20.0 + d; }
                            if (h = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, 1.0f, 0.0f)) {
                                entry->position.y = hitY - 20.0f;
                                r = h;
                            }
                            else {
                                r = NULL;
                            }
                        }
                    }
                    hit = r;
                }
                if (hit) {
                    entry->velocityY = -entry->velocityY / 4.0f; // FAKE: /4.0f (exact) gives the target register order
                    dy = 0.0f;
                    entry->airTime = 0;
                    entry->baseTime = entry->curTime;
                }
            }
            if (entry->flags2 & 0x100000 && entry->position.y <= -1000.0f) {
                itemModeChange(entry, 7);
                entry->flags2 &= ~4;
                entry->flags2 &= ~8;
                entry->flags2 &= ~0x4000;
                if (entry->itemId >= 1 && entry->itemId < 0x79) {
                    entry->timer = 0;
                }
                else if (entry->itemId >= 0xF0 && entry->itemId < 0x153) {
                    entry->timer = 0;
                }
                else {
                    entry->timer = 20000;
                }
                entry->flags2 |= 0x10000;
                entry->flags2 &= ~0x1000;
                entry->flags |= 0x100;
                break;
            }
            if (dy >= 0.0f) {
                hit = NULL;
                entry->position.y += dy;
            }
            // FAKE: no-op u32 cast so the earlier masked flags2 value is not reused (target reloads flags2 here)
            else if (*(u32*)&entry->flags2 & 0x100000) {
                hit = NULL;
                entry->position.y += dy;
            }
            else {
                // FAKE: itemHitCheckY written out here; the result goes through h so the caller's hit is set in each arm
                // (coalesces into r26 like the target). hit* declared in reverse for ascending stack slots.
                f32 hitNZ, hitNY, hitNX, hitDist, hitZ, hitY, hitX;
                HitEntry* h;
                f32 x = entry->position.x, y = 20.0f + entry->position.y, z = entry->position.z;
                { f64 d = __fabs(dy); hitDist = 20.0 + d; }
                h = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, -1.0f, 0.0f);
                if (h) {
                    entry->position.y = hitY;
                    hit = h;
                }
                else {
                    hit = NULL;
                    entry->position.y += dy;
                }
            }
            if (hit) {
                if (hitGetAttr(hit) & 0x600) {
                    entry->flags2 |= 0x100000;
                    {
                        Vec sndPos = (Vec){0.0f, 0.0f, 0.0f};
                        sndPos.x = entry->position.x;
                        sndPos.y = entry->position.y;
                        sndPos.z = entry->position.z;
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_FALL_WATER1), &sndPos);
                    }
                    effSplashEntry(0, entry->position.x, entry->position.y, entry->position.z, 0.7f);
                    entry->flags |= 2;
                }
                else {
                    entry->bounceCount++;
                    if (!(hitGetAttr(hit) & 0x80000000) && entry->maxBounces != 0 && entry->bounceCount >= entry->maxBounces) {
                        itemModeChange(entry, 1);
                        entry->flags2 &= ~4;
                        entry->flags2 &= ~8;
                        entry->flags2 &= ~0x4000;
                        break;
                    }
                    {
                    pos = (Vec){entry->position.x, entry->position.y, entry->position.z};
                    switch (itemId = entry->itemId) {
                        case 0x79:
                            psndSFXOn_3D("SFX_COIN_LEAPED1", &pos);
                            break;
                        case 0x7A:
                            psndSFXOn_3D("SFX_EVT_GAME_MONTE_LEAPED1", &pos);
                            break;
                        case 0x7B:
                            psndSFXOn_3D("SFX_HEART_LEAPED1", &pos);
                            break;
                        case 0x7C:
                            psndSFXOn_3D("SFX_FLOWER_LEAPED1", &pos);
                            break;
                        case 0x7D:
                            psndSFXOn_3D("SFX_STAR_PIECE_LEAPED1", &pos);
                            break;
                        case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: case 0x78:
                            psndSFXOn_3D("SFX_EVT_STARSTONE_JUMP1", &pos);
                            break;
                        default:
                            if (itemId >= 0xF0 && itemId < 0x153) {
                                psndSFXOn_3D("SFX_BADGE_LEAPED1", &pos);
                            }
                            else {
                                psndSFXOn_3D("SFX_ITEM_LEAPED1", &pos);
                            }
                            break;
                    }
                    }
                    isFloor = FALSE;
                    if (entry->flags & 0x80 && entry->velocityY != 0.0f) {
                        bounce = TRUE;
                    }
                    else if (hitGetAttr(hit) & 0x80000000 && entry->velocityY != 0.0f) {
                        mobj = mobjHitObjPtrToPtr(hit);
                        if (mario->characterId == CHARACTER_BOWSER) {
                            bounce = FALSE;
                        }
                        else if (mobj && strcmp(mobj->animName, "MOBJ_KururinFloor")) {
                            bounce = TRUE;
                            isFloor = TRUE;
                        }
                        else {
                            goto no_bounce;
                        }
                    }
                    else {
                    no_bounce:
                        bounce = FALSE;
                    }
                    if (!bounce) {
                        v = entry->velocityY * dt + (-980.0f * entry->gravityScale) * ((t2 + dt) * (t2 + dt) - t2sq);
                        entry->velocityY = 0.7f * (entry->bounceFactor * (-v / dt));
                    }
                    { f64 d = __fabs(0.5f * (entry->velocityY / (-980.0f * entry->gravityScale))); peak = d; } // FAKE: fabs result in an f64 local
                    height = entry->velocityY * peak + peak * ((-980.0f * entry->gravityScale) * peak);
                    if (!isFloor && height <= 8.0f) {
                        itemModeChange(entry, 1);
                        entry->flags2 &= ~4;
                        entry->flags2 &= ~8;
                        entry->flags2 &= ~0x4000;
                    }
                    if (entry->flags & 0x200 && entry->bounceCount <= 1) {
                        entry->direction = entry->savedDirection;
                        entry->speed = entry->savedSpeed;
                        entry->flags &= ~0x200;
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
    entry->flags2 &= ~0x10;
}

void itemSetPosition(ItemEntry* entry, f32 x, f32 y, f32 z) {
    entry->position.x = x;
    entry->position.y = y;
    entry->position.z = z;
}

void itemGetSound(ItemEntry* entry) {
    Vec pos;

    pos = (Vec){entry->position.x, entry->position.y, entry->position.z};
    switch (entry->itemId) {
        case 0x79:
            effStardustEntry(1, 3, 30, entry->position.x, 35.0f + entry->position.y, entry->position.z, 16.0f, 16.0f);
            psndSFXOn_3D("SFX_COIN_GET1", &pos);
            break;
        case 0x7A:
            effStardustEntry(1, 3, 30, entry->position.x, 35.0f + entry->position.y, entry->position.z, 16.0f, 16.0f);
            psndSFXOn_3D("SFX_EVT_GAME_MONTE_COUNT1", &pos);
            break;
        case 0x7B:
            psndSFXOn_3D("SFX_HEART_GET1", &pos);
            break;
        case 0x7C:
            psndSFXOn_3D("SFX_FLOWER_GET1", &pos);
            break;
        case 0x7D:
            break;
    }
}

HitEntry* itemHitCheckSide(ItemEntry* entry, f32* outX, f32 dx, f32* outZ, f32 dz, s32* angle) {
    HitEntry* hit;
    f32 a, b;
    f32 hitX, hitY, hitZ, hitDist, hitNX, hitNY, hitNZ;
    f32 sin, cos;
    f32 dot, ox, oz;
    f32 h; // FAKE: 15.0f in a local set before each hitDist store, so the y argument stays pos.y + h (a literal would be put on the left)

    *outX = entry->position.x;
    *outZ = entry->position.z;
    if (dx == 0.0f && dz == 0.0f) {
        return NULL;
    }
    sincosf(&sin, &cos, angleABf(0.0f, 0.0f, dx, dz));
    h = 15.0f;
    hitDist = 10.0f + sqrtf(dx * dx + dz * dz);
    hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ,
        entry->position.x, entry->position.y + h, entry->position.z, sin, 0.0f, cos);
    if (hit) {
        dot = dx * hitNX + dz * hitNZ;
        ox = 0.5f * (dx - dot * hitNX);
        oz = 0.5f * (dz - dot * hitNZ);
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
        entry->position.x, entry->position.y + h, entry->position.z, sin, 0.0f, cos);
    if (hit) {
        dot = dx * hitNX + dz * hitNZ;
        ox = 0.5f * (dx - dot * hitNX);
        oz = 0.5f * (dz - dot * hitNZ);
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

HitEntry*
itemHitCheckY(ItemEntry* entry, f32 z, f32 y, f32 x, f32 vy) {
    HitEntry* hit;
    f32 hitX, hitY, hitZ, hitDist, hitNX, hitNY, hitNZ;

    { f64 d = __fabs(vy); hitDist = 20.0 + d; } // FAKE: f64 local keeps 20.0 on the left of the fadd
    if (hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, -1.0f, 0.0f)) {
        entry->position.y = hitY;
    }
    else {
        hit = NULL;
        entry->position.y += vy;
    }
    return hit;
}

HitEntry*
itemHitCheckUp(ItemEntry* entry, f32 vy) {
    HitEntry* hit;
    f32 hitX, hitY, hitZ, hitDist, hitNX, hitNY, hitNZ;
    f32 x, y, z;

    if (vy <= 0.0f) {
        return NULL;
    }
    x = entry->position.x;
    y = 20.0f + entry->position.y;
    z = entry->position.z;
    { f64 d = __fabs(vy); hitDist = 20.0 + d; } // FAKE: f64 local keeps 20.0 on the left of the fadd
    if (hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, 1.0f, 0.0f)) {
        entry->position.y = hitY - 20.0f - 4.0f;
        return hit;
    }
    x = entry->position.x;
    y = entry->position.y;
    z = entry->position.z;
    { f64 d = __fabs(vy); hitDist = 20.0 + d; } // FAKE: f64 local keeps 20.0 on the left of the fadd
    if (hit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, 1.0f, 0.0f)) {
        entry->position.y = hitY - 20.0f;
        return hit;
    }
    return NULL;
}

void itemForceGet(ItemEntry* entry) {
    entry->timer = 0;
    itemModeChange(entry, 2);
    entry->flags |= 0x1000;
    entry->flags2 &= ~0x10000;
}

void itemNokoForceGet(ItemEntry* entry) {
    if (itemGetNokoCheck(entry)) {
        entry->timer = 0;
        itemModeChange(entry, 2);
        entry->flags |= 0x5000;
        entry->flags2 &= ~0x10000;
    }
    else {
        entry->timer = 0;
        itemModeChange(entry, 1);
        entry->flags2 &= ~4;
        entry->flags2 &= ~8;
        entry->flags2 &= ~0x4000;
        entry->flags2 &= ~0x1000;
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

void itemPickUp(ItemEntry* entry) {
    entry->flags2 |= 0x1000;
    itemModeChange(entry, 8);
    entry->timer = 0;
    entry->flags2 &= ~0x100;
}

// forward declarations: these functions are defined further down
void itemPickUp(ItemEntry* entry);
u32 itemStatus(ItemEntry* entry, u32 flags);
void itemStatusOn(ItemEntry* entry, u32 flags);
u16 itemFlag(ItemEntry* entry, u16 flags);
void itemFlagOff(ItemEntry* entry, u16 flags);
void itemFlagOn(ItemEntry* entry, u16 flags);
void itemNokoForceGet(ItemEntry* entry);
void itemForceGet(ItemEntry* entry);
HitEntry* itemHitCheckSide(ItemEntry* entry, f32* outX, f32 dx, f32* outZ, f32 dz, s32* angle);
void itemSetPosition(ItemEntry* entry, f32 x, f32 y, f32 z);
void itemseq_Bound(ItemEntry* entry);
void itemseq_GetItem(ItemEntry* entry);
void winFullDisp(WinMgrEntry* win);
void winHelpDisp(WinMgrEntry* win);
void winHelpMain(WinMgrEntry* win);
void winNameDisp2(WinMgrEntry* win);
void winNameDisp(WinMgrEntry* win);
ItemEntry* itemNearDistCheck(f32 x, f32 y, f32 z, f32 range);
ItemEntry* itemHitCheck(f32 x, f32 y, f32 z, f32 range);
ItemEntry* itemNameToPtr(char* name);
void itemDelete(const char* name);
ItemEntry* itemEntry(const char* name, s32 itemId, s32 type, s32 collectFlag, s32 evtCode, f32 x, f32 y, f32 z);
void itemModeChange(ItemEntry* entry, s32 mode);
void itemMain(void);
BOOL N_itemPickUpFromFieldCheck(void);
BOOL itemGetNokoCheck(ItemEntry* entry);
BOOL itemGetCheck(ItemEntry* entry);
void itemCoinDrop(Vec* position);
void itemReInit(void);
void itemInit(void);

